#ifndef __THREADS_H__
#define __THREADS_H__

#include <vector>
#include <algorithm>
#include <pthread.h>
#include <bits/local_lim.h>

using namespace std;

class Threads
  {
private:
  pthread_attr_t att;
  pthread_rwlock_t lock;
  vector<void*> pool;

public:
  void Spawn(void *(*)(void*), void *);
  void Empty(void  (*)(void*));

  void Add(void *);
  void Del(void *);

  Threads();
 ~Threads();
  };

struct TS
  {
  void    *data;
  Threads *pool;
  };

#endif //__THREADS_H__

