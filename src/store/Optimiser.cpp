/*============================================================================*/
/* Optimiser.cpp --                                                           */
/*============================================================================*/

#include "Optimiser.hpp"
#include "Cue.hpp"

#include "Dynamo.hpp"

#include "SP_PipeSpec.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Optimiser,CN_OPTIMISER)
__aNewFunction(Optimiser,CN_OPTIMISER)
__vDeleteFunction(Optimiser,CN_OPTIMISER)
__aFooterFunction(Optimiser)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Optimiser::Optimiser(TagSet* p_JobTags)
: Object(CN_OPTIMISER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
    
  m_p_JobTags      = p_JobTags;
  m_pUnOptPipeSpec = nullptr;
  m_pCardCtl       = new CardCtl;
  
  m_pBench       = new Bench(this);

  m_pDissector   = new Dissector(m_pBench);
  m_pTransformer = new Transformer(m_pBench);
  m_pWeaver      = new Weaver(m_pBench);

  _checkInitialised_(CN_OPTIMISER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Optimiser::~Optimiser(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
  
  delete m_pCardCtl;
  
  delete m_pBench;

  delete m_pDissector;
  delete m_pTransformer;
  delete m_pWeaver;

  Card::vCardLeaks();
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the optimiser                                                       */
/*----------------------------------------------------------------------------*/
void Optimiser::vRepair(Object* pObject)
{
  _static_

  Optimiser* pOptimiser = _pOptimiser(pObject);

  delete pOptimiser;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get bench                                                                  */
/*----------------------------------------------------------------------------*/
Bench* Optimiser::pBench(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  return m_pBench;
}

/*----------------------------------------------------------------------------*/
/* get cardCtl                                                                */
/*----------------------------------------------------------------------------*/
CardCtl* Optimiser::pCardCtl(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  return m_pCardCtl;
}

/*----------------------------------------------------------------------------*/
/* dissect                                                                    */
/*----------------------------------------------------------------------------*/
void Optimiser::vDissect(VertexSpec* pm_pUnOptPipeSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
   
  m_pUnOptPipeSpec = pm_pUnOptPipeSpec;
  
  SP_PipeSpec spPipeSpec(m_pUnOptPipeSpec);
  //vTrace("Optimiser: \n%s\n"_Stz,spPipeSpec.sz());
      
  m_pDissector->vDissectSpec(m_pUnOptPipeSpec);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //vTransform();

  return;
}

/*----------------------------------------------------------------------------*/
/* get cardCtl                                                                */
/*----------------------------------------------------------------------------*/
TagSet* Optimiser::pJobTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  return m_p_JobTags;
}

/*----------------------------------------------------------------------------*/
/* set m_pUnOptPipeSpec                                                   */
/*----------------------------------------------------------------------------*/
void Optimiser::vPipeSpec(VertexSpec* pm_pUnOptPipeSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
    
  if (m_pUnOptPipeSpec) {
    //delete m_pUnOptPipeSpec;                                        LEAK!
    m_pUnOptPipeSpec = nullptr;
  }
  
  m_pUnOptPipeSpec = pm_pUnOptPipeSpec;
}

/*----------------------------------------------------------------------------*/
/* pop & delete top board                                                     */
/*----------------------------------------------------------------------------*/
void Optimiser::vPopBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  Board* pBoard = m_pBench->pPop();

  if (pBoard)
    delete pBoard;
}

/*----------------------------------------------------------------------------*/
/* pop & delete top board                                                     */
/*----------------------------------------------------------------------------*/
void Optimiser::vPushBoard(Board* pBoard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  m_pBench->vPush(pBoard);
}

/*----------------------------------------------------------------------------*/
/* get query name                                                             */
/*----------------------------------------------------------------------------*/
Stz* Optimiser::szQueryNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  return m_sQueryNm;
}

/*----------------------------------------------------------------------------*/
/* set query name                                                             */
/*----------------------------------------------------------------------------*/
void Optimiser::vQueryNm(Stz* szQueryNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  m_sQueryNm = szQueryNm;
}

/*----------------------------------------------------------------------------*/
/* run dynamo                                                                 */
/*----------------------------------------------------------------------------*/
void Optimiser::vToPipeSpec(VertexSpec* pPipeSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
 
  vPipeSpec(pPipeSpec);
  
  delete m_pBench->pPop();
  
  m_pDissector->vDissectSpec(m_pUnOptPipeSpec);
}

/*----------------------------------------------------------------------------*/
/* get ewaved statement                                                       */
/*----------------------------------------------------------------------------*/
Stz* Optimiser::szWeave(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  Board* pBoard_query = m_pBench->pGetBoard(BK_SUBPIPE);
  m_pWeaver->vWeaveOptStmt(pBoard_query);

  Stz* szPipeline = m_pWeaver->szOptStmt();

  return szPipeline;
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Optimiser::bTransform(eEC* peErrorCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  Job* pJob = MY::pJob();
  TagSet* pJobTags = pJob->pJobTags();
  bool bInteractive = pJobTags->bHasName("<interactive>"_Stz);

  Board* pBoard = m_pBench->pHead();

  bool bClean = m_pTransformer->bTransform(peErrorCode);

   if (bClean && bInteractive) {
    m_pWeaver->vWeaveOptStmt(pBoard);
    vTrace("%s\n"_Stz,m_pWeaver->szOptStmt());
    Cue::vGetReturn("$$ The Optimised Pipeline >> "_Stz);
  }

   return bClean;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Transformer* Optimiser::pTransformer(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPTIMISER, _FOOTER_);

  return m_pTransformer;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
