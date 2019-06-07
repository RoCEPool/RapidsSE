/*============================================================================*/
/* FreeItem.hpp -- free item in item heap                                     */
/*============================================================================*/
#pragma once

namespace MiniHeaps
{
  class FreeItem : public Item_tbd
  {
  protected:
 
  public:
    void* operator new(size_t);
    void* operator new(size_t, void*);
  
    FreeItem(i8 nSizeItem);
    ~FreeItem(void) noexcept(false);
  
    static FreeItem* aMorphFreeItem(void* aFree, i8 nSizeFree);

    char* aAddressAfter(void);
    void  vAssertFreeEntry(void);
    bool  bValidFreeEntry(void);
  };
  
  FreeItem* _aFreeItem(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
