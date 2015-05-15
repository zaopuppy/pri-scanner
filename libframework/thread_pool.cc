//
// Created by Yi Zhao on 5/8/15.
//

#include "thread_pool.h"


namespace Z {


class Runnable {
public:
  virtual void run() = 0;
};


class Executor {
public:
  virtual void execute(Runnable *cmd) = 0;
};

class ExecutorService {
public:
  typedef void (*callback_t)(void *arg);

public:
  virtual bool init() = 0;

  virtual void shutdown() = 0;

  virtual void shutdownNow() = 0;

  // TODO: `future` needed
  virtual void submit(Runnable *task, callback_t callback, void *arg) = 0;
};

class ThreadPool: ExecutorService {
public:

private:
  virtual bool init() override;

  virtual void shutdown() override;

  virtual void shutdownNow() override;

  virtual void submit(Runnable *task, callback_t callback, void *arg) override;

private:

};


bool ThreadPool::init() {
  return ExecutorService::init();
}

void ThreadPool::shutdown() {
  ExecutorService::shutdown();
}

void ThreadPool::shutdownNow() {
  ExecutorService::shutdownNow();
}

void ThreadPool::submit(Runnable *task, ExecutorService::callback_t callback, void *arg) {
  ExecutorService::submit(task, callback, arg);
}
}

