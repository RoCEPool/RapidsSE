/*============================================================================*/
/* Predicator.cpp -- predicate tool                                          */
/*============================================================================*/

#include "Predicator.hpp"
#include "Resolver.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Predicator, CN_PREDICATOR)
__aNewFunction(Predicator, CN_PREDICATOR)
__vDeleteFunction(Predicator, CN_PREDICATOR)
__aFooterFunction(Predicator)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Predicator::Predicator(Bench* pBench)
: OptimiserTool(CN_PREDICATOR, pBench)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PREDICATOR, _FOOTER_);

  _checkInitialised_(CN_PREDICATOR);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_PREDICATOR, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Predicator::~Predicator(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PREDICATOR, _FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* predicate                                                                  */
/*----------------------------------------------------------------------------*/
//void Predicator::vDiscover(void)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_PREDICATOR, _FOOTER_);
//
//  i8 nMainCards = m_pBoard->nMainCards();
//  for (i8 i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
//    Card *pMainCard = m_pBoard->pGetMainCard(i_MainCard);
//
//    if (CK_FILTER == pMainCard->eCard()) {
//      vTrace("Predicator::vDiscover %lld\n"_Stz, i_MainCard);
//      vDiscoverRecursive(pMainCard);
//    }
//  }
//}
/*----------------------------------------------------------------------------*/
void Predicator::vDiscover(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PREDICATOR, _FOOTER_);

  CardIter* pMainCardIter = m_pBoard->pNewMainCardIter();

  while (pMainCardIter->bHasNext()) {
    Card *pMainCard = pMainCardIter->pNext();

    if (CK_FILTER == pMainCard->eCard()) {
      vTrace("Predicator::vDiscover %lld %s\n"_Stz,
             pMainCard->iCardNo(),aCK[pMainCard->eCard()]);
      vDiscoverRecursive(pMainCard);
    }
    else {
      vTrace("Predicator::Skipping %lld %s\n"_Stz,
             pMainCard->iCardNo(),aCK[pMainCard->eCard()]);
    }
  }

  delete pMainCardIter;
}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Predicator::vDiscoverRecursive(Card* pAnyCard, Card* pMainCard_filter) // recursive
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_PREDICATOR, _FOOTER_);
//
//  eCK eCard = pAnyCard->eCard();
//
//  //
//  // subcards first
//  //
//
//  //pCard->vDump();
//
//  i8 nSubCards = pAnyCard->nSubCards();
//  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
//    Card *pSubCard = pAnyCard->pGetSubCard(i_SubCard);
//    vDiscoverRecursive(pSubCard, pMainCard_filter);
//  }
//
//  //
//  // this card
//  //
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  if (CK_PREDCMP != eCard)
//    return;
//
//  if (! pAnyCard->szCmp()->bSame("=="_Stz))
//    return;
//
//  Card *pCard_Term_0 = pAnyCard->pGetSubCard(0);
//  Stz* szTerm_0 = pCard_Term_0->szTerm();
//
//  Card *pCard_Term_1 = pAnyCard->pGetSubCard(1);
//  Stz* szTerm_1 = pCard_Term_1->szTerm();
//
//  Resolver* pResolver = new Resolver(m_pBench);
//
//  s256 sStreamNm_0;
//  s256 sStreamColNm_0;
//
//  pResolver->vResolve(pMainCard_filter, szTerm_0, sStreamNm_0, sStreamColNm_0);
//
//  s256 sStreamNm_1;
//  s256 sStreamColNm_1;
//
//  pResolver->vResolve(pMainCard_filter, szTerm_1, sStreamNm_1, sStreamColNm_1);
//
//  delete pResolver;
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  Card_JoinPred* pCard_JoinPred = new Card_JoinPred;
//  m_pBoard->vInsertMainCard_before(pCard_JoinPred, pMainCard_filter);
//}
/*----------------------------------------------------------------------------*/
void Predicator::vDiscoverRecursive(Card* pMainCard_filter) // recursive
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PREDICATOR, _FOOTER_);

  Card_PredCmp* pPredCmp = _pCard_PredCmp(pMainCard_filter->pFindSubCard(CK_PREDCMP));

  if (! pPredCmp)
    return;

  if (! pPredCmp->szCmp()->bSame("=="_Stz))
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card *pCard_Term_0 = pPredCmp->pGetSubCard(0);
  Stz* szTerm_0 = pCard_Term_0->szTerm();

  Card *pCard_Term_1 = pPredCmp->pGetSubCard(1);
  Stz* szTerm_1 = pCard_Term_1->szTerm();

  Resolver* pResolver = new Resolver(m_pBench);

  s256 sStreamNm_0;
  s256 sStreamColNm_0;

  pResolver->vResolve(pMainCard_filter, szTerm_0, sStreamNm_0, sStreamColNm_0);

  s256 sStreamNm_1;
  s256 sStreamColNm_1;

  pResolver->vResolve(pMainCard_filter, szTerm_1, sStreamNm_1, sStreamColNm_1);

  delete pResolver;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_JoinPred* pCard_JoinPred = new Card_JoinPred;
  m_pBoard->vInsertMainCard_before(pCard_JoinPred, pMainCard_filter);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
