/*============================================================================*/
/* Steps_Cursor.cpp - cursor steps group                                      */
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
  vAddPrimitive("cursor@"_Str,         &vCursorFetch,      false);
  vAddPrimitive("cursor!"_Str,         &vCursorStore,      false);
}

/*----------------------------------------------------------------------------*/
/* cursor@ -- fetch cursor from return stack onto data stack                  */
/*----------------------------------------------------------------------------*/
void Task::vCursorFetch(NNP_Task nnpTask)
{
  ENTRY("vCursorFetch"_Str);
  
  i8 iPick = 1;
  
  ValWsp xCursorWsp;
  
  nnpTask->m_pReturnStack->vPeek(iPick,&xCursorWsp);
  
  nnpTask->m_pDataStack->vPush(xCursorWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cursor! -- replace cursor on return stack from data stack                  */
/*----------------------------------------------------------------------------*/
void Task::vCursorStore(NNP_Task nnpTask)
{
  ENTRY("vCursorStore"_Str);
  
  i8 iPeek = 1;
  
  ValWsp xCursorWsp;
  
  if ( ! nnpTask->m_pDataStack->bPop(xCursorWsp) )
    THROW_INVALID_DSTACK("cursor!"_Str);
  
  nnpTask->m_pReturnStack->vPoke(iPeek,&xCursorWsp);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
