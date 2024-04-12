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
  pCI->E.p_emit->vDelete_streamKey(pCI);


  pCI->E.p_streamKey = pCI->StreamOrder.m_pStreamKey = Card_StreamKey::pTo_streamKey(pCI->E.p_key);
  pCI->E.p_emit->vAppendChild(pCI->StreamOrder.m_pStreamKey);


  // set running projection for use by next op
  pCI->StreamOrder.m_pEmit = pCI->E.p_emit;
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Sort::vSmooth(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCarryForward);

  pCI->E.p_key = pCI->E.p_sort->pSub_key();

//Z_KEY_DIRECTION
  if (pCarryForward->Sort.m_bCorrect_After_Inject) {
    for (pCI->vBeginTarget(pCI->E.p_key,CK_EXPR); pCI->bEachTarget(); /**/) {
      for (pCI->vBeginTarget(pCI->E.xTarget.p_expr(),CK_TERM); pCI->bEachTarget(); /**/) {
        pCI->E.p_term = pCI->E.xTarget.p_term();
        if (pCI->E.p_term->pSymbol()->bHasFacet(SF_EXP_COL)) {
          pCI->E.p_term->vBump_ordCol(SF_EXP_COL);
          pCI->E.p_term->vMark1(MARK_CHANGED);
        }
      }
    }
  }


  if ((pCI->E.p_emit = pCI->E.p_sort->pSub_emit()))
    pCI->E.p_sort->vDeleteSub(pCI->E.p_emit);

  Card_Emit* p_emit_sort =  pCI->Arriving.m_pEmit->pInduct(pCI);

  for (Kid xEmit_sort(p_emit_sort); xEmit_sort.bEach(); /**/) {
    Card_Expr* p_expr_emit_sort = xEmit_sort.p_expr();
    Card_Expr* p_expr_key = pCI->E.p_key->pDecode_scalar_expr_Via_symbol(pCI,p_expr_emit_sort->pSymbol());
    if (! p_expr_key)
      continue;
    Symbol* pSym_expr_key = p_expr_key->pSymbol();
    p_expr_emit_sort->pGet_term(0)->pSymbol()->vApplyFacets(pSym_expr_key);
   }



  pCI->E.p_sort->vAppendChild(p_emit_sort);

  pCI->Dic.m_p_expr = pCI->Arriving.m_pEmit = pSub_emit();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Sort::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  vValidate_mustHave(CK_s_LEVEL, CK_s_KEY, CK_t_EMIT, CK_NONE);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  Card_OP::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
