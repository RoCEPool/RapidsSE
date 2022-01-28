/*============================================================================*/
/* Steps_To.cpp - to step group                                               */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_To(void)
{
  vAddStep(">bcd"_Stz,        &Step::vToBcd);
  vAddStep(">digitName"_Stz,  &Step::vToDigitName);
  vAddStep(">flt"_Stz,        &Step::vToFlt);
  vAddStep(">int"_Stz,        &Step::vToInt);
  vAddStep(">ptr"_Stz,        &Step::vToPtr);
  vAddStep(">str"_Stz,        &Step::vToStr);
}

/*----------------------------------------------------------------------------*/
/* >bcd -- convert value to bcd                                               */
/*----------------------------------------------------------------------------*/
void Step::vToBcd(void)
{
  ENTRY();
  
  s32 sBcd;
  
  m_pDataStack->vPopStr(sBcd,__func__);

  i8 i = sBcd.iToBcd();
  
  m_pDataStack->vPushBcd(i);
  
  EXIT();
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
/* >flt -- convert value to flt                                               */
/*----------------------------------------------------------------------------*/
void Step::vToFlt(void)
{
  ENTRY();
  
  s32 sFlt;
  
  m_pDataStack->vPopStr(sFlt,__func__);

  d8 d = sFlt.dToFlt();
  
  m_pDataStack->vPushFlt(d);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >int -- convert value to int                                               */
/*----------------------------------------------------------------------------*/
void Step::vToInt(void)
{
  ENTRY();
  
  s32 sInt;
  
  m_pDataStack->vPopStr(sInt,__func__);

  i8 i = sInt.iToI8();
  
  m_pDataStack->vPushInt(i);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >ptr -- convert int to ptr                                                 */
/*----------------------------------------------------------------------------*/
void Step::vToPtr(void)
{
  ENTRY();
  
  i8 iPtr = m_pDataStack->iPopInt(__func__);
  
  Object* pPtr = (Object*)iPtr;
  
  m_pDataStack->vPushPtr(pPtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >str -- convert value to str                                               */
/*----------------------------------------------------------------------------*/
void Step::vToStr(void)
{
  ENTRY();
  
  CellSingle xVar;
  m_pDataStack->vPopItem(xVar,__func__);
  
  s48K sString;
  
  xVar.vToString(sString);
  
  m_pDataStack->vPushStr(sString);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
