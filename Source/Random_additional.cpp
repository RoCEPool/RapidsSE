/*===========================================================================*/
/* Random.cpp -- random number interface                                     */
/*===========================================================================*/

#include "Standard.h"
#include "Platform.h"

#include "Random.h"
#include "Mersenne.h"
#include "Lock.h"

namespace UL {

/*---------------------------------------------------------------------------*/
/* iRandom: generate a random number                                         */
/*---------------------------------------------------------------------------*/
inw Random::iRandom(void)
{
  inw i64 = genrand64_int63();
  
  return i64;
}

/*---------------------------------------------------------------------------*/
/* iRandom: generate a random number between low and high (inclusive)        */
/*---------------------------------------------------------------------------*/
inw Random::iRandom(inw iLow, inw iHigh)
{
  inw r = genrand64_int63();
  
  inw m = iHigh - iLow + 1;
  inw v = r % m;
  inw n = v + iLow;

  assert (n >= iLow);
  assert (n <= iHigh);
  
  return n;
}

/*---------------------------------------------------------------------------*/
/* uRandom: generate a random number                                         */
/*---------------------------------------------------------------------------*/
uiw Random::uRandom(void)
{
  uiw u64 = genrand64_int64();
  
  return u64;
}
  
//---------------------------------------------------------------------------//
// iLogLevel: choose a random log level                                      //
//---------------------------------------------------------------------------//
inw Random::iLogLevel(inw nLevels)
{
  int iRandomBits   = (int)Random::iRandom(0,256*256-1);
  int _BitsInRandom = (int)nLevels;
  int iRandomsLeft  = _BitsInRandom/2;
  
  int level = 0;
  int b;
  
  do {
    b = iRandomBits & 3;
    if (!b) level++;
    iRandomBits>>=2;
    if (--iRandomsLeft == 0) {
      iRandomBits = (int)Random::iRandom(0,256*256-1);
      iRandomsLeft = _BitsInRandom/2;
    }
  } while (!b);
  
  return (level > nLevels - 1) ? nLevels - 1: level;
}
  
//---------------------------------------------------------------------------//
// nLevelLog: choose a random number of levels (log distribution)            //
//---------------------------------------------------------------------------//
inw Random::nLevelLog(inw nLevels)
{
  const inw NBITS = 3;
  const uiw XAND  = 0x7;
  
  uiw uBitPattern = uRandom();
  inw nBitsLeft   = C_bytesPerWord;
  
  inw nChosen = 1;
  
  for (;;) {
    
    uiw uBits = uBitPattern & XAND;
    
    if (uBits)
      break;
    
    nChosen++;
  
    uBitPattern >>= NBITS;
    nBitsLeft -= NBITS;
    
    if (nBitsLeft < NBITS) {
      uBitPattern = uRandom();
      nBitsLeft   = C_bytesPerWord;
    }
    
  }
  
  return (nChosen > nLevels) ? nLevels : nChosen;
}
  
}

/*===========================================================================*/
/* end of file                                                               */
/*===========================================================================*/