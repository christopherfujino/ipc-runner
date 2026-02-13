import M;

using namespace __CHRIS_MONOREPO__CPP__IPC__SHARED;

int main() {
  IPC::WriteChannel channel = IPC::WriteChannel();
  channel.write(Command::BUILD);
  return 0;
}
