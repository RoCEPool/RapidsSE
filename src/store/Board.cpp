/*============================================================================*/
/* Board.cpp -- Board                                                         */
/*============================================================================*/

#include "Board.hpp"

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
Board::Board(Stz* szCardNm)
: Object(CN_BOARD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
        
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
void Board::vPinBoard(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  if (! nItems())
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pRibbon->vSprint("%s"_Stz,szQueryNm());
  pRibbon->vTitleLines(1);
  pRibbon->vSetBase();
  pRibbon->vFirstColumn();

  Ribbon* pSubRibbon = new Ribbon(0);
  
  pSubRibbon->vFirstColumn();

  ListExoIter2_t<Card>* pBoard_I = pNewIter();

  i8 nCount = 0;
  while (pBoard_I->bHasNext()) {
    Card* pCard = pBoard_I->pNext();

    if (nCount++)
      pSubRibbon->vNextColumn();

    pCard->vBoardPrint(pSubRibbon,0);
  }
     
  pRibbon->vPinRibbon(1,0,pSubRibbon);
  
  delete pSubRibbon;
}

/*----------------------------------------------------------------------------*/
/* print sumamry of board                                                     */
/*----------------------------------------------------------------------------*/
void Board::vPrintSummary(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  vTrace("Board Summary\n"_Stz);

  ListExoIter2_t<Card>* pBoard_I = pNewIter();
  
  while (pBoard_I->bHasNext()) {
    Card* pCard = pBoard_I->pNext();

    eCN eCardClass = pCard->eGetClassNo();
    
    vTrace("  %s\n"_Stz,aCN[eCardClass]);
  }
  
  delete pBoard_I;
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
