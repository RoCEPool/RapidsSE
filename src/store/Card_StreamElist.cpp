/*============================================================================*/
/* Card_StreamElist.cpp -- List of Stream Expressions                         */
/*============================================================================*/

#include "Card_StreamElist.hpp"
#include "Card_Expr.hpp"
#include "Card_Type.hpp"
#include "Card_As.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_StreamElist,CK_STREAMELIST)
__aNewFunction(Card_StreamElist,CN_CARD)
__vDeleteFunction(Card_StreamElist,CN_CARD)


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
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_StreamElist::bSimpleDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* print StreamElist                                                          */
/*----------------------------------------------------------------------------*/
void Card_StreamElist::vStreamElistPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StreamElist* pCard_StreamElist = _pCard_StreamElist(this);

  if (! pCard_StreamElist->bSimpleDef()) {
    vPrintProperties(pRibbon, iDepth);
    return;
  }

  // expr
  // type
  // as
  // exprCalc
  // term

  eCK eSubCard;
  i8 nSubs = this->nItems();
  for (i8 i_Sub = 0; i_Sub < nSubs; i_Sub++) {
    Card* pSubCard = pGetChild(i_Sub, eSubCard);
    if (CK_EXPR != eSubCard)
      continue;

    Card_Expr* pCard_Expr = _pCard_Expr(pSubCard);

    Card_Type* pCard_Type = _pCard_Type(pCard_Expr->pFindSubCard(CK_TYPE));
    Card_As*   pCard_As   = _pCard_As  (pCard_Expr->pFindSubCard(CK_AS));

    pRibbon->vSprintRight("%s %c"_Stz
        ,pCard_As ? pCard_As->sz() : "??AS?"_Stz
        ,pCard_Type->cTypeShort()
    );

  }
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

    Card_Expr* pCard_Expr = new Card_Expr(pColumn);
    vAppend(pCard_Expr);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
