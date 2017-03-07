/*============================================================================*/
/* Queue.hpp -- Queue of Values                                               */
/*============================================================================*/
#pragma once

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
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);

    bool  bAppend(Value* pValue);
    i8    iCount(void);
    void  vContentDump(Str* aTab);
    void  vFinalize(void);
    void  vInitialize(void);
    bool  bPop(ValWsp* pValWsp);
    bool  bPop(ValSet* pValSet);
    
  private:
    void vExpand(void);
    void vDiscard(void);
    void vVerify(void);
    
    Footer* pFooter(void);
  };
  
  Queue* _pQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
