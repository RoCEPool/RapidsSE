/*============================================================================*/
/* Steps_Steps.cpp - steps step group                                         */
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
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// steps: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Steps(void)
{
  vAddPrimitive("steps::list"_Stz,  &vStepsList,  false);
  vAddPrimitive("steps::reset"_Stz, &vStepsReset, false);
}

/*----------------------------------------------------------------------------*/
/* steps::list -- list steps                                                  */
/*----------------------------------------------------------------------------*/
void Task::vStepsList(MP_Task mpTask)
{
ENTRY("vStepsList"_Stz);

  File xFile;
  xFile.bOpen("steps.csv"_Stz, C_OPEN_WRITE);
  
  gpStepArray->vList(xFile);
  
  xFile.vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* steps::reset -- reset steps                                               */
/*----------------------------------------------------------------------------*/
void Task::vStepsReset(MP_Task mpTask)
{
  ENTRY("vStepsReset"_Stz);
  
  gpStepArray->vReset();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
