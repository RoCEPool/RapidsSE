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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
