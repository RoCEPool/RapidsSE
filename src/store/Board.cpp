/*============================================================================*/
/* Board.cpp -- Board                                                         */
/*============================================================================*/

#include "Board.hpp"

#include "ShardDef.hpp"
#include "Cue.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Board,CN_BOARD)
__aNewFunction(Board,CN_BOARD)
__vDeleteFunction(Board,CN_BOARD)
__aFooterFunction(Board)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Board::Board(Stz* szQueryNm)
: Object(CN_BOARD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
        
  m_sQueryNm = szQueryNm;
  
  m_pCandidateIndex     = nullptr;
  m_pComparePred_ebound = nullptr;
  m_pComparePred_lbound = nullptr;
  m_pComparePred_ubound = nullptr;
  m_pFilter             = nullptr;
  m_pIndexScan          = nullptr;
  m_pProject            = nullptr;
  m_pScan               = nullptr;
  m_pStage              = nullptr;

  m_pIndex = nullptr;
  m_pTable = nullptr;

  _checkInitialised_(CN_BOARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Board::~Board(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* pin board onto given ribbon                                                */
/*----------------------------------------------------------------------------*/
void Board::vPinCards(Ribbon* pRibbon, i8 i_Board)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  if (! nItems())
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pRibbon->vSprint("[%lld] %s"_Stz,i_Board,szQueryNm());
  pRibbon->vTitleLines(1);
  pRibbon->vSetBase();
  pRibbon->vFirstColumn();

  Ribbon* pSubRibbon = new Ribbon(0);
  

  ListExoIter2_t<Card>* pBoard_I = pNewIter();

  i8 nCount = 0;
  while (pBoard_I->bHasNext()) {
    Card* pCard = pBoard_I->pNext();
    
    //if (_bIsCard_Filter(pCard))
    //  pCard->vDump();

    if (! nCount++)
      pSubRibbon->vFirstColumn();
    else
      pSubRibbon->vNextColumn();
    pCard->vCardHeader(pSubRibbon);

    pCard->vPrintCard(pSubRibbon,0,this);
  }
     
  pRibbon->vPinRibbon(1,0,pSubRibbon);
  
  delete pSubRibbon;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vPrintBoard(Stz* szCue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  bool bCue   = true;
  bool bPrint = true;

  vRenumber_Cards();
  
  if (! bPrint)
    return;
  
  Ribbon* pRibbon = new Ribbon(0);

  vPinCards(pRibbon,0);
  pRibbon->vPrintRibbon();

  delete pRibbon;
  
  bool bCR = false;
  if (m_pTable) {
    vTrace("m_pTable : %s ; "_Stz,m_pTable->szTableNm());
    bCR = true;
  }
  if (m_pIndex) {
    vTrace("m_pIndex : %s"_Stz,m_pIndex->szIndexNm());
    bCR = true;
  }
  if (bCR)
    vTrace("\n"_Stz);

  if (bCue)
    Cue::vGetReturn(szCue);

}

/*----------------------------------------------------------------------------*/
/* get query name                                                             */
/*----------------------------------------------------------------------------*/
Stz* Board::szQueryNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  return m_sQueryNm;
}

/*----------------------------------------------------------------------------*/
/* set query name                                                             */
/*----------------------------------------------------------------------------*/
void Board::vQueryNm(Stz* szQueryNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  m_sQueryNm = szQueryNm;
}

/*----------------------------------------------------------------------------*/
/* call closure for each board                                                */
/*----------------------------------------------------------------------------*/
void Board::vWalkAllCards(std::function<void(Card* pCard)> f_vClosure)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  ListExoIter2_t<Card>* pBoard_I = pNewIter();

  while (pBoard_I->bHasNext()) {
    Card* pCard = pBoard_I->pNext();
    
    pCard->vWalkAllCards(f_vClosure);
  }
  delete pBoard_I;
}


// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCandidateIndex(Card_CandidateIndex* pCandidateIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  if (m_pCandidateIndex)
    m_pCandidateIndex->vMark2(' ');
  
  m_pCandidateIndex = pCandidateIndex;
  
  if (m_pCandidateIndex) {
    m_pCandidateIndex->vMark2('m');
    
    m_pIndex = m_pCandidateIndex->pIndex();
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vComparePred_ebound(Card_ComparePred* pComparePred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  if (m_pComparePred_ebound)
    m_pComparePred_ebound->vMark2(' ');
  
  m_pComparePred_ebound = pComparePred;
  
  if (m_pComparePred_ebound)
    m_pComparePred_ebound->vMark2('E');
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vComparePred_lbound(Card_ComparePred* pComparePred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  if (m_pComparePred_lbound)
    m_pComparePred_lbound->vMark2(' ');
  
  m_pComparePred_lbound = pComparePred;
  
  if (m_pComparePred_lbound)
    m_pComparePred_lbound->vMark2('L');
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vComparePred_ubound(Card_ComparePred* pComparePred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  if (m_pComparePred_ubound)
    m_pComparePred_ubound->vMark2(' ');
  
  m_pComparePred_ubound = pComparePred;
  
  if (m_pComparePred_ubound)
    m_pComparePred_ubound->vMark2('U');
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vFilter(Card_Filter* pFilter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
    
  if (m_pFilter)
    m_pFilter->vMark2(' ');

  m_pFilter = pFilter;
  
  if (m_pFilter)
    m_pFilter->vMark2('m');
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vIndexScan(Card_IndexScan* pIndexScan)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  if (m_pIndexScan)
    m_pIndexScan->vMark2(' ');
  
  m_pIndexScan = pIndexScan;
  
  if (m_pIndexScan)
    m_pIndexScan->vMark2('m');
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vProject(Card_Project* pProject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  if (m_pProject)
    m_pProject->vMark2(' ');
  
  m_pProject = pProject;
  
  if (m_pProject)
    m_pProject->vMark2('m');
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vScan(Card_Scan* pScan)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  if (m_pScan)
    m_pScan->vMark2(' ');
  
  m_pScan = pScan;
  
  if (m_pScan)
    m_pScan->vMark2('m');
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vStage(Card_Stage* pStage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  if (m_pStage)
    m_pStage->vMark2(' ');
  
  m_pStage = pStage;
  
  if (m_pStage)
    m_pStage->vMark2('m');
}



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  m_pIndex = pIndex;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vResetRegisters(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  vCandidateIndex(nullptr);
  vComparePred_ebound(nullptr);
  vComparePred_lbound(nullptr);
  vComparePred_ubound(nullptr);
  vFilter(nullptr);
  vIndexScan(nullptr);
  vProject(nullptr);
  vScan(nullptr);
  vStage(nullptr);

  vIndex(nullptr);
  vTable(nullptr);
}


// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vRankCandidateIndex(Card_CandidateIndex* pCard_CandidateIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  RASSERT(m_pFilter);
  
  i8 iRank = 0;
  bool bEBound = false;
  bool bLBound = false;
  bool bUBound = false;

  Index* pIndex = pCard_CandidateIndex->pIndex();
  
  if (m_pFilter->bHasBounds(pIndex)) {
  
    if (m_pFilter->pFindBound(pIndex,BK_EBOUND)) {
      bEBound = true;
      pCard_CandidateIndex->vAddProperty("EBOUND"_Stz);
    }
    
    if (m_pFilter->pFindBound(pIndex,BK_LBOUND)) {
      bLBound = true;
      pCard_CandidateIndex->vAddProperty("LBOUND"_Stz);
    }
    
    if (m_pFilter->pFindBound(pIndex,BK_UBOUND)) {
      bUBound = true;
      pCard_CandidateIndex->vAddProperty("UBOUND"_Stz);
    }
  }

  if (bEBound)
    iRank = 3;
  else
  if (bLBound && bUBound)
    iRank = 2;
  else
  if (bLBound || bUBound)
    iRank = 1;

  pCard_CandidateIndex->vRank(iRank);
}



// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Board::bAdd_CandidateIndexes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  IdxVec* pIdxVec = m_pTable->pIdxVec();
  if (! pIdxVec) {
    vTrace("No IdxVec\n"_Stz);
    return false;
  }

  i8 nIndexes = pIdxVec->nItems();
  if (! nIndexes) {
    vTrace("No nIndexes\n"_Stz);
    return false;
  }
  
  i8 i_Filter = i_Find(m_pScan) + 1;
  Card_Filter* pCard_Filter = _pCard_Filter(pGet(i_Filter));


  Card_CandidateIndex* pCard_CandidateIndex = nullptr;
  
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    Index* pIndex = pIdxVec->pGet(i_Index);
    vTrace("Index %s\n"_Stz,pIndex->szIndexNm());
    
    pCard_CandidateIndex =
      new Card_CandidateIndex(pIndex);
    
    //i8 iRank = m_pTools->iRankIndex(pIndex,pCard_ComparePred);
    
    //pCard_CandidateIndex->vRank(iRank);
    
    vInsertAfter(pCard_CandidateIndex,pCard_Filter);
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vAdd_IndexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  //m_pFilter->vDump();

  Card_IndexScan* pCard_IndexScan = new Card_IndexScan(m_pIndex);

  Card_BOUND* pCard_BOUND;

  if (m_pComparePred_ebound) {
    pCard_BOUND = new Card_EBound;
    pCard_BOUND->vAppend(m_pComparePred_ebound->pDup());
  }
  else
  if (m_pComparePred_lbound && m_pComparePred_ubound) {
    pCard_BOUND = new Card_RBound;
    pCard_BOUND->vAppend(m_pComparePred_lbound->pDup());
    pCard_BOUND->vAppend(m_pComparePred_ubound->pDup());

  }
  else
  if (m_pComparePred_lbound) {
    pCard_BOUND = new Card_LBound;
    pCard_BOUND->vAppend(m_pComparePred_lbound->pDup());

  }
  else
  if (m_pComparePred_ubound) {
    pCard_BOUND = new Card_UBound;
    pCard_BOUND->vAppend(m_pComparePred_ubound->pDup());
  }
  else {
    RASSERT(0);
  }
  
  pCard_IndexScan->vAppend(pCard_BOUND);
  vInsertBefore(pCard_IndexScan,m_pScan);
  
  vIndexScan(pCard_IndexScan);
  
//  vComparePred_ebound(nullptr);
//  vComparePred_lbound(nullptr);
//  vComparePred_ubound(nullptr);
}
/*----------------------------------------------------------------------------*/
//void Board::vAdd_IndexScan(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  m_pFilter->vDump();
//
//  Card_ComparePred* pCard_ComparePred =
//    _pCard_ComparePred(m_pFilter->pFindSubCard(CK_COMPAREPRED));
//
//  pCard_ComparePred->vDump();
//
//  Card_LBound* pCard_LBound = new Card_LBound;
//
//
//  i8 nTerms = pCard_ComparePred->nItems();
//  for (i8 i_Term = 0; i_Term < nTerms; i_Term++) {
//    Card_Term* pCard_Term =
//      _pCard_Term(pCard_ComparePred->pGet(i_Term));
//
//    Stz* szTerm = pCard_Term->szTerm();
//
//    Card_Term* pCard_Term_lbound = new Card_Term(szTerm);
//
//
//    pCard_LBound->vAppend(pCard_Term_lbound);
//  }
//
//  Card_IndexScan* pCard_IndexScan =
//    new Card_IndexScan(m_pCandidateIndex->pIndex());
//
//
//  pCard_IndexScan->vAppend(pCard_LBound);
//
//  vInsertBefore(pCard_IndexScan,m_pScan);
//
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCull_Filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  if (! m_pFilter->bCull())
    return;
  
  vRemove(m_pFilter);
  
  delete m_pFilter;

  vFilter(nullptr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCut_CandidateIndexes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  vRemoveCards(CK_CANDIDATEINDEX);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCut_ComparePreds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  if (m_pComparePred_ebound)
    m_pFilter->bRemoveBound(m_pComparePred_ebound);
  
  if (m_pComparePred_lbound)
    m_pFilter->bRemoveBound(m_pComparePred_lbound);
  
  if (m_pComparePred_ubound)
    m_pFilter->bRemoveBound(m_pComparePred_ubound);
  
  
  vComparePred_ebound(nullptr);
  vComparePred_lbound(nullptr);
  vComparePred_ubound(nullptr);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCut_Scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  vRemove(m_pScan);
  
  delete m_pScan;
  
  vScan(nullptr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Board::bFind_Scan_Filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  vScan(nullptr);
  
  i8 nCards = nItems();
  
  for (i8 i_TestScan = 0; i_TestScan < nCards-1; i_TestScan++) {
    
    Card* pTestScan = pGet(i_TestScan);
    if (CK_SCAN != pTestScan->eCard())
      continue;

    Card* pTestFilter = pGet(i_TestScan+1);
    if (CK_FILTER != pTestFilter->eCard())
      continue;
    
    vScan(_pCard_Scan(pTestScan));

    vTable(m_pScan->pTable());
    
    vFilter(_pCard_Filter(pTestFilter));

    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vMark_ComparePreds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  i8 iRank = m_pCandidateIndex->iRank();
  
  switch (iRank) {
      
    case 1: {
      // lower bound or upper bound
      Card_ComparePred* pComparePred_lbound =
        m_pFilter->pFindBound(m_pIndex,BK_LBOUND);
      if (pComparePred_lbound) {
        vComparePred_lbound(pComparePred_lbound);
        break;
      }
      Card_ComparePred* pComparePred_ubound =
        m_pFilter->pFindBound(m_pIndex,BK_UBOUND);
      if (pComparePred_ubound) {
        vComparePred_ubound(pComparePred_ubound);
        break;
      }
      break;
    }

    case 2: {
      // lower bound and upper bound
      Card_ComparePred* pComparePred_lbound =
        m_pFilter->pFindBound(m_pIndex,BK_LBOUND);
      if (pComparePred_lbound)
        vComparePred_lbound(pComparePred_lbound);
      Card_ComparePred* pComparePred_ubound =
        m_pFilter->pFindBound(m_pIndex,BK_UBOUND);
      if (pComparePred_ubound)
        vComparePred_ubound(pComparePred_ubound);
      break;
    }
      
    case 3: {
      // equi-bound
      Card_ComparePred* pComparePred_ebound =
        m_pFilter->pFindBound(m_pIndex,BK_EBOUND);
      if (pComparePred_ebound)
        vComparePred_ebound(pComparePred_ebound);
      break;
    }

    default:
      vTrace("SWITCH %s: Unknown iRank %lld\n"_Stz
             ,__func__
             ,(i8)iRank
              );
      RASSERT(0);
      break;

  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Board::bPick_CandidateIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  i8 nCards = nItems();
  
  for (i8 i_Card = 0; i_Card < nCards; i_Card++) {
    Card* pCard = pGet(i_Card);
    
    if (CK_CANDIDATEINDEX != pCard->eCard())
      continue;

    vCandidateIndex(_pCard_CandidateIndex(pCard));
    
    
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vRank_CandidateIndexes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  //m_pFilter->vMark2('0');
                    
  i8 nCards = nItems();
  
  for (i8 i_Card = 0; i_Card < nCards; i_Card++) {
    Card* pCard = pGet(i_Card);
    
    if (CK_CANDIDATEINDEX != pCard->eCard())
      continue;

    Card_CandidateIndex* pCard_CandidateIndex = _pCard_CandidateIndex(pCard);
    
    vRankCandidateIndex(pCard_CandidateIndex);
    
  }
  
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReduce_Filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  if (! m_pFilter)
    return;
  
  m_pFilter->vReduceFilter();
  
  
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vRenumber_Cards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  ListExoIter2_t<Card>* pBoard_I = pNewIter();

  while (pBoard_I->bHasNext()) {
    Card* pCard = pBoard_I->pNext();
    
    pCard->vCardNo(-1);
    pCard->vRenumberSubCards();
  }
  
  delete pBoard_I;
}

/*----------------------------------------------------------------------------*/
/* replace stream column ordinals with stream column names                    */
/*----------------------------------------------------------------------------*/
void Board::vReplace_Ordinals(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  StreamDef* pStreamDef_recent = nullptr;
  Index* pIndex_recent = nullptr;

  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  /* test card and for those with column ref's replace ordinals with names  */
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vReplace = [&pStreamDef_recent,&pIndex_recent](Card* pCard) mutable
  {
    vTrace("vReplace: %s\n"_Stz,pCard->szCardNm());
  
    // get running streamDefn
    if (_bIsCard_StreamDef(pCard)) {
      Card_StreamDef* pCard_StreamDef = _pCard_StreamDef(pCard);
      
      pStreamDef_recent = pCard_StreamDef->pStreamDef();
      return;
    }
  
    // get running index
    if (_bIsCard_Index(pCard)) {
      Card_Index* pCard_Index = _pCard_Index(pCard);
    
      pIndex_recent = pCard_Index->pIndex();
      return;
    }

    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // an AGGREGPRED will contain columns
  
    if (_bIsCard_AggregPred(pCard)) {
      Card_AggregPred* pCard_AggregPred = _pCard_AggregPred(pCard);
      
      pCard_AggregPred->vReplaceOrdinals(pStreamDef_recent);
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // an BYPRED will contain columns

    if (_bIsCard_ByPred(pCard)) {
      Card_ByPred* pCard_ByPred = _pCard_ByPred(pCard);
      
      pCard_ByPred->vReplaceOrdinals(pStreamDef_recent);
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // an COMPAREPRED will contain columns

    if (_bIsCard_ComparePred(pCard)) {
      Card_ComparePred* pCard_ComparePred = _pCard_ComparePred(pCard);
      
      pCard_ComparePred->vReplaceOrdinals(pStreamDef_recent);
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // an INDEXPRED will contain columns

    if (_bIsCard_IndexPred(pCard)) {
      Card_IndexPred* pCard_IndexPred = _pCard_IndexPred(pCard);
      
      pCard_IndexPred->vReplaceOrdinals(pStreamDef_recent,pIndex_recent);
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // an PROJECTPRED will contain columns

    if (_bIsCard_CalcPred(pCard)) {
      Card_CalcPred* pCard_CalcPred = _pCard_CalcPred(pCard);
      
      pCard_CalcPred->vReplaceOrdinals(pStreamDef_recent);
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // otherwise ignore
    return;
  };
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/

  
  //
  // walk all cars, substituting columns names for ordinals
  //

  vWalkAllCards(vReplace);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vResolve_GenerativeStreamDefs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  /*

    A generative work unit can be one of the following:
   
      Scan .1          //  IndexScan .4     //  IndexJoin .6
      0)Table          //  0)Table          //  0)Table
             part      //           orders  //         lineitem
      ...              //  ...              //  ...
      0123456789ABCDE  //  0123456789ABCDE  //  0123456789ABCDE
  
  */
  
  StreamDef* pStreamDef_prior = nullptr;
  
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  /* examine given generative work unit and form the stream defn it emits   */
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vGenerate = [&pStreamDef_prior](Card* pGenerativeUnit) mutable
  {
    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // a generative work unit can either be a SCAN
  
    if (_bIsCard_Scan(pGenerativeUnit)) {
      Card_Scan* pCard_Scan = _pCard_Scan(pGenerativeUnit);

//      Card_Table* pCard_Table
//        = _pCard_Table(pCard_Scan->pFindSubCard(CK_TABLE));
//      Table* pTable = _pTable(pCard_Table->pTable());
      
      Table* pTable = pCard_Scan->pTable();

      
      StreamDef* pStreamDef = new StreamDef(pTable->pStreamDef());
      Card_StreamDef* pCard_StreamDef = new Card_StreamDef(pStreamDef);
      
      pCard_Scan->vAppend(pCard_StreamDef);
      
      pStreamDef_prior = pStreamDef;
      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or an INDEXSCAN
  
    if (_bIsCard_IndexScan(pGenerativeUnit)) {
      Card_IndexScan* pCard_IndexScan = _pCard_IndexScan(pGenerativeUnit);

      Card_Table* pCard_Table
        = _pCard_Table(pCard_IndexScan->pFindSubCard(CK_TABLE));
      Table* pTable = _pTable(pCard_Table->pTable());
      
      StreamDef* pStreamDef = new StreamDef(pTable->pStreamDef());
      Card_StreamDef* pCard_StreamDef = new Card_StreamDef(pStreamDef);
      
      pCard_IndexScan->vAppend(pCard_StreamDef);
      
      pStreamDef_prior = pStreamDef;
      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or an INDEXJOIN

    if (_bIsCard_IndexJoin(pGenerativeUnit)) {
      Card_IndexJoin* pCard_IndexJoin = _pCard_IndexJoin(pGenerativeUnit);
      Table* pTable = pCard_IndexJoin->pIndex()->pTable();

      StreamDef* pStreamDef = new StreamDef(pStreamDef_prior);
      Card_StreamDef* pCard_StreamDef = new Card_StreamDef(pStreamDef);
      
      pStreamDef->vAppendStreamDef(pTable->pStreamDef());
      pCard_IndexJoin->vAppend(pCard_StreamDef);

      pStreamDef_prior = pStreamDef;
      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // otherwise ignored
    return;
  };
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/

  vWalk(vGenerate);
}

/*----------------------------------------------------------------------------*/
/* form the stream definitions for all projective work units                  */
/*----------------------------------------------------------------------------*/
void Board::vResolve_ProjectiveStreamDefs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  /*

    A projective work unit can be one of the following:
   
      AggregList .1    //  OrdAggList .4    //  ProjectList .6
      ...              //  ...              //  ...
      0123456789ABCDE  //  0123456789ABCDE  //  0123456789ABCDE
  
  */

  StreamDef* pStreamDef_prior = nullptr;
  
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  /* examine given projective work unit and form the stream def it emits    */
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vGenerate = [&pStreamDef_prior](Card* pProjectiveUnit) mutable
  {
    //vTrace("vExpandStreamDefs: %s\n"_Stz,pCard->szCardNm());
  
    // get running streamDef from SCAN, INDEXSCAN or INDEXJOIN
    if (     _bIsCard_Scan(pProjectiveUnit) ||
        _bIsCard_IndexScan(pProjectiveUnit) ||
        _bIsCard_IndexJoin(pProjectiveUnit)) {
      Card_StreamDef* pCard_StreamDef
        = _pCard_StreamDef(pProjectiveUnit->pFindSubCard(CK_STREAMDEF));

      pStreamDef_prior = pCard_StreamDef->pStreamDef();
      return;
    }
  
    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // a projective work unit will be either a AGGREGLIST

    if (_bIsCard_Aggreg(pProjectiveUnit)) {
      //vTrace("vReproject : %s\n"_Stz,pProjectiveUnit->szCardNm());
      Card_Aggreg* pCard_Aggreg = _pCard_Aggreg(pProjectiveUnit);
      
      pStreamDef_prior = pCard_Aggreg->pProduceStreamDef();
      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or an ORDAGGLIST

    if (_bIsCard_OrdAgg(pProjectiveUnit)) {
      //vTrace("vReproject : %s\n"_Stz,pProjectiveUnit->szCardNm());
      Card_OrdAgg* pCard_OrdAgg = _pCard_OrdAgg(pProjectiveUnit);
      
      pStreamDef_prior = pCard_OrdAgg->pProduceStreamDef();
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or an PROJECTLIST

    if (_bIsCard_Project(pProjectiveUnit)) {
      Card_Project* pCard_Project = _pCard_Project(pProjectiveUnit);
      
      pStreamDef_prior = pCard_Project->pProduceStreamDef();
      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // otherwise ignored
    return;
  };
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/

  //
  // walk all units, forming the stream definitions for the projective ones
  //

  vWalk(vGenerate);
}


// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vOptimise_Board(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);


  vPrintBoard("vDissectSpec >> "_Stz);

  vResolve_GenerativeStreamDefs();
  vPrintBoard("vGenerativeStreamDefs >> "_Stz);
  
  vReplace_Ordinals();
  vPrintBoard("vReplace_Ordinals >> "_Stz);

  vResolve_ProjectiveStreamDefs();
  vPrintBoard("vResolve_ProjectiveStreamDefs >> "_Stz);

  
  // optional
  
  if (bFind_Scan_Filter()) {
    
    vPrintBoard("bFind_Scan_Filter >> "_Stz);
    
    bool bAdded = bAdd_CandidateIndexes();
    UNUSED(bAdded);
    vPrintBoard("bAdd_CandidateIndexes >> "_Stz);

    vRank_CandidateIndexes();
    vPrintBoard("vRank_CandidateIndexes >> "_Stz);
    
    bool bPicked = bPick_CandidateIndex();
    vPrintBoard("bPick_CandidateIndex >> "_Stz);
    
    
    if (bPicked) {
      vMark_ComparePreds();
      vPrintBoard("vMark_ComparePreds >> "_Stz);
      
      vAdd_IndexScan();
      vPrintBoard("vAdd_IndexScan >> "_Stz);

      vCut_ComparePreds();
      vPrintBoard("vCut_ComparePreds >> "_Stz);
      
      vReduce_Filter();
      vPrintBoard("vReduce_Filter >> "_Stz);
      
      vCull_Filter();
      vPrintBoard("vCull_Filter >> "_Stz);
      
      vCut_Scan();
      vPrintBoard("vCut_Scan >> "_Stz);
      
      vCut_CandidateIndexes();
      vPrintBoard("vCut_CandidateIndexes >> "_Stz);
    }
    
    vResetRegisters();
  }
  
  Cue::vGetReturn("$$$$$$ >>"_Stz);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
