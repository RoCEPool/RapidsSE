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
  
  m_pCandidateIndex       = nullptr;
  m_pIndexScan            = nullptr;
  m_pPickedCandidateIndex = nullptr;
  m_pScan                 = nullptr;
  m_pScanTable            = nullptr;

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
    
    if (_bIsCard_Filter(pCard))
      pCard->vDump();

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
void Board::vOptimiseBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  bool bCue   = true;
  bool bPrint = true;


  vRenumberCards();
  if (bPrint)
    vPrintBoard();
  if (bCue)
    Cue::vGetReturn("vDissectSpec >> "_Stz);

  vGenerativeStreamDefs();
  if (bPrint)
    vPrintBoard();
  if (bCue)
    Cue::vGetReturn("vGenerativeStreamDefs >> "_Stz);
  
  vReplaceOrdinals();
  if (bPrint)
    vPrintBoard();
  if (bCue)
    Cue::vGetReturn("vReplaceOrdinals >> "_Stz);

  vProjectiveStreamDefs();
  if (bPrint)
    vPrintBoard();
  if (bCue)
    Cue::vGetReturn("vProjectiveStreamDefs >> "_Stz);

  
  // optional
  
  if (bFind_Scan_Filter()) {
    
    if (bPrint)
      vPrintBoard();
    if (bCue)
      Cue::vGetReturn("bFind_Scan_Filter >> "_Stz);
    
    bool b = bAddCandidateIndexes();
    UNUSED(b);
    if (bPrint)
      vPrintBoard();
    if (bCue)
      Cue::vGetReturn("bAddCandidateIndexes >> "_Stz);

    
  }
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
  
  m_pCandidateIndex = pCandidateIndex;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vIndexScan(Card_IndexScan* pIndexScan)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  m_pIndexScan = pIndexScan;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vPickedCandidateIndex(Card_CandidateIndex* pPickedCandidateIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  m_pPickedCandidateIndex = pPickedCandidateIndex;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vScan(Card_Scan* pScan)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  m_pScan = pScan;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vScanTable(Table* pScanTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  m_pScanTable = pScanTable;
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Board::bAddCandidateIndexes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  IdxVec* pIdxVec = m_pScanTable->pIdxVec();
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
void Board::vAddIndexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCutCandidateIndexes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
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
    
    m_pScan->vMark1('F');

    vScanTable(m_pScan->pTable());

    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vGenerativeStreamDefs(void)
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
void Board::vProjectiveStreamDefs(void)
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


/*----------------------------------------------------------------------------*/
/* replace stream column ordinals with stream column names                    */
/*----------------------------------------------------------------------------*/
void Board::vReplaceOrdinals(void)
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

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vPrintBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  Ribbon* pRibbon = new Ribbon(0);

  vPinCards(pRibbon,0);
  pRibbon->vPrintRibbon();

  delete pRibbon;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vRenumberCards(void)
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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
