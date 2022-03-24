/*============================================================================*/
/* Section.cpp -- Section of Pipeline                                         */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "Marker.hpp"

#include "PtnMap.hpp"

#include "Grid.hpp"

#include "Op.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "Section.hpp"
#include "Section.hpp"
#include "Pipeline.hpp"
#include "PipeSpec.hpp"
#include "SectionSpec.hpp"
#include "StageSpec.hpp"
#include "SP_Stage.hpp"
#include "TagConstants.h" //?

extern AttachMap gxAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Section, CN_SECTION)
__aNewFunction(Section, CN_SECTION)
__vDeleteFunction(Section, CN_SECTION)
__aFooterFunction(Section)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Section::Section(SectionSpec *pSectionSpec, Pipeline *pPipeline)
: LifeGuard_t<Section>(this, CN_SECTION)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SECTION, _FOOTER_);

  m_sPipelineNm = pPipeline->szPipelineNm();
  m_sSectionNm  = pSectionSpec->szSectionNm();
  m_i_Section   = pSectionSpec->i_SectionSpec();

  m_pSectionSpec = pSectionSpec;
  m_pPipeline    = pPipeline;

  _checkInitialised_(CN_SECTION);

  //vCreateStreams();

  _postcondition_(_ADDRESS_, _MAGIC_, CN_SECTION, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Section::~Section(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SECTION, _FOOTER_);

  i8 nStages = this->nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    Stage *pStage = pGet(i_Stage);

    pStage->vGuardedDelete(CN_STAGE, &pStage,
                            CN_SECTION, this, (Stz *) __func__);
  }

  vClear();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
