/*============================================================================*/
/* Mersenne.hpp -- Mersenne Twister 64 bit random number generator            */
/*============================================================================*/
#pragma once

#include "Resources.h"

namespace Mersenne
{

  #define NN 312
  #define MM 156
  #define MATRIX_A 0xB5026F5AA96619E9ULL
  
  #define UM 0xFFFFFFFF80000000ULL /* Most significant 33 bits */
  #define LM 0x7FFFFFFFULL /* Least significant 31 bits */
  
#pragma GCC diagnostic ignored "-Wunused-variable"
  /* The array for the state vector */
  static unsigned long long mt[NN];
  
  /* mti==NN+1 means mt[NN] is not initialized */
  static int mti=NN+1;

  void init_genrand64 (unsigned long long seed);
  void init_by_array64(unsigned long long init_key[],
                       unsigned long long key_length);

  unsigned long long genrand64_int64(void);

  long long genrand64_int63(void);
  long long genrand64_int63(i8 iMin, i8 iMax);

  double genrand64_real1(void);
  double genrand64_real2(void);
  double genrand64_real3(void);

}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
