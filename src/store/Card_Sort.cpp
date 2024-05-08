/*============================================================================*/
/* Card_Sort.cpp -- Sort                                                      */
/*============================================================================*/

#include "Card_Sort.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Sort, CK_o_SORT)
__aNewFunction(Card_Sort,CN_CARD)
__vDeleteFunction(Card_Sort,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Sort::Card_Sort(void)
: Card_OP(CK_o_SORT, 1)
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
/* derive projection key                                                      */
/*----------------------------------------------------------------------------*/
void Card_Sort::vDerive_streamKey(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bPeg_Projection(pCI));
  RASSERT(bPeg_Key(pCI));

  vDeleteSub(CK_p_STREAMKEY);
  pCI->E.p_streamKey = pCI->StreamOrder.m_pStreamKey = Card_StreamKey::pTo_streamKey(pCI->E.p_key);
  vAppendChild(pCI->StreamOrder.m_pStreamKey);

  // set running emit for use by next op
  pCI->StreamOrder.m_pEmit = pCI->E.p_emit;
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Sort::vSmooth(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(pCI->E.p_Op == pCI->E.p_sort);

  pCI->E.p_key = pCI->E.p_sort->pSub_key();

  if (pCI->Smooth.Sort.m_bCorrect_After_Inject)
    pCI->E.p_key->vBump_expCol(pCI);

  if ((pCI->E.p_emit = pCI->E.p_sort->pSub_emit()))
    pCI->E.p_emit->vDeleteAllChildren();
  else {
    pCI->E.p_emit = new Card_Emit;
    pCI->E.p_sort->vAppendChild(pCI->E.p_emit);
  }

  pCI->Arriving.m_pEmitted_prior->vInduct(pCI);

  for (Kid xEmit_sort(pCI->E.p_emit); xEmit_sort.bEach(); /**/) {
    Card_Expr* p_expr_emit_sort = xEmit_sort.p_expr();
    Card_Expr* p_expr_key = pCI->E.p_key->pFind_expr(pCI, p_expr_emit_sort->pSymbol());
    if (! p_expr_key)
      continue;
    Symbol* pSym_expr_key = p_expr_key->pSymbol();
    p_expr_emit_sort->pGet_term(0)->pSymbol()->vApplyFacets(pSym_expr_key);
   }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Sort::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  vValidate_child_mustBe(CK_p_LEVEL, CK_p_KEY, CK_p_EMIT, CK_NONE);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  Card_OP::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
