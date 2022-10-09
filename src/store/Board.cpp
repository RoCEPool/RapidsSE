/*============================================================================*/
/* Board.cpp -- Board                                                         */
/*============================================================================*/

#include "Board.hpp"

#include "PtnDef.hpp"
#include "Cue.hpp"

#include "Optimiser.hpp"
#include "OptimiserFun.hpp"
#include "OptimiserCatalog.hpp"

#include "Card_StreamCol.hpp"

extern OptimiserCatalog gxOptimiserCatalog;

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
Board::Board(eBK eBoard, eBT eBoardType, Bench* pBench)
: Object(CN_BOARD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_eBoard       = eBoard;
  m_eBoardType   = eBoardType;
  m_pBench       = pBench;

  m_pOptimiser   = m_pBench->pOptimiser();

  m_pCard_Board  = new Card_Board;

  m_bBeenCued = false;

  _checkInitialised_(CN_BOARD);

  pBench->vAddBoard(this);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Board::~Board(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  if (BT_VIEW == m_eBoardType) {
    m_pCard_Board->vRemoveAll();
  }

  delete m_pCard_Board;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* append new main card at end                                                */
/*----------------------------------------------------------------------------*/
void Board::vAppendMain(Card* pNewMainCard, bool bRenumber)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_pCard_Board->vAppend(pNewMainCard);

  if (bRenumber)
    vRenumberMainCards();
}

/*----------------------------------------------------------------------------*/
/* get board kind                                                             */
/*----------------------------------------------------------------------------*/
eBK Board::eBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_eBoard;
}

/*----------------------------------------------------------------------------*/
/* get card registers                                                         */
/*----------------------------------------------------------------------------*/
CardCtl* Board::pCardCtl(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_pBench->pCardCtl();
}

/*----------------------------------------------------------------------------*/
/* get card subSet                                                            */
/*----------------------------------------------------------------------------*/
Card_Board* Board::pCard_Board(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_pCard_Board;
}

/*----------------------------------------------------------------------------*/
/* clear the board                                                            */
/*----------------------------------------------------------------------------*/
void Board::vClear(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  vDeleteAllCards();
}

