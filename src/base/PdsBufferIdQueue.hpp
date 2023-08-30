/*============================================================================*/
/* PdsBufferIdQueue.hpp -- Pds of BufferIds used as a queue                   */
/*============================================================================*/
#pragma once

namespace BASE
{
  class PdsBufferIdQueue : public Object
  {
  private:
    i8                        m_iSynthKey;     // Synthetic key for the Pds
    Pds_t<PdsItem_BufferId>*  m_pPdsOfBufferIds;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    PdsBufferIdQueue(void);
    ~PdsBufferIdQueue(void) noexcept(false);

    static void vRepair(Object* pObject);

    void        vAddBuffer(DataBuffer* pNewBuffer);
    DataBuffer* pCompletedBuffer(void);
    DataBuffer* pDanglingBuffer(void);
    DataBuffer* pDataBuffer(void);
    bool        bIsEmpty(void);

    Footer* aFooter(void);
  };

  PdsBufferIdQueue* _pPdsBufferIdQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
