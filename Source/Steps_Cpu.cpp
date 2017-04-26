/*============================================================================*/
/* Steps_Cpu.cpp - cpu steps group                                            */
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

// cpu: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Cpu(void)
{
  vAddPrimitive("cpu.rdtscp"_Str,      &vCpuRdtscp,        false);
}

/*----------------------------------------------------------------------------*/
/* cpu.rdtscp -- read cpu time stamp counter                                  */
/*----------------------------------------------------------------------------*/
void Task::vCpuRdtscp(NNP_Task nnpTask)
{
  ENTRY("vCpuRdtscp"_Str);
  
  i8 iTscp = Synch::iTicks();
  
  nnpTask->m_pDataStack->vPushInt(iTscp);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
