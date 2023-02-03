/*============================================================================*/
/* Card_Constraint.cpp -- Filter                                                  */
/*============================================================================*/

#include "Card_Constraint.hpp"

#include "Card_Expr.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Term.hpp"
#include "Card_Type.hpp"
#include "Card_SubSet.hpp"
#include "Card_AndPred.hpp"
#include "Card_OrPred.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Constraint,CK_CONSTRAINT)
__aNewFunction(Card_Constraint,CN_CARD)
__vDeleteFunction(Card_Constraint,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Constraint::Card_Constraint(void)
: Card(CK_CONSTRAINT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Constraint::~Card_Constraint(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Constraint::vConvert_stmColOdn_To_stmColNm(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_Main = pCI->G.pDeck->iFindChild(pCI->G.p_MainCard);

  Card_BoolCalc* p_boolCalc = pCI->G.p_constraint->pSub_boolCalc();
  RASSERT(1 == p_boolCalc->nChildCards());

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI,i_Main](Card_PRED* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    if (! Card::bIsStreamColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    Stz* szStmColOdn = p_term->szValue();
    pCI->G_bResolveSubCard(CK_STMCOL, CK_STMCOLODN, szStmColOdn, i_Main + 1);

    Stz* szColNm;
    if (pCI->G.p_stmCol) {
      Card_StmColNm* p_stmColNm = pCI->G.p_stmCol->pSub_stmColNm();
      szColNm = p_stmColNm->szValue();
    }
    else {

      pCI->G.p_table = pCI->G.p_MainCard->pSub_table();
      pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection();
      i8 nExprs = pCI->G.p_projection->nChildCards();
      i8 i_Odn = p_term->iGetOdnNo();
      i8 i_Col_base0 = i_Odn - nExprs - 1;

      szColNm = pCI->G.p_table->szColNm(i_Col_base0);
      
      vDebugPoint();

    }


    p_term->vSetValueMark1(szColNm, MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
