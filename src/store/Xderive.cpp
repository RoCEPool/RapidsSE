/*============================================================================*/
/* Xderive.cpp --                                                       */
/*============================================================================*/

#include "Xderive.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Xderive)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xderive::Xderive(eCN eClass)
: Xmodify(eClass)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xderive::~Xderive(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A */
/*----------------------------------------------------------------------------*/






/*----------------------------------------------------------------------------*/
/*T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------*/
/*X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Xderive::D_pDerive_Gby_expr_From_stmCol(Card_StmCol* p_stmCol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Expr*    p_expr    = new Card_Expr;
  Card_Type*    p_type    = new Card_Type(p_stmCol->pSub_type()->szValue());
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = new Card_Term(p_stmCol->pSub_stmColOdn()->szValue());

  p_expr->vAppend(p_type);
  p_expr->vAppend(p_rpnCalc);
  p_rpnCalc->vAppend(p_term);

  return p_expr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Xderive::D_pDerive_Proj_expr_From_Gby_expr(Card_Expr* p_expr_gby, Stz* szAs)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  s32 sKn;
  sKn.vSprint("$K%lld"_Stz,p_expr_gby->iCardNo()+1);

  Card_Expr*    p_expr    = new Card_Expr;
  Card_Type*    p_type    = new Card_Type(p_expr_gby->pSub_type()->szValue());
  Card_As*      p_as      = szAs ? new Card_As(szAs) : nullptr;
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = new Card_Term(sKn.szStz());

  p_expr->vAppend(p_type);
  if (p_as)
    p_expr->vAppend(p_as);
  p_expr->vAppend(p_rpnCalc);
  p_rpnCalc->vAppend(p_term);

  return p_expr;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Xderive::D_pDerive_Proj_expr_From_stmCol(Card_StmCol* p_stmCol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Expr*    p_expr    = new Card_Expr;
  Card_Type*    p_type    = new Card_Type(p_stmCol->pSub_type()->szValue());
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = new Card_Term(p_stmCol->pSub_stmColOdn()->szValue());

  p_expr->vAppend(p_type);
  p_expr->vAppend(p_rpnCalc);
  p_rpnCalc->vAppend(p_term);

  return p_expr;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xderive::D_vDerive_ObyElist(Card_ObyElist* p_obyElist, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  IndexDef* pIndexDef = pIndex->pIndexDef();
  i8 nIdxCols = pIndexDef->nColumns();

  for (i8 i_IdxCol = 0; i_IdxCol < nIdxCols; i_IdxCol++) {

    Column* pIdxCol = pIndexDef->pGetColumn(i_IdxCol);

    Card_Expr* pCard_Expr = new Card_Expr;
    p_obyElist->vAppend(pCard_Expr);


    Stz* szType = aCT[pIdxCol->eColumnType()];
    Card_Type* pCard_Type = new Card_Type(szType);
    pCard_Expr->vAppend(pCard_Type);

    // as
    Card_As* pCard_As = new Card_As(pIdxCol->szColNm());
    pCard_Expr->vAppend(pCard_As);

    // exprCalc
    Card_RpnCalc* pCard_RpnCalc = new Card_RpnCalc;
    pCard_Expr->vAppend(pCard_RpnCalc);

    Card_Term* pCard_Term = new Card_Term(pIdxCol->szColNm());
    pCard_RpnCalc->vAppend(pCard_Term);

  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
char Xderive::D_cPRED_TableSyn_First(Card_PRED* p_PRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  char cTblSyn = ' ';
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this,&cTblSyn](Card_Term* p_term)
  {
    if (! Card::bIsTableColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    char cTblSyn_test = Card::cTableColSyn((p_term->szValue()));
    if (' ' == cTblSyn)
      cTblSyn = cTblSyn_test;
    else
    if (cTblSyn_test < cTblSyn)
      cTblSyn = cTblSyn_test;

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  p_PRED->vApplyRecursive_term(bLambdaFn);

  return cTblSyn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Xderive::D_pProject_Simple_expr_Of(Card_Expr* p_expr, i8 i_Col)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  //Card_Expr* p_expr_simple = _pCard_Expr(p_expr->pDupCardAndSubCards());

  Card_Expr* p_expr_simple = new Card_Expr;

  p_expr_simple->vAppend(p_expr->pSub_type()->pDupCardAndSubCards());

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term* p_term = new Card_Term;
  p_term->vValueSprint("$%lld"_Stz,i_Col);
  p_rpnCalc->vAppend(p_term);

  p_expr_simple->vAppend(p_rpnCalc);

  p_expr_simple->vAppend(p_expr->pSub_as()->pDupCardAndSubCards());

  return p_expr_simple;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
