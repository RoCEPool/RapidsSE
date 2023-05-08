/*============================================================================*/
/* MakeCode.cpp -- Make Simpler                                            */
/*============================================================================*/

#include "MakeCode.hpp"

#include "Board.hpp"
#include "PipelineDef.hpp"
#include "Stencil_v2.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, MakeCode, CN_MAKECODE)
__aNewFunction(MakeCode, CN_MAKECODE)
__vDeleteFunction(MakeCode, CN_MAKECODE)
__aFooterFunction(MakeCode)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MakeCode::MakeCode(CardIter* pCI)
: Object(CN_MAKECODE)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  m_pCI = pCI;

  _checkInitialised_(CN_MAKECODE);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MakeCode::~MakeCode(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* call synthesiser                                                           */
/*----------------------------------------------------------------------------*/
void MakeCode::vSynthesise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKECODE, _FOOTER_);

  PipelineDef* pPipelineDef = new PipelineDef;
  eEC eError = Stencil_v2::eSynthesise(pPipelineDef);
  UNUSED(eError);
  delete pPipelineDef;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
