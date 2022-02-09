/*============================================================================*/
/* Dynamo.cpp -- Dynamo (pipeline plan compiler)                              */
/*============================================================================*/

#include "BASE.h"

#include "PipelineSpec.hpp"
#include "Dynamo.hpp"
#include "Job.hpp"
#include "Optimiser.hpp"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Dynamo,CN_DYNAMO)
__aNewFunction(Dynamo,CN_DYNAMO)
__vDeleteFunction(Dynamo,CN_DYNAMO)
__aFooterFunction(Dynamo)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dynamo::Dynamo(Stz* szPipelineStmt, TagSet* p_JobTags)
: Object(CN_DYNAMO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  m_szPipelineStmt = szPipelineStmt;
  m_p_JobTags      = p_JobTags;
  m_p_PipelineSpec = nullptr;
  m_bWasOptimised  = false;
  
  _checkInitialised_(CN_DYNAMO);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dynamo::~Dynamo(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  //delete m_pScanner;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* recursively compile the tokenised pipeline statement                       */
/*----------------------------------------------------------------------------*/
PipelineSpec* Dynamo::pCompile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
        
  Scanner* pScanner = new Scanner(m_szPipelineStmt);
  
  if (! (pScanner->pTkn()->bSame("PIPELINE"_Stz) ||
         pScanner->pTkn()->bSame("PIPELINELOCAL"_Stz))) {
    delete pScanner;
    return nullptr;
  }
  
  Stz* szPipelineNm = pScanner->pTkn(1)->szChars();

  m_p_PipelineSpec = new PipelineSpec(pScanner,m_p_JobTags);

  
  if (m_p_PipelineSpec->bCompile()) {
    
    if (! szPipelineNm->bSame("sqlCompiler"_Stz) &&
        m_p_PipelineSpec->pJobTags()->bHasName(C_TAG_OPTIMISE)) {
      Optimiser* pOptimiser = new Optimiser(m_p_PipelineSpec);
      pOptimiser->vOptimise(AM::SYSARGS::iOptimiserLvl());
      //delete m_p_PipelineSpec;
      m_p_PipelineSpec = pOptimiser->pPipelineSpec_original();

      vTrace("Weave:\n%s\n"_Stz, pOptimiser->szWeave());
      
      delete pOptimiser;
    }
    delete pScanner;
    
    return m_p_PipelineSpec;
  }
  
  m_p_PipelineSpec->vGuardedDelete(CN_PIPELINESPEC,&m_p_PipelineSpec,
                                        CN_DYNAMO,this,(Stz*)__func__);
  
  ASSERT(! m_p_PipelineSpec);
  
  delete pScanner;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get the pipeline spec                                                      */
/*----------------------------------------------------------------------------*/
PipelineSpec* Dynamo::pPipelineSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  return m_p_PipelineSpec;
}

/*----------------------------------------------------------------------------*/
/* was the statement optimised                                                */
/*----------------------------------------------------------------------------*/
bool Dynamo::bWasOptimised(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  return m_bWasOptimised;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
