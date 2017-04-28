/*============================================================================*/
/* Steps_To.cpp - to step group                                               */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// to: /////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_To(void)
{
  vAddPrimitive(">binval"_Str,     &vToBinVal,     false);
  vAddPrimitive(">body"_Str,       &vToBody,       false);
  vAddPrimitive(">digitName"_Str,  &vToDigitName,  false);
  vAddPrimitive(">int"_Str,        &vToInt,        false);
  vAddPrimitive(">object"_Str,     &vToObject,     false);
  vAddPrimitive(">ptr"_Str,        &vToPtr,        false);
  vAddPrimitive(">seitf"_Str,      &vToSEitf,      false);
  vAddPrimitive(">str"_Str,        &vToStr,        false);
  vAddPrimitive(">task"_Str,       &vToTask,       false);
}

/*----------------------------------------------------------------------------*/
/* >binval -- change object to binval                                         */
/*----------------------------------------------------------------------------*/
void Task::vToBinVal(NNP_Task nnpTask)
{
  ENTRY("vToBinVal"_Str);
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_RSTACK(">BinVal"_Str);
  
  nnpTask->m_pDataStack->vPushBinary(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >body -- convert word pointer to body pointer                              */
/*----------------------------------------------------------------------------*/
void Task::vToBody(NNP_Task nnpTask)
{
  ENTRY("vToBody"_Str);
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK(">body"_Str);
  Word* pWord = _pWord(pWordObject);
  
  //  char* pBody = pAdd<char*>(pWord,sizeof(Word));
  char* pBody = pWord->aBodyArea();
  
  Object* pPtrBody = reinterpret_cast<Object*>(pBody);
  
  nnpTask->m_pDataStack->vPushObject(pPtrBody);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >int -- convert value to int                                               */
/*----------------------------------------------------------------------------*/
void Task::vToInt(NNP_Task nnpTask)
{
  ENTRY("vToInt"_Str);
  
  s32 sInt;
  if ( ! nnpTask->m_pDataStack->bPop(sInt) )
    THROW_INVALID_DSTACK(">int"_Str);
  
  i8 i = sInt.iToI8();
  
  nnpTask->m_pDataStack->vPushInt(i);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >object -- change binval to object                                         */
/*----------------------------------------------------------------------------*/
void Task::vToObject(NNP_Task nnpTask)
{
  ENTRY("vToObject"_Str);
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPopBinary(pObject) )
    THROW_INVALID_RSTACK(">Object"_Str);
  
  nnpTask->m_pDataStack->vPushObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >ptr -- convert int to ptr                                                 */
/*----------------------------------------------------------------------------*/
void Task::vToPtr(NNP_Task nnpTask)
{
  ENTRY("vToPtr"_Str);
  
  i8 iPtr;
  if ( ! nnpTask->m_pDataStack->bPop(iPtr) )
    THROW_INVALID_DSTACK(">str"_Str);
  
  Object* pPtr = (Object*)iPtr;
  
  nnpTask->m_pDataStack->vPushObject(pPtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >seitf -- convert int to seitf ptr                                         */
/*----------------------------------------------------------------------------*/
void Task::vToSEitf(NNP_Task nnpTask)
{
  ENTRY("vToSEitf"_Str);
  
  i8 iVal;
  
  if ( ! nnpTask->m_pDataStack->bPop(iVal) )
    THROW_INVALID_DSTACK(">seitf"_Str);
  
  SEitf* pSEitf = _pSEitf(iVal);
  RASSERT(pSEitf->eClass() == CN_SEITF);
  
  Object* pPtrSEitf = pSEitf;
  
  nnpTask->m_pDataStack->vPushObject(pPtrSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >str -- convert value to str                                               */
/*----------------------------------------------------------------------------*/
void Task::vToStr(NNP_Task nnpTask)
{
  ENTRY("vToStr"_Str);
  
  ValWsp xVal;
  if ( ! nnpTask->m_pDataStack->bPop(xVal) )
    THROW_INVALID_DSTACK(">str"_Str);
  
  s48K sString;
  ValLib::vToString(xVal.pNestedValueSelf(),sString);
  
  nnpTask->m_pDataStack->vPushS48K(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >task -- convert int to task ptr                                           */
/*----------------------------------------------------------------------------*/
void Task::vToTask(NNP_Task nnpTask)
{
  ENTRY("vToTask"_Str);
  
  i8 iVal;
  if ( ! nnpTask->m_pDataStack->bPop(iVal) )
    THROW_INVALID_DSTACK(">task"_Str);
  
  Object* pPtrTask = _pTask(iVal);
  
  nnpTask->m_pDataStack->vPushObject(pPtrTask);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
