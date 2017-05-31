/*============================================================================*/
/* IntLib.hpp -- integer utility                                              */
/*============================================================================*/

#include "Base.h"

/*----------------------------------------------------------------------------*/
/* convert i8 to vlq (varying length quantity)                                */
/*----------------------------------------------------------------------------*/
void IntLib::vIntToVlq(i8 iInt, char* aVlq)
{
  i8 nBytes = 0;
  
  bool bNegative = (iInt < 0) ? true : false;
  
  if (bNegative)
    iInt = - iInt;
  
  while (iInt > 31) {
    aVlq[nBytes++] = iInt % 128;
    iInt = iInt / 128;
  }

  iInt |= 0x8;
  if (bNegative)
    iInt|= 0x4;
  
  aVlq[nBytes++] = iInt;
}

/*----------------------------------------------------------------------------*/
/* convert vlq (varying length quantity) to i8                                */
/*----------------------------------------------------------------------------*/
i8 IntLib::iVlqToInt(char* aVlq)
{
  i8 iInt = 0;

  char* aByte;
  for (aByte = aVlq; *aByte < 0x8; aByte++) {
    iInt *= 128;
    iInt += *aByte;
  }
  
  if (*aByte >0x8) {
    iInt = iInt * 128 + (*aByte | !0xC);
    iInt = -iInt;
  }
  else if (*aByte == 0x4) {
    iInt = C_NULL_INT;
  }
  else {
    iInt = iInt * 128 + (*aByte & !0x8);
  }
  
  return iInt;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
