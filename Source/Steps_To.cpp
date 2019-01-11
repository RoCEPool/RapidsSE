/*============================================================================*/
/* Steps_To.cpp - to step group                                               */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
void Step::vToBody(MP_Task mpTask)
{
  ENTRY("vToBody"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK(">body"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  //  char* pBody = pAdd<char*>(pWord,sizeof(Word));
  char* pBody = pWord->aBodyArea();
  
  Object* pPtrBody = reinterpret_cast<Object*>(pBody);
  
  mpTask->m_pDataStack->vPushPointer(pPtrBody);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >int -- convert value to int                                               */
/*----------------------------------------------------------------------------*/
void Step::vToInt(MP_Task mpTask)
{
  ENTRY("vToInt"_Stz);
  
  s32 sInt;
  if ( ! mpTask->m_pDataStack->bPop(sInt) )
    THROW_INVALID_DSTACK(">int"_Stz);
  
  i8 i = sInt.iToI8();
  
  mpTask->m_pDataStack->vPushI8(i);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >ptr -- convert int to ptr                                                 */
/*----------------------------------------------------------------------------*/
void Step::vToPtr(MP_Task mpTask)
{
  ENTRY("vToPtr"_Stz);
  
  i8 iPtr;
  if ( ! mpTask->m_pDataStack->bPop(iPtr) )
    THROW_INVALID_DSTACK(">str"_Stz);
  
  Object* pPtr = (Object*)iPtr;
  
  mpTask->m_pDataStack->vPushPointer(pPtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >str -- convert value to str                                               */
/*----------------------------------------------------------------------------*/
void Step::vToStr(MP_Task mpTask)
{
  ENTRY("vToStr"_Stz);
  
  GenVar xVal;
  if ( ! mpTask->m_pDataStack->bPop(xVal) )
    THROW_INVALID_DSTACK(">str"_Stz);
  
  s48K sString;
  
  xVal.pNestedValue()->vToString(sString);
  
  mpTask->m_pDataStack->vPushS48K(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >task -- convert int to task ptr                                           */
/*----------------------------------------------------------------------------*/
void Step::vToTask(MP_Task mpTask)
{
  ENTRY("vToTask"_Stz);
  
  i8 iVal;
  if ( ! mpTask->m_pDataStack->bPop(iVal) )
    THROW_INVALID_DSTACK(">task"_Stz);
  
  Object* pPtrTask = _pTask(iVal);
  
  mpTask->m_pDataStack->vPushPointer(pPtrTask);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
