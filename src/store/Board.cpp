/*============================================================================*/
/* Board.cpp -- Board                                                         */
/*============================================================================*/

#include "Board.hpp"

#include "PtnDef.hpp"
#include "Cue.hpp"

#include "Optimiser.hpp"
#include "OptimiserFun.hpp"
#include "OptimiserCatalog.hpp"

#include "RelOp.hpp"
#include "Error.hpp"
#include "CardIter.hpp"

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
Board::Board(i8 i_Board, eBK eBoard, Stz* szSqyNm, Bench* pBench)
: Object(CN_BOARD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_i_Board = i_Board;
  m_eBoard  = eBoard;
  m_ePhase  = OH_NONE;
  m_pBench  = pBench;

  m_pSubQuery = new SubQuery(i_Board, szSqyNm, this);
  m_pDeck     = new Card_Deck;
  m_pState    = new TagSet;
  m_pMark     = new Mark;

  _checkInitialised_(CN_BOARD);

  if (BK_JOINORDERING == eBoard)
    m_pMark->vChrSeq("JO"_Stz);
  else
    m_pMark->vSprint("SQ%lld"_Stz,i_Board);

  pBench->vAddBoard(this);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Board::~Board(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  delete m_pSubQuery;
  delete m_pDeck;
  delete m_pState;
  delete m_pMark;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_foldProjection_F(CardIter* pCI)
{
  _static_


  pCI->E.p_emit = nullptr;
  for (i8 i_Main = pCI->Flat.m_i_fold - 1; i_Main >= 0; i_Main--) {
    Card *pMain = pCI->E.p_deck->pGetChild(i_Main);
    if ((pCI->E.p_emit = pMain->pSub_emit()))
      break;
  }
  if (! pCI->E.p_emit)
    return;

  pCI->E.p_stream = new Card_Stream;
  pCI->E.p_fold->vAppendChild(pCI->E.p_stream);
  pCI->E.p_stream->vAppendChild(pCI->E.p_emit->pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/* find and mark possible call to subquery referenced in pCI->E.p_boolPred    */
/*----------------------------------------------------------------------------*/
bool Board::bMark_sqyCall(CardIter* pCI)
{
  _static_

  bool bFoundCall = false;

  Card_CONJ* p_conj_ROOT = pCI->E.p_boolPred->pRoot_conj();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfMarkSqyCall = [pCI,&bFoundCall](Card_CONJ* p_conj, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    //  --  --  --  --  --  --  --  --  --  --  --  -- -- --

    if (! (pCI->Flat.m_p_term_sqyCall = p_conj->pGetTerm_sqyNo(pCI)))
      return C_CONTINUE_RECURSION;

   pCI->Flat.m_p_conj_iqyCall = p_conj;
    pCI->Flat.m_p_conj_iqyCall->vMark1(MARK_AT);
    bFoundCall = true;

    if (p_conj->pSymbol()->bHasFacet("exists"_Stz) ||
        p_conj->pSymbol()->bHasFacet("notExists"_Stz)) {

      RASSERT(p_conj->bIsSqyOp());

      pCI->Flat.m_p_term_crlKey = p_conj->pGetTerm(0);
      pCI->Flat.m_p_term_sqyCall = p_conj->pGetTerm(1);
      return C_TERMINATE_RECURSION;
    }

    if (p_conj->pSymbol()->bHasFacet("in"_Stz) || p_conj->pSymbol()->bHasFacet("notIn"_Stz)) {
      RASSERT(p_conj->bIsSqyOp());
      pCI->Flat.m_p_term_crlKey = p_conj->pGetTerm(2);
      return C_TERMINATE_RECURSION;
    }

    RASSERT(p_conj->bIsRelnOp());

    return C_TERMINATE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_conj_ROOT->bLambdaWalk_conj(bfMarkSqyCall);
  UNUSED(bIgnore);

  return bFoundCall;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Board::i_Board(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  return m_i_Board;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vBoardNo(i8 i_Board)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  m_i_Board = i_Board;
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
/* clear all marks                                                           */
/*----------------------------------------------------------------------------*/
void Board::vClearCardMarks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_pDeck->vMarksClearRecursive();
}

/*----------------------------------------------------------------------------*/
/* get card subSet                                                            */
/*----------------------------------------------------------------------------*/
Card_Deck* Board::pDeck(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_pDeck;
}

/*----------------------------------------------------------------------------*/
/* dump this board                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDump(i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  vTrace("%sBoard %p %lld %s\n"_Stz,
         Stz::szTab(iDepth),
         this,
         m_i_Board,
         aBK[m_eBoard]
  );

  m_pSubQuery->vDump(iDepth+1);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vEndPhase(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  m_pState->vInsert(aOH[m_ePhase]);

  m_ePhase = OH_NONE;
}

/*----------------------------------------------------------------------------*/
/* has board passed given phase                                               */
/*----------------------------------------------------------------------------*/
bool Board::bPastPhase(eOH ePhase)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  if (m_pState->bHasName(aOH[ePhase]))
    return true;

  return false;;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eOH Board::ePhase(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  return m_ePhase;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vPhase(eOH ePhase)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  m_ePhase = ePhase;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vPrintBoard(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Stz* szQueryNm = m_pBench->pOptimiser()->szQueryNm();

  Ribbon* pRibbon = new Ribbon(m_pBench->pOptimiser()->pCardCtl());

  vPinTitle(pRibbon);

  vPinCards(pCI, pRibbon);

  pRibbon->vDrawH('-',0);
  pRibbon->vDrawH('-',2);
  pRibbon->vDrawH('=');

  vPinMargin(pRibbon);

  i8 iBenchNo = m_pBench->iBenchNo();
  eOH ePhase  = this->ePhase();
  i8 iPrintNo = m_pBench->nPrintBoard_inc();
  pRibbon->vPrintRibbon(szQueryNm, iBenchNo, ePhase, iPrintNo);

  delete pRibbon;
}

/*----------------------------------------------------------------------------*/
/* get sub-query                                                              */
/*----------------------------------------------------------------------------*/
void Board::vRenumberCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_pDeck->vRenumberRoot(m_i_Board);
}

/*----------------------------------------------------------------------------*/
/* get card subSet                                                            */
/*----------------------------------------------------------------------------*/
TagSet* Board::pState(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_pState;
}


/*----------------------------------------------------------------------------*/
/* get sub-query                                                              */
/*----------------------------------------------------------------------------*/
SubQuery* Board::pSubQuery(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_pSubQuery;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vValidate(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  RASSERT(pCI->E.pBoard);

  if (pCI->E.pBoard->eBoard() == BK_SUBQUERY) {
    m_pSubQuery->vValidate(pCI);
  }
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pin left margin                                                            */
/*----------------------------------------------------------------------------*/
void Board::vPinMargin(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  s32 sLeftMargin;

  sLeftMargin.vSprint("%s "_Stz,
                      m_pMark->szChrSeq());

  pRibbon->vInsertCharsV(0,0,sLeftMargin);
}
/*----------------------------------------------------------------------------*/
//void Board::vPinMargin(Ribbon* pRibbon)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
//
////  pRibbon->vDrawH('-',true);
////  pRibbon->vDrawH('=');
//
//  //pRibbon->vPutTextRight(szXhand(),1);
//  //pRibbon->vPutTextRight(szXfunc(),2);
//
//  s32 sLeftMargin;
//
//  char cPrefix = m_pBench->cPrefix();
//
//  if (' ' == cPrefix)
//    sLeftMargin.vSprint("%lld "_Stz, m_i_Board);
//  else
//  if ('m' == cPrefix || 'w' == cPrefix)
//    sLeftMargin.vSprint("%c "_Stz, cPrefix);
//  else
//  if (m_pBench->bIsFlat() && 0 == m_i_Board)
//    sLeftMargin.vSprint("fq "_Stz);
//  else
//    sLeftMargin.vSprint("%c%lld "_Stz, cPrefix,m_i_Board);
//
//  pRibbon->vInsertCharsV(0,0,sLeftMargin);
//  //pRibbon->vDrawV(0,0,"x"_Stz);
//}

/*----------------------------------------------------------------------------*/
/* pin board onto given ribbon                                                */
/*----------------------------------------------------------------------------*/
void Board::vPinCards(CardIter* pCI, Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  UNUSED(pCI);

  CardCtl* pCardCtl = m_pBench->pOptimiser()->pCardCtl();

  if (! m_pDeck->nItems())
    return;

  pRibbon->vLaneFirst();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Ribbon* pSubRibbon = new Ribbon(pCardCtl);

  Kid xBI(m_pDeck);

  i8 nCount = 0;

  while (xBI.bEach()) {

    if (! nCount++)
      pSubRibbon->vLaneFirst();
    else
    if (pSubRibbon->i_Char() && pCardCtl->bHeatNext(xBI.pChild()->eCard()))
      pSubRibbon->vHeatNext();
    else
      pSubRibbon->vLaneNext();


    bool bPrune = false;
    eAL eLevel = AL_NONE;
    Card_Level* p_level = xBI.pChild()->pSub_level();
    if (p_level)
      eLevel = (eAL) p_level->pSymbol()->Y_iGet(P_Level);
    xBI.pChild()->vPrintRecursive(pCI,
                                  pSubRibbon,
                                  0,
                                  m_pDeck,
                                  bPrune,
                                  eLevel,
                                  false,
                                  m_pBench->iXform(),
                                  m_pBench->szXfunc());
  }

  pRibbon->vPinSubRibbon(pRibbon->i_Heat(), 0, pSubRibbon);

  delete pSubRibbon;
}

/*----------------------------------------------------------------------------*/
/* pin board onto given ribbon                                                */
/*----------------------------------------------------------------------------*/
void Board::vPinTitle(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  if (! m_pDeck->nItems())
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pRibbon->vSprintSnippet(false,"PrintBoard: %s/%lld -- %s"_Stz,
                          aXF[m_pBench->eXform()],
                          m_pBench->nTransformPrint_inc(),
                          aXF_desc[m_pBench->eXform()]);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pRibbon->vHeatSet();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
