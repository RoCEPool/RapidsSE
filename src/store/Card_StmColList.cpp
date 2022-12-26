/*============================================================================*/
/* Card_StmColList.cpp -- Stream Column List                               */
/*============================================================================*/

#include "Card_StmColList.hpp"
#include "Card_Expr.hpp"
#include "Card_ObyElist.hpp"
#include "Card_AggElist.hpp"

#include "Card_StmCol.hpp"
#include "Card_As.hpp"
#include "Card_Type.hpp"
#include "CardIter.hpp"
#include "Card_TblColOdn.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_StmColList, CK_STMCOLLIST)
__aNewFunction(Card_StmColList, CN_CARD)
__vDeleteFunction(Card_StmColList, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StmColList::Card_StmColList(void)
: Card(CK_STMCOLLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StmColList::~Card_StmColList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is given <pred> within this stream?                                        */
/*----------------------------------------------------------------------------*/
Card_StmCol* Card_StmColList::pStmColList_Find_streamCol(Stz *szColNm, i8& ri_StreamColNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StmColList* pCard_StreamColList = this->pSub_stmColList();

  Card_StmCol* pCard_streamCol = nullptr;

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = this->pGetChild(i_Child);

    if (CK_STMCOL != pSubCard->eCard())
      continue;

    //Card_As* pCard_as = pSubCard->pSub_as();
    if (pSubCard->bSame(szColNm)) {
      pCard_streamCol = _pCard_StmCol(pSubCard);
      ri_StreamColNo = i_Child;
      break;
    }
  }

  return pCard_streamCol;
}

/*----------------------------------------------------------------------------*/
/* is given <pred> within this stream?                                        */
/*----------------------------------------------------------------------------*/
Card_StmCol* Card_StmColList::pStmCol_FindBy_tblColOdn(Stz *szTblColOdn, i8& ri_StmCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI(this);
  while (Card_StmCol *p_stmCol = _pCard_StmCol(xCI.pEachChild(CK_STMCOL))) {
    Card_TblColOdn* p_tblColOdn = p_stmCol->pSub_tblColOdn();
    if (p_tblColOdn->bSame(szTblColOdn)) {
      ri_StmCol = xCI.i_Child();
      return p_stmCol;
    }
  }

  ri_StmCol = C_UNKNOWN;
  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
