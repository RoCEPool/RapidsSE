/*============================================================================*/
/* Optimiser.cpp --                                                           */
/*============================================================================*/

#include "Optimiser.hpp"
#include "Cue.hpp"

#include "Dynamo.hpp"

#include "SP_PipeSpec.hpp"
#include "Job.hpp"
#include "InfoSet.hpp"
#include "Smoosh.hpp"
#include "Smooth.hpp"
#include "Fold.hpp"
#include "MakeCode.hpp"
#include "MakeFlat.hpp"
#include "MakeOptimal.hpp"
#include "MakeWeavable.hpp"
#include "MakeSimpler.hpp"
#include "CardIter.hpp"

#include "CarryForward.hpp"
#include "Error.hpp"

#include "Weaver.hpp"
#include "Weaver2.hpp"


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
/* transform matrix                                                           */
/*----------------------------------------------------------------------------*/
bool Optimiser::bApplyTransformationMatrix(Error& rxError)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  bool bClean = true;

  //m_pCardCtl->vReset();
  //m_pCardCtl->vPrintSubCards(false,CK_AGGELIST,CK_KEY,CK_PROJECTION,0);
  //m_pCardCtl->vPrintSubCards(CK_AGGELIST, true);
  //m_pCardCtl->vPrintSubCards(CK_KEY, true);

  try {
    //m_pOptimiser->pCardCtl()->vPrintSubCards(false, //CK_AGGELIST,
    //CK_KEY, //CK_PROJECTION,
    //CK_RPNCALC,
    //                                          0);

    if (AM::JOB::bHasTag(C_GLEAN_INTERACTIVE)) {
      vTrace("JobTags\n"_Stz);
      MY::pJob()->pJobTags()->vDumpSet(true);
    }
    pCardCtl()->vSaveShorter();
    pCardCtl()->vSaveXhand();

    vApplyTransformationMatrix();


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
/* apply the recursive transformation matrix                                  */
/*----------------------------------------------------------------------------*/
void Optimiser::vApplyTransformationMatrix(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPTIMISER, _FOOTER_);

  if (! AM::JOB::bHasTag(C_OPTIMISE_PASSTHRU)) {

    MakeSimpler* pMakeSimpler = new MakeSimpler(m_pCI);
    pMakeSimpler->vSimplify();
    delete pMakeSimpler;
    m_pCI->m_pBench->vMadeSimpler(true);

    Bench::vValidate(m_pCI);


    MakeFlat *pMakeFlat = new MakeFlat(m_pCI);
    pMakeFlat->vFlatten();
    delete pMakeFlat;
    m_pCI->m_pBench->vMadeFlat(true);

    Bench::vValidate(m_pCI);

    MakeOptimal *pMakeOptimal = new MakeOptimal(m_pCI);
    pMakeOptimal->vOptimise();
    delete pMakeOptimal;
    m_pCI->m_pBench->vMadeOptimal(true);

    Bench::vValidate(m_pCI);

    MakeWeavable* pMakeWeavable = new MakeWeavable(m_pCI);
    pMakeWeavable->vWeavable();
    delete pMakeWeavable;
    m_pCI->m_pBench->vMadeWeavable(true);

    Bench::vValidate(m_pCI);

    MakeCode* pMakeCode = new MakeCode(m_pCI);
    pMakeCode->vSynthesise();
    m_pPipelineDef = pMakeCode->pPipelineDef();
    pMakeCode->vPipelineDef(nullptr);
    m_pCI->m_pBench->vMadeCode(true);

    delete pMakeCode;
    Bench::vValidate(m_pCI);

  }
  else {

    MakeSimpler* pMakeSimpler = new MakeSimpler(m_pCI);
    pMakeSimpler->vSimplify_passThru();
    delete pMakeSimpler;
    m_pCI->m_pBench->vMadeSimpler(true);

    //m_pCI->vPrintBoard();

    MakeCode* pMakeCode = new MakeCode(m_pCI);
    pMakeCode->vSynthesise();
    m_pPipelineDef = pMakeCode->pPipelineDef();
    pMakeCode->vPipelineDef(nullptr);
    m_pCI->m_pBench->vMadeCode(true);

    delete pMakeCode;

  }

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

  if (AM::JOB::bHasTag(C_WEAVER2)) {
    Weaver2 *pWeaver2 = new Weaver2(m_pBench,m_pPipelineDef);

    Board *pBoard_query = m_pBench->pGetBoard((i8) 0);
    pWeaver2->vWeaveOptStmt(pBoard_query);

    if (! m_pOptStmt)
      m_pOptStmt = new ExtendedText;
    m_pOptStmt->vCopy(pWeaver2->szOptStmt());

    if (pWeaver2->szCodeBlock()) {
      if (!m_pCodeBlock)
        m_pCodeBlock = new ExtendedText;
      m_pCodeBlock->vCopy(pWeaver2->szCodeBlock());
    }

    delete pWeaver2;
  }
  else {
    Weaver *pWeaver = new Weaver(m_pBench);

    Board *pBoard_query = m_pBench->pGetBoard((i8) 0);
    pWeaver->vWeaveOptStmt(pBoard_query);

    if (! m_pOptStmt)
      m_pOptStmt = new ExtendedText;
    m_pOptStmt->vCopy(pWeaver->szOptStmt());

    delete pWeaver;
  }

  return m_pOptStmt->szTextBlock(0);
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Optimiser::bTransform(Error& rxError, InfoSet* pInfoSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  Job* pJob = MY::pJob();
  TagSet* pJobTags = pJob->pJobTags();

  bool bJoinOrder = pJobTags->bHasName("<joinOrder>"_Stz);
  bool bJoinPermutations = pJobTags->bHasName("<JoinPermutations>"_Stz);

  Board* pBoard = m_pBench->pHead();

  bool bClean = bApplyTransformationMatrix(rxError);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (bJoinOrder) {
    RASSERT(pInfoSet);
    Row *pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    m_sJoinOrder.szStz(),
                                    C_VARGS_END);
  }
  else if (bJoinPermutations) {
    RASSERT(pInfoSet);
    for (i8 iIndex = 0; iIndex < m_nJoinPermutations; iIndex++)
      Row *pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                      m_sJoinPermutation[iIndex].szStz(),
                                      C_VARGS_END);
  }

   return bClean;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
