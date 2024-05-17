/*============================================================================*/
/* QueueBuffer.hpp -- Queue Buffer of Object Pointers                         */
/*============================================================================*/
#pragma once

namespace BASE
{
  class QueueBuffer : public Object
  {
    friend class Queue;
    
  private:
    static const i8 C_SIZE_AREA = 480;

    i8       m_nPointers;     // number of pointers in this buffer
    Object** m_ppTopObject;   // the top Object*
    Object** m_ppNextFree;    // free space pointer
    QueueBuffer* m_pNext;     // next buffer in chain

    Object* m_pArea[C_SIZE_AREA]; // Space where the operands are stored.
                                  // We choose 120 so that QueueBuffer fits and makes the most
                                  // of the tiny heap (1KB).

    // must be last
    Footer m_xFooter;
  
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
    Object*  pPeek(void);
    Object*  pPop(bool& bDiscard);

  private:
    Object** ppFirstPointer(void);
    Object** ppNextPointer(Object** ppPointer);
    void     vVerify(void);
    Footer*  aFooter();

  };
  
  QueueBuffer* _pQueueBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
