module;

#include <cerrno>  // for errno
#include <cstdio>  // for fopen()
#include <cstdlib> // for exit(), getenv()
#include <cstring> // for strerror()
#include <format>
#include <stdexcept>  // for std::runtime_error()
#include <sys/stat.h> // for mkfifo()
#include <unistd.h>

export module M;

namespace __CHRIS_MONOREPO__CPP__IPC__SHARED {

export enum class Command : unsigned char {
  BUILD,
  // TODO GET_VERSION,
};

const char *getPath() {
  static const char *cachedPath = nullptr;

  if (cachedPath != nullptr) {
    return cachedPath;
  }

  const char *homePath = getenv("HOME");
  if (homePath == nullptr) {
    fprintf(stderr, "TODO: figure out a place to keep the named FIFO if user "
                    "does not have $HOME set in env\n");
    exit(1);
  }
  cachedPath = "/tmp/chris.fifo";

  return cachedPath;
}

namespace IPC {

export class WriteChannel {
public:
  WriteChannel() {
    _ptr = fopen(getPath(), "w");
    if (_ptr == nullptr) {
      fprintf(stderr, "Failure to open \"%s\" for writing\n", getPath());
      exit(1);
    }
  }

  void write(Command cmd) {
    int result = fputc((int)cmd, _ptr);
    if (result == EOF) {
      throw std::runtime_error("Failed to write to IPC channel");
    }
  }

private:
  FILE *_ptr = nullptr;
};

export class ReadChannel {
public:
  ReadChannel() {
    // If this does not include write, the first client will close the pipe
    // when it's done using it.
    _ptr = fopen(getPath(), "r+");
    if (_ptr == nullptr) {
      fprintf(stderr, "Failure to open \"%s\" for reading\n", getPath());
      exit(1);
    }
  }

  ~ReadChannel() {
    int result = unlink(getPath());
    if (result < 0) {
      fprintf(stderr, "Warning: failed to delete %s", getPath());
    }
  }

  Command read() {
    int b = fgetc(_ptr);
    if (b == EOF) {
      if (ferror(_ptr) != 0) {
        throw std::runtime_error("Error reading from IPC channel!");
      }
      // else EOF
      throw std::runtime_error("TODO figure out what to do on EOF");
    }

    return (Command)b;
  }

private:
  FILE *_ptr = nullptr;
};

export void create() {
  const char *path = getPath();
  int result = unlink(path);
  if (result < 0) {
    throw std::runtime_error(std::format(
        "FIFO already exists at `%s` and it could not be unlinked: %s", path,
        strerror(errno)));
  }
  int success = mkfifo(path, 0600);
  if (success != 0) {
    fprintf(stderr, "Failure to create IPC FIFO at `%s`\n", getPath());
    exit(1);
  }
}

} // namespace IPC

} // namespace __CHRIS_MONOREPO__CPP__IPC__SHARED
