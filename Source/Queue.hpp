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
    Lock     m_okLock;
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
    ~Queue(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    void vAppend(Object* pObject);
    i8    iCount(void);
    void  vContentDump(Stz* aTab);
    Object* pPop(void);
    
  private:
    void vExpand(void);
    void vDiscard(void);
    void vVerify(void);
    
    Footer* aFooter(void);
  };
  
  Queue* _pQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
