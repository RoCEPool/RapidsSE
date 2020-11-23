/*============================================================================*/
/* Steps_RegPath.cpp - path[] step group                                      */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegPath(void)
{
  vAddStep("path[]"_Stz,               &Step::vRegPathGet);
  
  vAddStep("path[].depth"_Stz,         &Step::vRegPathDepth);
  vAddStep("path[].popFolder"_Stz, &Step::vRegPathDiscard);
  vAddStep("path[].dump"_Stz,          &Step::vRegPathDump);
  vAddStep("path[].pick"_Stz,          &Step::vRegPathPick);
  vAddStep("path[].pushFolder"_Stz,    &Step::vRegPathPush);
  vAddStep("path[].save"_Stz,          &Step::vRegPathSave);
  vAddStep("path[].restore"_Stz,       &Step::vRegPathRestore);
}

/*----------------------------------------------------------------------------*/
/* path[] -- push task dictionary onto data stack                             */
/*----------------------------------------------------------------------------*/
void Step::vRegPathGet(void)
{
  ENTRY();
    
  Object* pPtrDicStack = &m_oDicStack;
  
  m_pDataStack->vPushPtr(pPtrDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* path[].depth -- get number of dictionaries on dictionary stack             */
/*----------------------------------------------------------------------------*/
void Step::vRegPathDepth(void)
{
  ENTRY();
  
  i8 iDepth = m_oDicStack.iCount();
  
  m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* path[].popFolder -- discard top of dictionary stack                          */
/*----------------------------------------------------------------------------*/
void Step::vRegPathDiscard(void)
{
  ENTRY();
  
  m_oDicStack.vDiscard();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* path[].dump -- dump dictionary stack                                       */
/*----------------------------------------------------------------------------*/
void Step::vRegPathDump(void)
{
  ENTRY();
  
  m_oDicStack.vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* path[].pick -- pick a dic from dic stack and push onto data stack          */
/*----------------------------------------------------------------------------*/
void Step::vRegPathPick(void)
{
  ENTRY();
  
  i8 iDic = m_pDataStack->iPopInt(__func__);
  
  Object* pPtrDic = m_oDicStack.pPick(iDic);

  m_pDataStack->vPushPtr(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* path[].pushFolder -- push a dictionary onto given dictionary stack         */
/*----------------------------------------------------------------------------*/
void Step::vRegPathPush(void)
{
  ENTRY();
  
  Dic* pDic = _pDic(m_pDataStack->pPopPtr(__func__));
  
  m_oDicStack.vPush(pDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* path[].save -- save path onto save stack                                   */
/*----------------------------------------------------------------------------*/
void Step::vRegPathSave(void)
{
  ENTRY();
  
  m_oSaveDicStack.vCopy(&m_oDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* path[].restore -- retore path from save stack                              */
/*----------------------------------------------------------------------------*/
void Step::vRegPathRestore(void)
{
  ENTRY();
  
  m_oDicStack.vCopy(&m_oSaveDicStack);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
