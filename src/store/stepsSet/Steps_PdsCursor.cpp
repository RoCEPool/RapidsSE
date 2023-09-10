/*============================================================================*/
/* Steps_PdsCursor.cpp -- Cursor for the Pds                                  */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PdsCursor(void)
{
  vAddStep("PdsCursor[]"_Stz,                     &Step::vPdsCursorGet);

  vAddStep("PdsCursor[].getItemAddr"_Stz,         &Step::vPdsCursorItemAddr);
  vAddStep("PdsCursor[].currIsNotNull"_Stz  ,     &Step::vPdsCursorCurrIsNotNull);
  vAddStep("PdsCursor[].next"_Stz,                &Step::vPdsCursorNext);
  vAddStep("PdsCursor[].set"_Stz  ,               &Step::vPdsCursorSet);
}
/*----------------------------------------------------------------------------*/
/* PdsCursor[].getItemAddr -- get the start addr of the item in this pds block */
/*----------------------------------------------------------------------------*/
void Step::vPdsCursorItemAddr(void)
{
  ENTRY();

  PdsCursor* pPdsCursor = _pPdsCursor(*m_pLogicUnit->m_xpPdsCursor);
  SkipListBlock* pSLBlock = pPdsCursor->m_pCurrentBlock;
  RASSERT(pSLBlock != nullptr);
  m_pDataStack->vPushPtr((Object*)pSLBlock->aItem());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsCursor[] -- get pds cursor                                              */
/*----------------------------------------------------------------------------*/
void Step::vPdsCursorGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpPdsCursor);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsCursor[].currIsNotNull -- is the current block not null                 */
/*----------------------------------------------------------------------------*/
void Step::vPdsCursorCurrIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPdsCursor->bIsCurrNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsCursor[].next -- get next block in the pds                              */
/*----------------------------------------------------------------------------*/
void Step::vPdsCursorNext(void)
{
  ENTRY();

  PdsCursor* pPdsCursor = _pPdsCursor(*m_pLogicUnit->m_xpPdsCursor);
  RASSERT(pPdsCursor != nullptr);
  pPdsCursor->vNext();

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* PdsCursor[].set -- set the cursor                                          */
/*----------------------------------------------------------------------------*/
void Step::vPdsCursorSet(void)
{
  ENTRY();

  m_pLogicUnit->m_xpPdsCursor = _pPdsCursor(m_pDataStack->pPopPtr(__func__));

  EXIT();
}
