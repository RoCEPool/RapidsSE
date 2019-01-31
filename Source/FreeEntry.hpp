/*============================================================================*/
/* FreeEntry.hpp -- free entry in buffer                                      */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Entry;
}

namespace MiniHeaps
{
  class FreeEntry : public Item
  {
  protected:
    FreeEntry* m_aNextFree;  // next free
    FreeEntry* m_aPrevFree;  // previous free

  public:
    void* operator new(size_t);
    void* operator new(size_t, DBobjects::Entry*);
    void* operator new(size_t, FreeEntry*);
  
    FreeEntry(i8 nSizeItem);
    ~FreeEntry(void) noexcept(false);
  
    void vAssertFreeEntry(void);
    bool bValidFreeEntry(void);
  
    FreeEntry* aNextFree(void);
    void       vNextFree(FreeEntry* aNextFree);
  
    FreeEntry* aPrevFree(void);
    void       vPrevFree(FreeEntry* aPrevFree);
  
    char* aAddressAfter(void);
  
    Footer* aFooter(void);
};
  
  FreeEntry* _aFreeEntry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
