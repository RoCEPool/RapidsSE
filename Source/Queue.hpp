/*============================================================================*/
/* Queue.hpp - queue of values                                                */
/*============================================================================*/

#pragma once

#include "Tasking.h"

namespace DataStructures
{
  class QueueBuffer;
  
  class Queue : public Object
  {
  private:
    Lock     m_zLock;
    i8       m_iCount;
    QueueBuffer* m_pTopBuffer;
    QueueBuffer* m_pEndBuffer;
    
   public:
    void* operator new(size_t);
    void  operator delete(void* pQueueAddress);
 
    Queue(void);
    ~Queue(void);
    
    bool  bAppend(OnStack* pValue);
    i8    iCount(void);
    void  vDump(void);
    void  vInitialize(void);
    bool  bPop(ValWsp* pValWsp);
    bool  bPop(ValSet* pValSet);
    
  private:
    void vExpand(void);
    void vDiscard(void);
    void vVerify(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
