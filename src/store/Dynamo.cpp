/*============================================================================*/
/* Dynamo.cpp -- Dynamo (pipeline plan compiler)                              */
/*============================================================================*/

#include "BASE.h"

#include "PipelineSpec.hpp"
#include "Dynamo.hpp"

#include "SP_PipelineSpec.hpp"

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
bool Dynamo::bCompile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
        
  m_pScanner->vTokenBegin(0);
      
  if (!  PipelineSpec::bCompile(nullptr,
                                nullptr,
                                m_pScanner,
                                this,
                                
                                m_p_PipelineSpec_root))
    return false;
  
  
#ifdef WITH_NESTED_PIPELINES
  if (m_p_PipelineSpec_root->szPipelineNm()->bSame("nestTest"_Stz)) {
    vShow();
    vDebugPoint();
  }
#endif

  return true;
}

/*----------------------------------------------------------------------------*/
/* get the pipeline spec                                                      */
/*----------------------------------------------------------------------------*/
PipelineSpec* Dynamo::pPipelineSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  return m_p_PipelineSpec_root;
}

/*----------------------------------------------------------------------------*/
/* show the pipeline                                                          */
/*----------------------------------------------------------------------------*/
void Dynamo::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DYNAMO,_FOOTER_);
  
  SP_PipelineSpec spPipelineSpec(m_p_PipelineSpec_root);
  
  vTrace("\nPipelineSpec:\n%s\n"_Stz,spPipelineSpec.sz());

  vTrace("- - - - - - - - - - - - - - - - - -\n\n"_Stz);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
