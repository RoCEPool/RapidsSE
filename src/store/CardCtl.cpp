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

  m_bCued = false;
  m_bCueDisabled = false;
  m_bTrace = false;

  vReset();

  vSave();
  vSaveDefault();
  vSaveExpanded();
  vSaveLonger();
  vSaveShorter();
  vSaveXform();
  vSaveXhand();
//                                     <--
//  MemLib::vCopy(m_xaCardCtl_default, m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
//  MemLib::vCopy(m_xaCardCtl_expanded,m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
//  MemLib::vCopy(m_xaCardCtl_longer,  m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
//  MemLib::vCopy(m_xaCardCtl_shorter, m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
//  MemLib::vCopy(m_xaCardCtl_shorter, m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
//  MemLib::vCopy(m_xaCardCtl_xform,   m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
//  MemLib::vCopy(m_xaCardCtl_xhand,   m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
//
//  MemLib::vCopy(m_xaCardCtl_save,    m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);

  m_iPrintNo = 0;

  m_pCardHistory = nullptr;
  if (AM::JOB::bHasTag(C_CARD_HISTORY))
    m_pCardHistory = new CardHistory;

  _checkInitialised_(CN_CARDCTL);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CardCtl::~CardCtl(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  if (m_pCardHistory)
    delete m_pCardHistory;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print bench ?                                                              */
/*----------------------------------------------------------------------------*/
bool CardCtl::bBenchPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  if (AM::JOB::bHasTag(C_GLEAN_INTERACTIVE))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* print board ?                                                              */
/*----------------------------------------------------------------------------*/
bool CardCtl::bBoardPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  if (AM::JOB::bHasTag(C_GLEAN_INTERACTIVE))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get card history                                                           */
/*----------------------------------------------------------------------------*/
CardHistory* CardCtl::pCardHistory(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  return m_pCardHistory;
}

/*----------------------------------------------------------------------------*/
/* get cued                                                                   */
/*----------------------------------------------------------------------------*/
bool CardCtl::bCued(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  return m_bCued;
}

/*----------------------------------------------------------------------------*/
/* set cued                                                                   */
/*----------------------------------------------------------------------------*/
void CardCtl::vCued(bool bCued)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  m_bCued = bCued;
}

/*----------------------------------------------------------------------------*/
/* get cued                                                                   */
/*----------------------------------------------------------------------------*/
bool CardCtl::bCueDisabled(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  return m_bCueDisabled;
}

/*----------------------------------------------------------------------------*/
/* set cued                                                                   */
/*----------------------------------------------------------------------------*/
void CardCtl::vCueDisabled(bool bCueDisabled)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  m_bCueDisabled = bCueDisabled;
}

