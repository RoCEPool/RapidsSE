/*============================================================================*/
/* Steps_To.cpp - to step group                                               */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_To(void)
{
  vAddStep(">body"_Stz,       &Step::vToBody);
  vAddStep(">digitName"_Stz,  &Step::vToDigitName);
  vAddStep(">int"_Stz,        &Step::vToInt);
  vAddStep(">ptr"_Stz,        &Step::vToPtr);
  vAddStep(">str"_Stz,        &Step::vToStr);
  vAddStep(">task"_Stz,       &Step::vToTask);
}

/*----------------------------------------------------------------------------*/
/* >body -- convert word pointer to body pointer                              */
/*----------------------------------------------------------------------------*/
void Step::vToBody(void)
{
  ENTRY(); // ENTRY("vToBody"_Stz);
  
  Object* pWordObject;
  m_pDataStack->vPopPtr(pWordObject,__func__);
  Word* pWord = _pWord(pWordObject);
  
  //  char* pBody = pAdd<char*>(pWord,sizeof(Word));
  char* pBody = pWord->aWspArea();
  
  Object* pPtrBody = reinterpret_cast<Object*>(pBody);
  
  m_pDataStack->vPushPtr(pPtrBody);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >int -- convert value to int                                               */
/*----------------------------------------------------------------------------*/
void Step::vToInt(void)
{
  ENTRY(); // ENTRY("vToInt"_Stz);
  
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
  ENTRY(); // ENTRY("vToPtr"_Stz);
  
  i8 iPtr; m_pDataStack->vPopInt(iPtr,__func__);
  
  Object* pPtr = (Object*)iPtr;
  
  m_pDataStack->vPushPtr(pPtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >str -- convert value to str                                               */
/*----------------------------------------------------------------------------*/
void Step::vToStr(void)
{
  ENTRY(); // ENTRY("vToStr"_Stz);
  
  GenVar xVar;
  m_pDataStack->vPopVal(xVar,__func__);
  
  s48K sString;
  
  xVar.vToString(sString);
  
  m_pDataStack->vPushStr(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >task -- convert int to task ptr                                           */
/*----------------------------------------------------------------------------*/
void Step::vToTask(void)
{
  ENTRY(); // ENTRY("vToTask"_Stz);
  
  i8 iVal; m_pDataStack->vPopInt(iVal,__func__);
  
  Object* pPtrTask = _pTask(iVal);
  
  m_pDataStack->vPushPtr(pPtrTask);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
