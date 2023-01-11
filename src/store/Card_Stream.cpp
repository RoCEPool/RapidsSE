/*============================================================================*/
/* Card_Stream.cpp -- Stream                                                  */
/*============================================================================*/

#include "Card_Stream.hpp"

#include "CardIter.hpp"
#include "Card_AggElist.hpp"
//#include "Card_ELIST.hpp"
#include "Card_ProjElist.hpp"
#include "Card_Expr.hpp"
#include "Card_Type.hpp"
#include "Card_As.hpp"
#include "Card_StmColNm.hpp"
#include "Table.hpp"
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
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* produce composite <stream> of <aggreg>                                     */
/*----------------------------------------------------------------------------*/
Card_Stream* Card_Stream::pDerive_Of_aggreg(Card_AggElist* p_aggElist)
{
  _static_

  Card_Stream *p_stream = new Card_Stream;
  Card_StmColList *p_stmColList = new Card_StmColList;
  p_stream->vAppend(p_stmColList);

  i8 i_Expr = 0;
  CardIter xCI_elist(p_aggElist);
  while (Card_Expr* p_expr = _pCard_Expr(xCI_elist.pEachChild())) {
    Card_As*   p_expr_as   = nullptr;
    Card_Type* p_expr_type = nullptr;

    i_Expr++;
    CardIter xCI_expr(p_expr);
    while (Card* p_expr_sub = xCI_expr.pEachChild()) {

      eCK e_expr_sub = p_expr_sub->eCard();

      if (CK_AS == e_expr_sub)
        p_expr_as = _pCard_As(p_expr_sub);
      else
      if (CK_TYPE == e_expr_sub)
        p_expr_type = _pCard_Type(p_expr_sub);

    }

    Card_StmCol* p_stmCol = new Card_StmCol();
    p_stmColList->vAppend(p_stmCol);

    Card_StmColNm* p_stmColNm = new Card_StmColNm();
    if (p_expr_as)
      p_stmColNm->vValue(p_expr_as->szValue());
    else
      p_stmColNm->vValueSprint("__%lld"_Stz,i_Expr);;
    p_stmCol->vAppend(p_stmColNm);

    Card_Type* p_type = _pCard_Type(p_expr_type->pDupCardAndSubCards());
    p_stmCol->vAppend(p_type);
  }

  return p_stream;
}

/*----------------------------------------------------------------------------*/
/* produce composite <stream> of <project>                                    */
/*----------------------------------------------------------------------------*/
Card_Stream* Card_Stream::pDerive_Of_project(Card_ProjElist* p_projElist)
{
  _static_

  Card_Stream *p_stream = new Card_Stream;
  Card_StmColList *p_stmColList = new Card_StmColList;
  p_stream->vAppend(p_stmColList);

  i8 i_Expr = 0;
  CardIter xCI_elist(p_projElist);
  while (Card_Expr* p_expr = _pCard_Expr(xCI_elist.pEachChild())) {
    Card_As*   p_expr_as   = nullptr;
    Card_Type* p_expr_type = nullptr;

    i_Expr++;
    CardIter xCI_expr(p_expr);
    while (Card* p_expr_sub = xCI_expr.pEachChild()) {

      eCK e_expr_sub = p_expr_sub->eCard();

      if (CK_AS == e_expr_sub)
        p_expr_as = _pCard_As(p_expr_sub);
      else
      if (CK_TYPE == e_expr_sub)
        p_expr_type = _pCard_Type(p_expr_sub);

    }

    Card_StmCol* p_stmCol = new Card_StmCol();
    p_stmColList->vAppend(p_stmCol);

    Card_StmColNm* p_stmColNm = new Card_StmColNm();
    if (p_expr_as)
      p_stmColNm->vValue(p_expr_as->szValue());
    else
      p_stmColNm->vValueSprint("__%lld"_Stz,i_Expr-1);
    p_stmCol->vAppend(p_stmColNm);

    Card_Type* p_type = _pCard_Type(p_expr_type->pDupCardAndSubCards());
    p_stmCol->vAppend(p_type);
  }

  return p_stream;
}


/*----------------------------------------------------------------------------*/
/* produce composite <stream> of table                                        */
/*----------------------------------------------------------------------------*/
Card_Stream* Card_Stream::pDerive_Of_table(Table* pTable)
{
  _static_

  if (!pTable)
    return nullptr;

  StreamDef* pStreamDef = pTable->pStreamDef();

  Card_Stream* p_stream = new Card_Stream;
  Card_StmColList* p_stmColList = new Card_StmColList;
  p_stream->vAppend(p_stmColList);

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_StmCol* p_stmCol = new Card_StmCol();
    p_stmCol->vValue(pColumn->szColNm());
    p_stmColList->vAppend(p_stmCol);

    Card_StmColNm* p_stmColNm = new Card_StmColNm();
    p_stmColNm->vValue(pColumn->szColNm());
    p_stmCol->vAppend(p_stmColNm);

    Stz* szType = aCT[pColumn->eColumnType()];
    Card_Type *pCard_Type = new Card_Type(szType);
    p_stmCol->vAppend(pCard_Type);
  }

  return p_stream;
}

