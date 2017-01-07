/*============================================================================*/
/* Task_Cursor.cpp - task cursor group                                        */
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

// cursor: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Cursor(void)
{
  vAddPrimitive("cursor@",         &vCursorFetch,      false);
  vAddPrimitive("cursor!",         &vCursorStore,      false);
}

/*----------------------------------------------------------------------------*/
/* cursor@ -- fetch cursor from return stack onto param stack                 */
/*----------------------------------------------------------------------------*/
void Task::vCursorFetch(NNP_Task nnpTask)
{
  ENTRY("vCursorFetch");
  
  i8 iPick = 1;
  
  ValWsp xCursorWsp;
  
  nnpTask->m_xReturnStack.vPeek(iPick,&xCursorWsp);
  
  nnpTask->m_xDataStack.vPush(xCursorWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cursor! -- replace cursor on return stack from param stack                 */
/*----------------------------------------------------------------------------*/
void Task::vCursorStore(NNP_Task nnpTask)
{
  ENTRY("vCursorStore");
  
  i8 iPeek = 1;
  
  ValWsp xCursorWsp;
  
  if ( ! nnpTask->m_xDataStack.bPop(xCursorWsp) )
    THROW_INVALID_DSTACK("cursor!");
  
  nnpTask->m_xReturnStack.vPoke(iPeek,&xCursorWsp);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
