#include <cstdio> // for printf()
#include <format>
#include <stdexcept> // for std::runtime_error()
#include <sys/wait.h>
#include <unistd.h> // execvp(), fork()
#include <vector>

import M;

using namespace __CHRIS_MONOREPO__CPP__IPC__SHARED;

void _exec(std::vector<std::string> cmd) {
  int pid = fork();

  if (pid < 0) {
    throw std::runtime_error("Failure to fork");
  } else if (pid == 0) {
    printf("About to exec...\n");
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
    printf("back\n");
  }
}

int main() {
  printf("Starting IPC command server...\n");
  IPC::create();
  IPC::ReadChannel ipc = IPC::ReadChannel();

  while (1) {
    fprintf(stderr, "Reading next command...\n");
    fflush(stdout);
    fflush(stderr);
    auto cmd = ipc.read();
    switch (cmd) {
    case Command::BUILD:
      printf("Received %d\n", (int)cmd);
      //_exec({"cmake", "-h"});
      break;
    default:
      throw std::runtime_error(
          std::format("Unreachable: {} '{}'", (int)cmd, (char)cmd));
    }
  }

  return 0;
}
