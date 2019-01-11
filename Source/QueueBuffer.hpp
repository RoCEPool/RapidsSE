/*============================================================================*/
/* QueueBuffer.hpp -- Queue Buffer of Values                                  */
/*============================================================================*/
#pragma once

namespace Forth
{
  class GenVar;
}

namespace Structures
{
  class QueueBuffer : public Buffer
  {
    friend class Queue;
    
  private:
    i8     m_nValues;     // number of values in the queue
    Value* m_aTopValue;   // the top value
    Value* m_aNextFree;   // free space pointer
    QueueBuffer* m_pNext; // next buffer
    // ..data..
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    QueueBuffer(void);
    ~QueueBuffer(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    bool     bAppend(Value* pValue);
    i8       iCount(void);
    void     vContentDump(Stz* aTab);
    QueueBuffer* pNext(void);
    void     vNext(QueueBuffer* pQbuffer);
    bool     bPop(Forth::GenVar* pValWrk, bool& bDiscard);

  private:
    Value*   aFirstValue(void);
    Value*   aNextValue(Value* pValue);
    void     vVerify(void);
  };
  
  QueueBuffer* _pQueueBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
