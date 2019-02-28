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
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    i8   iEach(char* aKey, i8 nBytes, void* aCursor);
    bool bExists(char* aKey, i8 nBytes, i8 iValue);
    void vInsert(char* aKey, i8 nBytes, i8 iValue);

  private:
    
    Footer* aFooter(void);
  };
  
  HashMap* _pHashMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
