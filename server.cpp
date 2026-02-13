#include <cstdio> // for printf()
#include <cstring> // for strcmp()
#include <ctime>
#include <format>
#include <stdexcept>  // for std::runtime_error()
#include <sys/wait.h> // for waitpid()
#include <unistd.h>   // execvp(), fork()
#include <vector>

import M;

using namespace __CHRIS_MONOREPO__CPP__IPC__SHARED;

void _exec(std::vector<std::string> cmd) {
  std::time_t t = std::time(nullptr);
  auto time_obj = std::localtime(&t);
  printf("[%02d:%02d:%02d]", time_obj->tm_hour, time_obj->tm_min,
         time_obj->tm_sec);
  for (auto &s : cmd) {
    printf(" %s", s.c_str());
  }
  printf("\n");
  int pid = fork();

  if (pid < 0) {
    throw std::runtime_error("Failure to fork");
  } else if (pid == 0) {
    // It's fine we're allocating from heap, since we're about to exec
    const char **v = new const char *[cmd.size() + 2];
    size_t i = 0;
    for (; i < cmd.size(); i++) {
      v[i] = cmd[i].c_str();
    }
    v[i] = NULL;
    execvp(cmd[0].c_str(), (char **)v);
  } else {
    int status = 0;
    int options = 0;
    waitpid(pid, &status, options);
    // TODO check status
    printf("\n");
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

  if (targetCommand.size() == 0) {
    fprintf(stderr, "Usage: ipcr-server -- [COMMAND] [ARGS]...");
  }

  printf("Starting ipc-runner-server...\n\n");
  IPC::create();
  IPC::ReadChannel ipc = IPC::ReadChannel();

  while (1) {
    auto cmd = ipc.read();
    switch (cmd) {
    case Command::BUILD:
      _exec(targetCommand);
      break;
    default:
      throw std::runtime_error(
          std::format("Unreachable: {} '{}'", (int)cmd, (char)cmd));
    }
  }

  return 0;
}
