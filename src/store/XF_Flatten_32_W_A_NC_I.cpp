/*============================================================================*/
/* XF_Flatten_32_W_A_NC_I.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_32_W_A_NC_I.hpp"

#include "XF_Flatten_Left_subQuery.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_32_W_A_NC_I, CN_XF_FLATTEN_32_W_A_NC_I)
__aNewFunction(XF_Flatten_32_W_A_NC_I, CN_XF_FLATTEN_32_W_A_NC_I)
__vDeleteFunction(XF_Flatten_32_W_A_NC_I, CN_XF_FLATTEN_32_W_A_NC_I)
__aFooterFunction(XF_Flatten_32_W_A_NC_I)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_32_W_A_NC_I::XF_Flatten_32_W_A_NC_I(CardIter* pCI)
: XF_FLATTEN(CN_XF_FLATTEN_32_W_A_NC_I, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_32_W_A_NC_I, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_32_W_A_NC_I);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_32_W_A_NC_I, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_32_W_A_NC_I::~XF_Flatten_32_W_A_NC_I(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_32_W_A_NC_I, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_32_W_A_NC_I::vFlatten(CardIter* pCI)
{
  _static_

  XF_Flatten_32_W_A_NC_I* pXF_Flatten_32_W_A_NC_I = new XF_Flatten_32_W_A_NC_I(pCI);

  pXF_Flatten_32_W_A_NC_I->vFlatten();

  delete pXF_Flatten_32_W_A_NC_I;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_32_W_A_NC_I::vFlatten(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_32_W_A_NC_I, _FOOTER_);

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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vFold_subQuery(m_pCI, "W_A_NC_I"_Stz, CK_o_WHERE);
  vSet_carry(m_pCI, CK_o_WHERE, QO_IN_OP);
  m_pCI->PRINTBOARD_dbg();

  XF_Flatten_Left_subQuery::vRotate_Next(m_pCI);
  m_pCI->PRINTBOARD_dbg();

  vSmooth(m_pCI);
  m_pCI->PRINTBOARD_dbg();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->E.p_deck->vMarksClearRecursive();
  m_pCI->PRINTBOARD_dbg();

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
