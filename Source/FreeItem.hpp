/*============================================================================*/
/* FreeItem.hpp -- free item in item heap                                     */
/*============================================================================*/
#pragma once

namespace MiniHeaps
{
  class FreeItem : public Item
  {
  protected:
    //FreeItem* m_aNextFree;  // next free
 
  public:
    void* operator new(size_t);
    void* operator new(size_t, Item*);
    void* operator new(size_t, FreeItem*);
  
    FreeItem(i8 nSizeItem);
    ~FreeItem(void) noexcept(false);
  
    static FreeItem* aMorphFreeItem(Item* aItem);

    void vAssertFreeEntry(void);
    bool bValidFreeEntry(void);
  
    //FreeItem* aNextFree(void);
    //void      vNextFree(FreeItem* aNextFree);
  
    char* aAddressAfter(void);
  
//    Footer* aFooter(void);
};
  
  FreeItem* _aFreeItem(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
