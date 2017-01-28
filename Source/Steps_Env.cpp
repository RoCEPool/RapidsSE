/*============================================================================*/
/* Steps_Env.cpp - env step group                                             */
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

// env: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Env(void)
{
  vAddPrimitive("env@",            &vEnvGet,           false);
}

/*----------------------------------------------------------------------------*/
/* env@ -- get environment variable                                           */
/*----------------------------------------------------------------------------*/
void Task::vEnvGet(NNP_Task nnpTask)
{
  ENTRY("vEnvGet");
  
  s256 sEnvName;
  
  if ( ! nnpTask->m_pDataStack->bPop(sEnvName) )
    THROW_INVALID_DSTACK("env@");
  
  s256 sEnvVal;
  
  if (! Env::bGet(sEnvName,sEnvVal))
    sEnvVal = "";
  
  nnpTask->m_pDataStack->vPush(sEnvVal);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
