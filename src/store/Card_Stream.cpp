/*============================================================================*/
/* Card_Stream.cpp -- Stream                                                  */
/*============================================================================*/

#include "Card_Stream.hpp"
#include "Card_Expr.hpp"
#include "Card_ObyElist.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Stream,CK_STREAM)
__aNewFunction(Card_Stream,CN_CARD)
__vDeleteFunction(Card_Stream,CN_CARD)



/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Stream::Card_Stream(void)
: Card(CK_STREAM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Stream::~Card_Stream(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is given <pred> within this stream?                                        */
/*----------------------------------------------------------------------------*/
bool Card_Stream::bContainsPred(Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  eCK ePredCard = pCard_PRED->eCard();

  if (CK_TERM == ePredCard) {
    Stz* szTerm = pCard_PRED->sz();
    if (!szTerm->bIsStreamColOrd())
      return true;

    i8 iOrdLo = m_pProperties->iValue(P_OrdLo);
    i8 iOrdHi = m_pProperties->iValue(P_OrdHi);

    i8 iOrd = szTerm->iOrdinal();
    if (iOrd > iOrdHi)
      return false;
  }

  i8 nSubCards = pCard_PRED->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pCard_PRED->pGetChild(i_Child);

    if (! bContainsPred(static_cast<Card_PRED*>(pSubCard)))
      return false;
  }

  return true;
}



/*----------------------------------------------------------------------------*/
/* get <streamColList>                                                       */
/*----------------------------------------------------------------------------*/
Card_StmColList* Card_Stream::pGet_streamColList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StmColList* pCard_StreamColList
    = _pCard_StmColList(pFindSubCard(CK_STMCOLLIST));

  return pCard_StreamColList;
}

/*----------------------------------------------------------------------------*/
/* get number of stream columns                                               */
/*----------------------------------------------------------------------------*/
void Card_Stream::vStreamPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  s32 sName;
  sName.vSprint("%s%s"_Stz,szCardNm(),nProperties()?" +":"");

  if (bHasProperty(P_OrdLo))
    sName.vCatSprint(" $%lld,%lld"_Stz,
                     Y_iVal(P_OrdLo), Y_iVal(P_OrdHi));

  vPrintCard_byCustomNm(pRibbon, iDepth, sName);
}

/*----------------------------------------------------------------------------*/
/* get number of stream columns                                               */
/*----------------------------------------------------------------------------*/
i8 Card_Stream::nStreamCols(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_StmColList* pCard_StreamColList = pGet_streamColList();

  i8 nStreamCols = pCard_StreamColList->nStreamCols();

  return nStreamCols;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
