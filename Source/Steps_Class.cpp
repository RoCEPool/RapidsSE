/*============================================================================*/
/* Steps_Class.cpp - class steps group                                        */
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

// class: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Class(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitive("class.name"_Str,      &vClassName,    false);
}

/*----------------------------------------------------------------------------*/
/* class.name -- translate class no to name                                   */
/*----------------------------------------------------------------------------*/
void Task::vClassName(NNP_Task nnpTask)
{
  ENTRY("vClassName"_Str);
  
  i8 iClassNo;
  if ( ! nnpTask->m_pDataStack->bPop(iClassNo) )
    THROW_INVALID_DSTACK("class.name"_Str);
  
  s32 sClassName;
  
  sClassName = aCN[iClassNo];
  
  nnpTask->m_pDataStack->vPushS32(sClassName);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
