/*============================================================================*/
/* Queue.hpp -- Queue of Values                                               */
/*============================================================================*/
#pragma once

namespace BASE
{
  class QueueBuffer;
  
  class Queue : public Object
  {
  private:
    Lock     m_okLock;
    i8       m_nItems;

    QueueBuffer* m_pTopBuffer;
    QueueBuffer* m_pEndBuffer;
    
    // must be last
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void* operator new(size_t, Queue* pQueue);
    void  operator delete(void* pQueueAddress);
 
    Queue(eCN eClass);
    ~Queue(void) noexcept(false);
    

    i8   iAppend(Object* pObject);
    void vContentDump(Stz* aTab);
    i8   nItems(void);
    Object* pPeek(void);
    Object* pPop(void);
    void vReset(void);

  private:
    void vDiscard(void);
    void vExpand(void);
    void vVerify(void);

    void vPostMortem(void);
  
    Footer* aFooter(void);
  };
  
  Queue* _pQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
