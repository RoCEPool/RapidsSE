/*============================================================================*/
/* XF_Flatten_32_W_A_NC_I.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_32_W_A_NC_I.hpp"


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
void XF_Flatten_32_W_A_NC_I::v32_W_A_NC_I(CardIter* pCI)
{
  _static_

  XF_Flatten_32_W_A_NC_I* pXF_Flatten_32_W_A_NC_I = new XF_Flatten_32_W_A_NC_I(pCI);

  pXF_Flatten_32_W_A_NC_I->v32_W_A_NC_I();

  delete pXF_Flatten_32_W_A_NC_I;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_32_W_A_NC_I::v32_W_A_NC_I(void)
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
