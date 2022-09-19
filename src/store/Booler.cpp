/*============================================================================*/
/* Booler.cpp -- transform <calc> into <calcBool>                             */
/*============================================================================*/

#include "Booler.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Booler,CN_BOOLER)
__aNewFunction(Booler,CN_BOOLER)
__vDeleteFunction(Booler,CN_BOOLER)
__aFooterFunction(Booler)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Booler::Booler(Bench* pBench)
: TransformerTool(CN_BOOLER,pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOOLER,_FOOTER_);

  _checkInitialised_(CN_BOOLER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BOOLER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Booler::~Booler(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOOLER,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* stream                                                                     */
/*----------------------------------------------------------------------------*/
void Booler::vBool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOOLER,_FOOTER_);

  i8 nMainCards = m_pBoard->nMainCards();
  for (i8 i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
    Card* pMainCard =  m_pBoard->pGetMainCard(i_MainCard);

    if (CK_FILTER != pMainCard->eCard())
      continue;

    vBool(pMainCard);
  }

}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* stream                                                                     */
/*----------------------------------------------------------------------------*/
void Booler::vBool(Card* pCard_Filter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOOLER,_FOOTER_);
  RASSERT(CK_FILTER == pCard_Filter->eCard());

  RASSERT(1 == pCard_Filter->nSubCards());

  Card_Expr* pCard_Expr = _pCard_Expr(pCard_Filter->pGet(0));
  RASSERT(2 == pCard_Expr->nSubCards());

  Card_Type* pCard_Type = _pCard_Type(pCard_Expr->pGet(0));
  Card_Calc* pCard_Calc = _pCard_Calc(pCard_Expr->pGet(1));

  Card* pCard_CalcBool = pTo_CalcBool(pCard_Calc);

  pCard_Expr->vRemove(pCard_Calc);
  delete pCard_Calc;
  pCard_Expr->vAppend(pCard_CalcBool);
}

/*----------------------------------------------------------------------------*/
/* convert a calculation into a boolean calculaiton                           */
/*----------------------------------------------------------------------------*/
Card* Booler::pTo_CalcBool(Card* pCard_Calc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOOLER, _FOOTER_);
  RASSERT(CK_CALC == pCard_Calc->eCard());

  Card_PredAnd* pCard_PredAnd = nullptr;
  Card_PredCmp* pCard_PredCmp = nullptr;
  Card_PredOr*  pCard_PredOr  = nullptr;

  Card_SubSet*  pCard_SubExpr = new Card_SubSet;

  Card_CalcBool* pCard_CalcBool = new Card_CalcBool;

  while (pCard_Calc->nSubCards()) {
    Card_Term *pCard_Term = _pCard_Term(pCard_Calc->pPop());

    eTE eTopTerm = pCard_Term->eTermEval();
    switch (eTopTerm) {

      case TE_AND:
        RASSERT(2 == pCard_CalcBool->nSubCards());
        pCard_PredAnd = new Card_PredAnd;
        pCard_PredAnd->vTransferAll(pCard_CalcBool);
        RASSERT(0 == pCard_CalcBool->nSubCards());
        pCard_CalcBool->vAppend(pCard_PredAnd);
        RASSERT(1 == pCard_CalcBool->nSubCards());
        pCard_PredAnd = nullptr;
        delete pCard_Term;
        break;

      case TE_OR:
        RASSERT(2 == pCard_CalcBool->nSubCards());
        pCard_PredOr = new Card_PredOr;
        pCard_PredOr->vTransferAll(pCard_CalcBool);
        RASSERT(0 == pCard_CalcBool->nSubCards());
        pCard_CalcBool->vAppend(pCard_PredOr);
        RASSERT(1 == pCard_CalcBool->nSubCards());
        pCard_PredOr = nullptr;
        delete pCard_Term;
        break;

      case TE_COMPARE:
        RASSERT(pCard_SubExpr->nSubCards() > 0);
        pCard_PredCmp = new Card_PredCmp;
        pCard_PredCmp->vCmp(pCard_Term->szTerm());
        pCard_PredCmp->vTransferAll(pCard_SubExpr);
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
    RASSERT(0 == pCard_CalcBool->nSubCards());
    pCard_PredCmp->vTransferAll(pCard_SubExpr);
    RASSERT(1 == pCard_CalcBool->nSubCards());
    RASSERT(0 == pCard_SubExpr->nSubCards());
  }

  delete pCard_SubExpr;

  return pCard_CalcBool;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
