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
    m_xaCardCtl[i_Card].m_ePrintForm     = CP_DEFAULT;
    m_xaCardCtl[i_Card].m_bPrintSubCards = true;
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
eCP CardCtl::ePrintForm(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  eCP ePrintForm = m_xaCardCtl[i_Card].m_ePrintForm;

  return ePrintForm;
}

/*----------------------------------------------------------------------------*/
/* set print form for card                                                    */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintForm(i8 i_Card, eCP ePrintForm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  m_xaCardCtl[i_Card].m_ePrintForm = ePrintForm;
}

/*----------------------------------------------------------------------------*/
/* set print form and print subcards for card                                 */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintFormSubCards(i8 i_Card, eCP ePrintForm, bool bPrintSubCards)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  vPrintForm(i_Card, ePrintForm);
  vPrintSubCards(i_Card, bPrintSubCards);
}

/*----------------------------------------------------------------------------*/
/* get bPrintSubCards for card                                                */
/*----------------------------------------------------------------------------*/
bool CardCtl::bPrintSubCards(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  bool bPrintSubCards = m_xaCardCtl[i_Card].m_bPrintSubCards;

  return bPrintSubCards;
}

/*----------------------------------------------------------------------------*/
/* set bPrintSubCards for card                                                 */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintSubCards(i8 i_Card, bool bPrintSubCards)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  m_xaCardCtl[i_Card].m_bPrintSubCards = bPrintSubCards;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
