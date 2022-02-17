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
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // add candidateIndexes after scan + filter
  
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


  Card_CandidateIndex* pCard_CandidateIndex = nullptr;
  
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    Index* pIndex = pIdxVec->pGet(i_Index);
    vTrace("Index %s\n"_Stz,pIndex->szIndexNm());
    
    pCard_CandidateIndex =
      new Card_CandidateIndex(pIndex);
    
    //i8 iRank = m_pTools->iRankIndex(pIndex,pCard_ComparePred);
    
    //pCard_CandidateIndex->vRank(iRank);
    
    pBoard->vInsertAfter(pCard_CandidateIndex,pCard_Filter);
  }
  
  if (bPrint)
    m_pBench->vPrintBench();
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // test index tools
  
  Card_FilterPred* pCard_FilterPred =
    _pCard_FilterPred(pCard_Filter->pFindSubCard(CK_FILTERPRED));
  
  i8 nUnits = pBoard->nItems();
  for (i8 i_Unit = 0; i_Unit < nUnits; i_Unit++) {
    Card* pUnit = pBoard->pGet(i_Unit);
    
    if (CK_CANDIDATEINDEX != pUnit->eCard())
      continue;
      
    Index* pIndex = pUnit->pIndex();
    
    if (m_pTools->bHasBounds(pIndex,pCard_FilterPred)) {
      vTrace("has bounds\n"_Stz);
      
      Card_ComparePred* pFound_LBound =
        m_pTools->pFindBound(pIndex,BK_LBOUND,pCard_FilterPred);
      
      if (pFound_LBound) {
        vTrace("found lbound\n"_Stz);
        pFound_LBound->vDump();
      }
      else {
        vTrace("unfound lbound\n"_Stz);
      }
      
      Card_ComparePred* pFound_UBound =
        m_pTools->pFindBound(pIndex,BK_UBOUND,pCard_FilterPred);
      
      if (pFound_UBound) {
        vTrace("found ubound\n"_Stz);
        pFound_UBound->vDump();
      }
      else {
        vTrace("unfound ubound\n"_Stz);
      }
      
      Card_ComparePred* pFound_EBound =
        m_pTools->pFindBound(pIndex,BK_EBOUND,pCard_FilterPred);
      
      if (pFound_EBound) {
        vTrace("found ebound\n"_Stz);
        pFound_UBound->vDump();
      }
      else {
        vTrace("unfound ebound\n"_Stz);
      }
      

    }
    else {
      vTrace("no bounds\n"_Stz);
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // find lbound in candidate index, remove lbound, reduce filterPred
  
  for (i8 i_Unit = 0; i_Unit < nUnits; i_Unit++) {
    Card* pUnit = pBoard->pGet(i_Unit);

    if (CK_CANDIDATEINDEX != pUnit->eCard())
      continue;
      
    Index* pIndex = pUnit->pIndex();

    Card_ComparePred* pFound_LBound =
      m_pTools->pFindBound(pIndex,BK_LBOUND,pCard_FilterPred);

    RASSERT(pFound_LBound);
    
    pCard_FilterPred->vDump();
    
    bool b = m_pTools->bRemoveBound(pCard_FilterPred,pFound_LBound);
    RASSERT(b);
    
    pCard_FilterPred->vDump();
    
    m_pTools->vReduceFilter(pCard_FilterPred);
    pCard_FilterPred->vDump();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // remove filter if filterPred empty

  for (i8 i_Unit = 0; i_Unit < nUnits; i_Unit++) {
    Card* pUnit = pBoard->pGet(i_Unit);
    
    if (CK_FILTER != pUnit->eCard())
      continue;
    
    Card_Filter* pCard_Filter = _pCard_Filter(pUnit);

    Card_FilterPred* pCard_FilterPred =
      _pCard_FilterPred(pCard_Filter->pFindSubCard(CK_FILTERPRED));
    if (pCard_FilterPred->nItems())
      continue;
    
    pBoard->vRemove(pCard_Filter);
    i_Unit--;
    nUnits--;
    
    delete pCard_Filter;
  }
  
  m_pBench->vRenumberCards();
  if (bPrint)
    m_pBench->vPrintBench();

  // end

  return;

  Card_ComparePred* pCard_ComparePred =
    _pCard_ComparePred(pCard_Filter->pFindSubCard(CK_COMPAREPRED));

  // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  
  m_pTools->vPickIndex(pBoard);
  
  if (bPrint)
    m_pBench->vPrintBench();

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // add indexScan with lbound  from lbound of comparePred
  
  Card_LBound* pCard_LBound = new Card_LBound;

  
  i8 nTerms = pCard_ComparePred->nItems();
  for (i8 i_Term = 0; i_Term < nTerms; i_Term++) {
    Card_Term* pCard_Term =
      _pCard_Term(pCard_ComparePred->pGet(i_Term));

    Stz* szTerm = pCard_Term->szTerm();
    
    Card_Term* pCard_Term_lbound = new Card_Term(szTerm);

    
    pCard_LBound->vAppend(pCard_Term_lbound);
  }
  
  Card_IndexScan* pCard_IndexScan =
    new Card_IndexScan(pCard_CandidateIndex->pIndex());
  
  
  pCard_IndexScan->vAppend(pCard_LBound);

  pBoard->vInsertBefore(pCard_IndexScan,pCard_Scan);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // remove&delete scan & filter
  
  pBoard->vRemove(pCard_Scan);
  pBoard->vRemove(pCard_Filter);
  pBoard->vRemove(pCard_CandidateIndex);

  m_pBench->vRenumberCards();
  if (bPrint)
    m_pBench->vPrintBench();


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
