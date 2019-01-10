/*============================================================================*/
/* Steps_OpFrame.cpp - operator frame step group                              */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_OpFrame(void)
{
  vAddStep("opFrame.dic"_Stz,       &Step::vOpFrameDic);
  vAddStep("opFrame.name"_Stz,      &Step::vOpFrameName);
}

/*----------------------------------------------------------------------------*/
/* opFrame.dic -- push dic of given op frame ( opFrame -- opDic)              */
/*----------------------------------------------------------------------------*/
void Step::vOpFrameDic(MP_Task mpTask)
{
  ENTRY("vOpFrameDic"_Stz);
  
  Object* pPtrOpFrame;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrOpFrame) )
    THROW_INVALID_DSTACK("opFrame.dic"_Stz);
  
  RegFrame* pOpFrame = _pRegFrame(pPtrOpFrame);
  
  Dic* pOpDic = pOpFrame->pOpDic();
  
  mpTask->m_pDataStack->vPushPointer(pOpDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* opFrame.name -- get name of given op frame ( opFrame -- string )           */
/*----------------------------------------------------------------------------*/
void Step::vOpFrameName(MP_Task mpTask)
{
  ENTRY("vOpFrameName"_Stz);
  
  Object* pPtrOpFrame;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrOpFrame) )
    THROW_INVALID_DSTACK("opFrame.dic"_Stz);
  
  RegFrame* pOpFrame = _pRegFrame(pPtrOpFrame);
  
  s32 sName;
  sName = pOpFrame->sName();
  
  mpTask->m_pDataStack->vPushS32(sName);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
