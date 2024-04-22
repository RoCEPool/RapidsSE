/*============================================================================*/
/* XF_Prolog_Reduce_not.cpp -- Reduce Not expression ops                     */
/*============================================================================*/

#include "XF_Prolog_Reduce_not.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Reduce_not, CN_XF_PROLOG_REDUCE_NOT)
__aNewFunction(XF_Prolog_Reduce_not, CN_XF_PROLOG_REDUCE_NOT)
__vDeleteFunction(XF_Prolog_Reduce_not, CN_XF_PROLOG_REDUCE_NOT)
__aFooterFunction(XF_Prolog_Reduce_not)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Reduce_not::XF_Prolog_Reduce_not(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_REDUCE_NOT, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_REDUCE_NOT, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_REDUCE_NOT);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_REDUCE_NOT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Reduce_not::~XF_Prolog_Reduce_not(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_REDUCE_NOT, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Reduce_not::vReduce(CardIter* pCI)
{
  _static_

  XF_Prolog_Reduce_not* pXF_not_Ops = new XF_Prolog_Reduce_not(pCI);

  pXF_not_Ops->vXF_not_Ops();

  delete pXF_not_Ops;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* not ops                                                                    */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Reduce_not::vXF_not_Ops(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_REDUCE_NOT, _FOOTER_);

  m_pCI->vTransform(__FILE__, __func__, XF_PROLOG_REDUCE_NOT);

  m_pCI->vClearRegisters();
  m_pCI->vBeginEach();
  while (m_pCI->bEachOp_bySub(CK_BOOLPRED)) {
    m_pCI->E.p_boolPred->vReduce_not(m_pCI);
  }

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
