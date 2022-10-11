/*============================================================================*/
/* Card_Stream.cpp -- Stream                                                  */
/*============================================================================*/

#include "Card_Stream.hpp"
#include "Card_Expr.hpp"
#include "Card_ObyElist.hpp"
#include "Card_StreamElist.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Stream,CK_STREAM)
__aNewFunction(Card_Stream,CN_CARD)
__vDeleteFunction(Card_Stream,CN_CARD)

__bIsCard(STORE,Card_Stream,CK_STREAM)
__pSubFunction_Card(STORE,Stream,CK_STREAM)


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
/* apply the order of given arg to this stream                                */
/*----------------------------------------------------------------------------*/
void Card_Stream::vApplyOrder(Card_Stream* pCard_Stream_src)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_ObyElist* pCard_ObyElist_src =
    _pCard_ObyElist(pCard_Stream_src->pFindSubCard(CK_OBYELIST));
  if (! pCard_ObyElist_src)
    return;
  i8 nOrderExprs_src = pCard_ObyElist_src->nItems();

  Card_StreamElist* pCard_StreamElist_dst =
    _pCard_StreamElist(this->pFindSubCard(CK_STREAMELIST));
  if (! pCard_StreamElist_dst)
    return;
  i8 nStreamExprs_dst = pCard_StreamElist_dst->nItems();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Card_ObyElist* pCard_ObyElist_dst = nullptr;

  for (i8 i_Expr = 0; i_Expr < nOrderExprs_src; i_Expr++) {
    if (i_Expr >= nStreamExprs_dst)
      break;
    
    Card_Expr* pCard_Expr_src =
      _pCard_Expr(pCard_ObyElist_src->pGetChild(i_Expr));
    Card_Expr* pCard_Expr_tst =
      _pCard_Expr(pCard_StreamElist_dst->pGetChild(i_Expr));

    if (! pCard_Expr_tst->bEquivalent(pCard_Expr_src))
      break;

    if (! pCard_ObyElist_dst)
      pCard_ObyElist_dst = new Card_ObyElist;
    
    Card_Expr* pCard_Expr_dst =
      _pCard_Expr(pCard_Expr_src->pDupCardAndSubCards());

    pCard_ObyElist_dst->vAppend(pCard_Expr_dst);
  }
  
  if (pCard_ObyElist_dst)
    this->vAppend(pCard_ObyElist_dst);
}

/*----------------------------------------------------------------------------*/
/* is given <pred> within this stream?                                        */
/*----------------------------------------------------------------------------*/
bool Card_Stream::bContainsPred(Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  eCK ePredCard = pCard_PRED->eCard();

  if (CK_TERM == ePredCard) {
    Stz* szTerm = pCard_PRED->sz();
    if (!szTerm->bIsStreamColOrdinal())
      return true;

    i8 iOrdLo = m_pProperties->iValue(C_PPY_OrdLo);
    i8 iOrdHi = m_pProperties->iValue(C_PPY_OrdHi);

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
/* is given <pred> within this stream?                                        */
/*----------------------------------------------------------------------------*/
Card_StreamCol* Card_Stream::pFind_streamCol(Stz *szColNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

   Card_StreamColList* pCard_StreamColList = _pSub_StreamColList(this);

  Card_StreamCol* pCard_streamCol = nullptr;

  return pCard_streamCol;
}

/*----------------------------------------------------------------------------*/
/* get a specific <streamCol>                                                 */
/*----------------------------------------------------------------------------*/
Card_StreamCol* Card_Stream::pGet_streamCol(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StreamColList* pCard_StreamColList = pGet_streamColList();

  i8 iOrdLo = iGetProperty(C_PPY_OrdLo);
  i8 i_ColNo = i_Col - iOrdLo;
  Card_StreamCol* pCard_StreamCol = pCard_StreamColList->pGet_streamCol(i_ColNo);

  return pCard_StreamCol;
}

/*----------------------------------------------------------------------------*/
/* get a specific <streamCol> given $n ordinal                                */
/*----------------------------------------------------------------------------*/
Card_StreamCol* Card_Stream::pGet_streamCol(Stz* szStreamColOrd)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i_Col = szStreamColOrd->iOrdinal() - 1;

  Card_StreamCol* p_streamCol = pGet_streamCol(i_Col);

  return p_streamCol;
}

/*----------------------------------------------------------------------------*/
/* get <streamColList>                                                       */
/*----------------------------------------------------------------------------*/
Card_StreamColList* Card_Stream::pGet_streamColList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StreamColList* pCard_StreamColList
    = _pCard_StreamColList(pFindSubCard(CK_STREAMCOLLIST));

  return pCard_StreamColList;
}

/*----------------------------------------------------------------------------*/
/* get number of stream columns                                               */
/*----------------------------------------------------------------------------*/
void Card_Stream::vOrdinate(String* psOrdinal, Stz* szOrdinal)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  psOrdinal->vCopy(szOrdinal);
}

/*----------------------------------------------------------------------------*/
/* get number of stream columns                                               */
/*----------------------------------------------------------------------------*/
void Card_Stream::vStreamPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  s32 sName;

  /*
  if (m_pProperties && C_UNKNOWN != iGetProperty(C_PPY_OrdLo))
    sName.vSprint("%s%lld]%s $%lld,%lld"_Stz
        ,Stz::szDots(iDepth-1)
        ,m_iCardNo
        ,szCardNm()
        ,iGetProperty(C_PPY_OrdLo)
        ,iGetProperty(C_PPY_OrdHi)
    );
  else
    sName.vSprint("%s%lld]%s"_Stz
        ,Stz::szDots(iDepth-1)
        ,m_iCardNo
        ,szCardNm()
    );

  pRibbon->vSprint("%-18s%c%c"_Stz
      ,sName.szStz()
      ,m_cMark1
      ,m_cMark2
  );
  */

  sName.vSprint("%s%s"_Stz,szCardNm(),nProperties()?" +":"");

  if (bHasProperty(C_PPY_OrdLo))
    sName.vCatSprint(" $%lld,%lld"_Stz,
                     iGetProperty(C_PPY_OrdLo), iGetProperty(C_PPY_OrdHi));

  vPrintCard_byCustom(pRibbon,iDepth,sName);
}

/*----------------------------------------------------------------------------*/
/* get number of stream columns                                               */
/*----------------------------------------------------------------------------*/
i8 Card_Stream::nStreamCols(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_StreamColList* pCard_StreamColList = pGet_streamColList();

  i8 nStreamCols = pCard_StreamColList->nStreamCols();

  return nStreamCols;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
