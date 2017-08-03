/*============================================================================*/
/* Steps_Error.cpp - error step group                                         */
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

// error: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Error(void)
{
  vAddPrimitive("error.msg"_Stz,       &vErrorMsg,         false);
}

/*----------------------------------------------------------------------------*/
/* error.msg -- get error message given the error code                        */
/*----------------------------------------------------------------------------*/
void Task::vErrorMsg(MP_Task mpTask)
{
  ENTRY("vErrorMsg"_Stz);
  
  
  i8 iErrorCode;
  if ( ! mpTask->m_pDataStack->bPop(iErrorCode) )
    THROW_INVALID_DSTACK("error.msg"_Stz);
  
  if (iErrorCode < EC_NONE || iErrorCode > EC_ENUMS)
    THROW("error.msg"_Stz);

  s256 sErrorMsg;
  sErrorMsg = aEC[iErrorCode];
  
  mpTask->m_pDataStack->vPushS256(sErrorMsg);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
