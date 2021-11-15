/*============================================================================*/
/* Eval_Filter.cpp -- Filter Evaluator                                        */
/*============================================================================*/

#include "Eval_Filter.hpp"

#include "Dissect.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Eval_Filter,CN_EVAL_FILTER)
__aNewFunction(Eval_Filter,CN_EVAL_FILTER)
__vDeleteFunction(Eval_Filter,CN_EVAL_FILTER)
__aFooterFunction(Eval_Filter)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Eval_Filter::Eval_Filter(void)
: Eval(CN_EVAL_FILTER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EVAL_FILTER,_FOOTER_);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_EVAL_FILTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Eval_Filter::~Eval_Filter(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EVAL_FILTER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* evaluate filter                                                            */
/*----------------------------------------------------------------------------*/
void Eval_Filter::vEvaluateExpr(Card_Expr* pCard_Expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EVAL_FILTER,_FOOTER_);
  
  Card* pCard_PredA = nullptr;
  Card* pCard_PredB = nullptr;

  Card_OrPred*      pCard_OrPred      = nullptr;
  Card_AndPred*     pCard_AndPred     = nullptr;
  Card_ComparePred* pCard_ComparePred = nullptr;
  
  Card_Expr* pCard_Expr_nested = new Card_Expr;
  Card_Expr* pCard_Expr_others = new Card_Expr;
  
  while (Card* pPoppedCard = pCard_Expr->pPop()) {
    
    Card_Atom* pCard_Atom = _pCard_Atom(pPoppedCard);
    Stz* szAtom = pCard_Atom->szAtom();
    eAE eAtomEval = Eval::eAtomEval(szAtom);
    
    switch (eAtomEval) {
        
      case AE_OR:
        if (pCard_ComparePred) {
          pCard_Expr_nested->vAppend(pCard_ComparePred);
          pCard_ComparePred = nullptr;
        }
        
        pCard_OrPred = new Card_OrPred;

        pCard_PredA = pCard_Expr_nested->pPop();
        pCard_PredB = pCard_Expr_nested->pPop();

        pCard_OrPred->vAppend(pPoppedCard);
        pCard_OrPred->vAppend(pCard_PredA);
        pCard_OrPred->vAppend(pCard_PredB);
        
        pCard_Expr_nested->vAppend(pCard_OrPred);
        pCard_OrPred = nullptr;
        break;

      case AE_AND:
        if (pCard_ComparePred) {
          pCard_Expr_nested->vAppend(pCard_ComparePred);
          pCard_ComparePred = nullptr;
        }
        
        pCard_AndPred = new Card_AndPred;

        pCard_PredA = pCard_Expr_nested->pPop();
        pCard_PredB = pCard_Expr_nested->pPop();
        
        pCard_AndPred->vAppend(pPoppedCard);
        pCard_AndPred->vAppend(pCard_PredA);
        pCard_AndPred->vAppend(pCard_PredB);
        
        pCard_Expr_nested->vAppend(pCard_AndPred);
        pCard_AndPred = nullptr;
        break;
        
      case AE_COMPARE:
        pCard_ComparePred->vAppend(pPoppedCard);
        pCard_Expr_nested->vAppend(pCard_ComparePred);
        pCard_ComparePred = nullptr;
        break;

      case AE_OTHER:
        if (! pCard_ComparePred)
          pCard_ComparePred = new Card_ComparePred;
        pCard_ComparePred->vAppend(pPoppedCard);
        break;

      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        
      case AE_BOOLEAN:
        pCard_Expr_others->vAppend(pPoppedCard);
        break;
        
      case AE_TYPE: {
        //pCard_Expr_others->vAppend(pPoppedCard);
        //pCard_Expr_others->vAppend(pPoppedCard);
        
        Card_Atom* pCard_Atom_column = _pCard_Atom(pPoppedCard);
        eCT eColumn = Column::eColumn(pCard_Atom_column->szAtom());
        Card_Type* pCard_Type = new Card_Type(eColumn);
        pCard_Expr->vAppend(pCard_Type);
        break;
      }
        
      default:
        pCard_Expr_others->vAppend(pPoppedCard);
        
        break;
    }

  }
  
  // dangling compare
  if (pCard_ComparePred) {
    pCard_Expr_nested->vAppend(pCard_ComparePred);
    pCard_ComparePred = nullptr;
  }
  
  while (Card* pPoppedCard = pCard_Expr_others->pPop()) {
    pCard_Expr->vAppend(pPoppedCard);
  }
  while (Card* pPoppedCard = pCard_Expr_nested->pPop()) {
    pCard_Expr->vAppend(pPoppedCard);
  }

  delete pCard_Expr_nested;
  delete pCard_Expr_others;
}

