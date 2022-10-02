/*============================================================================*/
/* CardCtl.cpp -- Card Control                                                */
/*============================================================================*/

 #include "CardCtl.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,CardCtl,CN_CARDCTL)
__aNewFunction(CardCtl,CN_CARDCTL)
__vDeleteFunction(CardCtl,CN_CARDCTL)
__aFooterFunction(CardCtl)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardCtl::CardCtl(void)
: Object(CN_CARDCTL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);
  
  for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {
    vPinForm(i_Card,0);
    vPrintSubCards(i_Card, true);
  }
  
  _checkInitialised_(CN_CARDCTL);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CardCtl::~CardCtl(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get iPinForm for card                                                      */
/*----------------------------------------------------------------------------*/
i8 CardCtl::iPinForm(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  i8 iPinForm = m_iaPinForm[i_Card];

  return iPinForm;
}

/*----------------------------------------------------------------------------*/
/* set iPinForm for card                                                      */
/*----------------------------------------------------------------------------*/
void CardCtl::vPinForm(i8 i_Card, i8 iPinForm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  m_iaPinForm[i_Card] = iPinForm;
}

/*----------------------------------------------------------------------------*/
/* get bPrintCards for card                                                     */
/*----------------------------------------------------------------------------*/
bool CardCtl::bPrintSubCards(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  bool bPrintCards = m_baPrintSubCards[i_Card];

  return bPrintCards;
}

/*----------------------------------------------------------------------------*/
/* set bPrintCards for card                                                     */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintSubCards(i8 i_Card, bool bSubCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  m_baPrintSubCards[i_Card] = bSubCard;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
