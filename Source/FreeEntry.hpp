/*============================================================================*/
/* FreeEntry.hpp -- free entry in buffer                                      */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class FreeEntry : public Item
  {
  protected:
    FreeEntry* m_pNextFree;  // next free
    FreeEntry* m_pPrevFree;  // previous free

  public:
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
    void* operator new(size_t, FreeEntry*);
  
    FreeEntry(i8 nSizeItem);
    ~FreeEntry(void) noexcept(false);
  
    void vAssertFreeEntry(void);
    bool bValidFreeEntry(void);
  
    FreeEntry* pNextFree(void);
    void       vNextFree(FreeEntry* pNextFree);
  
    FreeEntry* pPrevFree(void);
    void       vPrevFree(FreeEntry* pPrevFree);
  
    char* aAddressAfter(void);
  
    Footer* aFooter(void);
};
  
  FreeEntry* _pFreeEntry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
