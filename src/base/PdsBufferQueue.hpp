/*============================================================================*/
/* PdsBufferQueue.hpp -- Pds of Buffer addresses used as a queue              */
/*============================================================================*/
#pragma once

namespace BASE
{
  class PdsBufferQueue : public Object
  {
  private:
    i8                        m_iSynthKey;     // Synthetic key for the Pds
    DataBuffer*               m_pWorkingBuffer; // Temporary current working buffer
    Pds_t<PdsItem_Buffer>*  m_pPdsOfBufferAddrs;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    PdsBufferQueue(void);
    ~PdsBufferQueue(void) noexcept(false);

    static void vRepair(Object* pObject);

    void        vAddBuffer(DataBuffer* pNewBuffer);
    DataBuffer* pCompletedBuffer(void);
    DataBuffer* pDanglingBuffer(void);
    DataBuffer* pDataBuffer(void);
    void        vDump(void);
    bool        bIsEmpty(void);

    Footer* aFooter(void);
  };

  PdsBufferQueue* _pPdsBufferQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
