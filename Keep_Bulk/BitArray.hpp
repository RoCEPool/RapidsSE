/*============================================================================*/
/* BitArray.hpp - Bit array                                                   */
/*============================================================================*/
#pragma once

namespace Structures
{
  class BitArray : public Object
  {
  static const i8 C_BIT_ARRAY = 3 * C_1KB;
  private:
    Lock m_zLock;
    i8   m_nBitsOn;
    char m_bBytes[C_BIT_ARRAY];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    BitArray(void);
    ~BitArray(void) noexcept(false);
  
    i8 nBitsOn(void);
    bool bBitState(i8 i_BitNo);
    void vFlipBit(i8 i_BitNo);
    bool bIsOff(i8 i_BitNo);
    bool bIsOn(i8 i_BitNo);
    void vSetOn(i8 i_BitNo);
    void vSetOff(i8 i_BitNo);
  
  private:
    static void vDecode(i8 i_BitNo, i8& i_Byte, i8& i_Bit, char& cMask);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
