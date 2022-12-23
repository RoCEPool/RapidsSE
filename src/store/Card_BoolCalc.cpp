/*============================================================================*/
/* Card_BoolCalc.cpp -- Boolean Calculation                                   */
/*============================================================================*/

#include "Card_BoolCalc.hpp"

#include "Card_PRED.hpp"
#include "Card_BolPred.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Term.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_BoolCalc, CK_BOOLCALC)
__aNewFunction(Card_BoolCalc, CN_CARD)
__vDeleteFunction(Card_BoolCalc, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_BoolCalc::Card_BoolCalc(void)
: Card(CK_BOOLCALC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
      
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_BoolCalc::~Card_BoolCalc(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_BoolCalc::bIsBraced(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool bBraced = false;
  auto bTestBraced=[&bBraced](Card* p_card) {

    if (p_card->bNeedsBrace()) {
      bBraced = true;
      return C_TERMINATE_RECURSION;
    }

    return C_CONTINUE_RECURSION;
  };

  bApplyRecursive(CK_NONE,bTestBraced);

  return bBraced;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
