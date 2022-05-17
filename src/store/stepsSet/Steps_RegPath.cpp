/*============================================================================*/
/* Steps_RegPath.cpp - Path[] step group                                      */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegPath(void)
{
  vAddStep("Path[]"_Stz,               &Step::vRegPathGet);
  
  vAddStep("Path[].depth"_Stz,         &Step::vRegPathDepth);
  vAddStep("Path[].popFolder"_Stz,     &Step::vRegPathDiscard);
  vAddStep("Path[].discardAll"_Stz,    &Step::vRegPathDiscardAll);
  vAddStep("Path[].dump"_Stz,          &Step::vRegPathDump);
  vAddStep("Path[].pick"_Stz,          &Step::vRegPathPick);
  vAddStep("Path[].pushFolder"_Stz,    &Step::vRegPathPush);
  vAddStep("Path[].root"_Stz,          &Step::vRegPathRoot);
  vAddStep("Path[].save"_Stz,          &Step::vRegPathSave);
  vAddStep("Path[].restore"_Stz,       &Step::vRegPathRestore);
}

/*----------------------------------------------------------------------------*/
/* Path[] -- push task dictionary onto data stack                             */
/*----------------------------------------------------------------------------*/
void Step::vRegPathGet(void)
{
  ENTRY();
    
  Object* pPtrDicStack = & m_oDicStack;
  
  m_pDataStack->vPushPtr(pPtrDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Path[].depth -- get number of dictionaries on dictionary stack             */
/*----------------------------------------------------------------------------*/
void Step::vRegPathDepth(void)
{
  ENTRY();
  
  i8 iDepth =  m_oDicStack.nItems();
  
  m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Path[].popFolder -- discard top of dictionary stack                        */
/*----------------------------------------------------------------------------*/
void Step::vRegPathDiscard(void)
{
  ENTRY();
  
   m_oDicStack.vDiscard();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Path[].discardAll -- discard entire dictionary stack                       */
/*----------------------------------------------------------------------------*/
void Step::vRegPathDiscardAll(void)
{
  ENTRY();
  
   m_oDicStack.vDiscardAll();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Path[].dump -- dump dictionary stack                                       */
/*----------------------------------------------------------------------------*/
void Step::vRegPathDump(void)
{
  ENTRY();
  
   m_oDicStack.vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Path[].pick -- pick a dic from dic stack and push onto data stack          */
/*----------------------------------------------------------------------------*/
void Step::vRegPathPick(void)
{
  ENTRY();
  
  i8 iDic = m_pDataStack->iPopInt(__func__);
  
  Object* pPtrDic =  m_oDicStack.pPick(iDic);

  m_pDataStack->vPushPtr(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Path[].pushFolder -- push a dictionary onto given dictionary stack         */
/*----------------------------------------------------------------------------*/
void Step::vRegPathPush(void)
{
  ENTRY();
  
  Dic* pDic = _pDic(m_pDataStack->pPopPtr(__func__));
  
   m_oDicStack.vPush(pDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Path[].root -- set path to root dictionary                                 */
/*----------------------------------------------------------------------------*/
void Step::vRegPathRoot(void)
{
  ENTRY();
  
  while (m_oDicStack.nItems())
    m_oDicStack.vDiscard();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Path[].save -- save path onto save stack                                   */
/*----------------------------------------------------------------------------*/
void Step::vRegPathSave(void)
{
  ENTRY();
  
  m_oSaveDicStack.vCopy(& m_oDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Path[].restore -- retore path from save stack                              */
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
