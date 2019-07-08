/*============================================================================*/
/* BufferQueue.hpp -- queue of buffers to be processed by an op               */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class BufferQueue : public ListNL_t<OperandBuffer>
  {
  private:
    Lock m_okLock;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    BufferQueue(void);
    ~BufferQueue(void) noexcept(false);
    
    void vAppendBuffer(OperandBuffer* pBuffer);
    void vDropLock(void);
    void vGrabLock(void);
    bool bGrabLock(void);
    bool bIsLocked(void);
    OperandBuffer*  pPeekBuffer(void);
    OperandBuffer*  pPopBuffer(void);
    i8   nQueuedOps(void);
  
    void vContentDump(void);
    
    Footer* aFooter(void);
  };
  
  BufferQueue* _pBufferQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
