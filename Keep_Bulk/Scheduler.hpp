/*============================================================================*/
/* QueryScheduler.hpp - schedule tasks to threads                             */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Thread;
}

namespace Schedulers
{
  class Scheduler : public Object
  {
  private:
    Semaphore      m_xSemaphore;
    List_t<Task>   m_xTaskQueue;
    List_t<Thread> m_xThreadQueue;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Scheduler*);
    void  operator delete(void*);
    
    Scheduler(void);
    ~Scheduler(void);
      
    void vEnqueueTask_Mutex(Task* mpTask);
    bool bGetTask_Mutex(Thread* mpThread, bool bWait=true);

    i8   iQueuedTasks(void);
    i8   iQueuedThreads(void);
  
    Footer* aFooter(void);
  };

  Scheduler* _pScheduler(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
