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
/* append preliminary <indexJoin>'s to all <absJoin>                          */
/*----------------------------------------------------------------------------*/
void Joiner::vAppendPrelim_indexJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  m_pBoard->vClearAllMarks();

  Card *pMainCard = nullptr;

  vBeginIteration();

  while (Card_AbsJoin* pCard_absJoin = _pCard_AbsJoin(pEachMain(CK_ABSJOIN))) {

    Card_Scan* pCard_scan = _pGetSub_Scan(pCard_absJoin);
    Card_TablePtr* pCard_tablePtr = _pGetSub_TablePtr(pCard_scan);
    Table* pTable = pCard_tablePtr->pTable();

    IdxVec* pIdxVec = pTable->pIdxVec();
    if (! pIdxVec)
      return;

    i8 nIndexes = pIdxVec->nItems();
    for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
      Index* pIndex = pIdxVec->pGet(i_Index);

      Card_IndexJoin* pCard_indexJoin = new Card_IndexJoin;
      pCard_absJoin->vAppend(pCard_indexJoin);

      Card_IndexPtr* pCard_index = new Card_IndexPtr(pIndex);
      pCard_indexJoin->vAppend(pCard_index);

      Card_IdxColList* pCard_idxColList = new Card_IdxColList(pIndex);
      pCard_indexJoin->vAppend(pCard_idxColList);

    }
  }

  vEndIteration();

  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vAppendPrelim_indexJoin"_Stz);
}


/*----------------------------------------------------------------------------*/
/* form <joinPred>                                                            */
/*----------------------------------------------------------------------------*/
void Joiner::vForm_joinPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  vBeginIteration();

  while (Card_AbsJoin *pCard_absJoin = _pCard_AbsJoin(pEachMain(CK_ABSJOIN))) {

    Card_Filter*   pCard_filter   = _pGetSub_Filter(pCard_absJoin);
    Card_Scan*     pCard_scan     = _pGetSub_Scan(pCard_absJoin);
    Card_Stream*   pCard_stream   = _pGetSub_Stream(pCard_scan);
    Card_Expr*     pCard_expr     = _pCard_Expr(pCard_filter->pGetChild(0));
    Card_CalcBool* pCard_calcBool = _pGetSub_CalcBool(pCard_expr);
    Card_PredCmp*  pCard_predCmp  = _pGetSub_PredCmp(pCard_calcBool);
    Card_Term*     pCard_term_0   = _pCard_Term(pCard_predCmp->pGetChild(0));
    Card_Term*     pCard_term_1   = _pCard_Term(pCard_predCmp->pGetChild(1));

    // - - - -

    Card_IndexJoin* pCard_indexJoin = _pGetSub_IndexJoin(pCard_absJoin);

    Card_JoinPred* pCard_joinPred = new Card_JoinPred;
    pCard_absJoin->vAppend(pCard_joinPred);

    Card_Term* pCard_term;


    pCard_term = new Card_Term("BOUND"_Stz);
    pCard_joinPred->vAppend(pCard_term);

    // one of {pCard_term_0,pCard_term_1} is $In

    i8 iStreamOrdLo = pCard_stream->iOrdLo();
    i8 iStreamOrdHi = pCard_stream->iOrdHi();

    i8 i_iOrdLo_0 = pCard_term_0->iOrdinalStreamColNo();
    i8 i_iOrdLo_1 = pCard_term_1->iOrdinalStreamColNo();

    if (i_iOrdLo_0 >= iStreamOrdLo && i_iOrdLo_0 <= iStreamOrdHi) {
      pCard_term = new Card_Term("$I1"_Stz);
      pCard_joinPred->vAppend(pCard_term);
      pCard_term = new Card_Term(pCard_term_1->sz());
      pCard_joinPred->vAppend(pCard_term);
    }
    else
    if (i_iOrdLo_1 >= iStreamOrdLo && i_iOrdLo_1 <= iStreamOrdHi) {
      pCard_term = new Card_Term("$I1"_Stz);
      pCard_joinPred->vAppend(pCard_term);
      pCard_term = new Card_Term(pCard_term_1->sz());
      pCard_joinPred->vAppend(pCard_term);
    }
    else {
      NEVER_REACH
    }

    pCard_term = new Card_Term(pCard_predCmp->sz());
    pCard_joinPred->vAppend(pCard_term);

    pCard_predCmp->vMark1('^');
  }

  vEndIteration();

  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vForm_joinPred"_Stz);
}

