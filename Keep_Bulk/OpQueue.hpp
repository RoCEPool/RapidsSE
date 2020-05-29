/*============================================================================*/
/* OpQueue.hpp -- queue of ops waiting to be serviced                         */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class OpQueue : public ListNL_t<Op>
  {
  private:
    Lock m_okLock;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    OpQueue(void);
    ~OpQueue(void) noexcept(false);
    
    void vAppendOp(Op* pOp);
    void vDropLock(void);
    void vGrabLock(void);
    bool bGrabLock(void);
    bool bIsLocked(void);
    Op*  pPeekOp(void);
    Op*  pPopOp(void);
    i8   nQueuedOps(void);
  
    void vContentDump(void);
    
    Footer* aFooter(void);
  };
  
  OpQueue* _pOpQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
