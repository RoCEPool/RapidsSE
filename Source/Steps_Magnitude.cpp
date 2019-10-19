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
void Step::vToDigitName(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vToDigitName"_Stz);
  
  i8 iDigit;
  
  // get the arguments
  if ( ! pForth->m_pDataStack->bPop(iDigit) )
    THROW_DSTACK();
  
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
  
  pForth->m_pDataStack->vPushS32(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bytes.magnitude -- return magnitude of bytes                               */
/*----------------------------------------------------------------------------*/
void Step::vBytesMagnitude(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vBytesMagnitude"_Stz);
  
  i8 iBytes;
  if ( ! pForth->m_pDataStack->bPop(iBytes) )
    THROW_DSTACK();
  
  s256 sMag;
  MagLib::vByteMag(iBytes, sMag);
  
  pForth->m_pDataStack->vPushS256(sMag);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* number.magnitude -- get magnitude of number                                */
/*----------------------------------------------------------------------------*/
void Step::vNumberMagnitude(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vNumberMagnitude"_Stz);
  
  i8 iNumber;
  i8 iScale;
  
  if ( ! pForth->m_pDataStack->bPop(iNumber) )
    THROW_DSTACK();
  
  if ( ! pForth->m_pDataStack->bPop(iScale) )
    THROW_DSTACK();
  
  s256 s;
  
  MagLib::vByteMag(iNumber, iScale, s);
  
  pForth->m_pDataStack->vPushS256(s);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
