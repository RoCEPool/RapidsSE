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
  
    bool m_bTrace;
    i8   m_nEntriesPerBuffer;
    i8   m_nArcsPerBuffer;

    HashBuffer* m_pRootBuffer;
    HashBuffer* m_pRootDuplicates;

    i8 m_nBuffers;
    i8 m_iMaxDepth;
    i8 m_nDupBuffers;
  
    i8 m_nEntries;
    i8 m_nArcEntries;
    i8 m_nDupEntries;

    // must be last
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void* operator new(size_t, HashMap* pHashMap);
    void  operator delete(void* pHashMapAddress);
 
    HashMap(bool bTrace=false, i8 nEntriesPerBuffer=0, i8 nArcsPerBuffer=0);
    ~HashMap(void) noexcept(false);
  
    void vAnalysis(bool bBufferAnalysis);
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
