# Generates Version.h from cmake/Version.h.in, filling in the git commit count
# as the automatic rev. Run both at configure time and (via a custom target) at
# build time, so the rev stays current without a manual reconfigure.
#
# Expected -D arguments: VERSION_MAJOR VERSION_MINOR VERSION_PATCH SRC DST SOURCE_DIR

find_package(Git QUIET)

set(SVK_VERSION_REV "0")
if(GIT_EXECUTABLE)
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" rev-list --count HEAD
        WORKING_DIRECTORY "${SOURCE_DIR}"
        OUTPUT_VARIABLE SVK_VERSION_REV
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    if(NOT SVK_VERSION_REV MATCHES "^[0-9]+$")
        set(SVK_VERSION_REV "0")  # not a git checkout, or no commits yet
    endif()
endif()

# When invoked with SRC/DST defined (build-time `cmake -P` step), write the
# header. When merely include()'d from CMakeLists at configure time, SVK_VERSION_REV
# is all the caller needs, so skip file generation.
if(DEFINED SRC AND DEFINED DST)
    # Write to a temp file first, then copy only if changed, to avoid forcing a
    # recompile of every file that includes Version.h on each build.
    configure_file("${SRC}" "${DST}.tmp" @ONLY)
    execute_process(COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${DST}.tmp" "${DST}")
    file(REMOVE "${DST}.tmp")
endif()
