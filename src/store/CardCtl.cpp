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
    m_xaCardCtl[i_Card].m_bNextHoriz = false;
    m_xaCardCtl[i_Card].m_ePrintForm = CP_DEFAULT;

    m_xaCardCtl[i_Card].m_bPrintName       = true;
    m_xaCardCtl[i_Card].m_bPrintValue      = true;
    m_xaCardCtl[i_Card].m_bPrintProperties = true;
    m_xaCardCtl[i_Card].m_bPrintSubCards   = true;
  }
  
  _checkInitialised_(CN_CARDCTL);

  vNextHorizon(CK_STAGE, true);

  vPrintProperties(CK_TERM,false);

  vPrintName(CK_COMMENT,false);
  vPrintProperties(CK_COMMENT,false);

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
/* get bNextHoriz for card                                                    */
/*----------------------------------------------------------------------------*/
bool CardCtl::bNextHoriz(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  bool bNextHoriz = m_xaCardCtl[i_Card].m_bNextHoriz;

  return bNextHoriz;
}

/*----------------------------------------------------------------------------*/
/* set bNextHoriz for card                                                    */
/*----------------------------------------------------------------------------*/
void CardCtl::vNextHorizon(i8 i_Card, bool bNextHoriz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  m_xaCardCtl[i_Card].m_bNextHoriz = bNextHoriz;
}

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
/* get bPrintName for card                                                    */
/*----------------------------------------------------------------------------*/
bool CardCtl::bPrintName(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  bool bPrintName = m_xaCardCtl[i_Card].m_bPrintName;

  return bPrintName;
}

/*----------------------------------------------------------------------------*/
/* set bPrintName for card                                                    */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintName(i8 i_Card, bool bPrintName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  m_xaCardCtl[i_Card].m_bPrintName = bPrintName;
}

/*----------------------------------------------------------------------------*/
/* get bPrintValue for card                                                   */
/*----------------------------------------------------------------------------*/
bool CardCtl::bPrintValue(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  bool bPrintValue = m_xaCardCtl[i_Card].m_bPrintValue;

  return bPrintValue;
}

/*----------------------------------------------------------------------------*/
/* set bPrintValue for card                                                   */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintValue(i8 i_Card, bool bPrintValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  m_xaCardCtl[i_Card].m_bPrintValue = bPrintValue;
}

/*----------------------------------------------------------------------------*/
/* get bPrintValue for card                                                   */
/*----------------------------------------------------------------------------*/
bool CardCtl::bPrintProperties(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  bool bPrintProperties = m_xaCardCtl[i_Card].m_bPrintProperties;

  return bPrintProperties;
}

/*----------------------------------------------------------------------------*/
/* set bPrintProperties for card                                                   */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintProperties(i8 i_Card, bool bPrintProperties)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  m_xaCardCtl[i_Card].m_bPrintProperties = bPrintProperties;
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
