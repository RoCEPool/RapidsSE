/*============================================================================*/
/* Card_StreamElist.cpp -- List of Stream Expressions                         */
/*============================================================================*/

#include "Card_StreamElist.hpp"
#include "Card_Name.hpp"
#include "Card_Expr.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_StreamElist,CK_STREAMELIST)
__aNewFunction(Card_StreamElist,CN_CARD)
__vDeleteFunction(Card_StreamElist,CN_CARD)

__bIsCard(STORE,Card_StreamElist,CK_STREAMELIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamElist::Card_StreamElist(void)
: Card_ELIST(CK_STREAMELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamElist::Card_StreamElist(StreamDef* pStreamDef)
: Card_ELIST(CK_STREAMELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  vPopulate(pStreamDef);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StreamElist::~Card_StreamElist(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_StreamElist::vPopulate(StreamDef* pStreamDef)
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
