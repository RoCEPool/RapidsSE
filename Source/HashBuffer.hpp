/*============================================================================*/
/* HashBuffer.hpp -- HashMap Buffer                                           */
/*============================================================================*/
#pragma once

namespace Structures
{
  //static const i8 C_HASH_ENTRIES = (C_SIZE_LARGE_OBJECT - C_1KB) / sizeof(HashEntry);
  static const i8 C_HASH_ENTRIES = 10;
  static const i8 C_HASH_COLLIDES = 8;

  class HashCursor;
  class HashMap;
  
  class HashBuffer : public Object
  {
  friend HashCursor;

  private:
    HashMap* m_pHashMap;     // which hash map
    i8       m_iDepth;       // depth of hash buffer
    bool     m_bDuplicates;  // is this buffer handling duplicates?
  
    HashEntry   m_xHashEntry[C_HASH_ENTRIES];
    HashBuffer* m_paCollides[C_HASH_COLLIDES];

    i8 m_i_Duplicate;
    HashBuffer* m_pLinkDuplicates;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    HashBuffer(HashMap* pHashMap, i8 iDepth);
    HashBuffer(HashMap* pHashMap, i8 iDepth, HashBuffer* pLinkDuplicates);
    ~HashBuffer(void) noexcept(false);
  
    bool bAppendDuplicate(HashEntry* aHeadEntry, i8 iValue);
    void vInsertEntry(i8 iHash, byte* yaKey, i8 nBytes, i8 iValue);

  private:
     void vHandleOccupied(i8 i_Entry, byte* yaKey, i8 nBytes, i8 iValue);
     void vInsertCollision(byte* yaKey, i8 nBytes, i8 iValue);
  
    Footer* aFooter(void);
 };
  
  HashBuffer* _pHashBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
