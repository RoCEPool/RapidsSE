/*============================================================================*/
/* Card_KeyNotExists.cpp -- Key Not Exists                                    */
/*============================================================================*/

#include "Card_KeyNotExists.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_KeyNotExists,CK_KEYNOTEXISTS)
__aNewFunction(Card_KeyNotExists,CN_CARD)
__vDeleteFunction(Card_KeyNotExists,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_KeyNotExists::Card_KeyNotExists(void)
: Card_OP(CK_KEYNOTEXISTS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_KeyNotExists::~Card_KeyNotExists(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* table morphs to keyNotExists                                               */
/*----------------------------------------------------------------------------*/
void Card_KeyNotExists::vTableMorphsToKeyNotExists(STORE::CardIter *pCI)
{
  _static_

  pCI->G.p_index->vSet_Choose_index();

  pCI->G.p_table->vRemoveSubCard(pCI->G.p_keyNotExists);

  pCI->G.p_keyNotExists->vTransferChildren(pCI->G.p_table);

  pCI->G.pBoard->pDeck()->vReplace(pCI->G.p_table, pCI->G.p_keyNotExists);
  pCI->G.p_keyNotExists->vInsert(pCI->G.p_table);

//  pCI->G.p_keyNotExists->vRemoveSubCard(pCI->G.p_index);
//  pCI->G.p_keyNotExists->vAppend(pCI->G.p_index);

  pCI->G.p_MainCard = pCI->G.p_keyNotExists;


//  pCI->G.p_filter = pCI->G.p_keyNotExists->pSub_filter();
//  pCI->G.p_constraint = pCI->G.p_filter->pSub_constraint();
//  pCI->G.p_filter->vRemoveChild(pCI->G.p_constraint);
//
//  pCI->G.p_keyNotExists->vRemoveDeleteChild(pCI->G.p_filter);
//  pCI->G.p_keyNotExists->vAppend(pCI->G.p_constraint);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* apply stream                                                               */
/*----------------------------------------------------------------------------*/
void Card_KeyNotExists::vComputeProjection(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
