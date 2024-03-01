/*============================================================================*/
/* XF_Optimise_from_To_OP.cpp -- Delete flat                                          */
/*============================================================================*/

#include "XF_Optimise_from_To_OP.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Optimise_from_To_OP, CN_XF_OPTIMISE_FROM_TO_OP)
__aNewFunction(XF_Optimise_from_To_OP, CN_XF_OPTIMISE_FROM_TO_OP)
__vDeleteFunction(XF_Optimise_from_To_OP, CN_XF_OPTIMISE_FROM_TO_OP)
__aFooterFunction(XF_Optimise_from_To_OP)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Optimise_from_To_OP::XF_Optimise_from_To_OP(CardIter* pCI)
: XF_OPTIMISE(CN_XF_OPTIMISE_FROM_TO_OP, OT_FROM_TO_OP, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FROM_TO_OP, _FOOTER_);

  _checkInitialised_(CN_XF_OPTIMISE_FROM_TO_OP);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FROM_TO_OP, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Optimise_from_To_OP::~XF_Optimise_from_To_OP(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FROM_TO_OP, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Optimise_from_To_OP::vOptimise(CardIter* pCI)
{
  _static_

  XF_Optimise_from_To_OP* pXF_Morph_table_To_EEop = new XF_Optimise_from_To_OP(pCI);

  bool b1 = pXF_Morph_table_To_EEop->bOptimise();

  pXF_Morph_table_To_EEop->vOptimised(b1);

  delete pXF_Morph_table_To_EEop;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_Optimise_from_To_OP::bOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_OPTIMISE_FROM_TO_OP, _FOOTER_);

  bool bOptRun = false;

  m_pCI->vTransform(__FILE__, __func__, XF_OPTIMISE_FROM_TO_OP);

  m_pCI->vBeginEach();
  while (m_pCI->bEachMainWithSub(CK_o_FROM, CK_s_TABLE)) {
    m_pCI->G.p_table->vMorph_table_To_EEop(m_pCI);
  }

  bOptRun = true;

  m_pCI->vEndTransform(__LINE__);

  return bOptRun;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
