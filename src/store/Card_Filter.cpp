/*============================================================================*/
/* Card_Filter.cpp -- Filter                                                  */
/*============================================================================*/

#include "Card_Filter.hpp"

#include "Card_Expr.hpp"
#include "Card_Calc.hpp"
#include "Card_CalcBool.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Term.hpp"
#include "Card_Type.hpp"
#include "Card_SubSet.hpp"
#include "Card_AndPred.hpp"
#include "Card_OrPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Filter,CK_FILTER)
__aNewFunction(Card_Filter,CN_CARD)
__vDeleteFunction(Card_Filter,CN_CARD)

__bIsCard(STORE,Card_Filter,CK_FILTER)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Filter::Card_Filter(void)
: Card(CK_FILTER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Filter::~Card_Filter(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Filter::bCull(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Expr* pCard_Expr =
    _pCard_Expr(pFindSubCard(CK_EXPR));

  if (pCard_Expr->pFindSubCard(CK_EXPRBOOL))
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* stream                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Filter::vCalc_to_calcBool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(CK_FILTER == eCard());

  RASSERT(1 == nChildCards());

  Card_Expr* pCard_Expr = _pCard_Expr(pGetChild(0));
  RASSERT(2 == pCard_Expr->nChildCards());

  Card_Type* pCard_Type = _pCard_Type(pCard_Expr->pGet(0));
  Card_Calc* pCard_Calc = _pCard_Calc(pCard_Expr->pGet(1));

  Card* pCard_CalcBool = pTo_CalcBool(pCard_Calc);

  pCard_Expr->vRemove(pCard_Calc);
  delete pCard_Calc;
  pCard_Expr->vAppend(pCard_CalcBool);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vCullPred(char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = pGetChild(i_Child);

    eCK eCard = pChildCard->eCard();
    if (eCard != CK_EXPR)
      continue;

    Card_Expr* p_expr = _pCard_Expr(pChildCard);

    p_expr->vCullPred(cWithMark);
  }
}


/*----------------------------------------------------------------------------*/
/* to expr                                                                    */
/*----------------------------------------------------------------------------*/
void Card_Filter::vToExpr(Stz* szExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  szExpr->vCopy(""_Stz);
  
  Card_Type* pCard_Type = _pCard_Type(pGet(0));

  szExpr->vConcat("type "_Stz);
  szExpr->vConcat(pCard_Type->sz());

  Card_CmpPred* pCard_PredCmp = _pCard_CmpPred(pGet(1));
  
  i8 nChildren = pCard_PredCmp->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_Term* pCard_Term = _pCard_Term(pCard_PredCmp->pGetChild(i_Child));
    
    Stz* szTerm = pCard_Term->sz();
    
    szExpr->vConcat(" "_Stz);

    szExpr->vConcat(szTerm);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card_Filter::pFindBound(Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Expr* pCard_Expr =
    _pCard_Expr(pFindSubCard(CK_EXPR));

  Card_CmpPred* pCard_PredCmp = pCard_Expr->pFindBound(pIndex,eBound);
  
  return pCard_PredCmp;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Filter::bHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Expr* pCard_Expr =
    _pCard_Expr(pFindSubCard(CK_EXPR));
  
  bool bHas = pCard_Expr->bHasBounds(pIndex);
  
  return bHas;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Filter::bIsEmptyFilter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CalcBool* pCard_calcBool =
      _pCard_CalcBool(pFindSubCard(CK_CALCBOOL));

  if (0 == pCard_calcBool->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vReduceFilter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Expr* pCard_Expr =
    _pCard_Expr(pFindSubCard(CK_EXPR));

  pCard_Expr->vReduceBoolExpr();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Filter::bRemoveBound(Card_CmpPred* pCard_PredCmp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
    
  vRemoveSubCard(pCard_PredCmp);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* convert a calculation into a boolean calculaiton                           */
/*----------------------------------------------------------------------------*/
Card* Card_Filter::pTo_CalcBool(Card* pCard_Calc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  RASSERT(CK_CALC == pCard_Calc->eCard());

  Card_AndPred* pCard_PredAnd = nullptr;
  Card_CmpPred* pCard_PredCmp = nullptr;
  Card_OrPred*  pCard_PredOr  = nullptr;

  Card_SubSet*  pCard_SubExpr = new Card_SubSet;

  Card_CalcBool* pCard_CalcBool = new Card_CalcBool;

  while (pCard_Calc->nChildCards()) {
    Card_Term *pCard_Term = _pCard_Term(pCard_Calc->pPop());

    eTE eTopTerm = pCard_Term->eTermEval();
    switch (eTopTerm) {

      case TE_AND:
        RASSERT(2 == pCard_CalcBool->nChildCards());
        pCard_PredAnd = new Card_AndPred;
        pCard_PredAnd->vTransferChildren(pCard_CalcBool);
        RASSERT(0 == pCard_CalcBool->nChildCards());
        pCard_CalcBool->vAppend(pCard_PredAnd);
        RASSERT(1 == pCard_CalcBool->nChildCards());
        pCard_PredAnd = nullptr;
        delete pCard_Term;
        break;

      case TE_OR:
        RASSERT(2 == pCard_CalcBool->nChildCards());
        pCard_PredOr = new Card_OrPred;
        pCard_PredOr->vTransferChildren(pCard_CalcBool);
        RASSERT(0 == pCard_CalcBool->nChildCards());
        pCard_CalcBool->vAppend(pCard_PredOr);
        RASSERT(1 == pCard_CalcBool->nChildCards());
        pCard_PredOr = nullptr;
        delete pCard_Term;
        break;

      case TE_COMPARE:
        RASSERT(pCard_SubExpr->nChildCards() > 0);
        pCard_PredCmp = new Card_CmpPred;
        pCard_PredCmp->vValue(pCard_Term->sz());
        pCard_PredCmp->vTransferChildren(pCard_SubExpr);
        pCard_CalcBool->vAppend(pCard_PredCmp);
        pCard_PredCmp = nullptr;
        delete pCard_Term;
        break;

      case TE_OTHER:
        pCard_SubExpr->vAppend(pCard_Term);
        break;

        // - - - - - - -

      case TE_BOOLEAN:
        NEVER_REACH;
        break;

      case TE_TYPE:
        NEVER_REACH;
        break;

      case TE_AS:
        NEVER_REACH;
        break;

      default:
        NEVER_REACH;
        break;
    }
  }

  if (pCard_SubExpr->nItems()) {
    RASSERT(0 == pCard_CalcBool->nChildCards());
    pCard_PredCmp->vTransferChildren(pCard_SubExpr);
    RASSERT(1 == pCard_CalcBool->nChildCards());
    RASSERT(0 == pCard_SubExpr->nChildCards());
  }

  delete pCard_SubExpr;

  return pCard_CalcBool;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
