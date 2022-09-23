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

  vBeginIteration();

  pMainCard = pEachMain(CK_ABSJOIN);

  if (pMainCard) {
    Card_AbsJoin* pCard_AbsJoin = _pCard_AbsJoin(pMainCard);

    while (m_pMainCardIter->bHasPrev()) {
      pMainCard = m_pMainCardIter->pPrev();

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

  vEndIteration();
}

/*----------------------------------------------------------------------------*/
/* append preliminary <indexJoin>'s to all <absJoin>                          */
/*----------------------------------------------------------------------------*/
void Joiner::vAppendPrelim_indexJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  Card *pMainCard = nullptr;

  vBeginIteration();

  while (Card_AbsJoin* pCard_absJoin = _pCard_AbsJoin(pEachMain(CK_ABSJOIN))) {

    Card_Scan* pCard_scan = _pGetSub_Card_Scan(pCard_absJoin);
    Card_Table* pCard_table = _pGetSub_Card_Table(pCard_scan);
    Table* pTable = pCard_table->pTable();

    IdxVec* pIdxVec = pTable->pIdxVec();
    if (! pIdxVec)
      return;

    i8 nIndexes = pIdxVec->nItems();
    for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
      Index* pIndex = pIdxVec->pGet(i_Index);

      Card_IndexJoin* pCard_indexJoin = new Card_IndexJoin;
      pCard_absJoin->vAppend(pCard_indexJoin);

      Card_Index* pCard_index = new Card_Index(pIndex);
      pCard_indexJoin->vAppend(pCard_index);

      Card_IdxColList* pCard_idxColList = new Card_IdxColList(pIndex);
      pCard_indexJoin->vAppend(pCard_idxColList);

    }
  }

  vEndIteration();
}

/*----------------------------------------------------------------------------*/
/* append indexPred to absJoin                                                */
/*----------------------------------------------------------------------------*/
void Joiner::vAppend_indexPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  vBeginIteration();

  while (Card_AbsJoin* pCard_AbsJoin = _pCard_AbsJoin(pEachMain(CK_ABSJOIN))) {

    Card_JoinPred* pCard_JoinPred =
        _pCard_JoinPred(m_pBoard->pFindNextMain(pCard_AbsJoin,CK_JOINPRED));


    Card_PredCmp* pCard_PredCmp = _pCard_PredCmp(pCard_JoinPred->pFindSubCard(CK_CARD)->pCard());
    Card_IndexPred* pCard_IndexPred = pTo_IndexPred(pCard_AbsJoin,pCard_PredCmp);
    pCard_AbsJoin->vAppend(pCard_IndexPred);

    pCard_AbsJoin->vMark2('1');

  }

  vEndIteration();
}

/*----------------------------------------------------------------------------*/
/* convert <absJoin> into <indexJoin>                                         */
/*----------------------------------------------------------------------------*/
void Joiner::vConvert_absJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  vBeginIteration();

  while (Card_AbsJoin* pCard_AbsJoin = _pCard_AbsJoin(pEachMain(CK_ABSJOIN))) {
    pCard_AbsJoin->vCard(CK_INDEXJOIN);
  }

  vEndIteration();
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
/* delete <scan_r> from board                                               */
/*----------------------------------------------------------------------------*/
void Joiner::vDelete_scan_R(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  vBeginIteration();

  while (Card *pMainCard = pEachMain()) {

    i8 i_Test = pMainCard->iCardNo();
    Card* pTestCard = m_pBoard->pGetMainCard(i_Test++);
    if (CK_SCAN != pTestCard->eCard())
      continue;
    Card_Scan* pCard_scan_l = _pCard_Scan(pTestCard);

    pTestCard = m_pBoard->pGetMainCard(i_Test++);
    if (CK_SCAN != pTestCard->eCard())
      continue;
    Card_Scan* pCard_scan_r = _pCard_Scan(pTestCard);

    pTestCard = m_pBoard->pGetMainCard(i_Test++);
    if (CK_INDEXJOIN != pTestCard->eCard())
      continue;
    Card_IndexJoin* pCard_indexJoin = _pCard_IndexJoin(pTestCard);

    m_pBoard->vRemoveMainCard(pCard_scan_r);
    delete pCard_scan_r;

    break;
  }

  vEndIteration();
}

/*----------------------------------------------------------------------------*/
/* insert joinPred's                                                          */
/*----------------------------------------------------------------------------*/
void Joiner::vInsert_joinPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  vBeginIteration();

  while (Card *pMainCard = pEachMain(CK_ABSJOIN)) {

    Card_JoinPred* pCard_JoinPred = new Card_JoinPred;
    m_pBoard->vInsertMainCard_after(pCard_JoinPred, pMainCard);
  }

  vEndIteration();
}

/*----------------------------------------------------------------------------*/
/* resolve joinPred's                                                         */
/*----------------------------------------------------------------------------*/
void Joiner::vResolve_joinPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  vBeginIteration();

  while (Card *pCard_joinPred = _pCard_JoinPred(pEachMain(CK_JOINPRED))) {

    Card_Filter* pCard_filter =
        _pCard_Filter(m_pBoard->pFindNextMain(pCard_joinPred, CK_FILTER));


    Card_PredCmp* pCard_PredCmp = _pCard_PredCmp(pCard_filter->pFindSubCard(CK_PREDCMP));

    if (! pCard_PredCmp)
      return;

    if (! pCard_PredCmp->szCmp()->bSame("=="_Stz))
      return;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Card *pCard_Term_0 = pCard_PredCmp->pGetSubCard(0);
    Stz* szTerm_0 = pCard_Term_0->szTerm();

    Card *pCard_Term_1 = pCard_PredCmp->pGetSubCard(1);
    Stz* szTerm_1 = pCard_Term_1->szTerm();

    Resolver* pResolver = new Resolver(m_pBench);

    s256 sStreamNm_0;
    s256 sStreamColNm_0;
    i8   i_StreamColNo_0;

    pResolver->vResolve(pCard_filter, szTerm_0,
                        sStreamNm_0, sStreamColNm_0, i_StreamColNo_0);

    s256 sStreamNm_1;
    s256 sStreamColNm_1;
    i8   i_StreamColNo_1;

    pResolver->vResolve(pCard_filter, szTerm_1,
                        sStreamNm_1, sStreamColNm_1, i_StreamColNo_1);

    delete pResolver;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


    Card_Card* pCard_Card = new Card_Card(pCard_PredCmp);

    pCard_joinPred->vAppend(pCard_Card);

  }

  vEndIteration();
}

/*----------------------------------------------------------------------------*/
/* each <scan> <absJoin> becomes <absJoin>/<scan>                             */
/*----------------------------------------------------------------------------*/
void Joiner::vSubsume_scan(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  i8 nMainCards = m_pBoard->nMainCards();
  for (i8 i_MainCard = 0; i_MainCard < nMainCards - 1; i_MainCard++) {

    Card *pMainCard = m_pBoard->pGetMainCard(i_MainCard);
    if (CK_SCAN != pMainCard->eCard())
      continue;

    Card *pNextCard = m_pBoard->pGetMainCard(i_MainCard+1);
    if (CK_ABSJOIN != pNextCard->eCard())
      continue;

    m_pBoard->vRemoveMainCard(pMainCard);
    pNextCard->vAppend(pMainCard);
    i_MainCard--;
    nMainCards--;
  }
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
