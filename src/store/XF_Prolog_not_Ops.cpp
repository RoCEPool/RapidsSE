/*============================================================================*/
/* XF_Prolog_not_Ops.cpp -- Not Ops                                                  */
/*============================================================================*/

#include "XF_Prolog_not_Ops.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_not_Ops, CN_XF_NOT_OPS)
__aNewFunction(XF_Prolog_not_Ops, CN_XF_NOT_OPS)
__vDeleteFunction(XF_Prolog_not_Ops, CN_XF_NOT_OPS)
__aFooterFunction(XF_Prolog_not_Ops)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_not_Ops::XF_Prolog_not_Ops(CardIter* pCI)
: XF_PROLOG(CN_XF_NOT_OPS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_NOT_OPS, _FOOTER_);

  _checkInitialised_(CN_XF_NOT_OPS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_NOT_OPS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_not_Ops::~XF_Prolog_not_Ops(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_NOT_OPS, _FOOTER_);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachSubOfMain(CK_RPNCALC))
    m_pCI->G.p_rpnCalc->vConvert_sqyOp_not_To_notSqyOp();

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_not_Ops::vXF_not_Ops(CardIter* pCI)
{
  _static_

  XF_Prolog_not_Ops* pXF_not_Ops = new XF_Prolog_not_Ops(pCI);

  pXF_not_Ops->vXF_not_Ops();

  delete pXF_not_Ops;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* not ops                                                                    */
/*----------------------------------------------------------------------------*/
void XF_Prolog_not_Ops::vXF_not_Ops(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_NOT_OPS, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_NOT_OPS, "not ops"_Stz);

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachSubOfMain(CK_RPNCALC))
    m_pCI->G.p_rpnCalc->vConvert_sqyOp_not_To_notSqyOp();

  m_pCI->vEndTransform();
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
