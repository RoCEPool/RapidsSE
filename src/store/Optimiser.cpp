/*============================================================================*/
/* Optimiser.cpp --                                                           */
/*============================================================================*/

#include "Optimiser.hpp"
#include "Cue.hpp"

#include "SP_PipelineSpec.hpp"

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
Optimiser::Optimiser(PipelineSpec* pUnOptPipelineSpec,
                     i8 iOptimiserLvl)
: Object(CN_OPTIMISER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
    
  m_iOptimiserLvl = iOptimiserLvl;
  m_pUnOptPipelineSpec = pUnOptPipelineSpec;
  
  m_pBench   = new Bench;
  m_pDissect = new Dissect;
  m_pRules   = new Rules;
  m_pTools   = new Tools;
  m_pWeave   = new Weave;

  _checkInitialised_(CN_OPTIMISER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Optimiser::~Optimiser(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
  
  delete m_pBench;
  delete m_pDissect;
  delete m_pRules;
  delete m_pTools;
  delete m_pWeave;
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the ruler                                                           */
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
/* run                                                                        */
/*----------------------------------------------------------------------------*/
void Optimiser::vOptimise(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);
  
  bool bCue   = false;
  bool bPrint = true;

  if (! m_iOptimiserLvl)
    return;
 
    
  SP_PipelineSpec spPipelineSpec(m_pUnOptPipelineSpec);
  vTrace("Optimiser: %s\n"_Stz,spPipelineSpec.sz());
    
  if (1 == m_iOptimiserLvl)
    return;
  
  m_pDissect->vDissectSpec(m_pBench,m_pUnOptPipelineSpec);
  
  m_pBench->vRenumberCards();
  if (bPrint)
    m_pBench->vPrintBench();
  if (bCue)
    Cue::vGetReturn("dissect>> "_Stz);
  
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  /*
  m_pTools->vGenerativeStreamDefs(m_pBench);
  m_pBench->vRenumberCards();
  if (bPrint)
    m_pBench->vPrintBench();
  if (bCue)
    Cue::vGetReturn("expand>> "_Stz);

  m_pTools->vSubstituteOrdinals(m_pBench);
  m_pBench->vRenumberCards();
  if (bPrint)
    m_pBench->vPrintBench();
  if (bCue)
    Cue::vGetReturn("ordinals>> "_Stz);

  m_pTools->vProjectiveStreamDefs(m_pBench);
  m_pBench->vRenumberCards();
  if (bPrint)
    m_pBench->vPrintBench();
  if (bCue)
    Cue::vGetReturn("reproject>> "_Stz);
  */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  /*
  Board* pBoard = m_pBench->pGet(0);
  Card_Scan* pCard_Scan = m_pTools->pFind_Scan_Filter(pBoard);
  
  if (! pCard_Scan)
    return;
  
  vTrace("Found pFind_Scan_Filter\n"_Stz);
  pCard_Scan->vMark1('F');

  if (bPrint)
    m_pBench->vPrintBench();

  Table* pTable = pCard_Scan->pTable();
  IdxVec* pIdxVec = pTable->pIdxVec();
  if (! pIdxVec) {
    vTrace("No IdxVec\n"_Stz);
    return;
  }

  i8 nIndexes = pIdxVec->nItems();
  if (! nIndexes) {
    vTrace("No nIndexes\n"_Stz);
    return;
  }
  
  i8 i_Filter = pBoard->i_Find(pCard_Scan) + 1;
  Card_Filter* pCard_Filter = _pCard_Filter(pBoard->pGet(i_Filter));


  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    Index* pIndex = pIdxVec->pGet(i_Index);
    vTrace("Index %s\n"_Stz,pIndex->szIndexNm());
    
    Card_CandidateIndex* pCard_CandidateIndex =
      new Card_CandidateIndex(pIndex);
    
    pBoard->vInsertAfter(pCard_CandidateIndex,pCard_Filter);
  }
  
  if (bPrint)
    m_pBench->vPrintBench();
  */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  
  
  if (2 >= m_iOptimiserLvl)
    return;
  
  
  
  
  
  
  /*
  
  
  
  m_pTools->vGenerativeStreamDefs(m_pBench);
  m_pBench->vRenumberCards();
  if (bPrint)
    m_pBench->vPrintBench();
  if (bCue)
    Cue::vGetReturn("expand>> "_Stz);
  
  m_pTools->vSubstituteOrdinals(m_pBench);
  m_pBench->vRenumberCards();
  if (bPrint)
    m_pBench->vPrintBench();
  if (bCue)
    Cue::vGetReturn("ordinals>> "_Stz);
  
  m_pTools->vProjectiveStreamDefs(m_pBench);
  m_pBench->vRenumberCards();
  if (bPrint)
    m_pBench->vPrintBench();
  if (bCue)
    Cue::vGetReturn("reproject>> "_Stz);
    
  m_pTools->vStreamOrder(m_pBench);
  m_pBench->vRenumberCards();
  if (bPrint)
    m_pBench->vPrintBench();
  if (bCue)
    Cue::vGetReturn("streamOrder>> "_Stz);
    
//  m_pTools->vRenumberCards(this);
//  if (bCue)
//    Cue::vGetReturn("rework>> "_Stz);

  Board* pBoard = m_pBench->pGet(0);
  m_pWeave->vReassemble(pBoard);
  if (bCue)
    Cue::vGetReturn("synth>> "_Stz);
   
  */
}


/*----------------------------------------------------------------------------*/
/* run                                                                        */
/*----------------------------------------------------------------------------*/
Stz* Optimiser::szWeave(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPTIMISER,_FOOTER_);

  Board* pBoard = m_pBench->pGet(0);
  m_pWeave->vWeaveOptimisedStmt(pBoard);

  Stz* szPipeline = m_pWeave->szWeavedStmt();

  return szPipeline;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
