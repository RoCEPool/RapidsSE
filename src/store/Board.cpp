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
Board::Board(Stz* szQueryNm, Optimiser* pOptimiser)
//: Card_OTHER(CK_BOARD)
: Object(CN_BOARD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
      
  m_sQueryNm   = szQueryNm;
  m_pOptimiser = pOptimiser;
   
  m_pCardRegs  = new CardRegs;
  m_pRuleCtl   = new RuleCtl;

  m_pCard_SubSet = new Card_SubSet;

  //_checkInitialised_(CN_BOARD);
  _checkInitialisedSize_(sizeof(Board));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Board::~Board(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  delete m_pCardRegs;
  delete m_pRuleCtl;

  delete m_pCard_SubSet;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get card registers                                                         */
/*----------------------------------------------------------------------------*/
CardRegs* Board::pCardRegs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  return m_pCardRegs;
}

/*----------------------------------------------------------------------------*/
/* clear the board                                                            */
/*----------------------------------------------------------------------------*/
void Board::vClear(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  m_pCardRegs->vResetCardRegs();
  
  vDeleteAllCards();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDeleteAllCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  while (Card* pCard = m_pCard_SubSet->pPop())
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
    Card* pMainCard = m_pCard_SubSet->pGet(i_Main);
    if (pMainCard == pFrom)
      break;
  }

  if (i_Main >= nMainCards)
    return;

  for (;;) {

    nMainCards = this->nMainCards();
    if (i_Main >= nMainCards)
      break;

    Card* pDeleteCard = m_pCard_SubSet->pGet(i_Main);

    if (pDeleteCard == pTillBefore)
      break;

    m_pCard_SubSet->vRemove(pDeleteCard);
    delete pDeleteCard;
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Board::pGetMainCard(i8 i_MainCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Card* pMainCard = m_pCard_SubSet->pGet(i_MainCard);
  return pMainCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Board::pGetMainCard(i8 i_MainCard, eCK& reCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Card* pMainCard = m_pCard_SubSet->pGet(i_MainCard);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
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
/* find next main card after given arg which is an eFindCard                  */
/*----------------------------------------------------------------------------*/
Card* Board::pFindNextMain(Card* pAfter, eCK eFindCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Card* pMainCard = nullptr;

  i8 i_MainCard = -1;
  i8 nMainCards = this->nMainCards();
  for (i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
    pMainCard = pGetMainCard(i_MainCard);

    if (pMainCard == pAfter)
      break;
  }

  if (pMainCard != pAfter)
    return nullptr;

  // found after

  // get next eCard
  for (i8 i_TestCard = i_MainCard+1; i_TestCard < nMainCards; i_TestCard++) {
    Card* pTestCard = pGetMainCard(i_TestCard);

    if (eFindCard == pTestCard->eCard())
      return pTestCard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get number of cards                                                        */
/*----------------------------------------------------------------------------*/
i8 Board::nMainCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  i8 nCards = this->m_pCard_SubSet->nItems();
  
  return nCards;
}

/*----------------------------------------------------------------------------*/
/* pin board onto given ribbon                                                */
/*----------------------------------------------------------------------------*/
void Board::vPinCards(Ribbon* pRibbon, i8 i_Board)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  if (! nMainCards())
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pRibbon->vSprint("[%lld] %s"_Stz,i_Board,szQueryNm());
  pRibbon->vTitleLines(1);
  pRibbon->vSetBase();
  pRibbon->vFirstColumn();

  Ribbon* pSubRibbon = new Ribbon(m_pOptimiser->pCardCtl(),this,0);
  

  ListExoIter2_t<Card>* pBoard_I = m_pCard_SubSet->pNewIter();

  i8 nCount = 0;
  while (pBoard_I->bHasNext()) {
    Card* pCard = pBoard_I->pNext();

    //pCard->vDump();
    
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

  delete pBoard_I;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vPrintBoard(Stz* szCue)
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
  
  Ribbon* pRibbon = new Ribbon(m_pOptimiser->pCardCtl(),this,0);

  vPinCards(pRibbon,0);
  pRibbon->vPrintRibbon();

  delete pRibbon;
  
  if (! bCue)
    return;
  if (! szCue)
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

  for (i8 i_MainCard = i_PriorToMain - 1; i_MainCard >= 0; i_MainCard--) {
    rpMainCard = pGetMainCard(i_MainCard);

    Card* pSubCard;
    if (pSubCard = rpMainCard->pFindSubCard(eSubCard))
      return pSubCard;
  }

  rpMainCard = nullptr;
  return nullptr;
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
/* renumber the cards                                                         */
/*----------------------------------------------------------------------------*/
void Board::vRenumberMainCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  ListExoIter2_t<Card>* pMainCard_I = m_pCard_SubSet->pNewIter();

  i8 iMainCardNo = 0;
  while (pMainCard_I->bHasNext()) {
    Card* pMainCard = pMainCard_I->pNext();

    pMainCard->vCardNo(iMainCardNo++);
    pMainCard->vRenumberSubCards(nullptr);
  }

  delete pMainCard_I;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Board::pSearch(Stz* szCardNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
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
void Board::vResolveFilter(Card_Filter* pCard_Filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  RASSERT(1 == pCard_Filter->nSubCards());

  Card_Expr* pCard_Expr = _pCard_Expr(pCard_Filter->pGet(0));
  RASSERT(2 == pCard_Expr->nSubCards());

  Card_Type* pCard_Type = _pCard_Type(pCard_Expr->pGet(0));
  Card_Calc* pCard_Calc = _pCard_Calc(pCard_Expr->pGet(1));

  Card_CalcBool* pCard_CalcBool = pTo_CalcBool(pCard_Calc);

  pCard_Expr->vRemove(pCard_Calc);
  delete pCard_Calc;
  pCard_Expr->vAppend(pCard_CalcBool);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vResolveJoin(Card_AbsJoin* pCard_AbsJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  Card_Scan* pCard_Scan = _pCard_Scan(pGetMainCard(pCard_AbsJoin->iCardNo() - 1));
  Card_Table* pCard_Table = _pCard_Table(pCard_Scan->pFindSubCard(CK_TABLE));
  Table* pTable = pCard_Table->pTable();

  Index* pIndex = pTable->pIdxVec()->pGet(0);

  Card_IndexJoin* pIndexJoin = new Card_IndexJoin;
  pIndexJoin->vAppend(new Card_Table(pTable));
  pIndexJoin->vAppend(new Card_Index(pIndex));

  Card_Filter* pCard_Filter = _pCard_Filter(pGetMainCard(pCard_AbsJoin->iCardNo() + 3));
  pCard_Filter->vDump();

  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;
  pIndexJoin->vAppend(pCard_IndexPred);

  pCard_IndexPred->vAppend(new Card_Term("BOUND"_Stz));
  pCard_IndexPred->vAppend(new Card_Term("$I1"_Stz));
  pCard_IndexPred->vAppend(new Card_Term("$L1"_Stz));
  pCard_IndexPred->vAppend(new Card_Term("=="_Stz));

  m_pCard_SubSet->vInsertAfter(pIndexJoin,pCard_AbsJoin);

  Card_Combine* pCard_Combine = new Card_Combine;
  m_pCard_SubSet->vInsertAfter(pCard_Combine,pIndexJoin);


  // temp hack

  Card_Stage* pCard_Stage_next =
    _pCard_Stage(pFindNextMain(pIndexJoin,CK_STAGE));
  if (! pCard_Stage_next)
    return;

  Card_Stage* pCard_Stage_following =
    _pCard_Stage(pFindNextMain(pCard_Stage_next,CK_STAGE));

  vDeleteFromTill(pCard_Stage_next,pCard_Stage_following);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vResolveJoin2(Card_AbsJoin* pCard_AbsJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  Card_Scan* pCard_Scan = _pCard_Scan(pGetMainCard(pCard_AbsJoin->iCardNo() - 1));
  Card_Table* pCard_Table = _pCard_Table(pCard_Scan->pFindSubCard(CK_TABLE));
  Table* pTable = pCard_Table->pTable();

  Index* pIndex = pTable->pIdxVec()->pGet(0);

  Card_IndexJoin* pIndexJoin = new Card_IndexJoin;
  pIndexJoin->vAppend(new Card_Table(pTable));
  pIndexJoin->vAppend(new Card_Index(pIndex));

  Card_Filter* pCard_Filter = _pCard_Filter(pGetMainCard(pCard_AbsJoin->iCardNo() + 3));
  //pCard_Filter->vDump();

  Card_Expr* pCard_Expr = _pCard_Expr(pCard_Filter->pGetSubCard(0));

  Card_Type* pCard_Type = _pCard_Type(pCard_Expr->pGetSubCard(0));
  Card_CalcBool* pCard_CalcBool = _pCard_CalcBool(pCard_Expr->pGetSubCard(1));
  Card_PredCmp* pCard_PredCmp = _pCard_PredCmp(pCard_CalcBool->pGetSubCard(0));

  Card_IndexPred* pCard_IndexPred = pTo_IndexPred(pCard_PredCmp);
  pIndexJoin->vAppend(pCard_IndexPred);

  m_pCard_SubSet->vInsertAfter(pIndexJoin,pCard_AbsJoin);

  Card_Combine* pCard_Combine = new Card_Combine;
  m_pCard_SubSet->vInsertAfter(pCard_Combine,pIndexJoin);

  //delete pCard_PredCmp;

  // temp hack

  Card_Stage* pCard_Stage_next =
          _pCard_Stage(pFindNextMain(pIndexJoin,CK_STAGE));
  if (! pCard_Stage_next)
    return;

  Card_Stage* pCard_Stage_following =
          _pCard_Stage(pFindNextMain(pCard_Stage_next,CK_STAGE));

  vDeleteFromTill(pCard_Stage_next,pCard_Stage_following);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vConvertJoin(Card_AbsJoin* pCard_AbsJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  m_pCard_SubSet->vRemove(pCard_AbsJoin);

  Card_Scan* pCard_Scan = _pCard_Scan(pGetMainCard(pCard_AbsJoin->iCardNo() - 1));
  Card_Table* pCard_Table = _pCard_Table(pCard_Scan->pFindSubCard(CK_TABLE));
  Table* pTable = pCard_Table->pTable();


  Card_IndexJoin* pIndexJoin = new Card_IndexJoin;
  pIndexJoin->vAppend(new Card_Table(pTable));

  // TODO:
  Index* pIndex = pTable->pIdxVec()->pGet(0);
  pIndexJoin->vAppend(new Card_Index(pIndex));

  m_pCard_SubSet->vInsertAfter(pIndexJoin,pCard_Scan);

  m_pCard_SubSet->vRemove(pCard_Scan);
  delete pCard_Scan;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vResolveRecursive(Card* pAnyCard) // recursive
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  eCK eCard = pAnyCard->eCard();

  //
  // leaves
  //

  if (CK_INDEX == eCard) {
    m_pCard_SubSet->vIndex(pAnyCard->pIndex());
    return;
  }

  if (CK_STREAM == eCard) {
    m_pCardRegs->vCardReg(CK_STREAM, pAnyCard);
    return;
  }

  if (CK_TABLE == eCard) {
    m_pCard_SubSet->vTable(pAnyCard->pTable());
    return;
  }

  if (CK_TERM == eCard) {
    vResolveTerm(_pCard_Term(pAnyCard));
    return;
  }


  //
  // subcards first
  //

  //pCard->vDump();

  i8 nSubCards = pAnyCard->nSubCards();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card *pSubCard = pAnyCard->pGetSubCard(i_SubCard);
    vResolveRecursive(pSubCard);
  }

  //
  // this card
  //

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggElist

  Card_AggElist *pCard_AggElist = _pCard_AggElist(pAnyCard->pFindChildCard(CK_AGGELIST));
  if (pCard_AggElist) {

    Card_Stream *pCard_Stream = new Card_Stream;
    pAnyCard->vAppend(pCard_Stream);

    m_pCardRegs->vCardReg(CK_STREAM, pCard_Stream);

    Card_StreamColList *pCard_StreamColList = new Card_StreamColList(pCard_AggElist);
    pCard_Stream->vAppend(pCard_StreamColList);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // projElist

  // derive the stream definition for projection
  Card_ProjElist *pCard_ProjElist = _pCard_ProjElist(pAnyCard->pFindChildCard(CK_PROJELIST));
  if (pCard_ProjElist) {

    Card_Stream *pCard_Stream = new Card_Stream;
    pAnyCard->vAppend(pCard_Stream);

    m_pCardRegs->vCardReg(CK_STREAM, pCard_Stream);

    Card_StreamColList *pCard_StreamColList = pCard_ProjElist->pDerive_StreamColList();
    pCard_Stream->vAppend(pCard_StreamColList);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  pCard_Index = _pCard_Index(pAnyCard->pFindChildCard(CK_INDEX));
//  if (pCard_Index) {
//    Index* pIndex = pCard_Index->pIndex();
//    m_pCard_SubSet->vIndex(pIndex);
//  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // scan

  if (CK_SCAN == eCard) {
    Card_Table *pCard_Table = _pCard_Table(pAnyCard->pFindChildCard(CK_TABLE));
    if (pCard_Table) {

      StreamDef *pStreamDef = pCard_Table->pTable()->pStreamDef();

      Card_Stream *pCard_Stream = new Card_Stream;
      pAnyCard->vAppend(pCard_Stream);

      m_pCardRegs->vCardReg(CK_STREAM, pCard_Stream);

      Card_StreamColList *pCard_StreamColList = new Card_StreamColList(pStreamDef);
      pCard_Stream->vAppend(pCard_StreamColList);
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check type and as for expr

  if (CK_EXPR == eCard) {
    Card_Expr *pCard_Expr = _pCard_Expr(pAnyCard);

    Card_As   *pCard_As   = _pCard_As (pCard_Expr->pFindChildCard(CK_AS));
    Card_Type *pCard_Type = _pCard_Type(pCard_Expr->pFindChildCard(CK_TYPE));

    if (pCard_As && pCard_Type) {
      pCard_Expr->vRemove(pCard_As);
      pCard_Expr->vInsertAfter(pCard_As,pCard_Type);
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // absJoin

  if (CK_ABSJOIN == eCard) {
    Card_AbsJoin* pCard_AbsJoin = _pCard_AbsJoin(pAnyCard);

    Card* pPriorMain;
    Card_Stream* pCard_StreamB = _pCard_Stream(pPriorSubCard(pPriorMain,
                                                             CK_STREAM,
                                                             pCard_AbsJoin->iCardNo()));
    if (pCard_StreamB) {
      Card_Stream* pCard_StreamA = _pCard_Stream(pPriorSubCard(pPriorMain,
                                                               CK_STREAM,
                                                               pPriorMain->iCardNo()));

      if (pCard_StreamA) {
        Card_StreamColList* pCard_StreamColListA =
                _pCard_StreamColList(pCard_StreamA->pFindSubCard(CK_STREAMCOLLIST));
        Card_StreamColList* pCard_StreamColListB =
                _pCard_StreamColList(pCard_StreamB->pFindSubCard(CK_STREAMCOLLIST));

        Card_Stream *pCard_Stream = new Card_Stream;
        pAnyCard->vAppend(pCard_Stream);
        m_pCardRegs->vCardReg(CK_STREAM, pCard_Stream);

        Card_StreamColList* pCard_StreamColList = new Card_StreamColList;
        pCard_Stream->vAppend(pCard_StreamColList);

        //pCard_StreamColList->vAppend(pCard_StreamColListA->pDupSubCards());
        //pCard_StreamColList->vAppend(pCard_StreamColListB->pDupSubCards());

        pCard_StreamColList->vAppendDupSubCards(pCard_StreamColListA);
        pCard_StreamColList->vAppendDupSubCards(pCard_StreamColListB);

        pCard_StreamColListA->vDump();
        pCard_StreamColListB->vDump();
        pCard_Stream->vDump();
        vDebugPoint();
      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // indexJoin

  if (CK_INDEXJOIN == eCard) {
    Card_IndexJoin *pCard_IndexJoin = _pCard_IndexJoin(pAnyCard);

    Card* pCard_mainPrior;
    Card_Stream* pCard_Stream_prior = _pCard_Stream(pPriorSubCard(pCard_mainPrior,
                                                                   CK_STREAM,
                                                                pCard_IndexJoin->iCardNo()));

    if (pCard_Stream_prior) {
      Card_StreamColList *pCard_StreamColList_prior =
              _pCard_StreamColList(pCard_Stream_prior->pFindSubCard(CK_STREAMCOLLIST));

      Card_Stream *pCard_Stream_join = new Card_Stream;
      m_pCardRegs->vCardReg(CK_STREAM, pCard_Stream_join);

      pAnyCard->vAppend(pCard_Stream_join);
      m_pCardRegs->vCardReg(CK_STREAM, pCard_Stream_join);

      Card_StreamColList* pCard_StreamColList_join = new Card_StreamColList;

      pCard_StreamColList_join->vAppendDupSubCards(pCard_StreamColList_prior);

      pCard_Stream_join->vAppend(pCard_StreamColList_join);



      Card_Table *pCard_Table_right = _pCard_Table(pAnyCard->pFindChildCard(CK_TABLE));
      if (pCard_Table_right) {

        StreamDef *pStreamDef_right = pCard_Table_right->pTable()->pStreamDef();

        Card_StreamColList *pCard_StreamColList_right =
          new Card_StreamColList(pStreamDef_right,"J1_"_Stz);

        pCard_StreamColList_join->vAppendDupSubCards(pCard_StreamColList_right);
        delete pCard_StreamColList_right;
      }

    }

  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vResolveTerm(Card_Term* pCard_Term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  
  Card_Stream* pReg_Stream = _pCard_Stream(m_pCardRegs->pCardReg(CK_STREAM));

  if (pCard_Term->bIsOrdinalStreamTerm()) {

    i8 i_Ord = pCard_Term->iOrdinalStreamColNo();
    i8 i_Col = i_Ord - 1;

    Card_StreamColList* pCard_StreamColList =
            _pCard_StreamColList(pReg_Stream->pFindSubCard(CK_STREAMCOLLIST));

    Card_StreamCol *pCard_StreamCol = _pCard_StreamCol(pCard_StreamColList->pGet(i_Col));

    Card_As *pCard_As = _pCard_As(pCard_StreamCol->pFindSubCard(CK_AS));

    Stz *szColNm_resolved = pCard_As->szAs();

    pCard_Term->vTerm(szColNm_resolved);

    vTrace("Resolved $%lld To: %s\n"_Stz,i_Ord,szColNm_resolved);

    return;
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Index* pIndex = this->m_pCard_SubSet->pIndex();
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
 
  Card_Scan* pReg_Scan = _pCard_Scan(m_pCardRegs->pCardReg(CK_SCAN));

  Table* pTable = this->m_pCard_SubSet->pTable();

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
  
  i8 i_Filter = m_pCard_SubSet->i_Find(pReg_Scan) + 1;
  Card_Filter* pCard_Filter = _pCard_Filter(m_pCard_SubSet->pGet(i_Filter));

  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    Index* pIndex = pIdxVec->pGet(i_Index);
    //vTrace("Index %s\n"_Stz,pIndex->szIndexNm());
    
    Card_CandIndex* pCard_CandIndex = new Card_CandIndex;
    Card_Index* pCard_Index = new Card_Index(pIndex);
    pCard_CandIndex->vAppend(pCard_Index);

    m_pCard_SubSet->vInsertAfter(pCard_CandIndex,pCard_Filter);
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
  Card_Scan* pReg_Scan = _pCard_Scan(m_pCardRegs->pCardReg(CK_SCAN));

  Card_PredCmp* pReg_ebound = m_pCardRegs->pPredCmp_ebound();
  Card_PredCmp* pReg_lbound = m_pCardRegs->pPredCmp_lbound();
  Card_PredCmp* pReg_ubound = m_pCardRegs->pPredCmp_ubound();
  
  Index* pIndex = this->m_pCard_SubSet->pIndex();
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


  m_pCard_SubSet->vInsertBefore(pCard_IndexScan,pReg_Scan);
  
  m_pCardRegs->vCardReg(CK_INDEXSCAN,pCard_IndexScan);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCull_Filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  Card_Filter* pReg_Filter = _pCard_Filter(m_pCardRegs->pCardReg(CK_FILTER));

  if (! pReg_Filter->bCull())
    return;

  m_pCard_SubSet->vRemove(pReg_Filter);
  
  delete pReg_Filter;

  //vReg_Filter(nullptr);
  m_pCardRegs->vCardReg(CK_FILTER,nullptr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCut_CandidateIndexes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_pCard_SubSet->vRemoveSubCards(CK_CANDINDEX);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCut_PredCmps(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  Card_Scan* pReg_Scan = _pCard_Scan(m_pCardRegs->pCardReg(CK_SCAN));


  m_pCard_SubSet->vRemove(pReg_Scan);
  
  m_pCardRegs->vCardReg(CK_SCAN,nullptr);
  delete pReg_Scan;
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
    m_pCard_SubSet->vTable(pCard_Table->pTable());
    
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  Card_Filter* pReg_Filter = _pCard_Filter(m_pCardRegs->pCardReg(CK_FILTER));

  Card_CandIndex* pCard_CandIndex =
    _pCard_CandIndex(m_pCardRegs->pCardReg(CK_CANDINDEX));

  i8 iRank = pCard_CandIndex->iRank();
 
  Index* pIndex = this->m_pCard_SubSet->pIndex();

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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

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
  m_pCard_SubSet->vIndex(pCard_CandIndex->pFindSubCard(CK_INDEX)->pIndex());

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vRank_CandidateIndexes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  Card_Filter* pReg_Filter = _pCard_Filter(m_pCardRegs->pCardReg(CK_FILTER));

  if (! pReg_Filter)
    return;
  
  pReg_Filter->vReduceFilter();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vResolve_Filters(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  i8 nMainCards = this->nMainCards();
  for (i8 i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
    Card* pMainCard = pGetMainCard(i_MainCard);

    if (CK_FILTER != pMainCard->eCard())
      continue;

    vResolveFilter(_pCard_Filter(pMainCard));
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vResolve_Joins(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  i8 nMainCards = this->nMainCards();
  for (i8 i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
    Card* pMainCard = pGetMainCard(i_MainCard);

    if (CK_ABSJOIN != pMainCard->eCard())
      continue;

    vResolveJoin(_pCard_AbsJoin(pMainCard));

    Card_Scan* pCard_Scan = _pCard_Scan(pGetMainCard(i_MainCard - 1));
    m_pCard_SubSet->vRemove(pCard_Scan);
    delete pCard_Scan;
    break;
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vResolve_Joins2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  i8 nMainCards = this->nMainCards();
  for (i8 i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
    Card* pMainCard = pGetMainCard(i_MainCard);

    if (CK_ABSJOIN != pMainCard->eCard())
      continue;

    Card_AbsJoin* pCard_AbsJoin = _pCard_AbsJoin(pMainCard);
    vResolveJoin2(pCard_AbsJoin);

    Card_Scan* pCard_Scan = _pCard_Scan(pGetMainCard(i_MainCard - 1));
    m_pCard_SubSet->vRemove(pCard_Scan);
    delete pCard_Scan;

    m_pCard_SubSet->vRemove(pCard_AbsJoin);
    delete pCard_AbsJoin;

    break;
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vResolve_Joins3(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vConvert_Joins(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  i8 nMainCards = this->nMainCards();
  for (i8 i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
    Card* pMainCard = pGetMainCard(i_MainCard);

    if (CK_ABSJOIN != pMainCard->eCard())
      continue;

    Card_AbsJoin* pCard_AbsJoin = _pCard_AbsJoin(pMainCard);
    vConvertJoin(pCard_AbsJoin);
    i_MainCard--;

    vRenumberMainCards();

    break;
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
//void Board::vResolve_RootStreams(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
//  i8 nMainCards = this->nMainCards();
//  for (i8 i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
//    Card* pMainCard = pGetMainCard(i_MainCard);
//
//    Card* pCard_Table = _pCard_Table(pMainCard->pFindSubCard(CK_TABLE));
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
//  for (i8 i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
//    Card* pMainCard = pGetMainCard(i_MainCard);
//
//    vResolveSymbolTerms(pMainCard);
//  }
//
//}
/*----------------------------------------------------------------------------*/
void Board::vResolve_Symbols(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  i8 nMainCards = this->nMainCards();
  for (i8 i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
    Card* pMainCard = pGetMainCard(i_MainCard);

    vTrace("Resolving MainCard %lld\n"_Stz,i_MainCard);
    vResolveRecursive(pMainCard);

    //vPrintBoard();

    //pMainCard->vRenumberSubCards(nullptr);
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

#ifdef RULE_NO
  #undef RULE_NO
#endif
  
#define RULE_NO 5

  return false;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert a calculation into a boolean calculaiton                           */
/*----------------------------------------------------------------------------*/
Card_CalcBool* Board::pTo_CalcBool(Card_Calc* pCard_Calc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  Card_PredAnd* pCard_PredAnd = nullptr;
  Card_PredCmp* pCard_PredCmp = nullptr;
  Card_PredOr*  pCard_PredOr  = nullptr;

  Card_SubSet*  pCard_SubExpr = new Card_SubSet;

  Card_CalcBool* pCard_CalcBool = new Card_CalcBool;

  while (pCard_Calc->nSubCards()) {
    Card_Term *pCard_Term = _pCard_Term(pCard_Calc->pPop());

    eTE eTopTerm = pCard_Term->eTermEval();
    switch (eTopTerm) {

      case TE_AND:
        RASSERT(2 == pCard_CalcBool->nSubCards());
        pCard_PredAnd = new Card_PredAnd;
        pCard_PredAnd->vTransferAll(pCard_CalcBool);
        RASSERT(0 == pCard_CalcBool->nSubCards());
        pCard_CalcBool->vAppend(pCard_PredAnd);
        RASSERT(1 == pCard_CalcBool->nSubCards());
        pCard_PredAnd = nullptr;
        delete pCard_Term;
        break;

      case TE_OR:
        RASSERT(2 == pCard_CalcBool->nSubCards());
        pCard_PredOr = new Card_PredOr;
        pCard_PredOr->vTransferAll(pCard_CalcBool);
        RASSERT(0 == pCard_CalcBool->nSubCards());
        pCard_CalcBool->vAppend(pCard_PredOr);
        RASSERT(1 == pCard_CalcBool->nSubCards());
        pCard_PredOr = nullptr;
        delete pCard_Term;
        break;

      case TE_COMPARE:
        RASSERT(pCard_SubExpr->nSubCards() > 0);
        pCard_PredCmp = new Card_PredCmp;
        pCard_PredCmp->vCmp(pCard_Term->szTerm());
        pCard_PredCmp->vTransferAll(pCard_SubExpr);
        pCard_CalcBool->vAppend(pCard_PredCmp);
        pCard_PredCmp = nullptr;
        delete pCard_Term;
        break;

      case TE_OTHER:
        pCard_SubExpr->vAppend(pCard_Term);
        break;

        // - - - - - - -

      case TE_BOOLEAN:
        NEVER_REACH;
        break;

      case TE_TYPE:
        NEVER_REACH;
        break;

      case TE_AS:
        NEVER_REACH;
        break;
    }
  }

  if (pCard_SubExpr->nItems()) {
    RASSERT(0 == pCard_CalcBool->nSubCards());
    pCard_PredCmp->vTransferAll(pCard_SubExpr);
    RASSERT(1 == pCard_CalcBool->nSubCards());
    RASSERT(0 == pCard_SubExpr->nSubCards());
  }

  delete pCard_SubExpr;

  return pCard_CalcBool;
}

/*----------------------------------------------------------------------------*/
/* convert a calculation into a boolean calculaiton                           */
/*----------------------------------------------------------------------------*/
Card_IndexPred* Board::pTo_IndexPred(Card_PredCmp* pCard_PredCmp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;

  pCard_IndexPred->vAppend(new Card_Term("BOUND"_Stz));
  pCard_IndexPred->vAppend(new Card_Term("$I1"_Stz));
  pCard_IndexPred->vAppend(new Card_Term("$L1"_Stz));
  pCard_IndexPred->vAppend(new Card_Term("=="_Stz));

  return pCard_IndexPred;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
