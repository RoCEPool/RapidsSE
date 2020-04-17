/*============================================================================*/
/* Steps_Magnitude.cpp - magnitude step group                                 */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Magnitude(void)
{
  vAddStep("toDigitName"_Stz,     &Step::vToDigitName);
  vAddStep("bytes.magnitude"_Stz, &Step::vBytesMagnitude);
  vAddStep("number.magnitude"_Stz, &Step::vNumberMagnitude);
}

/*----------------------------------------------------------------------------*/
/* >DigitName -- convert to digit name                                        */
/*----------------------------------------------------------------------------*/
void Step::vToDigitName(void)
{
  ENTRY();
  
  // get the arguments
  i8 iDigit = m_pDataStack->iPopInt(__func__);
  
  s32 sName;
  
  switch (iDigit) {
    case 0: sName = "zero"_Stz;    break;
    case 1: sName = "one"_Stz;     break;
    case 2: sName = "two"_Stz;     break;
    case 3: sName = "three"_Stz;   break;
    case 4: sName = "four"_Stz;    break;
    case 5: sName = "five"_Stz;    break;
    case 6: sName = "six"_Stz;     break;
    case 7: sName = "seven"_Stz;   break;
    case 8: sName = "eight"_Stz;   break;
    case 9: sName = "nine"_Stz;    break;
    default:sName = "invalid"_Stz; break;
  }
  
  m_pDataStack->vPushStr(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bytes.magnitude -- return magnitude of bytes                               */
/*----------------------------------------------------------------------------*/
void Step::vBytesMagnitude(void)
{
  ENTRY();
  
  i8 iBytes = m_pDataStack->iPopInt(__func__);
  
  s256 sMag;
  MagLib::vByteMag(iBytes, sMag);
  
  m_pDataStack->vPushStr(sMag);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* number.magnitude -- get magnitude of number                                */
/*----------------------------------------------------------------------------*/
void Step::vNumberMagnitude(void)
{
  ENTRY();
  
  i8 iNumber = m_pDataStack->iPopInt(__func__);
  
  i8 iScale = m_pDataStack->iPopInt(__func__);
  
  s256 s;
  
  MagLib::vByteMag(iNumber, iScale, s);
  
  m_pDataStack->vPushStr(s);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
