/*============================================================================*/
/* PipelineSpec.cpp -- pipeline Specification                                 */
/*============================================================================*/

#include "BASE.h"

#include "OpSpec.hpp"
#include "StageSpec.hpp"
#include "FormworkSpec.hpp"
#include "PipelineSpec.hpp"
#include "SubPipelines.hpp"
#include "Dynamo.hpp"
#include "Procedure.hpp"
#include "Job.hpp"
#include "Grid.hpp"
#include "TagConstants.h"

#include "SP_PipelineSpec.hpp"
#include "SP_Token.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, PipelineSpec, CN_PIPELINESPEC)
__aNewFunction(PipelineSpec, CN_PIPELINESPEC)
__vDeleteFunction(PipelineSpec, CN_PIPELINESPEC)
__aFooterFunction(PipelineSpec)


#ifdef WITH_FORMWORK

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PipelineSpec::PipelineSpec(Scanner *p_Scanner, TagSet *p_JobTags)
    
: Object(CN_PIPELINESPEC)
, LifeGuard_t<PipelineSpec>(this, CN_PIPELINESPEC)

{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  m_pFormworkSpec = new FormworkSpec(p_Scanner,p_JobTags,this);
  m_pSubPipelines = nullptr;
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  m_p_Scanner = p_Scanner;
  m_p_JobTags = p_JobTags;

  _checkInitialised_(CN_PIPELINESPEC);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PipelineSpec::~PipelineSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);
  
  if (m_pSubPipelines) {
    
    while (PipelineSpec *pSubPipelineSpec = m_pSubPipelines->pPop())
      delete pSubPipelineSpec;
    
    delete m_pSubPipelines;
  }
  
  
  m_pFormworkSpec->vGuardedDelete(CN_FORMWORKSPEC,&m_pFormworkSpec,
                                  CN_PIPELINESPEC,this,(Stz*)__func__);;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* pipeline compile                                                           */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCompile(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  bool bCompiled = m_pFormworkSpec->bCompile();
  
  return bCompiled;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get the job tag set                                                        */
/*----------------------------------------------------------------------------*/
TagSet* PipelineSpec::pJobTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  return m_p_JobTags;
}

/*----------------------------------------------------------------------------*/
/* get pipeline name                                                          */
/*----------------------------------------------------------------------------*/
Stz* PipelineSpec::szPipelineNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  return m_pFormworkSpec->szPipelineNm();
}

/*----------------------------------------------------------------------------*/
/* number of stages                                                           */
/*----------------------------------------------------------------------------*/
i8 PipelineSpec::nStages(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  i8 nStages = m_pFormworkSpec->nItems();
  
  return nStages;
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StageSpec* PipelineSpec::pStageSpec(i8 i_Stage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  StageSpec *pStageSpec = m_pFormworkSpec->pGet(i_Stage);

  return pStageSpec;
}

/*----------------------------------------------------------------------------*/
/* get sub-pipelines                                                          */
/*----------------------------------------------------------------------------*/
SubPipelines* PipelineSpec::pSubPipelines(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  return m_pSubPipelines;
}



// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#else

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PipelineSpec::PipelineSpec(Scanner *p_Scanner, TagSet *p_JobTags)
    
: Object(CN_PIPELINESPEC)
, LifeGuard_t<PipelineSpec>(this, CN_PIPELINESPEC)

{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  m_pFormworkSpec = new FormworkSpec(p_Scanner,p_JobTags,this);
  m_pSubPipelines = nullptr;
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  m_p_Scanner = p_Scanner;
  m_p_JobTags = p_JobTags;

  _checkInitialised_(CN_PIPELINESPEC);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PipelineSpec::~PipelineSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);
  
  if (m_pSubPipelines) {
    
    while (PipelineSpec *pSubPipelineSpec = m_pSubPipelines->pPop())
      delete pSubPipelineSpec;
    
    delete m_pSubPipelines;
  }
  
  
  m_pFormworkSpec->vGuardedDelete(CN_FORMWORKSPEC,&m_pFormworkSpec,
                                  CN_PIPELINESPEC,this,(Stz*)__func__);;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* pipeline compile                                                           */
/*----------------------------------------------------------------------------*/
bool PipelineSpec::bCompile(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  bool bCompiled = m_pFormworkSpec->bCompile();
  
  return bCompiled;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get the job tag set                                                        */
/*----------------------------------------------------------------------------*/
TagSet* PipelineSpec::pJobTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINESPEC,_FOOTER_);
  
  return m_p_JobTags;
}

/*----------------------------------------------------------------------------*/
/* get pipeline name                                                          */
/*----------------------------------------------------------------------------*/
Stz* PipelineSpec::szPipelineNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  return m_pFormworkSpec->szPipelineNm();
}

/*----------------------------------------------------------------------------*/
/* number of stages                                                           */
/*----------------------------------------------------------------------------*/
i8 PipelineSpec::nStages(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  i8 nStages = m_pFormworkSpec->nItems();
  
  return nStages;
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StageSpec* PipelineSpec::pStageSpec(i8 i_Stage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  StageSpec *pStageSpec = m_pFormworkSpec->pGet(i_Stage);

  return pStageSpec;
}

/*----------------------------------------------------------------------------*/
/* get sub-pipelines                                                          */
/*----------------------------------------------------------------------------*/
SubPipelines* PipelineSpec::pSubPipelines(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PIPELINESPEC, _FOOTER_);

  return m_pSubPipelines;
}

#endif // WITH_FORMWORK

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
