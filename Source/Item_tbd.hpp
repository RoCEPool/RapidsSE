/*============================================================================*/
/* Item_tbd.hpp -- buffer item                                                */
/*============================================================================*/
#pragma once

namespace MiniHeaps
{
  class Item_tbd : public Header
  {
  protected:
    i8  m_nSizeBytes;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, Item_tbd*);
  
    Item_tbd(eCN eClassNo, i8 m_nSizeBytes);
    ~Item_tbd(void) noexcept(false);
  
    i8 nSizeBytes(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
