/*============================================================================*/
/* SpaceUnit.hpp -- space unit                                                */
/*============================================================================*/
#pragma once

namespace Base
{
  class SpaceUnit
  {
  public:
    byte m_yaByte[sizeof(DoubleWord)];

  public:
    SpaceUnit(void);
    ~SpaceUnit(void) noexcept(false);
  
    void vPut(i8 i_Offset, w8 wWord, i8 iEncoding);
  
    void vSetAll(byte yVal);
  
    w8 w8_1B(i8 i_Byte);
    w8 w8_2B(i8 i_Byte);
    w8 w8_3B(i8 i_Byte);
    w8 w8_4B(i8 i_Byte);
    w8 w8_5B(i8 i_Byte);
    w8 w8_6B(i8 i_Byte);
    w8 w8_7B(i8 i_Byte);
    w8 w8_8B(i8 i_Byte);

    w8 w8_1N(i8 i_Nibble);

    void v1B(i8 i_Byte, w8 wArg);
    void v2B(i8 i_Byte, w8 wArg);
    void v3B(i8 i_Byte, w8 wArg);
    void v4B(i8 i_Byte, w8 wArg);
    void v5B(i8 i_Byte, w8 wArg);
    void v6B(i8 i_Byte, w8 wArg);
    void v7B(i8 i_Byte, w8 wArg);
    void v8B(i8 i_Byte, w8 wArg);

    void v1N(i8 i_Nibble, w8 wArg);
  };
  
  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline SpaceUnit* _aSpaceUnit(void* aAddr)
  {
    SpaceUnit* aSpaceUnit = reinterpret_cast<SpaceUnit*>(aAddr);
    
    return aSpaceUnit;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
