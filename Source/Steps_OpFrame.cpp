/*============================================================================*/
/* Steps_OpFrame.cpp - operator frame step group                              */
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

// opFrame: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_OpFrame(void)
{
  vAddPrimitive("opFrame.dic"_Str,       &vOpFrameDic,       false);
  vAddPrimitive("opFrame.name"_Str,      &vOpFrameName,      false);
}

/*----------------------------------------------------------------------------*/
/* opFrame.dic -- push dic of given op frame ( opFrame -- opDic)              */
/*----------------------------------------------------------------------------*/
void Task::vOpFrameDic(MP_Task mpTask)
{
  ENTRY("vOpFrameDic"_Str);
  
  Object* pPtrOpFrame;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrOpFrame) )
    THROW_INVALID_DSTACK("opFrame.dic"_Str);
  
  RegFrame* pOpFrame = _pRegFrame(pPtrOpFrame);
  
  Dic* pOpDic = pOpFrame->pOpDic();
  
  mpTask->m_pDataStack->vPushObject(pOpDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* opFrame.name -- get name of given op frame ( opFrame -- string )           */
/*----------------------------------------------------------------------------*/
void Task::vOpFrameName(MP_Task mpTask)
{
  ENTRY("vOpFrameName"_Str);
  
  Object* pPtrOpFrame;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrOpFrame) )
    THROW_INVALID_DSTACK("opFrame.dic"_Str);
  
  RegFrame* pOpFrame = _pRegFrame(pPtrOpFrame);
  
  s32 sName;
  sName = pOpFrame->sName();
  
  mpTask->m_pDataStack->vPushS32(sName);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
