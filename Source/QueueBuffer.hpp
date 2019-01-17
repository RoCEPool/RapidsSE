/*============================================================================*/
/* QueueBuffer.hpp -- Queue Buffer of Object Pointers                         */
/*============================================================================*/
#pragma once

namespace Structures
{
  class QueueBuffer : public Buffer
  {
    friend class Queue;
    
  private:
    i8       m_nPointers;     // number of pointers in this buffer
    Object** m_ppTopObject;   // the top Object*
    Object** m_ppNextFree;    // free space pointer
  
    QueueBuffer* m_pNext;     // next buffer in chain
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    QueueBuffer(void);
    ~QueueBuffer(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    bool     bAppend(Object* pObject);
    i8       iCount(void);
    void     vContentDump(Stz* aTab);
    QueueBuffer* pNext(void);
    void     vNext(QueueBuffer* pQbuffer);
    Object*  pPop(bool& bDiscard);

  private:
    Object** ppFirstPointer(void);
    Object** ppNextPointer(Object** ppPointer);
    void     vVerify(void);
  };
  
  QueueBuffer* _pQueueBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
