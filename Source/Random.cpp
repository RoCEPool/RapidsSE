/*============================================================================*/
/* Random.cpp -- generate random number/string                                */
/*============================================================================*/

#include "System.h"

/*----------------------------------------------------------------------------*/
/* generate a random number                                                   */
/*----------------------------------------------------------------------------*/
i8 Random::iInteger(void)
{
  i8 iRand = ThirdParty::genrand64_int63();
  
  return iRand;
}

/*----------------------------------------------------------------------------*/
/* generate a random number                                                   */
/*----------------------------------------------------------------------------*/
i8 Random::iInteger(i8 iMax)
{
  i8 iRand = ThirdParty::genrand64_int63() % iMax;
  
  return iRand;
}

/*----------------------------------------------------------------------------*/
/* generate a random number between low and high (inclusive)                  */
/*----------------------------------------------------------------------------*/
i8 Random::iInteger(i8 iLow, i8 iHigh)
{
  i8 r = ThirdParty::genrand64_int63();
  
  i8 m = iHigh - iLow + 1;
  i8 v = r % m;
  i8 n = v + iLow;
  
  assert (n >= iLow);
  assert (n <= iHigh);
  
  return n;
}

/*----------------------------------------------------------------------------*/
/* generate a random number                                                   */
/*----------------------------------------------------------------------------*/
u8 Random::uInteger(void)
{
  u8 iRand = ThirdParty::genrand64_int64();
  
  return iRand;
}

/*----------------------------------------------------------------------------*/
/* generate a random number                                                   */
/*----------------------------------------------------------------------------*/
u8 Random::uInteger(u8 uMax)
{
  u8 uRand = ThirdParty::genrand64_int64() % uMax;
  
  return uRand;
}

/*----------------------------------------------------------------------------*/
/* generate a random number between low and high (inclusive)                  */
/*----------------------------------------------------------------------------*/
u8 Random::uInteger(u8 uLow, u8 uHigh)
{
  u8 r = ThirdParty::genrand64_int64();
  
  u8 m = uHigh - uLow + 1;
  u8 v = r % m;
  u8 n = v + uLow;
  
  assert (n >= uLow);
  assert (n <= uHigh);
  
  return n;
}

/*----------------------------------------------------------------------------*/
/* choose a random logarithemic level (up to n levels, at given frequency)    */
/*----------------------------------------------------------------------------*/
i8 Random::iLogLevel(i8 nLevels, i8 iFrequency)
{
  //
  // This function generates a number between 0 and nLevels - 1.
  //
  // A level 0 is generated most of the time,
  //   the probability that a level 1 is generated is 1 / (2 ^ uFrequency),
  //   the probability that a level 2 is generated is 1 / (2 ^ uFrequency ^ 2),
  //   the probability that a level 3 is generated is 1 / (2 ^ uFrequency ^ 3).
  //
  // So if uFrequency is 2,
  //   then level 1 will probably be generated once every 4 times,
  //        level 2 will probably be generated once every 16 times,
  //        level 4 will probably be generated once every 64 times.
  //
  
  //
  // The algorithm works by repeatedly examining a subset of bits from a
  // random pattern.  The probability that all bits are zero is a function of
  // the number of bits in the subset.  The more bits in the subset the
  // less likely (logarithemically) that all bits are zero.
  //
  
  u8 uBitMask = C_ALL_BITS_ON >> (C_BITS_PER_WORD - iFrequency);
  
  u8 uBitSubset = 0;
  u8 nSubsetsLeft = 0;
  u8 uRandomPattern = 0;
  
  // step thru bit subsets until we find a subset with some bits set
  i8 nAllZeroSubsets;
  for (nAllZeroSubsets = -1; ! uBitSubset; nAllZeroSubsets++) {
    
    // if all bits used from the random bit pattern, generate a new pattern
    if (0 == nSubsetsLeft) {
      uRandomPattern = Random::uInteger();
      nSubsetsLeft = nLevels / iFrequency;
    }

    // take the next lower subset of bits
    uBitSubset = uRandomPattern & uBitMask;
    uRandomPattern >>= iFrequency;
    nSubsetsLeft--;
  }
  
  // compute the level based on the number of times we observed subsets
  // whose bits were all zero
  i8 iLevel = (nAllZeroSubsets >= nLevels)
                ? nLevels - 1
                : nAllZeroSubsets;
  
  return iLevel;
}

/*----------------------------------------------------------------------------*/
/* generate a random string of given length                                   */
/*----------------------------------------------------------------------------*/
void Random::vString(s256& sStr, i8 iLength)
{
  static char caCharSet[69] = // 68 actual chars + the '\0'
    "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-#?!";
  // 01234567890123456789012345678901234567890123456789012345678901234567
  // 0         1         2         3         4         5         6      6
  
  char* z = sStr.aChar();
  
  for (i8 i = 0; i < iLength; i++)
    *z++ = caCharSet[iInteger(68)];
  
  *z = '\0';
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
