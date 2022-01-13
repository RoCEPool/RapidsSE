/*============================================================================*/
/* Card_CorrSubquery.cpp -- Correlated Subquery                               */
/*============================================================================*/

#include "Card_CorrSubquery.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card_CorrSubquery,CN_CARD_CORRSUBQUERY)
__aNewFunction(Card_CorrSubquery,CN_CARD_CORRSUBQUERY)
__vDeleteFunction(Card_CorrSubquery,CN_CARD_CORRSUBQUERY)
__aFooterFunction(Card_CorrSubquery)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CorrSubquery::Card_CorrSubquery(eCK eKind, Stz* szQueryNm)
: Card(eKind,CN_CARD_CORRSUBQUERY,"CorrSubquery"_Stz,"QueryNm"_Stz,szQueryNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_CORRSUBQUERY,_FOOTER_);
      
  m_fpDeleteThis = &Card_CorrSubquery::vDeleteThis_;
  
  _checkInitialised_(CN_CARD_CORRSUBQUERY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD_CORRSUBQUERY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CorrSubquery::~Card_CorrSubquery(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_CORRSUBQUERY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get query name                                                             */
/*----------------------------------------------------------------------------*/
Stz* Card_CorrSubquery::szQueryNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_CORRSUBQUERY,_FOOTER_);

  Stz* szQueryNm = szGetProperty("QueryNm"_Stz);
  
  return szQueryNm;
}

/*----------------------------------------------------------------------------*/
/* set query name                                                             */
/*----------------------------------------------------------------------------*/
void Card_CorrSubquery::vQueryNm(Stz* szQueryNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_CORRSUBQUERY,_FOOTER_);

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
void Card_CorrSubquery::vDeleteThis_(Card* pCard)
{
  _static_

  delete (Card_CorrSubquery*)pCard;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
