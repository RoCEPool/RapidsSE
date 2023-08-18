/*============================================================================*/
/* XF_Optimise_from_To_OP.cpp -- Delete flat                                          */
/*============================================================================*/

#include "XF_Optimise_from_To_OP.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_from_To_OP, CN_XF_FROM_TO_OP)
__aNewFunction(XF_Optimise_from_To_OP, CN_XF_FROM_TO_OP)
__vDeleteFunction(XF_Optimise_from_To_OP, CN_XF_FROM_TO_OP)
__aFooterFunction(XF_Optimise_from_To_OP)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_from_To_OP::XF_Optimise_from_To_OP(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FROM_TO_OP, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FROM_TO_OP, _FOOTER_);

  _checkInitialised_(CN_XF_FROM_TO_OP);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FROM_TO_OP, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_from_To_OP::~XF_Optimise_from_To_OP(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FROM_TO_OP, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_from_To_OP::vXF_Optimise_from_By_OP(CardIter* pCI)
{
  _static_

  XF_Optimise_from_To_OP* pXF_Morph_table_To_EEop = new XF_Optimise_from_To_OP(pCI);

  pXF_Morph_table_To_EEop->vXF_Morph_table_To_EEop();

  delete pXF_Morph_table_To_EEop;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_from_To_OP::vXF_Morph_table_To_EEop(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FROM_TO_OP, _FOOTER_);

  m_pCI->vTransform(__func__, XF_MORPH_TABLE_TO_EEOP, "morph table to EE op"_Stz);

  Board::vMorphAll_table_To_EEop(m_pCI);

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
