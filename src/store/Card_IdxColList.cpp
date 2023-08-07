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
    : Card_symbol(CK_IDXCOLLIST)
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

     pCI->G.p_expr = pCI->G.p_projection->pFindExpr(pCI->G.p_idxCol->pSymbol());
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
  while (xCI.G_bEachMain(CK_IDXCOL)) {
    Stz* szColNm = xCI.G.p_idxCol->szSymbol();

    CardIter xCI_p(p_projection);
    while (xCI_p.G_bEachMain_reverse(CK_EXPR)) {
      if (xCI_p.G.p_expr->pSub_as()->bSameSymbol(szColNm)) {
        Stz* szPrjCol = xCI_p.G.p_expr->pGetTerm(0)->szSymbol();

        xCI.G.p_idxCol->vUpdateSymbol(Symbol(szPrjCol, SF_OTHER));
        xCI.G.p_idxCol->vMark1(MARK_CHANGED);
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_IdxCol* Card_IdxColList::pGetIdxCol_byIdxColOdn(Stz* szIdxColOdn)
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
