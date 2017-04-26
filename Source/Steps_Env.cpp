/*============================================================================*/
/* Steps_Env.cpp - env step group                                             */
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

// env: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Env(void)
{
  vAddPrimitive("env@"_Str,            &vEnvGet,           false);
}

/*----------------------------------------------------------------------------*/
/* env@ -- get environment variable                                           */
/*----------------------------------------------------------------------------*/
void Task::vEnvGet(NNP_Task nnpTask)
{
  ENTRY("vEnvGet"_Str);
  
  s256 sEnvName;
  
  if ( ! nnpTask->m_pDataStack->bPop(sEnvName) )
    THROW_INVALID_DSTACK("env@"_Str);
  
  s256 sEnvVal;
  
  if (! Env::bGet(sEnvName,sEnvVal))
    sEnvVal = ""_Str;
  
  nnpTask->m_pDataStack->vPushS256(sEnvVal);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
