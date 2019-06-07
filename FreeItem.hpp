/*============================================================================*/
/* FreeItem.hpp -- free item in item heap                                     */
/*============================================================================*/
#pragma once

namespace Structures
{
  class FreeItem : public SizedHeader
  {
  protected:
 
  public:
    void* operator new(size_t);
    void* operator new(size_t, void*);
  
    FreeItem(i8 nSizeBytes);
    ~FreeItem(void) noexcept(false);
  
    static FreeItem* aMorphFreeItem(void* aFree, i8 nSizeBytes);

    byte* ayAddressAfter(void);
    bool  bValidFreeItem(void);
  };
  
  FreeItem* _aFreeItem(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
