/*============================================================================*/
/* CardCtl.cpp -- Card Control                                                */
/*============================================================================*/

#include "CardCtl.hpp"
#include "Card.hpp"

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
  
  vReset();

  //                                 <--
  MemLib::vCopy(m_xaCardCtl_default, m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
  MemLib::vCopy(m_xaCardCtl_expanded,m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
  MemLib::vCopy(m_xaCardCtl_longer,  m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
  MemLib::vCopy(m_xaCardCtl_shorter, m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);

  MemLib::vCopy(m_xaCardCtl_save,    m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vFormDefault(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                        <--
  MemLib::vCopy(m_xaCardCtl,m_xaCardCtl_default,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vFormExpanded(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                        <--
  MemLib::vCopy(m_xaCardCtl,m_xaCardCtl_expanded,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vFormLonger(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                        <--
  MemLib::vCopy(m_xaCardCtl,m_xaCardCtl_longer,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vFormShorter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                        <--
  MemLib::vCopy(m_xaCardCtl,m_xaCardCtl_shorter,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/* get bNextHoriz for card                                                    */
/*----------------------------------------------------------------------------*/
bool CardCtl::bNextHoriz(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  bool bNextHoriz = m_xaCardCtl[i_Card].m_bHeatNext;

  return bNextHoriz;
}

/*----------------------------------------------------------------------------*/
/* set bNextHoriz for card                                                    */
/*----------------------------------------------------------------------------*/
void CardCtl::vNextHorizon(i8 i_Card, bool bNextHoriz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  if (i_Card) {
    m_xaCardCtl[i_Card].m_bHeatNext = bNextHoriz;
    return;
  }

  for (i_Card = 1; i_Card < CK_ENUMS; i_Card++)
    m_xaCardCtl[i_Card].m_bHeatNext = bNextHoriz;
}

/*----------------------------------------------------------------------------*/
/* get bPrintByKind for card                                                  */
/*----------------------------------------------------------------------------*/
bool CardCtl::bPrintByKind(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  bool bPrintByKind = m_xaCardCtl[i_Card].m_bPrintByKind;

  return bPrintByKind;
}

/*----------------------------------------------------------------------------*/
/* set bPrintByKind for card                                                  */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintByKind(i8 i_Card, bool bPrintByKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  if (i_Card) {
    m_xaCardCtl[i_Card].m_bPrintByKind = bPrintByKind;
    return;
  }

  for (i_Card = 1; i_Card < CK_ENUMS; i_Card++)
    m_xaCardCtl[i_Card].m_bPrintByKind = bPrintByKind;
}

/*----------------------------------------------------------------------------*/
/* get szHorizExclude for card                                                */
/*----------------------------------------------------------------------------*/
Stz* CardCtl::szHorizExclude(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  Stz* szHorizExclude = m_xaCardCtl[i_Card].m_sHideHeat;

  return szHorizExclude;
}

/*----------------------------------------------------------------------------*/
/* set szHorizExclude for card                                                */
/*----------------------------------------------------------------------------*/
void CardCtl::vHorizExclude(i8 i_Card, Stz* szHorizExclude)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  if (i_Card) {
    m_xaCardCtl[i_Card].m_sHideHeat = szHorizExclude;
    return;
  }

  for (i_Card = 1; i_Card < CK_ENUMS; i_Card++)
    m_xaCardCtl[i_Card].m_sHideHeat = szHorizExclude;
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

  if (i_Card) {
    m_xaCardCtl[i_Card].m_bPrintName = bPrintName;
    return;
  }

  for (i_Card = 1; i_Card < CK_ENUMS; i_Card++)
    m_xaCardCtl[i_Card].m_bPrintName = bPrintName;
}

/*----------------------------------------------------------------------------*/
/* get bPrintName for card                                                    */
/*----------------------------------------------------------------------------*/
bool CardCtl::bPrintNo(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  bool bPrintNo = m_xaCardCtl[i_Card].m_bPrintNo;

  return bPrintNo;
}

/*----------------------------------------------------------------------------*/
/* set bPrintName for card                                                    */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintNo(i8 i_Card, bool bPrintNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  if (i_Card) {
    m_xaCardCtl[i_Card].m_bPrintNo = bPrintNo;
    return;
  }

  for (i_Card = 1; i_Card < CK_ENUMS; i_Card++)
    m_xaCardCtl[i_Card].m_bPrintNo = bPrintNo;
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

  if (i_Card) {
    m_xaCardCtl[i_Card].m_bPrintValue = bPrintValue;
    return;
  }

  for (i_Card = 1; i_Card < CK_ENUMS; i_Card++)
    m_xaCardCtl[i_Card].m_bPrintValue = bPrintValue;
}

/*----------------------------------------------------------------------------*/
/* get bPrintPptName for card                                                 */
/*----------------------------------------------------------------------------*/
bool CardCtl::bPrintPptName(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  bool bPrintPptName = m_xaCardCtl[i_Card].m_bPrintPptName;

  return bPrintPptName;
}

/*----------------------------------------------------------------------------*/
/* set bPrintPptName for card                                                 */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintPptName(i8 i_Card, bool bPrintPptName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  if (i_Card) {
    m_xaCardCtl[i_Card].m_bPrintPptName = bPrintPptName;
    return;
  }

  for (i_Card = 1; i_Card < CK_ENUMS; i_Card++)
    m_xaCardCtl[i_Card].m_bPrintPptName = bPrintPptName;
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
/* set bPrintProperties for card                                              */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintProperties(i8 i_Card, bool bPrintProperties)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  if (i_Card) {
    m_xaCardCtl[i_Card].m_bPrintProperties = bPrintProperties;
    return;
  }

  for (i_Card = 1; i_Card < CK_ENUMS; i_Card++)
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
/* set bPrintSubCards for card                                                */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintSubCards(i8 i_Card, bool bPrintSubCards)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  if (i_Card) {
    m_xaCardCtl[i_Card].m_bPrintSubCards = bPrintSubCards;
    return;
  }

  for (i_Card = 1; i_Card < CK_ENUMS; i_Card++)
    m_xaCardCtl[i_Card].m_bPrintSubCards = bPrintSubCards;
}

/*----------------------------------------------------------------------------*/
/* set bPrintSubCards for card                                                */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintSubCards(bool bPrintSubCards, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  va_list args;
  va_start(args,bPrintSubCards);

  for (;;) {
    eCK e_Card = va_arg(args, eCK);
    if (e_Card == CK_NONE || e_Card == CK_ENUMS)
      break;
    m_xaCardCtl[e_Card].m_bPrintSubCards = bPrintSubCards;
  }

  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* get bPrintSubShort for card                                                */
/*----------------------------------------------------------------------------*/
bool CardCtl::bPrintSubShort(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  bool bPrintSubShort = m_xaCardCtl[i_Card].m_bPrintSubShort;

  return bPrintSubShort;
}

/*----------------------------------------------------------------------------*/
/* set bPrintSubShort for card                                                */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintSubShort(i8 i_Card, bool bPrintSubShort)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  if (i_Card) {
    m_xaCardCtl[i_Card].m_bPrintSubShort = bPrintSubShort;
    return;
  }

  for (i_Card = 1; i_Card < CK_ENUMS; i_Card++)
    m_xaCardCtl[i_Card].m_bPrintSubShort = bPrintSubShort;
}

/*----------------------------------------------------------------------------*/
/* get bPrintSubShort for card                                                */
/*----------------------------------------------------------------------------*/
bool CardCtl::bPrune(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  bool bPrune = m_xaCardCtl[i_Card].m_bPrune;

  return bPrune;
}

/*----------------------------------------------------------------------------*/
/* set bPrintSubShort for card                                                */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrune(i8 i_Card, bool bPrune)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  if (i_Card) {
    m_xaCardCtl[i_Card].m_bPrune = bPrune;
    return;
  }

  for (i_Card = 1; i_Card < CK_ENUMS; i_Card++)
    m_xaCardCtl[i_Card].m_bPrune = bPrune;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {

    m_xaCardCtl[i_Card].m_bHeatNext = false;
    m_xaCardCtl[i_Card].m_sHideHeat = "n/a"_Stz;

    m_xaCardCtl[i_Card].m_bPrintByKind     = false;
    m_xaCardCtl[i_Card].m_bPrintName       = true;
    m_xaCardCtl[i_Card].m_bPrintNo         = true;
    m_xaCardCtl[i_Card].m_bPrintValue      = true;
    m_xaCardCtl[i_Card].m_bPrintProperties = true;
    m_xaCardCtl[i_Card].m_bPrintPptName    = true;
    m_xaCardCtl[i_Card].m_bPrintSubCards   = true;
    m_xaCardCtl[i_Card].m_bPrintSubShort   = false;

    m_xaCardCtl[i_Card].m_bPrune     = false;
  }

  vPrintValue(CK_ANDPRED, false);
  vPrintProperties(CK_ANDPRED, false);

  vPrintProperties(CK_CMPPRED, false);

  vPrintName(CK_COMMENT,false);
  vPrintProperties(CK_COMMENT,false);

  vPrintSubShort(CK_COPTNJOIN,true);
  vPrintSubCards(CK_COPTNJOIN,false);
  vPrune(CK_COPTNJOIN,true);

  vPrintSubShort(CK_CROSSPTNJOIN,true);
  vPrintSubCards(CK_CROSSPTNJOIN,false);
  vPrune(CK_CROSSPTNJOIN,true);

  vPrintValue(CK_ORPRED, false);
  vPrintProperties(CK_ORPRED, false);

  vNextHorizon(CK_STAGE, true);
  vHorizExclude(CK_STAGE, P_HideHeat);

  vPrintByKind(CK_TABLE,true);
  vPrintValue(CK_TABLE, false);
  vPrintProperties(CK_TABLE, false);

  vPrintName(CK_TERM,false);
  vPrintNo(CK_TERM,false);
  vPrintProperties(CK_TERM,false);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vRestore(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                        <--
  MemLib::vCopy(m_xaCardCtl,m_xaCardCtl_save,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vSave(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                             <--
  MemLib::vCopy(m_xaCardCtl_save,m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vSetDefaultForm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                                <--
  MemLib::vCopy(m_xaCardCtl_default,m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vSetExpandedForm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                                 <--
  MemLib::vCopy(m_xaCardCtl_expanded,m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vSetLongerForm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                               <--
  MemLib::vCopy(m_xaCardCtl_longer,m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vSetShorterForm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                                <--
  MemLib::vCopy(m_xaCardCtl_shorter,m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vSetSubsPrintAll(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++)
    m_xaCardCtl[i_Card].m_bPrintSubCards = true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vSetSubsPrintNone(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++)
    m_xaCardCtl[i_Card].m_bPrintSubCards = false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
