/*============================================================================*/
/* Bench.cpp -- Bench                                                         */
/*============================================================================*/

#include "Bench.hpp"
#include "Board.hpp"
#include "Optimiser.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Bench,CN_BENCH)
__aNewFunction(Bench,CN_BENCH)
__vDeleteFunction(Bench,CN_BENCH)
__aFooterFunction(Bench)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Bench::Bench(Optimiser* pOptimiser)
: Object(CN_BENCH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
    
  m_pOptimiser = pOptimiser;

  m_cPrefix = ' ';

  m_bIsFlat = false;

  m_pCI = new CardIter(this);
  
  _checkInitialised_(CN_BENCH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Bench::~Bench(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
  delete m_pCI;
  vDeleteAll();
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a new board                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vAddBoard(Board* pBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  //RASSERT(! pGetBoard(pBoard->eBoardKind()));

  vPush(pBoard);
}

/*----------------------------------------------------------------------------*/
/* does given board exist?                                                    */
/*----------------------------------------------------------------------------*/
bool Bench::bBoardExists(eBK eBoard, i8 i_SubBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  if (pGetBoard(eBoard,i_SubBoard))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get card registers                                                         */
/*----------------------------------------------------------------------------*/
CardCtl* Bench::pCardCtl(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  return m_pOptimiser->pCardCtl();
}

/*----------------------------------------------------------------------------*/
/* clear marks                                                                */
/*----------------------------------------------------------------------------*/
void Bench::vClearMarks(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  i8 nBoards = this->nItems();
  for (i8 i_Board = 0; i_Board < nBoards; i_Board++) {
    Board *pBoard = this->pGet(i_Board);

    pBoard->vClearMarks();
  }
}



/*----------------------------------------------------------------------------*/
/* get board of given kind                                                    */
/*----------------------------------------------------------------------------*/
Board* Bench::pGetBoard(eBK eBoard, i8 i_Board)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  i8 nBoards = this->nItems();
  for (i8 i_each = 0; i_each < nBoards; i_each++) {
    Board *pBoard = this->pGet(i_each);

    if (eBoard == pBoard->eBoardKind() && (C_UNKNOWN == i_Board ||
                                       i_Board == pBoard->i_Board()))
      return pBoard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get board                                                                  */
/*----------------------------------------------------------------------------*/
Board* Bench::pGetBoard(i8 i_Board)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  Board *pBoard = this->pGet(i_Board);

  return pBoard;
}

/*----------------------------------------------------------------------------*/
/* get board of given name                                                    */
/*----------------------------------------------------------------------------*/
Board* Bench::pGetBoard(Stz* szBoardNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  i8 nBoards = this->nItems();
  for (i8 i_Board = 0; i_Board < nBoards; i_Board++) {
    Board *pBoard = this->pGet(i_Board);

    if (pBoard->szBoardNm()->bSame(szBoardNm))
      return pBoard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* is the bench flat?                                                         */
/*----------------------------------------------------------------------------*/
bool Bench::bIsFlat(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  return m_bIsFlat;
}

/*----------------------------------------------------------------------------*/
/* set is the bench flat?                                                     */
/*----------------------------------------------------------------------------*/
void Bench::vIsFlat(bool bIsFlat)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_bIsFlat = bIsFlat;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Optimiser* Bench::pOptimiser(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  return m_pOptimiser;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
char Bench::cPrefix(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  return m_cPrefix;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vPrefix(char cPrefix)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_cPrefix = cPrefix;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   vSeries_40_Flatten_Query */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS40_T05_Flatten_By_Decision_Tree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  if (1 ==  nItems())
    return;

  m_pCI->vTransform(__func__, "((10))"_Stz);

  vAssert_Board_mqA();

  m_pCI->G.pSubQuery =  pGetBoard(C_SQ_BOARD)->pSubQuery();

  if (m_pCI->G.pSubQuery->bViaFrom())                /* Fsq_       Q7,8,9 */ vS40_T10_Fold_F();
  else
  if (m_pCI->G.pSubQuery->bViaWhere()) {
    if (m_pCI->G.pSubQuery->bIsAggregation()) {
      if (m_pCI->G.pSubQuery->bIsCorrelated()) {
        if (m_pCI->G.pSubQuery->bByExists())    /* W_A_C_E          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* W_A_C_NE         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* W_A_C_I          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* W_A_C_NI         */ NEVER_REACH
        else                                  /* W_A_C_ro   Q2,17 */ vS40_T20_Fold_W_A_C_ro();
      } else {
        if (m_pCI->G.pSubQuery->bByExists())    /* W_A_NC_E         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* W_A_NC_NE        */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* W_A_NC_I     Q18 */ vS40_T30_Fold_W_A_NC_I();
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* W_A_NC_NI        */ NEVER_REACH
        else                                  /* W_A_NC_ro    Q15 */ vS40_T40_Fold_W_A_NC_ro();
      }
    } else {
      if (m_pCI->G.pSubQuery->bIsCorrelated()) {
        if (m_pCI->G.pSubQuery->bByExists())    /* W_L_C_E       Q4 */ vS40_T50_Fold_W_L_C_E();
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* W_L_C_NE         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* W_L_C_I          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* W_L_C_NI         */ NEVER_REACH
        else                                  /* W_L_C_ro         */ NEVER_REACH
      } else {
        if (m_pCI->G.pSubQuery->bByExists())    /* W_L_NC_E         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* W_L_NC_NE        */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* W_L_NC_I         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* W_L_NC_NI    Q16 */ vS40_T60_Fold_W_L_NC_NI();
        else                                  /* W_L_NC_ro        */ NEVER_REACH
      }
    }
  }
  else
  if (m_pCI->G.pSubQuery->bViaHaving()) {
    if (m_pCI->G.pSubQuery->bIsAggregation()) {
      if (m_pCI->G.pSubQuery->bIsCorrelated()) {
        if (m_pCI->G.pSubQuery->bByExists())    /* H_A_C_E          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* H_A_C_NE         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* H_A_C_I          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* H_A_C_NI         */ NEVER_REACH
        else                                  /* H_A_C_ro         */ NEVER_REACH
      } else {
        if (m_pCI->G.pSubQuery->bByExists())    /* H_A_NC_E         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* H_A_NC_NE        */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* H_A_NC_I         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* H_A_NC_NI        */ NEVER_REACH
        else                                  /* H_A_NC_ro    Q11 */ vS40_T70_Fold_H_A_NC_ro();
      }
    } else {
      if (m_pCI->G.pSubQuery->bIsCorrelated()) {
        if (m_pCI->G.pSubQuery->bByExists())    /* H_L_C_E          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* H_L_C_NE         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* H_L_C_I          */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* H_L_C_NI         */ NEVER_REACH
        else                                  /* H_L_C_ro         */ NEVER_REACH
      } else {
        if (m_pCI->G.pSubQuery->bByExists())    /* H_L_NC_E         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotExists()) /* H_L_NC_NE        */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByIn())        /* H_L_NC_I         */ NEVER_REACH
        else if (m_pCI->G.pSubQuery->bByNotIn())     /* H_L_NC_NI        */ NEVER_REACH
        else                                  /* H_L_NC_ro        */ NEVER_REACH
      }
    }
  }
  else
    NEVER_REACH;

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS40_T10_Fold_F(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  // F
  // push a (from) sub-query before the main-query
  // as an op-sequence
  // (refer: Q7,8,9)

  /*
      select count(*)
        from (select *
                from orders, lineitem
               where L_ORDERKEY = O_ORDERKEY)
      ;
   */

  m_pCI->vTransform(__func__, "((20))"_Stz);

  vAssert_Board_mqA();
  m_pCI->vPrintBoard(__LINE__);

  // 1) find W_A_C_ro subquery call in mq:<where>/<boolCalc>
  vAssert_Board_mqA();
  RASSERT(m_pCI->G_bEachMain(CK_CALL));
  vTrace(">>>>>>>>> Found From CALl: %s\n"_Stz,m_pCI->G.p_call->szValue());
  m_pCI->vPrintBoard(__LINE__);


  m_pCI->G_vNewMember(CK_FOLD,"FROM"_Stz);
  m_pCI->G.pDeck->vPush(m_pCI->G.p_fold);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get copy of sq and push before mq

  // x) push to sq
  if (! m_pCI->G_bPushBoard(m_pCI->G.p_call->szValue())) {
    vTrace("DID NOT FIND Board %s\n"_Stz,m_pCI->G.p_call->szValue());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  m_pCI->m_pCard_copied = m_pCI->G.pDeck->pDupSubCards(0);

  m_pCI->vPrintBoard(__LINE__);

  // ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^

  // x) pop back to mq
  m_pCI->G_vPopIter();
  vAssert_Board_mqA();

  m_pCI->G.pDeck->vPush(m_pCI->m_pCard_copied);
  m_pCI->G.pDeck->vReplaceChildWithItsSubs(m_pCI->m_pCard_copied);
  DELETE_CARD(m_pCI->m_pCard_copied);

  m_pCI->G.pDeck->vRemoveChild(m_pCI->G.p_call);
  DELETE_CARD(m_pCI->G.p_call);

  m_pCI->G.pDeck->vRenumberRecursive();

  m_pCI->vPrintBoard(__LINE__);

  Card_Stream::vStream_AdjustLoHi(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  Board::vRenumberStmOdn(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  Board::vRelabelExpressions(m_pCI);

  vIsFlat(true);
  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vPrintBoard(__LINE__);


  // discuss with Alka
  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_FOLD)) {
    if (m_pCI->G_bEachMain(CK_WHERE)) {
      Card_Level* p_level_where = m_pCI->G.p_where->pSub_level();
      if (m_pCI->G_bEachMainWithSub(CK_PROJECT, CK_LEVEL)) {
        p_level_where->Y_vSet(P_Level,m_pCI->G.p_level->Y_iGet(P_Level));
      }
    }
  }

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS40_T20_Fold_W_A_C_ro(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  // W A C ro
  // push a (single-value aggregate correlated ro) sub-query before the main-query
  // as an aggregation-fan (projecting key-of-correlation and aggregated value)
  // further: the key-of-correlation needs to be added to group-by)
  // (refer: Q2,17)

  /* ARCHETYPE :
    select count(*)
      from Lineitem, Part, PartSupp
     where P_PARTKEY  = L_PARTKEY
       and PS_PARTKEY = P_PARTKEY
       and l_quantity < (select avg(l_quantity)
                           from lineitem
                          where L_PARTKEY = P_PARTKEY)
    ;
 */

  m_pCI->vTransform(__func__, "((20))"_Stz);

  // 1) find W_A_C_ro subquery call in mq:<where>/<boolCalc>
  vAssert_Board_mqA();
  RASSERT(m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC));
  if (!Board::bFindAndMarkSubQryCall(m_pCI)) {
    vTrace("DID NOT FIND W_A_C_ro SubQuery call\n"_Stz);
    NEVER_REACH
  }
  vTrace(">>>>>>>>> Found W_A_C_ro call:  %s %s %s\n"_Stz,m_pCI->m_sSqyArg.szStz(),m_pCI->m_sSqyRO.szStz(),m_pCI->m_sSqyNm.szStz());
  m_pCI->vPrintBoard(__LINE__);


  m_pCI->G_vNewMember(CK_FOLD,"W_A_C_ro"_Stz);
  m_pCI->G.pDeck->vPush(m_pCI->G.p_fold);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get copy of sq and push before mq

  // x) push to sq
  if (! m_pCI->G_bPushBoard(m_pCI->m_sSqyNm)) {
    vTrace("DID NOT FIND Board %s\n"_Stz,m_pCI->m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  m_pCI->m_pCard_copied = m_pCI->G.pDeck->pDupSubCards(1);

  m_pCI->vPrintBoard(__LINE__);

  // ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^

  // x) pop back to mq
  m_pCI->G_vPopIter();
  vAssert_Board_mqA();

  m_pCI->G.pDeck->vPush(m_pCI->m_pCard_copied);
  m_pCI->G.pDeck->vReplaceChildWithItsSubs(m_pCI->m_pCard_copied);
  DELETE_CARD(m_pCI->m_pCard_copied);

  m_pCI->G.pDeck->vRenumberRecursive();

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // convert the parameterised aggreg-sq into a gbyAgg-sq gby correlation column
  // such that the result of the sq is a stream { __SQ1IGBKY , __SQ1RESULT }

  m_pCI->G_vRestart();

  // v                             //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                             //  ..]cmpPred ==
  // f                             //           $d.2  ((m_pCI->m_pTerm_corrPred))
  //                               //             %1
  RASSERT(m_pCI->G_bEachMainWith2Sub(CK_TABLE,CK_STREAM,CK_FILTER));
  Board::vIdentify_Correlation_Column_In_cmpPred_Of_sq_boolCalc(m_pCI);
  RASSERT(m_pCI->m_pTerm_corrPred); // eg $d.2 (L_PARTKEY)
  Card_StmCol::vFind_stmCol_Definition_Of_Correlation_Column(m_pCI);
  RASSERT(m_pCI->G.p_stmCol);       // eg L_PARTKEY


  // T A A A P F T T T W A A A     //
  //   v v v                       //
  // T G G G P F T T T W A A A     // push corrPred <expr> into <gbyElist> and <aggElist>

  RASSERT(m_pCI->G_bEachMain(CK_AGGREG));
  Board::vEnrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Column(m_pCI);

  m_pCI->vPrintBoard(__LINE__);

  // T A A A P F T T T W A A A     //
  //         v                     //
  // T G G G P F T T T W A A A     // push corrPred <expr> into <projElist>

  if (m_pCI->G_bIsNextMain(CK_PROJECT)) {
    RASSERT(m_pCI->G_bEachMain(CK_PROJECT));
    Board::vEnhance_sq_project_To_Include_Correlation_Column(m_pCI);
  }
  else {

  }
  RASSERT(m_pCI->G_bEachMain(CK_FOLD));
  Board::vDerive_foldElist_From_project_stream(m_pCI);

  m_pCI->G.pDeck->vRenumberRecursive();
  Board::vRelabelExpressions(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  // T G G G P F T T T W A A A      //
  //             s s s              // adjust stream ordinsla

  Card_Stream::vStream_AdjustLoHi(m_pCI);
  Board::vRenumberStmOdn(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // convert the mq to join to the gby correlation column streamed from sq

  m_pCI->G_vRestart();

  // v                             //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                             //  ..]cmpPred ==
  // f                             //           $d.2  ((m_pCI->m_pTerm_corrPred))
  //                               //             %1

  RASSERT(m_pCI->G_bEachMainWith2Sub(CK_TABLE,CK_FILTER,CK_BOOLCALC))
  bool b = m_pCI->G.p_boolCalc->bYank_PRED(MARK_AT, m_pCI->G.p_PRED);
  RASSERT(b);
  RASSERT(m_pCI->G.p_PRED->Y_bHas(P_CorrPred));
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.p_table->vRemove_filter_IfEmpty(m_pCI->G.p_filter);
  m_pCI->vPrintBoard(__LINE__);

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                 f           //  ..]cmpPred ==
  // f                 bc          //           $d.2  ((m_pCI->m_pTerm_corrPred))
  //                               //             %1

  RASSERT(m_pCI->G_bEachMain(CK_FOLD));
  RASSERT(m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC));
  m_pCI->G.p_boolCalc->vPush_Correlated_cmpPred_Onto_mq_boolCalc(m_pCI->G.p_PRED);
  m_pCI->vPrintBoard(__LINE__);

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                 f           //  ..]roPred <
  // f                 bc          //           $a.5
  //                               //           $b.1
  //                               // __subquery0002

  Board::vResolve_SubqueryCall_In_mq_boolCalc_Into_cmpPred(m_pCI, m_pCI->G.p_boolCalc);
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // move the correlation predicate to table as a bounds

  // TODO: this should be in two steps

  m_pCI->G_vRestart();

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                 f           //  ..]cmpPred ==
  // f                 bc          //           $d.2  ((m_pCI->m_pTerm_corrPred))
  //             b <-  cp          //             %1

  RASSERT(m_pCI->G_bEachMain(CK_FOLD));
  RASSERT(m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC));
  Board::vMove_corrPred_To_mq_table_As_bounds(m_pCI, m_pCI->G.p_boolCalc);
  m_pCI->vPrintBoard(__LINE__);


//  m_pCI->G_vRestart();
//  if (m_pCI->G_bEachMain(CK_FOLD)) {
//    if (m_pCI->G_bEachMain(CK_WHERE)) {
//      // TODO:
//      // R_vUTE_Unproject_foldExpr_After_where();
//      R_vUTE_Bump_where_BaseOn_foldExpr();
//    }
//  }

  vIsFlat(true);
  m_pCI->G.pDeck->vClearMarksRecursive();
  m_pCI->vPrintBoard(__LINE__);
  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS40_T30_Fold_W_A_NC_I(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  // W A NC I
  // push a (single-value aggregate non-correlated in) sub-query before the main-query
  // as an aggregation-fan (projecting a single aggregated value)
  // (refer: Q18)

  /* ARCHETYPE :
      select count(*)
        from orders, lineitem
       where O_ORDERKEY = L_ORDERKEY
         and O_ORDERKEY in (select L_ORDERKEY
                              from lineitem
                             group by L_ORDERKEY
                            having sum(l_quantity) > 100)
      ;
   */

  m_pCI->vTransform(__func__, "((30))"_Stz);

  vAssert_Board_mqA();


  // W A NC I
  //               //
  // T T W A A A   // find W_L_NC_I subquery call in mq:<where>/<boolCalc>
  //     f         //
  //     bc        //
  //    @ro        //

  vAssert_Board_mqA();
  if (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    if (!Board::bFindAndMarkSubQryCall(m_pCI)) {
      m_pCI->vTracer("DID NOT FIND W_A_NC_I SubQuery call\n"_Stz);
      NEVER_REACH
    }
  }
  vTrace("Found W_A_NC_I call to %s(%s)->%s\n"_Stz,m_pCI->m_sSqyNm.szStz(),m_pCI->m_sSqyArg.szStz(),m_pCI->m_sSqyRsl.szStz());

  m_pCI->G_vNewMember(CK_FOLD,"W_A_NC_I"_Stz);
  m_pCI->G.pDeck->vPush(m_pCI->G.p_fold);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // x) push to sq
  if (! m_pCI->G_bPushBoard(m_pCI->m_sSqyNm)) {
    vTrace("DID NOT FIND Board %s\n"_Stz,m_pCI->m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  m_pCI->m_pCard_yanked = m_pCI->G.pDeck->pDupSubCards(0);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // x) pop back to mq
  m_pCI->G_vPopIter();
  vAssert_Board_mqA();

  // W A NC I
  //       I-sqr-> $1            // Folded nc sq provides sqr into mq as $1
  //               ----mq-----   //
  // T G G G H P F T T W A A A   // push sq before Fold
  // ----sq-----                 //

  m_pCI->G.pDeck->vPush(m_pCI->m_pCard_yanked);
  m_pCI->G.pDeck->vReplaceChildWithItsSubs(m_pCI->m_pCard_yanked);
  DELETE_CARD(m_pCI->m_pCard_yanked);

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G_vRestart();

  // W A NC I
  //       I-sqr-> $1            // Put an expr describing sqr into Fold
  //           v v               //
  // T G G G H P F T T W A A A   // derive foldElist from sq Project
  //           s el              //

  RASSERT(m_pCI->G_bEachMainWithSub(CK_PROJECT,CK_STREAM));
  RASSERT(m_pCI->G_bEachMain(CK_FOLD));
  Board::vDerive_foldElist_From_project_stream(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  // W A NC I
  //       I-sqr-> $1            // Injection of sqr into mq adds new $1
  //                     v v v   //
  // T G G G H P F T T W G G G   // bump stmOdn:$n in mq gbyElist by 1
  //                     elelel  // (if G is actually G -- might be A)

  if (m_pCI->G_bEachMainWithSub(CK_GBYAGG,CK_GBYELIST)) {
    i8 nBumpRight = m_pCI->G.p_fold->Y_iGet(P_BumpRight);
    m_pCI->G.p_gbyElist->vBump_stmOdn_In_rpnCalc(m_pCI);

    m_pCI->G.p_fold->Y_vCut(P_BumpRight);
    m_pCI->G.p_fold->Y_vSet(P_BumpedRight,(nBumpRight));
  }

  // W A NC I
  //       I-sqr-> $1            // The mq IN transforms to arg == $1 (the corr pred)
  //                             //
  // T G G G H P F T T W G G G   // find table for sqy arg (will be an mq table)
  //               f             // derive cmpPred from sqy arg
  //               bc            // add cmpPred to table
  //               cp  @ro       //

  char cTblSyn = Card::cTableColSyn(m_pCI->m_sSqyArg);
  m_pCI->G.p_table = m_pCI->G.pDeck->pFindTableCard(cTblSyn, m_pCI->G.p_MainCard);

  m_pCI->G_vNewMember(CK_CMPPRED,"=="_Stz);
  m_pCI->G_vNewMember(CK_TERM,m_pCI->m_sSqyArg.szStz());
  m_pCI->G.p_cmpPred->vAppend(m_pCI->G.p_term);
  //m_pCI->G_vNewMember(CK_TERM,m_pCI->m_sSqyRsl.szStz());
  m_pCI->G_vNewMember(CK_TERM,"$1"_Stz);
  m_pCI->G.p_cmpPred->vAppend(m_pCI->G.p_term);

  m_pCI->G.p_table->vAddPredToTableFilter(m_pCI,m_pCI->G.p_cmpPred);
  m_pCI->vPrintBoard(__LINE__);

  // W A NC I
  //       I-sqr-> $1            // The corr pred in mq is a new bounds
  //                             //
  // T G G G H P F T T W G G G   // derive bounds from filter
  //               b             //
  //               ^             //
  //               f             //
  //               bc            //
  //              @cp            //

  m_pCI->G.p_table->vBounds_DeriveFrom_filter(m_pCI->G.p_filter,false);
  m_pCI->vPrintBoard(__LINE__);

  // W A NC I
  //       I-sqr-> $1            // Moved corr pred can be culled
  //                             //
  // T G G G H P F T T W G G G   // cull @cp from filter
  //               b             //
  //               f             //
  //               bc            //

  m_pCI->G.p_boolCalc = m_pCI->G.p_filter->pSub_boolCalc();
  m_pCI->G.p_boolCalc->vCullPred(MARK_UP);
  m_pCI->vPrintBoard(__LINE__);

  // W A NC I
  //       I-sqr-> $1            // Culled corr pred may leave filter empty
  //                             //
  // T G G G H P F T T W G G G   // delete empty filter
  //               b             //

  Board::vS36_T30_Delete_Empty_filter(m_pCI);

  // W A NC I
  //       I-sqr-> $1            // The original ro pred can now be removed
  //                             //
  // T G G G H P F T T W A A A   // cull the ro pred
  //                   f         //
  //                   bc        //
  //                  @ro (cull) //

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }

  m_pCI->vPrintBoard(__LINE__);

  // W A NC I
  //       I-sqr-> $1            // Removal of ro pred may leave Where empty
  //                             //
  // T G G G H P F T T W A A A   // delete potentially empty where
  //                   f         //
  //                   bc        //
  //                 (empty)     //

  m_pCI->G_vRestart();
  m_pCI->G_vDeleteWhenEmptySub(CK_WHERE,CK_BOOLCALC);

  m_pCI->vPrintBoard(__LINE__);


  // todo: should this be earlier? As soon as $1 is injected into mq??

  Card_Stream::vStream_AdjustLoHi(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  Board::vRenumberStmOdn(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  Board::vRelabelExpressions(m_pCI);

  vIsFlat(true);
  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS40_T40_Fold_W_A_NC_ro(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  // W A NC ro
  // push a (single-value aggregate non-correlated in) sub-query before the main-query
  // as an aggregation-fan (projecting a single aggregated value)
  // (refer: Q15)

  /*
   */

  m_pCI->vTransform(__func__, "((30))"_Stz);

  vAssert_Board_mqA();


  // 1) find W_L_C_E subquery call in mq:<where>/<boolCalc>
  vAssert_Board_mqA();
  if (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    if (!Board::bFindAndMarkSubQryCall(m_pCI)) {
      m_pCI->vTracer("DID NOT FIND W_A_NC_ro SubQuery call\n"_Stz);
      NEVER_REACH
    }
  }
  vTrace("Found W_A_NC_ro call to %s()->%s\n"_Stz,m_pCI->m_sSqyNm.szStz(),m_pCI->m_sSqyRsl.szStz());

  m_pCI->G_vNewMember(CK_FOLD,"W_A_NC_I"_Stz);
  m_pCI->G.pDeck->vPush(m_pCI->G.p_fold);

  /*
  // find table for sqyarg
  char cTblSyn = Card::cTableColSyn(m_pCI->m_sSqyArg);
  m_pCI->G.p_table = m_pCI->G.pDeck->pFindTableCard(cTblSyn, m_pCI->G.p_MainCard);

  // TODO: derive cmpPred from marked tstPred and add to corresponding table

  m_pCI->G_vNewMember(CK_CMPPRED,"=="_Stz);
  m_pCI->G_vNewMember(CK_TERM,m_pCI->m_sSqyArg.szStz());
  m_pCI->G.p_cmpPred->vAppend(m_pCI->G.p_term);
  //m_pCI->G_vNewMember(CK_TERM,m_pCI->m_sSqyRsl.szStz());
  m_pCI->G_vNewMember(CK_TERM,"$1"_Stz);
  m_pCI->G.p_cmpPred->vAppend(m_pCI->G.p_term);

  R_vUTE_AddPredToTableFilter(m_pCI->G.p_table,m_pCI->G.p_cmpPred);
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vBoolCalc_CullPred(MARK_AT);
  }

  Board::vRelabelExpressions(m_pCI);

  m_pCI->vPrintBoard(__LINE__);

  // delete potentially empty where
  m_pCI->G_vRestart();
  m_pCI->G_vDeleteWhenEmptySub(CK_WHERE,CK_BOOLCALC);

  m_pCI->vPrintBoard(__LINE__);

*/

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // x) push to sq
  if (! m_pCI->G_bPushBoard(m_pCI->m_sSqyNm)) {
    vTrace("DID NOT FIND Board %s\n"_Stz,m_pCI->m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  m_pCI->m_pCard_yanked = m_pCI->G.pDeck->pDupSubCards(0);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // x) pop back to mq
  m_pCI->G_vPopIter();
  vAssert_Board_mqA();

  m_pCI->G.pDeck->vPush(m_pCI->m_pCard_yanked);
  m_pCI->G.pDeck->vReplaceChildWithItsSubs(m_pCI->m_pCard_yanked);
  DELETE_CARD(m_pCI->m_pCard_yanked);

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G_vRestart();

  while (! m_pCI->G_bIsNextMain(CK_FOLD)) {

    if (m_pCI->G_bIsNextMain(CK_TABLE)) {
      RASSERT(m_pCI->G_bEachMain());
      continue;
    }
    else
    if (m_pCI->G_bIsNextMain(CK_PROJECT)) {
      RASSERT(m_pCI->G_bEachMainWithSub(CK_PROJECT, CK_STREAM));
      break;
    }
    else
    if (m_pCI->G_bIsNextMain(CK_AGGREG)) {
      RASSERT(m_pCI->G_bEachMainWithSub(CK_AGGREG, CK_PROJELIST));
      m_pCI->G.p_stream = Card_Stream::pDerive_Of_aggreg((Card_AggElist *) (m_pCI->G.p_projElist));
      m_pCI->G.p_aggreg->vAppend(m_pCI->G.p_stream);
      RASSERT(m_pCI->G_bEachMainWithSub(CK_AGGREG, CK_PROJELIST));
      m_pCI->G.p_stream = Card_Stream::pDerive_Of_aggreg((Card_AggElist *) (m_pCI->G.p_projElist));
      m_pCI->G.p_aggreg->vAppend(m_pCI->G.p_stream);
      RASSERT(m_pCI->G_bEachMainWithSub(CK_AGGREG, CK_PROJELIST));
      m_pCI->G.p_stream = Card_Stream::pDerive_Of_aggreg((Card_AggElist *) (m_pCI->G.p_projElist));
      m_pCI->G.p_aggreg->vAppend(m_pCI->G.p_stream);
      break;
    }
    else {

    }
  }

  RASSERT(m_pCI->G_bEachMain(CK_FOLD));
  Board::vDerive_foldElist_From_project_stream(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  if (m_pCI->G_bEachMainWithSub(CK_GBYAGG,CK_GBYELIST)) {
    i8 nBumpRight = m_pCI->G.p_fold->Y_iGet(P_BumpRight);
    m_pCI->G.p_gbyElist->vBump_stmOdn_In_rpnCalc(m_pCI);

    m_pCI->G.p_fold->Y_vCut(P_BumpRight);
    m_pCI->G.p_fold->Y_vSet(P_BumpedRight,(nBumpRight));
  }

  // find table for sqyarg
  char cTblSyn = Card::cTableColSyn(m_pCI->m_sSqyArg);
  m_pCI->G.p_table = m_pCI->G.pDeck->pFindTableCard(cTblSyn, m_pCI->G.p_MainCard);

  // TODO: derive cmpPred from marked tstPred and add to corresponding table

  m_pCI->G_vNewMember(CK_CMPPRED,"=="_Stz);
  m_pCI->G_vNewMember(CK_TERM,m_pCI->m_sSqyArg.szStz());
  m_pCI->G.p_cmpPred->vAppend(m_pCI->G.p_term);
  //m_pCI->G_vNewMember(CK_TERM,m_pCI->m_sSqyRsl.szStz());
  m_pCI->G_vNewMember(CK_TERM,"$1"_Stz);
  m_pCI->G.p_cmpPred->vAppend(m_pCI->G.p_term);

  m_pCI->G.p_table->vAddPredToTableFilter(m_pCI, m_pCI->G.p_cmpPred);
  m_pCI->vPrintBoard(__LINE__);


  // filter into bounds

  m_pCI->G.p_table->vBounds_DeriveFrom_filter(m_pCI->G.p_filter,false);
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.p_boolCalc = m_pCI->G.p_filter->pSub_boolCalc();
  m_pCI->G.p_boolCalc->vCullPred(MARK_UP);
  m_pCI->vPrintBoard(__LINE__);

  Board::vS36_T30_Delete_Empty_filter(m_pCI);

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }


  m_pCI->vPrintBoard(__LINE__);

  // delete potentially empty where
  m_pCI->G_vRestart();
  m_pCI->G_vDeleteWhenEmptySub(CK_WHERE,CK_BOOLCALC);

  m_pCI->vPrintBoard(__LINE__);


  Card_Stream::vStream_AdjustLoHi(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  Board::vRenumberStmOdn(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  Board::vRelabelExpressions(m_pCI);

  vIsFlat(true);
  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vPrintBoard(__LINE__);


  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS40_T50_Fold_W_L_C_E(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  // W L C E
  // push a (linear correlated exists) sub-query before the main-query
  // as a keyExists.  join ordering will be required to make it runnable
  // (refer: Q4)

  // assumptions
  // A1) sq is a single <table>
  // A2) will have index and bound [BVO: vSeries_42_Bound_Analysis]

  /* ARCHETYPE :
      select count(*)
        from orders
       where exists (select *
                       from lineitem
                      where L_ORDERKEY = O_ORDERKEY
                        and l_commitdate < l_receiptdate)
      ;
   */

  m_pCI->vTransform(__func__, "((20))"_Stz);
  vAssert_Board_mqA();

  // W L C E
  //             //
  //             //
  // T W A A A   // find W_L_C_E subquery call in mq:<where>/<boolCalc>
  //   f         //
  //   bc        //
  //  @ro exists //

  // 1) find W_L_C_E subquery call in mq:<where>/<boolCalc>
  if (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    if (!Board::bFindAndMarkSubQryCall(m_pCI)) {
      m_pCI->vTracer("DID NOT FIND W_L_C_E SubQuery call\n"_Stz);
      NEVER_REACH
    }
  }
  vTrace("Found W_L_C_E call to %s(%s)\n"_Stz,m_pCI->m_sSqyNm.szStz(),m_pCI->m_sSqyArg.szStz());
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 2) push to sq
  if (! m_pCI->G_bPushBoard(m_pCI->m_sSqyNm)) {
    vTrace("DID NOT FIND Board %s\n"_Stz,m_pCI->m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWithSub(CK_TABLE,CK_FILTER)) {
    m_pCI->G_vMorphCard(m_pCI->G.p_filter,CK_CONSTRAINT);
  }
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWith2Sub(CK_TABLE,CK_INDEX,CK_CONSTRAINT)) {
    m_pCI->G.p_table->vRemoveChild(m_pCI->G.p_constraint);
    m_pCI->G.p_index->vAppend(m_pCI->G.p_constraint);
  }
  m_pCI->vPrintBoard(__LINE__);

  // 3) get copy of sq:<table>/* into cc:<table>
  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_TABLE)) {
    m_pCI->G_vCopyCard(m_pCI->G.p_table);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 4) pop back to mq
  m_pCI->G_vPopIter();
  vAssert_Board_mqA();

  m_pCI->G_vNewMember(CK_FOLD,"W_L_C_E"_Stz);
  m_pCI->G_vInsertBefore(m_pCI->G.p_fold,m_pCI->G.p_where);


  // W L C E
  //                 //
  //       E-sqr->   // Insert sq table before fold
  // --    ----mq--- //
  // T T F W A A A   // put cc:<table>/* before mq:<where>
  //   --sq          //

  m_pCI->G_vInsertBefore(m_pCI->m_pCard_copied,m_pCI->G.p_fold);
  m_pCI->G_vAppendNewChild(m_pCI->m_pCard_copied, CK_KEYEXISTS);
  m_pCI->vPrintBoard(__LINE__);

  // W L C E
  //                 // Replace %1 parameter with calling argument
  //                 //
  // T T F W A A A   // %1 -> $t.n
  //   b             //
  //   %1->$t.c      //
  //   ke            //

  Card_Term::bReplaceValueOfSub(m_pCI, m_pCI->m_pCard_copied,CK_TERM,"%1"_Stz,m_pCI->m_sSqyArg);
  m_pCI->G_vCopyCard(nullptr);
  m_pCI->vPrintBoard(__LINE__);

  // W L C E
  //                 // Cull @ro exists from <where>/<filter>
  //                 //
  // T T F W A A A   //
  //       f         //
  //       bc        //
  //      @ro exists //

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }
  m_pCI->vPrintBoard(__LINE__);

  // W L C E
  //                 // Delete potentially empty where
  //                 //
  // T T F A A A     //
  //                 //

  m_pCI->G_vRestart();
  m_pCI->G_vDeleteWhenEmptySub(CK_WHERE,CK_BOOLCALC);
  m_pCI->vPrintBoard(__LINE__);

  vIsFlat(true);
  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS40_T60_Fold_W_L_NC_NI(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  // W L NC NI
  // push a (linear non-correlated not in) sub-query before the main-query
  // as a keyExists
  // (refer: Q16)

  /* ARCHETYPE :
      select count(*)
        from partsupp, part
       where PS_PARTKEY = P_PARTKEY
         and PS_SUPPKEY not in (select S_SUPPKEY
                                  from supplier
                                 where s_comment like '%Customer%Complaints%')
      ;
 */

  m_pCI->vTransform(__func__, "((20))"_Stz);

  // 1) find W_L_NC_NI subquery call in mq:<where>/<boolCalc>
  vAssert_Board_mqA();

  // W L NC NI
  //               //
  //               //
  // T T W A A A   // find W_L_NC_NI subquery call in mq:<where>/<boolCalc>
  //     f         //
  //     bc        //
  //    @ro notIn  //
  //        __sq2  // sub-query
  //           #1  // result
  //         $a.2  // argument


  if (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    if (!Board::bFindAndMarkSubQryCall(m_pCI)) {
      m_pCI->vTracer("DID NOT FIND W_L_NC_NI SubQuery call\n"_Stz);
      NEVER_REACH
    }
  }
  vTrace(">>>>>>>>> Found W_L_NC_NI call to %s(%s)->%s\n"_Stz,m_pCI->m_sSqyNm.szStz(),m_pCI->m_sSqyArg.szStz(),m_pCI->m_sSqyRsl.szStz());
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 2) push to sq
  if (! m_pCI->G_bPushBoard(m_pCI->m_sSqyNm)) {
    m_pCI->vTracer("DID NOT FIND Board %s\n"_Stz,m_pCI->m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  // x) pull <where> into <table>/<filter>



  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_TABLE)) {
    if (m_pCI->G_bEachMain(CK_WHERE)) {
      Card_Where::vMoveWhereToTableAsConstraint(m_pCI);
    }
  }
  m_pCI->vPrintBoard(__LINE__);

  // 3) get copy of sq:<table>/* into cc:<table>
  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_TABLE)) {
    m_pCI->G_vCopyCard(m_pCI->G.p_table);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 4) pop back to mq
  m_pCI->G_vPopIter();
  vAssert_Board_mqA();

  // 5) fold
  //vR_S40_T50a_Fold_W_L_C_E_Before_Mq_As_keyExists();
  // put cc:<table>/* before mq:<where>

  m_pCI->G_vNewMember(CK_FOLD,"W_L_NC_NI"_Stz);
  m_pCI->G_vInsertBefore(m_pCI->G.p_fold,m_pCI->G.p_where);

  // W L NC NI
  //                   //
  // ------  --mq----  //
  // T T T F W A A A   // find W_L_NC_NI subquery call in mq:<where>/<boolCalc>
  //     --sq          //
  //     kne           //

  m_pCI->G_vInsertBefore(m_pCI->m_pCard_copied,m_pCI->G.p_fold);
  m_pCI->G_vAppendNewChild(m_pCI->m_pCard_copied, CK_KEYNOTEXISTS);

  m_pCI->G.pDeck->vRenumberRecursive();
  Card_Stream::vStream_AdjustLoHi(m_pCI);
  Board::vRenumberStmOdn(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.p_bounds = Card_Bounds::pBounds_Create_Eq_ForGivenCol(m_pCI->m_sSqyArg);
  //m_pCI->m_pCard_copied->vAppend(m_pCI->G.p_bounds);

  m_pCI->G.p_indexList = m_pCI->G.p_table->pSub_indexList();
  m_pCI->G.p_index = m_pCI->G.p_table->pSub_index();
  m_pCI->G.p_index->vAppend(m_pCI->G.p_bounds);


  m_pCI->vPrintBoard(__LINE__);

  // W L NC NI
  //                   //
  // T T T F W A A A   //
  //     b             //
  //     kne           //

  m_pCI->G_vRestart();
  RASSERT(m_pCI->G_bEachMainWith2Sub(CK_TABLE,CK_KEYNOTEXISTS,CK_CONSTRAINT));
  m_pCI->G.p_table->vRemoveChild(m_pCI->G.p_constraint);
  m_pCI->G.p_indexList = m_pCI->G.p_table->pSub_indexList();
  m_pCI->G.p_index = m_pCI->G.p_table->pSub_index();
  m_pCI->G.p_index->vAppend(m_pCI->G.p_constraint);

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G_vCopyCard(nullptr);


  // W L NC NI
  //                 // Cull @ro notIn
  //                 //
  // T T T W A A A   //
  //       f         //
  //       bc        //
  //      @ro notIn  //
  //          __sq2  //
  //             #1  //
  //           $a.2  //

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }

  m_pCI->vPrintBoard(__LINE__);


  // W L NC NI
  //               // Delete potentially empty where
  //               //
  // T T T A A A   //
  //               //
  //               //

  // delete potentially empty where
  m_pCI->G_vRestart();
  m_pCI->G_vDeleteWhenEmptySub(CK_WHERE,CK_BOOLCALC);

  m_pCI->vPrintBoard(__LINE__);

  vIsFlat(true);
  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS40_T70_Fold_H_A_NC_ro(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  // H A NC ro
  // push a (single-value aggregate non-correlated ro) sub query before main queryy
  // as am aggregation-fan
  // further: the aggregation-value is injected/factored into the main-query
  // (refer: Q11)

  /* ARCHETYPE :
      select ps_partkey,
             sum(ps_supplycost * ps_availqty) as value
        from partsupp, supplier
       where PS_SUPPKEY = S_SUPPKEY
       group by ps_partkey
      having sum(ps_supplycost * ps_availqty) > (select avg(ps_supplycost * ps_availqty)
                                                   from partsupp, supplier
                                                  where PS_SUPPKEY = S_SUPPKEY)
      ;
 */

  m_pCI->vTransform(__func__, "((20))"_Stz);

  vAssert_Board_mqA();

  // H A NC ro
  //                  //
  //                  //
  // T T G G G H      //
  //           bc     //
  //          @ro     //
  //              $2  //
  //           __sq2  //

  // 1) find W_L_NC_NI subquery call in mq:<where>/<boolCalc>
  if (m_pCI->G_bEachMainWithSub(CK_HAVING, CK_BOOLCALC)) {
    if (!Board::bFindAndMarkSubQryCall(m_pCI)) {
      vTrace("DID NOT FIND H_A_NC_ro SubQuery call\n"_Stz);
      NEVER_REACH
    }
  }
  vTrace(">>>>>>>>> Found H_A_NC_ro call:  %s %s %s\n"_Stz,m_pCI->m_sSqyArg.szStz(),m_pCI->m_sSqyRO.szStz(),m_pCI->m_sSqyNm.szStz());
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 2) push to sq
  if (! m_pCI->G_bPushBoard(m_pCI->m_sSqyNm)) {
    m_pCI->vTracer("DID NOT FIND Board %s\n"_Stz,m_pCI->m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  m_pCI->m_pCard_yanked = m_pCI->G.pDeck->pDupSubCards(0);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 4) pop back to mq
  m_pCI->G_vPopIter();
  vAssert_Board_mqA();

  m_pCI->G_vNewMember(CK_FOLD,"H_A_NC_ro"_Stz);
  m_pCI->G.pDeck->vPush(m_pCI->G.p_fold);
//  m_pCI->G_vAppendNewChild(m_pCI->G.p_fold,CK_FOLDELIST);
//  m_pCI->G_vAppendNewChild(m_pCI->G.p_foldElist,CK_TYPE,"((type))"_Stz);
//  m_pCI->G_vAppendNewChild(m_pCI->G.p_foldElist,CK_AS,"__SQ1RESULT"_Stz);
//  m_pCI->G_vAppendNewChild(m_pCI->G.p_foldElist,CK_RPNCALC);
//  m_pCI->G_vAppendNewChild(m_pCI->G.p_rpnCalc,CK_TERM,"$1"_Stz);

  // H A NC ro
  //                               //
  //               --mq--------    //
  // T T A A A P F T T G G G H     //
  // --sq--------                  //


  m_pCI->G.pDeck->vPush(m_pCI->m_pCard_yanked);
  m_pCI->G.pDeck->vReplaceChildWithItsSubs(m_pCI->m_pCard_yanked);
  DELETE_CARD(m_pCI->m_pCard_yanked);

  m_pCI->vPrintBoard(__LINE__);

  Card_Stream::vStream_AdjustLoHi(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  Board::vRenumberStmOdn(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  // H A NC ro
  //                               //
  //                               //
  // T T A A A P F T T G G G H     //
  //             fe                //

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_FOLD)) {
    m_pCI->G.i_FoldCard = m_pCI->G.i_MainCard;
    Board::vDerive_foldExpr(m_pCI);
  }
  m_pCI->vPrintBoard(__LINE__);


  // H A NC ro
  //                               //
  //                               //
  // T T A A A P F T T G G G H     //
  //                   gegege      //
  //                   $1$1$1      //
  //                   aeaeae      //
  //                   k1k1k1      //

  Board::vFoldIn_foldExpr(m_pCI);
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  // H A NC ro
  //                               //
  //                               //
  // T T A A A P F T T G G G H P   //
  //                         bc    //
  //                        @ro    //
  //                         $3    //
  //                         $1    //

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_HAVING)) {
    Board::vUnproject_foldExpr_After_having(m_pCI);
    Board::vCorrect_having_BaseOn_foldExpr(m_pCI);
  }

  m_pCI->vPrintBoard(__LINE__);

  vIsFlat(true);
  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   vSeries_50_Join_Ordering */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS50_T00_Join_Ordering_v2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "((replace with an explanation))"_Stz);

  // vAssert_Board_mqA();

  Card_Deck* pPipeline = m_pCI->G.pDeck;

  Board* pBoard = new Board(aBK[BK_PARTITION],
                            BK_PARTITION,
                            C_UNKNOWN,
                            BT_ORIGINAL,
                            m_pCI->m_pBench);
  m_pCI->G_vPushBoard(pBoard);

  m_pCI->G.p_tableList = Card_TableList::pExtract_v2(pPipeline);

  m_pCI->G.p_joinPredList = Card_JoinPredList::pExtract(pPipeline);

  m_pCI->G.p_coPtnPredList = Card_CoPtnPredList::pExtract(m_pCI->G.p_joinPredList,m_pCI->G.p_tableList);

  m_pCI->G.p_coPtnJoinList = Card_CoPtnJoinList::pExtract(m_pCI->G.p_coPtnPredList);

  m_pCI->G.p_crossPtnJoinList = Card_CrossPtnJoinList::pExtract(m_pCI->G.p_joinPredList,
                                                                m_pCI->G.p_coPtnJoinList);

  m_pCI->G.p_ptndJoinSet = Card_PtndJoinSet::pExtract(m_pCI->G.p_coPtnJoinList,
                                                      m_pCI->G.p_crossPtnJoinList);

  m_pCI->G.pDeck->vAppend(m_pCI->G.p_tableList);
  m_pCI->G.pDeck->vAppend(m_pCI->G.p_joinPredList);
  m_pCI->G.pDeck->vAppend(m_pCI->G.p_coPtnPredList);
  m_pCI->G.pDeck->vAppend(m_pCI->G.p_coPtnJoinList);
  m_pCI->G.pDeck->vAppend(m_pCI->G.p_crossPtnJoinList);
  m_pCI->G.pDeck->vAppend(m_pCI->G.p_ptndJoinSet);

  m_pCI->vPrintBoard(__LINE__);

  // copy ptndJoinSet
  m_pCI->G.pDeck->vAppendMain(new Card_Note("PERMUTATIONS"_Stz));
  m_pCI->G.p_subSet = m_pCI->G.p_ptndJoinSet->pDupSubCards();
  while ((m_pCI->m_pCard_popped = m_pCI->G.p_subSet->pPop())) {
    m_pCI->G.pDeck->vAppendMain(m_pCI->m_pCard_popped);
  }
  DELETE_CARD(m_pCI->G.p_subSet);

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.p_ptndJoinSet->vPermute(m_pCI->G.pDeck);

  m_pCI->vPrintBoard(__LINE__);

  Card_Permutation::vRemoveInvalid(m_pCI->G.pDeck);

  m_pCI->vPrintBoard(__LINE__);

  i8 i_Permutation = m_pCI->G.pDeck->iFindChild(CK_PERMUTATION);
  m_pCI->G.p_subSet = m_pCI->G.pDeck->pDupSubCards(i_Permutation);


  pBoard = new Board(aBK[BK_COSTEST],
                     BK_COSTEST,
                     C_UNKNOWN,
                     BT_ORIGINAL,
                     m_pCI->m_pBench);

  m_pCI->G_vPushBoard(pBoard);

  m_pCI->G.pDeck->vTransferChildren(m_pCI->G.p_subSet);

  DELETE_CARD(m_pCI->G.p_subSet);


  m_pCI->vPrintBoard(__LINE__);


  Card_JoinOrdering::vEstimateJoinedBuff(m_pCI->G.pDeck, m_pCI->G.p_tableList);
  m_pCI->vPrintBoard(__LINE__);

  Card_JoinOrdering::vEstimateNetwork(m_pCI->G.pDeck);
  m_pCI->vPrintBoard(__LINE__);

  Card_JoinOrdering* p_joinOrdering = Card_JoinOrdering::pCompute(m_pCI->G.pDeck);

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.pDeck->vRemoveChild(p_joinOrdering);


  pBoard = m_pCI->G.pBoard;
  m_pCI->G_vPopIter();
  vRemove(pBoard);
  delete pBoard;

  //m_pCI->G.p_joinOrder = Card_JoinOrder::pExtract(m_pCI->G.pDeck);
  //m_pCI->G.pDeck->vApply(m_pCI->G.p_joinOrder);



  pBoard = m_pCI->G.pBoard;
  m_pCI->G_vPopIter();

  vRemove(pBoard);

  delete pBoard;

  p_joinOrdering->vReorder(m_pCI->G.pDeck);

  m_pCI->vPrintBoard(__LINE__);

  DELETE_CARD(p_joinOrdering);

  Card_Stream::vStream_AdjustLoHi(m_pCI);
  Board::vRenumberStmOdn(m_pCI);

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_75_EE_operators */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* convert all <table> into either <scan> , <indexJoin> or <keyExists>        */
/*----------------------------------------------------------------------------*/
void Bench::vS75_T10_Morph_table_To_EEop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "convert all <table> into either <scan> or <indexJoin>"_Stz);

  vAssert_Board_mqA();

  m_pCI->vPrintBoard(__LINE__);

  while (m_pCI->G_bEachMain(CK_TABLE)) {

    m_pCI->G.p_bounds         = m_pCI->G.p_table->pSub_bounds();
    m_pCI->G.p_index          = m_pCI->G.p_table->pSub_index();
    m_pCI->G.p_indexJoin      = m_pCI->G.p_table->pSub_indexJoin();
    m_pCI->G.p_indexList      = m_pCI->G.p_table->pSub_indexList();
    m_pCI->G.p_keyExists      = m_pCI->G.p_table->pSub_keyExists();
    m_pCI->G.p_keyNotExists   = m_pCI->G.p_table->pSub_keyNotExists();

//    vTrace("MainCard %p : [%lld] %s %s\n"_Stz,
//           m_pCI->G.p_table,m_pCI->G.p_table->iCardNo(),aCK[m_pCI->G.p_table->eCard()],m_pCI->G.p_table->szValue());

    RASSERT(! m_pCI->G.p_indexJoin);
    Board::vS75_T10_Morph_table_To_EEop_v2(m_pCI);

    m_pCI->G.pDeck->vRenumberRecursive();

    m_pCI->vPrintBoard(__LINE__);

    vDebugPoint();
  }

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_SCAN)) {
    i8 i_Child;
    m_pCI->G.p_note = _pCard_Note(m_pCI->G.p_scan->pFindChild(CK_NOTE,i_Child));

    if (i_Child == m_pCI->G.p_scan->nChildCards() - 1) {
      m_pCI->G.p_scan->vRemoveChild(m_pCI->G.p_note);
      DELETE_CARD(m_pCI->G.p_note);
    }
  }
  m_pCI->vPrintBoard(__LINE__);

//  m_pCI->G_vRestart();
//  while (m_pCI->G_bEachMain(CK_INDEXJOIN)) {
//    vR_S75_T10_Morph_table_To_EEop_c();
//  }
//  m_pCI->vPrintBoard(__LINE__);

//  m_pCI->G_vRestart();
//  while (m_pCI->G_bEachMain(CK_KEYEXISTS)) {
//    if (m_pCI->G.p_keyExists) {
//      i8 i_Child;
//      m_pCI->G.p_note = _pCard_Note(m_pCI->G.p_keyExists->pFindChild(CK_NOTE, i_Child));
//      m_pCI->G.p_note->vValue(SEP_DERIVED);
//    }
//  }
//  m_pCI->vPrintBoard(__LINE__);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS75_T20_Morph_keyExists_filter_To_constraint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "convert all <table> into either <scan> or <indexJoin>"_Stz);

  vAssert_Board_mqA();

  while (m_pCI->G_bEachMainWithSub(CK_KEYEXISTS,CK_FILTER)) {
    m_pCI->G.p_filter->vCard(CK_CONSTRAINT);
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "convert all <table> into either <scan> or <indexJoin>"_Stz);

  vAssert_Board_mqA();

  while (m_pCI->G_bEachMainWithSub(CK_KEYEXISTS,CK_CONSTRAINT)) {
    Board::vS75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(m_pCI);
  }

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                      vSeries_80_Index_Scan */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bS80_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS80_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS80_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->m_pCardCtl->vUseShorter();

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS80_T30_Morph_scan_To_indexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWith2Sub(CK_SCAN,CK_INDEXLIST,CK_BOUNDS)) {
    if (m_pCI->G.p_bounds->pSub_lBound() || m_pCI->G.p_bounds->pSub_uBound()) {
      m_pCI->G.p_scan->vCard(CK_INDEXSCAN);

      m_pCI->G.p_indexList->vChoose_index_For_indexScan();
    }
  }

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS80_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSTZZ_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                 vSeries_90_Stream_Ordering */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bS90_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS90_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS90_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  //m_pCI->m_pCardCtl->vUseShorter();

  m_pCI->m_pCardCtl->vPrintSubCards(true,
                                    CK_AGGELIST,
                                    CK_GBYELIST,
                                    CK_INDEXLIST,
                                    CK_PROJELIST,
                                    CK_RPNCALC,
                                    CK_STREAM,
                                    0);
  m_pCI->m_pCardCtl->vPrintSubCards(false,
                                    CK_STMCOLLIST,
                                    0);

  m_pCI->m_pCardCtl->vPrintBelow(CK_PROJECT,true);

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS90_T30_Derive_order(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "remove <sort> if <sbyElist> is the same as the <gbyElist>"_Stz);

  vAssert_Board_mqA();

  Board::vDerive_order(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vS90_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSTZZ_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                               vSeries_A0_Make_Weavable */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vSA0_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->m_pCardCtl->vUseShorter();

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/* inspect filters and remove cmp predicates which are in bounds              */
/*----------------------------------------------------------------------------*/
void Bench::vSA0_T05_Cull_Nonused_bounds_of_Chosen_index(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "((desc))"_Stz);

  vAssert_Board_mqA();

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain()) {

    if (!(m_pCI->G.p_MainCard->bIsCard(CK_INDEXJOIN) ||
          m_pCI->G.p_MainCard->bIsCard(CK_INDEXSCAN)))
      continue;
    if (!(m_pCI->G.p_indexList = m_pCI->G.p_MainCard->pSub_indexList()))
      continue;

    // have <bounds> & <filter> under <main>

    // find & ignore the first ebound
    bool bFoundFirst = false;

    CardIter xCI_indexes(m_pCI->G.p_indexList);
    while ((m_pCI->G.p_index = _pCard_Index(xCI_indexes.pEachChild(CK_INDEX)))) {

      bool bChosenIndex = m_pCI->G.p_index->Y_bHas(P_ChosenIndex);

      m_pCI->G.p_bounds = m_pCI->G.p_index->pSub_bounds();
      if (! m_pCI->G.p_bounds)
        continue;

      CardIter xCI_bounds(m_pCI->G.p_bounds);
      while (xCI_bounds.bEachChild()) {
        if (bChosenIndex && ! bFoundFirst && xCI_bounds.G.m_pChild->bIsCard(CK_EBOUND)) {
          bFoundFirst = true;
          continue;
        }

        xCI_bounds.G.m_pChild->vMark1(MARK_AT);
      }

      m_pCI->vPrintBoard(__LINE__);
      m_pCI->G.p_bounds->vCullBound(MARK_AT);

    }
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/* inspect filters and remove cmp predicates which are in bounds              */
/*----------------------------------------------------------------------------*/
void Bench::vSA0_T10_Eliminate_bounds_From_filter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "((desc))"_Stz);

  vAssert_Board_mqA();

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark any cmpPred that are a bound in bounds

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain()) {

    if (! (m_pCI->G.p_MainCard->bIsCard(CK_INDEXJOIN) ||
           m_pCI->G.p_MainCard->bIsCard(CK_INDEXSCAN)))
      continue;
    if (! (m_pCI->G.p_bounds = m_pCI->G.p_MainCard->pSub_bounds()))
      continue;
    if (! (m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter()))
      continue;

    // have <bounds> & <filter> under <main>

    CardIter xCR(m_pCI->G.p_filter);
    while ((m_pCI->G.p_cmpPred = _pCard_CmpPred(xCR.pRecurseEach(CK_CMPPRED)))) {
      if (m_pCI->G.p_cmpPred->bSubsetOf_bounds(m_pCI->G.p_bounds))
        m_pCI->G.p_cmpPred->vMark1(MARK_AT);
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cull marked cmpPred from filter

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain()) {
    if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter()))
      m_pCI->G.p_filter->vCullPred(MARK_AT);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete any filter if empty

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain()) {
    if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter()))
      m_pCI->G.p_MainCard->vDeleteFilterIfEmpty(m_pCI->G.p_filter);
  }

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* individuate <scan|indexJoin>/<filter> as main <filter>                     */
/*----------------------------------------------------------------------------*/
void Bench::vSA0_T20_Individuate_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "individuate <scan|indexJoin>/<filter> as main <filter>"_Stz);

  vAssert_Board_mqA();

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_SCAN,CK_FILTER)) {
    m_pCI->G.p_scan->vRemoveChild(m_pCI->G.p_filter);
    m_pCI->G.pDeck->vInsertAfter(m_pCI->G.p_filter, m_pCI->G.p_scan);

    if (! m_pCI->G.p_filter->pSub_level()) {
      m_pCI->G.p_filter->vPush(m_pCI->G.p_scan->pSub_level()->pDupCardAndSubCards());
    }
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_INDEXSCAN,CK_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_INDEXJOIN,CK_FILTER)) {
    Board::vSA0_T10_Individuate_indexJoin_filter_b(m_pCI);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_CROSSJOIN,CK_FILTER)) {
    Board::vSA0_T10c_Individuate_crossJoin_filter(m_pCI);
  }


  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vSA0_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vSTZZ_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                       vSeries_E0_To_StmOdn */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* individuate <scan|indexJoin>/<filter> as main <filter>                     */
/*----------------------------------------------------------------------------*/
void Bench::vSE0_T10_Convert_TblColOdn_To_StmOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "individuate <scan|indexJoin>/<filter> as main <filter>"_Stz);

  vAssert_Board_mqA();

  m_pCI->G.pBoard->vMayHaveStmColOdn(true);

  while (m_pCI->G_bEachMain()) {

    Board::vConvert_tblColOdn_To_stmOdn(m_pCI);
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vSE0_T20_Convert_tblColOdn_Of_constraint_To_stmColNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_pCI->vTransform(__func__, "((20))))"_Stz);

  vAssert_Board_mqA();

  m_pCI->G.pBoard->vMayHaveStmColOdn(true);

  while (m_pCI->G_bEachMain()) {

    if (! (   CK_KEYEXISTS    == m_pCI->G.e_MainCard
              || CK_KEYNOTEXISTS == m_pCI->G.e_MainCard
    ))
      continue;

    m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint();

    Board::vConvert_tblColOdn_Of_constraint_To_stmColNm(m_pCI);
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                vSeries_Z0_Restate_Pipeline */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* reinstate stages                                                           */
/*----------------------------------------------------------------------------*/
void Bench::vSZ0_T20_Reinstate_stagesV3(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "reinstate stages"_Stz);

  vAssert_Board_mqA();

  //m_pCI->m_pCardCtl->vPrintSubCards(true,CK_PROJELIST,0);
  //m_pCI->m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);

  m_pCI->vPrintBoard(__LINE__);

  Board::vSZ0_T20_Reinstate_stagesV3(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* add partitioning                                                           */
/*----------------------------------------------------------------------------*/
void Bench::vSZ0_T30_Add_Partitioning(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  m_pCI->vTransform(__func__, "add partitioning"_Stz);

  vAssert_Board_mqA();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Next* p_next_prior = nullptr;
  Card_Combined* p_combined_prior = nullptr;

  Card* p_main_prior = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  while (m_pCI->G_bEachMain()) {
    if (CK_INDEXJOIN == m_pCI->G.e_MainCard ||
        CK_INDEXSCAN == m_pCI->G.e_MainCard ||
        CK_KEYEXISTS == m_pCI->G.e_MainCard ||
        CK_KEYNOTEXISTS == m_pCI->G.e_MainCard ||
        CK_SCAN == m_pCI->G.e_MainCard) {
      if (p_main_prior) {
        i8 nSkip = Board::iStage_Partitioning(m_pCI, p_main_prior, p_combined_prior,
                                              p_next_prior, m_pCI->G.p_MainCard);
        while (nSkip--)
          m_pCI->G_vSkipNextMain();
        m_pCI->vPrintBoard(__LINE__);
        vDebugPoint();
      }
      p_main_prior = m_pCI->G.p_MainCard;
    }
    else
    if (CK_COMBINED == m_pCI->G.e_MainCard) {
      p_combined_prior = m_pCI->G.p_combined;
    }
    else
    if (CK_NEXT == m_pCI->G.e_MainCard) {
      p_next_prior = m_pCI->G.p_next;
    }

  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vAssert_Board_any(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  // nothing to do?
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vAssert_Board_mqA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  RASSERT(0 == m_pCI->G.pBoard->i_Board());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vAssert_Board_sqB(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  RASSERT(1 == m_pCI->G.pBoard->i_Board());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vST00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  if (! m_pCI->m_pCardCtl->bBoardPrint())
    return;

  char* azSeparator = (char*)
      "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>"
      "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>"
      "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>";

  vTrace("\n%s\n%186s\n%s\n"_Stz,
         azSeparator,
         m_pCI->szXhand(),
         azSeparator);

  m_pCI->vXdesc(""_Stz);
  m_pCI->vXform(""_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vSTZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
