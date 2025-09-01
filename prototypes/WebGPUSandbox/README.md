# WebGPUSandbox

## Emscripten

- `emsdk install latest`
- `emsdk activate latest`
- `emcmake cmake --version`
- `emcmake cmake --preset debug-llvm-web -G Ninja -B build-web`
- `cmake --build ./build-debug-llvm-web --target WebGPUSandbox`

---

- configure: cmake --preset debug-llvm-web -G Ninja -B build-web '-DCMAKE_TOOLCHAIN_FILE=E:\dev\hvm\languages\emscripten\emsdk\upstream\emscripten\cmake\Modules\Platform\Emscripten.cmake' -DCMAKE_CROSSCOMPILING_EMULATOR=E:/dev/hvm/languages/emscripten/emsdk/node/22.16.0_64bit/bin/node.exe
