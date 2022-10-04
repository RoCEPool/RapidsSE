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

//  while (m_p_ptndJoin = _pCard_PtndJoin(xBI.pEachMain(CK_PTNDJOIN))) {
  while ((m_pMainCard = xBI.pEachMain())) {
    eCK eCard = m_pMainCard->eCard();
    if (CK_PERMUTATION == eCard) {
      m_p_tableList->vDeletePropertyRecursively(C_CARD_TableUsed);
      continue;
    }
    if (CK_PTNDJOIN != eCard)
      continue;
    m_p_ptndJoin = _pCard_PtndJoin(m_pMainCard);
    m_p_ptndJoin->vJoinEst(m_p_tableList);
  }

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

  i8 nNetwork = 0;
  i8 nSentOut = 0;
  m_p_permutation = nullptr;
  m_p_ptndJoin    = nullptr;

  while ((m_pMainCard = xBI.pEachMain())) {
    eCK eCard = m_pMainCard->eCard();
    if (CK_PERMUTATION == eCard) {
      if (m_p_permutation)
        m_p_permutation->vAddProperty_int(C_CARD_Network, nNetwork);
      m_p_permutation = _pCard_Permutation(m_pMainCard);

      m_p_ptndJoin = nullptr;
      nNetwork = 0;
      nSentOut = 0;
    }
    else
    if (CK_PTNDJOIN == eCard) {
      if (m_p_ptndJoin) {
        nSentOut += m_p_ptndJoin->iGetProperty(C_CARD_JoinedBuff);
        nNetwork += nSentOut;
        m_p_ptndJoin->vAddProperty_int(C_CARD_SentOut, nSentOut);
      }
      m_p_ptndJoin = _pCard_PtndJoin(m_pMainCard);
    }
  }

  if (m_p_permutation)
    m_p_permutation->vAddProperty_int(C_CARD_Network, nNetwork);

  if (m_bInteractive)
    m_pBoard_costEst->vPrintBoard("Estimator::vEstimate_network"_Stz);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Estimator::vEstimate_joinOrder(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_ESTIMATOR, _FOOTER_);

  i8 iLowestNetwork = 999999999;

  BoardIter xBI(m_pBoard_costEst);

  while ((m_p_permutation =
      _pCard_Permutation(xBI.pEachMain(CK_PERMUTATION,m_i_Main)))) {
    i8 iNetwork = m_p_permutation->iGetProperty(C_CARD_Network);
    if (iNetwork >= iLowestNetwork)
      continue;
    m_p_permutation_low = m_p_permutation;
    iLowestNetwork = iNetwork;
  }

  m_pBoard_costEst->vAppendMain((m_p_joinOrderSeq = new Card_JoinOrderSeq));
  m_p_joinOrderSeq->vAddProperty_int(C_CARD_Network,iLowestNetwork);

  xBI.vSet(m_p_permutation_low);
  while ((m_pMainCard = xBI.pEachMain(m_i_Main,m_eMainCard))) {
    if (CK_PTNDJOIN != m_eMainCard)
      break;

    m_p_ptndJoin = _pCard_PtndJoin(m_pMainCard);
    m_p_joinOrderSeq->vExtractJoinOrder(m_p_ptndJoin);
  }

  if (m_bInteractive)
   m_pBoard_costEst->vPrintBoard("Estimator::vEstimate_joinOrder"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Estimator::vExtract_joinOrderSeq(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_ESTIMATOR, _FOOTER_);

  m_pBoard_costEst->vAppendMain((m_p_joinOrderSeq = new Card_JoinOrderSeq));

  BoardIter xBI(m_pBoard_costEst);

  xBI.vSet(m_p_permutation_low);
  while ((m_pMainCard = xBI.pEachMain(m_i_Main,m_eMainCard))) {
    if (CK_PTNDJOIN != m_eMainCard)
      break;

    m_p_ptndJoin = _pCard_PtndJoin(m_pMainCard);
    m_p_joinOrderSeq->vExtractJoinOrder(m_p_ptndJoin);
  }

  if (m_bInteractive)
    m_pBoard_costEst->vPrintBoard("Estimator::vEstimate_joinOrder"_Stz);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
