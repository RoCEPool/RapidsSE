/*============================================================================*/
/* Random.cpp -- generate a random number                                     */
/*============================================================================*/

#include "System.h"

//---------------------------------------------------------------------------//
// generate a random number                                                  //
//---------------------------------------------------------------------------//
i8 Random::iRandom(void)
{
  i8 iRand = ThirdParty::genrand64_int63();
  
  return iRand;
}

//---------------------------------------------------------------------------//
// generate a random number                                                  //
//---------------------------------------------------------------------------//
i8 Random::iRandom(i8 iMax)
{
  i8 iRand = ThirdParty::genrand64_int63() %iMax;
  
  return iRand;
}

/*---------------------------------------------------------------------------*/
/* iRandom: generate a random number between low and high (inclusive)        */
/*---------------------------------------------------------------------------*/
i8 Random::iRandom(i8 iLow, i8 iHigh)
{
  i8 r = ThirdParty::genrand64_int63();
  
  i8 m = iHigh - iLow + 1;
  i8 v = r % m;
  i8 n = v + iLow;
  
  assert (n >= iLow);
  assert (n <= iHigh);
  
  return n;
}

//---------------------------------------------------------------------------//
// generate a random number                                                  //
//---------------------------------------------------------------------------//
u8 Random::uRandom(void)
{
  u8 iRand = ThirdParty::genrand64_int64();
  
  return iRand;
}

//---------------------------------------------------------------------------//
// generate a random number                                                  //
//---------------------------------------------------------------------------//
u8 Random::uRandom(u8 uMax)
{
  u8 uRand = ThirdParty::genrand64_int64() % uMax;
  
  return uRand;
}

/*---------------------------------------------------------------------------*/
/* uRandom: generate a random number between low and high (inclusive)        */
/*---------------------------------------------------------------------------*/
u8 Random::uRandom(u8 uLow, u8 uHigh)
{
  u8 r = ThirdParty::genrand64_int64();
  
  u8 m = uHigh - uLow + 1;
  u8 v = r % m;
  u8 n = v + uLow;
  
  assert (n >= uLow);
  assert (n <= uHigh);
  
  return n;
}

/*---------------------------------------------------------------------------*/
/* sRandomString: generate a random string of given length                   */
/*---------------------------------------------------------------------------*/
s48K Random::sRandomString(i8 iLength) {
  
  s256 sCharSet =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-#?!"_Str;
  
  s48K sRandomString;
  
  ASSERT(iLength < static_cast<i8>(sizeof(s48K)));
  
  i8 iCharSetSize = sCharSet.iLength() - 1;
  
  for (i8 i = 0; i < iLength; i++) {
    i8 iKey = iRandom(iCharSetSize);
    sRandomString.vAppend(sCharSet.s()[iKey]);
  }
  
  sRandomString.vAppend('\0');

  return sRandomString;
}

/*----------------------------------------------------------------------------*/
/* choose a random logarithemic level (up to 8 levels)                        */
/*----------------------------------------------------------------------------*/
i8 Random::iLogLevel(i8 nLevels)
{
  // create a random 16 bit pattern
  int iRandomPattern = (int)Random::iRandom(0,256*256-1);
  
  int iBitsLeft = (int)nLevels / 2;
  
  int iLevel = 0;
  
  int bBits;
  do {
    bBits = iRandomPattern & 0x3;
    if ( ! bBits )
      iLevel++;
    
    iRandomPattern >>= 2;
    if (--iBitsLeft == 0) {
      iRandomPattern = (int)Random::iRandom(0,256*256-1);
      iBitsLeft = (int)nLevels / 2;
    }
    
  } while ( !bBits );
  
  i8 iLogLevel = (iLevel > nLevels - 1) ? nLevels - 1 : iLevel;
  
  return iLogLevel;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
