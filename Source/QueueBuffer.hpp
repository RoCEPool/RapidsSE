/*============================================================================*/
/* QueueBuffer.hpp - queue buffer of values                                       */
/*============================================================================*/

#pragma once

#include "Tasks.h"

namespace DataStructures
{
  class QueueBuffer : public Buffer
  {
    friend class Queue;
    
  private:
    QueueBuffer* m_pNext;
    i8       m_iCount;
    OnStack* m_pTopValue;
    OnStack* m_pNextFree;
    // ..data..
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    QueueBuffer(void);
    ~QueueBuffer(void);
    
    bool     bAppend(OnStack* pValue);
    i8       iCount(void);
    void     vDump(void);
    QueueBuffer* pNext(void);
    void     vNext(QueueBuffer* pQbuffer);
    bool     bPop(ValWsp* pValWsp, bool& bDiscard);
    bool     bPop(ValSet* pValSet, bool& bDiscard);

  private:
    OnStack*   pFirstValue(void);
    OnStack*   pNextValue(OnStack* pValue);
    void     vVerify(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
