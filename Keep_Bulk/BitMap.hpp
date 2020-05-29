/*============================================================================*/
/* BitMap.hpp - bit map (an array of bit arrays)                              */
/*============================================================================*/
#pragma once

namespace Structures
{
  class BitMap : public Object
  {
  static const i8 C_BIT_ARRAYS = 120000;
  
  public:
    BitArray* m_pBitArray[120000];
 
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    BitMap(void);
    ~BitMap(void) noexcept(false);
  
    void vSetOn(i8 i_Array, i8 i_Bit);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
