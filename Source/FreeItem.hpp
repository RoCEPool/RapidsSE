/*============================================================================*/
/* FreeItem.hpp -- free item in item heap                                     */
/*============================================================================*/
#pragma once

namespace MiniHeaps
{
  class FreeItem : public Item
  {
  protected:
 
  public:
    void* operator new(size_t);
    void* operator new(size_t, void*);
    void* operator new(size_t, Item*);
    void* operator new(size_t, FreeItem*);
  
    FreeItem(i8 nSizeItem);
    ~FreeItem(void) noexcept(false);
  
    static FreeItem* aMorphFreeItem(Item* aItem);
    static FreeItem* aMorphFreeItem(void* aFree, i8 nSizeFree);

    void vAssertFreeEntry(void);
    bool bValidFreeEntry(void);
  
    char* aAddressAfter(void);
  
};
  
  FreeItem* _aFreeItem(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
