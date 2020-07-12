/*============================================================================*/
/* Steps_Tx.cpp - tx step group                                               */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Tx(void)
{
  vAddStep("tx[].begin"_Stz,       &Step::vTx0Begin);
  vAddStep("tx[].commit"_Stz,      &Step::vTx0Commit);
  vAddStep("tx[].perspective"_Stz, &Step::vTx0Perspective);

  vAddStep("tx::dumpAll"_Stz,      &Step::vTxDumpAll);
}

/*----------------------------------------------------------------------------*/
/* tx[].begin -- begin a new current tx                                       */
/*----------------------------------------------------------------------------*/
void Step::vTx0Begin(void)
{
  ENTRY();
  
  Tx* pTx = new Tx;
    
  m_pLogicUnit->m_xpTx = pTx;
  
  // add tnx to the tnxSet
  AM::pTxSet()->vAddTx(pTx);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx[].commit -- commit current tx                                           */
/*----------------------------------------------------------------------------*/
void Step::vTx0Commit(void)
{
  ENTRY();
  
  Tx* pTx = *m_pLogicUnit->m_xpTx;
  m_pLogicUnit->m_xpTx = nullptr;

  pTx->vPhase(XP_COMMITTED);
  
  // remove the tx from the txSet
  AM::pTxSet()->vRemoveTx(pTx);
  
  delete pTx;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx[].perspective -- get current tx perspective                             */
/*----------------------------------------------------------------------------*/
void Step::vTx0Perspective(void)
{
  ENTRY();
  
  Tx* pTx = *m_pLogicUnit->m_xpTx;
  
  t8 tPerspective = pTx->tPerspective();

  m_pDataStack->vPushInt(tPerspective);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* tx::dumpAll -- dump all tx's                                               */
/*----------------------------------------------------------------------------*/
void Step::vTxDumpAll(void)
{
  ENTRY();
  
  AM::pTxSet()->vDumpAll();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
