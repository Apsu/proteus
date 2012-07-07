#ifndef __COMM_H__
#define __COMM_H__

#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Socket
  {
private:
  int  sock;
  bool conn;
  char ip[17];

  void Init();

public:
  bool GetHost(char*);

  Socket *Accept();
  void Listen (char*, unsigned short int);
  void Connect(char*, unsigned short int);

  int  RecvTok(char *, char, int);
  int  Recv(char*, int);
  void Send(char*, int);

  void ReuseAddr(bool);
  bool Active();
  void Close ();

  const char *IP();

  Socket(int);
  Socket();
 ~Socket();
  };

#endif //__COMM_H__

