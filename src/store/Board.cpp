/*============================================================================*/
/* Board.cpp -- Board                                                         */
/*============================================================================*/

#include "Board.hpp"

#include "PtnDef.hpp"
#include "Cue.hpp"

#include "Optimiser.hpp"
#include "OptimiserFun.hpp"
#include "OptimiserCatalog.hpp"

#include "Card_StmCol.hpp"

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
Board::Board(Stz* szBoardNm, eBK eBoard, i8 i_SubBoard, eBT eBoardType, Bench* pBench)
: Object(CN_BOARD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_sBoardNm     = szBoardNm;
  m_eBoard       = eBoard;
  m_i_SubBoard   = i_SubBoard;
  m_eBoardType   = eBoardType;

  m_pBench       = pBench;

  m_pOptimiser   = m_pBench->pOptimiser();

  m_pCard_Deck  = new Card_Deck;

  m_iTransformNo = C_UNKNOWN;
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
    m_pCard_Deck->vRemoveAll();
  }

  delete m_pCard_Deck;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get board kind                                                             */
/*----------------------------------------------------------------------------*/
eBK Board::eBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_eBoard;
}

/*----------------------------------------------------------------------------*/
/* get board name                                                             */
/*----------------------------------------------------------------------------*/
Stz* Board::szBoardNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_sBoardNm;
}

/*----------------------------------------------------------------------------*/
/* set board name                                                             */
/*----------------------------------------------------------------------------*/
void Board::vBoardNm(Stz* szBoardNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_sBoardNm = szBoardNm;
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
/* clear all marks                                                           */
/*----------------------------------------------------------------------------*/
void Board::vClearMarks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_pCard_Deck->vClearMarksRecursive();
}

/*----------------------------------------------------------------------------*/
/* get card subSet                                                            */
/*----------------------------------------------------------------------------*/
Card_Deck* Board::pDeck(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_pCard_Deck;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDuplicateBoard(Board* pBoard_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  for (i8 i_Main_arg = 0; i_Main_arg < pBoard_arg->m_pCard_Deck->nItems(); i_Main_arg++)
    m_pCard_Deck->vAppendMain(pBoard_arg->m_pCard_Deck->pGetChild(i_Main_arg)->pDupCardAndSubCards());
}

/*----------------------------------------------------------------------------*/
/* pin board onto given ribbon                                                */
/*----------------------------------------------------------------------------*/
void Board::vPinBanners(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  pRibbon->vDrawH('-',true);
  pRibbon->vDrawH('=');

  Stz* szPhase = m_pBench->pOptimiser()->pTransformer()->pTransforms()->szPhase();
  Stz* szXfrom = m_pBench->pOptimiser()->pTransformer()->pTransforms()->szTransformation();

  pRibbon->vPutTextRight(szPhase,1);
  pRibbon->vPutTextRight(szXfrom,2);
}

/*----------------------------------------------------------------------------*/
/* pin board onto given ribbon                                                */
/*----------------------------------------------------------------------------*/
void Board::vPinCards(Ribbon* pRibbon,
                      i8 iPrintNo, Stz* szDescription)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  CardCtl* pCardCtl = m_pOptimiser->pCardCtl();

  if (! m_pCard_Deck->nItems())
    return;

  pRibbon->vLaneFirst();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Ribbon* pSubRibbon = new Ribbon(pCardCtl,this);

  BoardIter xBI(this);

  i8 nCount = 0;
  while (Card* pCard = xBI.pEachMain()) {
    eCK eCard = pCard->eCard();

    // skip non-printable horizontal lines
    if (pCardCtl->bNextHoriz(eCard) &&
        pCard->bHasProperty(pCardCtl->szHorizExclude(eCard))) {

      while (Card* pNextMain = xBI.pNextMain()) {
        eCK eNextCard = pNextMain->eCard();
        if (pCardCtl->bNextHoriz(eNextCard))
          break;
        xBI.vForward();
      }

      continue;
    }

    if (! nCount++)
      pSubRibbon->vLaneFirst();
    else
    if (pSubRibbon->i_Char() && pCardCtl->bNextHoriz(eCard))
      pSubRibbon->vHeatNext();
    else
      pSubRibbon->vLaneNext();

    bool bPrune = false;
    pCard->vPrintCardRecursive(pSubRibbon, 0, m_pCard_Deck, bPrune);
  }

  pRibbon->vPinSubRibbon(pRibbon->i_Heat(), 0, pSubRibbon);
  
  delete pSubRibbon;
}

/*----------------------------------------------------------------------------*/
/* pin board onto given ribbon                                                */
/*----------------------------------------------------------------------------*/
void Board::vPinTitle(Ribbon* pRibbon,
                      i8 iTransformNo, Stz* szDescription)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  CardCtl* pCardCtl = m_pOptimiser->pCardCtl();

  if (! m_pCard_Deck->nItems())
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pRibbon->vSprint("Transform: %02lld of board %s (%s %s [%lld]) Query=%s"_Stz,
                   iTransformNo,
                   m_sBoardNm.szStz(),

                   aBT[m_eBoardType],
                   aBK[m_eBoard],
                   m_i_SubBoard,

                   m_pOptimiser->szQueryNm());

  pRibbon->vSprint("%s"_Stz,szDescription ? szDescription : ""_Stz);
  pRibbon->vTitleLines();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vPrintBoard(Stz* szPhase,
                        Stz* szTransformation,
                        Stz* szDescription)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Ribbon* pRibbon = new Ribbon(m_pOptimiser->pCardCtl(),this);

  vPinTitle(pRibbon, m_iTransformNo, szDescription);

  vPinCards(pRibbon, m_iTransformNo, szDescription);

  vPinBanners(pRibbon);

  pRibbon->vPrintRibbon();

  delete pRibbon;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Board::i_SubBoard(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  return m_i_SubBoard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vTransformNoInc(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  m_iTransformNo++;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
