#include <cstdio> // for printf()

import M;

using namespace __CHRIS_MONOREPO__CPP__IPC__SHARED;

int main() {
  fprintf(stderr, "Starting IPC command client...\n");
  FILE *ipcHandle = ipcOpenRead();
  printf("About to read from FIFO...\n");
  constexpr int SIZE = 256;
  char buffer[SIZE];
  char *ptr = fgets(buffer, SIZE, ipcHandle);
  if (ptr == nullptr) {
    fprintf(stderr, "Error reading from ipcHandle!\n");
    return 1;
  }
  printf("Read from ipcHandle: \"%s\"\n", buffer);
  return 0;
}
