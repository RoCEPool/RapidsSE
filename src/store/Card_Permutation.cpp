/*============================================================================*/
/* Card_Permutation.cpp -- Permutation                                        */
/*============================================================================*/

#include "Card_Permutation.hpp"

#include "Card_Deck.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Permutation,CK_PERMUTATION)
__aNewFunction(Card_Permutation,CN_CARD)
__vDeleteFunction(Card_Permutation,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Permutation::Card_Permutation(void)
: Card(CK_PERMUTATION)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Permutation::~Card_Permutation(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Permutation::vRemoveInvalid(Card_Deck* pDeck)
{
  _static_

  i8 i_Main = 0;
  i8 i_Permutation = 0;
  while (i_Main < pDeck->nChildCards()) {
    Card* p_MainCard = pDeck->pGetChild(i_Main);

    if (CK_PERMUTATION != p_MainCard->eCard()) {
      i_Main++;
      continue;
    }

    if (! p_MainCard->Y_bHas(P_Invalid)) {
      p_MainCard->Y_vSet(P_Permutation, i_Permutation++);
      i_Main++;
      continue;
    }


    pDeck->vRemoveChild(p_MainCard);
    while (i_Main < pDeck->nChildCards()) {
      p_MainCard = pDeck->pGetChild(i_Main);
      if (CK_PTNDJOIN != p_MainCard->eCard() && CK_NOTE != p_MainCard->eCard())
        break;
      pDeck->vRemoveChild(p_MainCard);
      delete p_MainCard;
    }
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
