#!/usr/bin/env python3
"""Set SuperVirtualKeyboard's release version.

Updates the two places the version lives:
  * CMakeLists.txt  project(SUPERVIRTUALKEYBOARD VERSION x.y.z) -- the source of
    truth for the plugin, Version.h and build.py's zip names. (The 4th "rev"
    number is the git commit count and is added automatically at build time.)
  * README.md       the "LATEST VERSION: <label> <date> (vx.y.z)" line, with
    today's date. The label (e.g. "Alpha") is kept unless --label is given.

Optionally commits those two files and creates a local tag. It never pushes:
pushing a v* tag triggers the GitHub release workflow, so do that yourself.

Examples:
    python set_version.py                  # show the current version
    python set_version.py patch            # 0.4.0 -> 0.4.1
    python set_version.py minor            # 0.4.0 -> 0.5.0
    python set_version.py 1.0.0 --label Beta
    python set_version.py minor --dry-run  # show what would change
    python set_version.py minor --commit   # also commit the bump
    python set_version.py minor --tag      # commit + tag v0.5.0
    python set_version.py minor --tag v0.5.0-alpha
"""
from __future__ import annotations

import argparse
import datetime
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
CMAKE = ROOT / "CMakeLists.txt"
README = ROOT / "README.md"

CMAKE_RE = re.compile(r"(project\s*\(\s*SUPERVIRTUALKEYBOARD\s+VERSION\s+)(\d+)\.(\d+)\.(\d+)", re.IGNORECASE)
README_RE = re.compile(r"^(LATEST VERSION:[ \t]*)(.*?)[ \t]*(\d{4}-\d{2}-\d{2})[ \t]*\(v[\d.]+\)[ \t]*$", re.MULTILINE)


def log(msg: str) -> None:
    print(f"[version] {msg}", flush=True)


def fail(msg: str) -> "NoReturn":
    print(f"[version] ERROR: {msg}", file=sys.stderr, flush=True)
    sys.exit(1)


def git(*args: str, check: bool = True) -> str:
    result = subprocess.run(["git", *args], cwd=ROOT, capture_output=True, text=True)
    if check and result.returncode != 0:
        fail(f"git {' '.join(args)} failed:\n{result.stderr.strip()}")
    return result.stdout.strip()


def current_version(cmake_text: str) -> tuple[int, int, int]:
    m = CMAKE_RE.search(cmake_text)
    if not m:
        fail(f"Couldn't find project(SUPERVIRTUALKEYBOARD VERSION x.y.z) in {CMAKE.name}.")
    return int(m.group(2)), int(m.group(3)), int(m.group(4))


def resolve_target(spec: str, cur: tuple[int, int, int]) -> tuple[int, int, int]:
    major, minor, patch = cur
    if spec == "major":
        return major + 1, 0, 0
    if spec == "minor":
        return major, minor + 1, 0
    if spec == "patch":
        return major, minor, patch + 1
    m = re.fullmatch(r"v?(\d+)\.(\d+)\.(\d+)", spec)
    if not m:
        fail(f"'{spec}' isn't major/minor/patch or a version like 1.2.3 "
             "(CMake versions can't have suffixes; use --tag NAME for e.g. v1.2.3-alpha).")
    return int(m.group(1)), int(m.group(2)), int(m.group(3))


def main() -> None:
    ap = argparse.ArgumentParser(description="Set the release version (see module docstring).")
    ap.add_argument("version", nargs="?",
                    help="major | minor | patch | X.Y.Z  (omit to print the current version)")
    ap.add_argument("--label", help="README release label, e.g. Alpha or Beta (default: keep current)")
    ap.add_argument("--dry-run", action="store_true", help="Show the changes without writing them")
    ap.add_argument("--commit", action="store_true", help="Commit CMakeLists.txt and README.md")
    ap.add_argument("--tag", nargs="?", const="", metavar="NAME",
                    help="Commit, then create a local tag (default name: vX.Y.Z). Never pushed.")
    args = ap.parse_args()

    cmake_text = CMAKE.read_text(encoding="utf-8")
    readme_text = README.read_text(encoding="utf-8")
    cur = current_version(cmake_text)
    cur_str = ".".join(map(str, cur))

    if args.version is None:
        print(cur_str)
        return

    new = resolve_target(args.version, cur)
    new_str = ".".join(map(str, new))
    if new == cur:
        fail(f"Version is already {cur_str}.")
    if new < cur:
        log(f"warning: {new_str} is lower than the current {cur_str}.")

    do_tag = args.tag is not None
    tag_name = (args.tag or f"v{new_str}") if do_tag else None
    do_commit = args.commit or do_tag  # a tag must point at the bump commit

    if do_commit:
        # Refuse if these files already have uncommitted edits: they'd be swept
        # into the version commit.
        dirty = git("status", "--porcelain", "--", CMAKE.name, README.name)
        if dirty:
            fail("CMakeLists.txt or README.md has uncommitted changes; commit or stash "
                 "them first (or run without --commit/--tag):\n" + dirty)
        if do_tag and git("rev-parse", "-q", "--verify", f"refs/tags/{tag_name}", check=False):
            fail(f"Tag {tag_name} already exists.")

    new_cmake = CMAKE_RE.sub(lambda m: f"{m.group(1)}{new_str}", cmake_text, count=1)

    today = datetime.date.today().isoformat()
    readme_m = README_RE.search(readme_text)
    if readme_m:
        label = args.label if args.label is not None else readme_m.group(2)
        line = f"{readme_m.group(1)}{label + ' ' if label else ''}{today} (v{new_str})"
        new_readme = readme_text[:readme_m.start()] + line + readme_text[readme_m.end():]
    else:
        log("warning: no 'LATEST VERSION: ... (vX.Y.Z)' line found in README.md; leaving it alone.")
        new_readme = readme_text

    log(f"{cur_str} -> {new_str}")
    log(f"  {CMAKE.name}: project(SUPERVIRTUALKEYBOARD VERSION {new_str})")
    if readme_m:
        log(f"  {README.name}: {line}")

    if args.dry_run:
        extra = f", commit, tag {tag_name}" if do_tag else (", commit" if do_commit else "")
        log(f"dry run: nothing written{' (would also' + extra[1:] + ')' if extra else ''}.")
        return

    CMAKE.write_text(new_cmake, encoding="utf-8")
    if new_readme != readme_text:
        README.write_text(new_readme, encoding="utf-8")

    if do_commit:
        git("add", "--", CMAKE.name, README.name)
        git("commit", "-m", f"Bump version to {new_str}", "--", CMAKE.name, README.name)
        log(f"committed: Bump version to {new_str}")
    if do_tag:
        git("tag", "-a", tag_name, "-m", f"SuperVirtualKeyboard {new_str}")
        log(f"tagged {tag_name} (local only)")
        log(f"to publish the release: git push && git push origin {tag_name}")
    elif not do_commit:
        log("files updated; review the diff, then commit.")


if __name__ == "__main__":
    main()
