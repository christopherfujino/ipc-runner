# IPC Command Runner

Run commands from another process.

## Usage

```bash
$ ipc-runner-server -- 'cmake --build build && ./build/main'
Starting ipc-runner-server...

# From another terminal...
# ipc-runner-client

# Back in our initial (server) terminal...
[23:44:29] cmake --build build && ./build/main
ninja: no work to do.
Hello, world!

Success!
```
