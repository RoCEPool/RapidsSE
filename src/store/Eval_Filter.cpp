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
void Eval_Filter::vEvaluateFilter(Card_Filter* pCard_Filter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EVAL_FILTER,_FOOTER_);
  
  Card* pCard_PredA = nullptr;
  Card* pCard_PredB = nullptr;

  Card_OrPred*      pCard_OrPred      = nullptr;
  Card_AndPred*     pCard_AndPred     = nullptr;
  Card_ComparePred* pCard_ComparePred = nullptr;
  
  Card_Filter* pCard_Filter_nested = new Card_Filter(CK_FILTER);
  Card_Filter* pCard_Filter_others = new Card_Filter(CK_FILTER);

  Card_Expr* pCard_Expr = new Card_Expr(CK_EXPR);
  
  while (Card* pPoppedCard = pCard_Filter->pPop()) {
    
    Card_Term* pCard_Term = _pCard_Term(pPoppedCard);
    Stz* szTerm = pCard_Term->szTerm();
    eAE eAtomEval = Eval::eAtomEval(szTerm);
    
    switch (eAtomEval) {
        
      case AE_OR:
        if (pCard_ComparePred) {
          pCard_Filter_nested->vAppend(pCard_ComparePred);
          pCard_ComparePred = nullptr;
        }
        
        pCard_OrPred = new Card_OrPred(CK_ORPRED);

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
        
        pCard_AndPred = new Card_AndPred(CK_ANDPRED);

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
          pCard_ComparePred = new Card_ComparePred(CK_COMPAREPRED);
        pCard_ComparePred->vAppend(pPoppedCard);
        break;

      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        
      case AE_BOOLEAN:
        pCard_Filter_others->vAppend(pPoppedCard);
        break;
        
      case AE_TYPE: {
        //pCard_Filter_others->vAppend(pPoppedCard);
        //pCard_Filter_others->vAppend(pPoppedCard);
        
        pPoppedCard = pCard_Filter->pPop();
        Card_Term* pCard_Term_column = _pCard_Term(pPoppedCard);
        //eCT eColumn = Column::eColumn(pCard_Term_column->szTerm());
        Card_Type* pCard_Type = new Card_Type(CK_TYPE,
                                              pCard_Term_column->szTerm());
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
  
  pCard_Filter->vTransferAll(pCard_Expr);
  
  while (Card* pPoppedCard = pCard_Filter_others->pPop()) {
    pCard_Expr->vAppend(pPoppedCard);
  }
  while (Card* pPoppedCard = pCard_Filter_nested->pPop()) {
    pCard_Expr->vAppend(pPoppedCard);
  }

  pCard_Filter->vTransferAll(pCard_Expr);
  delete pCard_Expr;

  delete pCard_Filter_nested;
  delete pCard_Filter_others;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
