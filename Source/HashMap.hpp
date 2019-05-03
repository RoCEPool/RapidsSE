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
    HashBuffer* m_pRootBuffer;
    HashBuffer* m_pRootDuplicates;

    // must be last
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void* operator new(size_t, HashMap* pHashMap);
    void  operator delete(void* pHashMapAddress);
 
    HashMap(void);
    ~HashMap(void) noexcept(false);
  
    void vAppendDuplicate(HashEntry* aHeadEntry, i8 iValue);
    i8   iEach(byte* yaKey,   i8 nBytes, i8& iCursor);
    bool bExists(byte* yaKey, i8 nBytes);
    bool bExists(byte* yaKey, i8 nBytes, i8 iValue);
    void vInsert(byte* yaKey, i8 nBytes, i8 iValue);
    bool bIterate(HashCursor* pCursor, byte*& yaKey, i8& nBytes, i8& iValue);

  private:

    Footer* aFooter(void);
  };
  
  HashMap* _pHashMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
