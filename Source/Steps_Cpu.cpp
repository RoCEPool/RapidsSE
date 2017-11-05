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

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// cpu: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Cpu(void)
{
  vAddPrimitive("cpu.rdtscp"_Stz,      &vCpuRdtscp,        false);
}

/*----------------------------------------------------------------------------*/
/* cpu.rdtscp -- read cpu time stamp counter                                  */
/*----------------------------------------------------------------------------*/
void Task::vCpuRdtscp(MP_Task mpTask)
{
  ENTRY("vCpuRdtscp"_Stz);
  
  i8 iTscp = Synch::iTicks();
  
  mpTask->m_pDataStack->vPushInt(iTscp);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