/*----------------------------------------------------------------------------*/
/* visit each stream and adjust it's lo-hi range                              */
/*----------------------------------------------------------------------------*/
void Card_Stream::vStream_AdjustLoHi(CardIter* pCI)
{
  _static_

  i8 iOdn_shift = 0;

  i8 iLoOdn_running = 0;
  i8 iHiOdn_running = 0;

  // TODO: G_bEachSub(CK_STREAM)

  pCI->G_vRestart();
  while (pCI->G_bEachMain()) {

//    if (! (G.p_MainCard->bIsCard(CK_TABLE) || G.p_MainCard->bIsCard(CK_PROJECT)))
//      continue;

    pCI->G.p_stream = pCI->G.p_MainCard->pSub_stream();
    if (! pCI->G.p_stream)
      continue;

    if (pCI->G.p_MainCard->bIsCard(CK_AGGREG) ||
        pCI->G.p_MainCard->bIsCard(CK_GBYAGG) ||
        pCI->G.p_MainCard->bIsCard(CK_PROJECT)) {
      iOdn_shift = 0;
      iLoOdn_running = 0;
      iHiOdn_running = 0;
    }

    pCI->G.p_stmColList = pCI->G.p_stream->pSub_stmColList();
    i8 nStmCols = pCI->G.p_stmColList->nChildCards();

    i8 iLoOdn_stream = pCI->G.p_stream->iGetOdnLow();
    if (C_UNKNOWN == iLoOdn_stream) {
      i8 iLoOdn_adjusted = iHiOdn_running + 1;
      i8 iHiOdn_adjusted = iLoOdn_adjusted + nStmCols - 1;
      pCI->G.p_stream->vAdjust_OrdinalRange(iLoOdn_adjusted, iHiOdn_adjusted);

      iOdn_shift = iHiOdn_adjusted;
      iLoOdn_running = iLoOdn_adjusted;
      iHiOdn_running = iHiOdn_adjusted;

    }
    else {
      i8 iLoOdn_adjusted = iOdn_shift + 1;
      i8 iHiOdn_adjusted = iLoOdn_adjusted + nStmCols - 1;

      pCI->G.p_stream->vAdjust_OrdinalRange(iLoOdn_adjusted, iHiOdn_adjusted);
      iOdn_shift = iHiOdn_adjusted;

      iLoOdn_running = iLoOdn_adjusted;
      iHiOdn_running = iHiOdn_adjusted;
    }

  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Stream::vAdjust_OrdinalRange(i8 iLo, i8 iHi)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  this->Y_vSet(P_OdnLo, iLo);
  this->Y_vSet(P_OdnHi, iHi);

  this->vMark1(MARK_CHANGED);
}

/*----------------------------------------------------------------------------*/
/* get stream ordinal low                                                     */
/*----------------------------------------------------------------------------*/
i8 Card_Stream::iGetOdnLow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 iOdnLo = Y_iGet(P_OdnLo);

  return iOdnLo;
}

/*----------------------------------------------------------------------------*/
/* get stream ordinal high                                                    */
/*----------------------------------------------------------------------------*/
i8 Card_Stream::iGetOdnHigh(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 iOdnHi = Y_iGet(P_OdnHi);

  return iOdnHi;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Stream::vDerive_OrdinalRange_aggreg(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nStreamCols = this->pSub_stmColList()->nChildCards();
  this->Y_vSet(P_OdnLo, (i8)1);
  this->Y_vSet(P_OdnHi, nStreamCols);

  this->vMark1(MARK_CHANGED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Stream::vDerive_OrdinalRange_project(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nStreamCols = this->pSub_stmColList()->nChildCards();
  this->Y_vSet(P_OdnLo, (i8)1);
  this->Y_vSet(P_OdnHi, nStreamCols);

  this->vMark1(MARK_CHANGED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Stream::vDerive_OrdinalRange_table(char cTblSyn, i8* iOdnHi, i8* iOdnLo)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  // <scan>/<stream>:iOrdLo,iOdnHi
  i8 nStreamCols = this->pSub_stmColList()->nChildCards();
  this->Y_vSet(P_OdnLo, *iOdnLo);
  *iOdnHi = *iOdnLo + nStreamCols - 1;
  this->Y_vSet(P_OdnHi, *iOdnHi);

  this->vMark1(MARK_CHANGED);

  CardIter xCI(this->pSub_stmColList());
  i8 iTblColOdn = 1;
  i8 iStmColOdn = *iOdnLo;
  while (Card_StmCol* p_stmCol = _pCard_StmCol(xCI.pEachChild(CK_STMCOL))) {
    Card_StmColOdn* p_stmColOrd = new Card_StmColOdn;
    p_stmColOrd->vSetStreamColOrdinal(iStmColOdn);
    p_stmCol->vAppend(p_stmColOrd);

    Card_TblColOdn* p_tblColOrd = new Card_TblColOdn;
    p_tblColOrd->vSetTableColOrdinal(cTblSyn, iTblColOdn);
    p_stmCol->vAppend(p_tblColOrd);

    iStmColOdn++;
    iTblColOdn++;
  }

  *iOdnLo = *iOdnHi + 1;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
