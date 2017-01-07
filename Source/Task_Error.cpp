/*============================================================================*/
/* Task_groups.cpp - task groups                                              */
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

// error: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Error(void)
{
  vAddPrimitive("error.msg",       &vErrorMsg,         false);
}

/*----------------------------------------------------------------------------*/
/* error.msg -- get error message given the error code                        */
/*----------------------------------------------------------------------------*/
void Task::vErrorMsg(NNP_Task nnpTask)
{
  ENTRY("vErrorMsg");
  
  
  i8 iErrorCode;
  if ( ! nnpTask->m_xDataStack.bPop(iErrorCode) )
    THROW_INVALID_DSTACK("error.msg");
  
  if (iErrorCode < EC_NONE || iErrorCode > EC_ENUMS)
    THROW("error.msg");

  s256 sErrorMsg;
  sErrorMsg = pEC[iErrorCode];
  
  nnpTask->m_xDataStack.vPush(sErrorMsg);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
