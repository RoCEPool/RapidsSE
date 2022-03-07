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
Board::Board(Optimiser* pOptimiser, Stz* szQueryNm)
: Card_OTHER(CK_BOARD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
      
  m_pOptimiser = pOptimiser;
  m_sQueryNm   = szQueryNm;
  
  m_pPredCmp_ebound = nullptr;
  m_pPredCmp_lbound = nullptr;
  m_pPredCmp_ubound = nullptr;
  
  m_pReg_Aggreg    = nullptr;
  m_pReg_CandIndex = nullptr;
  m_pReg_Combine   = nullptr;
  m_pReg_Filter    = nullptr;
  m_pReg_Hash      = nullptr;
  m_pReg_IndexJoin = nullptr;
  m_pReg_IndexScan = nullptr;
  m_pReg_Limit     = nullptr;
  m_pReg_OrdAgg    = nullptr;
  m_pReg_Project   = nullptr;
  m_pReg_Scan      = nullptr;
  m_pReg_Send      = nullptr;
  m_pReg_Sort      = nullptr;
  m_pReg_Stage     = nullptr;
  m_pReg_Stream    = nullptr;

  m_pReg_LastSet       = nullptr;
  m_pReg_LastSet_main  = nullptr;
  m_pReg_LastSet_other = nullptr;
  m_pReg_LastSet_sub   = nullptr;

  m_pIndex     = nullptr;
  m_pTable     = nullptr;
  
  MemLib::vZero(m_baFlag,sizeof(m_baFlag),CN_CARD);

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
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


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
/* clear the board                                                            */
/*----------------------------------------------------------------------------*/
void Board::vClear(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vResetRegisters();
  vDeleteAllCards();
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
  
  i8 iAction = 1;
  TokenVec* pTknVec = nullptr;
  
  L_Entry:
  
  bool bCue   = true;
  bool bPrint = true;
  bool bExecuted = false;

  vRenumber_Cards();
  
  if (! bPrint)
    return;
  
  Ribbon* pRibbon = new Ribbon(m_pOptimiser->pCardCtl(),this,0);

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
  if (m_pReg_Stream) {
    vTrace("m_pReg_Stream : %llp"_Stz,m_pReg_Stream);
    bCR = true;
  }
  if (bCR)
    vTrace("\n"_Stz);

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
    
  iAction = 0;
  bExecuted = pOptFun->m_fpOptFun(m_pOptimiser,pTknVec,iAction);

  L_Next:
  
  if (! bExecuted)
    goto L_Input;
  
  if (0 == iAction)
    goto L_Entry;
  else
  if (1 == iAction)
    goto L_Input;
  else
  if (2 == iAction)
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
/* call closure for each board                                                */
/*----------------------------------------------------------------------------*/
void Board::vWalkAllCards(std::function<void(Card* pCard)> f_vClosure)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
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
void Board::vReg_Aggreg(Card_Aggreg* pAggreg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
    
  m_pReg_Aggreg = pAggreg;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_CandIndex(Card_CandIndex* pCandidateIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
    
  m_pReg_CandIndex = pCandidateIndex;
  
  if (m_pReg_CandIndex) {
    vIndex(m_pReg_CandIndex->pFindSubCard(CK_INDEX)->pIndex());
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_Combine(Card_Combine* pCombine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
    
  m_pReg_Combine = pCombine;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_Filter(Card_Filter* pFilter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
    
  m_pReg_Filter = pFilter;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_Hash(Card_Hash* pHash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
    
  m_pReg_Hash = pHash;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_IndexJoin(Card_IndexJoin* pIndexJoin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
    
  m_pReg_IndexJoin = pIndexJoin;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_IndexScan(Card_IndexScan* pIndexScan)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
    
  m_pReg_IndexScan = pIndexScan;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_Limit(Card_Limit* pLimit)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
    
  m_pReg_Limit = pLimit;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_OrdAgg(Card_OrdAgg* pOrdAgg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
    
  m_pReg_OrdAgg = pOrdAgg;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_Project(Card_Project* pProject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  m_pReg_Project = pProject;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_Scan(Card_Scan* pScan)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  m_pReg_Scan = pScan;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_Send(Card_Send* pSend)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
    
  m_pReg_Send = pSend;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_Sort(Card_Sort* pSort)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
    
  m_pReg_Sort = pSort;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_Stage(Card_Stage* pStage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  m_pReg_Stage = pStage;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_Stream(Card_Stream* pStream)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  m_pReg_Stream = pStream;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_Last(Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vReg_LastSet(pCard);

  if (pCard->bIs_MAIN())
    vReg_LastSet_main(pCard);
  else
  if (pCard->bIs_SUB())
    vReg_LastSet_sub(pCard);
  else
    vReg_LastSet_other(pCard);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_LastSet(Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  m_pReg_LastSet = pCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_LastSet_main(Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  m_pReg_LastSet_main = pCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_LastSet_other(Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  m_pReg_LastSet_other = pCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vReg_LastSet_sub(Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  m_pReg_LastSet_sub = pCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vPredCmp_ebound(Card_PredCmp* pPredCmp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  if (m_pPredCmp_ebound)
    m_pPredCmp_ebound->vMark2(' ');
  
  m_pPredCmp_ebound = pPredCmp;
  
  if (m_pPredCmp_ebound)
    m_pPredCmp_ebound->vMark2('E');
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vPredCmp_lbound(Card_PredCmp* pPredCmp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  if (m_pPredCmp_lbound)
    m_pPredCmp_lbound->vMark2(' ');
  
  m_pPredCmp_lbound = pPredCmp;
  
  if (m_pPredCmp_lbound)
    m_pPredCmp_lbound->vMark2('L');
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vPredCmp_ubound(Card_PredCmp* pPredCmp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  if (m_pPredCmp_ubound)
    m_pPredCmp_ubound->vMark2(' ');
  
  m_pPredCmp_ubound = pPredCmp;
  
  if (m_pPredCmp_ubound)
    m_pPredCmp_ubound->vMark2('U');
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  m_pIndex = pIndex;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Board::bFlag(i8 i_Flag)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  return m_baFlag[i_Flag];
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vFlag(i8 i_Flag, bool bFlag)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  m_baFlag[i_Flag] = bFlag;
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
bool Board::bInRegister(Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  if (pCard == m_pReg_Aggreg)    return true;
  if (pCard == m_pReg_CandIndex) return true;
  if (pCard == m_pReg_Combine)   return true;
  if (pCard == m_pReg_Filter)    return true;
  if (pCard == m_pReg_Hash)      return true;
  if (pCard == m_pReg_IndexJoin) return true;
  if (pCard == m_pReg_IndexScan) return true;
  if (pCard == m_pReg_Limit)     return true;
  if (pCard == m_pReg_OrdAgg)    return true;
  if (pCard == m_pReg_Project)   return true;
  if (pCard == m_pReg_Scan)      return true;
  if (pCard == m_pReg_Send)      return true;
  if (pCard == m_pReg_Sort)      return true;
  if (pCard == m_pReg_Stage)     return true;
  if (pCard == m_pReg_Stream)    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vResetRegisters(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vReg_Aggreg   (nullptr);
  vReg_CandIndex(nullptr);
  vReg_Combine  (nullptr);
  vReg_Filter   (nullptr);
  vReg_Hash     (nullptr);
  vReg_IndexJoin(nullptr);
  vReg_IndexScan(nullptr);
  vReg_Limit    (nullptr);
  vReg_OrdAgg   (nullptr);
  vReg_Project  (nullptr);
  vReg_Scan     (nullptr);
  vReg_Send     (nullptr);
  vReg_Sort     (nullptr);
  vReg_Stage    (nullptr);
  vReg_Stream   (nullptr);
  
  vReg_LastSet_main (nullptr);
  vReg_LastSet_other(nullptr);
  vReg_LastSet_sub  (nullptr);

  vPredCmp_ebound(nullptr);
  vPredCmp_lbound(nullptr);
  vPredCmp_ubound(nullptr);
  
  vIndex    (nullptr);
  vTable    (nullptr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vSetRegister(Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  eCK eCard = pCard->eCard();
  
  switch (eCard) {
    case CK_AGGREG:       vReg_Aggreg   (_pCard_Aggreg   (pCard)); break;
    case CK_CANDINDEX:    vReg_CandIndex(_pCard_CandIndex(pCard)); break;
    case CK_COMBINE:      vReg_Combine  (_pCard_Combine  (pCard)); break;
    case CK_FILTER:       vReg_Filter   (_pCard_Filter   (pCard)); break;
    case CK_HASH:         vReg_Hash     (_pCard_Hash     (pCard)); break;
    case CK_INDEXJOIN:    vReg_IndexJoin(_pCard_IndexJoin(pCard)); break;
    case CK_INDEXSCAN:    vReg_IndexScan(_pCard_IndexScan(pCard)); break;
    case CK_LIMIT:        vReg_Limit    (_pCard_Limit    (pCard)); break;
    case CK_ORDAGG:       vReg_OrdAgg   (_pCard_OrdAgg   (pCard)); break;
    case CK_PROJECT:      vReg_Project  (_pCard_Project  (pCard)); break;
    case CK_SCAN:         vReg_Scan     (_pCard_Scan     (pCard)); break;
    case CK_SEND:         vReg_Send     (_pCard_Send     (pCard)); break;
    case CK_SORT:         vReg_Sort     (_pCard_Sort     (pCard)); break;
    case CK_STAGE:        vReg_Stage    (_pCard_Stage    (pCard)); break;
      
    default:
      vTrace("SWITCH %s: Missing case for eCard %lld %s\n"_Stz
             ,__func__
             ,(i8)eCard
             ,aCK[eCard]
             );
      RASSERT(0);
      break;
  }

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
  
  RASSERT(m_pReg_Filter);
  
  i8 iRank = 0;
  bool bEBound = false;
  bool bLBound = false;
  bool bUBound = false;

  Index* pIndex = pCard_CandIndex->pFindSubCard(CK_INDEX)->pIndex();
  
  if (m_pReg_Filter->bHasBounds(pIndex)) {
  
    if (m_pReg_Filter->pFindBound(pIndex,BK_EBOUND)) {
      bEBound = true;
      pCard_CandIndex->vHasEBound(true);
    }
    
    if (m_pReg_Filter->pFindBound(pIndex,BK_LBOUND)) {
      bLBound = true;
      pCard_CandIndex->vHasLBound(true);
    }
    
    if (m_pReg_Filter->pFindBound(pIndex,BK_UBOUND)) {
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
      vReg_Stream(_pCard_Stream(pSubCard));
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
    
    vReg_Stream(pCard_Stream);
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
    
    vReg_Stream(pCard_Stream);
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
    vReg_Stream(pCard_Stream);

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
  
//  if (pCard_Term->bIsOrdinalStreamTerm()) {
//
//    i8 i_Ord = pCard_Term->iOrdinalStreamColNo();
//    i8 i_Col = i_Ord - 1;
//
//    Column* pColumn = m_pStreamDef->pGetColumn(i_Col);
//    Stz* szColNm = pColumn->szColNm();
//
//    pCard_Term->vTerm(szColNm);
//
//    return;
//  }
  if (pCard_Term->bIsOrdinalStreamTerm()) {

    i8 i_Ord = pCard_Term->iOrdinalStreamColNo();
    i8 i_Col = i_Ord - 1;

    //Column* pColumn = m_pStreamDef->pGetColumn(i_Col);
    //Stz* szColNm = pColumn->szColNm();

    Card_StreamElist* pCard_StreamElist_src =
      _pCard_StreamElist(m_pReg_Stream->pFindSubCard(CK_STREAMELIST));

    Card_Expr* pCard_Expr_src = _pCard_Expr(pCard_StreamElist_src->pGet(i_Col));

    Card_Term* pCard_Term_src =
      _pCard_Term(pCard_Expr_src->pFindSubCard(CK_TERM));

    Stz* szColNm_resolved = pCard_Term_src->szTerm();
    
    pCard_Term->vTerm(szColNm_resolved);

    return;
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pIndex && pCard_Term->bIsOrdinalIndexTerm()) {
    
    i8 i_IdxOrd = pCard_Term->iOrdinalIndexColNo();
    i8 i_IdxCol = i_IdxOrd - 1;

    Column* pIdxColumn = m_pIndex->pGetColumn(i_IdxCol);
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
  
  i8 i_Filter = i_Find(m_pReg_Scan) + 1;
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

  Card_Index* pCard_Index = new Card_Index(m_pIndex);
  pCard_IndexScan->vAppend(pCard_Index);

  
  Card_BOUND* pCard_BOUND;
  Card_PRED*  pCard_PRED;

  if (m_pPredCmp_ebound) {
    pCard_BOUND = new Card_EBound;
    
    pCard_PRED = m_pPredCmp_ebound->pDup();
    pCard_PRED->vHasEBound(true);
    pCard_BOUND->vAppend(pCard_PRED);
  }
  else
  if (m_pPredCmp_lbound && m_pPredCmp_ubound) {
    pCard_BOUND = new Card_RBound;
    
    pCard_PRED = m_pPredCmp_lbound->pDup();
    pCard_PRED->vHasLBound(true);
    pCard_BOUND->vAppend(pCard_PRED);
    
    pCard_PRED = m_pPredCmp_ubound->pDup();
    pCard_PRED->vHasUBound(true);
    pCard_BOUND->vAppend(pCard_PRED);
  }
  else
  if (m_pPredCmp_lbound) {
    pCard_BOUND = new Card_LBound;
    
    pCard_PRED = m_pPredCmp_lbound->pDup();
    pCard_PRED->vHasLBound(true);
    pCard_BOUND->vAppend(pCard_PRED);
  }
  else
  if (m_pPredCmp_ubound) {
    pCard_BOUND = new Card_UBound;
    
    pCard_PRED = m_pPredCmp_ubound->pDup();
    pCard_PRED->vHasUBound(true);
    pCard_BOUND->vAppend(m_pPredCmp_ubound->pDup());
  }
  else {
    RASSERT(0);
  }
  
  pCard_IndexScan->vAppend(pCard_BOUND);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the stream elist

  StreamDef* pStreamDef = m_pIndex->pTable()->pStreamDef();
  
  Card_Stream* pCard_Stream = new Card_Stream;
  pCard_IndexScan->vAppend(pCard_Stream);

  Card_StreamElist* pCard_StreamElist = new Card_StreamElist(pStreamDef);
  
  
  pCard_Stream->vAppend(pCard_StreamElist);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the stream order
  
  Card_OrderElist* pCard_OrderElist = new Card_OrderElist(m_pIndex);
  pCard_Stream->vAppend(pCard_OrderElist);

  //bool bUnique = pIndexDef->bIsUnique();
  //pStreamDef->vUnique(bUnique);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  vInsertBefore(pCard_IndexScan,m_pReg_Scan);
  
  vReg_IndexScan(pCard_IndexScan);
  
//  vPredCmp_ebound(nullptr);
//  vPredCmp_lbound(nullptr);
//  vPredCmp_ubound(nullptr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCull_Filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  if (! m_pReg_Filter->bCull())
    return;
  
  vRemove(m_pReg_Filter);
  
  delete m_pReg_Filter;

  vReg_Filter(nullptr);
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
  
  if (m_pPredCmp_ebound)
    m_pReg_Filter->vRemoveSubCard(m_pPredCmp_ebound);
  
  if (m_pPredCmp_lbound)
    m_pReg_Filter->vRemoveSubCard(m_pPredCmp_lbound);
  
  if (m_pPredCmp_ubound)
    m_pReg_Filter->vRemoveSubCard(m_pPredCmp_ubound);
  
  vPredCmp_ebound(nullptr);
  vPredCmp_lbound(nullptr);
  vPredCmp_ubound(nullptr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCut_Scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vRemove(m_pReg_Scan);
  
  vReg_Scan(nullptr);

  delete m_pReg_Scan;  
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
  
  vReg_Scan(nullptr);
  
  i8 nMainCards = this->nMainCards();
  for (i8 i_MainCard = 0; i_MainCard < nMainCards-1; i_MainCard++) {
    Card* pTestScan = pGetMainCard(i_MainCard);
    if (CK_SCAN != pTestScan->eCard())
      continue;

    Card* pTestFilter = pGetMainCard(i_MainCard+1);
    if (CK_FILTER != pTestFilter->eCard())
      continue;
    
    if (bMark)
      vReg_Scan(_pCard_Scan(pTestScan));

    Card_Table* pCard_Table = _pCard_Table(pTestScan->pFindSubCard(CK_TABLE));
    vTable(pCard_Table->pTable());
    
    if (bMark)
      vReg_Filter(_pCard_Filter(pTestFilter));

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
  
  i8 iRank = m_pReg_CandIndex->iRank();
  
  switch (iRank) {
      
    case 1: {
      // lower bound or upper bound
      Card_PredCmp* pPredCmp_lbound =
        m_pReg_Filter->pFindBound(m_pIndex,BK_LBOUND);
      if (pPredCmp_lbound) {
        vPredCmp_lbound(pPredCmp_lbound);
        break;
      }
      Card_PredCmp* pPredCmp_ubound =
        m_pReg_Filter->pFindBound(m_pIndex,BK_UBOUND);
      if (pPredCmp_ubound) {
        vPredCmp_ubound(pPredCmp_ubound);
        break;
      }
      break;
    }

    case 2: {
      // lower bound and upper bound
      Card_PredCmp* pPredCmp_lbound =
        m_pReg_Filter->pFindBound(m_pIndex,BK_LBOUND);
      if (pPredCmp_lbound)
        vPredCmp_lbound(pPredCmp_lbound);
      Card_PredCmp* pPredCmp_ubound =
        m_pReg_Filter->pFindBound(m_pIndex,BK_UBOUND);
      if (pPredCmp_ubound)
        vPredCmp_ubound(pPredCmp_ubound);
      break;
    }
      
    case 3: {
      // equi-bound
      Card_PredCmp* pPredCmp_ebound =
        m_pReg_Filter->pFindBound(m_pIndex,BK_EBOUND);
      if (pPredCmp_ebound)
        vPredCmp_ebound(pPredCmp_ebound);
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
  
  Card_Stream* pCard_Stream_src =
    _pCard_Stream(m_pReg_IndexScan->pFindSubCard(CK_STREAM));

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
    vReg_CandIndex(nullptr);
    return false;
  }
  
  vReg_CandIndex(pCard_CandIndex);

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
  
  if (! m_pReg_Filter)
    return;
  
  m_pReg_Filter->vReduceFilter();
  
  
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vRenumber_Cards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  ListExoIter2_t<Card>* pBoard_I = pNewIter();

  i8 iCardNo = 0;
  while (pBoard_I->bHasNext()) {
    Card* pCard = pBoard_I->pNext();
    
    pCard->vCardNo(iCardNo++);
    pCard->vRenumberSubCards();
  }
  
  delete pBoard_I;
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
  
  vFlag(RULE_NO,bMay);
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
  
  vFlag(RULE_NO,bMay);
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
  
  vFlag(RULE_NO,bMay);
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
  
  vFlag(RULE_NO,bMay);
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
  
  vFlag(RULE_NO,bMay);
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
  
  vFlag(RULE_NO,bMay);
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

  bool bMay = false;
  
  return bMay;
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

  bool bMay = false;
  
  return bMay;
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

  bool bMay = false;
  
  return bMay;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
