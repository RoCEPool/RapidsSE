/*============================================================================*/
/* Ent_gen.h -- the flattened layout for the general case                     */
/*============================================================================*/
#pragma once

  
// the flattened layout for the general case

struct Ent_gen
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
    u8 m_uFree    :  1; // free entry
    u8 m_uSpare   :  7; // spare
  } Ref;         // 64b

  tET m_taNextEntry[C_MAX_LVL_LEVELS]; // tickets
  union {
  u2  m_uEntryNo;  // used to verify the order of entries
  tET m_tOriginal; // used to verify the order of entries
  };

  i8 m_nElmSeqSize;     // total size of the key's elements
  i8 m_iIntElm0;        // first element of an integer key
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
