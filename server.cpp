#include "server.h"

void Server::Start(char *addr, unsigned short int port, void *(*thread)(void *))
  {
  Socket *nsck;

  sock.Listen(addr, port);

  while(sock.Active())
    {
    if(nsck = sock.Accept())
      { pool.Spawn(thread, (void*)nsck); }
    }

  sock.Close();
  }

void CloseSock(void *s)
  { ((Socket*)s)->Close(); }

void Server::Stop()
  {
  sock.Close();
  pool.Empty(CloseSock);
  }

Server::~Server()
  { sock.Close(); }
