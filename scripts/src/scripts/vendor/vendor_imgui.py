from pathlib import Path

from scripts.utils import exec
from scripts.vendor.vendor_config import DIR_BUILD_VENDOR

"""
https://github.com/ocornut/imgui/tree/docking
"""

BUILD_TYPE = "Debug"
CC = "clang"
C_STANDARD = "99"
CXX = "clang++"
CXX_STANDARD = "17"

DIR_SRC = Path(DIR_BUILD_VENDOR / "imgui-tmp")
DIR_CODE = Path(DIR_SRC / "code")
DIR_BUILD = Path(DIR_SRC, "build")
DIR_INSTALL = Path(DIR_SRC, "install")

GIT_SOURCE_URL = "https://github.com/ocornut/imgui"
GIT_HASH = "9fdeb41"  # Backends: Vulkan: Suppress trivially-copyable warning (#8537)


def get_code():
    if Path.exists(DIR_CODE):
        return

    exec(f'git clone {GIT_SOURCE_URL} "{DIR_CODE}"')
    exec("git chechout docking")
    exec(f"git checkout {GIT_HASH}", cwd=DIR_CODE)


def main():
    # Prepare dirs
    if not Path.exists(DIR_SRC):
        Path.mkdir(DIR_SRC, parents=True)
    if not Path.exists(DIR_INSTALL):
        Path.mkdir(DIR_INSTALL, parents=True)

    get_code()


if __name__ == "__main__":
    main()
