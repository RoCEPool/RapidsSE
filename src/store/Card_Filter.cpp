/*============================================================================*/
/* Card_Filter.cpp -- Filter                                                  */
/*============================================================================*/

#include "Card_Filter.hpp"

#include "Card_Expr.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_BoolCalc.hpp"
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
void Card_Filter::vAddPred(Card_PRED *p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_BoolCalc* p_calcBool = this->pSub_boolCalc();

  if (0 == p_calcBool->nChildCards()) {
    p_calcBool->vAppend(p_PRED);
    return;
  }

  Card* p_top = p_calcBool->pPop();

  Card_AndPred* pCard_predAnd = new Card_AndPred;
  pCard_predAnd->vAppend(p_top);
  pCard_predAnd->vAppend(p_PRED);

  p_calcBool->vAppend(pCard_predAnd);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vCullPred(char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = this->pGetChild(i_Child);

    eCK eCard = pChildCard->eCard();
    if (eCard != CK_BOOLCALC)
      continue;

    Card_BoolCalc* p_boolCalc = _pCard_BoolCalc(pChildCard);

    p_boolCalc->vCullPred(cWithMark);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Filter::bIsEmpty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_BoolCalc* pCard_calcBool =
      _pCard_BoolCalc(this->pFindSubCard(CK_BOOLCALC));

  if (0 == pCard_calcBool->nChildCards())
    return true;

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
