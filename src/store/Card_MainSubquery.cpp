/*============================================================================*/
/* Card_MainSubquery.cpp -- Main Subquery                                     */
/*============================================================================*/

#include "Card_MainSubquery.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_MainSubquery,CN_CARD_HASHCAST)
__aNewFunction(Card_MainSubquery,CN_CARD_HASHCAST)
__vDeleteFunction(Card_MainSubquery,CN_CARD_HASHCAST)
__aFooterFunction(Card_MainSubquery)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_MainSubquery::Card_MainSubquery(eCK eKind, Stz* szQueryNm)
: Card(CK_MAINSUBQUERY,CN_CARD_HASHCAST, "MainSubquery"_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_HASHCAST,_FOOTER_);
      
  m_fpDeleteThis = &Card_MainSubquery::vDeleteThis_;
  
  vAddProperty("szQueryNm"_Stz,szQueryNm);
  
  _checkInitialised_(CN_CARD_HASHCAST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_HASHCAST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_MainSubquery::~Card_MainSubquery(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_HASHCAST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get query name                                                             */
/*----------------------------------------------------------------------------*/
Stz* Card_MainSubquery::szQueryNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_HASHCAST,_FOOTER_);

  Stz* szQueryNm = szGetProperty("szQueryNm"_Stz);

  return szQueryNm;
}

/*----------------------------------------------------------------------------*/
/* set query name                                                             */
/*----------------------------------------------------------------------------*/
void Card_MainSubquery::vQueryNm(Stz* szQueryNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_HASHCAST,_FOOTER_);

  vAddProperty("szQueryNm"_Stz,szQueryNm);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Card_MainSubquery::vDeleteThis_(Card* pCard)
{
  _static_

  delete (Card_MainSubquery*)pCard;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
