/*============================================================================*/
/* HashSet.hpp -- Set of Partition Hashed Buffers with dangling support       */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  class HashSet : public Object
  {
  public:
    DataBuffer*  m_paBufferChain[C_MAX_PARTITIONS];
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    HashSet(void);
   ~HashSet(void) noexcept(false);
    
  public:
  
    void vAddBuffer(i8 i_Ptn, DataBuffer* pDataBuffer);

    DataBuffer* pBuffer(i8 i_Ptn);
    DataBuffer* pCompletedBuffer(i8 i_Ptn);
    DataBuffer* pDanglingBuffer(i8 i_Ptn);

    Footer* aFooter(void);
  };
  
  HashSet* _pHashSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
