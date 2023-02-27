/*============================================================================*/
/* Card_BolPred.cpp -- Boolean Predicate                                      */
/*============================================================================*/

#include "Card_BolPred.hpp"

#include "Card_Term.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_BolPred, CK_BOLPRED)
__aNewFunction(Card_BolPred, CN_CARD)
__vDeleteFunction(Card_BolPred, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_BolPred::Card_BolPred(void)
: Card_PRED(CK_BOLPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_BolPred::~Card_BolPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BolPred::vMorph_To_cmpPred(CardIter* pCI, Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bIsValue("exists"_Stz)) {

    vCard(CK_CMPPRED);
    vValue("=="_Stz);

    Card_Term* p_term_one = _pCard_Term(pGetChild(1));
    ASSERT(p_term_one->bIsValue("__subquery0002"_Stz)); //temp
    p_term_one->vValue(p_term ? p_term->szValue() : "$1"_Stz);
    return;
  }

  if (bIsValue("in"_Stz)) {

    vCard(CK_CMPPRED);
    vValue("=="_Stz);
    Card_Term* p_term_popped = _pCard_Term(pPopChild());
    ASSERT(StrLib::bBeginsWith(p_term_popped->szValue()->azChar(),"__subquery")); //temp
    DELETE_CARD(p_term_popped);

    Card_Term* p_term_head = _pCard_Term(pGetChild(0));
    ASSERT(p_term_head->bIsValue("#1"_Stz)); //temp
    p_term_head->vValue(p_term?p_term->szValue():"$1"_Stz);
    return;
  }

  if (bIsValue("notIn"_Stz)) {

    vCard(CK_CMPPRED);
    vValue("=="_Stz);
    Card_Term* p_term_popped = _pCard_Term(pPopChild());
    ASSERT(p_term_popped->bIsValue("__subquery0002"_Stz)); //temp
    DELETE_CARD(p_term_popped);

    Card_Term* p_term_head = _pCard_Term(pGetChild(0));
    ASSERT(p_term_head->bIsValue("#1"_Stz)); //temp
    p_term_head->vValue(p_term?p_term->szValue():"$1"_Stz);
    return;
  }

  NEVER_REACH
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
