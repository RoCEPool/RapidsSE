/*============================================================================*/
/* Ent_Int.hpp -- Index Entry / Specific Int case                             */
/*============================================================================*/
#pragma once

namespace Entries
{
  // this is the goal

  struct Ent_Int
  {
    struct {
      u8 m_uBuffer  : 40; // which buffer
      u8 m_uItem    : 16; // item in that buffer
      u8 m_uSpare   :  4; // spare
      u8 m_uPayload :  1; // is there is a payload?
      u8 m_uOffsets :  3; // number of neo's that follow
    } Ref;         // 64                                              // 1 word
  
    u2 m_uaOffset[6];     // the offsets                              // 2 words
    u2 m_uSkipNo;         // skip numbering
    u2 m_uSpare;          // spare
  
    i8 m_iIntKey;         // the key                                  // 1 word
                                                                      //--------
  };                                                                  // 4 words

  //----------------------------------------------------------------------------
  
  // this is the current layout

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
  
    i4 m__iaOffset__[6];  // the offsets (not used yet)

    #ifdef WITH_DEBUG_VERSION
    i8  m_uSkipNo;        // verify of order of skip items
    #endif
  
    i8 m_nKeySize;        // size of the key
    i8 m_iIntKey;         // the key 
  
    i4 m_iaOffset[1];     // variable array must be last
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
