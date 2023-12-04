/*============================================================================*/
/* Optimiser.cpp --                                                           */
/*============================================================================*/

#include "Optimiser.hpp"

#include "Dynamo.hpp"

#include "SP_PipeSpec.hpp"
#include "Job.hpp"
#include "InfoSet.hpp"
#include "XF_Synthesise_code.hpp"
#include "CardIter.hpp"

#include "CarryForward.hpp"
#include "Error.hpp"

#include "Weaver.hpp"
#include "XF.hpp"


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

  m_sJoinOrder   = "(none)"_Stz;

  m_nJoinPermutations = 0;

  for (i8 iIndex = 0; iIndex < C_PERMUTATIONS; iIndex++) {
    m_sJoinPermutation[iIndex] = "(none)"_Stz;
  }

  m_pCI = new CardIter(m_pBench);

  m_pPipelineDef = nullptr;

  m_pOptStmt = nullptr;
  m_pCodeBlock = nullptr;

  _checkInitialised_(CN_OPTIMISER);

//  MY::pJob()->pJobTags()->vAppend(C_OPTIMISER_TRACE);
//  MY::pJob()->pJobTags()->vAppend(C_PIPELINE_DEF);
//  MY::pJob()->pJobTags()->vAppend(C_WEAVED);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Optimiser::~Optimiser(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  //RASSERT(0 == m_pCI->nItems());

  if (m_pCardCtl->pCardHistory())
    m_pCardCtl->pCardHistory()->vPrintImages(CK_o_GBYAGG, AL_PARTITION);

  delete m_pCardCtl;
  
  delete m_pBench;

  delete m_pDissector;

  delete m_pCI;

  delete m_pPipelineDef;

  delete m_pOptStmt;
  delete m_pCodeBlock;

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
/* apply transforms                                                           */
/*----------------------------------------------------------------------------*/
bool Optimiser::bApplyTransformations(Error& rxError)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  bool bClean = false;

  try {

    if (AM::JOB::bHasTag(C_OPTIMISER_TRACE)) {
      vTrace("JobTags\n"_Stz);
      MY::pJob()->pJobTags()->vDumpSet(true);
    }

    pCardCtl()->vSaveShorter();
    pCardCtl()->vSaveXhand();

    XF::vXF(m_pCI);

    bClean = true;
  }

  catch (Exception xException) {
    // set peErrorCode
    bClean = false;
  }

  catch (Error xError) {
    rxError.vSet(xError);
    bClean = false;
  }

  return bClean;
}

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
/* get code block                                                             */
/*----------------------------------------------------------------------------*/
ExtendedText* Optimiser::pCodeBlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  return m_pCodeBlock;
}

/*----------------------------------------------------------------------------*/
/* set code block                                                             */
/*----------------------------------------------------------------------------*/
void Optimiser::vCodeBlock(ExtendedText* pCodeBlock)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  m_pCodeBlock = pCodeBlock;
}

/*----------------------------------------------------------------------------*/
/* dissect                                                                    */
/*----------------------------------------------------------------------------*/
void Optimiser::vDissect(VertexSpec* pm_pUnOptPipeSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
   
  m_pUnOptPipeSpec = pm_pUnOptPipeSpec;
  
  //SP_PipeSpec spPipeSpec(m_pUnOptPipeSpec);
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
/* get join order string                                                      */
/*----------------------------------------------------------------------------*/
i8 Optimiser::nJoinPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  return m_nJoinPermutations;
}

/*----------------------------------------------------------------------------*/
/* get join order string                                                      */
/*----------------------------------------------------------------------------*/
void Optimiser::vJoinPermutations(i8 iJoinPermutations)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  m_nJoinPermutations = iJoinPermutations;
}

/*----------------------------------------------------------------------------*/
/* get join order string                                                      */
/*----------------------------------------------------------------------------*/
Stz* Optimiser::szJoinPermutation(i8 iJoinPermutation)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  return m_sJoinPermutation[iJoinPermutation];
}

/*----------------------------------------------------------------------------*/
/* set join order string                                                      */
/*----------------------------------------------------------------------------*/
void Optimiser::vJoinPermutation(i8 iJoinPermutation, Stz* szJoinPermutation)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  m_sJoinPermutation[iJoinPermutation] = szJoinPermutation;
}

/*----------------------------------------------------------------------------*/
/* get join order string                                                      */
/*----------------------------------------------------------------------------*/
Stz* Optimiser::szJoinOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  return m_sJoinOrder;
}

/*----------------------------------------------------------------------------*/
/* set join order string                                                      */
/*----------------------------------------------------------------------------*/
void Optimiser::vJoinOrder(Stz* szJoinOrder)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  m_sJoinOrder = szJoinOrder;
}

/*----------------------------------------------------------------------------*/
/* get code block                                                             */
/*----------------------------------------------------------------------------*/
ExtendedText* Optimiser::pOptStmt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  return m_pOptStmt;
}

/*----------------------------------------------------------------------------*/
/* set code block                                                             */
/*----------------------------------------------------------------------------*/
void Optimiser::vOptStmt(ExtendedText* pOptStmt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  m_pOptStmt = pOptStmt;
}

/*----------------------------------------------------------------------------*/
/* retrieve m_pPipelineDef                                                    */
/*----------------------------------------------------------------------------*/
PipelineDef* Optimiser::pPipelineDef(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPTIMISER, _FOOTER_);

  return m_pPipelineDef;
}

/*----------------------------------------------------------------------------*/
/* store m_pPipelineDef                                                       */
/*----------------------------------------------------------------------------*/
void Optimiser::vPipelineDef(PipelineDef* pPipelineDef)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPTIMISER, _FOOTER_);

  m_pPipelineDef = pPipelineDef;
}

/*----------------------------------------------------------------------------*/
/* set m_pUnOptPipeSpec                                                       */
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
/* get weaved statement                                                       */
/*----------------------------------------------------------------------------*/
Stz* Optimiser::szWeave(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  Weaver *pWeaver = new Weaver(m_pBench, m_pPipelineDef);

  Board *pBoard_query = m_pBench->pGetBoard((i8) 0);
  pWeaver->vWeaveOptStmt(pBoard_query);

  m_pOptStmt = pWeaver->pOptStmt();
  pWeaver->vOptStmt(nullptr);

  if (pWeaver->pCodeBlock()) {
    m_pCodeBlock = pWeaver->pCodeBlock();
    pWeaver->vCodeBlock(nullptr);
  }

  delete pWeaver;

  return m_pOptStmt->szTextBlock(0);
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Optimiser::bTransform(Error& rxError, RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  Job* pJob = MY::pJob();
  TagSet* pJobTags = pJob->pJobTags();

  bool bJoinOrder = pJobTags->bHasName("<joinOrder>"_Stz);
  bool bJoinPermutations = pJobTags->bHasName("<JoinPermutations>"_Stz);

  bool bClean = bApplyTransformations(rxError);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (bJoinOrder) {
    RASSERT(pRowBuffer);
    Row *pRow = pRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                    m_sJoinOrder.szStz(),
                                    C_VARGS_END);
    UNUSED(pRow);

  }
  else if (bJoinPermutations) {
    RASSERT(pRowBuffer);
    for (i8 iIndex = 0; iIndex < m_nJoinPermutations; iIndex++) {
      Row* pRow = pRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                      m_sJoinPermutation[iIndex].szStz(),
                                      C_VARGS_END);
      UNUSED(pRow);
    }
  }

   return bClean;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
