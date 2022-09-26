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
/* simplify                                                                   */
/*----------------------------------------------------------------------------*/
void Combiner::vAdd_combined(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINER, _FOOTER_);


  vBeginIteration();

  while (Card_Send *pCard_send = _pCard_Send(pEachMain(CK_SEND))) {

    Card_Combine* pCard_combine = new Card_Combine;

    m_pBoard->vInsertMainCard_before(pCard_combine,pCard_send);

    //break;
  }

  vEndIteration();
}

/*----------------------------------------------------------------------------*/
/* simplify                                                                   */
/*----------------------------------------------------------------------------*/
void Combiner::vAdd_combined2(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINER, _FOOTER_);

  i8 iStage = 0;
  i8 iScan = 0;
  i8 iIndexJoin = 0;
  i8 iProject = 0;
  i8 iAggreg = 0;


  i8 nMainCards = m_pBoard->nMainCards();
  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
    Card *pMainCard = m_pBoard->pGetMainCard(i_Main);

    eCK eCard = pMainCard->eCard();

    if (CK_PROJECT == eCard) {
      iProject++;

      Card_Combine *pCard_combine = new Card_Combine;
      m_pBoard->vInsertMainCard_after(pCard_combine, pMainCard);
      i_Main++;
      nMainCards = m_pBoard->nMainCards();
      continue;
    }

    if (CK_INDEXJOIN == eCard) {
      iIndexJoin++;

      Card* pMainCard_next = m_pBoard->pGetMainCard(i_Main+1);
      eCK eCard_next = pMainCard_next->eCard();
      if (CK_AGGREG == eCard_next || CK_FILTER == eCard_next || CK_PROJECT == eCard_next)
        continue;

      Card_Combine *pCard_combine = new Card_Combine;
      m_pBoard->vInsertMainCard_after(pCard_combine, pMainCard);
      i_Main++;
      nMainCards = m_pBoard->nMainCards();
      continue;
    }

    if (CK_AGGREG == eCard) {
      iProject++;

      Card_Combine *pCard_combine = new Card_Combine;
      m_pBoard->vInsertMainCard_after(pCard_combine, pMainCard);
      i_Main++;
      nMainCards = m_pBoard->nMainCards();
      continue;
    }


  }
}

/*----------------------------------------------------------------------------*/
/* delete combines                                                            */
/*----------------------------------------------------------------------------*/
void Combiner::vDelete_Combines(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINER, _FOOTER_);

  i8 nMainCards = m_pBoard->nMainCards();
  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
    Card* pMainCard = m_pBoard->pGetMainCard(i_Main);

    if (CK_COMBINE == pMainCard->eCard()) {
      m_pBoard->vRemoveMainCard(pMainCard);
      nMainCards = m_pBoard->nMainCards();
      i_Main--;
      delete pMainCard;
    }

  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
