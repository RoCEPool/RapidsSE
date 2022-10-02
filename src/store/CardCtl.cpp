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
    vPrintForm(i_Card, 0);
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
/* get print form for card                                                    */
/*----------------------------------------------------------------------------*/
i8 CardCtl::iPrintForm(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  i8 iPrintForm = m_iaPrintForm[i_Card];

  return iPrintForm;
}

/*----------------------------------------------------------------------------*/
/* set print form for card                                                    */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintForm(i8 i_Card, i8 iPrintForm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  m_iaPrintForm[i_Card] = iPrintForm;
}

/*----------------------------------------------------------------------------*/
/* get bPrintSubCards for card                                                */
/*----------------------------------------------------------------------------*/
bool CardCtl::bPrintSubCards(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  bool bPrintSubCards = m_baPrintSubCards[i_Card];

  return bPrintSubCards;
}

/*----------------------------------------------------------------------------*/
/* set bPrintSubCards for card                                                 */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintSubCards(i8 i_Card, bool bPrintSubCards)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  m_baPrintSubCards[i_Card] = bPrintSubCards;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
