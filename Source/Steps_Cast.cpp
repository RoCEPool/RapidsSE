/*============================================================================*/
/* Steps_Cast.cpp - cast step group                                           */
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

// cast: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Cast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitive("(int)"_Stz,           &vCastInt,          false);
  vAddPrimitive("(ptr)"_Stz,           &vCastPtr,          false);
}

/*----------------------------------------------------------------------------*/
/* (int) -- cast ptr to int                                                   */
/*----------------------------------------------------------------------------*/
void Task::vCastInt(MP_Task mpTask)
{
  ENTRY("vCastInt"_Stz);
  
  Object* pPtr;
  if ( ! mpTask->m_pDataStack->bPop(pPtr) )
    THROW_INVALID_DSTACK("(int)"_Stz);
  
  mpTask->m_pDataStack->vPushInt((i8)pPtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* (ptr) -- cast int to ptr                                                   */
/*----------------------------------------------------------------------------*/
void Task::vCastPtr(MP_Task mpTask)
{
  ENTRY("vCastPtr"_Stz);
  
  i8 iInt;
  if ( ! mpTask->m_pDataStack->bPop(iInt) )
    THROW_INVALID_DSTACK("(ptr)"_Stz);
  
  mpTask->m_pDataStack->vPushObject((Object*)iInt);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
