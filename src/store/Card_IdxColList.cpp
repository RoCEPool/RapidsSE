/*============================================================================*/
/* Card_IdxColList.cpp -- Index Column List                                   */
/*============================================================================*/

#include "Card_IdxCol.hpp"
#include "Card_IdxColList.hpp"
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
    : Card_symbol(CK_IDXCOLLIST, 5)
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
void Card_IdxColList::vAdd_streamKey_exprs(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nChildren = this->nChildCards();
  for (i8 i_idxCol = 0; i_idxCol < nChildren; i_idxCol++) {

    pCI->E.p_idxCol = _pCard_IdxCol(pGetChild(i_idxCol));

     pCI->E.p_expr = pCI->E.p_emit->pFind_expr_Matches(pCI->E.p_idxCol->pSymbol());
    if (!pCI->E.p_expr)
      continue;

     pCI->E.p_streamKey->vAdd_key_expr_BasedOn_expr(pCI->E.p_expr, true);
  }
  //pCI->G.p_key->vDump();
}


/*----------------------------------------------------------------------------*/
/* affix each idxCol with it's $c.n of this table stream                      */
/*----------------------------------------------------------------------------*/
void Card_IdxColList::vAffix_tblCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xIdxColList(this);
  while (xIdxColList.bEach(CK_IDXCOL)) {
    Card_IdxCol* p_idxCol = xIdxColList.p_idxCol();
    Symbol* pSym_idxCol   = p_idxCol->pSymbol();

    Card_Expr* p_expr_inj = pCI->E.p_injectable->pFind_expr_Matches(pSym_idxCol);
    pSym_idxCol->vApplyFacet(SF_TBL_COL, p_expr_inj->szFacet(SF_TBL_COL));
    p_idxCol->vMark1(MARK_CHANGED);

  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_IdxCol* Card_IdxColList::pGetIdxCol_byIdxCol(Stz* szIdxCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_IdxCol = StrLib::iGleanInt(szIdxCol->azChar()) - 1;

  if (i_IdxCol < 0 || i_IdxCol >= nChildCards())
    return nullptr;

  Card_IdxCol* pIdxCol = _pCard_IdxCol(pGetChild(i_IdxCol));

  return pIdxCol;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_IdxColList::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_IDXCOL,CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