/*----------------------------------------------------------------------------*/
/* clear all marks                                                            */
/*----------------------------------------------------------------------------*/
void Board::vClearAllMarks(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  i8 nMainCards = this->nMainCards();
  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
    Card *pMainCard = m_pCard_Board->pGet(i_Main);

    pMainCard->vClearAllMarks();
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDeleteAllCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  while (Card* pCard = m_pCard_Board->pPop())
    delete pCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDeleteFromTill(Card* pFrom, Card* pTillBefore)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  i8 i_Main = -1;
  i8 nMainCards = this->nMainCards();
  for (i_Main = 0; i_Main < nMainCards; i_Main++) {
    Card* pMainCard = m_pCard_Board->pGet(i_Main);
    if (pMainCard == pFrom)
      break;
  }

  if (i_Main >= nMainCards)
    return;

  for (;;) {

    nMainCards = this->nMainCards();
    if (i_Main >= nMainCards)
      break;

    Card* pDeleteCard = m_pCard_Board->pGet(i_Main);

    if (pDeleteCard == pTillBefore)
      break;

    m_pCard_Board->vRemove(pDeleteCard);
    delete pDeleteCard;
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDuplicateBoard(Board* pBoard_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  for (i8 i_Main_arg = 0; i_Main_arg < pBoard_arg->nMainCards(); i_Main_arg++)
    vAppendMain(pBoard_arg->pGetMain(i_Main_arg)->pDupCardAndSubCards());
}

/*----------------------------------------------------------------------------*/
/* find i_Main of given main card                                             */
/*----------------------------------------------------------------------------*/
i8 Board::iFindMain(Card* pMainCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  i8 nMainCards = this->nMainCards();
  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
    Card* pTestCard = pGetMain(i_Main);

    if (pTestCard == pMainCard)
      return i_Main;
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Board::pFindMain(eCK eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  Card* pMainCard = m_pCard_Board->pFindSubCard(eCard);

  return pMainCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Board::pFindMain(eCK eCard, i8& i_Main)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  Card* pMainCard = m_pCard_Board->pFindChild(eCard);

  return pMainCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Board::pFindMain(Stz* szCardNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  eCK eCard = Card::eTo_eCK(szCardNm);
  
  i8 nMainCards = this->nMainCards();
  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
    Card* pMainCard = pGetMain(i_Main);
    
    if (pMainCard->eCard() == eCard)
      return pMainCard;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find next main card after given arg which is an eFindCard                  */
/*----------------------------------------------------------------------------*/
Card* Board::pFindNextMain(Card* pAfter, eCK eFindCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  i8 i_Main;
  Card* pMainCard = pFindNextMain(pAfter,eFindCard,i_Main);

  return pMainCard;
}

/*----------------------------------------------------------------------------*/
/* find next main card after given arg which is an eFindCard                  */
/*----------------------------------------------------------------------------*/
Card* Board::pFindNextMain(Card* pAfter, eCK eFindCard, i8& i_Main)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Card* pMainCard = nullptr;

  i8 nMainCards = this->nMainCards();
  for (i_Main = 0; i_Main < nMainCards; i_Main++) {
    pMainCard = pGetMain(i_Main);

    if (pMainCard == pAfter)
      break;
  }

  if (pMainCard != pAfter)
    return nullptr;

  // found after

  // get next eCard
  for (i8 i_TestCard = i_Main + 1; i_TestCard < nMainCards; i_TestCard++) {
    Card* pTestCard = pGetMain(i_TestCard);

    if (eFindCard == pTestCard->eCard()) {
      i_Main = i_TestCard;
      return pTestCard;
    }
  }

  i_Main = C_UNKNOWN;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find prev main card given given arg which is an eFindCard                  */
/*----------------------------------------------------------------------------*/
Card* Board::pFindPrevMain(Card* pBefore, eCK eFindCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  i8 i_Main;
  Card* pMainCard = pFindPrevMain(pBefore, eFindCard, i_Main);

  return pMainCard;
}

/*----------------------------------------------------------------------------*/
/* find prev main card given given arg which is an eFindCard                  */
/*----------------------------------------------------------------------------*/
Card* Board::pFindPrevMain(Card* pBefore, eCK eFindCard, i8& i_Main)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Card* pMainCard = nullptr;

  i8 nMainCards = this->nMainCards();
  for (i_Main = 0; i_Main < nMainCards; i_Main++) {
    pMainCard = pGetMain(i_Main);

    if (pMainCard == pBefore)
      break;
  }

  if (pMainCard != pBefore)
    return nullptr;

  // found before

  // get prev eCard
  for (i8 i_TestCard = i_Main-1; i_TestCard >= 0; i_TestCard--) {
    Card* pTestCard = pGetMain(i_TestCard);

    if (eFindCard == pTestCard->eCard()) {
      i_Main = i_TestCard;
      return pTestCard;
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find next main card after given arg which is an eFindCard                  */
/*----------------------------------------------------------------------------*/
Card_Filter* Board::pFindFilterCard(i8 iFilter, Card*& rpMainCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  i8 i_FilterSoFar = 0;

  rpMainCard = nullptr;

  i8 nMains = this->nMainCards();
  for (i8 i_Main = 0; i_Main < nMains; i_Main++) {
    rpMainCard = pGetMain(i_Main);

    eCK eMainCard = rpMainCard->eCard();
    if (CK_ABSJOIN == eMainCard) {
      if (i_FilterSoFar++ != iFilter)
        continue;
      Card_Filter* pCard_filter = _pGetSub_Filter(rpMainCard);
      return pCard_filter;
    }

    if (CK_SCAN == eMainCard) {
      if (i_FilterSoFar++ != iFilter)
        continue;
      Card_Filter* pCard_filter = _pGetSub_Filter(rpMainCard);
      return pCard_filter;
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find next main card after given arg which is an eFindCard                  */
/*----------------------------------------------------------------------------*/
Card_Stream* Board::pFindStreamCard(i8 iStream, Card*& rpMainCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  rpMainCard = nullptr;

  i8 nMains = this->nMainCards();
  for (i8 i_Main = 0; i_Main < nMains; i_Main++) {
    rpMainCard = pGetMain(i_Main);

    eCK eMainCard = rpMainCard->eCard();
    if (CK_ABSJOIN == eMainCard) {
      Card_Scan* pCard_scan = _pGetSub_Scan(rpMainCard);
      Card_Stream* pCard_stream = _pGetSub_Stream(pCard_scan);
      if (iStream != pCard_stream->iStream())
        continue;

      return pCard_stream;
    }

    if (CK_SCAN == eMainCard) {
      Card_Stream* pCard_stream = _pGetSub_Stream(rpMainCard);
      if (iStream != pCard_stream->iStream())
        continue;

      return pCard_stream;
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Board::pGetMain(i8 i_Main)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Card* pMainCard = m_pCard_Board->pGet(i_Main);
  return pMainCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Board::pGetMain(i8 i_Main, eCK& reCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Card* pMainCard = m_pCard_Board->pGet(i_Main);

  if (pMainCard)
    reCard = pMainCard->eCard();
  else
    reCard = CK_NONE;

  return pMainCard;
}

/*----------------------------------------------------------------------------*/
/* insert new main card after given card                                      */
/*----------------------------------------------------------------------------*/
void Board::vInsertMainCard_after(Card* pNewMainCard, Card* pAfterMainCard,
                                  bool bRenumber)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_pCard_Board->vInsertAfter(pNewMainCard,pAfterMainCard);

  if (bRenumber)
    vRenumberMainCards();
}

/*----------------------------------------------------------------------------*/
/* insert new main card before given card                                     */
/*----------------------------------------------------------------------------*/
void Board::vInsertMainCard_before(Card* pNewMainCard, Card* pBeforeMainCard,
                                   bool bRenumber)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_pCard_Board->vInsertBefore(pNewMainCard,pBeforeMainCard);

  if (bRenumber)
    vRenumberMainCards();
}

/*----------------------------------------------------------------------------*/
/* get number of cards                                                        */
/*----------------------------------------------------------------------------*/
i8 Board::nMainCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  i8 nCards = this->m_pCard_Board->nItems();
  
  return nCards;
}

/*----------------------------------------------------------------------------*/
/* new a main card iterator                                                   */
/*----------------------------------------------------------------------------*/
SubCardIter* Board::pNewMainCardIter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  SubCardIter* pMainCardIter = m_pCard_Board->pNewSubCardIter();

  return pMainCardIter;
}

/*----------------------------------------------------------------------------*/
/* pin board onto given ribbon                                                */
/*----------------------------------------------------------------------------*/
void Board::vPinCards(Ribbon* pRibbon, Stz* szTransformation, Stz* szExplain)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  if (! nMainCards())
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pRibbon->vSprint("\n[Board: %s %s] Query=%s Transformation=%s"_Stz,
                   aBK[m_eBoard],
                   aBT[m_eBoardType],
                   m_pOptimiser->szQueryNm(),
                   szTransformation ? szTransformation : ""_Stz
                   );
  if (szExplain && *szExplain->azChar()) {
    pRibbon->vSprint("Explanation: %s"_Stz, szExplain);
    pRibbon->vTitleLines(2);
  }
  else {
    pRibbon->vTitleLines(1);
  }
  pRibbon->vSetBase();
  pRibbon->vFirstColumn();

  Ribbon* pSubRibbon = new Ribbon(m_pOptimiser->pCardCtl(),this,0);
  

  BoardIter xBi(this);

  i8 nCount = 0;
  while (Card* pCard = xBi.pEachMain()) {
    eCK eCard = pCard->eCard();

    if (! nCount++)
      pSubRibbon->vFirstColumn();
    else
    if ( pSubRibbon->iChar() && m_pOptimiser->pCardCtl()->bNextHoriz(eCard))
      pSubRibbon->vNextHorizontal();
    else
      pSubRibbon->vNextColumn();

    bool bPrune = false;
    pCard->vPrintCard(pSubRibbon,0,bPrune);
  }
     
  pRibbon->vPinRibbon(pRibbon->nTitleLines(),0,pSubRibbon);
  
  delete pSubRibbon;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vPrintBoard(Stz* szCue, Stz* szExplain)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  eOA eOptAct = OA_ENTRY;
  TokenVec* pTknVec = nullptr;
  
  L_Entry:
  
  bool bCue   = true;
  bool bPrint = true;
  bool bExecuted = false;

  vRenumberMainCards();
  
  if (! bPrint)
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Ribbon* pRibbon = new Ribbon(m_pOptimiser->pCardCtl(),this,0);

  vPinCards(pRibbon,szCue,szExplain);

  pRibbon->vPrintRibbon();

  delete pRibbon;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (! bCue)
    return;
  if (! szCue)
    return;

  if (! m_bBeenCued && AM::JOB::bHasTag("<cue>"_Stz)) {
    Stz* szTagCue = AM::JOB::szTagValue("<cue>"_Stz);
    if (! szTagCue->bSame(szCue)) {
      printf("%s\n",(char*)szCue);
      return;
    }
  }

  m_bBeenCued = true;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  L_Input:

  s256 sPrompt;

  sPrompt.vSprint("%s >> "_Stz,szCue);

  s4K sInput;
  Cue::vGetInput(sPrompt,sInput);
    
  if (! sInput.nLength()) {
    return;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pTknVec) {
    delete pTknVec;
    pTknVec = nullptr;
  }
  
  pTknVec = AM::TOKENISE::pTokeniseSen(sInput);
  
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
/* get prior subcard                                                          */
/*----------------------------------------------------------------------------*/
Card* Board::pPriorSubCard(Card*& rpMainCard, eCK eSubCard, i8 i_PriorToMain)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  for (i8 i_Main = i_PriorToMain - 1; i_Main >= 0; i_Main--) {
    rpMainCard = pGetMain(i_Main);

    Card* pSubCard;
    if ((pSubCard = rpMainCard->pFindSubCard(eSubCard)))
      return pSubCard;
  }

  rpMainCard = nullptr;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vRemoveMainCard(Card* pMainCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_pCard_Board->vRemove(pMainCard);
}

/*----------------------------------------------------------------------------*/
/* renumber the cards                                                         */
/*----------------------------------------------------------------------------*/
void Board::vRenumberMainCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  ListExoIter2_t<Card>* pMainCard_I = m_pCard_Board->pNewIter();

  i8 iMainCardNo = 0;
  while (pMainCard_I->bHasNext()) {
    Card* pMainCard = pMainCard_I->pNext();

    pMainCard->vCardNo(iMainCardNo++);
    pMainCard->vRenumberSubCards(nullptr);
  }

  delete pMainCard_I;
}



// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vRankCandIndex(Card_CandIndex* pCard_CandIndex)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  Card_Filter* pReg_Filter = _pCard_Filter(m_pCardRegs->pCardReg(CK_FILTER));
//
//  RASSERT(pReg_Filter);
//
//  i8 iRank = 0;
//  bool bEBound = false;
//  bool bLBound = false;
//  bool bUBound = false;
//
//  Index* pIndex = pCard_CandIndex->pFindSubCard(CK_INDEXPTR)->pIndex();
//
//  if (pReg_Filter->bHasBounds(pIndex)) {
//
//    if (pReg_Filter->pFindBound(pIndex,PB_EBOUND)) {
//      bEBound = true;
//      pCard_CandIndex->vHasEBound(true);
//    }
//
//    if (pReg_Filter->pFindBound(pIndex,PB_LBOUND)) {
//      bLBound = true;
//      pCard_CandIndex->vHasLBound(true);
//    }
//
//    if (pReg_Filter->pFindBound(pIndex,PB_UBOUND)) {
//      bUBound = true;
//      pCard_CandIndex->vHasUBound(true);
//    }
//  }
//
//  if (bEBound)
//    iRank = 3;
//  else
//  if (bLBound && bUBound)
//    iRank = 2;
//  else
//  if (bLBound || bUBound)
//    iRank = 1;
//
//  pCard_CandIndex->vRank(iRank);
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vResolveFilter(Card_Filter* pCard_Filter)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);
//
//  RASSERT(1 == pCard_Filter->nChildCards());
//
//  Card_Expr* pCard_Expr = _pCard_Expr(pCard_Filter->pGet(0));
//  RASSERT(2 == pCard_Expr->nChildCards());
//
//  Card_Type* pCard_Type = _pCard_Type(pCard_Expr->pGet(0));
//  Card_Calc* pCard_Calc = _pCard_Calc(pCard_Expr->pGet(1));
//
//  Card_CalcBool* pCard_CalcBool = pTo_CalcBool(pCard_Calc);
//
//  pCard_Expr->vRemove(pCard_Calc);
//  delete pCard_Calc;
//  pCard_Expr->vAppend(pCard_CalcBool);
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vResolveJoin(Card_AbsJoin* pCard_AbsJoin)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);
//
//  Card_Scan* pCard_Scan = _pCard_Scan(pGetMain(pCard_AbsJoin->iCardNo() - 1));
//  Card_TablePtr* pCard_TablePtr = _pCard_TablePtr(pCard_Scan->pFindSubCard(CK_TABLEPTR));
//  Table* pTable = pCard_TablePtr->pTable();
//
//  Index* pIndex = pTable->pIdxVec()->pGet(0);
//
//  Card_IndexJoin* pIndexJoin = new Card_IndexJoin;
//  pIndexJoin->vAppend(new Card_TablePtr(pTable));
//  pIndexJoin->vAppend(new Card_IndexPtr(pIndex));
//
//  Card_Filter* pCard_Filter = _pCard_Filter(pGetMain(pCard_AbsJoin->iCardNo() + 3));
//  pCard_Filter->vDump();
//
//  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;
//  pIndexJoin->vAppend(pCard_IndexPred);
//
//  pCard_IndexPred->vAppend(new Card_Term("BOUND"_Stz));
//  pCard_IndexPred->vAppend(new Card_Term("$I1"_Stz));
//  pCard_IndexPred->vAppend(new Card_Term("$L1"_Stz));
//  pCard_IndexPred->vAppend(new Card_Term("=="_Stz));
//
//  m_pCard_Board->vInsertAfter(pIndexJoin,pCard_AbsJoin);
//
//  Card_Combined* pCard_Combine = new Card_Combined;
//  m_pCard_Board->vInsertAfter(pCard_Combine,pIndexJoin);
//
//
//  // temp hack
//
//  Card_Stage* pCard_Stage_next =
//    _pCard_Stage(pFindNextMain(pIndexJoin,CK_STAGE));
//  if (! pCard_Stage_next)
//    return;
//
//  Card_Stage* pCard_Stage_following =
//    _pCard_Stage(pFindNextMain(pCard_Stage_next,CK_STAGE));
//
//  vDeleteFromTill(pCard_Stage_next,pCard_Stage_following);
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vResolveJoin2(Card_AbsJoin* pCard_AbsJoin)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);
//
//  Card_Scan* pCard_Scan = _pCard_Scan(pGetMain(pCard_AbsJoin->iCardNo() - 1));
//  Card_TablePtr* pCard_TablePtr = _pCard_TablePtr(pCard_Scan->pFindSubCard(CK_TABLEPTR));
//  Table* pTable = pCard_TablePtr->pTable();
//
//  Index* pIndex = pTable->pIdxVec()->pGet(0);
//
//  Card_IndexJoin* pIndexJoin = new Card_IndexJoin;
//  pIndexJoin->vAppend(new Card_TablePtr(pTable));
//  pIndexJoin->vAppend(new Card_IndexPtr(pIndex));
//
//  Card_Filter* pCard_Filter = _pCard_Filter(pGetMain(pCard_AbsJoin->iCardNo() + 3));
//  //pCard_Filter->vDump();
//
//  Card_Expr* pCard_Expr = _pCard_Expr(pCard_Filter->pGetChild(0));
//
//  Card_Type* pCard_Type = _pCard_Type(pCard_Expr->pGetChild(0));
//  Card_CalcBool* pCard_CalcBool = _pCard_CalcBool(pCard_Expr->pGetChild(1));
//  Card_PredCmp* pCard_PredCmp = _pCard_PredCmp(pCard_CalcBool->pGetChild(0));
//
//  Card_IndexPred* pCard_IndexPred = pTo_IndexPred(pCard_PredCmp,nullptr);
//  pIndexJoin->vAppend(pCard_IndexPred);
//
//  m_pCard_Board->vInsertAfter(pIndexJoin,pCard_AbsJoin);
//
//  Card_Combined* pCard_Combine = new Card_Combined;
//  m_pCard_Board->vInsertAfter(pCard_Combine,pIndexJoin);
//
//  //delete pCard_PredCmp;
//
//  // temp hack
//
//  Card_Stage* pCard_Stage_next =
//          _pCard_Stage(pFindNextMain(pIndexJoin,CK_STAGE));
//  if (! pCard_Stage_next)
//    return;
//
//  Card_Stage* pCard_Stage_following =
//          _pCard_Stage(pFindNextMain(pCard_Stage_next,CK_STAGE));
//
//  vDeleteFromTill(pCard_Stage_next,pCard_Stage_following);
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vResolveJoin3(Card_IndexJoin* pCard_IndexJoin)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);
//
//  Card_Filter* pCard_Filter =
//    _pCard_Filter(pFindNextMain(pCard_IndexJoin, CK_FILTER));
//
//  Card_Expr* pCard_Expr = _pCard_Expr(pCard_Filter->pGetChild(0));
//
//  Card_Type* pCard_Type = _pCard_Type(pCard_Expr->pGetChild(0));
//  Card_CalcBool* pCard_CalcBool = _pCard_CalcBool(pCard_Expr->pGetChild(1));
//  Card_PredCmp* pCard_PredCmp = _pCard_PredCmp(pCard_CalcBool->pGetChild(0));
//
//  Card_MapSet* pCard_MapSet = _pCard_MapSet(pCard_IndexJoin->pFindSubCard(CK_MAPSET));
//  Card_IndexPred* pCard_IndexPred = pTo_IndexPred(pCard_PredCmp,pCard_MapSet);
//  Card_IndexPtr* pCard_IndexPtr = _pCard_IndexPtr(pCard_IndexJoin->pFindSubCard(CK_INDEXPTR));
//  pCard_IndexJoin->vInsertAfter(pCard_IndexPred, pCard_IndexPtr);
//
//  Card_Combined* pCard_Combine = new Card_Combined;
//  m_pCard_Board->vInsertAfter(pCard_Combine,pCard_IndexJoin);
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vAResolveJoin(Card_IndexJoin* pCard_IndexJoin)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);
//
//  // indexJoin
//  // .0){table}
//  // .1){index}
//  // .2)indexPred
//  // ..0)term BOUND
//  // ..1)term COL -> $K1
//  // ..2)term COL -> $1
//  // ..3)term ==
//
//  Card_IndexPred* pCard_IndexPred =
//    _pCard_IndexPred(pCard_IndexJoin->pFindSubCard(CK_INDEXPRED));
//
//  Card_Term* pCard_key =
//    _pCard_Term(pCard_IndexJoin->pGetChild(1));
//
//  Card_Term* pCard_col =
//    _pCard_Term(pCard_IndexJoin->pGetChild(2));
//
//
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vConvertJoin(Card_AbsJoin* pCard_AbsJoin)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);
//
//  m_pCard_Board->vRemove(pCard_AbsJoin);
//
//  Card_Scan* pCard_Scan = _pCard_Scan(pGetMain(pCard_AbsJoin->iCardNo() - 1));
//  Card_TablePtr* pCard_TablePtr = _pCard_TablePtr(pCard_Scan->pFindSubCard(CK_TABLEPTR));
//  Table* pTable = pCard_TablePtr->pTable();
//
//
//  Card_IndexJoin* pIndexJoin = new Card_IndexJoin;
//  pIndexJoin->vAppend(new Card_TablePtr(pTable));
//
//  // TODO:
//  Index* pIndex = pTable->pIdxVec()->pGet(0);
//  pIndexJoin->vAppend(new Card_IndexPtr(pIndex));
//
//  m_pCard_Board->vInsertAfter(pIndexJoin,pCard_Scan);
//
//  m_pCard_Board->vRemove(pCard_Scan);
//  delete pCard_Scan;
//}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//bool Board::bAdd_CandIndexes(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  Card_Scan* pReg_Scan = _pCard_Scan(m_pCardRegs->pCardReg(CK_SCAN));
//
//  Table* pTable = this->m_pCard_Board->pTable();
//
//  IdxVec* pIdxVec = pTable->pIdxVec();
//  if (! pIdxVec) {
//    vTrace("No IdxVec\n"_Stz);
//    return false;
//  }
//
//  i8 nIndexes = pIdxVec->nItems();
//  if (! nIndexes) {
//    vTrace("No nIndexes\n"_Stz);
//    return false;
//  }
//
//  i8 i_Filter = m_pCard_Board->i_Find(pReg_Scan) + 1;
//  Card_Filter* pCard_Filter = _pCard_Filter(m_pCard_Board->pGet(i_Filter));
//
//  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
//    Index* pIndex = pIdxVec->pGet(i_Index);
//    //vTrace("Index %s\n"_Stz,pIndex->szIndexNm());
//
//    Card_CandIndex* pCard_CandIndex = new Card_CandIndex;
//    Card_IndexPtr* pCard_Index = new Card_IndexPtr(pIndex);
//    pCard_CandIndex->vAppend(pCard_Index);
//
//    m_pCard_Board->vInsertAfter(pCard_CandIndex,pCard_Filter);
//  }
//
//  return true;
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vAdd_IndexScan(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  //m_pFilter->vDump();
//
//  Card_IndexScan* pCard_IndexScan = new Card_IndexScan;
//  Card_Scan* pReg_Scan = _pCard_Scan(m_pCardRegs->pCardReg(CK_SCAN));
//
//  Card_PredCmp* pReg_ebound = m_pCardRegs->pPredCmp_ebound();
//  Card_PredCmp* pReg_lbound = m_pCardRegs->pPredCmp_lbound();
//  Card_PredCmp* pReg_ubound = m_pCardRegs->pPredCmp_ubound();
//
//  Index* pIndex = this->m_pCard_Board->pIndex();
//  Card_IndexPtr* pCard_Index = new Card_IndexPtr(pIndex);
//  pCard_IndexScan->vAppend(pCard_Index);
//
//
//  Card_BOUND* pCard_BOUND;
//  Card_PRED*  pCard_PRED;
//
//  if (pReg_ebound) {
//    pCard_BOUND = new Card_EBound;
//
//    pCard_PRED = pReg_ebound->pDup();
//    pCard_PRED->vHasEBound(true);
//    pCard_BOUND->vAppend(pCard_PRED);
//  }
//  else
//  if (pReg_lbound && pReg_ubound) {
//    pCard_BOUND = new Card_RBound;
//
//    pCard_PRED = pReg_lbound->pDup();
//    pCard_PRED->vHasLBound(true);
//    pCard_BOUND->vAppend(pCard_PRED);
//
//    pCard_PRED = pReg_ubound->pDup();
//    pCard_PRED->vHasUBound(true);
//    pCard_BOUND->vAppend(pCard_PRED);
//  }
//  else
//  if (pReg_lbound) {
//    pCard_BOUND = new Card_LBound;
//
//    pCard_PRED = pReg_lbound->pDup();
//    pCard_PRED->vHasLBound(true);
//    pCard_BOUND->vAppend(pCard_PRED);
//  }
//  else
//  if (pReg_ubound) {
//    pCard_BOUND = new Card_UBound;
//
//    pCard_PRED = pReg_ubound->pDup();
//    pCard_PRED->vHasUBound(true);
//    pCard_BOUND->vAppend(pReg_ubound->pDup());
//  }
//  else {
//    RASSERT(0);
//  }
//
//  pCard_IndexScan->vAppend(pCard_BOUND);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // set the stream elist
//
//  StreamDef* pStreamDef = pIndex->pTable()->pStreamDef();
//
//  Card_Stream* pCard_Stream = new Card_Stream;
//  pCard_IndexScan->vAppend(pCard_Stream);
//
//  Card_StreamElist* pCard_StreamElist = new Card_StreamElist(pStreamDef);
//
//
//  pCard_Stream->vAppend(pCard_StreamElist);
//
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // set the stream order
//
//  Card_ObyElist* pCard_ObyElist = new Card_ObyElist(pIndex);
//  pCard_Stream->vAppend(pCard_ObyElist);
//
//  //bool bUnique = pIndexDef->bIsUnique();
//  //pStreamDef->vUnique(bUnique);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//
//  m_pCard_Board->vInsertBefore(pCard_IndexScan,pReg_Scan);
//
//  m_pCardRegs->vCardReg(CK_INDEXSCAN,pCard_IndexScan);
//
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vCull_Filter(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  Card_Filter* pReg_Filter = _pCard_Filter(m_pCardRegs->pCardReg(CK_FILTER));
//
//  if (! pReg_Filter->bCull())
//    return;
//
//  m_pCard_Board->vRemove(pReg_Filter);
//
//  delete pReg_Filter;
//
//  //vReg_Filter(nullptr);
//  m_pCardRegs->vCardReg(CK_FILTER,nullptr);
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vCut_CandidateIndexes(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  m_pCard_Board->vRemoveSubCards(CK_CANDINDEX);
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vCut_PredCmps(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  Card_Filter* pReg_Filter = _pCard_Filter(m_pCardRegs->pCardReg(CK_FILTER));
//
//  Card_PredCmp* pReg_ebound = m_pCardRegs->pPredCmp_ebound();
//  Card_PredCmp* pReg_lbound = m_pCardRegs->pPredCmp_lbound();
//  Card_PredCmp* pReg_ubound = m_pCardRegs->pPredCmp_ubound();
//
//  if (pReg_ebound)
//    pReg_Filter->vRemoveSubCard(pReg_ebound);
//
//  if (pReg_lbound)
//    pReg_Filter->vRemoveSubCard(pReg_lbound);
//
//  if (pReg_ubound)
//    pReg_Filter->vRemoveSubCard(pReg_ubound);
//
//  m_pCardRegs->vPredCmp_ebound(nullptr);
//  m_pCardRegs->vPredCmp_lbound(nullptr);
//  m_pCardRegs->vPredCmp_ubound(nullptr);
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vCut_Scan(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  Card_Scan* pReg_Scan = _pCard_Scan(m_pCardRegs->pCardReg(CK_SCAN));
//
//
//  m_pCard_Board->vRemove(pReg_Scan);
//
//  m_pCardRegs->vCardReg(CK_SCAN,nullptr);
//  delete pReg_Scan;
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//bool Board::bFind_Scan_AggGby(bool bMark)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  return false;
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//bool Board::bFind_Scan_Filter(bool bMark)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  m_pCardRegs->vCardReg(CK_SCAN,nullptr);
//
//  i8 nMainCards = this->nMainCards();
//  for (i8 i_Main = 0; i_Main < nMainCards-1; i_Main++) {
//    Card* pTestScan = pGetMain(i_Main);
//    if (CK_SCAN != pTestScan->eCard())
//      continue;
//
//    Card* pTestFilter = pGetMain(i_Main + 1);
//    if (CK_FILTER != pTestFilter->eCard())
//      continue;
//
//    if (bMark)
//      //vReg_Scan(_pCard_Scan(pTestScan));
//      m_pCardRegs->vCardReg(CK_SCAN,pTestScan);
//    Card_TablePtr* pCard_TablePtr = _pCard_TablePtr(pTestScan->pFindSubCard(CK_TABLEPTR));
//    m_pCard_Board->vTable(pCard_TablePtr->pTable());
//
//    if (bMark)
//      //vReg_Filter(_pCard_Filter(pTestFilter));
//      m_pCardRegs->vCardReg(CK_FILTER,pTestFilter);
//
//    return true;
//  }
//
//  return false;
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vMark_PredCmps(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  Card_Filter* pReg_Filter = _pCard_Filter(m_pCardRegs->pCardReg(CK_FILTER));
//
//  Card_CandIndex* pCard_CandIndex =
//    _pCard_CandIndex(m_pCardRegs->pCardReg(CK_CANDINDEX));
//
//  i8 iRank = pCard_CandIndex->iRank();
//
//  Index* pIndex = this->m_pCard_Board->pIndex();
//
//  switch (iRank) {
//
//    case 1: {
//      // lower bound or upper bound
//      Card_PredCmp* pPredCmp_lbound =
//        pReg_Filter->pFindBound(pIndex,PB_LBOUND);
//      if (pPredCmp_lbound) {
//        m_pCardRegs->vPredCmp_lbound(pPredCmp_lbound);
//        pPredCmp_lbound->vMark2('L');
//        break;
//      }
//      Card_PredCmp* pPredCmp_ubound =
//        pReg_Filter->pFindBound(pIndex,PB_UBOUND);
//      if (pPredCmp_ubound) {
//        m_pCardRegs->vPredCmp_ubound(pPredCmp_ubound);
//        pPredCmp_ubound->vMark2('U');
//        break;
//      }
//      break;
//    }
//
//    case 2: {
//      // lower bound and upper bound
//      Card_PredCmp* pPredCmp_lbound =
//        pReg_Filter->pFindBound(pIndex,PB_LBOUND);
//      if (pPredCmp_lbound) {
//        m_pCardRegs->vPredCmp_lbound(pPredCmp_lbound);
//        pPredCmp_lbound->vMark2('L');
//      }
//      Card_PredCmp* pPredCmp_ubound =
//        pReg_Filter->pFindBound(pIndex,PB_UBOUND);
//      if (pPredCmp_ubound) {
//        m_pCardRegs->vPredCmp_ubound(pPredCmp_ubound);
//        pPredCmp_ubound->vMark2('U');
//      }
//      break;
//    }
//
//    case 3: {
//      // equi-bound
//      Card_PredCmp* pPredCmp_ebound =
//        pReg_Filter->pFindBound(pIndex,PB_EBOUND);
//      if (pPredCmp_ebound) {
//        m_pCardRegs->vPredCmp_ebound(pPredCmp_ebound);
//        pPredCmp_ebound->vMark2('E');
//      }
//      break;
//    }
//
//    default:
//      vTrace("SWITCH %s: Unknown iRank %lld\n"_Stz
//             ,__func__
//             ,(i8)iRank
//              );
//      RASSERT(0);
//      break;
//
//  }
//}

/*----------------------------------------------------------------------------*/
/* order the projective stream defs which are downstream of indexScan         */
/*----------------------------------------------------------------------------*/
//void Board::vOrder_ProjectiveStreamDefs(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  Card_IndexScan* pReg_IndexScan
//    = _pCard_IndexScan(m_pCardRegs->pCardReg(CK_INDEXSCAN));
//
//  Card_Stream* pCard_Stream_src =
//    _pCard_Stream(pReg_IndexScan->pFindSubCard(CK_STREAM));
//
//  i8 nMainCards = this->nMainCards();
//  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
//    Card* pMainCard = pGetMain(i_Main);
//    if (CK_PROJECT != pMainCard->eCard())
//      continue;
//
//    Card_Project* pCard_Project = _pCard_Project(pMainCard);
//
//    //--- --- ---
//
//    if (! pCard_Stream_src)
//      continue;
//
//    Card_Stream* pCard_Stream_dst =
//      _pCard_Stream(pCard_Project->pFindSubCard(CK_STREAM));
//    if (! pCard_Stream_dst)
//      continue;
//
//    pCard_Stream_dst->vApplyOrder(pCard_Stream_src);
//
//    //--- --- ---
//
//  }
//
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//bool Board::bPick_CandidateIndex(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  i8 iBestRank = C_UNKNOWN;
//  Card_CandIndex* pCard_CandIndex = nullptr;
//
//  i8 nMainCards = this->nMainCards();
//  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
//    Card* pMainCard = pGetMain(i_Main);
//    if (CK_CANDINDEX != pMainCard->eCard())
//      continue;
//
//    Card_CandIndex* pCadIdx = _pCard_CandIndex(pMainCard);
//
//    i8 iRank = pCadIdx->iRank();
//    if (iRank > iBestRank) {
//      iBestRank = iRank;
//      pCard_CandIndex = pCadIdx;
//    }
//  }
//
//  if (! pCard_CandIndex) {
//    //vReg_CandIndex(nullptr);
//    m_pCardRegs->vCardReg(CK_CANDINDEX,nullptr);
//    return false;
//  }
//
//  //vReg_CandIndex(pCard_CandIndex);
//  m_pCardRegs->vCardReg(CK_CANDINDEX,pCard_CandIndex);
//  m_pCard_Board->vIndex(pCard_CandIndex->pFindSubCard(CK_INDEXPTR)->pIndex());
//
//  return true;
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vRank_CandidateIndexes(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  //m_pFilter->vMark2('0');
//
//  i8 nMainCards = this->nMainCards();
//  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
//    Card* pMainCard = pGetMain(i_Main);
//    if (CK_CANDINDEX != pMainCard->eCard())
//      continue;
//
//    Card_CandIndex* pCard_CandIndex = _pCard_CandIndex(pMainCard);
//
//    vRankCandIndex(pCard_CandIndex);
//  }
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vReduce_Filter(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  Card_Filter* pReg_Filter = _pCard_Filter(m_pCardRegs->pCardReg(CK_FILTER));
//
//  if (! pReg_Filter)
//    return;
//
//  pReg_Filter->vReduceFilter();
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vResolve_Filters(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  i8 nMainCards = this->nMainCards();
//  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
//    Card* pMainCard = pGetMain(i_Main);
//
//    if (CK_FILTER != pMainCard->eCard())
//      continue;
//
//    vResolveFilter(_pCard_Filter(pMainCard));
//  }
//
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vSimplify_Initial(void)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);
//
//  i8 i_Test;
//
//  i8 nMainCards = this->nMainCards();
//  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
//    Card *pMainCard = pGetMain(i_Main);
//
//    // of stage 1
//    i8 i_Test = i_Main;
//    Card* pStage_1 = pGetMain(i_Test++);
//    if (CK_STAGE != pStage_1->eCard())
//      continue;
//
//    Card* pScan_1 = pGetMain(i_Test++);
//    if (CK_SCAN != pScan_1->eCard())
//      continue;
//
//    Card* pSend_1 = pGetMain(i_Test++);
//    if (CK_SEND != pSend_1->eCard())
//      continue;
//
//    // of stage 2
//    Card* pStage_2 = pGetMain(i_Test++);
//    if (CK_STAGE != pStage_2->eCard())
//      continue;
//
//    Card* pScan_2 = pGetMain(i_Test++);
//    if (CK_SCAN != pScan_2->eCard())
//      continue;
//
//    Card* pAbsJoin_2 = pGetMain(i_Test++);
//    if (CK_ABSJOIN != pAbsJoin_2->eCard())
//      continue;
//
//    Card* pSend_2 = pGetMain(i_Test++);
//    if (CK_SEND != pSend_2->eCard())
//      continue;
//
//    // of stage 3
//    Card* pStage_3 = pGetMain(i_Test++);
//    if (CK_STAGE != pStage_3->eCard())
//      continue;
//
//    Card* pFilter_3 = pGetMain(i_Test++);
//    if (CK_FILTER != pFilter_3->eCard())
//      continue;
//
//    Card* pProject_3 = pGetMain(i_Test++);
//    if (CK_PROJECT != pProject_3->eCard())
//      continue;
//
//    Card* pCombine_3 = pGetMain(i_Test++);
//    if (CK_COMBINED != pCombine_3->eCard())
//      continue;
//
//    Card* pSend_3 = pGetMain(i_Test++);
//    if (CK_SEND != pSend_3->eCard())
//      continue;
//
//    m_pCard_Board->vRemove(pSend_1);
//    m_pCard_Board->vRemove(pStage_2);
//    m_pCard_Board->vRemove(pSend_2);
//    m_pCard_Board->vRemove(pStage_3);
//    m_pCard_Board->vRemove(pCombine_3);
//
//    delete pSend_1;
//    delete pStage_2;
//    delete pSend_2;
//    delete pStage_3;
//    delete pCombine_3;
//
//    //m_pCard_Board->vRemove(pFilter_3);
//    //m_pCard_Board->vInsertAfter(pFilter_3,pScan_2);
//
//    break;
//  }
//
//}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vResolve_RootStreams(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  i8 nMainCards = this->nMainCards();
//  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
//    Card* pMainCard = pGetMain(i_Main);
//
//    Card* pCard_Table = _pCard_Table(pMainCard->pFindSubCard(CK_TABLEPTR));
//    if (! pCard_Table)
//      continue;
//
//    StreamDef* pStreamDef = pCard_Table->pTable()->pStreamDef();
//    Card_StreamElist* pCard_StreamElist = new Card_StreamElist(pStreamDef);
//
//    Card_Stream* pCard_Stream = new Card_Stream;
//    pCard_Stream->vAppend(pCard_StreamElist);
//
//    pMainCard->vAppend(pCard_Stream);
//  }
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vResolve_Symbols(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  i8 nMainCards = this->nMainCards();
//  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
//    Card* pMainCard = pGetMain(i_Main);
//
//    vResolveSymbolTerms(pMainCard);
//  }
//
//}
/*----------------------------------------------------------------------------*/
//void Board::vResolve_Symbols(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  i8 nMainCards = this->nMainCards();
//  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
//    Card* pMainCard = pGetMain(i_Main);
//
//    vTrace("Resolving MainCard %lld\n"_Stz,i_Main);
//    vResolveRecursive(pMainCard);
//
//    //vPrintBoard();
//
//    //pMainCard->vRenumberSubCards(nullptr);
//  }
//
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vAResolve_Joins(void)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);
//
//  i8 nMainCards = this->nMainCards();
//  for (i8 i_Main = 0; i_Main < nMainCards; i_Main++) {
//    Card* pMainCard = pGetMain(i_Main);
//
//    if (CK_INDEXJOIN != pMainCard->eCard())
//      continue;
//
//    Card_IndexJoin* pCard_IndexJoin = _pCard_IndexJoin(pMainCard);
//    vAResolveJoin(pCard_IndexJoin);
//  }
//}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
