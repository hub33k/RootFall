#include "RootFall/Core/Application.hpp"

int main(const int argc, char *argv[]) {
  const hub33k::ApplicationCommandLineArgs args{argc, argv};
  const hub33k::ApplicationSpecification spec{
    .Name = "RootFall",
    .WorkingDirectory = ".",
    .CommandLineArgs = args,
  };

  const auto app = new hub33k::Application(spec);
  app->Run();
  delete app;

  return 0;
}
