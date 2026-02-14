#include <cstdio>  // for printf()
#include <cstring> // for strcmp()
#include <ctime>
#include <format>
#include <stdexcept>  // for std::runtime_error()
#include <sys/wait.h> // for waitpid()
#include <unistd.h>   // execvp(), fork()
#include <vector>

import M;

using namespace __CHRIS_MONOREPO__CPP__IPC__SHARED;

void _exec(std::string cmd) {
  const char *cmd_s = cmd.c_str();
  std::time_t t = std::time(nullptr);
  auto time_obj = std::localtime(&t);
  printf("[%02d:%02d:%02d] %s\n", time_obj->tm_hour, time_obj->tm_min,
         time_obj->tm_sec, cmd_s);

  int result = system(cmd_s);
  if (result != 0) {
    fprintf(stderr, "\nCommand failed with %d\n\n", result);
  } else {
    printf("\nSuccess!\n\n");
  }
}

int main(int argc, char **argv) {
  std::vector<std::string> targetCommand = {};

  bool parseCommand = false;
  for (int i = 1; i < argc; i++) {
    if (parseCommand) {
      targetCommand.push_back(argv[i]);
    } else if (strncmp(argv[i], "--", 3) == 0) {
      parseCommand = true;
    }
  }

  if (targetCommand.size() != 1) {
    if (targetCommand.size() == 0) {
      fprintf(stderr, "Usage: ipc-runner-server -- [COMMAND_STRING]\n");
    } else {
      fprintf(stderr, "Usage: ipc-runner-server -- [COMMAND_STRING]\n\n");
      fprintf(stderr, "Your desired command and its arguments should be a "
                      "single string.\n");
      // TODO construct the string programmatically
    }
    return 1;
  }

  printf("Starting ipc-runner-server...\n\n");

  IPC::create();
  IPC::ReadChannel ipc = IPC::ReadChannel();

  while (1) {
    auto cmd = ipc.read();
    switch (cmd) {
    case Command::BUILD:
      _exec(targetCommand[0]);
      break;
    default:
      throw std::runtime_error(
          std::format("Unreachable: {} '{}'", (int)cmd, (char)cmd));
    }
  }

  return 0;
}
