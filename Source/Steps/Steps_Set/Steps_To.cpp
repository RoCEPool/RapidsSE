/*============================================================================*/
/* Steps_To.cpp - to step group                                               */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_To(void)
{
  vAddStep(">digitName"_Stz,  &Step::vToDigitName);
  vAddStep(">int"_Stz,        &Step::vToInt);
  vAddStep(">ptr"_Stz,        &Step::vToPtr);
  vAddStep(">str"_Stz,        &Step::vToStr);
}

/*----------------------------------------------------------------------------*/
/* >int -- convert value to int                                               */
/*----------------------------------------------------------------------------*/
void Step::vToInt(void)
{
  ENTRY();
  
  s32 sInt;
  
  m_pDataStack->vPopStr(sInt,__func__);
//  m_pDataStack->vPopStr(sInt,__func__);

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
  
  Datum xVar;
  m_pDataStack->vPopVal(xVar,__func__);
  
  s48K sString;
  
  xVar.vToString(sString);
  
  m_pDataStack->vPushStr(sString);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
