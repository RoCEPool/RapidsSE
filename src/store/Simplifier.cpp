/*============================================================================*/
/* Simplifier.cpp --                                                           */
/*============================================================================*/

#include "Simplifier.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Simplifier,CN_SIMPLIFIER)
__aNewFunction(Simplifier,CN_SIMPLIFIER)
__vDeleteFunction(Simplifier,CN_SIMPLIFIER)
__aFooterFunction(Simplifier)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Simplifier::Simplifier(Card_SubSet* pCard_SubSet)
: Object(CN_SIMPLIFIER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SIMPLIFIER,_FOOTER_);

  m_pCard_SubSet = pCard_SubSet;

  _checkInitialised_(CN_SIMPLIFIER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SIMPLIFIER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Simplifier::~Simplifier(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SIMPLIFIER,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* simplify                                                                   */
/*----------------------------------------------------------------------------*/
void Simplifier::vSimplify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SIMPLIFIER,_FOOTER_);

  i8 i_Test;

  i8 nMainCards = m_pCard_SubSet->nSubCards();
  for (i8 i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
    Card *pMainCard = m_pCard_SubSet->pGetSubCard(i_MainCard);

    // of stage 1
    i8 i_Test = i_MainCard;
    Card* pStage_1 = m_pCard_SubSet->pGetSubCard(i_Test++);
    if (CK_STAGE != pStage_1->eCard())
      continue;

    Card* pScan_1 = m_pCard_SubSet->pGetSubCard(i_Test++);
    if (CK_SCAN != pScan_1->eCard())
      continue;

    Card* pSend_1 = m_pCard_SubSet->pGetSubCard(i_Test++);
    if (CK_SEND != pSend_1->eCard())
      continue;

    // of stage 2
    Card* pStage_2 = m_pCard_SubSet->pGetSubCard(i_Test++);
    if (CK_STAGE != pStage_2->eCard())
      continue;

    Card* pScan_2 = m_pCard_SubSet->pGetSubCard(i_Test++);
    if (CK_SCAN != pScan_2->eCard())
      continue;

    Card* pAbsJoin_2 = m_pCard_SubSet->pGetSubCard(i_Test++);
    if (CK_ABSJOIN != pAbsJoin_2->eCard())
      continue;

    Card* pSend_2 = m_pCard_SubSet->pGetSubCard(i_Test++);
    if (CK_SEND != pSend_2->eCard())
      continue;

    // of stage 3
    Card* pStage_3 = m_pCard_SubSet->pGetSubCard(i_Test++);
    if (CK_STAGE != pStage_3->eCard())
      continue;

    Card* pFilter_3 = m_pCard_SubSet->pGetSubCard(i_Test++);
    if (CK_FILTER != pFilter_3->eCard())
      continue;

    Card* pProject_3 = m_pCard_SubSet->pGetSubCard(i_Test++);
    if (CK_PROJECT != pProject_3->eCard())
      continue;

    Card* pCombine_3 = m_pCard_SubSet->pGetSubCard(i_Test++);
    if (CK_COMBINE != pCombine_3->eCard())
      continue;

    Card* pSend_3 = m_pCard_SubSet->pGetSubCard(i_Test++);
    if (CK_SEND != pSend_3->eCard())
      continue;

    m_pCard_SubSet->vRemove(pSend_1);
    m_pCard_SubSet->vRemove(pStage_2);
    m_pCard_SubSet->vRemove(pSend_2);
    m_pCard_SubSet->vRemove(pStage_3);
    m_pCard_SubSet->vRemove(pCombine_3);

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
