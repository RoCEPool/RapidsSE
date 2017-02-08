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
  vAddPrimitive(">binval",     &vToBinVal,     false);
  vAddPrimitive(">body",       &vToBody,       false);
  vAddPrimitive(">cmd",        &vToCmd,        false);
  vAddPrimitive(">digitName",  &vToDigitName,  false);
  vAddPrimitive(">int",        &vToInt,        false);
  vAddPrimitive(">object",     &vToObject,     false);
  vAddPrimitive(">seitf",      &vToSEitf,      false);
  vAddPrimitive(">str",        &vToStr,        false);
  vAddPrimitive(">task",       &vToTask,       false);
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
  
  char* pBody = pcAdd(pWord,sizeof(Word));
  
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
      String::vConcat(sCmd.a()," ");
    
    xValWsp.vToCommand(sCmd.z());
  }
  
  xStr = sCmd.a();
  
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
  xStr.vToString(sBuff.a());
  
  i8 i = String::iToI8(sBuff.a());
  
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
  xVal.vToString(sString.a());
  
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
