/*============================================================================*/
/* XF_Synthesise_Cut_code.cpp -- Cut Code                                     */
/*============================================================================*/

#include "XF_Synthesise_Cut_code.hpp"

#include "Bench.hpp"
#include "Board.hpp"
#include "Optimiser.hpp"
#include "PipelineDef.hpp"
#include "Stencil.hpp"

#include "Symbol.hpp"
#include "Table.hpp"
#include "StreamDef.hpp"

#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Synthesise_Cut_code, CN_XF_SYNTHESISE_CODE)
__aNewFunction(XF_Synthesise_Cut_code, CN_XF_SYNTHESISE_CODE)
__vDeleteFunction(XF_Synthesise_Cut_code, CN_XF_SYNTHESISE_CODE)
__aFooterFunction(XF_Synthesise_Cut_code)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Synthesise_Cut_code::XF_Synthesise_Cut_code(CardIter* pCI)
: XF_SYNTHESISE(CN_XF_SYNTHESISE_CODE,pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  m_pCI = pCI;

  _checkInitialised_(CN_XF_SYNTHESISE_CODE);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Synthesise_Cut_code::~XF_Synthesise_Cut_code(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Cut_code::vCut(CardIter* pCI)
{
  _static_

  XF_Synthesise_Cut_code* pXF_Make_Code = new XF_Synthesise_Cut_code(pCI);
  pXF_Make_Code->vCut();

  delete pXF_Make_Code;
}





/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* call synthesiser                                                           */
/*----------------------------------------------------------------------------*/
void XF_Synthesise_Cut_code::vCut(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_SYNTHESISE_CODE, _FOOTER_);

  if (AM::JOB::bHasTag(C_HASCODEFILE))
    return;

  PipelineDef* pPipelineDef = m_pCI->m_pBench->pOptimiser()->pPipelineDef();

  eEC eError = Stencil::eSynthesise(pPipelineDef);
  UNUSED(eError);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
