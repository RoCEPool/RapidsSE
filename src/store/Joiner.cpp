/*============================================================================*/
/* Joiner.cpp --                                                           */
/*============================================================================*/

#include "Joiner.hpp"
#include "Resolver.hpp"

#include "Table.hpp"
#include "Index.hpp"

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
/* add table and index                                                        */
/*----------------------------------------------------------------------------*/
void Joiner::vAppend_index(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  Card *pMainCard = nullptr;

  CardIter *pMainCardIter = m_pBoard->pNewMainCardIter();
  while (pMainCardIter->bHasNext()) {
    pMainCard = pMainCardIter->pNext();

    if (CK_ABSJOIN == pMainCard->eCard())
      break;
  }

  if (pMainCard) {
    Card_AbsJoin* pCard_AbsJoin = _pCard_AbsJoin(pMainCard);

    while (pMainCardIter->bHasPrev()) {
      pMainCard = pMainCardIter->pPrev();

      if (CK_SCAN == pMainCard->eCard()) {
        Card_Table* pCard_Table = _pCard_Table(pMainCard->pFindSubCard(CK_TABLE));
        pCard_AbsJoin->vAppend(pCard_Table->pDupCardAndSubCards());

        Table* pTable = pCard_Table->pTable();
        Index* pIndex = pTable->pIdxVec()->pGet(0);
        Card_Index* pCard_Index = new Card_Index(pIndex);
        pCard_AbsJoin->vAppend(pCard_Index);

        break;
      }
    }
  }

  delete pMainCardIter;

}

/*----------------------------------------------------------------------------*/
/* append indexPred to absJoin                                                */
/*----------------------------------------------------------------------------*/
void Joiner::vAppend_indexPred(void)
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

    pCard_AbsJoin->vMark2('1');

  }

  delete pMainCardIter;
}

/*----------------------------------------------------------------------------*/
/* delete <joinPred> from board                                               */
/*----------------------------------------------------------------------------*/
void Joiner::vDelete_joinPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  Card_JoinPred* pCard_joinPred
    = _pCard_JoinPred(m_pBoard->pFindMain(CK_JOINPRED));

  m_pBoard->vRemoveMainCard(pCard_joinPred);

  delete pCard_joinPred;
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

  s256 sStreamNm_0;
  s256 sStreamColNm_0;
  i8   i_StreamColNo_0;

  pResolver->vResolve(pCard_AbsJoin, szTerm_0,
                      sStreamNm_0, sStreamColNm_0,i_StreamColNo_0);

  s256 sStreamNm_1;
  s256 sStreamColNm_1;
  i8   i_StreamColNo_1;

  pResolver->vResolve(pCard_AbsJoin, szTerm_1,
                      sStreamNm_1, sStreamColNm_1, i_StreamColNo_1);

  s256 sPred_0;
  s256 sPred_1;

  if (sStreamNm_0.bSame("L"_Stz)) {
    sPred_0.vSprint("$I%lld"_Stz,i_StreamColNo_1+1);
    sPred_1.vSprint("$%lld"_Stz,i_StreamColNo_0+1);
  }
  else {
    sPred_0.vSprint("$I%lld"_Stz,i_StreamColNo_0+1);
    sPred_1.vSprint("$%lld"_Stz,i_StreamColNo_1+1);
  }

  pCard_IndexPred->vAppend(new Card_Term(sPred_0));
  pCard_IndexPred->vAppend(new Card_Term(sPred_1));

  pCard_IndexPred->vAppend(new Card_Term(pCard_PredCmp->szCmp()));

  delete pResolver;

  return pCard_IndexPred;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
