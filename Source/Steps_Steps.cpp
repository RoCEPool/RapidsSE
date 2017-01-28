/*============================================================================*/
/* Steps_Steps.cpp - steps step group                                         */
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

// steps: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Steps(void)
{
  vAddPrimitive("steps.list",  &vStepsList,  false);
  vAddPrimitive("steps.reset", &vStepsReset, false);
}

/*----------------------------------------------------------------------------*/
/* steps.list -- list steps                                                   */
/*----------------------------------------------------------------------------*/
void Task::vStepsList(NNP_Task nnpTask)
{
ENTRY("vStepsList");

  gpStepArray->vList();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* steps.reset -- reset steps                                                 */
/*----------------------------------------------------------------------------*/
void Task::vStepsReset(NNP_Task nnpTask)
{
  ENTRY("vStepsReset");
  
  gpStepArray->vReset();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
