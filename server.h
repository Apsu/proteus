#ifndef __SERVER_H__
#define __SERVER_H__

#include "comm.h"
#include "threads.h"

class Server
  {
private:
  Socket  sock;
  Threads pool;

public:
  void Start(char *, unsigned short int, void *(*)(void *));
  void Stop();

 ~Server();
  };

#endif //__SERVER_H__

