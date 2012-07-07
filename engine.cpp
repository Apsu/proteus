#include "engine.h"

Server server;

void handler(int sig)
  {
  switch(sig)
    {
    case SIGINT:
      server.Stop();
      break;
    };
  }

int main(int argc, char **argv)
  {
  if(argc != 3)
    {
    cout << "Usage: engine <ip> <port>\n";
    return false;
    }

  signal(SIGINT, handler);

  try
    { server.Start(argv[1], strtoul(argv[2], 0, 10), thread); }
  catch(const char *except)
    { cerr << "Engine: " << except << endl; }

  pthread_exit(0);
  }
