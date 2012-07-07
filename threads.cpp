#include "threads.h"

void Threads::Spawn(void *(*thread)(void*), void *data)
  {
  pthread_t tid;
  TS *ts = new TS;

  ts->data = data;
  ts->pool = this;

  pthread_create(&tid, &att, thread, (void*)ts);
  }

void Threads::Add(void *data)
  {
  pthread_rwlock_wrlock(&lock);
  pool.push_back(data);
  pthread_rwlock_unlock(&lock);
  }

void Threads::Del(void *data)
  {
  std::vector<void*>::iterator idx;
  pthread_rwlock_wrlock(&lock);
  idx = find(pool.begin(), pool.end(), data);
  pool.erase(idx);
  pthread_rwlock_unlock(&lock);
  }

void Threads::Empty(void (*func)(void*))
  {
  std::vector<void*>::iterator idx;
  pthread_rwlock_wrlock(&lock);
  for(idx = pool.begin(); idx != pool.end(); idx++)
    {
    if(func)
      { func(*idx); }
    }
  pthread_rwlock_unlock(&lock);
  }

Threads::Threads()
  {
  pthread_attr_init(&att);
  pthread_rwlock_init(&lock, 0);
  pthread_attr_setstacksize(&att, PTHREAD_STACK_MIN);
  pthread_attr_setdetachstate(&att, PTHREAD_CREATE_DETACHED);
  }

Threads::~Threads()
  {
  pthread_attr_destroy(&att);
  pthread_rwlock_unlock(&lock);
  pthread_rwlock_destroy(&lock);
  }
