#!/usr/bin/env python3
"""Cross-platform build + package script for SuperVirtualKeyboard.

Configures and builds the CMake project, then zips the VST3 and Standalone
artefacts into dist/ with the version and platform baked into the filename.

Works on Windows, macOS, and Linux. Requires: python3, cmake, git, and the
juce_DIR environment variable pointing at your JUCE checkout (same as CMake).

Examples:
    python build.py                       # configure + build Release + zip
    python build.py --config Debug        # build Debug instead
    python build.py --no-build            # just re-zip existing artefacts
    python build.py --clean               # wipe the build dir first
"""
from __future__ import annotations

import argparse
import os
import platform
import re
import shutil
import subprocess
import sys
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent
PRODUCT = "SuperVirtualKeyboard"
ARTEFACTS = f"{PRODUCT}_artefacts"


def log(msg: str) -> None:
    print(f"[build] {msg}", flush=True)


def fail(msg: str) -> "NoReturn":
    print(f"[build] ERROR: {msg}", file=sys.stderr, flush=True)
    sys.exit(1)


def run(cmd: list[str], **kw) -> None:
    log(" ".join(str(c) for c in cmd))
    subprocess.run(cmd, check=True, **kw)


def platform_tag() -> str:
    return {"Windows": "windows", "Darwin": "macos", "Linux": "linux"}.get(
        platform.system(), platform.system().lower()
    )


def project_version() -> str:
    """MAJOR.MINOR.PATCH read from CMakeLists project(... VERSION ...). The
    git-commit-count rev used by cmake/GenerateVersion.cmake is intentionally
    left off so package names stay clean."""
    text = (ROOT / "CMakeLists.txt").read_text(encoding="utf-8")
    m = re.search(r"project\s*\([^)]*VERSION\s+(\d+\.\d+\.\d+)", text, re.IGNORECASE)
    return m.group(1) if m else "0.0.0"


def find_artefacts_dir(build_dir: Path, config: str) -> Path:
    """VS/Xcode (multi-config) nest artefacts under <config>/; Make/Ninja
    (single-config) put them directly under the artefacts dir."""
    base = build_dir / ARTEFACTS
    for candidate in (base / config, base):
        if (candidate / "VST3").exists() or (candidate / "Standalone").exists():
            return candidate
    fail(f"No artefacts found under {base}. Build first (drop --no-build).")


def collect_artefacts(art_dir: Path) -> list[Path]:
    """The plugin/app bundles to ship. .vst3 is a folder on macOS/Linux and a
    folder-shaped bundle on Windows; the standalone is .app / .exe / a binary."""
    items: list[Path] = []
    vst3 = art_dir / "VST3"
    if vst3.exists():
        items += sorted(vst3.glob("*.vst3"))
    standalone = art_dir / "Standalone"
    if standalone.exists():
        for pat in ("*.app", "*.exe", PRODUCT):
            items += sorted(standalone.glob(pat))
    # De-dupe while preserving order.
    seen, unique = set(), []
    for p in items:
        if p not in seen:
            seen.add(p)
            unique.append(p)
    return unique


def add_to_zip(zf: zipfile.ZipFile, path: Path) -> None:
    # Artefacts are written at the archive root (no wrapper folder): the zip's
    # own filename becomes the folder GUI extractors create, so the release
    # download is unzip -> folder -> plugins, not folder -> folder -> plugins.
    if path.is_dir():
        for f in sorted(path.rglob("*")):
            if f.is_file() or f.is_symlink():
                zf.write(f, f.relative_to(path.parent).as_posix())
    else:
        zf.write(path, path.name)


def make_zip(items: list[Path], out: Path) -> None:
    out.parent.mkdir(parents=True, exist_ok=True)
    if out.exists():
        out.unlink()
    with zipfile.ZipFile(out, "w", zipfile.ZIP_DEFLATED) as zf:
        for item in items:
            add_to_zip(zf, item)
    size_mb = out.stat().st_size / (1024 * 1024)
    log(f"wrote {out.relative_to(ROOT)} ({size_mb:.1f} MB)")


def main() -> None:
    ap = argparse.ArgumentParser(description="Build and package SuperVirtualKeyboard.")
    ap.add_argument("--config", default="Release",
                    help="CMake build config (default: Release)")
    ap.add_argument("--build-dir", default="build", help="CMake build dir (default: build)")
    ap.add_argument("--dist-dir", default="dist", help="Zip output dir (default: dist)")
    ap.add_argument("--generator", default=None, help="CMake generator override (-G)")
    ap.add_argument("--cmake-arg", action="append", default=[], metavar="ARG",
                    help="Extra arg passed to the CMake configure step "
                         "(repeatable, e.g. --cmake-arg=-DSVK_COPY_PLUGIN_AFTER_BUILD=OFF)")
    ap.add_argument("--jobs", "-j", type=int, default=None, metavar="N",
                    help="Max parallel build jobs (default: the build tool's max). "
                         "Lower this if the build is killed by the OOM killer — "
                         "unbounded parallelism can exhaust RAM on CI runners.")
    ap.add_argument("--no-build", action="store_true", help="Skip configure/build; just zip")
    ap.add_argument("--clean", action="store_true", help="Delete the build dir first")
    args = ap.parse_args()

    build_dir = (ROOT / args.build_dir).resolve()
    version = project_version()
    log(f"{PRODUCT} {version} ({platform_tag()}, {args.config})")

    if "juce_DIR" not in os.environ and not (build_dir / "CMakeCache.txt").exists():
        fail("juce_DIR is not set and no existing CMake cache found. "
             "Set juce_DIR to your JUCE checkout (see CMakeLists.txt).")

    if args.clean and build_dir.exists():
        log(f"cleaning {build_dir}")
        shutil.rmtree(build_dir)

    if not args.no_build:
        if not shutil.which("cmake"):
            fail("cmake not found on PATH.")
        configure = ["cmake", "-S", str(ROOT), "-B", str(build_dir)]
        if args.generator:
            configure += ["-G", args.generator]
        # Single-config generators need the build type at configure time.
        configure += [f"-DCMAKE_BUILD_TYPE={args.config}"]
        configure += args.cmake_arg
        run(configure)
        build = ["cmake", "--build", str(build_dir),
                 "--config", args.config, "--parallel"]
        # A bare --parallel means "all cores"; on Make generators that is an
        # unbounded `make -j`, which can OOM-kill memory-heavy JUCE builds.
        if args.jobs is not None:
            build.append(str(args.jobs))
        run(build)

    art_dir = find_artefacts_dir(build_dir, args.config)
    items = collect_artefacts(art_dir)
    if not items:
        fail(f"No VST3/Standalone artefacts found in {art_dir}.")
    log("packaging: " + ", ".join(p.name for p in items))

    out = (ROOT / args.dist_dir).resolve() / f"{PRODUCT}-{version}-{platform_tag()}.zip"
    make_zip(items, out)
    log("done.")


if __name__ == "__main__":
    main()
