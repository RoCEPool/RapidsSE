/*============================================================================*/
/* SectionSpec.cpp -- Section Specification                                   */
/*============================================================================*/

#include "BASE.h"

#include "PipelineSpec.hpp"
#include "SectionSpec.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(STORE,SectionSpec,CN_SECTIONSPEC)
__aNewFunction(SectionSpec,CN_SECTIONSPEC)
__vDeleteFunction(SectionSpec,CN_SECTIONSPEC)
__aFooterFunction(SectionSpec)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SectionSpec::SectionSpec(Stz* szSectionNm,
                         PipelineSpec* pPipelineSpec)
: ListNL_t<StageSpec>(CN_SECTIONSPEC)
, LifeGuard_t<SectionSpec>(this,CN_SECTIONSPEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SECTIONSPEC,_FOOTER_);
  
  m_sSectionNm      = szSectionNm;
  m_pPipelineSpec   = pPipelineSpec;

  _checkInitialised_(CN_SECTIONSPEC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SECTIONSPEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SectionSpec::~SectionSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SECTIONSPEC,_FOOTER_);
    
  while (StageSpec* pStageSpec = pPop())
    pStageSpec->vGuardedDelete(CN_STAGESPEC,&pStageSpec,
                               CN_SECTIONSPEC,this,(Stz*)__func__);

  vClear();
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz* SectionSpec::szSectionNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SECTIONSPEC,_FOOTER_);
  
  return m_sSectionNm;
}

/*----------------------------------------------------------------------------*/
/* get i_Section                                                              */
/*----------------------------------------------------------------------------*/
i8 SectionSpec::i_SectionSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SECTIONSPEC,_FOOTER_);
  
  NEVER_REACH;
  
  i8 i_SectionSpec = C_UNKNOWN; // m_pPipelineSpec->iFind(this);
  
  return i_SectionSpec;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
