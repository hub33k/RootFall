#include "RootFall/Game/Application.hpp"

int main(const int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  hub33k::Log::Init();

  auto app = new hub33k::Application();
  app->Run();
  delete app;

  return 0;
}
