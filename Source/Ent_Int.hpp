/*============================================================================*/
/* Ent_Int.hpp -- Index Entry / Specific Int case                             */
/*============================================================================*/
#pragma once

namespace Entries
{
  // this is the goal

  struct Ent_int
  {
    struct {
      u8 m_uBuffer  : 40; // which buffer
      u8 m_uItem    : 16; // item in that buffer
      u8 m_uLevels  :  3; // number of neo's that follow
      u8 m_uPayload :  1; // is there is a payload?
      u8 m_uKind    :  4; // kind
    } Ref;         // 64b                                             // 1 word
  
    tET m_taNextEntry[C_MAX_LVL_LEVELS];  // tickets                  // 2 words
    u2  m_uSpare;         // spare

    i8  m_iIntKey;        // the key                                  // 1 word
                                                                      //--------
  };                                                                  // 4 words

  //----------------------------------------------------------------------------
  
  // this is the current layout

  struct Ent_flat
  {
    _Header m_xHeader;
  
    struct {
      u8 m_uBuffer  : 40; // which buffer
      u8 m_uItem    : 16; // item in that buffer
      u8 m_uLevels  :  3; // number of neo's that follow
      u8 m_uPayload :  1; // is there is a payload?
      u8 m_uSpare   :  4; // spare
    } Ref;         // 64b
  
    tET m_taNextEntry[C_MAX_LVL_LEVELS]; // tickets

    #ifdef WITH_DEBUG_VERSION
    i8 m_iEntryNo;        // verify of order of skip items
    #endif
  
    i8 m_nKeySize;        // size of the key
    i8 m_iIntKey;         // the key 
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
