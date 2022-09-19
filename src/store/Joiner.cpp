/*============================================================================*/
/* Joiner.cpp --                                                           */
/*============================================================================*/

#include "Joiner.hpp"
#include "Resolver.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Joiner,CN_JOINER)
__aNewFunction(Joiner,CN_JOINER)
__vDeleteFunction(Joiner,CN_JOINER)
__aFooterFunction(Joiner)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Joiner::Joiner(Bench* pBench)
: TransformerTool(CN_JOINER,pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINER,_FOOTER_);

  _checkInitialised_(CN_JOINER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOINER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Joiner::~Joiner(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINER,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* stream                                                                     */
/*----------------------------------------------------------------------------*/
void Joiner::vJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  CardIter *pMainCardIter = m_pBoard->pNewMainCardIter();

  while (pMainCardIter->bHasNext()) {
    Card *pMainCard = pMainCardIter->pNext();

    if (CK_ABSJOIN != pMainCard->eCard())
      continue;

    Card_AbsJoin* pCard_AbsJoin = _pCard_AbsJoin(pMainCard);

    Card_JoinPred* pCard_JoinPred =
        _pCard_JoinPred(m_pBoard->pFindNextMain(pCard_AbsJoin,CK_JOINPRED));


    Card_PredCmp* pCard_PredCmp = _pCard_PredCmp(pCard_JoinPred->pFindSubCard(CK_CARD)->pCard());
    Card_IndexPred* pCard_IndexPred = pTo_IndexPred(pCard_AbsJoin,pCard_PredCmp);
    pCard_AbsJoin->vAppend(pCard_IndexPred);

  }

  delete pMainCardIter;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert a calculation into a boolean calculaiton                           */
/*----------------------------------------------------------------------------*/
Card_IndexPred* Joiner::pTo_IndexPred(Card_AbsJoin* pCard_AbsJoin,
                                      Card_PredCmp* pCard_PredCmp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  Card_IndexPred *pCard_IndexPred = new Card_IndexPred;

//  pCard_IndexPred->vAppend(new Card_Term("BOUND"_Stz));
//  pCard_IndexPred->vAppend(new Card_Term("$I1"_Stz));
//  pCard_IndexPred->vAppend(new Card_Term("$L1"_Stz));
//  pCard_IndexPred->vAppend(new Card_Term("=="_Stz));

  Resolver *pResolver = new Resolver(m_pBench);

  pCard_IndexPred->vAppend(new Card_Term("BOUND"_Stz));

  Card_Term *pCard_Term_0 = _pCard_Term(pCard_PredCmp->pGetSubCard(0));
  Stz *szTerm_0 = pCard_Term_0->szTerm();
  Card_Term *pCard_Term_1 = _pCard_Term(pCard_PredCmp->pGetSubCard(1));
  Stz *szTerm_1 = pCard_Term_1->szTerm();

  Stz *szPred_0 = szTerm_0;
  Stz *szPred_1 = szTerm_1;

  s256 sStreamNm_0;
  s256 sStreamColNm_0;

  pResolver->vResolve(pCard_AbsJoin, szTerm_0,
                      sStreamNm_0, sStreamColNm_0);

  s256 sStreamNm_1;
  s256 sStreamColNm_1;

  pResolver->vResolve(pCard_AbsJoin, szTerm_1,
                      sStreamNm_1, sStreamColNm_1);

  if (sStreamNm_0.bSame("L"_Stz)) {
    szPred_0 = szTerm_1;
    szPred_1 = szTerm_0;
  }
  else {
    szPred_0 = szTerm_0;
    szPred_1 = szTerm_1;
  }

  pCard_IndexPred->vAppend(new Card_Term(szPred_0));
  pCard_IndexPred->vAppend(new Card_Term(szPred_1));

  pCard_IndexPred->vAppend(new Card_Term(pCard_PredCmp->szCmp()));

  delete pResolver;

  return pCard_IndexPred;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
