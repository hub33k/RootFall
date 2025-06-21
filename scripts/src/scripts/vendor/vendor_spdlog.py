import sys
from pathlib import Path

from scripts.utils import exec, is_windows, is_macos
from scripts.vendor.vendor_config import DIR_BUILD_VENDOR

"""
https://github.com/gabime/spdlog
"""

BUILD_TYPE = "Release"
CC = "clang"
C_STANDARD = "99"
CXX = "clang++"
CXX_STANDARD = "17"

DIR_SRC = Path(DIR_BUILD_VENDOR / "spdlog")
DIR_CODE = Path(DIR_SRC / "code")
DIR_BUILD = Path(DIR_SRC, "build")
DIR_INSTALL = Path(DIR_SRC, "install")

GIT_SOURCE_URL = "https://github.com/gabime/spdlog"
GIT_HASH = "3335c38"  # Update README.md (#3338)


def get_code():
    if Path.exists(DIR_CODE):
        return

    exec(f'git clone {GIT_SOURCE_URL} "{DIR_CODE}"')
    exec(f"git checkout {GIT_HASH}", cwd=DIR_CODE)


def configure():
    common_cmake_options = (
        f' -S "{DIR_CODE}" -B "{DIR_BUILD}"'
        " -G Ninja"
        #
        f" -DCMAKE_CXX_COMPILER={CXX}"
        f" -DCMAKE_CXX_STANDARD={CXX_STANDARD}"
        " -DCMAKE_CXX_STANDARD_REQUIRED=ON"
        f" -DCMAKE_BUILD_TYPE={BUILD_TYPE}"
        # Library specific options
    )

    if is_windows:
        command = (
            "cmake"
            #
            f"{common_cmake_options}"
        )
    elif is_macos:
        command = (
            "cmake"
            #
            f"{common_cmake_options}"
        )
    else:
        sys.exit("Only Windows and MacOS are supported")
    exec(command)


def build():
    command = (
        "cmake"
        #
        f' --build "{DIR_BUILD}"'
        f" --config {BUILD_TYPE}"
    )
    exec(command)


def install():
    command = (
        "cmake"
        #
        f' --install "{DIR_BUILD}"'
        f' --prefix "{DIR_INSTALL}"'
    )
    exec(command)


def main():
    # Prepare dirs
    if not Path.exists(DIR_SRC):
        Path.mkdir(DIR_SRC, parents=True)
    if not Path.exists(DIR_INSTALL):
        Path.mkdir(DIR_INSTALL, parents=True)

    get_code()

    configure()
    build()
    install()


if __name__ == "__main__":
    main()
