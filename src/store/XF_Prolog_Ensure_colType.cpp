/*============================================================================*/
/* XF_Prolog_Ensure_colType.cpp -- Ensure Column Type                         */
/*============================================================================*/

#include "XF_Prolog_Ensure_colType.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Ensure_colType, CN_XF_PROLOG_ENSURE_COLTYPE)
__aNewFunction(XF_Prolog_Ensure_colType, CN_XF_PROLOG_ENSURE_COLTYPE)
__vDeleteFunction(XF_Prolog_Ensure_colType, CN_XF_PROLOG_ENSURE_COLTYPE)
__aFooterFunction(XF_Prolog_Ensure_colType)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Ensure_colType::XF_Prolog_Ensure_colType(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_ENSURE_COLTYPE, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ENSURE_COLTYPE, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_ENSURE_COLTYPE);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ENSURE_COLTYPE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Ensure_colType::~XF_Prolog_Ensure_colType(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ENSURE_COLTYPE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Ensure_colType::vEnsure(CardIter* pCI)
{
  _static_

  XF_Prolog_Ensure_colType* pXF_Validate = new XF_Prolog_Ensure_colType(pCI);

  pXF_Validate->vEnsure();

  delete pXF_Validate;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* ensure each op has a level                                                 */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Ensure_colType::vEnsure(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ENSURE_COLTYPE, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_ENSURE_COLTYPE, "validate boards"_Stz);

  m_pCI->vClearRestart();
//  while (m_pCI->G_bEachMainWith2Sub(CK_FROM,CK_INJECTABLE,CK_EMIT)) {
//    m_pCI->G.p_emit->vUpdate_cast(m_pCI->G.p_injectable);
//  }

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
