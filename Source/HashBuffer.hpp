/*============================================================================*/
/* HashBuffer.hpp -- HashMap Buffer                                           */
/*============================================================================*/
#pragma once

namespace Structures
{
  static const i8 C_HASH_ENTRIES = (C_SIZE_LARGE_OBJECT - C_1KB) / sizeof(HashEntry);
  static const i8 C_HASH_COLLIDES = 8;

  class HashBuffer : public Object
  {
  private:
    i8 m_i_Duplicate;
    HashEntry m_xHashEntry[C_HASH_ENTRIES];
  
    HashBuffer* m_pDuplicates;
    HashBuffer* m_paCollides[C_HASH_COLLIDES];
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    HashBuffer(void);
    ~HashBuffer(void) noexcept(false);
  
    void vAppendDuplicate(i8 iDepth, i8 iHash, byte* yaKey, i8 nBytes, i8 iValue);
    void vInsert(i8 iDepth, i8 iHash, byte* yaKey, i8 nBytes, i8 iValue);
    void vInsertCollide(i8 iDepth, byte* yaKey, i8 nBytes, i8 iValue);
    void vInsertDuplicate(i8 iDepth, byte* yaKey, i8 nBytes, i8 iValue);
    bool bIterate(i8& iCursor, byte*& yaKey, i8& nBytes, i8& iValue);

  private:
    Footer* aFooter(void);
 };
  
  HashBuffer* _pHashBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
