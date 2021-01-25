/*============================================================================*/
/* BytesTypes.h -- Bytes Types                                                */
/*============================================================================*/
#pragma once

namespace BASE
{
  template <i8 AREA> class Bytes_t;
  typedef Bytes_t<32>      b32;
  typedef Bytes_t<256>     b256;
  typedef Bytes_t<C_4K>    b4K;   // note: K, not KB
  typedef Bytes_t<C_8K>    b8K;   // note: K, not KB
  typedef Bytes_t<C_32K>   b32K;  // note: K, not KB
  typedef Bytes_t<C_256K>  b256K; // note: K, not KB
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
