/*============================================================================*/
/* XF_Flatten_Fold_subquery.cpp -- Flatten                                             */
/*============================================================================*/

#include "XF_Flatten_Fold_subquery.hpp"

#include "XF_Prolog_Analyse_emit.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_Fold_subquery, CN_XF_FLATTEN_10_F)
__aNewFunction(XF_Flatten_Fold_subquery, CN_XF_FLATTEN_10_F)
__vDeleteFunction(XF_Flatten_Fold_subquery, CN_XF_FLATTEN_10_F)
__aFooterFunction(XF_Flatten_Fold_subquery)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_Fold_subquery::XF_Flatten_Fold_subquery(CardIter* pCI)
: XF_FLATTEN(CN_XF_FLATTEN_10_F, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_10_F, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_10_F);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_10_F, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_Fold_subquery::~XF_Flatten_Fold_subquery(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_10_F, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Fold_subquery::v10_F(CardIter* pCI)
{
  _static_

  XF_Flatten_Fold_subquery* pXF_Flatten_10_F = new XF_Flatten_Fold_subquery(pCI);

  pXF_Flatten_10_F->v10_F();

  delete pXF_Flatten_10_F;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Fold_subquery::v24_W_A_C_ro(CardIter* pCI)
{
  _static_

  XF_Flatten_Fold_subquery* pXF_Flatten_24_W_A_C_ro = new XF_Flatten_Fold_subquery(pCI);

  pXF_Flatten_24_W_A_C_ro->v24_W_A_C_ro();

  delete pXF_Flatten_24_W_A_C_ro;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Fold_subquery::v10_F(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_10_F, _FOOTER_);

 // m_pCI->PRINTBOARD_dbg();

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
  m_pCI->E.p_deck->vMarksClearRecursive();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // todo: discuss with Alka
//  m_pCI->vClearRegisters();
//  m_pCI->vBeginEach();
//  if (m_pCI->bEachOp(CK_o_FOLD)) {
//    if (m_pCI->bEachOp(CK_o_WHERE)) {
//      Card_Level* p_level_where = m_pCI->E.p_where->pSub_level();
//      if (m_pCI->bEachOp(CK_o_PROJECT, CK_p_LEVEL)) {
//        p_level_where->pSymbol()->Y_vSet(P_Level, m_pCI->E.p_level->pSymbol()->Y_iGet(P_Level));
//      }
//    }
//  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->vEndTransform(__LINE__);

//  XF_Prolog_Analyse_emit::vAnalyse(m_pCI);
}

/*----------------------------------------------------------------------------*/
/* flatten                                                                    */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Fold_subquery::v24_W_A_C_ro(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_10_F, _FOOTER_);

  // m_pCI->PRINTBOARD_dbg();

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
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Fold_subquery::v32_W_A_NC_I(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_10_F, _FOOTER_);

  // m_pCI->PRINTBOARD_dbg();

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
