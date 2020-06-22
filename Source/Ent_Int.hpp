/*============================================================================*/
/* Ent_Int.hpp -- Index Entry / Specific Int case                             */
/*============================================================================*/
#pragma once

namespace Entries
{ 
  struct Ent_Int
  {
    struct {
      u8 m_uBuffer  : 40; // which buffer
      u8 m_uItem    : 16; // item in that buffer
      u8 m_uSpare   :  4; // spare
      u8 m_uPayload :  1; // is there is a payload?
      u8 m_uOffsets :  3; // number of neo's that follow
    } Ref;         // 64
  
    u2 m_uOffset[5];      // the offsets
    u2 m_uSpare[3];       // some spares

    i8 m_iIntKey;         // the key
  };

  //----------------------------------------------------------------------------

  struct Ent_Cur
  {
    struct {
      u8 m_uBuffer  : 40; // which buffer
      u8 m_uItem    : 16; // item in that buffer
      u8 m_uSpare   :  4; // spare
      u8 m_uPayload :  1; // is there is a payload?
      u8 m_uOffsets :  3; // number of neo's that follow
    } Ref;         // 64

    i8 m_nKeySize;        // the key
    i8 m_iIntKey;         // the key
  
    i4 m_iaOffset[1];     // variable array must be last
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
