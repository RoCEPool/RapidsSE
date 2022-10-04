/*============================================================================*/
/* Combiner.cpp --                                                           */
/*============================================================================*/

#include "Combiner.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Combiner, CN_COMBINER)
__aNewFunction(Combiner, CN_COMBINER)
__vDeleteFunction(Combiner, CN_COMBINER)
__aFooterFunction(Combiner)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Combiner::Combiner(Bench* pBench)
: TransformerTool(CN_COMBINER, pBench)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINER, _FOOTER_);

  _checkInitialised_(CN_COMBINER);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_COMBINER, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Combiner::~Combiner(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINER, _FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Combiner::vAdd_combined(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINER, _FOOTER_);

  i8 iAggreg    = 0;
  i8 iIndexJoin = 0;
  i8 iFilter    = 0;
  i8 iOther     = 0;
  i8 iProject   = 0;
  i8 iScan      = 0;
  i8 iSort      = 0;

  BoardIter xBI(m_pBoard);

  i8 nMainCards = m_pBoard->nMainCards();
  while ( (m_pMainCard = xBI.pEachMain()) ) {

    m_pNextMain = xBI.pNextMain();
    if (m_pNextMain)
      m_eNextMain = m_pNextMain->eCard();
    else
      m_eNextMain = CK_NONE;

    switch (eDecodeMain(m_pMainCard)) {

      case CK_AGGREG:
        RASSERT(m_p_aggreg == m_pMainCard);
        iAggreg++;
        m_p_combined = new Card_Combined;
        m_pBoard->vInsertMainCard_after(m_p_combined, m_p_aggreg);
        break;

      case CK_FILTER:
        RASSERT(m_p_filter == m_pMainCard);
        iFilter++;
        m_p_combined = new Card_Combined;
        m_pBoard->vInsertMainCard_after(m_p_combined, m_p_filter);
        break;

      case CK_INDEXJOIN:
        RASSERT(m_p_indexJoin == m_pMainCard);
        iIndexJoin++;
        if (CK_AGGREG == m_eNextMain || CK_FILTER == m_eNextMain || CK_PROJECT == m_eNextMain)
          break;
        m_p_combined = new Card_Combined;
        m_pBoard->vInsertMainCard_after(m_p_combined, m_p_indexJoin);
        break;

      case CK_PROJECT:
        RASSERT(m_p_project == m_pMainCard);
        iProject++;
        m_p_combined = new Card_Combined;
        m_pBoard->vInsertMainCard_after(m_p_combined, m_p_project);
        break;

      case CK_SORT:
        RASSERT(m_p_sort == m_pMainCard);
        iSort++;
        m_p_combined = new Card_Combined;
        m_pBoard->vInsertMainCard_after(m_p_combined, m_p_sort);
        break;

      default:
        iOther++;
        break;
    }
  }

  if (m_bInteractive)
    m_pBoard->vPrintBoard("Combiner::vAdd_combined"_Stz);

}

/*----------------------------------------------------------------------------*/
/* delete combines                                                            */
/*----------------------------------------------------------------------------*/
void Combiner::vDelete_Combines(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINER, _FOOTER_);

  i8 nMainCards = m_pBoard->nMainCards();
  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
    Card* pMainCard = m_pBoard->pGetMain(i_Main);

    if (CK_COMBINED == pMainCard->eCard()) {
      m_pBoard->vRemoveMainCard(pMainCard);
      nMainCards = m_pBoard->nMainCards();
      i_Main--;
      delete pMainCard;
    }
  }

  if (m_bInteractive)
    m_pBoard->vPrintBoard("Combiner::vDelete_Combines"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
