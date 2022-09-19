/*============================================================================*/
/* Curator.cpp --                                                           */
/*============================================================================*/

#include "Curator.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Curator, CN_CURATOR)
__aNewFunction(Curator, CN_CURATOR)
__vDeleteFunction(Curator, CN_CURATOR)
__aFooterFunction(Curator)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Curator::Curator(Bench* pBench)
: TransformerTool(CN_CURATOR, pBench)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CURATOR, _FOOTER_);

  _checkInitialised_(CN_CURATOR);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_CURATOR, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Curator::~Curator(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CURATOR, _FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* simplify                                                                   */
/*----------------------------------------------------------------------------*/
void Curator::vInitial_Simplification(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CURATOR, _FOOTER_);

  i8 i_Test;

  i8 nMainCards = m_pBoard->nMainCards();
  for (i8 i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
    Card *pMainCard = m_pBoard->pGetMainCard(i_MainCard);

    // of stage 1
    i8 i_Test = i_MainCard;
    Card* pStage_1 = m_pBoard->pGetMainCard(i_Test++);
    if (CK_STAGE != pStage_1->eCard())
      continue;

    Card* pScan_1 = m_pBoard->pGetMainCard(i_Test++);
    if (CK_SCAN != pScan_1->eCard())
      continue;

    Card* pSend_1 = m_pBoard->pGetMainCard(i_Test++);
    if (CK_SEND != pSend_1->eCard())
      continue;

    // of stage 2
    Card* pStage_2 = m_pBoard->pGetMainCard(i_Test++);
    if (CK_STAGE != pStage_2->eCard())
      continue;

    Card* pScan_2 = m_pBoard->pGetMainCard(i_Test++);
    if (CK_SCAN != pScan_2->eCard())
      continue;

    Card* pAbsJoin_2 = m_pBoard->pGetMainCard(i_Test++);
    if (CK_ABSJOIN != pAbsJoin_2->eCard())
      continue;

    Card* pSend_2 = m_pBoard->pGetMainCard(i_Test++);
    if (CK_SEND != pSend_2->eCard())
      continue;

    // of stage 3
    Card* pStage_3 = m_pBoard->pGetMainCard(i_Test++);
    if (CK_STAGE != pStage_3->eCard())
      continue;

    Card* pFilter_3 = m_pBoard->pGetMainCard(i_Test++);
    if (CK_FILTER != pFilter_3->eCard())
      continue;

    Card* pProject_3 = m_pBoard->pGetMainCard(i_Test++);
    if (CK_PROJECT != pProject_3->eCard())
      continue;

    Card* pCombine_3 = m_pBoard->pGetMainCard(i_Test++);
    if (CK_COMBINE != pCombine_3->eCard())
      continue;

    Card* pSend_3 = m_pBoard->pGetMainCard(i_Test++);
    if (CK_SEND != pSend_3->eCard())
      continue;

    m_pBoard->vRemoveMainCard(pSend_1);
    m_pBoard->vRemoveMainCard(pStage_2);
    m_pBoard->vRemoveMainCard(pSend_2);
    m_pBoard->vRemoveMainCard(pStage_3);
    m_pBoard->vRemoveMainCard(pCombine_3);

    delete pSend_1;
    delete pStage_2;
    delete pSend_2;
    delete pStage_3;
    delete pCombine_3;

    //m_pCard_SubSet->vRemove(pFilter_3);
    //m_pCard_SubSet->vInsertAfter(pFilter_3,pScan_2);

    break;
  }


}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
