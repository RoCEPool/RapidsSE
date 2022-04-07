/*============================================================================*/
/* LglBufSet.hpp -- Set of Partition Hashed Buffers with dangling support       */
/*============================================================================*/
#pragma once

namespace STORE
{
  class LglBufSet : public Object
  {
  public:
    DataBuffer*  m_paBufferChain[C_MAX_LGLPTNS];
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    LglBufSet(void);
   ~LglBufSet(void) noexcept(false);
    
  public:
  
    void vAddBuffer(i8 i_Ptn, DataBuffer* pDataBuffer);

    DataBuffer* pBuffer(LglPtn tLglPtn);
    DataBuffer* pCompletedBuffer(LglPtn tLglPtn);
    DataBuffer* pDanglingBuffer(LglPtn tLglPtn);

    Footer* aFooter(void);
  };
  
  LglBufSet* _pLglBufSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
