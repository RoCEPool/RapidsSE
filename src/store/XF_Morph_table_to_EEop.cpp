/*============================================================================*/
/* XF_Morph_table_To_EEop.cpp -- Delete flat                                          */
/*============================================================================*/

#include "XF_Morph_table_To_EEop.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Morph_table_To_EEop, CN_XF_MORPH_TABLE_TO_EEOP)
__aNewFunction(XF_Morph_table_To_EEop, CN_XF_MORPH_TABLE_TO_EEOP)
__vDeleteFunction(XF_Morph_table_To_EEop, CN_XF_MORPH_TABLE_TO_EEOP)
__aFooterFunction(XF_Morph_table_To_EEop)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Morph_table_To_EEop::XF_Morph_table_To_EEop(CardIter* pCI)
: XF_TRANSFORM(CN_XF_MORPH_TABLE_TO_EEOP, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_MORPH_TABLE_TO_EEOP, _FOOTER_);

  _checkInitialised_(CN_XF_MORPH_TABLE_TO_EEOP);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_MORPH_TABLE_TO_EEOP, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Morph_table_To_EEop::~XF_Morph_table_To_EEop(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_MORPH_TABLE_TO_EEOP, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Morph_table_To_EEop::vXF_Morph_table_To_EEop(CardIter* pCI)
{
  _static_

  XF_Morph_table_To_EEop* pXF_Morph_table_To_EEop = new XF_Morph_table_To_EEop(pCI);

  pXF_Morph_table_To_EEop->vXF_Morph_table_To_EEop();

  delete pXF_Morph_table_To_EEop;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Morph_table_To_EEop::vXF_Morph_table_To_EEop(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_MORPH_TABLE_TO_EEOP, _FOOTER_);

  m_pCI->vTransform(__func__, XF_MORPH_TABLE_TO_EEOP, "morph table to EE op"_Stz);

  Board::vMorphAll_table_To_EEop(m_pCI);

  m_pCI->vEndTransform();

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
