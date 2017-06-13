/*============================================================================*/
/* QueryScheduler.hpp - queue of tasks meets queue of threads                 */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Thread;
}

namespace Schedulers
{
  
  class QueryScheduler : public Object,
                    List_t<Task>
  {
  private:
    Lock    m_xLock;
    i8      m_iCount;
    NP_Task m_npHead;
    NP_Task m_npTail;
  
    List_t<Thread> m_xThreadQueue;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, QueryScheduler* pTaskQueue);
    void  operator delete(void*);
    
    QueryScheduler(void);
    ~QueryScheduler(void);
      
    void  vGrabLock(void);
    void  vDropLock(void);
    
    void  vQueueTask(Task* pTask);
    i8    iQueuedTasks(void);
    Task* mpPopTask(void);

    void  vQueueThread(Thread* pThread);
    i8    iQueuedThreads(void);
    Thread* mpPopThread(void);
  
  private:
    void vVerify(void);
    
    Footer* pFooter(void);
  };

  QueryScheduler* _pTaskQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
