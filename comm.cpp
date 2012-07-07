#include "comm.h"

void Socket::Listen(char *addr, unsigned short int port)
  {
  sockaddr_in sa;

  sa.sin_port   = htons(port);
  sa.sin_family = AF_INET;

  if(!inet_aton(addr, &(sa.sin_addr)))
    { throw "Invalid address"; }

  if(conn)
    { Close(); }
  else
    {
    if(bind(sock, (sockaddr*)&sa, sizeof(sa)) < 0)
      { throw "Error binding socket"; }

    if(listen(sock, 5) < 0)
      { throw "Error listening on socket"; }

    conn = true;
    }
  }

Socket *Socket::Accept()
  {
  int nsck;
  sockaddr_in sa;
  socklen_t len = sizeof(sa);

AcceptLoop:
  nsck = accept(sock, 0, 0);

  if(!nsck)
    {
    Close();
    return false;
    }
  else if(nsck < 0)
    {
    switch(errno)
      {
      case EINTR:
        { goto AcceptLoop; }
      case EBADF:
      case EINVAL:
        { Close(); }
      case ECONNABORTED:
        { return false; }
      default:
        { throw strerror(errno); }
      }
    }

  getpeername(nsck, (sockaddr*)&sa, &len);
  strcpy(ip, inet_ntoa(sa.sin_addr));

  Socket *s = new Socket(nsck);
  return s;
  }

void Socket::Connect(char *addr, unsigned short int port)
  {
  sockaddr_in sa;
  socklen_t len = sizeof(sa);

  sa.sin_port        = htons(port);
  sa.sin_family      = AF_INET;

  if(!inet_aton(addr, &(sa.sin_addr)))
    { throw "Invalid address"; }

  if(conn)
    { throw "Socket already connected"; }

  if(connect(sock, (sockaddr*)&sa, sizeof(sa)) < 0)
    { throw "Error connecting socket"; }

  getpeername(sock, (sockaddr*)&sa, &len);
  strcpy(ip, inet_ntoa(sa.sin_addr));
  conn = true;
  }

void Socket::Send(char *buf, int len)
  {
  int num = 0;

  if(!conn || len < 1)
    { return; }

SendLoop:
  do
    {
    buf += num;
    len -= num;
    num = send(sock, buf, len, 0);
    }while(num < len && num > 0);

  if(!num)
    { Close(); }
  else if(num < 0)
    {
    switch(errno)
      {
      case EINTR:
        { goto SendLoop; }
      case EPIPE:
      case ENOTCONN:
      case ECONNRESET:
        {
        Close();
        break;
        }
      default:
        { throw "Error sending to socket"; }
      }
    }
  }

int Socket::Recv(char *buf, int len)
  {
  int num;

  if(!conn || len < 1)
    { return false; }

RecvLoop:
  num = recv(sock, buf, len, 0);

  if(!num)
    { Close(); }
  else if(num < 0)
    {
    switch(errno)
      {
      case EINTR:
        { goto RecvLoop; }
      case EPIPE:
      case ENOTCONN:
      case ECONNRESET:
        {
        Close();
        return false;
        }
      default:
        { throw "Error receiving from socket"; }
      }
    }

  return num;
  }

int Socket::RecvTok(char *buf, char tok, int len)
  {
  int   num = 0;
  char *pos, *ptr = buf;

  do
    {
    buf += num;
    len -= num;

    num = Recv(buf, len);
    pos = (char*)memchr(buf, tok, num);
    }while(!pos && num < len && num > 0);

  if(pos)
    {
   *pos = 0;
    num = pos - ptr;
    }
  else
    {
   *ptr = 0;
    num = 0;
    }

  return num;
  }

bool Socket::GetHost(char *buf)
  {
  socklen_t len;
  struct hostent *host;
  struct sockaddr_in sa;

  len = sizeof(sa);

  if(getpeername(sock, (struct sockaddr *)&sa, &len) < 0)
    { return false; }

  if(!(host = gethostbyaddr((const char*)&(sa.sin_addr), sizeof(struct in_addr), AF_INET)))
    { return false; }

  strcpy(buf, host->h_name);
  return true;
  }

void Socket::ReuseAddr(bool flag)
  {
  int opt = (flag) ? 1 : 0;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
  }

const char *Socket::IP()
  { return ip; }

bool Socket::Active()
  { return conn; }

void Socket::Close()
  {
  shutdown(sock, SHUT_RDWR);
  conn = false;
  close(sock);
  }

void Socket::Init()
  {
  if(!(sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)))
    { throw "Error creating socket"; }

  conn = false;
  ReuseAddr(true);
  }

Socket::Socket(int s)
  {
  sockaddr_in sa;
  socklen_t len = sizeof(sa);

  sock = s;

  getpeername(sock, (struct sockaddr*)&sa, &len);
  strcpy(ip, inet_ntoa(sa.sin_addr));

  conn = true;
  }

Socket::Socket()
  { Init(); }

Socket::~Socket()
  {
  if(conn)
    { Close(); }
  }

