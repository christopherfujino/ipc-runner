module;

#include <cstdio>  // for fopen()
#include <cstdlib> // for exit()
#include <fcntl.h> // for O_RDONLY

export module M;

namespace __CHRIS_MONOREPO__CPP__IPC__SHARED {

export FILE *ipcOpenRead() {
  constexpr const char *path = "/home/chris/git/ipc/fifo";
  FILE *f = fopen(path, "r");
  if (f == nullptr) {
    fprintf(stderr, "Failure to open \"%s\" for reading\n", path);
    exit(1);
  }
  return f;
}

} // namespace __CHRIS_MONOREPO__CPP__IPC__SHARED
