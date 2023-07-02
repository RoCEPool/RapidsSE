/*============================================================================*/
/* MakeWeavable.cpp -- Make Weavable                                          */
/*============================================================================*/

#include "MakeWeavable.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Optimiser.hpp"

#include "XF_Dedupe_as.hpp"
#include "XF_Reinstate_stage.hpp"
#include "XF_stage_cast.hpp"
#include "XF_stmCol_To_colNm.hpp"
#include "XF_tblCol_To_stmCol.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, MakeWeavable, CN_MAKEWEAVABLE)
__aNewFunction(MakeWeavable, CN_MAKEWEAVABLE)
__vDeleteFunction(MakeWeavable, CN_MAKEWEAVABLE)
__aFooterFunction(MakeWeavable)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MakeWeavable::MakeWeavable(CardIter* pCI)
: Object(CN_MAKEWEAVABLE)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);

  m_pCI = pCI;

  _checkInitialised_(CN_MAKEWEAVABLE);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MakeWeavable::~MakeWeavable(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeWeavable::vWeavable(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEWEAVABLE, _FOOTER_);

  XF_Reinstate_stage::vXF_Reinstate_stage(m_pCI);

  XF_stage_cast::vXF_stage_cast(m_pCI);

  XF_tblCol_To_stmCol::vXF_tblCol_To_stmCol(m_pCI);

  XF_stmCol_to_colNm::vXF_stmCol_to_colNm(m_pCI);

  XF_Dedupe_as::vXF_Dedupe_as(m_pCI);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
