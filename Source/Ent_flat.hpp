/*============================================================================*/
/* Ent_flat.hpp -- flattened representations of Index Entry                   */
/*============================================================================*/
#pragma once

namespace Entries
{
  // this is the goal : an int Ent of 32B

  struct Ent_int
  {
    #ifdef __ENT_HEADER
    _Header m_xHeader;
    #endif
    struct {
      u8 m_uBuffer  : 32; // which buffer (2^32 * 1MB is 4PB)
      u8 m_uItem    : 16; // item in that buffer (2^16 is max of 64K items)
      u8 m_uLevels  :  3; // number of skip levels in next entry array
      u8 m_uPayload :  1; // is there is a payload? (not used yet)
      u8 m_uKind    :  4; // kind (would be KE_INT)
      u8 m_uSpare   :  8; // spare
    } Ref;         // 64b                                             // 1 word
  
    tET m_taNextEntry[C_MAX_LVL_LEVELS];  // tickets                  // 2 words
    u2  m_uEntryNo;       // entry number (not always set)

    i8  m_iIntElm0;       // first element of an integer key          // 1 word
                                                                      //--------
  };                                                                  // 4 words

  //----------------------------------------------------------------------------
  
  // this is the current flattened layout

  struct Ent_flat
  {
    #ifdef __ENT_HEADER
    _Header m_xHeader;
    #endif
    struct {
      u8 m_uBuffer  : 32; // which buffer (2^32 * 1MB is 4PB)
      u8 m_uItem    : 16; // item in that buffer (2^16 is max of 64K items)
      u8 m_uLevels  :  3; // number of skip levels in next entry array
      u8 m_uPayload :  1; // is there is a payload? (not used yet)
      u8 m_uKind    :  4; // kind (KE_GENERAL, KE_INT, ...)
      u8 m_uSpare   :  8; // spare
    } Ref;         // 64b
  
    tET m_taNextEntry[C_MAX_LVL_LEVELS]; // tickets

    #ifdef WITH_DEBUG_VERSION
    u2  m_uEntryNo;       // entry number (not always set)
    #endif
  
    i8 m_nElmSeqSize;     // total size of the key's elements
    i8 m_iIntElm0;        // first element of an integer key 
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
