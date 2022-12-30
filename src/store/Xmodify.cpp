/*============================================================================*/
/* Xmodify.cpp --                                                       */
/*============================================================================*/

#include "Xmodify.hpp"

#include "Table.hpp"
#include "Scope.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Xmodify)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xmodify::Xmodify(eCN eClass)
: Xsentinals(eClass)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_iOdnLo      = 1;
  m_iOdnHi      = C_UNKNOWN;
  m_cTableSyn   = 'a';
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xmodify::~Xmodify(void) noexcept(false)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Xmodify::E_iELIST_rpnCalc_Has_stmOrdOdn(Card_ELIST* p_ELIST, Card_OrdElmOdn* p_stmOrdOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Stz* szStrOrdOdn = p_stmOrdOdn->szValue();

  CardIter xCI_elist(p_ELIST);
  while (Card_Expr* p_expr = _pCard_Expr(xCI_elist.pEachChild())) {

    Card_RpnCalc* p_rpnCalc = p_expr->pSub_rpnCalc();
    if (! p_rpnCalc)
      continue;

    if (p_rpnCalc->nChildCards() > 1)
      continue;

    CardIter xCI_rpnCalc(p_rpnCalc);
    while (Card_Term* p_term = _pCard_Term(xCI_rpnCalc.pEachChild())) {
      if (p_term->bIsValue(szStrOrdOdn))
        return xCI_elist.i_Child();
    }
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Xmodify::E_iExpr_rpnCalc_WhichGbyKey(Card_Expr* p_expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_RpnCalc* p_rpnCalc = p_expr->pSub_rpnCalc();
  if (! p_rpnCalc)
    return C_UNKNOWN;

  if (p_rpnCalc->nChildCards() > 1)
    return C_UNKNOWN;

  CardIter xCI(p_rpnCalc);
  while (Card_Term* p_term = _pCard_Term(xCI.pEachChild())) {
    if (Card::bIsKeyOrdinal(p_term->szValue())) {
      i8 i_Key = Card::iGetColOdn(p_term->szValue());
      return i_Key;
    }
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vStream_Derive_OrdinalRange_table(Card_Stream* p_stream, char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // <scan>/<stream>:iOrdLo,iOdnHi
  i8 nStreamCols = p_stream->pSub_stmColList()->nChildCards();
  p_stream->Y_vSet(P_OdnLo, m_iOdnLo);
  m_iOdnHi = m_iOdnLo + nStreamCols - 1;
  p_stream->Y_vSet(P_OdnHi, m_iOdnHi);

  p_stream->vMark1(MARK_CHANGED);

  CardIter xCI(p_stream->pSub_stmColList());
  i8 iTblColOdn = 1;
  i8 iStmColOdn = m_iOdnLo;
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

  m_iOdnLo = m_iOdnHi + 1;
}

/*----------------------------------------------------------------------------*/
/* set synonym of table                                                       */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vTable_Set_Syn(Card_Table* p_table)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_TableNm* p_tableNm = p_table->pSub_tableNm();

  Table* pTable = Card::pAccess_Table(p_tableNm->sz());

  p_table->vMark1(MARK_CHANGED);

  char cTableSyn_actual = m_cTableSyn;
  if (pTable->bIsReplicated())
    cTableSyn_actual = StrLib::cToUpper(cTableSyn_actual);

  p_table->vValueSprint("%c"_Stz,cTableSyn_actual);

  m_cTableSyn++;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vTerm_Bump_KeyOrdinal(Card_Term* p_term, i8 nBy)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 iKeyOdn = Card::iGetColOdn(p_term->szValue());

  p_term->vSetKeyOrdinal(iKeyOdn+nBy);

  if (p_term->Y_bHas(P_BumpedBy)) {
    i8 nBumpedBy = p_term->Y_iGet(P_BumpedBy);
    p_term->Y_vSet(P_BumpedBy,nBumpedBy+nBy);
  }
  else {
    p_term->Y_vSet(P_BumpedBy,(i8)nBy);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vTerm_Bump_StreamColOrdinal(Card_Term* p_term, i8 nBy)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 iStmColOdn = Card::iGetColOdn(p_term->szValue());

  p_term->vSetStreamColOrdinal(iStmColOdn+nBy);

  if (p_term->Y_bHas(P_BumpedBy)) {
    i8 nBumpedBy = p_term->Y_iGet(P_BumpedBy);
    p_term->Y_vSet(P_BumpedBy,nBumpedBy+nBy);
  }
  else {
    p_term->Y_vSet(P_BumpedBy,(i8)nBy);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vTerm_MarkIfMatchesAn_idxCol(Card_Term* p_term, Card_IdxColList* p_idxColList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Stz* szTerm = p_term->sz();
  if (!szTerm->bIsStreamColOrd())
    return;

  i8 i_IdxCol = C_UNKNOWN;

  Card_IdxCol* p_idxCol =
      _pCard_IdxCol(p_idxColList->pFindChild(CK_IDXCOL, szTerm, i_IdxCol));
  if (! p_idxCol)
    return;

  p_term->vMark1('I');
  p_term->vMark2(i_IdxCol);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xmodify::bPRED_Descend(Card_PRED* p_pred,
                            std::function<bool(Card_PRED* )> bfLambdaFn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(p_pred);
  while (Card* pChildCard = xCI.pEachChild()) {
    if (! pChildCard->bIsPRED())
      continue;

    bool bContinue = bPRED_Descend((Card_PRED*)pChildCard, bfLambdaFn);
    if (! bContinue)
      return C_TERMINATE_RECURSION;
  }

  return C_CONTINUE_RECURSION;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::vStreamAnalysisReset(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_iOdnLo      = 1;
  m_iOdnHi      = C_UNKNOWN;
  m_cTableSyn   = 'a';
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
