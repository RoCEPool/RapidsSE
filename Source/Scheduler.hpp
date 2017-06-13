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
    Lock           m_xLock;
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
      
    void  vEnqueueTask(Task* mpTask);
    i8    iQueuedTasks(void);

    void  vEnqueueThread(Thread* mpThread);
    i8    iQueuedThreads(void);
  
  private:
    void  vGrabLock(void);
    void  vDropLock(void);
  
    Footer* pFooter(void);
  };

  Scheduler* _pScheduler(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
