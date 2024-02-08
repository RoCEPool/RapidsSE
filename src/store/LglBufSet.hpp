/*============================================================================*/
/* LglBufSet.hpp -- Set of Partition Hashed Buffers with dangling support       */
/*============================================================================*/
#pragma once

namespace STORE
{
  typedef struct {
    i8 iLglPtn;
    i8 iSynthKey;
    DataBuffer* pDataBuffer;
  } LglBufSetItem;

  class LglBufSet : public Object
  {
  public:
    i8                      m_iSynthKey;     // Synthetic key used to append buffers to the Pds structure
    Pds_t<LglBufSetItem>*   m_pPdsOfBuffers; // Pds of buffers

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    LglBufSet(void);
    ~LglBufSet(void) noexcept(false);

  public:

    void vAddBuffer(LglPtn tLglPtn, DataBuffer* pDataBuffer);
    DataBuffer* pBuffer(LglPtn tLglPtn);
    DataBuffer* pCompletedBuffer(LglPtn tLglPtn);
    DataBuffer* pDanglingBuffer(LglPtn tLglPtn);

    Footer* aFooter(void);
  };
  static i8 iLglBufSetKeyCompare(void* aItem_l, void* aItem_r);
  LglBufSet* _pLglBufSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
