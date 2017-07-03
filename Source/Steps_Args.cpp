/*============================================================================*/
/* Steps_Args.cpp - args step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
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
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// args: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Args(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitive("args.print"_Str,      &vArgsPrint,     false);
}

/*----------------------------------------------------------------------------*/
/* args.print -- print arguments                                              */
/*----------------------------------------------------------------------------*/
void Task::vArgsPrint(MP_Task mpTask)
{
ENTRY("vArgsPrint"_Str);
  
  gxArguments.vPrint();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
