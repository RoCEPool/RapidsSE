/*============================================================================*/
/* Board.cpp -- Board                                                         */
/*============================================================================*/

#include "Board.hpp"

#include "ShardDef.hpp"

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
void Board::vPinBoard(Ribbon* pRibbon, i8 i_Board)
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

    if (! nCount++)
      pSubRibbon->vFirstColumn();
    else
      pSubRibbon->vNextColumn();
    pCard->vCardHeader(pSubRibbon);

    if (CK_TABLE == pCard->eCard())
      vPrintTable(pCard,pSubRibbon);
    else
      pCard->vCardPrint(pSubRibbon,0,this);
  }
     
  pRibbon->vPinRibbon(1,0,pSubRibbon);
  
  delete pSubRibbon;
}

/*----------------------------------------------------------------------------*/
/* print card onto given ribbon                                               */
/*----------------------------------------------------------------------------*/
void Board::vPrintCard(Card* pCard, Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  eCK eCard = pCard->eCard();
  
  switch (eCard) {

      case CK_AGGREG:        break;
      case CK_AGGREGPRED:    break;
      case CK_ANDPRED:       break;
      case CK_AS:            break;
      
      case CK_BY:            break;
      case CK_BYLIST:        break;
      case CK_BYPRED:        break;
      
      case CK_COMBINE:       break;
      case CK_COMPAREPRED:   break;
      case CK_CORRSUBQUERY:  break;
      case CK_CREATETABLE:   break;
      
      case CK_DEBUG:         break;
      case CK_DROPTABLE:     break;
      
      case CK_EXISTS:        break;
      case CK_EXPR:          break;
      case CK_FILTER:        break;
      
      case CK_HASH:          break;
      case CK_HASHCAST:      break;
      case CK_HAVING:        break;
       
      case CK_INDEX:         break;
      case CK_INDEXJOIN:     break;
      case CK_INDEXPRED:     break;
      case CK_INDEXSCAN:     break;
      
      case CK_LIMIT:         break;
      
      case CK_MAINSUBQUERY:  break;
      
      case CK_NODECAST:      break;
      case CK_NONCSUBQUERY:  break;
      
      case CK_ON:            break;
      case CK_ORDAGG:        break;
      case CK_ORIGINCAST:    break;
      case CK_ORPRED:        break;
      
      case CK_PREDICATE:     break;
      case CK_PROJECT:       break;
      case CK_PROJECTION:    break;
      case CK_PROJECTPRED:   break;
      
      case CK_SCAN:          break;
      case CK_SEND:          break;
      case CK_SORT:          break;
      case CK_STAGE:         break;
      case CK_STREAMDEF:     break;
      
      case CK_TABLE:         break;
      case CK_TERM:          break;
      case CK_TERMLIST:      break;
      case CK_TYPE:          break;

    default:
      break;
  }
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



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* print table card onto given ribbon                                         */
/*----------------------------------------------------------------------------*/
void Board::vPrintTable(Card* pCard, Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
  Card_Table* pCardTable = _pCard_Table(pCard);
  Table* pTable = _pTable(pCardTable->pTable());

  pRibbon->vSput(FR_NAME,pTable->szTableNm());

  if (pRibbon->iArea())
    return;
  
  //pCardTable->vPrintPartition(pRibbon);
  ShardDef* pShardDef = pTable->pShardDef();
  Column* pColumn = pShardDef->pGetColumn(0);
  
  pRibbon->vSput(FR_ATOM_SLASH,pColumn->szColNm());

  
  //pCardTable->vPrintSize(pRibbon);
  StreamDef* pStreamDef = pTable->pStreamDef();

  pRibbon->vSput(FR_SIZE,pStreamDef->jTableSize());

  
  //pCardTable->vPrintPrimary(pRibbon);
  IdxVec*   pIdxVec    = pTable->pIdxVec();
  Index*    pIndex     = pIdxVec->pGet(0);
  IndexDef* pIndexDef  = pIndex->pIndexDef();
  Column*   pIdxColumn = pIndexDef->pGetColumn(0);
  
  pRibbon->vSput(FR_ATOM_SQUARE,pIdxColumn->szColNm());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
