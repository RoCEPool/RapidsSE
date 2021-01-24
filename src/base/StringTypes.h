/*============================================================================*/
/* StringTypes.h -- String Types                                              */
/*============================================================================*/
#pragma once

namespace BASE
{
  template <i8 AREA> class String_t;
  typedef String_t<32>     s32;
  typedef String_t<256>    s256;
  typedef String_t<C_2K>   s2K;   // note: 2K not 2KB
  typedef String_t<C_4K>   s4K;   // note: 4K not 4KB
  typedef String_t<C_16KB> s16KB; // note: this is 16KB
  typedef String_t<C_24KB> s24KB; // note: this is 24KB
  typedef String_t<C_32KB> s32KB; // note: this is 32KB
  typedef String_t<C_48K>  s48K;  // note: 48K not 24KB
  typedef String_t<C_1M>   s1M;   // note: 1M not 1MB

  typedef String_t<C_MAX_STATEMENT> sSTMT;
  typedef String_t<C_MAX_VCH>       sVCH;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
