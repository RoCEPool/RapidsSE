/*============================================================================*/
/* Card_Deck.cpp -- Board                                                    */
/*============================================================================*/

#include "Card_Deck.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Deck, CK_DECK)
__aNewFunction(Card_Deck, CN_CARD)
__vDeleteFunction(Card_Deck, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Deck::Card_Deck(void)
: Card(CK_DECK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

    
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Deck::~Card_Deck(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* find i_fold (reverse)                                                      */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Deck::p_expr_fold(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Projection* p_projection_fold = this-> p_projection_fold())
    return p_projection_fold->pSub_expr();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find i_fold (reverse)                                                      */
/*----------------------------------------------------------------------------*/
i8 Card_Deck::i_Fold(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nChildren = nChildCards();
  for (i8 i_Main = nChildren - 1; i_Main >= 0; i_Main--) {
    Card *pMain = pGetChild(i_Main);
    if (pMain->bIsCard(CK_FOLD))
      return i_Main;
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* find p_fold (reverse)                                                      */
/*----------------------------------------------------------------------------*/
Card_Fold* Card_Deck::p_fold(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nChildren = nChildCards();
  for (i8 i_Main = nChildren - 1; i_Main >= 0; i_Main--) {
    Card *pMain = pGetChild(i_Main);
    if (pMain->bIsCard(CK_FOLD))
      return _pCard_Fold(pMain);
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find project before fold (reverse)                                         */
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Deck::p_projection_fold(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i_Fold = this->i_Fold();
  for (i8 i_Main = i_Fold - 1; i_Main >= 0; i_Main--) {
    Card *pMain = pGetChild(i_Main);
    if (Card_Projection* p_projection = pMain->pSub_projection())
      return p_projection;
  }

  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
