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
__pGetSubFunction_Card(STORE,StreamColList,CK_STREAMCOLLIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamColList::Card_StreamColList(void)
: Card(CK_STREAMCOLLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamColList::Card_StreamColList(StreamDef* pStreamDef, Stz* szPrefix_opt)
: Card(CK_STREAMCOLLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  vPopulate(pStreamDef,szPrefix_opt);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamColList::Card_StreamColList(Card_AggElist* pCard_AggElist)
: Card(CK_STREAMCOLLIST)
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
/* get <streamColList>                                                       */
/*----------------------------------------------------------------------------*/
Card_StreamCol* Card_StreamColList::pGet_streamCol(i8 i_StreamColNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StreamCol *pCard_StreamCol
      = _pCard_StreamCol(pGetChild(i_StreamColNo));

  return pCard_StreamCol;
}

/*----------------------------------------------------------------------------*/
/* is given <pred> within this stream?                                        */
/*----------------------------------------------------------------------------*/
Card_StreamCol* Card_StreamColList::pFind_streamCol(Stz *szColNm, i8& i_StreamColNo)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_StreamColList* pCard_StreamColList = _pGetSub_StreamColList(this);

  Card_StreamCol* pCard_streamCol = nullptr;

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = this->pGetChild(i_Child);

    if (CK_STREAMCOL != pSubCard->eCard())
      continue;

    Card_As* pCard_as = _pGetSub_As(pSubCard);
    if (pCard_as->sz()->bSame(szColNm)) {
      pCard_streamCol = _pCard_StreamCol(pSubCard);
      i_StreamColNo = i_Child;
      break;
    }
  }

  return pCard_streamCol;
}


/*----------------------------------------------------------------------------*/
/* pre-pend given prefix to each As                                           */
/*----------------------------------------------------------------------------*/
void Card_StreamColList::vPrependToAs(Stz* szPrefix)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = this->pGetChild(i_Child);

    if (CK_STREAMCOL != pSubCard->eCard())
      continue;

    Card_StreamCol *pCard_StreamCol = _pCard_StreamCol(pSubCard);

    Card_As* pCard_As = _pCard_As(pCard_StreamCol->pFindSubCard(CK_AS));

    s256 sRevisedAs;
    sRevisedAs.vSprint("%s%s"_Stz,szPrefix,pCard_As->sz());

    pCard_As->vAs(sRevisedAs);
  }
}
/*----------------------------------------------------------------------------*/
/* populate from stream def                                                   */
/*----------------------------------------------------------------------------*/
void Card_StreamColList::vPopulate(StreamDef* pStreamDef, Stz* szPrefix_opt)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_StreamCol *pCard_StreamCol = new Card_StreamCol;

    vAppend(pCard_StreamCol);

    Card_As *pCard_As = new Card_As(pColumn);
    Card_Type *pCard_Type = new Card_Type(pColumn);

    if (szPrefix_opt) {
      s256 sAs;
      sAs.vSprint("%s%s"_Stz,szPrefix_opt,pColumn->szColNm());
      pCard_As->vAs(sAs);
    }

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

  i8 nSubCards = pCard_AggElist->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pSubCard = pCard_AggElist->pGetChild(i_Child);
    if (CK_EXPR != pSubCard->eCard())
      continue;

    Card_As*   pCard_As   = _pCard_As  (pSubCard->pFindSubCard(CK_AS));
    Card_Type* pCard_Type = _pCard_Type(pSubCard->pFindSubCard(CK_TYPE));

    RASSERT(pCard_As);
    RASSERT(pCard_Type);

    Card_StreamCol *pCard_StreamCol = new Card_StreamCol;

    vAppend(pCard_StreamCol);

    pCard_StreamCol->vAppend(new Card_As(pCard_As->sz()));
    pCard_StreamCol->vAppend(new Card_Type(pCard_Type->sz()));
  }

}

/*----------------------------------------------------------------------------*/
/* get number of stream columns                                               */
/*----------------------------------------------------------------------------*/
i8 Card_StreamColList::nStreamCols(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nStreamCols = nChildCards();
  return nStreamCols;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
