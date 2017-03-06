/*============================================================================*/
/* Steps_To.cpp - to step group                                               */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

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
  vAddPrimitive(">cmd"_Str,        &vToCmd,        false);
  vAddPrimitive(">digitName"_Str,  &vToDigitName,  false);
  vAddPrimitive(">int"_Str,        &vToInt,        false);
  vAddPrimitive(">object"_Str,     &vToObject,     false);
  vAddPrimitive(">seitf"_Str,      &vToSEitf,      false);
  vAddPrimitive(">str"_Str,        &vToStr,        false);
  vAddPrimitive(">task"_Str,       &vToTask,       false);
}

/*----------------------------------------------------------------------------*/
/* >binval -- change object to binval                                         */
/*----------------------------------------------------------------------------*/
void Task::vToBinVal(NNP_Task nnpTask)
{
  ENTRY("vToBinVal");
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_RSTACK(">BinVal");
  
  nnpTask->m_pDataStack->vPushBinary(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >body -- convert word pointer to body pointer                              */
/*----------------------------------------------------------------------------*/
void Task::vToBody(NNP_Task nnpTask)
{
  ENTRY("vToBody");
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK(">body");
  Word* pWord = _pWord(pWordObject);
  
  //  char* pBody = pAdd<char*>(pWord,sizeof(Word));
  char* pBody = pWord->aBodyArea();
  
  Object* pPtrBody = reinterpret_cast<Object*>(pBody);
  
  nnpTask->m_pDataStack->vPush(pPtrBody);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >cmd -- convert stack values to cmd string                                 */
/*----------------------------------------------------------------------------*/
void Task::vToCmd(NNP_Task nnpTask)
{
  ENTRY("vToCmd");
  
  i8 nValues;
  if ( ! nnpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK(">cmd");
  
  s48K sCmd;
  
  ValWsp xStr;
  xStr = "<cmd>";
  
  for (i8 i = 0; i < nValues; i++) {
    ValWsp xValWsp;
    nnpTask->m_pDataStack->vPeek(i,&xValWsp);
    
    if (i)
      StrLib::vConcat(sCmd.aStr()," ");
    
    char* aCmdEnd = StrLib::zTerminator(sCmd.aStr());
    
    ValLib::vToCommand(&xValWsp,aCmdEnd);
  }
  
  xStr = sCmd.aStr();
  
  nnpTask->m_pDataStack->vPush(&xStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >int -- convert value to int                                               */
/*----------------------------------------------------------------------------*/
void Task::vToInt(NNP_Task nnpTask)
{
  ENTRY("vToInt");
  
  ValWsp xStr;
  if ( ! nnpTask->m_pDataStack->bPop(xStr) )
    THROW_INVALID_DSTACK(">int");
  
  s256 sBuff;
  ValLib::vToString(&xStr,sBuff.aStr());
  
  i8 i = StrLib::iToI8(sBuff.aStr());
  
  nnpTask->m_pDataStack->vPush(i);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >object -- change binval to object                                         */
/*----------------------------------------------------------------------------*/
void Task::vToObject(NNP_Task nnpTask)
{
  ENTRY("vToObject");
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPopBinary(pObject) )
    THROW_INVALID_RSTACK(">Object");
  
  nnpTask->m_pDataStack->vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >seitf -- convert int to seitf ptr                                         */
/*----------------------------------------------------------------------------*/
void Task::vToSEitf(NNP_Task nnpTask)
{
  ENTRY("vToSEitf");
  
  i8 iVal;
  
  if ( ! nnpTask->m_pDataStack->bPop(iVal) )
    THROW_INVALID_DSTACK(">seitf");
  
  SEitf* pSEitf = _pSEitf(iVal);
  RASSERT(pSEitf->eClass() == CN_SEITF);
  
  Object* pPtrSEitf = pSEitf;
  
  nnpTask->m_pDataStack->vPush(pPtrSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >str -- convert value to str                                               */
/*----------------------------------------------------------------------------*/
void Task::vToStr(NNP_Task nnpTask)
{
  ENTRY("vToStr");
  
  ValWsp xVal;
  if ( ! nnpTask->m_pDataStack->bPop(xVal) )
    THROW_INVALID_DSTACK(">str");
  
  s48K sString;
  ValLib::vToString(&xVal,sString.aStr());
  
  nnpTask->m_pDataStack->vPush(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >task -- convert int to task ptr                                           */
/*----------------------------------------------------------------------------*/
void Task::vToTask(NNP_Task nnpTask)
{
  ENTRY("vToTask");
  
  i8 iVal;
  if ( ! nnpTask->m_pDataStack->bPop(iVal) )
    THROW_INVALID_DSTACK(">task");
  
  Object* pPtrTask = _pTask(iVal);
  
  nnpTask->m_pDataStack->vPush(pPtrTask);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
