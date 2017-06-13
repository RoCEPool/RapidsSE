/*============================================================================*/
/* TaskQueue.hpp - set of tasks                                               */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Thread;
}

namespace Tasks
{
  
  class TaskQueue : public Object,
                    List_t<Task>
  {
  private:
    Lock    m_xLock;
    i8      m_iCount;
    NP_Task m_npHead;
    NP_Task m_npTail;
  
    List_t<Threads::Thread> m_xThreadQueue;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, TaskQueue* pTaskQueue);
    void  operator delete(void*);
    
    TaskQueue(void);
    ~TaskQueue(void);
      
    void  vGrabLock(void);
    void  vDropLock(void);
    
    void  vQueueTask(Task* pTask);
    i8    iQueuedTasks(void);
    Task* mpPopTask(void);

    void  vQueueThread(Threads::Thread* pThread);
    i8    iQueuedThreads(void);
    Threads::Thread* mpPopThread(void);
  
  private:
    void vVerify(void);
    
    Footer* pFooter(void);
  };

  TaskQueue* _pTaskQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
