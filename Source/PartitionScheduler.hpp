/*============================================================================*/
/* PartitionScheduler.hpp - schedule tasks to threads                         */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Thread;
}

namespace Schedulers
{
  class PartitionScheduler : public Scheduler
  {
  private:
    Semaphore      m_xSemaphore;
    List_t<Task>   m_xTaskQueue;
    List_t<Thread> m_xThreadQueue;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, PartitionScheduler*);
    void  operator delete(void*);
    
    PartitionScheduler(void);
    ~PartitionScheduler(void);
      
    void vEnqueueTask_Mutex(Task* mpTask);
    bool bGetTask_Mutex(Thread* mpThread, bool bWait=true);

    i8   iQueuedTasks(void);
    i8   iQueuedThreads(void);
  
    Footer* aFooter(void);
  };

  PartitionScheduler* _pPartitionScheduler(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
