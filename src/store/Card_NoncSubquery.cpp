/*============================================================================*/
/* Card_NoncSubquery.cpp -- Non-Correlated Subquery                           */
/*============================================================================*/

#include "Card_NoncSubquery.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_NoncSubquery,CN_CARD_NONCSUBQUERY)
__aNewFunction(Card_NoncSubquery,CN_CARD_NONCSUBQUERY)
__vDeleteFunction(Card_NoncSubquery,CN_CARD_NONCSUBQUERY)
__aFooterFunction(Card_NoncSubquery)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_NoncSubquery::Card_NoncSubquery(eCK eKind, Stz* szQueryNm)
: Card(CK_NONCSUBQUERY,CN_CARD_NONCSUBQUERY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_NONCSUBQUERY,_FOOTER_);
      
  m_fpDeleteThis = &Card_NoncSubquery::vDeleteThis_;
  
  vAddProperty("QueryNm"_Stz,szQueryNm);
  
  _checkInitialised_(CN_CARD_NONCSUBQUERY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_NONCSUBQUERY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_NoncSubquery::~Card_NoncSubquery(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_NONCSUBQUERY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get query name                                                             */
/*----------------------------------------------------------------------------*/
Stz* Card_NoncSubquery::szQueryNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_NONCSUBQUERY,_FOOTER_);

  Stz* szQueryNm = szGetProperty("QueryNm"_Stz);
  
  return szQueryNm;
}

/*----------------------------------------------------------------------------*/
/* set query name                                                             */
/*----------------------------------------------------------------------------*/
void Card_NoncSubquery::vQueryNm(Stz* szQueryNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_NONCSUBQUERY,_FOOTER_);

  vAddProperty("QueryNm"_Stz,szQueryNm);
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
void Card_NoncSubquery::vDeleteThis_(Card* pCard)
{
  _static_

  delete (Card_NoncSubquery*)pCard;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
