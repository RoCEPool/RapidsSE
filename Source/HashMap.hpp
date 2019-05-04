/*============================================================================*/
/* HashMap.hpp -- HashMap                                                     */
/*============================================================================*/
#pragma once

namespace Structures
{
  class HashMap : public Object
  {
  friend HashBuffer;
  friend HashCursor;

  private:
  
    i8 m_nEntriesPerBuffer;

    HashBuffer* m_pRootBuffer;
    HashBuffer* m_pRootDuplicates;

    i8 m_nBuffers;
    i8 m_nDupBuffers;
    i8 m_nEntries;
    i8 m_nDupEntries;

    // must be last
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void* operator new(size_t, HashMap* pHashMap);
    void  operator delete(void* pHashMapAddress);
 
    HashMap(i8 nEntriesPerBuffer=0);
    ~HashMap(void) noexcept(false);
  
    void vAnalysis(void);
    void vAppendDuplicate(HashEntry* aHeadEntry, i8 iValue);
    i8   iEach(byte* yaKey,   i8 nBytes, i8& iCursor);
    bool bExists(byte* yaKey, i8 nBytes);
    bool bExists(byte* yaKey, i8 nBytes, i8 iValue);
    void vInsert(byte* yaKey, i8 nBytes, i8 iValue);

  private:

    Footer* aFooter(void);
  };
  
  HashMap* _pHashMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
