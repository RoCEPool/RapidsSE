/*============================================================================*/
/* BfrBuffer.hpp -- Bloom Filter Buffer                                       */
/*============================================================================*/
#pragma once

static const i8 C_MAX_WORDS = (C_SIZE_LARGE_OBJECT - C_4KB) / C_BYTES_PER_WORD;


namespace BASE
{
  class BfrBuffer : public DataBuffer
  {
  private:
    u8 m_uBitMap[C_MAX_WORDS];

    i8 m_nKeys;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    BfrBuffer(void);
   ~BfrBuffer(void) noexcept(false);
  
    static void       vPostRecv(void* aCsvBuffer);
    static void       vRepair(Object* pObject);


    bool bIsKeySet(u8 uKey);
    i8   nKeys(void);
    bool bSetKey(u8 uKey);
    bool bValidBfrBuffer(void);

  private:
    Footer* aFooter(void);
  };
  
  BfrBuffer* _pBfrBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
