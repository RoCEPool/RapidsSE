/*============================================================================*/
/* Optimiser.cpp --                                                           */
/*============================================================================*/

#include "Optimiser.hpp"

#include "Dynamo.hpp"

#include "SP_PipeSpec.hpp"
#include "Job.hpp"
#include "MakeCode.hpp"
#include "MakeFlat.hpp"
#include "MakeOptimal.hpp"
#include "MakeWeavable.hpp"
#include "MakeSimpler.hpp"
#include "CardIter.hpp"

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
bool Optimiser::bApplyTransformationMatrix(eEC* peErrorCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  bool bClean = true;

  try {
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
    *peErrorCode = xError.eErrorCode();
    // todo: this should read xError.szErrorMsg()
    //Stz* szError = xError.aErrorMsg();

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

  MakeSimpler* pMakeSimpler = new MakeSimpler(m_pCI);
  pMakeSimpler->vSimplify();
  delete pMakeSimpler;

  Board::vValidate(m_pCI);

  MakeFlat* pMakeFlat = new MakeFlat(m_pCI);
  pMakeFlat->vFlatten();
  delete pMakeFlat;

  MakeOptimal* pMakeOptimal = new MakeOptimal(m_pCI);
  pMakeOptimal->vOptimise();
  delete pMakeOptimal;

  MakeWeavable* pMakeWeavable = new MakeWeavable(m_pCI);
  pMakeWeavable->vWeavable();
  delete pMakeWeavable;

  MakeCode* pMakeCode = new MakeCode(m_pCI);
  pMakeCode->vSynthesise();
  m_pPipelineDef = pMakeCode->pPipelineDef();
  pMakeCode->vPipelineDef(nullptr);

  delete pMakeCode;
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
/* dissect                                                                    */
/*----------------------------------------------------------------------------*/
void Optimiser::vDissect(VertexSpec* pm_pUnOptPipeSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
   
  m_pUnOptPipeSpec = pm_pUnOptPipeSpec;
  
  SP_PipeSpec spPipeSpec(m_pUnOptPipeSpec);

  m_pDissector->vDissectSpec(m_pUnOptPipeSpec);
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

  Stz* szPipeline;

  Board* pBoard_query = m_pBench->pGetBoard((i8)0);

  if (AM::JOB::bHasTag(C_WEAVER2)) {
    Weaver2* pWeaver2 = new Weaver2(m_pBench,nullptr);
    pWeaver2->vWeaveOptStmt(pBoard_query);
    szPipeline = pWeaver2->szOptStmt();
    delete pWeaver2;
  }
  else {
    Weaver *pWeaver = new Weaver(m_pBench);
    pWeaver->vWeaveOptStmt(pBoard_query);
    szPipeline = pWeaver->szOptStmt();
    delete pWeaver;
  }

  return szPipeline;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
