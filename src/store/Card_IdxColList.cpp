/*============================================================================*/
/* Card_IdxColList.cpp -- Index Column List                                   */
/*============================================================================*/

#include "Card_IdxCol.hpp"
#include "Card_IdxColList.hpp"
#include "Card_StmCol.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"

#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_IdxColList,CK_IDXCOLLIST)
__aNewFunction(Card_IdxColList,CN_CARD)
__vDeleteFunction(Card_IdxColList,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IdxColList::Card_IdxColList(void)
    : Card(CK_IDXCOLLIST)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IdxColList::~Card_IdxColList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_IdxColList::vAdd_key_exprs(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nChildren = this->nChildCards();
  for (i8 i_idxCol = 0; i_idxCol < nChildren; i_idxCol++) {

    pCI->G.p_idxCol = _pCard_IdxCol(pGetChild(i_idxCol));
    pCI->G.p_idxColNm = pCI->G.p_idxCol->pSub_idxColNm();

    pCI->G.p_expr = pCI->G.p_projection->pFind_expr_By_colNm(pCI, pCI->G.p_idxColNm->szValue());
    if (!pCI->G.p_expr)
      continue;

    pCI->G.p_key->vAdd_key_expr_BasedOn_expr(pCI->G.p_expr,true);
  }
  //pCI->G.p_key->vDump();
}


/*----------------------------------------------------------------------------*/
/* affix each idxCol with it's $c.n of this table stream                      */
/*----------------------------------------------------------------------------*/
void Card_IdxColList::vAffix_tblColOdn(Card_Projection* p_projection)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_ProjectionColNo = C_UNKNOWN;

  CardIter xCI(this);
  while (Card_IdxCol* p_idxCol = _pCard_IdxCol(xCI.pEachChild(CK_IDXCOL))) {
    Stz* szColNm = p_idxCol->sz();

  CardIter xCI_p(p_projection);
  while (Card_Expr* expr_prj = _pCard_Expr(xCI_p.pEachChild())) {
    if (expr_prj->pSub_as()->bSame(szColNm)) {

//        Card_TblColOdn* p_tblColOdn = expr_prj->pSub_tblColOdn();
//        Stz* szTblColOdn = p_tblColOdn->sz();
        Stz* szTblColOdn = expr_prj->szProjectedCol();

        p_idxCol->vSetValueMark1(szTblColOdn, MARK_CHANGED);
      }
    }
  }
}
/*----------------------------------------------------------------------------*/
//void Card_IdxColList::vAffix_tblColOdn(Card_Projection* p_projection)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//  i8 i_ProjectionColNo = C_UNKNOWN;
//
//  CardIter xCI(this);
//  while (Card_IdxCol* p_idxCol = _pCard_IdxCol(xCI.pEachChild(CK_IDXCOL))) {
//    Stz* szColNm = p_idxCol->sz();
//
//    CardIter xCI_p(p_projection);
//    Card_Expr* expr;
//    while (expr = _pCard_Expr(xCI_p.pEachChild())) {
//      if (expr->pSub_colNm()->bSame(szColNm)) {
//        Card_TblColOdn* p_tblColOdn = expr->pSub_tblColOdn();
//        Stz* szTblColOdn = p_tblColOdn->sz();
//
//        p_idxCol->vSetValueMark1(szTblColOdn, MARK_CHANGED);
//      }
//    }
//  }
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_IdxCol* Card_IdxColList::pIdxCol_byIdxColOdn(Stz* szIdxColOdn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_IdxCol = StrLib::iGleanInt(szIdxColOdn->azChar()) - 1;

  if (i_IdxCol < 0 || i_IdxCol >= nChildCards())
    return nullptr;

  Card_IdxCol* pIdxCol = _pCard_IdxCol(pGetChild(i_IdxCol));

  return pIdxCol;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
