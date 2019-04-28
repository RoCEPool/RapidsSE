/*============================================================================*/
/* HashBuffer.hpp -- HashMap Buffer                                           */
/*============================================================================*/
#pragma once

namespace Structures
{
  static const i8 C_HASH_ENTRIES = (C_SIZE_LARGE_OBJECT - C_1KB) / sizeof(HashEntry);
  static const i8 C_HASH_COLLIDES = 8;

  class HashMap;
  
  class HashBuffer : public Object
  {
  private:
    HashMap* m_pHashMap;     // which hash map
    bool     m_bDuplicates;  // is this buffer handling duplicates?
    i8       m_iDepth;       // depth of hash buffer
  
    HashEntry   m_xHashEntry[C_HASH_ENTRIES];
    HashBuffer* m_paCollides[C_HASH_COLLIDES];

    i8 m_i_Duplicate;
    HashBuffer* m_pLinkDuplicates;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    HashBuffer(HashMap* pHashMap, bool bDuplicates, i8 iDepth);
    ~HashBuffer(void) noexcept(false);
  
    void vAppendDuplicate(HashEntry* aHeadEntry, i8 iValue);
    void vHandleOccupied(i8 i_Entry, i8 iHash, byte* yaKey, i8 nBytes, i8 iValue);
    void vInsert(i8 iHash, byte* yaKey, i8 nBytes, i8 iValue);
    void vInsertCollide(byte* yaKey, i8 nBytes, i8 iValue);
    bool bIterate(i8& iCursor, byte*& yaKey, i8& nBytes, i8& iValue);

  private:
    Footer* aFooter(void);
 };
  
  HashBuffer* _pHashBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
