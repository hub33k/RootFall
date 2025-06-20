# RootFall

## Requirements

- [uv](https://github.com/astral-sh/uv)
  - `uv python install 3.13`
  - `uv sync`
- [git](https://git-scm.com/)
- [cmake](https://cmake.org/)
- [clang](https://clang.llvm.org/)
  - [donwload clang](https://releases.llvm.org/)
- [ninja](https://ninja-build.org/)

## Quick start

```sh
git clone https://github.com/hub33k/RootFall.git
cd RootFall

uv run ./setup.py # check if all requirements are installed

cmake --list-presets
cmake --preset windows-debug # adjust

cmake --build ./build-debug --target help # list all targets
cmake --build ./build-debug --target RootFall
```

## Docs

- [scripts](docs/scripts.md)
- [structure](docs/structure.md)

## Based on

- CMake config based on [cmake-init](https://github.com/cginternals/cmake-init) by cginternals
