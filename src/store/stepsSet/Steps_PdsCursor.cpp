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
  vAddStep("PdsCursor[].new"_Stz  ,               &Step::vPdsCursorNew);
  vAddStep("PdsCursor[].next"_Stz,                &Step::vPdsCursorNext);
  vAddStep("PdsCursor[].row"_Stz  ,               &Step::vPdsCursorRow);
  vAddStep("PdsCursor[].set"_Stz  ,               &Step::vPdsCursorSet);
  vAddStep("PdsCursor[].delete"_Stz  ,             &Step::vPdsCursorDelete);
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
  m_oDataStack.vPushPtr((Object*)pSLBlock->aItem());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsCursor[] -- get pds cursor                                              */
/*----------------------------------------------------------------------------*/
void Step::vPdsCursorGet(void)
{
  ENTRY();

  m_oDataStack.vPushPtr(*m_pLogicUnit->m_xpPdsCursor);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsCursor[].currIsNotNull -- is the current block not null                 */
/*----------------------------------------------------------------------------*/
void Step::vPdsCursorCurrIsNotNull(void)
{
  ENTRY();

  m_oDataStack.vPushInt(m_pLogicUnit->m_xpPdsCursor->bIsCurrNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsCursor[].new -- new a pds cursor                                        */
/*----------------------------------------------------------------------------*/
void Step::vPdsCursorNew(void)
{
  ENTRY();

  Pds* pPds = _pPds(*m_pLogicUnit->m_xpPds);
  RASSERT(pPds != nullptr);
  PdsCursor* pCursor = new PdsCursor(pPds);

  m_pLogicUnit->m_xpPdsCursor = pCursor;

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
/* PdsCursor[].row -- get current row in the pds                              */
/*----------------------------------------------------------------------------*/
void Step::vPdsCursorRow(void)
{
  ENTRY();

  PdsCursor* pPdsCursor = _pPdsCursor(*m_pLogicUnit->m_xpPdsCursor);
  SkipListBlock* pSLBlock = pPdsCursor->m_pCurrentBlock;
  RASSERT(pSLBlock != nullptr);
  m_oDataStack.vPushPtr((Row*)pSLBlock->aRow());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsCursor[].set -- set the cursor                                          */
/*----------------------------------------------------------------------------*/
void Step::vPdsCursorSet(void)
{
  ENTRY();

  m_pLogicUnit->m_xpPdsCursor = _pPdsCursor(m_oDataStack.pPopPtr(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsCursor[].delete -- delete the cursor                                    */
/*----------------------------------------------------------------------------*/
void Step::vPdsCursorDelete(void)
{
  ENTRY();

  m_pLogicUnit->m_xpPdsCursor->m_pPds = nullptr;
  m_pLogicUnit->m_xpPdsCursor.vDeleteComponent();

  EXIT();
}
