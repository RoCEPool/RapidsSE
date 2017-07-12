/*============================================================================*/
/* BitArray.hpp - Bit array                                                   */
/*============================================================================*/
#pragma once

namespace DBobjects
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
    ~BitArray(void);
  
    i8 nBitsOn(void);
    bool bBitState(i8 iBitNo);
    void vFlipBit(i8 iBitNo);
    bool bIsOff(i8 iBitNo);
    bool bIsOn(i8 iBitNo);
    void vSetOn(i8 iBitNo);
    void vSetOff(i8 iBitNo);
  
  private:
    static void vDecode(i8 iBitNo, i8& iByte, i8& iBit, char& cMask);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
