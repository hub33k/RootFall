# Structure

- `.files` - Ignored files per OS (`.vscode`, scripts, `CMakeUserPresets.json`, etc.)
- `.venv` - Python virtual environment created by `uv`
- `.vscode` - Visual Studio Code settings
- `cmake` - CMake modules/utilities
- `data` - Game assets, runtime data
- `docs` - Documentation (Markdown, Doxygen, etc.)
- `scripts` - Python build/run scripts, utils, CI helpers
- `source` - Game source, executables and libraries
- `tools` - Dev tools, scripts, asset converters, etc.
- `vendor` - External libraries (added via FetchContent or submodules)

# RootFall

- RootFall - main executable
- RootFallCore
- RootFallExtern
- RootFallGame
- RootFallPlatform
- RootFallRenderer
- RootFallTests
- RootFallUtility
