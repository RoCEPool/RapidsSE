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
