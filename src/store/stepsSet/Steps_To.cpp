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
  vAddStep(">bol"_Stz,        &Step::vToBol);
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
  
  m_oDataStack.vPopStr(sBcd, __func__);

  i8 i = sBcd.iToBcd();
  
  m_oDataStack.vPushBcd(i);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >bol -- convert value to bol                                               */
/*----------------------------------------------------------------------------*/
void Step::vToBol(void)
{
  ENTRY();
  
  s32 sBcd;
  
  m_oDataStack.vPopStr(sBcd, __func__);

  i8 i = sBcd.iToBol();
  
  m_oDataStack.vPushBol(i);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >DigitName -- convert to digit name                                        */
/*----------------------------------------------------------------------------*/
void Step::vToDigitName(void)
{
  ENTRY();
  
  // get the arguments
  i8 iDigit = m_oDataStack.iPopInt(__func__);
  
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
  
  m_oDataStack.vPushStr(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >flt -- convert value to flt                                               */
/*----------------------------------------------------------------------------*/
void Step::vToFlt(void)
{
  ENTRY();
  
  s32 sFlt;
  
  m_oDataStack.vPopStr(sFlt, __func__);

  d8 d = sFlt.dToFlt();
  
  m_oDataStack.vPushFlt(d);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >int -- convert value to int                                               */
/*----------------------------------------------------------------------------*/
void Step::vToInt(void)
{
  ENTRY();
  
  s32 sInt;
  
  m_oDataStack.vPopStr(sInt, __func__);

  i8 i = sInt.iToI8();
  
  m_oDataStack.vPushInt(i);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >ptr -- convert int to ptr                                                 */
/*----------------------------------------------------------------------------*/
void Step::vToPtr(void)
{
  ENTRY();
  
  i8 iPtr = m_oDataStack.iPopInt(__func__);
  
  Object* pPtr = (Object*)iPtr;
  
  m_oDataStack.vPushPtr(pPtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >str -- convert value to str                                               */
/*----------------------------------------------------------------------------*/
void Step::vToStr(void)
{
  ENTRY();
  
  SenCell xVar;
  m_oDataStack.vPopSenCell(xVar, __func__);
  
  s48K sString;
  
  xVar.vToString(sString);
  
  m_oDataStack.vPushStr(sString);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
