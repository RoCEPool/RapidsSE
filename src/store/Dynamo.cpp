/*============================================================================*/
/* Dynamo.cpp -- Dynamo (pipeline plan compiler)                              */
/*============================================================================*/

#include "BASE.h"

#include "PipeSpec.hpp"
#include "Dynamo.hpp"
#include "Job.hpp"
#include "Optimiser.hpp"
#include "TagConstants.h"
#include "Cue.hpp"

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
Dynamo::Dynamo(TagSet* p_JobTags)
: Object(CN_DYNAMO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  m_p_JobTags      = p_JobTags;
  
  m_szPipelineStmt = nullptr;
  m_p_PipeSpec = nullptr;
  m_bWasOptimised  = false;
  //m_sOptPipelineStmt
  
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
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair a dynamo                                                            */
/*----------------------------------------------------------------------------*/
void Dynamo::vRepair(Object* pObject)
{
  _static_

  Dynamo* pDynamo = _pDynamo(pObject);

  delete pDynamo;
}

/*----------------------------------------------------------------------------*/
/* convert pipeline statement to pipeline spec                                */
/*----------------------------------------------------------------------------*/
PipeSpec* Dynamo::pToPipeSpec(Stz* szPipelineStmt, TagSet* p_JobTags)
{
  _static_
  
  Dynamo* pDynamo = new Dynamo(p_JobTags);
  
  PipeSpec* pPipeSpec = pDynamo->pCompile(szPipelineStmt);

  delete pDynamo;
  
  return pPipeSpec;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* recursively compile the tokenised pipeline statement                       */
/*----------------------------------------------------------------------------*/
PipeSpec* Dynamo::pCompile(Stz* szPipelineStmt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
        
  m_szPipelineStmt = szPipelineStmt;
  
  Scanner* pScanner = new Scanner(m_szPipelineStmt);
  
  if (! (pScanner->pTkn()->bSame("PIPELINE"_Stz) ||
         pScanner->pTkn()->bSame("PIPELINELOCAL"_Stz))) {
    delete pScanner;
    return nullptr;
  }
  

  m_p_PipeSpec = new PipeSpec(pScanner,m_p_JobTags);

  
  if (m_p_PipeSpec->bCompile()) {
    
    //vOptimise();
 
    delete pScanner;
    
    return m_p_PipeSpec;
  }
  
  delete m_p_PipeSpec;
  
  ASSERT(! m_p_PipeSpec);
  
  delete pScanner;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* optimise the pipeline statement                                            */
/*----------------------------------------------------------------------------*/
void Dynamo::vOptimise(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  Job* pJob = MY::pJob();
      
  if (! bWillOptimise())
    return;
  
  vTrace("vOptimise: \n%s\n"_Stz,m_szPipelineStmt);
  Cue::vGetReturn("optimise >> "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Optimiser* pOptimiser = new Optimiser(m_p_JobTags);
  
  pOptimiser->vDissect(m_p_PipeSpec);
                                        
  pOptimiser->vOptimise();

  Stz* szWeavedStmt = pOptimiser->szWeave();
  vTrace("\nWeave:\n%s\n"_Stz, szWeavedStmt);
  
  Cue::vGetReturn("weave >> "_Stz);

  pJob->vCmd(szWeavedStmt);
  
  m_bWasOptimised = true;
   
  m_p_JobTags->vDumpSet(true);

  delete pOptimiser;
}

/*----------------------------------------------------------------------------*/
/* get the pipeline spec                                                      */
/*----------------------------------------------------------------------------*/
PipeSpec* Dynamo::pPipeSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  return m_p_PipeSpec;
}

/*----------------------------------------------------------------------------*/
/* was the statement optimised                                                */
/*----------------------------------------------------------------------------*/
bool Dynamo::bWasOptimised(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  return m_bWasOptimised;
}

/*----------------------------------------------------------------------------*/
/* will the pipeline statement get optimised ?                                */
/*----------------------------------------------------------------------------*/
bool Dynamo::bWillOptimise(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  Job* pJob = MY::pJob();
  
  Stz* szPipelineNm = m_p_PipeSpec->szPipeSpecNm();
  
  i8 iOptimiserLvl = AM::SYSARGS::iOptimiserLvl();

  if (   ! iOptimiserLvl
      || ! pJob->bIsOriginator()
      ||   szPipelineNm->bSame("sqlCompiler"_Stz)
      || ! m_p_PipeSpec->pJobTags()->bHasName(C_TAG_OPTIMISE)
      ||   m_p_PipeSpec->pJobTags()->bHasName(C_TAG_OPTIMISED)
      )
    return false;
  else
    return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
