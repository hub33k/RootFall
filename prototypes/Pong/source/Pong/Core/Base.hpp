#pragma once

#define ROOT_DIR (PROJECT_ROOT_DIR + std::string("prototypes/Pong/"))
#define DATA_DIR (ROOT_DIR + std::string("data/"))

#if HK_BUILD_DEBUG
  #define ASSETS_DIR DATA_DIR
#else
  #define ASSETS_DIR "data/"
#endif

#define TEXTURES_DIR(fileName) (std::string(ASSETS_DIR) + "textures/" + (fileName))
#define SHADERS_DIR(fileName) (std::string(ASSETS_DIR) + "shaders/" + (fileName))
#define FONTS_DIR(fileName) (std::string(ASSETS_DIR) + "fonts/" + (fileName))
#define SOUNDS_DIR(fileName) (std::string(ASSETS_DIR) + "sounds/" + (fileName))
