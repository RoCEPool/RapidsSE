/*============================================================================*/
/* Board.cpp -- Board                                                         */
/*============================================================================*/

#include "Board.hpp"

#include "ShardDef.hpp"
#include "Cue.hpp"

#include "Optimiser.hpp"
#include "OptimiserFun.hpp"
#include "OptimiserCatalog.hpp"

extern OptimiserCatalog gxOptimiserCatalog;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Board,CN_CARD)
__aNewFunction(Board,CN_CARD)
__vDeleteFunction(Board,CN_CARD)
__aFooterFunction(Board)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Board::Board(Stz* szQueryNm, Optimiser* pOptimiser)
: Card_OTHER(CK_BOARD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
      
  m_sQueryNm   = szQueryNm;
  m_pOptimiser = pOptimiser;
   
  m_pCardRegs  = new CardRegs;
  m_pRuleCtl   = new RuleCtl;
  
  //_checkInitialised_(CN_CARD);
  _checkInitialisedSize_(sizeof(Board));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Board::~Board(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  delete m_pCardRegs;
  delete m_pRuleCtl;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get card registers                                                         */
/*----------------------------------------------------------------------------*/
CardRegs* Board::pCardRegs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  return m_pCardRegs;
}

/*----------------------------------------------------------------------------*/
/* clear the board                                                            */
/*----------------------------------------------------------------------------*/
void Board::vClear(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  m_pCardRegs->vResetCardRegs();
  
  vDeleteAllCards();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDeleteAllCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  while (Card* pCard = pPop())
    delete pCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Board::pGetMainCard(i8 i_MainCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card* pMainCard = pGet(i_MainCard);
  return pMainCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Board::pGetMainCard(i8 i_MainCard, eCK& reCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card* pMainCard = pGet(i_MainCard);
  
  if (pMainCard)
    reCard = pMainCard->eCard();
  else
    reCard = CK_NONE;

  return pMainCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Board::pFindMain(Stz* szCardNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  eCK eCard = Card::eToECard(szCardNm);
  
  i8 nMainCards = this->nMainCards();
  for (i8 i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
    Card* pMainCard = pGetMainCard(i_MainCard);
    
    if (pMainCard->eCard() == eCard)
      return pMainCard;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get number of cards                                                        */
/*----------------------------------------------------------------------------*/
i8 Board::nMainCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 nCards = this->nItems();
  
  return nCards;
}

/*----------------------------------------------------------------------------*/
/* pin board onto given ribbon                                                */
/*----------------------------------------------------------------------------*/
void Board::vPinCards(Ribbon* pRibbon, i8 i_Board)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! nMainCards())
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pRibbon->vSprint("[%lld] %s"_Stz,i_Board,szQueryNm());
  pRibbon->vTitleLines(1);
  pRibbon->vSetBase();
  pRibbon->vFirstColumn();

  Ribbon* pSubRibbon = new Ribbon(m_pOptimiser->pCardCtl(),this,0);
  

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

    pCard->vPrintCard(pSubRibbon,0,0);
  }
     
  pRibbon->vPinRibbon(1,0,pSubRibbon);
  
  delete pSubRibbon;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vPrintBoard(Stz* szCue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  eOA eOptAct = OA_ENTRY;
  TokenVec* pTknVec = nullptr;
  
  L_Entry:
  
  bool bCue   = true;
  bool bPrint = true;
  bool bExecuted = false;

  vRenumberCards();
  
  if (! bPrint)
    return;
  
  Ribbon* pRibbon = new Ribbon(m_pOptimiser->pCardCtl(),this,0);

  vPinCards(pRibbon,0);
  pRibbon->vPrintRibbon();

  delete pRibbon;
  
  if (! bCue)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  L_Input:
  
  s4K sInput;
  Cue::vGetInput(szCue,sInput);
    
  if (! sInput.nLength()) {
    return;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pTknVec) {
    delete pTknVec;
    pTknVec = nullptr;
  }
  
  pTknVec = TokenVec::pTokeniseSen(sInput);
  
  i8 nTokens = pTknVec->nTokens();
  
  OptimiserFun* pOptFun = nullptr;
  
  if (0 == nTokens)
    goto L_Exit;
  
  if (1 == nTokens)
    pOptFun = gxOptimiserCatalog.pFindFun(pTknVec->pGet(0)->szChars(),
                                          nullptr);
  else
    pOptFun = gxOptimiserCatalog.pFindFun(pTknVec->pGet(0)->szChars(),
                                          pTknVec->pGet(1)->szChars());

  if (! pOptFun) {
    vTrace("Unknown command\n"_Stz);
    delete pTknVec;
    goto L_Input;
  }
    
  eOptAct = OA_ENTRY;
  bExecuted = pOptFun->m_fpOptFun(m_pOptimiser,pTknVec,eOptAct);

  L_Next:
  
  if (! bExecuted)
    goto L_Input;
  
  if (OA_ENTRY == eOptAct)
    goto L_Entry;
  else
  if (OA_INPUT == eOptAct)
    goto L_Input;
  else
  if (OA_EXIT == eOptAct)
    return;
  else
    NEVER_REACH;
  
L_Exit:
  if (pTknVec) {
    delete pTknVec;
    pTknVec = nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* get query name                                                             */
/*----------------------------------------------------------------------------*/
Stz* Board::szQueryNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  return m_sQueryNm;
}

/*----------------------------------------------------------------------------*/
/* set query name                                                             */
/*----------------------------------------------------------------------------*/
void Board::vQueryNm(Stz* szQueryNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  m_sQueryNm = szQueryNm;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Board::pSearch(Stz* szCardNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card* pCard = nullptr;
  if (szCardNm->bIsInteger()) {
    i8 i_MainCard = szCardNm->iToI8();
    pCard = pGetMainCard(i_MainCard);
    
    return pCard;
  }

  pCard = pFindMain(szCardNm);
  if (pCard)
    return pCard;
  
  eCK eCard = Card::eToECard(szCardNm);
  pCard = m_pCardRegs->pSearchReg(eCard);

  return pCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vSearch(Stz* szCardNm, Card*& rpMainCard, Card*& rpSubCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  if (szCardNm->bIsInteger()) {
    i8 i_MainCard = szCardNm->iToI8();
    rpMainCard = pGetMainCard(i_MainCard);
    
    return;
  }

  rpMainCard = pFindMain(szCardNm);
  if (rpMainCard)
    return;
  
  eCK eCard = Card::eToECard(szCardNm);
  rpSubCard = m_pCardRegs->pSearchReg(eCard);
}



// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vRankCandIndex(Card_CandIndex* pCard_CandIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Filter* pReg_Filter = _pCard_Filter(m_pCardRegs->pCardReg(CK_FILTER));
  
  RASSERT(pReg_Filter);
  
  i8 iRank = 0;
  bool bEBound = false;
  bool bLBound = false;
  bool bUBound = false;

  Index* pIndex = pCard_CandIndex->pFindSubCard(CK_INDEX)->pIndex();
  
  if (pReg_Filter->bHasBounds(pIndex)) {
  
    if (pReg_Filter->pFindBound(pIndex,BK_EBOUND)) {
      bEBound = true;
      pCard_CandIndex->vHasEBound(true);
    }
    
    if (pReg_Filter->pFindBound(pIndex,BK_LBOUND)) {
      bLBound = true;
      pCard_CandIndex->vHasLBound(true);
    }
    
    if (pReg_Filter->pFindBound(pIndex,BK_UBOUND)) {
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
void Board::vResolveAll(Card* pAnyCard) // recursive
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
    
  
  Card_ELIST* pCard_ELIST;
  Card_Index* pCard_Index;
  Card_Table* pCard_Table;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //pCard->vDump();
  
  eCK eSubCard;
  i8 nSubCards = pAnyCard->nSubCards();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card* pSubCard = pAnyCard->pGetSubCard(i_SubCard,eSubCard);
       
    if (CK_INDEX == eSubCard) {
      vIndex(pSubCard->pIndex());
      continue;
    }

    if (CK_STREAM == eSubCard) {
      //vReg_Stream(_pCard_Stream(pSubCard));
      m_pCardRegs->vCardReg(CK_STREAM,pSubCard);
      continue;
    }

    if (CK_TABLE == eSubCard) {
      vTable(pSubCard->pTable());
      continue;
    }

    if (CK_TERM == eSubCard) {
      vResolveTerm(_pCard_Term(pSubCard));
      continue;
    }
    
    vResolveAll(pSubCard);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCard_ELIST = (Card_ELIST*)pAnyCard->pFindSubCard(CK_AGGELIST);
  if (pCard_ELIST) {
        
    Card_Stream* pCard_Stream = new Card_Stream;
    pAnyCard->vAppend(pCard_Stream);
    
    Card_SubSet* pCard_SubSet = pCard_ELIST->pDupSubCards();
    Card_StreamElist* pCard_StreamElist = new Card_StreamElist;
    pCard_StreamElist->vTransferAll(pCard_SubSet);
    pCard_Stream->vAppend(pCard_StreamElist);
    delete pCard_SubSet;
    
    //vReg_Stream(pCard_Stream);
    m_pCardRegs->vCardReg(CK_STREAM,pCard_Stream);
  }
  
  pCard_ELIST = (Card_ELIST*)pAnyCard->pFindSubCard(CK_PROJELIST);
  if (pCard_ELIST) {
        
    Card_Stream* pCard_Stream = new Card_Stream;
    pAnyCard->vAppend(pCard_Stream);
    
    Card_SubSet* pCard_SubSet = pCard_ELIST->pDupSubCards();
    Card_StreamElist* pCard_StreamElist = new Card_StreamElist;
    pCard_StreamElist->vTransferAll(pCard_SubSet);
    pCard_Stream->vAppend(pCard_StreamElist);
    delete pCard_SubSet;
    
    //vReg_Stream(pCard_Stream);
    m_pCardRegs->vCardReg(CK_STREAM,pCard_Stream);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pCard_Index = _pCard_Index(pAnyCard->pFindSubCard(CK_INDEX));
  if (pCard_Index) {
    Index* pIndex = pCard_Index->pIndex();
    vIndex(pIndex);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  
  pCard_Table = _pCard_Table(pAnyCard->pFindSubCard(CK_TABLE));
  if (pCard_Table) {

    StreamDef* pStreamDef = pCard_Table->pTable()->pStreamDef();

    
    Card_Stream* pCard_Stream = new Card_Stream;
    pAnyCard->vAppend(pCard_Stream);
    //vReg_Stream(pCard_Stream);
    m_pCardRegs->vCardReg(CK_STREAM,pCard_Stream);

    Card_StreamElist* pCard_StreamElist = new Card_StreamElist(pStreamDef);
    pCard_Stream->vAppend(pCard_StreamElist);
  }

}
 
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vResolveTerm(Card_Term* pCard_Term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  
  Card_Stream* pReg_Stream = _pCard_Stream(m_pCardRegs->pCardReg(CK_STREAM));

  if (pCard_Term->bIsOrdinalStreamTerm()) {

    i8 i_Ord = pCard_Term->iOrdinalStreamColNo();
    i8 i_Col = i_Ord - 1;

    Card_StreamElist* pCard_StreamElist_src =
      _pCard_StreamElist(pReg_Stream->pFindSubCard(CK_STREAMELIST));

    Card_Expr* pCard_Expr_src = _pCard_Expr(pCard_StreamElist_src->pGet(i_Col));

    Card_Term* pCard_Term_src =
      _pCard_Term(pCard_Expr_src->pFindSubCard(CK_TERM));

    Stz* szColNm_resolved = pCard_Term_src->szTerm();
    
    pCard_Term->vTerm(szColNm_resolved);

    return;
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Index* pIndex = this->pIndex();
  if (pIndex && pCard_Term->bIsOrdinalIndexTerm()) {
    
    i8 i_IdxOrd = pCard_Term->iOrdinalIndexColNo();
    i8 i_IdxCol = i_IdxOrd - 1;

    Column* pIdxColumn = pIndex->pGetColumn(i_IdxCol);
    Stz* szIdxColNm_resolved = pIdxColumn->szColNm();
  
    pCard_Term->vTerm(szIdxColNm_resolved);

    return;
  }

}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Board::bAdd_CandIndexes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
 
  Card_Scan* pReg_Scan = _pCard_Scan(m_pCardRegs->pCardReg(CK_SCAN));

  Table* pTable = this->pTable();

  IdxVec* pIdxVec = pTable->pIdxVec();
  if (! pIdxVec) {
    vTrace("No IdxVec\n"_Stz);
    return false;
  }

  i8 nIndexes = pIdxVec->nItems();
  if (! nIndexes) {
    vTrace("No nIndexes\n"_Stz);
    return false;
  }
  
  i8 i_Filter = i_Find(pReg_Scan) + 1;
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  //m_pFilter->vDump();

  Card_IndexScan* pCard_IndexScan = new Card_IndexScan;
  Card_Scan* pReg_Scan = _pCard_Scan(m_pCardRegs->pCardReg(CK_SCAN));

  Card_PredCmp* pReg_ebound = m_pCardRegs->pPredCmp_ebound();
  Card_PredCmp* pReg_lbound = m_pCardRegs->pPredCmp_lbound();
  Card_PredCmp* pReg_ubound = m_pCardRegs->pPredCmp_ubound();
  
  Index* pIndex = this->pIndex();
  Card_Index* pCard_Index = new Card_Index(pIndex);
  pCard_IndexScan->vAppend(pCard_Index);

  
  Card_BOUND* pCard_BOUND;
  Card_PRED*  pCard_PRED;

  if (pReg_ebound) {
    pCard_BOUND = new Card_EBound;
    
    pCard_PRED = pReg_ebound->pDup();
    pCard_PRED->vHasEBound(true);
    pCard_BOUND->vAppend(pCard_PRED);
  }
  else
  if (pReg_lbound && pReg_ubound) {
    pCard_BOUND = new Card_RBound;
    
    pCard_PRED = pReg_lbound->pDup();
    pCard_PRED->vHasLBound(true);
    pCard_BOUND->vAppend(pCard_PRED);
    
    pCard_PRED = pReg_ubound->pDup();
    pCard_PRED->vHasUBound(true);
    pCard_BOUND->vAppend(pCard_PRED);
  }
  else
  if (pReg_lbound) {
    pCard_BOUND = new Card_LBound;
    
    pCard_PRED = pReg_lbound->pDup();
    pCard_PRED->vHasLBound(true);
    pCard_BOUND->vAppend(pCard_PRED);
  }
  else
  if (pReg_ubound) {
    pCard_BOUND = new Card_UBound;
    
    pCard_PRED = pReg_ubound->pDup();
    pCard_PRED->vHasUBound(true);
    pCard_BOUND->vAppend(pReg_ubound->pDup());
  }
  else {
    RASSERT(0);
  }
  
  pCard_IndexScan->vAppend(pCard_BOUND);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the stream elist

  StreamDef* pStreamDef = pIndex->pTable()->pStreamDef();
  
  Card_Stream* pCard_Stream = new Card_Stream;
  pCard_IndexScan->vAppend(pCard_Stream);

  Card_StreamElist* pCard_StreamElist = new Card_StreamElist(pStreamDef);
  
  
  pCard_Stream->vAppend(pCard_StreamElist);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the stream order
  
  Card_ObyElist* pCard_ObyElist = new Card_ObyElist(pIndex);
  pCard_Stream->vAppend(pCard_ObyElist);

  //bool bUnique = pIndexDef->bIsUnique();
  //pStreamDef->vUnique(bUnique);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  vInsertBefore(pCard_IndexScan,pReg_Scan);
  
  m_pCardRegs->vCardReg(CK_INDEXSCAN,pCard_IndexScan);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCull_Filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Filter* pReg_Filter = _pCard_Filter(m_pCardRegs->pCardReg(CK_FILTER));

  if (! pReg_Filter->bCull())
    return;
  
  vRemove(pReg_Filter);
  
  delete pReg_Filter;

  //vReg_Filter(nullptr);
  m_pCardRegs->vCardReg(CK_FILTER,nullptr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCut_CandidateIndexes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vRemoveSubCards(CK_CANDINDEX);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCut_PredCmps(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Filter* pReg_Filter = _pCard_Filter(m_pCardRegs->pCardReg(CK_FILTER));

  Card_PredCmp* pReg_ebound = m_pCardRegs->pPredCmp_ebound();
  Card_PredCmp* pReg_lbound = m_pCardRegs->pPredCmp_lbound();
  Card_PredCmp* pReg_ubound = m_pCardRegs->pPredCmp_ubound();

  if (pReg_ebound)
    pReg_Filter->vRemoveSubCard(pReg_ebound);
  
  if (pReg_lbound)
    pReg_Filter->vRemoveSubCard(pReg_lbound);
  
  if (pReg_ubound)
    pReg_Filter->vRemoveSubCard(pReg_ubound);
  
  m_pCardRegs->vPredCmp_ebound(nullptr);
  m_pCardRegs->vPredCmp_lbound(nullptr);
  m_pCardRegs->vPredCmp_ubound(nullptr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCut_Scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Scan* pReg_Scan = _pCard_Scan(m_pCardRegs->pCardReg(CK_SCAN));


  vRemove(pReg_Scan);
  
  m_pCardRegs->vCardReg(CK_SCAN,nullptr);
  delete pReg_Scan;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Board::bFind_Scan_AggGby(bool bMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Board::bFind_Scan_Filter(bool bMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  m_pCardRegs->vCardReg(CK_SCAN,nullptr);

  i8 nMainCards = this->nMainCards();
  for (i8 i_MainCard = 0; i_MainCard < nMainCards-1; i_MainCard++) {
    Card* pTestScan = pGetMainCard(i_MainCard);
    if (CK_SCAN != pTestScan->eCard())
      continue;

    Card* pTestFilter = pGetMainCard(i_MainCard+1);
    if (CK_FILTER != pTestFilter->eCard())
      continue;
    
    if (bMark)
      //vReg_Scan(_pCard_Scan(pTestScan));
      m_pCardRegs->vCardReg(CK_SCAN,pTestScan);
    Card_Table* pCard_Table = _pCard_Table(pTestScan->pFindSubCard(CK_TABLE));
    vTable(pCard_Table->pTable());
    
    if (bMark)
      //vReg_Filter(_pCard_Filter(pTestFilter));
      m_pCardRegs->vCardReg(CK_FILTER,pTestFilter);

    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vMark_PredCmps(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Filter* pReg_Filter = _pCard_Filter(m_pCardRegs->pCardReg(CK_FILTER));

  Card_CandIndex* pCard_CandIndex =
    _pCard_CandIndex(m_pCardRegs->pCardReg(CK_CANDINDEX));

  i8 iRank = pCard_CandIndex->iRank();
 
  Index* pIndex = this->pIndex();

  switch (iRank) {
      
    case 1: {
      // lower bound or upper bound
      Card_PredCmp* pPredCmp_lbound =
        pReg_Filter->pFindBound(pIndex,BK_LBOUND);
      if (pPredCmp_lbound) {
        m_pCardRegs->vPredCmp_lbound(pPredCmp_lbound);
        pPredCmp_lbound->vMark2('L');
        break;
      }
      Card_PredCmp* pPredCmp_ubound =
        pReg_Filter->pFindBound(pIndex,BK_UBOUND);
      if (pPredCmp_ubound) {
        m_pCardRegs->vPredCmp_ubound(pPredCmp_ubound);
        pPredCmp_ubound->vMark2('U');
        break;
      }
      break;
    }

    case 2: {
      // lower bound and upper bound
      Card_PredCmp* pPredCmp_lbound =
        pReg_Filter->pFindBound(pIndex,BK_LBOUND);
      if (pPredCmp_lbound) {
        m_pCardRegs->vPredCmp_lbound(pPredCmp_lbound);
        pPredCmp_lbound->vMark2('L');
      }
      Card_PredCmp* pPredCmp_ubound =
        pReg_Filter->pFindBound(pIndex,BK_UBOUND);
      if (pPredCmp_ubound) {
        m_pCardRegs->vPredCmp_ubound(pPredCmp_ubound);
        pPredCmp_ubound->vMark2('U');
      }
      break;
    }
      
    case 3: {
      // equi-bound
      Card_PredCmp* pPredCmp_ebound =
        pReg_Filter->pFindBound(pIndex,BK_EBOUND);
      if (pPredCmp_ebound) {
        m_pCardRegs->vPredCmp_ebound(pPredCmp_ebound);
        pPredCmp_ebound->vMark2('E');
      }
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_IndexScan* pReg_IndexScan
    = _pCard_IndexScan(m_pCardRegs->pCardReg(CK_INDEXSCAN));

  Card_Stream* pCard_Stream_src =
    _pCard_Stream(pReg_IndexScan->pFindSubCard(CK_STREAM));

  i8 nMainCards = this->nMainCards();
  for (i8 i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
    Card* pMainCard = pGetMainCard(i_MainCard);
    if (CK_PROJECT != pMainCard->eCard())
      continue;
    
    Card_Project* pCard_Project = _pCard_Project(pMainCard);

    //--- --- ---

    if (! pCard_Stream_src)
      continue;
    
    Card_Stream* pCard_Stream_dst =
      _pCard_Stream(pCard_Project->pFindSubCard(CK_STREAM));
    if (! pCard_Stream_dst)
      continue;

    pCard_Stream_dst->vApplyOrder(pCard_Stream_src);
    
    //--- --- ---
    
  }
  
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Board::bPick_CandidateIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 iBestRank = C_UNKNOWN;
  Card_CandIndex* pCard_CandIndex = nullptr;
  
  i8 nMainCards = this->nMainCards();
  for (i8 i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
    Card* pMainCard = pGetMainCard(i_MainCard);
    if (CK_CANDINDEX != pMainCard->eCard())
      continue;

    Card_CandIndex* pCadIdx = _pCard_CandIndex(pMainCard);

    i8 iRank = pCadIdx->iRank();
    if (iRank > iBestRank) {
      iBestRank = iRank;
      pCard_CandIndex = pCadIdx;
    }
  }

  if (! pCard_CandIndex) {
    //vReg_CandIndex(nullptr);
    m_pCardRegs->vCardReg(CK_CANDINDEX,nullptr);
    return false;
  }
  
  //vReg_CandIndex(pCard_CandIndex);
  m_pCardRegs->vCardReg(CK_CANDINDEX,pCard_CandIndex);
  vIndex(pCard_CandIndex->pFindSubCard(CK_INDEX)->pIndex());

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vRank_CandidateIndexes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //m_pFilter->vMark2('0');
                    
  i8 nMainCards = this->nMainCards();
  for (i8 i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
    Card* pMainCard = pGetMainCard(i_MainCard);
    if (CK_CANDINDEX != pMainCard->eCard())
      continue;

    Card_CandIndex* pCard_CandIndex = _pCard_CandIndex(pMainCard);
    
    vRankCandIndex(pCard_CandIndex);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReduce_Filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Filter* pReg_Filter = _pCard_Filter(m_pCardRegs->pCardReg(CK_FILTER));

  if (! pReg_Filter)
    return;
  
  pReg_Filter->vReduceFilter();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vResolve_Symbols(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 nMainCards = this->nMainCards();
  for (i8 i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
    Card* pMainCard = pGetMainCard(i_MainCard);
    
    vResolveAll(pMainCard);
  }
  
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* may 00: AggGby + (Sort) => AggGby                                          */
/*----------------------------------------------------------------------------*/
bool Board::bMay_AggGby_Sort_to_AggGby(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

#ifdef RULE_NO
  #undef RULE_NO
#endif
  
#define RULE_NO RN_ZERO
  
  bool bMay = false;
  
  m_pRuleCtl->vFlag(RULE_NO,bMay);
  return bMay;
}

/*----------------------------------------------------------------------------*/
/* may 00: ordered stream + (AggGby) => OrdAgg ?                              */
/*----------------------------------------------------------------------------*/
bool Board::bMay_ordStream_AggGby_to_OrdAgg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

#ifdef RULE_NO
  #undef RULE_NO
#endif
  
#define RULE_NO 1
  
  bool bMay = false;
  
  m_pRuleCtl->vFlag(RULE_NO,bMay);
  return bMay;
}

/*----------------------------------------------------------------------------*/
/* may 00: OrdAgg + (Sort) => OrdAgg ?                                        */
/*----------------------------------------------------------------------------*/
bool Board::bMay_OrdAgg_Sort_to_OrdAgg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
#ifdef RULE_NO
  #undef RULE_NO
#endif
  
#define RULE_NO 2

  bool bMay = false;
  
  m_pRuleCtl->vFlag(RULE_NO,bMay);
  return bMay;
}

/*----------------------------------------------------------------------------*/
/* may 00: (Scan) + (AggGby) => IndexScan ?                                   */
/*----------------------------------------------------------------------------*/
bool Board::bMay_Scan_AggGby_to_IndexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
#ifdef RULE_NO
  #undef RULE_NO
#endif
  
#define RULE_NO 3

  bool bMay = false;
  
  m_pRuleCtl->vFlag(RULE_NO,bMay);
  return bMay;
}

/*----------------------------------------------------------------------------*/
/* may 00: (Scan) + (Filter) => IndexScan ?                                   */
/*----------------------------------------------------------------------------*/
bool Board::bMay_Scan_Filter_to_IndexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
#ifdef RULE_NO
  #undef RULE_NO
#endif
  
#define RULE_NO 4

  bool bMay = bFind_Scan_Filter(false);
  
  m_pRuleCtl->vFlag(RULE_NO,bMay);
  return bMay;
}

/*----------------------------------------------------------------------------*/
/* may 00: (Scan) + (Sort) => IndexScan ?                                     */
/*----------------------------------------------------------------------------*/
bool Board::bMay_Scan_Sort_to_IndexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

#ifdef RULE_NO
  #undef RULE_NO
#endif
  
#define RULE_NO 5

  bool bMay = false;
  
  m_pRuleCtl->vFlag(RULE_NO,bMay);
  return bMay;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* rule 00: OrdAgg + (sort) => OrdAgg                                         */
/*----------------------------------------------------------------------------*/
bool Board::bRule_AggGby_Sort_to_AggGby(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

#ifdef RULE_NO
  #undef RULE_NO
#endif
  
#define RULE_NO 0
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* rule 00: ordered stream + (AggGby) => OrdAgg                               */
/*----------------------------------------------------------------------------*/
bool Board::bRule_ordStream_AggGby_to_OrdAgg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
#ifdef RULE_NO
  #undef RULE_NO
#endif
  
#define RULE_NO 1

  return false;
}

/*----------------------------------------------------------------------------*/
/* rule 00: OrdAgg + (Sort) => OrdAgg                                         */
/*----------------------------------------------------------------------------*/
bool Board::bRule_OrdAgg_Sort_to_OrdAgg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

#ifdef RULE_NO
  #undef RULE_NO
#endif
  
#define RULE_NO 2

  return false;
}

/*----------------------------------------------------------------------------*/
/* rule: (Scan) + (AggGby) => IndexScan                                       */
/*----------------------------------------------------------------------------*/
bool Board::bRule_Scan_AggGby_to_IndexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
#ifdef RULE_NO
  #undef RULE_NO
#endif
  
#define RULE_NO 3

  return false;
}

/*----------------------------------------------------------------------------*/
/* may: (Scan) + (Filter) => IndexScan                                        */
/*----------------------------------------------------------------------------*/
bool Board::bRule_Scan_Filter_to_IndexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

#ifdef RULE_NO
  #undef RULE_NO
#endif
  
#define RULE_NO 4

  bool bMay = bFind_Scan_Filter(true);
  RASSERT(bMay);
  
  vPrintBoard("bFind_Scan_Filter >>> "_Stz);
  
  bool bAdded = bAdd_CandIndexes();
  UNUSED(bAdded);
  vPrintBoard("bAdd_CandidateIndexes >>> "_Stz);

  vRank_CandidateIndexes();
  vPrintBoard("vRank_CandidateIndexes >>> "_Stz);
  
  bool bPicked = bPick_CandidateIndex();
  vPrintBoard("bPick_CandidateIndex >>> "_Stz);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (! bPicked) {
    
    vCut_CandidateIndexes();
    vPrintBoard("vCut_CandidateIndexes >>> "_Stz);
    
    return false;
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vMark_PredCmps();
  vPrintBoard("vMark_PredCmps >>> "_Stz);
  
  vAdd_IndexScan();
  vPrintBoard("vAdd_IndexScan >>> "_Stz);

  vCut_PredCmps();
  vPrintBoard("vCut_PredCmps >>> "_Stz);
  
  vReduce_Filter();
  vPrintBoard("vReduce_Filter >>> "_Stz);
  
  vCull_Filter();
  vPrintBoard("vCull_Filter >>> "_Stz);
  
  vCut_Scan();
  vPrintBoard("vCut_Scan >>> "_Stz);
  
  vCut_CandidateIndexes();
  vPrintBoard("vCut_CandidateIndexes >>> "_Stz);
  
  vOrder_ProjectiveStreamDefs();
  vPrintBoard("vOrder_ProjectiveStreamDefs >>> "_Stz);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* rule: (Scan) + (Sort) => IndexScan                                         */
/*----------------------------------------------------------------------------*/
bool Board::bRule_Scan_Sort_to_IndexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

#ifdef RULE_NO
  #undef RULE_NO
#endif
  
#define RULE_NO 5

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
