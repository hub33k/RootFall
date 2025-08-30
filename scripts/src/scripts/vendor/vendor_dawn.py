#!/usr/bin/env python3

"""
https://dawn.googlesource.com/dawn
https://dawn.googlesource.com/dawn/+/HEAD/docs/quickstart-cmake.md

https://github.com/roche-emmanuel/nervproj/blob/931d511ff1429d3aafcf4745f449752258a012cc/nvp/builders/dawn.py
https://github.com/mmozeiko/build-dawn

https://eliemichel.github.io/LearnWebGPU/getting-started/hello-webgpu.html
https://github.com/eliemichel/WebGPU-distribution
https://github.com/eliemichel/wgpu-native
https://github.com/eliemichel/WebGPU-Cpp
"""

import platform
import sys
from pathlib import Path

from scripts.vendor.vendor_config import DIR_BUILD_VENDOR
from scripts.utils import exec

# from bin.deps.config_deps import exec, is_macos, is_windows # TODO (hub33k): add is_macos, is_windows utils

BUILD_TYPE = "Debug"
CC = "clang"
C_STANDARD = "99"
CXX = "clang++"
CXX_STANDARD = "20"

DIR_SRC = Path(DIR_BUILD_VENDOR / "dawn")
DIR_CODE = Path(DIR_SRC / "code")
DIR_BUILD = Path(DIR_SRC, "build")
DIR_INSTALL = Path(DIR_SRC, "install")

GIT_SOURCE_URL = "https://dawn.googlesource.com/dawn"
GIT_HASH = "d33f9062cd0da6faf2aca3235a21658bde241b84"  # Move VkCommandPool recycling to a SerialTask
# https://dawn.googlesource.com/dawn/+/d33f9062cd0da6faf2aca3235a21658bde241b84

# remove PROTOBUF_CONSTINIT from third_party/protobuf/src/google/protobuf/port.cc - line 104
# TODO (hub33k): set build/release folders


def get_code():
    if Path.exists(DIR_CODE):
        return

    exec(f'git clone {GIT_SOURCE_URL} "{DIR_CODE}"')
    exec(f"git checkout {GIT_HASH}", cwd=DIR_CODE)


def configure():
    common_cmake_options = (
        f' -S "{DIR_CODE}" -B "{DIR_BUILD}"'
        " -G Ninja"  # Not working on windows
        #
        f" -DCMAKE_BUILD_TYPE={BUILD_TYPE}"
        # Library specific options
        " -DDAWN_FETCH_DEPENDENCIES=ON"
        " -DDAWN_ENABLE_INSTALL=ON"
        " -DDAWN_BUILD_MONOLITHIC_LIBRARY=SHARED"
        " -DDAWN_BUILD_SAMPLES=OFF"
        " -DTINT_BUILD_TESTS=OFF"
        " -DTINT_BUILD_FUZZERS=OFF"
        " -DTINT_BUILD_BENCHMARKS=OFF"
        " -DTINT_BUILD_AS_OTHER_OS=OFF"
    )

    if platform.system() == "Windows":
        command = (
            "cmake "
            f" -DCMAKE_C_COMPILER={CC}"
            f" -DCMAKE_C_STANDARD={C_STANDARD}"
            " -DCMAKE_C_STANDARD_REQUIRED=ON"
            f" -DCMAKE_CXX_COMPILER={CXX}"
            f" -DCMAKE_CXX_STANDARD={CXX_STANDARD}"
            " -DCMAKE_CXX_STANDARD_REQUIRED=ON"
            # Extra
            ' -DCMAKE_CXX_FLAGS="-DNOMINMAX"'
            #
            f"{common_cmake_options}"
        )
    elif platform.system() == "Darwin":
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
    # TODO (hub33k): handle key interrupt, try/catch
    # TODO (hub33k): add typer (cli options - configure, build, install, etc.)

    # Prepare dirs
    if not Path.exists(DIR_SRC):
        Path.mkdir(DIR_SRC, parents=True)
    if not Path.exists(DIR_INSTALL):
        Path.mkdir(DIR_INSTALL, parents=True)

    get_code()

    configure()
    build()
    install()
