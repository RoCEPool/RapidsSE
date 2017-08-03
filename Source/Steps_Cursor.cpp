/*============================================================================*/
/* Steps_Cursor.cpp - cursor steps group                                      */
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

// cursor: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Cursor(void)
{
  vAddPrimitive("cursor@"_Stz,         &vCursorFetch,      false);
  vAddPrimitive("cursor!"_Stz,         &vCursorStore,      false);
}

/*----------------------------------------------------------------------------*/
/* cursor@ -- fetch cursor from return stack onto data stack                  */
/*----------------------------------------------------------------------------*/
void Task::vCursorFetch(MP_Task mpTask)
{
  ENTRY("vCursorFetch"_Stz);
  
  i8 iPeek = 1;
  
  ValWrk xCursorWsp;
  
  mpTask->m_pReturnStack->vPeekValWrk(iPeek,xCursorWsp);
  
  mpTask->m_pDataStack->vPushValWrk(xCursorWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cursor! -- replace cursor on return stack from data stack                  */
/*----------------------------------------------------------------------------*/
void Task::vCursorStore(MP_Task mpTask)
{
  ENTRY("vCursorStore"_Stz);
  
  i8 iPoke = 1;
  
  ValWrk xCursorWsp;
  
  if ( ! mpTask->m_pDataStack->bPop(xCursorWsp) )
    THROW_INVALID_DSTACK("cursor!"_Stz);
  
  mpTask->m_pReturnStack->vPokeValWrk(iPoke,xCursorWsp);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
