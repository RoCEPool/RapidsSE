/*============================================================================*/
/* Queue.hpp - queue of values                                                */
/*============================================================================*/

#pragma once

#include "Structures.h"

namespace Structures
{
  class QueueBuffer;
  
  class Queue : public Object
  {
  private:
    Lock     m_zLock;
    i8       m_iCount;
    QueueBuffer* m_pTopBuffer;
    QueueBuffer* m_pEndBuffer;
    
    // must be last
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void* operator new(size_t, Queue* pQueue);
    void  operator delete(void* pQueueAddress);
 
    Queue(void);
    ~Queue(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);

    bool  bAppend(Value* pValue);
    i8    iCount(void);
    void  vDump(const char* pTab);
    void  vInitialize(void);
    bool  bPop(ValWsp* pValWsp);
    bool  bPop(ValSet* pValSet);
    
  private:
    void vExpand(void);
    void vDiscard(void);
    void vVerify(void);
    
    Footer* pFooter(void);
  };
  
  Queue* _pQueue(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
