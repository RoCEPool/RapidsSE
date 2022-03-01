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
      
  m_pOptimiser = nullptr;
  m_sQueryNm   = szQueryNm;
  
  m_pCandidateIndex     = nullptr;
  m_pComparePred_ebound = nullptr;
  m_pComparePred_lbound = nullptr;
  m_pComparePred_ubound = nullptr;
  m_pFilter             = nullptr;
  m_pIndexScan          = nullptr;
  m_pProject            = nullptr;
  m_pScan               = nullptr;
  m_pStage              = nullptr;

  m_pIndex     = nullptr;
  m_pStreamDef = nullptr;
  m_pTable     = nullptr;

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
/* set optimiser                                                              */
/*----------------------------------------------------------------------------*/
void Board::vOptimiser(Optimiser* pOptimiser)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  m_pOptimiser = pOptimiser;
}

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

//    if (pCard->bIsCard(CK_STAGE) && pSubRibbon->iChar())
//      pSubRibbon->vNextHorizontal();


    if (! nCount++)
      pSubRibbon->vFirstColumn();
    else
    if (pCard->bIsCard(CK_STAGE) && pSubRibbon->iChar()) {
      pSubRibbon->vNextHorizontal();
      //pSubRibbon->vFirstColumn();
    }
    else
      pSubRibbon->vNextColumn();
    pCard->vPrintHeader(pSubRibbon);

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
void Board::vCandidateIndex(Card_CandIndex* pCandidateIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  if (m_pCandidateIndex)
    m_pCandidateIndex->vMark2(' ');
  
  m_pCandidateIndex = pCandidateIndex;
  
  if (m_pCandidateIndex) {
    m_pCandidateIndex->vMark2('m');
    
    vIndex(m_pCandidateIndex->pFindSubCard(CK_INDEX)->pIndex());
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vComparePred_ebound(Card_PredCmp* pComparePred)
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
void Board::vComparePred_lbound(Card_PredCmp* pComparePred)
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
void Board::vComparePred_ubound(Card_PredCmp* pComparePred)
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
void Board::vStreamDef(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  m_pStreamDef = pStreamDef;
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
  vStreamDef(nullptr);
  vTable(nullptr);
}


// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vRankCandidateIndex(Card_CandIndex* pCard_CandIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  RASSERT(m_pFilter);
  
  i8 iRank = 0;
  bool bEBound = false;
  bool bLBound = false;
  bool bUBound = false;

  Index* pIndex = pCard_CandIndex->pFindSubCard(CK_INDEX)->pIndex();
  
  if (m_pFilter->bHasBounds(pIndex)) {
  
    if (m_pFilter->pFindBound(pIndex,BK_EBOUND)) {
      bEBound = true;
      pCard_CandIndex->vHasEBound(true);
    }
    
    if (m_pFilter->pFindBound(pIndex,BK_LBOUND)) {
      bLBound = true;
      pCard_CandIndex->vHasLBound(true);
    }
    
    if (m_pFilter->pFindBound(pIndex,BK_UBOUND)) {
      bUBound = true;
      pCard_CandIndex->vHasUBound(true);
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

  pCard_CandIndex->vRank(iRank);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCardResolve(Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
    
  eCK eSubCard;
  
  Card_ELIST* pCard_ELIST;
  Card_Index* pCard_Index;
  Card_Table* pCard_Table;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //pCard->vDump();
  
  i8 nSubCards = pCard->nItems();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card* pSubCard = pCard->pGetCard(i_SubCard,eSubCard);
       
    if (CK_INDEX == eSubCard) {
      vIndex(pSubCard->pIndex());
      continue;
    }

    if (CK_STREAMDEF == eSubCard) {
      vStreamDef(pSubCard->pStreamDef());
      continue;
    }

    if (CK_TABLE == eSubCard) {
      vTable(pSubCard->pTable());
      continue;
    }

    if (CK_TERM == eSubCard) {
      vTermResolve(_pCard_Term(pSubCard));
      continue;
    }
    
    vCardResolve(pSubCard);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCard_ELIST = (Card_ELIST*)pCard->pFindSubCard(CK_AGGELIST);
  if (pCard_ELIST) {
    Card_StreamDef* pCard_StreamDef = pNewStreamDef(pCard_ELIST);
    pCard->vAppend(pCard_StreamDef);
    StreamDef* pStreamDef = pCard_StreamDef->pStreamDef();
    vStreamDef(pStreamDef);
  }
  
  pCard_ELIST = (Card_ELIST*)pCard->pFindSubCard(CK_PROJELIST);
  if (pCard_ELIST) {
    Card_StreamDef* pCard_StreamDef = pNewStreamDef(pCard_ELIST);
    pCard->vAppend(pCard_StreamDef);
    StreamDef* pStreamDef = pCard_StreamDef->pStreamDef();
    vStreamDef(pStreamDef);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pCard_Index = _pCard_Index(pCard->pFindSubCard(CK_INDEX));
  if (pCard_Index) {
    Index* pIndex = pCard_Index->pIndex();
    vIndex(pIndex);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  
  pCard_Table = _pCard_Table(pCard->pFindSubCard(CK_TABLE));
  if (pCard_Table) {
    Card_StreamDef* pCard_StreamDef = pNewStreamDef(pCard_Table);
    pCard->vAppend(pCard_StreamDef);
    StreamDef* pStreamDef = pCard_StreamDef->pStreamDef();
    vStreamDef(pStreamDef);
  }

}
 
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vTermResolve(Card_Term* pCard_Term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  if (pCard_Term->bIsOrdinalStreamTerm()) {

    i8 i_Ord = pCard_Term->iOrdinalStreamColNo();
    i8 i_Col = i_Ord - 1;

    Column* pColumn = m_pStreamDef->pGetColumn(i_Col);
    Stz* szColNm = pColumn->szColNm();

    pCard_Term->vTerm(szColNm);

    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pIndex && pCard_Term->bIsOrdinalIndexTerm()) {
    
    i8 i_IdxOrd = pCard_Term->iOrdinalIndexColNo();
    i8 i_IdxCol = i_IdxOrd - 1;

    Column* pIdxColumn = m_pIndex->pGetColumn(i_IdxCol);
    Stz* szIdxColNm = pIdxColumn->szColNm();
  
    pCard_Term->vTerm(szIdxColNm);

    return;
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_StreamDef* Board::pNewStreamDef(Card_ELIST* pCard_ELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  StreamDef* pStreamDef = new StreamDef;

  eCK eSubCard;
  i8 nSubCards = pCard_ELIST->nItems();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card* pSubCard = pCard_ELIST->pGetCard(i_SubCard,eSubCard);
    
    if (CK_EXPR != eSubCard)
      continue;
    
    s32 sColNm  = "(unknown)"_Stz;
    eCT eColumn = CT_NONE;;
    i8  iLength = 0;
    i8  iScale  = 0;

    // TYPE (mandatory)
    Card_Type* pCard_Type = _pCard_Type(pSubCard->pFindSubCard(CK_TYPE));
    if (pCard_Type)
      eColumn = Column::eColumn(pCard_Type->szType());

    // AS (optional)
    Card_As* pCard_As = _pCard_As(pSubCard->pFindSubCard(CK_AS));
    if (pCard_As)
      sColNm = pCard_As->szAs();
    else {
      Card_ExprCalc* pCard_ExprCalc
        = _pCard_ExprCalc(pSubCard->pFindSubCard(CK_EXPRCALC));

      if (pCard_ExprCalc) {
        Card_Term* pCard_Term = _pCard_Term(pCard_ExprCalc->pGet(0));
        sColNm = pCard_Term->szTerm();
      }
    }
  
    //vTrace("pNewStreamDef: %s %s\n"_Stz,sColNm.szStz(),aCT[eColumn]);
    pStreamDef->vAddColumn(sColNm,eColumn,iLength,iScale);
  }
  
  Card_StreamDef* pCard_StreamDef = new Card_StreamDef(pStreamDef);
  
  return pCard_StreamDef;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_StreamDef* Board::pNewStreamDef(Card_Table* pCard_Table)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  Table* pTable = pCard_Table->pTable();
    
  StreamDef* pStreamDef = new StreamDef(pTable->pStreamDef());
  
  Card_StreamDef* pCard_StreamDef = new Card_StreamDef(pStreamDef);

  return pCard_StreamDef;
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Board::bAdd_CandIndexes(void)
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

  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    Index* pIndex = pIdxVec->pGet(i_Index);
    //vTrace("Index %s\n"_Stz,pIndex->szIndexNm());
    
    Card_CandIndex* pCard_CandIndex = new Card_CandIndex;
    Card_Index* pCard_Index = new Card_Index(pIndex);
    pCard_CandIndex->vAppend(pCard_Index);
    
    vInsertAfter(pCard_CandIndex,pCard_Filter);
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

  Card_IndexScan* pCard_IndexScan = new Card_IndexScan;

  Card_Index* pCard_Index = new Card_Index(m_pIndex);
  pCard_IndexScan->vAppend(pCard_Index);

  
  Card_BOUND* pCard_BOUND;
  Card_PRED*  pCard_PRED;

  if (m_pComparePred_ebound) {
    pCard_BOUND = new Card_EBound;
    
    pCard_PRED = m_pComparePred_ebound->pDup();
    pCard_PRED->vHasEBound(true);
    pCard_BOUND->vAppend(pCard_PRED);
  }
  else
  if (m_pComparePred_lbound && m_pComparePred_ubound) {
    pCard_BOUND = new Card_RBound;
    
    pCard_PRED = m_pComparePred_lbound->pDup();
    pCard_PRED->vHasLBound(true);
    pCard_BOUND->vAppend(pCard_PRED);
    
    pCard_PRED = m_pComparePred_ubound->pDup();
    pCard_PRED->vHasUBound(true);
    pCard_BOUND->vAppend(pCard_PRED);
  }
  else
  if (m_pComparePred_lbound) {
    pCard_BOUND = new Card_LBound;
    
    pCard_PRED = m_pComparePred_lbound->pDup();
    pCard_PRED->vHasLBound(true);
    pCard_BOUND->vAppend(pCard_PRED);
  }
  else
  if (m_pComparePred_ubound) {
    pCard_BOUND = new Card_UBound;
    
    pCard_PRED = m_pComparePred_ubound->pDup();
    pCard_PRED->vHasUBound(true);
    pCard_BOUND->vAppend(m_pComparePred_ubound->pDup());
  }
  else {
    RASSERT(0);
  }
  
  pCard_IndexScan->vAppend(pCard_BOUND);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  StreamDef* pStreamDef_scan = m_pScan->pFindSubCard(CK_STREAMDEF)->pStreamDef();

  StreamDef* pStreamDef = pStreamDef_scan->pDup();
 
  Card_StreamDef* pCard_StreamDef = new Card_StreamDef(pStreamDef);
  
  pCard_IndexScan->vAppend(pCard_StreamDef);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the stream order
  
  IndexDef* pIndexDef = m_pIndex->pIndexDef();
  i8 nIdxCols = pIndexDef->nColumns();
  
  for (i8 i_IdxCol = 0; i_IdxCol < nIdxCols; i_IdxCol++) {

    Column* pIdxCol = pIndexDef->pGetColumn(i_IdxCol);
    Stz* szIdxColNm = pIdxCol->szColNm();

    Column* pColumn = pStreamDef->pFindColumn(szIdxColNm);
    i8 i_Col = pColumn->iColumnNo();
    pStreamDef->vAddOrder(i_Col);
  }
  
  bool bUnique = pIndexDef->bIsUnique();
  pStreamDef->vUnique(bUnique);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


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
//  Card_PredCmp* pCard_PredCmp =
//    _pCard_PredCmp(m_pFilter->pFindSubCard(CK_PREDCMP));
//
//  pCard_PredCmp->vDump();
//
//  Card_LBound* pCard_LBound = new Card_LBound;
//
//
//  i8 nTerms = pCard_PredCmp->nItems();
//  for (i8 i_Term = 0; i_Term < nTerms; i_Term++) {
//    Card_Term* pCard_Term =
//      _pCard_Term(pCard_PredCmp->pGet(i_Term));
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
  
  vRemoveCards(CK_CANDINDEX);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCut_ComparePreds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  if (m_pComparePred_ebound)
    m_pFilter->vRemoveSubCard(m_pComparePred_ebound);
  
  if (m_pComparePred_lbound)
    m_pFilter->vRemoveSubCard(m_pComparePred_lbound);
  
  if (m_pComparePred_ubound)
    m_pFilter->vRemoveSubCard(m_pComparePred_ubound);
  
  
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
  
  vScan(nullptr);

  delete m_pScan;  
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Board::bFind_Scan_AggGby(bool bMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Board::bFind_Scan_Filter(bool bMark)
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
    
    if (bMark)
      vScan(_pCard_Scan(pTestScan));

    Card_Table* pCard_Table = _pCard_Table(pTestScan->pFindSubCard(CK_TABLE));
    vTable(pCard_Table->pTable());
    
    if (bMark)
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
      Card_PredCmp* pComparePred_lbound =
        m_pFilter->pFindBound(m_pIndex,BK_LBOUND);
      if (pComparePred_lbound) {
        vComparePred_lbound(pComparePred_lbound);
        break;
      }
      Card_PredCmp* pComparePred_ubound =
        m_pFilter->pFindBound(m_pIndex,BK_UBOUND);
      if (pComparePred_ubound) {
        vComparePred_ubound(pComparePred_ubound);
        break;
      }
      break;
    }

    case 2: {
      // lower bound and upper bound
      Card_PredCmp* pComparePred_lbound =
        m_pFilter->pFindBound(m_pIndex,BK_LBOUND);
      if (pComparePred_lbound)
        vComparePred_lbound(pComparePred_lbound);
      Card_PredCmp* pComparePred_ubound =
        m_pFilter->pFindBound(m_pIndex,BK_UBOUND);
      if (pComparePred_ubound)
        vComparePred_ubound(pComparePred_ubound);
      break;
    }
      
    case 3: {
      // equi-bound
      Card_PredCmp* pComparePred_ebound =
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
/* order the projective stream defs which are downstream of indexScan         */
/*----------------------------------------------------------------------------*/
void Board::vOrder_ProjectiveStreamDefs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  StreamDef* pStreamDef_scan =
    m_pIndexScan->pFindSubCard(CK_STREAMDEF)->pStreamDef();

  i8 nCards = nItems();
  
  for (i8 i_Card = 0; i_Card < nCards; i_Card++) {
    Card* pCard = pGet(i_Card);
    
    if (CK_PROJECT != pCard->eCard())
      continue;
    
    Card_Project* pCard_Project = _pCard_Project(pCard);
    
    Card_StreamDef* pCard_StreamDef =
      _pCard_StreamDef(pCard_Project->pFindSubCard(CK_STREAMDEF));

    pCard_StreamDef->vApplyOrder(pStreamDef_scan);
  }
  
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Board::bPick_CandidateIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  i8 iBestRank = -1;
  Card_CandIndex* pCard_CandIndex = nullptr;
  
  i8 nCards = nItems();
  for (i8 i_Card = 0; i_Card < nCards; i_Card++) {
    Card* pCard = pGet(i_Card);
    
    if (CK_CANDINDEX != pCard->eCard())
      continue;

    Card_CandIndex* pCadIdx = _pCard_CandIndex(pCard);

    i8 iRank = pCadIdx->iRank();
    if (iRank > iBestRank) {
      iBestRank = iRank;
      pCard_CandIndex = pCadIdx;
    }
  }

  if (! pCard_CandIndex) {
    vCandidateIndex(nullptr);
    return false;
  }
  
  vCandidateIndex(pCard_CandIndex);

  return true;
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
    
    if (CK_CANDINDEX != pCard->eCard())
      continue;

    Card_CandIndex* pCard_CandIndex = _pCard_CandIndex(pCard);
    
    vRankCandidateIndex(pCard_CandIndex);
    
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vResolve_Symbols(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  i8 nMainCards = nItems();
  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
    Card* pMainCard = pGetCard(i_Main);
    
    vCardResolve(pMainCard);
  }
  
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* may 00: AggGby + (Sort) => AggGby                                          */
/*----------------------------------------------------------------------------*/
bool Board::bMay_AggGby_Sort_to_AggGby(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  bool bMay = false;
  
  return bMay;
}

/*----------------------------------------------------------------------------*/
/* may 00: ordered stream + (AggGby) => OrdAgg ?                              */
/*----------------------------------------------------------------------------*/
bool Board::bMay_ordStream_AggGby_to_OrdAgg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  bool bMay = false;
  
  return bMay;
}

/*----------------------------------------------------------------------------*/
/* may 00: OrdAgg + (Sort) => OrdAgg ?                                        */
/*----------------------------------------------------------------------------*/
bool Board::bMay_OrdAgg_Sort_to_OrdAgg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  bool bMay = false;
  
  return bMay;
}

/*----------------------------------------------------------------------------*/
/* may 00: (Scan) + (AggGby) => IndexScan ?                                   */
/*----------------------------------------------------------------------------*/
bool Board::bMay_Scan_AggGby_to_IndexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  bool bMay = false;
  
  return bMay;
}

/*----------------------------------------------------------------------------*/
/* may 00: (Scan) + (Filter) => IndexScan ?                                   */
/*----------------------------------------------------------------------------*/
bool Board::bMay_Scan_Filter_to_IndexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  bool bMay = bFind_Scan_Filter(false);
  
  return bMay;
}

/*----------------------------------------------------------------------------*/
/* may 00: (Scan) + (Sort) => IndexScan ?                                     */
/*----------------------------------------------------------------------------*/
bool Board::bMay_Scan_Sort_to_IndexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  bool bMay = false;
  
  return bMay;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* rule 00: OrdAgg + (sort) => OrdAgg                                         */
/*----------------------------------------------------------------------------*/
bool Board::bRule_AggGby_Sort_to_AggGby(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  bool bMay = false;
  
  return bMay;
}

/*----------------------------------------------------------------------------*/
/* rule 00: ordered stream + (AggGby) => OrdAgg                               */
/*----------------------------------------------------------------------------*/
bool Board::bRule_ordStream_AggGby_to_OrdAgg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* rule 00: OrdAgg + (Sort) => OrdAgg                                         */
/*----------------------------------------------------------------------------*/
bool Board::bRule_OrdAgg_Sort_to_OrdAgg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  bool bMay = false;
  
  return bMay;
}

/*----------------------------------------------------------------------------*/
/* rule: (Scan) + (AggGby) => IndexScan                                       */
/*----------------------------------------------------------------------------*/
bool Board::bRule_Scan_AggGby_to_IndexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* may: (Scan) + (Filter) => IndexScan                                        */
/*----------------------------------------------------------------------------*/
bool Board::bRule_Scan_Filter_to_IndexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  bool bMay = bFind_Scan_Filter(true);
  RASSERT(bMay);
  
  vPrintBoard("bFind_Scan_Filter >> "_Stz);
  
  bool bAdded = bAdd_CandIndexes();
  UNUSED(bAdded);
  vPrintBoard("bAdd_CandidateIndexes >> "_Stz);

  vRank_CandidateIndexes();
  vPrintBoard("vRank_CandidateIndexes >> "_Stz);
  
  bool bPicked = bPick_CandidateIndex();
  vPrintBoard("bPick_CandidateIndex >> "_Stz);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (! bPicked) {
    
    vCut_CandidateIndexes();
    vPrintBoard("vCut_CandidateIndexes >> "_Stz);
    
    return false;
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
  
  vOrder_ProjectiveStreamDefs();
  vPrintBoard("vOrder_ProjectiveStreamDefs >> "_Stz);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* rule: (Scan) + (Sort) => IndexScan                                         */
/*----------------------------------------------------------------------------*/
bool Board::bRule_Scan_Sort_to_IndexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  bool bMay = false;
  
  return bMay;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
