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
Card_OrderElist::Card_OrderElist(StreamDef* pStreamDef)
: Card_ELIST(CK_ORDERELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  vPopulate(pStreamDef);
  
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_OrderElist::bSimpleDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OrderElist::vPopulate(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column* pColumn = pStreamDef->pGetColumn(i_Col);
    
//    Card_Name* pCard_Name = new Card_Name(pColumn->szColNm());
//    vAppend(pCard_Name);
    
    Card_Expr* pCard_Expr = new Card_Expr(pColumn);
    vAppend(pCard_Expr);
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
