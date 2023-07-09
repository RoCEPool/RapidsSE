/*============================================================================*/
/* XF_Flatten.cpp -- Flatten SubQueries                                */
/*============================================================================*/

#include "XF_Flatten.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Fold.hpp"
#include "CorrelatedJoin.hpp"
#include "Smoosh.hpp"
#include "Smooth.hpp"
#include "Optimiser.hpp"
#include "XF_Join_Ordering.hpp"

#include "XF_stmCol_To_tblCol.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten, CN_XF_FLATTEN)
__aNewFunction(XF_Flatten, CN_XF_FLATTEN)
__vDeleteFunction(XF_Flatten, CN_XF_FLATTEN)
__aFooterFunction(XF_Flatten)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten::XF_Flatten(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  m_pCI = pCI;

  _checkInitialised_(CN_XF_FLATTEN);

  m_pCI->vUsingBoard((i8)0);
  m_pCI->Sqy.m_pSubQuery = m_pCI->G.pBoard->pSubQuery();


  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten::~XF_Flatten(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlatten(CardIter* pCI)
{
  _static_

  XF_Flatten* pMakeFlat = new XF_Flatten(pCI);

  pMakeFlat->vFlatten();

  delete pMakeFlat;

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlatten(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  m_pCI->G.pBoard = m_pCI->m_pBench->pGetBoard((i8)0);
  m_pCI->Sqy.m_pSubQuery = m_pCI->G.pBoard->pSubQuery();

//  vFlatten_recurse();
  vFlatten_recurse(0);


  m_pCI->m_pBench->vMadeFlat(true);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlatten_recurse(i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  // things before sub-queries

  SubQuery* pOuterQuery = m_pCI->Sqy.m_pSubQuery;

  XF_stmCol_To_tblCol::vXF_stmCol_To_tblCol(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // flatten the sub-queries of this outer query

  for (;;) {

    m_pCI->vRestart();
    if (! m_pCI->G_bEachMain(CK_CALL))
      break;

    s256 sSubQueryNm = m_pCI->G.p_call->szSymbol();

    Board *pOuterBoard_save = m_pCI->G.pBoard;
    m_pCI->Sqy.m_pSubQuery = m_pCI->m_pBench->pGetSubQuery(sSubQueryNm);
    m_pCI->G_vPushBoard(m_pCI->Sqy.m_pSubQuery->pBoard());
    m_pCI->Sqy.m_pSubQuery = m_pCI->G.pBoard->pSubQuery();

    vFlatten_recurse(iDepth + 1);

    m_pCI->G_vPopIter();
    m_pCI->G_vSetBoard(pOuterBoard_save);
    SubQuery *pSubQuery = m_pCI->Sqy.m_pSubQuery = m_pCI->m_pBench->pGetSubQuery(sSubQueryNm);
    m_pCI->vPrintBoard(__LINE__);

    vFlattenByDecisionTree();

    m_pCI->vPrintBoard(__LINE__);
  }

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  bool bFound;

  do {
     m_pCI->vRestart();

    if (! m_pCI->G_bEachMainWithSub_reverse_foldBreak(CK_WHERE, CK_BOOLCALC))
      break;

    CardIter* pCI = m_pCI;

    auto bLambdaFn = [&bFound,this,pCI, iDepth](Card_Term* p_term)
    {
       if (!pCI->G_bIsBoardNm(p_term->szSymbol()))
         return C_CONTINUE_RECURSION;

      s256 sSubQueryNm = p_term->szSymbol();

      Board* pOuterBoard_save = pCI->G.pBoard;
      pCI->Sqy.m_pSubQuery = pCI->m_pBench->pGetSubQuery(sSubQueryNm);
      pCI->G_vPushBoard(pCI->Sqy.m_pSubQuery->pBoard());
      pCI->Sqy.m_pSubQuery = pCI->G.pBoard->pSubQuery();

      vFlatten_recurse(iDepth + 1);

      pCI->G_vPopIter();
      pCI->G_vSetBoard(pOuterBoard_save);
      SubQuery* pSubQuery = pCI->Sqy.m_pSubQuery = pCI->m_pBench->pGetSubQuery(sSubQueryNm);
      pCI->vPrintBoard(__LINE__);

      vFlattenByDecisionTree();

      pCI->vPrintBoard(__LINE__);

      bFound = true;
      return C_TERMINATE_RECURSION;
    };


    bFound = false;
    bool b = m_pCI->G.p_boolCalc->bApplyRecursive_term(bLambdaFn);
    vDebugPoint();
  } while (bFound);


  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  do {
    m_pCI->vRestart();

    if (! m_pCI->G_bEachMainWithSub_reverse_foldBreak(CK_HAVING, CK_BOOLCALC))
      break;

    CardIter* pCI = m_pCI;

    auto bLambdaFn = [&bFound,this,pCI, iDepth](Card_Term* p_term)
    {
      if (!pCI->G_bIsBoardNm(p_term->szSymbol()))
        return C_CONTINUE_RECURSION;

      s256 sSubQueryNm = p_term->szSymbol();

      Board* pOuterBoard_save = pCI->G.pBoard;
      pCI->Sqy.m_pSubQuery = pCI->m_pBench->pGetSubQuery(sSubQueryNm);
      pCI->G_vPushBoard(pCI->Sqy.m_pSubQuery->pBoard());
      pCI->Sqy.m_pSubQuery = pCI->G.pBoard->pSubQuery();

      vFlatten_recurse(iDepth + 1);

      pCI->G_vPopIter();
      pCI->G_vSetBoard(pOuterBoard_save);
      SubQuery* pSubQuery = pCI->Sqy.m_pSubQuery = pCI->m_pBench->pGetSubQuery(sSubQueryNm);
      pCI->vPrintBoard(__LINE__);

      vFlattenByDecisionTree();

      pCI->vPrintBoard(__LINE__);

      bFound = true;
      return C_TERMINATE_RECURSION;
    };


    bFound = false;
    bool b = m_pCI->G.p_boolCalc->bApplyRecursive_term(bLambdaFn);
    vDebugPoint();
  } while (bFound);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // things to do to this outer query after flattening it's sub-queries

  XF_Join_Ordering::vXF_Join_Ordering(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sub-query has been flattened

  Card_Fold::vMorph_to_flat(m_pCI);

  m_pCI->vPrintBoard(__LINE__);

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlattenByDecisionTree(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  m_pCI->vTransform(__func__, XF_NONE, "Flatten by decision tree (recursive model)"_Stz);

  if (m_pCI->Sqy.m_pSubQuery->bViaFrom())                /* Fsq_      Q7,8,9 */ vFlatten_10_F();
  else
  if (m_pCI->Sqy.m_pSubQuery->bViaWhere()) {
    if (m_pCI->Sqy.m_pSubQuery->bIsAggregation()) {
      if (m_pCI->Sqy.m_pSubQuery->bIsCorrelated()) {                                /* 20*/
             if (m_pCI->Sqy.m_pSubQuery->bByExists())    /* W_A_C_E          */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotExists()) /* W_A_C_NE         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByIn())        /* W_A_C_I          */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotIn())     /* W_A_C_NI         */ NEVER_REACH
        else                                             /* W_A_C_ro   Q2,17 */ vFlatten_24_W_A_C_ro();
      } else {                                                                      /* 30*/
             if (m_pCI->Sqy.m_pSubQuery->bByExists())    /* W_A_NC_E         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotExists()) /* W_A_NC_NE        */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByIn())        /* W_A_NC_I     Q18 */ vFlatten_32_W_A_NC_I();
        else if (m_pCI->Sqy.m_pSubQuery->bByNotIn())     /* W_A_NC_NI        */ NEVER_REACH
        else                                             /* W_A_NC_ro    Q15 */ vFlatten_34_W_A_NC_ro();
      }
    } else {
      if (m_pCI->Sqy.m_pSubQuery->bIsCorrelated()) {                                /* 40*/
             if (m_pCI->Sqy.m_pSubQuery->bByExists())    /* W_L_C_E       Q4 */ vFlatten_40_W_L_C_E();
        else if (m_pCI->Sqy.m_pSubQuery->bByNotExists()) /* W_L_C_NE         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByIn())        /* W_L_C_I          */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotIn())     /* W_L_C_NI         */ NEVER_REACH
        else                                             /* W_L_C_ro         */ NEVER_REACH
      } else {                                                                      /* 50*/
             if (m_pCI->Sqy.m_pSubQuery->bByExists())    /* W_L_NC_E         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotExists()) /* W_L_NC_NE        */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByIn())        /* W_L_NC_I         */ vFlatten_52_W_L_NC_I_join();
        else if (m_pCI->Sqy.m_pSubQuery->bByNotIn())     /* W_L_NC_NI    Q16 */ vFlatten_53_W_L_NC_NI_join();
        else                                             /* W_L_NC_ro        */ NEVER_REACH
      }
    }
  }
  else
  if (m_pCI->Sqy.m_pSubQuery->bViaHaving()) {
    if (m_pCI->Sqy.m_pSubQuery->bIsAggregation()) {
      if (m_pCI->Sqy.m_pSubQuery->bIsCorrelated()) {                                /* 60*/
             if (m_pCI->Sqy.m_pSubQuery->bByExists())    /* H_A_C_E          */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotExists()) /* H_A_C_NE         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByIn())        /* H_A_C_I          */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotIn())     /* H_A_C_NI         */ NEVER_REACH
        else                                             /* H_A_C_ro         */ NEVER_REACH
      } else {                                                                      /* 70*/
             if (m_pCI->Sqy.m_pSubQuery->bByExists())    /* H_A_NC_E         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotExists()) /* H_A_NC_NE        */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByIn())        /* H_A_NC_I         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotIn())     /* H_A_NC_NI        */ NEVER_REACH
        else                                             /* H_A_NC_ro    Q11 */ vFlatten_74_H_A_NC_ro();
      }
    } else {
      if (m_pCI->Sqy.m_pSubQuery->bIsCorrelated()) {                                /* 80*/
             if (m_pCI->Sqy.m_pSubQuery->bByExists())    /* H_L_C_E          */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotExists()) /* H_L_C_NE         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByIn())        /* H_L_C_I          */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotIn())     /* H_L_C_NI         */ NEVER_REACH
        else                                             /* H_L_C_ro         */ NEVER_REACH
      } else {                                                                      /* 90*/
             if (m_pCI->Sqy.m_pSubQuery->bByExists())    /* H_L_NC_E         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotExists()) /* H_L_NC_NE        */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByIn())        /* H_L_NC_I         */ NEVER_REACH
        else if (m_pCI->Sqy.m_pSubQuery->bByNotIn())     /* H_L_NC_NI        */ NEVER_REACH
        else                                             /* H_L_NC_ro        */ NEVER_REACH
      }
    }
  }
  else
    NEVER_REACH;

  m_pCI->vEndTransform();

  Bench::vValidate(m_pCI);

}



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlatten_10_F(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

  // F
  // (refer: Q7,8,9)

  /*
      select count(*)
        from (select *
                from orders, lineitem
               where L_ORDERKEY = O_ORDERKEY)
      ;
   */

  m_pCI->vTransform(__func__, XF_NONE, "((vFlatten_10_F))"_Stz);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of from

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
  CarryForward* pCarryForward = pFold->pFold_from("From"_Stz,m_pCI);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing

  pCarryForward->Table.m_bReproject = true;
  Smooth* pSmooth = new Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);

  m_pCI->G.pDeck->vProjectionUpdate(m_pCI);
  m_pCI->vRenumberCards();
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vPrintBoard(__LINE__);


  // todo: discuss with Alka
  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_FOLD)) {
    if (m_pCI->G_bEachMain(CK_WHERE)) {
      Card_Level* p_level_where = m_pCI->G.p_where->pSub_level();
      if (m_pCI->G_bEachMainWithSub(CK_PROJECT, CK_LEVEL)) {
        p_level_where->pSymbol()->Y_vSet(P_Level, m_pCI->G.p_level->pSymbol()->Y_iGet(P_Level));
      }
    }
  }

  m_pCI->vEndTransform();

  delete pSmooth;
  delete pCarryForward;
  delete pFold;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlatten_24_W_A_C_ro(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

  // W A C ro

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

  m_pCI->vTransform(__func__, XF_NONE, "((vFlatten_24_W_A_C_ro))"_Stz);

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
  pFold->vFold_basic(m_pCI,"24_W_A_C_ro"_Stz,CK_WHERE);
  m_pCI->vPrintBoard(__LINE__);

  CorrelatedJoin* pCorrelatedJoin = new CorrelatedJoin;
  pCorrelatedJoin->vVia_gbyAgg_Promotion(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  pFold->vDerive_Correleted_fold_projection(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.pDeck->vClearMarksRecursive();
  m_pCI->vPrintBoard(__LINE__);
  m_pCI->vEndTransform();

  delete pCorrelatedJoin;
  delete pFold;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlatten_32_W_A_NC_I(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

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

  m_pCI->vTransform(__func__, XF_NONE, "((vFlatten_32_W_A_NC_I))"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of subQuery

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
  CarryForward* pCarryForward =
      pFold->pFold_nc_subQuery(m_pCI, "W_A_NC_I"_Stz,
                               CK_WHERE, QO_IN_OP, SM_NONE);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing of: subQuery | Fold | Table* [Where] GbyAgg_PNO [Project (avg)] Having

  Smooth* pSmooth = new Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);

  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->vEndTransform();

  delete pCarryForward;
  delete pSmooth;
  delete pFold;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlatten_34_W_A_NC_ro(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

  // W A NC ro
  // push a (single-value aggregate non-correlated in) sub-query before the main-query
  // as an aggregation-fan (projecting a single aggregated value)
  // (refer: Q15)

  /*
   */

  m_pCI->vTransform(__func__, XF_NONE, "((vFlatten_34_W_A_NC_ro))"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of subQuery

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
  CarryForward* pCarryForward =
      pFold->pFold_nc_subQuery(m_pCI, "W_A_NC_ro"_Stz,
                               CK_WHERE, QO_RELN_OP, SM_NONE);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing W_A_NC_ro

  // Fold | Table* Where GbyAgg_P GbyAgg_N GbyAgg_O

  m_pCI->vRestart();
  //RASSERT(m_pCI->G_bEachMainWithSub(CK_PROJECT,CK_PROJECTION));
  RASSERT(m_pCI->G_bEachMain(CK_FOLD));

  Smooth* pSmooth = new Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->vEndTransform();

  delete pSmooth;
  delete pCarryForward;
  delete pFold;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlatten_40_W_L_C_E(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

  /* ARCHETYPE :
      select count(*)
        from orders
       where exists (select *
                       from lineitem
                      where L_ORDERKEY = O_ORDERKEY
                        and l_commitdate < l_receiptdate)
      ;
   */

  m_pCI->vTransform(__func__, XF_NONE, "((vFlatten_40_W_L_C_E))"_Stz);

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
  pFold->vFold_basic(m_pCI,"40_W_L_C_E"_Stz,CK_WHERE);
  m_pCI->vPrintBoard(__LINE__);

  Smoosh* pSmoosh = new Smoosh;
  pSmoosh->vInto_KEYnotEXISTS(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vEndTransform();

  delete pSmoosh;
  delete pFold;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlatten_52_W_L_NC_I_exists(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  vFlatten_53_W_L_NC_NI_exists();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlatten_52_W_L_NC_I_join(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
  i8 nTables = pFold->nTables_In_SubQuery(m_pCI,"52_W_L_NC_I_join"_Stz,CK_WHERE);

  delete pFold;

  if (1 == nTables)
    vFlatten_53_W_L_NC_NI_join();
  else
    vFlatten_53_W_L_NC_NI_exists();

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlatten_53_W_L_NC_NI_exists(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

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

  m_pCI->vTransform(__func__, XF_NONE, "((vFlatten_53_W_L_NC_NI_exists))"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of exists subQuery

//  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
//  CarryForward* pCarryForward = pFold->pFold_exists("W_L_NC_NI"_Stz,m_pCI);
//
//  m_pCI->vPrintBoard(__LINE__);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing

//  m_pCI->G_vRestart();
//  if (m_pCI->G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
//    m_pCI->G.p_where->Y_vSet(P_Smoothed);
//    m_pCI->G.p_boolCalc->vCullPred(MARK_AT);
//    if (0 == m_pCI->G.p_boolCalc->nChildCards())
//      m_pCI->G_vRemoveDeleteMain(CK_WHERE);
//  }
//  m_pCI->vPrintBoard(__LINE__);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);

  //pFold->vFold_basic(m_pCI,"W_L_NC_NI_exists"_Stz,CK_WHERE);
  CarryForward* pCarryForward =
      pFold->pFold_nc_subQuery(m_pCI, "W_L_NC_NI_exists"_Stz,
                               CK_WHERE, QO_IN_OP, SM_NONE);

  m_pCI->vPrintBoard(__LINE__);


  Smooth* pSmooth = new Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vEndTransform();

  delete pSmooth;
  delete pCarryForward;
  delete pFold;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlatten_53_W_L_NC_NI_join(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

  // W L NC NI
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

  m_pCI->vTransform(__func__, XF_NONE, "((vFlatten_53_W_L_NC_NI_exists))"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of exists subQuery


  Fold* pFold = new Fold(0,m_pCI->G.pDeck);

  pFold->vFold_basic(m_pCI,"W_L_NC_NI_join"_Stz,CK_WHERE);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smooshing

  Smoosh* pSmoosh = new Smoosh();

  pSmoosh->vInto_KEYnotEXISTS(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing

  m_pCI->G_vRestart();
  //RASSERT(m_pCI->G_bEachMainWithSub(CK_PROJECT,CK_PROJECTION));
  RASSERT(m_pCI->G_bEachMain(CK_FOLD));

  CarryForward* pCarryForward = new CarryForward;
  pCarryForward->Table.m_bReproject = true;

  Smooth* pSmooth = new Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vEndTransform();

  delete pSmoosh;
  delete pSmooth;
  delete pCarryForward;
  delete pFold;
}

/*----------------------------------------------------------------------------*/
/* Folding a non-correlated aggregate subquery (via having clause)            */
/*----------------------------------------------------------------------------*/
void XF_Flatten::vFlatten_74_H_A_NC_ro(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN, _FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

  // H A NC ro (refer: Q11)
  // - the nc subquery produces a single nc result, such as avg(col)
  // - that SQ result needs to be in every OQ row so the Having clause
  //     can use it to filter rows

  /* ARCHETYPE _r_a_b_HAVING:
      select l_orderkey, sum(l_quantity)
        from lineitem
       group by l_orderkey
      having sum(l_quantity) > (select avg(l_quantity)
                                  from lineitem);
  */

  m_pCI->vTransform(__func__, XF_NONE, "((vFlatten_74_H_A_NC_ro))"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of subQuery

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
  CarryForward* pCarryForward =
      pFold->pFold_nc_subQuery(m_pCI, "H_A_NC_ro"_Stz,
                               CK_HAVING, QO_RELN_OP, SM_NONE);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing of: subQuery | Fold | Table* [Where] GbyAgg_PNO [Project (avg)] Having

  Smooth* pSmooth = new Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->G.pDeck->vClearMarksRecursive();

  delete pFold;
  delete pCarryForward;
  delete pSmooth;

  m_pCI->vEndTransform();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/