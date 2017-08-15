/*============================================================================*/
/* ServerScheduler.hpp - schedule tasks to threads                            */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Thread;
}

namespace Schedulers
{
  class ServerScheduler : public Scheduler
  {
  private:
    Semaphore    m_xSemaphore;
    TaskQueue    m_xTaskQueue;
    ThreadQueue  m_xThreadQueue;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, ServerScheduler*);
    void  operator delete(void*);
    
    ServerScheduler(void);
    ~ServerScheduler(void);
      
    void vEnqueueTask_Mutex(Task* mpTask);
    bool bGetTask_Mutex(Thread* mpThread, bool bWait=true);

    i8   iQueuedTasks(void);
    i8   iQueuedThreads(void);
  
    Footer* aFooter(void);
  };

  ServerScheduler* _pServerScheduler(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
