/*============================================================================*/
/* Estimator.cpp -- scan tool                                                    */
/*============================================================================*/

#include "Estimator.hpp"
#include "Resolver.hpp"

#include "PermGen.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Estimator, CN_ESTIMATOR)
__aNewFunction(Estimator, CN_ESTIMATOR)
__vDeleteFunction(Estimator, CN_ESTIMATOR)
__aFooterFunction(Estimator)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Estimator::Estimator(Bench* pBench)
: TransformerTool(CN_ESTIMATOR, pBench)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_ESTIMATOR, _FOOTER_);

  _checkInitialised_(CN_ESTIMATOR);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_ESTIMATOR, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Estimator::~Estimator(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_ESTIMATOR, _FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Estimator::vSetup_costEst(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_ESTIMATOR, _FOOTER_);

  vSetupCostEstBoard();

  m_pBoard_costEst->vDuplicateBoard(m_pBoard_permute);

  while ((m_pMainCard = m_pBoard_costEst->pGetMain(0))) {

    if (CK_PERMUTATION == m_pMainCard->eCard())
      break;

    m_pBoard_costEst->vRemoveMainCard(m_pMainCard);
    delete m_pMainCard;
  }


  if (m_bInteractive)
    m_pBoard_costEst->vPrintBoard("Estimator::vSetup_costEst"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Estimator::vEstimate_ptndJoins(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_ESTIMATOR, _FOOTER_);

  m_p_tableList =
      _pCard_TableList(m_pBoard_partition->pFindMain(CK_TABLELIST));


  BoardIter xBI(m_pBoard_costEst);

  while (m_p_ptndJoin = _pCard_PtndJoin(xBI.pEachMain(CK_PTNDJOIN)))
    m_p_ptndJoin->vJoinEst(m_p_tableList);

  if (m_bInteractive)
    m_pBoard_costEst->vPrintBoard("Estimator::vEstimate_ptndJoins"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Estimator::vEstimate_network(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_ESTIMATOR, _FOOTER_);


  BoardIter xBI(m_pBoard_costEst);

//  while (m_p_ptndJoin = _pCard_PtndJoin(xBI.pEachMain(CK_PTNDJOIN)))
//    m_p_ptndJoin->vJoinEst(m_p_tableList);

  if (m_bInteractive)
    m_pBoard_costEst->vPrintBoard("Estimator::vEstimate_network"_Stz);
}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
