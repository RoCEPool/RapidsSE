/*============================================================================*/
/* Steps_Comment.cpp - comment steps group                                    */
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

// comment: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Comment(void)
{
  vAddPrimitive("("_Str,               &vComment,          true );
}

/*----------------------------------------------------------------------------*/
/* comment -- comment                                                         */
/*----------------------------------------------------------------------------*/
void Task::vComment(MP_Task mpTask)
{
  ENTRY("vComment"_Str);
  
  for (;;) {
    mpTask->m_pTokeniser->vNextToken();
    if (mpTask->m_pTokeniser->sToken().bEqual(")"_Str))
      break;
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
