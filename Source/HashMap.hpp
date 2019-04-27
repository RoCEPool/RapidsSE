/*============================================================================*/
/* HashMap.hpp -- HashMap                                                     */
/*============================================================================*/
#pragma once

namespace Structures
{
  class HashBuffer;
  
  class HashMap : public Object
  {
  private:
    HashBuffer* m_pRootBuffer;
  
    // must be last
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void* operator new(size_t, HashMap* pHashMap);
    void  operator delete(void* pHashMapAddress);
 
    HashMap(void);
    ~HashMap(void) noexcept(false);
  
    i8   iEach(byte* aKey,   i8 nBytes, i8& iCursor);
    bool bExists(byte* aKey, i8 nBytes);
    bool bExists(byte* aKey, i8 nBytes, i8 iValue);
    void vInsert(byte* aKey, i8 nBytes, i8 iValue);
    bool bIterate(i8& iCursor, byte*& aKey, i8& nBytes, i8& iValue);

  private:
    void vInsert(i8 iHash, byte* aKey, i8 nBytes, i8 iValue);

    Footer* aFooter(void);
  };
  
  HashMap* _pHashMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
