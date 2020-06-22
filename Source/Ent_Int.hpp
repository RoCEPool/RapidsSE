/*============================================================================*/
/* Ent_Int.hpp -- Index Entry / Specific Int case                             */
/*============================================================================*/
#pragma once

namespace Entries
{ 
  struct Ent_Int
  {
    struct {                                                          // 1 word
      u8 m_uBuffer  : 40; // which buffer
      u8 m_uItem    : 16; // item in that buffer
      u8 m_uSpare   :  4; // spare
      u8 m_uPayload :  1; // is there is a payload?
      u8 m_uOffsets :  3; // number of neo's that follow
    } Ref;         // 64
  
    u2 m_uOffset[6];      // the offsets                              // 2 words
    u2 m_uSpare[2];       // some spares

    i8 m_iIntKey;         // the key                                  // 1 word
                                                                      //--
  };                                                                  // 4 words

  //----------------------------------------------------------------------------

  struct Ent_Cur
  {
    _Header m_xHeader;
  
    struct {
      u8 m_uBuffer  : 40; // which buffer
      u8 m_uItem    : 16; // item in that buffer
      u8 m_uSpare   :  4; // spare
      u8 m_uPayload :  1; // is there is a payload?
      u8 m_uOffsets :  3; // number of neo's that follow
    } Ref;         // 64

    #ifdef WITH_DEBUG_VERSION
    i8  m_iSkipNo;        // verify of order of skip items
    #endif
  
    i8 m_nKeySize;        // size of the key
    i8 m_iIntKey;         // the key 
  
    i4 m_iaOffset[1];     // variable array must be last
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
