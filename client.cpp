#include <cstdio> // for printf()

import M;

using namespace __CHRIS_MONOREPO__CPP__IPC__SHARED;

int main() {
  fprintf(stderr, "Starting IPC command client...\n");
  IPC::WriteChannel channel = IPC::WriteChannel();
  channel.write(Command::BUILD);
  return 0;
}