/*----------------------------------------------------------------------------*/
/* markup index columns                                                       */
/*----------------------------------------------------------------------------*/
void Joiner::vMark_idxCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  m_pBoard->vClearAllMarks();

  Card *pMainCard = nullptr;

  vBeginIteration();

  while (Card_AbsJoin* pCard_absJoin = _pCard_AbsJoin(pEachMain(CK_ABSJOIN))) {

    Card_Scan* pCard_scan = _pGetSub_Scan(pCard_absJoin);
    Card_Stream* pCard_stream = _pGetSub_Stream(pCard_scan);
    Card_StreamColList* pCard_streamColList = _pGetSub_StreamColList(pCard_scan);

    Card_IndexJoin* pCard_indexJoin = _pGetSub_IndexJoin(pCard_absJoin);
    Card_IdxColList* pCard_idxColList = _pGetSub_IdxColList(pCard_indexJoin);

    pCard_idxColList->vMark1('!');

    //s32 sOrdinal;
    //pCard_stream->vOrdinate(sOrdinal,"$$"_Stz);
    //pCard_idxColList->vOrdinal(sOrdinal);
    pCard_idxColList->vOrdinate(pCard_stream);
  }

  vEndIteration();

  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vMark_idxCol"_Stz);
}

/*----------------------------------------------------------------------------*/
/* reduce absJoin                                                             */
/*----------------------------------------------------------------------------*/
void Joiner::vReduce_absJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  vBeginIteration();

  while (Card_AbsJoin *pCard_absJoin = _pCard_AbsJoin(pEachMain(CK_ABSJOIN))) {

    //pCard_absJoin->vDeleteChildCard(CK_CALC);
    //pCard_absJoin->vDeleteChildCard(CK_FILTER);

    Card_Scan* pCard_scan = _pGetSub_Scan(pCard_absJoin);
    pCard_absJoin->vReplaceChildWithKids(pCard_scan);
    delete pCard_scan;

    Card_IndexJoin* pCard_indexJoin = _pGetSub_IndexJoin(pCard_absJoin);
    pCard_absJoin->vReplaceChildWithKids(pCard_indexJoin);
    delete pCard_indexJoin;

    Card_Stream* pCard_stream = _pGetSub_Stream(pCard_absJoin);
    pCard_absJoin->vMoveToLastChild(pCard_stream);

    pCard_absJoin->vCard(CK_INDEXJOIN);
  }
}

/*----------------------------------------------------------------------------*/
/* each <scan> <absJoin> becomes <absJoin>/<scan>                             */
/*----------------------------------------------------------------------------*/
void Joiner::vSubsume_scan(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  i8 nMainCards = m_pBoard->nMainCards();
  for (i8 i_Main = 0; i_Main < nMainCards - 1; i_Main++) {

    Card *pMainCard = m_pBoard->pGetMain(i_Main);

    eCK eCard = pMainCard->eCard();
    if (CK_ABSJOIN == eCard) {
      pMainCard->vDeleteChildCard(CK_CALC);
      continue;
    }

    if (CK_SCAN != pMainCard->eCard())
      continue;

    Card *pNextCard = m_pBoard->pGetMain(i_Main + 1);
    if (CK_ABSJOIN != pNextCard->eCard())
      continue;

    m_pBoard->vRemoveMainCard(pMainCard);
    pNextCard->vAppend(pMainCard);
    i_Main--;
    nMainCards--;
  }

  if (m_bInteractive)
    m_pBoard->vPrintBoard("Joiner::vSubsume_scan"_Stz);
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

  Card_Term *pCard_Term_0 = _pCard_Term(pCard_PredCmp->pGetChild(0));
  Stz *szTerm_0 = pCard_Term_0->sz();
  Card_Term *pCard_Term_1 = _pCard_Term(pCard_PredCmp->pGetChild(1));
  Stz *szTerm_1 = pCard_Term_1->sz();

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

  pCard_IndexPred->vAppend(new Card_Term(pCard_PredCmp->sz()));

  delete pResolver;

  return pCard_IndexPred;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
