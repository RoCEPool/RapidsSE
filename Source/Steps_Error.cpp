/*============================================================================*/
/* Steps_Error.cpp - error step group                                         */
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
  if ( ! nnpTask->m_pDataStack->bPop(iErrorCode) )
    THROW_INVALID_DSTACK("error.msg");
  
  if (iErrorCode < EC_NONE || iErrorCode > EC_ENUMS)
    THROW("error.msg");

  s256 sErrorMsg;
  sErrorMsg = aEC[iErrorCode]->a();
  
  nnpTask->m_pDataStack->vPush(sErrorMsg);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
