/*============================================================================*/
/* Card_Sort.cpp -- Sort                                                      */
/*============================================================================*/

#include "Card_Sort.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Sort,CK_SORT)
__aNewFunction(Card_Sort,CN_CARD)
__vDeleteFunction(Card_Sort,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Sort::Card_Sort(void)
: Card_OP(CK_SORT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Sort::~Card_Sort(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* column mapping                                                             */
/*----------------------------------------------------------------------------*/
void Card_Sort::vColumn_Mapping(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);


  pCI->G.p_key = pSub_key();
  pCI->G.p_projection = pSub_projection();

  i8 nExpr_key = 0;
  i8 i_Emitted = 1;
  i8 i_Cell_payload = 1;
  i8 i_Cell_key;

  for (Kid xPrjn(pCI->G.p_projection); xPrjn.bEach(CK_EXPR); /**/) {
    Card_Expr *p_expr_prjn = xPrjn.p_expr();

    if (p_expr_prjn->bIsExpressionOfKey()) {

      nExpr_key++;

      Symbol *pSymbol_key = p_expr_prjn->pFirstKeyCol();

      i_Cell_key = pSymbol_key->iExtractOrdinal();

      p_expr_prjn->pSymbol()->Y_vSet(P_EmittedCol, i_Emitted++);
      p_expr_prjn->pSymbol()->Y_vSet(P_Key);
      p_expr_prjn->pSymbol()->Y_vSet(P_Cell, i_Cell_key);
    }
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    else {

      Symbol* pSymbol_stmCol_projection = p_expr_prjn->pFirstStmCol();

      if (pSymbol_stmCol_projection) {
        i_Cell_payload = pSymbol_stmCol_projection->iExtractOrdinal() - nExpr_key;

        p_expr_prjn->pSymbol()->Y_vSet(P_EmittedCol, i_Emitted++);
        p_expr_prjn->pSymbol()->Y_vSet(P_Payload);
        p_expr_prjn->pSymbol()->Y_vSet(P_Cell, i_Cell_payload++);
      }
      else {

        p_expr_prjn->pSymbol()->Y_vSet(P_EmittedCol, i_Emitted++);
        p_expr_prjn->pSymbol()->Y_vSet(P_Payload);
        p_expr_prjn->pSymbol()->Y_vSet(P_Cell, i_Cell_payload++);
      }

    }
  }

}

/*----------------------------------------------------------------------------*/
/* test & cut sort                                                            */
/*----------------------------------------------------------------------------*/
void Card_Sort::vCut_sort(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! pCI->StreamOrder.m_pKey_running)
    return;

  pCI->G.p_key = pSub_key();

  if (! pCI->G.p_key->bIsEqual(pCI->StreamOrder.m_pKey_running))
    return;

  pCI->G.pDeck->vDeleteChild(this);
}

/*----------------------------------------------------------------------------*/
/* derive projection key                                                      */
/*----------------------------------------------------------------------------*/
void Card_Sort::vDerive_Projection_Key(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->StreamOrder.m_pKey_running = pSub_key();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Sort::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_LEVEL,CK_KEY,CK_PROJECTION,CK_NONE);

  pSub_level()->vValidate(pCI,iDepth+1,this);
  pSub_key()->vValidate(pCI,iDepth+1,this);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
