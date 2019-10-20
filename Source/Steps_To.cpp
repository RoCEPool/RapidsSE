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
  ENTRY("vToBody"_Stz);
  
  Object* pWordObject;
  if ( ! m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  //  char* pBody = pAdd<char*>(pWord,sizeof(Word));
  char* pBody = pWord->aWspArea();
  
  Object* pPtrBody = reinterpret_cast<Object*>(pBody);
  
  m_pDataStack->vPushPointer(pPtrBody);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >int -- convert value to int                                               */
/*----------------------------------------------------------------------------*/
void Step::vToInt(void)
{
  ENTRY("vToInt"_Stz);
  
  s32 sInt;
  if ( ! m_pDataStack->bPopS32(sInt) )
    THROW_DSTACK();
  
  i8 i = sInt.iToI8();
  
  m_pDataStack->vPushI8(i);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >ptr -- convert int to ptr                                                 */
/*----------------------------------------------------------------------------*/
void Step::vToPtr(void)
{
  ENTRY("vToPtr"_Stz);
  
  i8 iPtr;
  if ( ! m_pDataStack->bPop(iPtr) )
    THROW_DSTACK();
  
  Object* pPtr = (Object*)iPtr;
  
  m_pDataStack->vPushPointer(pPtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >str -- convert value to str                                               */
/*----------------------------------------------------------------------------*/
void Step::vToStr(void)
{
  ENTRY("vToStr"_Stz);
  
  GenVar xVal;
  if ( ! m_pDataStack->bPopGenVar(xVal) )
    THROW_DSTACK();
  
  s48K sString;
  
  xVal.aNestedValue()->vToString(sString);
  
  m_pDataStack->vPushS48K(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >task -- convert int to task ptr                                           */
/*----------------------------------------------------------------------------*/
void Step::vToTask(void)
{
  ENTRY("vToTask"_Stz);
  
  i8 iVal;
  if ( ! m_pDataStack->bPop(iVal) )
    THROW_DSTACK();
  
  Object* pPtrTask = _pTask(iVal);
  
  m_pDataStack->vPushPointer(pPtrTask);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
