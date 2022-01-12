/*============================================================================*/
/* Card_Project.cpp -- List of Project Cards                              */
/*============================================================================*/

#include "Card_Project.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_Project,CN_CARD_PROJECT)
__aNewFunction(Card_Project,CN_CARD_PROJECT)
__vDeleteFunction(Card_Project,CN_CARD_PROJECT)
__aFooterFunction(Card_Project)


__bIsClass(STORE,Card_Project,CN_CARD_PROJECT)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Project::Card_Project(eCK eKind)
: Card_Unit(eKind,CN_CARD_PROJECT,"Project"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECT,_FOOTER_);
    
  m_fpDeleteThis = &Card_Project::vDeleteThis_;

  _checkInitialised_(CN_CARD_PROJECT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Project::Card_Project(eCK eKind, eCN eClassNo, Stz* szCardNm)
: Card_Unit(eKind,eClassNo,szCardNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECT,_FOOTER_);
    
  m_fpDeleteThis = &Card_Project::vDeleteThis_;

  _checkInitialised_(CN_CARD_PROJECT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Project::Card_Project(eCK eKind, eCN eClassNo, Stz* szCardNm,
                           Stz* szProperty, i8 iValue)
: Card_Unit(eKind,eClassNo,szCardNm,szProperty,iValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECT,_FOOTER_);
    
  m_fpDeleteThis = &Card_Project::vDeleteThis_;

  _checkInitialised_(CN_CARD_PROJECT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Project::Card_Project(eCK eKind, eCN eClassNo, Stz* szCardNm,
                           Stz* szProperty, Object* pValue)
: Card_Unit(eKind,eClassNo,szCardNm,szProperty,pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECT,_FOOTER_);
    
  m_fpDeleteThis = &Card_Project::vDeleteThis_;

  _checkInitialised_(CN_CARD_PROJECT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Project::Card_Project(eCK eKind, eCN eClassNo, Stz* szCardNm,
                           Stz* szProperty, Stz* szValue)
: Card_Unit(eKind,eClassNo,szCardNm,szProperty,szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECT,_FOOTER_);
    
  m_fpDeleteThis = &Card_Project::vDeleteThis_;

  _checkInitialised_(CN_CARD_PROJECT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Project::~Card_Project(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_PROJECT,_FOOTER_);

  vDeleteAll();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Project::vDeleteThis_(Card* pCard)
{
  _static_
  
  delete (Card_Project*)pCard;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
