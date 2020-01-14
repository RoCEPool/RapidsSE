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
  ENTRY(); // ENTRY("vTx0Begin"_Stz);
  
  Tx* pTx = new Tx;
    
  m_pRegSet->m_xpTx = pTx;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx[].commit -- commit current tx                                           */
/*----------------------------------------------------------------------------*/
void Step::vTx0Commit(void)
{
  ENTRY(); // ENTRY("vTx0Commit"_Stz);
  
  Tx* pTx = *m_pRegSet->m_xpTx;
  m_pRegSet->m_xpTx = nullptr;

  pTx->vPhase(XP_COMMITTED);

  delete pTx;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx[].perspective -- get current tx perspective                             */
/*----------------------------------------------------------------------------*/
void Step::vTx0Perspective(void)
{
  ENTRY(); // ENTRY("vTx0Perspective"_Stz);
  
  Tx* pTx = *m_pRegSet->m_xpTx;
  
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
  ENTRY(); // ENTRY("vTxDumpAll"_Stz);
  
  AM::pTxSet()->vDumpAll();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
