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
  if ( ! m_pDataStack->bPopPtr(pWordObject) )
    THROW_DSTACK();
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
  
  if ( ! m_pDataStack->bPopStr(sInt) )
    THROW_DSTACK();
//  if ( ! m_pDataStack->bPopS32(sInt) )
//    THROW_DSTACK();

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
  
  i8 iPtr;
  if ( ! m_pDataStack->bPopInt(iPtr) )
    THROW_DSTACK();
  
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
  
  GenVar xVal;
  if ( ! m_pDataStack->bPopVar(xVal) )
    THROW_DSTACK();
  
  s48K sString;
  
  xVal.aEmbeddedStkVal()->vToString(sString);
  
  m_pDataStack->vPushS48K(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >task -- convert int to task ptr                                           */
/*----------------------------------------------------------------------------*/
void Step::vToTask(void)
{
  ENTRY(); // ENTRY("vToTask"_Stz);
  
  i8 iVal;
  if ( ! m_pDataStack->bPopInt(iVal) )
    THROW_DSTACK();
  
  Object* pPtrTask = _pTask(iVal);
  
  m_pDataStack->vPushPtr(pPtrTask);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
