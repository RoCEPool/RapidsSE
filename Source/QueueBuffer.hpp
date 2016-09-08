/*============================================================================*/
/* QueueBuffer.hpp - queue buffer of values                                       */
/*============================================================================*/

#pragma once

#include "Structures.h"

namespace Structures
{
  class QueueBuffer : public Buffer
  {
    friend class Queue;
    
  private:
    QueueBuffer* m_pNext;
    i8       m_iCount;
    Value* m_pTopValue;
    Value* m_pNextFree;
    // ..data..
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    QueueBuffer(void);
    ~QueueBuffer(void);
    
    bool     bAppend(Value* pValue);
    i8       iCount(void);
    void     vDump(void);
    QueueBuffer* pNext(void);
    void     vNext(QueueBuffer* pQbuffer);
    bool     bPop(ValWsp* pValWsp, bool& bDiscard);
    bool     bPop(ValSet* pValSet, bool& bDiscard);

  private:
    Value*   pFirstValue(void);
    Value*   pNextValue(Value* pValue);
    void     vVerify(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
