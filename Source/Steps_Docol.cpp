/*============================================================================*/
/* Steps_Docol.cpp - docol step group                                         */
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

// docol: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Docol(void)
{
  vAddPrimitive("]"_Str,                     &vDocolCompiling,      false);
  vAddPrimitive("["_Str,                     &vDocolInterpreting,   true );
  vAddPrimitive("docol.isCompiling"_Str,     &vDocolIsCompiling,    false);
  vAddPrimitive("docol.isInterpreting"_Str,  &vDocolIsInterpreting, false);
}

/*----------------------------------------------------------------------------*/
/* ] -- enter compiler state                                                  */
/*----------------------------------------------------------------------------*/
void Task::vDocolCompiling(MP_Task mpTask)
{
  ENTRY("vDocolCompiling"_Str);
  
  mpTask->m_bCompiling = true;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ -- enter interpreter state                                               */
/*----------------------------------------------------------------------------*/
void Task::vDocolInterpreting(MP_Task mpTask)
{
  ENTRY("vDocolInterpreting"_Str);
  
  mpTask->m_bCompiling = false;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol.isCompiling -- currently in compiling mode?                          */
/*----------------------------------------------------------------------------*/
void Task::vDocolIsCompiling(MP_Task mpTask)
{
  ENTRY("vDocolIsCompiling"_Str);
  
  i8 iIsCompiling = mpTask->m_bCompiling ? 1 : 0;
  
  mpTask->m_pDataStack->vPushInt(iIsCompiling);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol.isInterpreting -- currently in interpreting mode?                    */
/*----------------------------------------------------------------------------*/
void Task::vDocolIsInterpreting(MP_Task mpTask)
{
  ENTRY("vDocolIsInterpreting"_Str);
  
  i8 iIsInterpreting = mpTask->m_bCompiling ? 0 : 1;
  
  mpTask->m_pDataStack->vPushInt(iIsInterpreting);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
