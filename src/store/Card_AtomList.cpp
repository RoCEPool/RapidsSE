/*============================================================================*/
/* Card_AtomList.cpp -- List of Atom Cards                                    */
/*============================================================================*/

#include "Card_AtomList.hpp"
#include "Card_Atom.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_AtomList,CK_ATOMLIST)
__aNewFunction(Card_AtomList,CN_CARD_ATOMLIST)
__vDeleteFunction(Card_AtomList,CN_CARD_ATOMLIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_AtomList::Card_AtomList(eCK eKind)
: Card(CK_ATOMLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_ATOMLIST,_FOOTER_);
    
  _checkInitialised_(CN_CARD_ATOMLIST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_ATOMLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_AtomList::~Card_AtomList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_ATOMLIST,_FOOTER_);

  vDeleteAll();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
