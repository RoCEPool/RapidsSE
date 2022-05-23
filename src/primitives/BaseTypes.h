/*============================================================================*/
/* BaseTypes.h -- Base Types                                                  */
/*============================================================================*/
#pragma once

namespace BASE
{
#include "BaseTypes_tcc.h"

  typedef __int128           i16;  // 16 byte signed integer (GNU extension)
  typedef unsigned __int128  u16;  // 16 byte unsigned integer (GNU extension)

  typedef i8                 t8;   // 8 byte transaction perspective

  typedef void           Opaque;   //

  const u8 C_BITS_PER_WORD         = 64;
  const u8 C_BYTES_PER_DOUBLE_WORD = 16;
  const u8 C_BYTES_PER_QUAD_WORD   = 32;
  const u8 C_BYTES_PER_CACHE_LINE  = 64;
  const u8 C_NIBBLES_PER_BYTE      = 2;

  const u8 C_WORDS_PER_WORD        = 1;
  const u8 C_WORDS_PER_DOUBLE_WORD = 2;
  const u8 C_WORDS_PER_QUAD_WORD   = 4;
  const u8 C_WORDS_PER_CACHE_LINE  = 8;

  struct _WordAsBytes {
    union {
      b1 m_yaByte[C_BYTES_PER_WORD];
      i8 m_iI8;
      u8 m_uU8;
    };
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