/*----------------------------------------------------------------------------*/
/* evaluate filter                                                            */
/*----------------------------------------------------------------------------*/
void Eval_Filter::vEvaluateFilter(Card_Filter* pCard_Filter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EVAL_FILTER,_FOOTER_);
  
  Card* pCard_PredA = nullptr;
  Card* pCard_PredB = nullptr;

  Card_OrPred*      pCard_OrPred      = nullptr;
  Card_AndPred*     pCard_AndPred     = nullptr;
  Card_ComparePred* pCard_ComparePred = nullptr;
  
  Card_Filter* pCard_Filter_nested = new Card_Filter;
  Card_Filter* pCard_Filter_others = new Card_Filter;

  Card_Expr* pCard_Expr = _pCard_Expr(pCard_Filter->pPop());
  
  while (Card* pPoppedCard = pCard_Filter->pPop()) {
    
    Card_Atom* pCard_Atom = _pCard_Atom(pPoppedCard);
    Stz* szAtom = pCard_Atom->szAtom();
    eAE eAtomEval = Eval::eAtomEval(szAtom);
    
    switch (eAtomEval) {
        
      case AE_OR:
        if (pCard_ComparePred) {
          pCard_Filter_nested->vAppend(pCard_ComparePred);
          pCard_ComparePred = nullptr;
        }
        
        pCard_OrPred = new Card_OrPred;

        pCard_PredA = pCard_Filter_nested->pPop();
        pCard_PredB = pCard_Filter_nested->pPop();

        pCard_OrPred->vAppend(pPoppedCard);
        pCard_OrPred->vAppend(pCard_PredA);
        pCard_OrPred->vAppend(pCard_PredB);
        
        pCard_Filter_nested->vAppend(pCard_OrPred);
        pCard_OrPred = nullptr;
        break;

      case AE_AND:
        if (pCard_ComparePred) {
          pCard_Filter_nested->vAppend(pCard_ComparePred);
          pCard_ComparePred = nullptr;
        }
        
        pCard_AndPred = new Card_AndPred;

        pCard_PredA = pCard_Filter_nested->pPop();
        pCard_PredB = pCard_Filter_nested->pPop();
        
        pCard_AndPred->vAppend(pPoppedCard);
        pCard_AndPred->vAppend(pCard_PredA);
        pCard_AndPred->vAppend(pCard_PredB);
        
        pCard_Filter_nested->vAppend(pCard_AndPred);
        pCard_AndPred = nullptr;
        break;
        
      case AE_COMPARE:
        pCard_ComparePred->vAppend(pPoppedCard);
        pCard_Filter_nested->vAppend(pCard_ComparePred);
        pCard_ComparePred = nullptr;
        break;

      case AE_OTHER:
        if (! pCard_ComparePred)
          pCard_ComparePred = new Card_ComparePred;
        pCard_ComparePred->vAppend(pPoppedCard);
        break;

      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        
      case AE_BOOLEAN:
        pCard_Filter_others->vAppend(pPoppedCard);
        break;
        
      case AE_TYPE: {
        //pCard_Filter_others->vAppend(pPoppedCard);
        //pCard_Filter_others->vAppend(pPoppedCard);
        
        Card_Atom* pCard_Atom_column = _pCard_Atom(pPoppedCard);
        eCT eColumn = Column::eColumn(pCard_Atom_column->szAtom());
        Card_Type* pCard_Type = new Card_Type(eColumn);
        pCard_Expr->vAppend(pCard_Type);
        break;
      }
        
      default:
        pCard_Filter_others->vAppend(pPoppedCard);
        
        break;
    }

  }
  
  // dangling compare
  if (pCard_ComparePred) {
    pCard_Filter_nested->vAppend(pCard_ComparePred);
    pCard_ComparePred = nullptr;
  }
  
  while (Card* pPoppedCard = pCard_Filter_others->pPop()) {
    pCard_Expr->vAppend(pPoppedCard);
  }
  while (Card* pPoppedCard = pCard_Filter_nested->pPop()) {
    pCard_Expr->vAppend(pPoppedCard);
  }

  pCard_Filter->vAppend(pCard_Expr);

  delete pCard_Filter_nested;
  delete pCard_Filter_others;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
