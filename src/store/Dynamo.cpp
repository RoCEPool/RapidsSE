/*============================================================================*/
/* Dynamo.cpp -- Dynamo (pipeline plan compiler)                              */
/*============================================================================*/

#include "BASE.h"

#include "PipelineSpec.hpp"
#include "Dynamo.hpp"
#include "Job.hpp"

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
Dynamo::Dynamo(Stz* szPipelineStmt, Job* p_PipelineJob)
: Object(CN_DYNAMO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  m_p_PipelineJob = p_PipelineJob;
  m_p_PipelineSpec_root = nullptr;
  
  m_pScanner = new Scanner(szPipelineStmt);
    
  _checkInitialised_(CN_DYNAMO);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dynamo::~Dynamo(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
    
  delete m_pScanner;
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
        
  m_pScanner->vTokenBegin(0);
      
  if (PipelineSpec::bCompile(nullptr,
                             m_p_PipelineJob->pJobTags(),
                             m_pScanner,                                
                             m_p_PipelineSpec_root)) {
    
    PipelineSpec* pPipelineSpce = m_p_PipelineSpec_root;
    m_p_PipelineSpec_root = nullptr;
    
    return pPipelineSpce;
  }
  
  m_p_PipelineSpec_root->vGuardedDelete(m_p_PipelineSpec_root,
                                        this,(Stz*)__func__);
  //delete m_p_PipelineSpec_root;
  //m_p_PipelineSpec_root = nullptr;
  
  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
