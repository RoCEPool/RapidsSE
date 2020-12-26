/*============================================================================*/
/* Ent_int.hpp -- the flattened layout for the int case                       */
/*============================================================================*/
#pragma once

// the flattened layout for the int case

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
    u8 m_uFree    :  1; // free entry
    u8 m_uSpare   :  7; // spare
  } Rrf;         // 64b                                             // 1 word

  tET m_taNextEntry[C_MAX_LVL_LEVELS];  // tickets                  // 2 words
  union {
  u2  m_uEntryNo;  // used to verify the order of entries
  tET m_tOriginal; // used to verify the order of entries
  };

  i8  m_iIntElm0;       // first element of an integer key          // 1 word
                                                                    //--------
};                                                                  // 4 words

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
