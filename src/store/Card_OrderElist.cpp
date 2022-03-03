/*============================================================================*/
/* Card_OrderElist.cpp -- List of Order Expressions                           */
/*============================================================================*/

#include "Card_OrderElist.hpp"
#include "Card_Name.hpp"
#include "Card_Expr.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_OrderElist,CK_ORDERELIST)
__aNewFunction(Card_OrderElist,CN_CARD)
__vDeleteFunction(Card_OrderElist,CN_CARD)

__bIsCard(STORE,Card_OrderElist,CK_ORDERELIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_OrderElist::Card_OrderElist(void)
: Card_ELIST(CK_ORDERELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_OrderElist::Card_OrderElist(Index* pIndex)
: Card_ELIST(CK_ORDERELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  vPopulate(pIndex);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_OrderElist::~Card_OrderElist(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OrderElist::vPopulate(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  IndexDef* pIndexDef = pIndex->pIndexDef();
  i8 nIdxCols = pIndexDef->nColumns();
  
  for (i8 i_IdxCol = 0; i_IdxCol < nIdxCols; i_IdxCol++) {

    Column* pIdxCol = pIndexDef->pGetColumn(i_IdxCol);

    Card_Expr* pCard_Expr = new Card_Expr(pIdxCol);
    vAppend(pCard_Expr);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
