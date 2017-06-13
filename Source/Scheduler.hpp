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
    Lock m_xLock;
    List_t<Thread> m_xThreadQueue;
    List_t<Task>   m_xTaskQueue;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, Scheduler*);
    void  operator delete(void*);
    
    Scheduler(void);
    ~Scheduler(void);
      
    void  vGrabLock(void);
    void  vDropLock(void);
    
    void  vEnqueueTask(Task* pTask);
    i8    iQueuedTasks(void);

    void  vEnqueueThread(Thread* pThread);
    i8    iQueuedThreads(void);
  
  private:    
    Footer* pFooter(void);
  };

  Scheduler* _pScheduler(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
