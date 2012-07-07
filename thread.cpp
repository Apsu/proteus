#include "engine.h"

bool do_login(Socket *s)
  {
  char data[4096];
 
  while(s->Active())
    {
    s->RecvTok(data, '\n', 4096); // Read until newline
    s->Send(data, strlen(data));  // Echo data back
    }
  }

void *thread(void *arg)
  {
  TS *ts = (TS*)arg;
  Socket *s = (Socket*)ts->data;

  ts->pool->Add(ts->data); // Add thread to thread pool

  try
    { do_login(s); }
  catch(const char *except)
    {
    cout << "Thread: " << s->IP() << ": " << except << endl;
    cout << "Thread: " << strerror(errno);
    }

  s->Close();
  ts->pool->Del(s); // Remove thread from thread pool on exit
  }
