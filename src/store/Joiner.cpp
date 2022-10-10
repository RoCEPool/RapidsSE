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
void Joiner::vInsert_preliminary_indexJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  vTransformation((char*)(__func__));
  vExplain("Blah"_Stz);

  m_pBoard->vClearAllMarks();

  BoardIter xBI(m_pBoard);

  while (m_p_scan = _pCard_Scan(xBI.pEachMain(CK_SCAN))) {

    if (! (m_p_filter = m_p_scan->pSub_filter()))
      continue;

    m_p_tablePtr = _pSub_TablePtr(m_p_scan);
    m_pTable = m_p_tablePtr->pTable();

    if (! (m_pIdxVec = m_pTable->pIdxVec()))
      return;

    i8 nIndexes = m_pIdxVec->nItems();
    for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
      m_pIndex = m_pIdxVec->pGet(i_Index);

      m_p_indexJoin = new Card_IndexJoin;
      m_p_scan->vInsertBefore(m_p_indexJoin,m_p_filter);

      m_p_indexPtr = new Card_IndexPtr(m_pIndex);
      m_p_indexJoin->vAppend(m_p_indexPtr);

      Card_IdxColList* pCard_idxColList = new Card_IdxColList(m_pIndex);
      m_p_indexJoin->vAppend(pCard_idxColList);
    }
  }

  //m_pCardCtl->vPrintFormSubCards(CK_STREAM, CP_DEFAULT, true);
  vSetPrintSubCards(CK_FILTER, false);
  vSetPrintSubCards(CK_STREAM, false);

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete sends                                                               */
/*----------------------------------------------------------------------------*/
void Joiner::vDeleteAll_absJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  vTransformation((char*)(__func__),false);
  vExplain("Blah"_Stz);

  vRemoveDeleteMains(CK_ABSJOIN);

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* form <joinPred>                                                            */
/*----------------------------------------------------------------------------*/
void Joiner::vForm_joinPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  vTransformation((char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ((m_p_absJoin = _pCard_AbsJoin(xBI.pEachMain(CK_ABSJOIN)))) {

    m_p_filter   = _pSub_Filter(m_p_absJoin);
    m_p_scan     = _pSub_Scan(m_p_absJoin);
    m_p_stream   = _pSub_Stream(m_p_scan);
    m_p_expr     = _pCard_Expr(m_p_filter->pGetChild(0));
    m_p_calcBool = _pSub_CalcBool(m_p_expr);
    m_p_predCmp  = _pSub_PredCmp(m_p_calcBool);
    m_p_term_0   = _pCard_Term(m_p_predCmp->pGetChild(0));
    m_p_term_1   = _pCard_Term(m_p_predCmp->pGetChild(1));

    // - - - -

    Card_IndexJoin* pCard_indexJoin = _pSub_IndexJoin(m_p_absJoin);

    Card_JoinPred* pCard_joinPred = new Card_JoinPred;
    m_p_absJoin->vAppend(pCard_joinPred);

    m_p_term = new Card_Term("BOUND"_Stz);
    pCard_joinPred->vAppend(m_p_term);

    // one of {pCard_term_0,pCard_term_1} is $In

    i8 iStreamOrdLo = m_p_stream->iGetProperty(C_PPT_OrdLo);
    i8 iStreamOrdHi = m_p_stream->iGetProperty(C_PPT_OrdHi);

    i8 i_iOrdLo_0 = m_p_term_0->iStreamColNo();
    i8 i_iOrdLo_1 = m_p_term_1->iStreamColNo();

    if (i_iOrdLo_0 >= iStreamOrdLo && i_iOrdLo_0 <= iStreamOrdHi) {
      m_p_term = new Card_Term("$I1"_Stz);
      pCard_joinPred->vAppend(m_p_term);
      m_p_term = new Card_Term(m_p_term_1->sz());
      pCard_joinPred->vAppend(m_p_term);
    }
    else
    if (i_iOrdLo_1 >= iStreamOrdLo && i_iOrdLo_1 <= iStreamOrdHi) {
      m_p_term = new Card_Term("$I1"_Stz);
      pCard_joinPred->vAppend(m_p_term);
      m_p_term = new Card_Term(m_p_term_1->sz());
      pCard_joinPred->vAppend(m_p_term);
    }
    else {
      NEVER_REACH
    }

    m_p_predCmp->vMark1('^');
    m_p_term = new Card_Term(m_p_predCmp->sz());
    pCard_joinPred->vAppend(m_p_term);
  }

  vSetPrintSubCards(CK_FILTER, true);
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* markup index columns                                                       */
/*----------------------------------------------------------------------------*/
void Joiner::vConvertAll_idxColNm_To_streamColOrdinal(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  vTransformation((char*)(__func__));
  vExplain("Blah"_Stz);

  m_pBoard->vClearAllMarks();

  BoardIter xBI(m_pBoard);

  while ((m_p_scan = _pCard_Scan(xBI.pEachMain(CK_SCAN)))) {
    if (! (m_p_indexJoin = _pSub_IndexJoin(m_p_scan)))
      continue;

    m_p_stream = _pSub_Stream(m_p_scan);
    m_p_streamColList = _pSub_StreamColList(m_p_scan);
    m_p_idxColList = _pSub_IdxColList(m_p_indexJoin);

    m_p_idxColList->vOrdinate(m_p_stream);
  }

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* reduce absJoin                                                             */
/*----------------------------------------------------------------------------*/
void Joiner::vRefactor_absJoin_Into_indexJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  vTransformation((char*)(__func__));
  vExplain("Blah"_Stz);

  vBeginIteration();

  while (Card_AbsJoin *pCard_absJoin = _pCard_AbsJoin(pEachMain(CK_ABSJOIN))) {

    //pCard_absJoin->vDeleteChildCard(CK_CALC);
    //pCard_absJoin->vDeleteChildCard(CK_FILTER);

    Card_Scan* pCard_scan = _pSub_Scan(pCard_absJoin);
    pCard_absJoin->vReplaceChildWithItsSubs(pCard_scan);
    delete pCard_scan;

    Card_IndexJoin* pCard_indexJoin = _pSub_IndexJoin(pCard_absJoin);
    pCard_absJoin->vReplaceChildWithItsSubs(pCard_indexJoin);
    delete pCard_indexJoin;

    Card_Stream* pCard_stream = _pSub_Stream(pCard_absJoin);
    pCard_absJoin->vMoveToLastChild(pCard_stream);

    pCard_absJoin->vCard(CK_INDEXJOIN);
  }

  if (m_bInteractive)
    m_pBoard->vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* each <scan> <absJoin> becomes <absJoin>/<scan>                             */
/*----------------------------------------------------------------------------*/
void Joiner::vSubsume_scan_Under_absJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINER, _FOOTER_);

  vTransformation((char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);
  while ((m_p_absJoin = _pCard_AbsJoin(xBI.pEachMain(CK_ABSJOIN))))
    m_p_absJoin->vDeleteChildCard(CK_CALC);

  xBI.vRestart();

  // E <scan>
  while ((m_p_scan = _pCard_Scan(xBI.pEachMain(CK_SCAN)))) {

    // ? <scan <absJoin>
    if (! (m_p_absJoin = _pCard_AbsJoin(xBI.pNextMainIf(CK_ABSJOIN))))
      continue;

    // <absJoin>/<scan>
    xBI.vRemoveCard(m_p_scan);
    m_p_absJoin->vAppend(m_p_scan);
  }

  vPrintBoard();
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
