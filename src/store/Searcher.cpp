/*============================================================================*/
/* Searcher.cpp --                                                           */
/*============================================================================*/

#include "Searcher.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Searcher,CN_SEARCHER)
__aNewFunction(Searcher,CN_SEARCHER)
__vDeleteFunction(Searcher,CN_SEARCHER)
__aFooterFunction(Searcher)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Searcher::Searcher(Bench* pBench)
: TransformerTool(CN_SEARCHER, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEARCHER,_FOOTER_);

  _checkInitialised_(CN_SEARCHER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SEARCHER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Searcher::~Searcher(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEARCHER,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* search for parent of given sub card                                        */
/*----------------------------------------------------------------------------*/
Card* Searcher::pFindMainOf(Card* pGivenCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  Card *pMainCard = nullptr;

  CardIter *pMainCardIter = m_pBoard->pNewMainCardIter();

  while (pMainCardIter->bHasNext()) {
    Card *pMainCard = pMainCardIter->pNext();

    if (pMainCard->bHasSubCard(pGivenCard))
      break;
  }

  delete pMainCardIter;

  return pMainCard;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
