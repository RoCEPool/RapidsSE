/*============================================================================*/
/* Queues.hpp -- Collection of the Systems' Queues                            */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Queues : public Object
  {
  private:
    TaskQueue*  m_pSlotQueues[C_SLOTS_PER_NODE];
    TaskQueue*  m_pGeneralQueue;
    TaskQueue*  m_pBlockedQueue;
    TaskQueue*  m_pSuspendedQueue;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Queues(void);
    ~Queues(void);
  
    Footer* aFooter(void);
  };
  
  Queues* _pQueues(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
