/*============================================================================*/
/* Card_Sort.cpp -- Sort                                                      */
/*============================================================================*/

#include "Card_Sort.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Sort, CK_m_SORT)
__aNewFunction(Card_Sort,CN_CARD)
__vDeleteFunction(Card_Sort,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Sort::Card_Sort(void)
: Card_OP(CK_m_SORT, 1)
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
  pCI->G.p_emit->vDelete_streamKey(pCI);


  pCI->G.p_streamKey = pCI->StreamOrder.m_pStreamKey = Card_StreamKey::pTo_streamKey(pCI->G.p_key);
  pCI->G.p_emit->vAppendChild(pCI->StreamOrder.m_pStreamKey);


  // set running projection for use by next op
  pCI->StreamOrder.m_pEmitted = pCI->G.p_emit;
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Sort::vSmooth(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  UNUSED(pCarryForward);

  pCI->G.p_key = pCI->G.p_sort->pSub_key();

  if (pCarryForward->Sort.m_bCorrect_After_Inject)
    pCI->G.p_key->vBump_expCol_In_rpnCalc(pCI);

  vEnsure_stream(pCI);

  if ((pCI->G.p_emit = pCI->G.p_sort->pSub_emit()))
    pCI->G.p_sort->vDeleteSub(pCI->G.p_emit);

  Card_Emit* p_emitted_sort =  pCI->Flux.m_pArriving->pPropagate();
  for (Kid xKey_sort(pSub_key()); xKey_sort.bEach(); /**/) {
    Card_Expr* p_expr_key = xKey_sort.p_expr();

    Symbol* pSym_expr_x = p_expr_key->pFirst_expCol();
    Card_Expr* p_expr_prj = p_emitted_sort->pGet_expr(pSym_expr_x);

    p_expr_prj->pGet_term(0)->pSymbol()->vCastFacets(p_expr_key->pSymbol());
  }

  pCI->G.p_stream->vAppendChild(p_emitted_sort);

  pCI->Flux.m_pArriving = pSub_emit();

 // pCI->vRenumberCards();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Sort::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  vValidate_mustHave(CK_s_LEVEL, CK_s_KEY, CK_s_STREAM, CK_NONE);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  Card_OP::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
