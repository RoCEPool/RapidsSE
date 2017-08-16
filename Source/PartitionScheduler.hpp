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
  
    struct {
      Semaphore*   m_pSemaphore;
      TaskQueue*   m_pTaskQueue;
      ThreadQueue* m_pThreadQueue;
    } m_xSlots[C_SLOTS_PER_NODE];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, PartitionScheduler*);
    void  operator delete(void*);
    
    PartitionScheduler(void);
    ~PartitionScheduler(void);
      
    void vEnqueueTask_Mutex(i8 i_PtnNo, Task* mpTask);
    bool bGetTask_Mutex(i8 i_PtnNo, Thread* mpThread, bool bWait=true);
  
    Footer* aFooter(void);
  };

  PartitionScheduler* _pPartitionScheduler(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
