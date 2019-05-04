/*============================================================================*/
/* HashBuffer.hpp -- HashMap Buffer                                           */
/*============================================================================*/
#pragma once

namespace Structures
{
  static const i8 C_HASH_ENTRIES = (C_SIZE_LARGE_OBJECT - C_1KB) / sizeof(HashEntry);
  static const i8 C_HASH_ARCS = 64;

  class HashCursor;
  class HashMap;
  
  class HashBuffer : public Object
  {
  friend HashCursor;

  private:
    HashMap*    m_pHashMap;     // which hash map
    HashBuffer* m_pParent;      // parent buffer of this buffer
    i8          m_iDepth;       // depth of this hash buffer
    i8          m_iArc;         // which arc from parent
    bool        m_bDuplicates;  // is this buffer handling duplicates?
  
    s256        m_sDotNotation; // dot notation of this buffer
  
    HashEntry   m_xHashEntry[C_HASH_ENTRIES]; // array of entries
  
    HashBuffer* m_paCollides[C_HASH_ARCS]; // child collision buffers

    i8 m_i_Duplicate;
    HashBuffer* m_pLinkDuplicates;
  
    i8 m_nEntries;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    HashBuffer(HashMap* pHashMap, HashBuffer* pParent, i8 iArc, bool bDuplicates);
    ~HashBuffer(void) noexcept(false);
  
    void vAnalysis(void);
    bool bAppendDuplicate(HashEntry* aHeadEntry, i8 iAltValue);
    void vInsertEntry(i8 iHash, byte* yaKey, i8 nBytes, i8 iValue);

  private:
     void vDumpEntry(i8 i_Entry, char cIndicator);
     void vHandleOccupied(i8 i_Entry, byte* yaKey, i8 nBytes, i8 iValue);
     void vInsertCollision(byte* yaKey, i8 nBytes, i8 iValue);
  
    Footer* aFooter(void);
 };
  
  HashBuffer* _pHashBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
