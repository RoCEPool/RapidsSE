/*============================================================================*/
/* XF_Flatten_34_W_A_NC_ro.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_34_W_A_NC_ro.hpp"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_34_W_A_NC_ro, CN_XF_FLATTEN_34_W_A_NC_RO)
__aNewFunction(XF_Flatten_34_W_A_NC_ro, CN_XF_FLATTEN_34_W_A_NC_RO)
__vDeleteFunction(XF_Flatten_34_W_A_NC_ro, CN_XF_FLATTEN_34_W_A_NC_RO)
__aFooterFunction(XF_Flatten_34_W_A_NC_ro)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_34_W_A_NC_ro::XF_Flatten_34_W_A_NC_ro(CardIter* pCI)
: XF_FLATTEN(CN_XF_FLATTEN_34_W_A_NC_RO, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_34_W_A_NC_RO, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_34_W_A_NC_RO);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_34_W_A_NC_RO, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_34_W_A_NC_ro::~XF_Flatten_34_W_A_NC_ro(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_34_W_A_NC_RO, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_34_W_A_NC_ro::vFlatten(CardIter* pCI)
{
  _static_

  XF_Flatten_34_W_A_NC_ro* pXF_Flatten_34_W_A_NC_ro = new XF_Flatten_34_W_A_NC_ro(pCI);

  pXF_Flatten_34_W_A_NC_ro->vFlatten();

  delete pXF_Flatten_34_W_A_NC_ro;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_34_W_A_NC_ro::vFlatten(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_34_W_A_NC_RO, _FOOTER_);

 // m_pCI->PRINTBOARD_dbg();

  // W A NC ro
  // push a (single-value aggregate non-correlated in) sub-query before the main-query
  // as an aggregation-fan (projecting a single aggregated value)
  // (refer: Q15)

  /*
   */

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_34_W_A_NC_RO);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of subQuery

  CarryForward* pCarryForward =
      pFold_nc_subQuery(m_pCI, "W_A_NC_ro"_Stz, CK_o_WHERE, QO_RELN_OP);

 // m_pCI->PRINTBOARD_dbg();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing W_A_NC_ro

  // XF_Fold | Table* Where GbyAgg_P GbyAgg_N GbyAgg_O

  m_pCI->vBeginEach();
  //RASSERT(m_pCI->bEachOp(CK_o_PROJECT,CK_t_EMIT));
  RASSERT(m_pCI->bEachOp(CK_c_FOLD));

  vSmooth(m_pCI,pCarryForward);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->E.p_deck->vMarksClearRecursive();

 // m_pCI->PRINTBOARD_dbg();

  delete pCarryForward;

  m_pCI->vEndTransform(__LINE__);
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
