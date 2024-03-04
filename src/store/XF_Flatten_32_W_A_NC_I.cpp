/*============================================================================*/
/* XF_Flatten_32_W_A_NC_I.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_32_W_A_NC_I.hpp"

#include "XF_Fold.hpp"
#include "XF_Flatten_Smooth.hpp"
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
: XF_FLATTEN_ABS(CN_XF_FLATTEN_32_W_A_NC_I, pCI)
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

 // m_pCI->vPrintBoard(__func__,__FILE__,__LINE__);

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
  // fold of subQuery

  XF_Fold* pFold = new XF_Fold;
  CarryForward* pCarryForward =
      pFold_nc_subQuery(m_pCI, "W_A_NC_I"_Stz, CK_o_WHERE, QO_IN_OP);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing of: subQuery | XF_Fold | Table* [Where] GbyAgg_PNO [Project (avg)] Having

  XF_Flatten_Left_subQuery::vRotate_Next(m_pCI);

  XF_Flatten_Smooth* pSmooth = new XF_Flatten_Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);

  m_pCI->G.pDeck->vMarksClearRecursive();

 // m_pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  delete pCarryForward;
  delete pSmooth;
  delete pFold;

  m_pCI->vEndTransform(__LINE__);
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
