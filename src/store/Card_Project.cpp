/*============================================================================*/
/* Card_Project.cpp -- List of Project Cards                              */
/*============================================================================*/

#include "Card_Project.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Project,CK_PROJECT)
__aNewFunction(Card_Project,CN_CARD_PROJECT)
__vDeleteFunction(Card_Project,CN_CARD_PROJECT)

__bIsClass(STORE,Card_Project,CN_CARD_PROJECT)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Project::Card_Project(eCK eKind)
: Card_Unit(eKind,CN_CARD_PROJECT)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
    
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Project::Card_Project(eCK eKind, eCN eClassNo)
: Card_Unit(eKind,eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
    
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Project::~Card_Project(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  vDeleteAll();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
