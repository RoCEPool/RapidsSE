/*============================================================================*/
/* XF_rpnCalc_To_boolCalc.cpp -- RpnCalc to BoolCalc                          */
/*============================================================================*/

#include "XF_rpnCalc_To_boolCalc.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_rpnCalc_To_boolCalc, CN_XF_RPNCALC_TO_BOOLCALC)
__aNewFunction(XF_rpnCalc_To_boolCalc, CN_XF_RPNCALC_TO_BOOLCALC)
__vDeleteFunction(XF_rpnCalc_To_boolCalc, CN_XF_RPNCALC_TO_BOOLCALC)
__aFooterFunction(XF_rpnCalc_To_boolCalc)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_rpnCalc_To_boolCalc::XF_rpnCalc_To_boolCalc(CardIter* pCI)
: XF_TRANSFORM(CN_XF_RPNCALC_TO_BOOLCALC, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_RPNCALC_TO_BOOLCALC, _FOOTER_);

  _checkInitialised_(CN_XF_RPNCALC_TO_BOOLCALC);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_RPNCALC_TO_BOOLCALC, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_rpnCalc_To_boolCalc::~XF_rpnCalc_To_boolCalc(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_RPNCALC_TO_BOOLCALC, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_rpnCalc_To_boolCalc::vXF_rpnCalc_To_boolCalc(CardIter* pCI)
{
  _static_

  XF_rpnCalc_To_boolCalc* pXF_rpnCalc_To_boolCalc = new XF_rpnCalc_To_boolCalc(pCI);

  pXF_rpnCalc_To_boolCalc->vXF_rpnCalc_To_boolCalc();

  delete pXF_rpnCalc_To_boolCalc;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* all $n become $c.n                                                         */
/*----------------------------------------------------------------------------*/
void XF_rpnCalc_To_boolCalc::vXF_rpnCalc_To_boolCalc(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_RPNCALC_TO_BOOLCALC, _FOOTER_);

  m_pCI->vTransform(__func__, "all $n become $c.n"_Stz);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_RPNCALC))
    ((Card_OP*)m_pCI->G.p_MainCard)->vConvert_rpnCalc_To_boolCalc(m_pCI);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_HAVING, CK_RPNCALC))
    ((Card_OP*)m_pCI->G.p_MainCard)->vConvert_rpnCalc_To_boolCalc(m_pCI);

  m_pCI->vEndTransform();


  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
