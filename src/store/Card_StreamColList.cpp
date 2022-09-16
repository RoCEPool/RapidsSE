/*============================================================================*/
/* Card_StreamColList.cpp -- Stream Column List                               */
/*============================================================================*/

#include "Card_StreamColList.hpp"
#include "Card_Expr.hpp"
#include "Card_ObyElist.hpp"
#include "Card_StreamElist.hpp"
#include "Card_AggElist.hpp"

#include "Card_StreamCol.hpp"
#include "Card_As.hpp"
#include "Card_Type.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_StreamColList,CK_STREAMCOLLIST)
__aNewFunction(Card_StreamColList,CN_CARD)
__vDeleteFunction(Card_StreamColList,CN_CARD)

__bIsCard(STORE,Card_StreamColList,CK_STREAMCOLLIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamColList::Card_StreamColList(void)
: Card_SUB(CK_STREAMCOLLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamColList::Card_StreamColList(StreamDef* pStreamDef)
: Card_SUB(CK_STREAMCOLLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  vPopulate(pStreamDef);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamColList::Card_StreamColList(Card_AggElist* pCard_AggElist)
: Card_SUB(CK_STREAMCOLLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  vPopulate(pCard_AggElist);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StreamColList::~Card_StreamColList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate from stream def                                                   */
/*----------------------------------------------------------------------------*/
void Card_StreamColList::vPopulate(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_StreamCol *pCard_StreamCol = new Card_StreamCol;

    vAppend(pCard_StreamCol);

    Card_As *pCard_As = new Card_As(pColumn);
    Card_Type *pCard_Type = new Card_Type(pColumn);

    pCard_StreamCol->vAppend(pCard_As);
    pCard_StreamCol->vAppend(pCard_Type);
  }
}

/*----------------------------------------------------------------------------*/
/* populate from agg elist                                                    */
/*----------------------------------------------------------------------------*/
void Card_StreamColList::vPopulate(Card_AggElist* pCard_AggElist)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nSubCards = pCard_AggElist->nSubCards();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card* pSubCard = pCard_AggElist->pGetSubCard(i_SubCard);
    if (CK_EXPR != pSubCard->eCard())
      continue;

    Card_As*   pCard_As   = _pCard_As  (pSubCard->pFindSubCard(CK_AS));
    Card_Type* pCard_Type = _pCard_Type(pSubCard->pFindSubCard(CK_TYPE));

    RASSERT(pCard_As);
    RASSERT(pCard_Type);

    Card_StreamCol *pCard_StreamCol = new Card_StreamCol;

    vAppend(pCard_StreamCol);

    pCard_StreamCol->vAppend(new Card_As(pCard_As->szAs()));
    pCard_StreamCol->vAppend(new Card_Type(pCard_Type->szType()));
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
