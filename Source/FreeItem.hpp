/*============================================================================*/
/* FreeItem.hpp -- free item in item heap                                     */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Entry;
}

namespace MiniHeaps
{
  class FreeItem : public Item
  {
  protected:
    FreeItem* m_aNextFree;  // next free
    FreeItem* m_aPrevFree;  // previous free

  public:
    void* operator new(size_t);
    void* operator new(size_t, DBobjects::Entry*);
    void* operator new(size_t, FreeItem*);
  
    FreeItem(i8 nSizeItem);
    ~FreeItem(void) noexcept(false);
  
    void vAssertFreeEntry(void);
    bool bValidFreeEntry(void);
  
    FreeItem* aNextFree(void);
    void      vNextFree(FreeItem* aNextFree);
  
    FreeItem* aPrevFree(void);
    void      vPrevFree(FreeItem* aPrevFree);
  
    char* aAddressAfter(void);
  
    Footer* aFooter(void);
};
  
  FreeItem* _aFreeItem(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