/*----------------------------------------------------------------------------*/
/* set cued                                                                   */
/*----------------------------------------------------------------------------*/
bool CardCtl::bCueUser(Stz* szXhand, Stz* szXform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  if (m_bCueDisabled)
    return false;

  if (m_bCued)
    return true;

  if (! AM::JOB::bHasTag(C_GLEAN_CUE_FROM))
    return false;

  Stz* szTagCue = AM::JOB::szTagValue(C_GLEAN_CUE_FROM);
  if (   (szTagCue->bSame(szXhand) && szXform->bSame(C_NA))
      || (szXform && szTagCue->bSame(szXform))) {
    m_bCued = true;
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* set cued                                                                   */
/*----------------------------------------------------------------------------*/
void CardCtl::vFormPrompt(String* sPrompt,
                          char cPrefix, Stz* szXhand, Stz* szXform,
                          i8 i_Board, i8 nBoards, i8 iLineNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  sPrompt->vSprint(""_Stz);

//  if (! ('m' == cPrefix || 'w' == cPrefix))
//    if (C_UNKNOWN != i_Board)
//      sPrompt->vCatSprint("%lld/%lld "_Stz, i_Board, nBoards);
  if ('r' == cPrefix || 's' == cPrefix)
    if (C_UNKNOWN != i_Board)
      sPrompt->vCatSprint("%lld/%lld "_Stz, i_Board, nBoards);

  if (szXhand && szXform && ! szXform->bSame(C_NA)) {
    sPrompt->vCatSprint("%s :: %s"_Stz, szXhand, szXform);
  }
  else
  if (szXhand)
    sPrompt->vCatSprint("%s"_Stz, szXhand);

  if (C_UNKNOWN != iLineNo)
    sPrompt->vCatSprint(" [%lld]"_Stz, iLineNo);

}

/*----------------------------------------------------------------------------*/
/* get bHeatNext for card                                                    */
/*----------------------------------------------------------------------------*/
bool CardCtl::bHeatNext(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  bool bNextHoriz = m_xaCardCtl[i_Card].m_bHeatNext;

  return bNextHoriz;
}

/*----------------------------------------------------------------------------*/
/* set bHeatNext for card                                                    */
/*----------------------------------------------------------------------------*/
void CardCtl::vHeatNext(i8 i_Card, bool bNextHoriz)
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
/* get bPrintBelow for card                                                   */
/*----------------------------------------------------------------------------*/
bool CardCtl::bPrintBelow(i8 i_Card)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  bool bPrintBelow = m_xaCardCtl[i_Card].m_bPrintBelow;

  return bPrintBelow;
}

/*----------------------------------------------------------------------------*/
/* set bPrintBelow for card                                                   */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintBelow(i8 i_Card, bool bPrintBelow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  if (i_Card) {
    m_xaCardCtl[i_Card].m_bPrintBelow = bPrintBelow;
    return;
  }

  for (i_Card = 1; i_Card < CK_ENUMS; i_Card++)
    m_xaCardCtl[i_Card].m_bPrintBelow = bPrintBelow;
}

/*----------------------------------------------------------------------------*/
/* set bPrintBelow for card                                                   */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintBelow(i8 iBelow, va_list xVarArgs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  if (CK_NONE == iBelow)
    return;

  m_xaCardCtl[iBelow].m_bPrintBelow = true;

  for (;;) {
    iBelow = (i8)va_arg(xVarArgs,eCK);
    if (CK_NONE == iBelow)
      return;
    m_xaCardCtl[iBelow].m_bPrintBelow = true;
  }

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
/* get print                                                                  */
/*----------------------------------------------------------------------------*/
i8 CardCtl::iPrintNo(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  return m_iPrintNo;
}

/*----------------------------------------------------------------------------*/
/* increment print                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vPrintNoInc(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  m_iPrintNo++;
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
    // Alka: On linux the following statement causes an illegal instruction
    // cannot pass an enum type to va_arg, figured it passes the enum as an int
    //eCK e_Card = va_arg(args, eCK);

    eCK e_Card = (eCK)va_arg(args, int);

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
    m_xaCardCtl[i_Card].m_sHideHeat = C_NA;

    m_xaCardCtl[i_Card].m_bPrintBelow      = false;
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

  //vPrintProperties(CK_CMPPRED, false);


//  vPrintByKind(CK_KEY,true);
//  vPrintSubCards(CK_KEY, false);

  vPrintName(CK_NOTE,false);
  vPrintProperties(CK_NOTE,false);

  vPrintValue(CK_ORPRED, false);
  vPrintProperties(CK_ORPRED, false);

//  vPrintByKind(CK_PROJECTION,true);
//  vPrintSubCards(CK_PROJECTION, false);

  vHeatNext(CK_SECTION, true);

  vHeatNext(CK_STAGE, true);
  vHorizExclude(CK_STAGE, P_HideHeat);

  vPrintByKind(CK_TABLE,true);
  vPrintValue(CK_TABLE, false);
  //vPrintProperties(CK_TABLE, false);

  vPrintByKind(CK_TERM,true);
  vPrintName(CK_TERM,false);
  vPrintNo(CK_TERM,false);
  //vPrintProperties(CK_TERM,false);

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
void CardCtl::vSaveDefault(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                                <--
  MemLib::vCopy(m_xaCardCtl_default,m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vSaveExpanded(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                                 <--
  MemLib::vCopy(m_xaCardCtl_expanded,m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vSaveLonger(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                               <--
  MemLib::vCopy(m_xaCardCtl_longer,m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vSaveShorter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                                <--
  MemLib::vCopy(m_xaCardCtl_shorter,m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vSaveXform(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                              <--
  MemLib::vCopy(m_xaCardCtl_xform,m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vSaveXhand(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                              <--
  MemLib::vCopy(m_xaCardCtl_xhand,m_xaCardCtl,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
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
void CardCtl::vSetSubsPrintEverything(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {
    m_xaCardCtl[i_Card].m_bPrintName       = true;
    m_xaCardCtl[i_Card].m_bPrintNo         = true;
    m_xaCardCtl[i_Card].m_bPrintValue      = true;
    m_xaCardCtl[i_Card].m_bPrintProperties = true;
    m_xaCardCtl[i_Card].m_bPrintPptName    = true;
    m_xaCardCtl[i_Card].m_bPrintSubCards   = true;
    m_xaCardCtl[i_Card].m_bPrintSubShort   = false;
  }
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

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vShowSettings(i8 i_Card)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  vTrace("Card Settings %lld %s\n"_Stz, i_Card, aCK[i_Card]);

  vTrace("  m_bPrintName       %s\n"_Stz,m_xaCardCtl[i_Card].m_bPrintName       ? "true" : "false");
  vTrace("  m_bPrintNo         %s\n"_Stz,m_xaCardCtl[i_Card].m_bPrintNo         ? "true" : "false");
  vTrace("  m_bPrintValue      %s\n"_Stz,m_xaCardCtl[i_Card].m_bPrintValue      ? "true" : "false");
  vTrace("  m_bPrintProperties %s\n"_Stz,m_xaCardCtl[i_Card].m_bPrintProperties ? "true" : "false");
  vTrace("  m_bPrintPptName    %s\n"_Stz,m_xaCardCtl[i_Card].m_bPrintPptName    ? "true" : "false");
  vTrace("  m_bPrintSubCards   %s\n"_Stz,m_xaCardCtl[i_Card].m_bPrintSubCards   ? "true" : "false");
  vTrace("  m_bPrintSubShort   %s\n"_Stz,m_xaCardCtl[i_Card].m_bPrintSubShort   ? "true" : "false");
}

/*----------------------------------------------------------------------------*/
/* get cued                                                                   */
/*----------------------------------------------------------------------------*/
bool CardCtl::bTrace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  return m_bTrace;
}

/*----------------------------------------------------------------------------*/
/* set cued                                                                   */
/*----------------------------------------------------------------------------*/
void CardCtl::vTraceing(bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  m_bTrace = bTrace;
}

/*----------------------------------------------------------------------------*/
/* get cued                                                                   */
/*----------------------------------------------------------------------------*/
bool CardCtl::bTracing(Stz* szXhand, Stz* szXform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARDCTL,_FOOTER_);

  if (m_bTrace)
    return true;

  if (! AM::JOB::bHasTag(C_GLEAN_TRACE))
    return false;

  Stz* szTagTrace = AM::JOB::szTagValue(C_GLEAN_TRACE);
  if (   (szTagTrace->bSame(szXhand) && szXform->bSame(C_NA))
         || (szXform && szTagTrace->bSame(szXform))) {
    return true;
  }

  return false;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vUseDefault(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                        <--
  MemLib::vCopy(m_xaCardCtl,m_xaCardCtl_default,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vUseExpanded(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                        <--
  MemLib::vCopy(m_xaCardCtl,m_xaCardCtl_expanded,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vUseLonger(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                        <--
  MemLib::vCopy(m_xaCardCtl,m_xaCardCtl_longer,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vUseShorter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                        <--
  MemLib::vCopy(m_xaCardCtl,m_xaCardCtl_shorter,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vUseXform(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                        <--
  MemLib::vCopy(m_xaCardCtl,m_xaCardCtl_xform,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardCtl::vUseXhand(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARDCTL, _FOOTER_);

  //                        <--
  MemLib::vCopy(m_xaCardCtl,m_xaCardCtl_xhand,sizeof(m_xaCardCtl),BASE::CN_CARDCTL);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
