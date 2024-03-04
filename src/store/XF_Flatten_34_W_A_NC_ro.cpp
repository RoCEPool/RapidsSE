/*============================================================================*/
/* XF_Flatten_34_W_A_NC_ro.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_34_W_A_NC_ro.hpp"

#include "XF_Fold.hpp"
#include "XF_Flatten_Smooth.hpp"


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
: XF_TRANSFORM(CN_XF_FLATTEN_34_W_A_NC_RO, pCI)
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

  pXF_Flatten_34_W_A_NC_ro->vXF_Flatten_34_W_A_NC_ro();

  delete pXF_Flatten_34_W_A_NC_ro;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_34_W_A_NC_ro::vXF_Flatten_34_W_A_NC_ro(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_34_W_A_NC_RO, _FOOTER_);

 // m_pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  // W A NC ro
  // push a (single-value aggregate non-correlated in) sub-query before the main-query
  // as an aggregation-fan (projecting a single aggregated value)
  // (refer: Q15)

  /*
   */

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_34_W_A_NC_RO);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of subQuery

  XF_Fold* pFold = new XF_Fold(0, m_pCI->G.pDeck);
  CarryForward* pCarryForward =
      pFold->pFold_nc_subQuery(m_pCI, "W_A_NC_ro"_Stz,
                               CK_o_WHERE, QO_RELN_OP);

 // m_pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing W_A_NC_ro

  // XF_Fold | Table* Where GbyAgg_P GbyAgg_N GbyAgg_O

  m_pCI->vBeginEach();
  //RASSERT(m_pCI->bEachMainWithSub(CK_o_PROJECT,CK_t_EMIT));
  RASSERT(m_pCI->bEachMain(CK_c_FOLD));

  XF_Flatten_Smooth* pSmooth = new XF_Flatten_Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->G.pDeck->vMarksClearRecursive();

 // m_pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  delete pSmooth;
  delete pCarryForward;
  delete pFold;

  m_pCI->vEndTransform(__LINE__);
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
