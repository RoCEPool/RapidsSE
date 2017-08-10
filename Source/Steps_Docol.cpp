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

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// docol: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Docol(void)
{
  vAddPrimitive("]"_Stz,                     &vDocolCompiling,      false);
  vAddPrimitive("["_Stz,                     &vDocolInterpreting,   true );
  vAddPrimitive("docol.isCompiling"_Stz,     &vDocolIsCompiling,    false);
  vAddPrimitive("docol.isInterpreting"_Stz,  &vDocolIsInterpreting, false);
}

/*----------------------------------------------------------------------------*/
/* ] -- enter compiler state                                                  */
/*----------------------------------------------------------------------------*/
void Task::vDocolCompiling(MP_Task mpTask)
{
  ENTRY("vDocolCompiling"_Stz);
  
  mpTask->m_bCompiling = true;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ -- enter interpreter state                                               */
/*----------------------------------------------------------------------------*/
void Task::vDocolInterpreting(MP_Task mpTask)
{
  ENTRY("vDocolInterpreting"_Stz);
  
  mpTask->m_bCompiling = false;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol.isCompiling -- currently in compiling mode?                          */
/*----------------------------------------------------------------------------*/
void Task::vDocolIsCompiling(MP_Task mpTask)
{
  ENTRY("vDocolIsCompiling"_Stz);
  
  i8 iIsCompiling = mpTask->m_bCompiling ? 1 : 0;
  
  mpTask->m_pDataStack->vPushInt(iIsCompiling);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* docol.isInterpreting -- currently in interpreting mode?                    */
/*----------------------------------------------------------------------------*/
void Task::vDocolIsInterpreting(MP_Task mpTask)
{
  ENTRY("vDocolIsInterpreting"_Stz);
  
  i8 iIsInterpreting = mpTask->m_bCompiling ? 0 : 1;
  
  mpTask->m_pDataStack->vPushInt(iIsInterpreting);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
