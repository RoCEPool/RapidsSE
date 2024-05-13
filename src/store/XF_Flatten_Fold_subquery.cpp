/*============================================================================*/
/* XF_Flatten_Fold_subquery.cpp -- Flatten                                    */
/*============================================================================*/

#include "XF_Flatten_Fold_subquery.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_Fold_subquery, CN_XF_FLATTEN_FOLD_SUBQUERY)
__aNewFunction(XF_Flatten_Fold_subquery, CN_XF_FLATTEN_FOLD_SUBQUERY)
__vDeleteFunction(XF_Flatten_Fold_subquery, CN_XF_FLATTEN_FOLD_SUBQUERY)
__aFooterFunction(XF_Flatten_Fold_subquery)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_Fold_subquery::XF_Flatten_Fold_subquery(CardIter* pCI)
: XF_FLATTEN(CN_XF_FLATTEN_FOLD_SUBQUERY, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_FOLD_SUBQUERY, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_FOLD_SUBQUERY);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_FOLD_SUBQUERY, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_Fold_subquery::~XF_Flatten_Fold_subquery(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_FOLD_SUBQUERY, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decide how to fold inner subquery into outer subquery                      */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Fold_subquery::vDecisionTree(CardIter* pCI, Symbol* pSym_sqyNo)
{
  _static_

  XF_Flatten_Fold_subquery* pFoldBy = new XF_Flatten_Fold_subquery(pCI);

  pCI->Flat.m_pSqy_inner = pCI->m_pBench->pFindSubQuery(pSym_sqyNo);
  SubQuery* pInner = pCI->Flat.m_pSqy_inner;

  if (pInner->bViaFrom())                /* F       Q7,8,9 */ pFoldBy->v10_F();
  else
  if (pInner->bViaWhere()) {
    if (pInner->bIsAggregation()) {
      if (pInner->bIsCorrelated()) {     /* 20_ */
        if (pInner->bByExists())         /* W_A_C_E        */ NEVER_REACH
        else if (pInner->bByNotExists()) /* W_A_C_NE       */ NEVER_REACH
        else if (pInner->bByIn())        /* W_A_C_I        */ NEVER_REACH
        else if (pInner->bByNotIn())     /* W_A_C_NI       */ NEVER_REACH
        else                             /* W_A_C_ro Q2,17 */ pFoldBy->v24_W_A_C_ro();
      } else {                           /* 30_ */
        if (pInner->bByExists())         /* W_A_NC_E       */ NEVER_REACH
        else if (pInner->bByNotExists()) /* W_A_NC_NE      */ NEVER_REACH
        else if (pInner->bByIn())        /* W_A_NC_I   Q18 */ pFoldBy->v32_W_A_NC_I();
        else if (pInner->bByNotIn())     /* W_A_NC_NI      */ NEVER_REACH
        else                             /* W_A_NC_ro  Q15 */ pFoldBy->v34_W_A_NC_ro();
      }
    } else {
      if (pInner->bIsCorrelated()) {     /* 40_ */
        if (pInner->bByExists())         /* W_L_C_E     Q4 */ pFoldBy->v40_W_L_C_E();
        else if (pInner->bByNotExists()) /* W_L_C_NE       */ NEVER_REACH
        else if (pInner->bByIn())        /* W_L_C_I        */ NEVER_REACH
        else if (pInner->bByNotIn())     /* W_L_C_NI       */ NEVER_REACH
        else                             /* W_L_C_ro       */ NEVER_REACH
      } else {                           /* 50_ */
        if (pInner->bByExists())         /* W_L_NC_E       */ NEVER_REACH
        else if (pInner->bByNotExists()) /* W_L_NC_NE      */ NEVER_REACH
        else if (pInner->bByIn())        /* W_L_NC_I       */ pFoldBy->v52_W_L_NC_I_join();
        else if (pInner->bByNotIn())     /* W_L_NC_NI  Q16 */ pFoldBy->v53_W_L_NC_NI_join();
        else                             /* W_L_NC_ro      */ NEVER_REACH
      }
    }
  }
  else
  if (pInner->bViaHaving()) {
    if (pInner->bIsAggregation()) {
      if (pInner->bIsCorrelated()) {     /* 60_ */
        if (pInner->bByExists())         /* H_A_C_E        */ NEVER_REACH
        else if (pInner->bByNotExists()) /* H_A_C_NE       */ NEVER_REACH
        else if (pInner->bByIn())        /* H_A_C_I        */ NEVER_REACH
        else if (pInner->bByNotIn())     /* H_A_C_NI       */ NEVER_REACH
        else                             /* H_A_C_ro       */ NEVER_REACH
      } else {                           /* 70_ */
        if (pInner->bByExists())         /* H_A_NC_E       */ NEVER_REACH
        else if (pInner->bByNotExists()) /* H_A_NC_NE      */ NEVER_REACH
        else if (pInner->bByIn())        /* H_A_NC_I       */ NEVER_REACH
        else if (pInner->bByNotIn())     /* H_A_NC_NI      */ NEVER_REACH
        else                             /* H_A_NC_ro  Q11 */ pFoldBy->v74_H_A_NC_ro();
      }
    } else {
      if (pInner->bIsCorrelated()) {     /* 80_ */
        if (pInner->bByExists())         /* H_L_C_E        */ NEVER_REACH
        else if (pInner->bByNotExists()) /* H_L_C_NE       */ NEVER_REACH
        else if (pInner->bByIn())        /* H_L_C_I        */ NEVER_REACH
        else if (pInner->bByNotIn())     /* H_L_C_NI       */ NEVER_REACH
        else                             /* H_L_C_ro       */ NEVER_REACH
      } else {                           /* 90_ */
        if (pInner->bByExists())         /* H_L_NC_E       */ NEVER_REACH
        else if (pInner->bByNotExists()) /* H_L_NC_NE      */ NEVER_REACH
        else if (pInner->bByIn())        /* H_L_NC_I       */ NEVER_REACH
        else if (pInner->bByNotIn())     /* H_L_NC_NI      */ NEVER_REACH
        else                             /* H_L_NC_ro      */ NEVER_REACH
      }
    }
  }
  else
    NEVER_REACH;

  delete pFoldBy;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* 10 F                                                                       */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Fold_subquery::v10_F(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_FOLD_SUBQUERY, _FOOTER_);

  // F
  // (refer: Q7,8,9)

  /*
      select count(*)
        from (select *
                from orders, lineitem
               where L_ORDERKEY = O_ORDERKEY)
      ;
   */

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_10_F);
  //m_pCI->PRINTBOARD_dbg();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vFold_from("From"_Stz,m_pCI);
  vSmooth(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->E.p_deck->vMarksClearRecursive();
  m_pCI->vEndTransform(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* 24 W_A_C_ro                                                                */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Fold_subquery::v24_W_A_C_ro(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_FOLD_SUBQUERY, _FOOTER_);

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

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_24_W_A_C_RO);
  //m_pCI->PRINTBOARD_dbg();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vFold_subQuery(m_pCI, "24_W_A_C_ro"_Stz, CK_o_WHERE);
  vPromote_aggreg_To_gbyAgg(m_pCI);
  vRestate_emit_fold(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->E.p_deck->vMarksClearRecursive();
  m_pCI->vEndTransform(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* 32 W_A_NC_I                                                                */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Fold_subquery::v32_W_A_NC_I(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_FOLD_SUBQUERY, _FOOTER_);

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

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_32_W_A_NC_I);
  //m_pCI->PRINTBOARD_dbg();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vFold_subQuery(m_pCI, "W_A_NC_I"_Stz, CK_o_WHERE);
  vSet_smooth(m_pCI, CK_o_WHERE, QO_IN_OP);
  vRotate_Next();
  vSmooth(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->E.p_deck->vMarksClearRecursive();
  m_pCI->vEndTransform(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* 34 W_A_NC_ro                                                               */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Fold_subquery::v34_W_A_NC_ro(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_FOLD_SUBQUERY, _FOOTER_);

  // W A NC ro
  // push a (single-value aggregate non-correlated in) sub-query before the main-query
  // as an aggregation-fan (projecting a single aggregated value)
  // (refer: Q15)

  /*
   */

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_34_W_A_NC_RO);
  //m_pCI->PRINTBOARD_dbg();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vFold_subQuery(m_pCI, "W_A_NC_ro"_Stz, CK_o_WHERE);
  vSet_smooth(m_pCI, CK_o_WHERE, QO_RELN_OP);
  vSmooth(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->E.p_deck->vMarksClearRecursive();
  m_pCI->vEndTransform(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* 40 W_L_C_E                                                                 */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Fold_subquery::v40_W_L_C_E(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_FOLD_SUBQUERY, _FOOTER_);

  /* ARCHETYPE :
      select count(*)
        from orders
       where exists (select *
                       from lineitem
                      where L_ORDERKEY = O_ORDERKEY
                        and l_commitdate < l_receiptdate)
      ;
   */

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_40_W_L_C_E);
  //m_pCI->PRINTBOARD_dbg();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vFold_subQuery(m_pCI, "40_W_L_C_E"_Stz, CK_o_WHERE);
  vInto_KEYnotEXISTS(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->E.p_deck->vMarksClearRecursive();
  m_pCI->vEndTransform(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* 52 W_L_NC_I_join                                                           */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Fold_subquery::v52_W_L_NC_I_join(void)
{
  _static_

  i8 nTables = nTables_In_SubQuery(m_pCI, "52_W_L_NC_I_join"_Stz, CK_o_WHERE);

  if (1 == nTables)
    v53_W_L_NC_NI_join();
  else
    v53_W_L_NC_NI_exists();
}

/*----------------------------------------------------------------------------*/
/* 53 W_L_NC_NI_exists                                                        */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Fold_subquery::v53_W_L_NC_NI_exists(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_FOLD_SUBQUERY, _FOOTER_);

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

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_53_W_L_NC_NI_EXISTS);
  //m_pCI->PRINTBOARD_dbg();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vFold_subQuery(m_pCI, "W_L_NC_NI_exists"_Stz, CK_o_WHERE);
  vSet_smooth(m_pCI, CK_o_WHERE, QO_IN_OP);
  vSmooth(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->E.p_deck->vMarksClearRecursive();
  m_pCI->vEndTransform(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* 53 W_L_NC_NI_join                                                          */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Fold_subquery::v53_W_L_NC_NI_join(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_FOLD_SUBQUERY, _FOOTER_);

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

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_53_W_L_NC_NI_JOIN);
  //m_pCI->PRINTBOARD_dbg();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vFold_subQuery(m_pCI, "53_W_L_NC_NI_join"_Stz, CK_o_WHERE);
  vInto_KEYnotEXISTS(m_pCI);
  vSmooth(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->E.p_deck->vMarksClearRecursive();
  m_pCI->vEndTransform(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* 74 H_A_NC_ro                                                               */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Fold_subquery::v74_H_A_NC_ro(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_FOLD_SUBQUERY, _FOOTER_);

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

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_74_H_A_NC_RO);
  //m_pCI->PRINTBOARD_dbg();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vFold_subQuery(m_pCI, "H_A_NC_ro"_Stz, CK_o_HAVING);
  vSet_smooth(m_pCI, CK_o_HAVING, QO_RELN_OP);
  vRotate_Next();
  vSmooth(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->E.p_deck->vMarksClearRecursive();
  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
