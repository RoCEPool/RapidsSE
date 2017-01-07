/*============================================================================*/
/* Task_Comment.cpp - task comment group                                      */
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

// comment: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Comment(void)
{
  vAddPrimitive("(",               &vComment,          true );
}

/*----------------------------------------------------------------------------*/
/* comment -- comment                                                         */
/*----------------------------------------------------------------------------*/
void Task::vComment(NNP_Task nnpTask)
{
  ENTRY("vComment");
  
  for (;;) {
    nnpTask->vTokenNext();
    if (String::bEqualTo(")",nnpTask->m_sToken.a()))
      break;
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
