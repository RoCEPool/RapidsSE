/*============================================================================*/
/* Card.cpp -- Abstract Card                                                  */
/*============================================================================*/

#include "Card.hpp"
#include "CardCtl.hpp"
#include "Board.hpp"
#include "CardIter.hpp"

#include "Card_Index.hpp"  // TODO: remove
#include "Index.hpp"       // TODO: remove

//Card* gapCard[C_10K];

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card,CN_CARD)
__aNewFunction(Card,CN_CARD)
__vDeleteFunction(Card,CN_CARD)
__aFooterFunction(Card)

i8 Card::m__iSeqGen { 1 };

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card::Card(eCK eCard)
: Object(CN_CARD)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vInit(eCard);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card::Card(eCK eCard, Stz* szValue)
    : Object(CN_CARD)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vInit(eCard);

  vValue(szValue);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card::Card(eCK eCard, Stz* szValue, Stz* szPpyNm, i8 iPptVal)
    : Object(CN_CARD)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vInit(eCard);

  vValue(szValue);

  Y_vSet(szPpyNm, iPptVal);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card::Card(eCK eCard, Stz* szValue, Stz* szPpyNm, Object* pPptVal)
    : Object(CN_CARD)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vInit(eCard);

  vValue(szValue);

  Y_vSet(szPpyNm, pPptVal);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card::Card(eCK eCard, Stz* szValue, Stz* szPpyNm, Stz* szPptVal)
    : Object(CN_CARD)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vInit(eCard);

  vValue(szValue);

  Y_vSet(szPpyNm, szPptVal);
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void Card::vInit(eCK eCard)
{

  MemLib::vZero(md_Bearing, sizeof(md_Bearing), CN_CARD);

  if (CK_BOUNDS == eCard)
    vDebugPoint();

  m_eCard   = eCard;
  m_iCardNo = C_UNKNOWN;

  m_cMark1 = ' ';
  m_cMark2 = ' ';

  m_pProperties = nullptr;

  m_pLastPrintParent = nullptr;

  md__SeqNo = Card::m__iSeqGen++;
  //gapCard[md__SeqNo] = this;
  md__FirstDetected  = C_UNKNOWN;
  md__LeakDetected   = C_UNKNOWN;

  s32 sBearing;
  sBearing.vSprint("((%s new %04lld))"_Stz,aCK[m_eCard],md__SeqNo);
  StrLib::vCopy(md_Bearing,sBearing.azChar());
//  if (11 == md__SeqNo) {
//    vTrace("Card [%02lld] %p\n"_Stz, md__SeqNo, this);
//    vDebugPoint();
//  }
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card::~Card(void) noexcept(false)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  static Card* pPriorCard = nullptr;

//  vTrace("Card [~%02lld] %p %s %s : %s\n"_Stz,
//         md__SeqNo,this,aCK[m_eCard],m_sValue.szStz(),md_Bearing);
//
//  if (this == pPriorCard)
//    vDebugPoint();
//
//  if (678 == md__SeqNo) {
//    pPriorCard = this;
//    vDebugPoint();
//  }

  //gapCard[md__SeqNo] = nullptr;

  for (Card* pCard = pPop(); pCard; pCard = pPop())
    delete pCard;
  
  delete m_pProperties;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* access table                                                               */
/*----------------------------------------------------------------------------*/
Table* Card::pAccess_Table(Stz* szTableNm)
{
  _static_

  Table* pTable = AM::pTableFind(szTableNm);

  return pTable;
}

/*----------------------------------------------------------------------------*/
/* get bound for relational op                                                */
/*----------------------------------------------------------------------------*/
Stz* Card::szBound(Stz* szRelnOp)
{
  _static_

  if (szRelnOp->bSame(">"_Stz) || szRelnOp->bSame(">="_Stz))
    return "LBOUND"_Stz;

  if (szRelnOp->bSame("<"_Stz) || szRelnOp->bSame("<="_Stz))
    return "UBOUND"_Stz;

  return "BOUND"_Stz;
}

/*----------------------------------------------------------------------------*/
/* check for leaks                                                            */
/*----------------------------------------------------------------------------*/
void Card::vCardLeaks(void)
{
  _static_

//  for (i8 i_Card = 0; i_Card < C_10K; i_Card++) {
//    if (gapCard[i_Card]) {
//      Card *pLeaked = gapCard[i_Card];
//      vTrace("Card Leak ~%02lld %p %s:%s\n"_Stz,
//             i_Card, pLeaked, aCK[pLeaked->eCard()], pLeaked->sz());
//      vDebugPoint();
//    }
//  }
}

/*----------------------------------------------------------------------------*/
/* check for leaks                                                            */
/*----------------------------------------------------------------------------*/
void Card::vConsistencyCheck(Card* p_deck, i8 iTransformNo)
{
  _static_

//  for (i8 i_Card = 0; i_Card < m__iSeqGen; i_Card++) {
//    if (! gapCard[i_Card])
//      continue;
//
//    Card* p_cardInQuestion = gapCard[i_Card];
//
//    // is this the first time a transformation has seen this card?
//    if (C_UNKNOWN == p_cardInQuestion->md__FirstDetected)
//      p_cardInQuestion->md__FirstDetected = iTransformNo;
//
//    // is the card the entire deck?
//    if (p_cardInQuestion == p_deck)
//      continue;
//
//    // is the card in the deck?
//    if (p_deck->pFindParent(p_cardInQuestion))
//      continue;
//
//    // **Leak Detected**
//
//    // is this the first time this card has been detected?
//    if (C_UNKNOWN != p_cardInQuestion->md__LeakDetected)
//      continue;
//
//    // report the leak
//    p_cardInQuestion->md__LeakDetected = iTransformNo;
//    vTrace("Card leak detected : %04lld %p %s:%s\n"_Stz,
//         i_Card,
//         p_cardInQuestion,
//         aCK[p_cardInQuestion->eCard()],
//         p_cardInQuestion->sz());
//    vDebugPoint();
//  }
//
}

/*----------------------------------------------------------------------------*/
/* convert eLevel to P/N/O                                                    */
/*----------------------------------------------------------------------------*/
char Card::cAggrLevelToChar(eAL eLevel)
{
  _static_

  switch (eLevel) {
    case AL_PARTITION : return 'P';
    case AL_NODE      : return 'N';
    case AL_ORIGIN    : return 'O';
    default           : return '?';
  }
}

/*----------------------------------------------------------------------------*/
/* convert eAL to eYG                                                         */
/*----------------------------------------------------------------------------*/
eYG Card::eAggrLevelToStageTopology(eAL eLevel)
{
  _static_

  switch (eLevel) {
    case AL_PARTITION : return YG_ALL_PARTITIONS;
    case AL_NODE      : return YG_ALL_NODES;
    case AL_ORIGIN    : return YG_ORIGIN_NODE;
    default           : return YG_ALL_PARTITIONS;
  }
}

/*----------------------------------------------------------------------------*/
/* get column ordinal n  (i.e. $n)                                            */
/*----------------------------------------------------------------------------*/
i8 Card::iGetColOdn(Stz* szValue)
{
  _static_

  i8 i_Col = StrLib::iGleanInt(szValue->azChar());

  return i_Col;
}

/*----------------------------------------------------------------------------*/
/* is term an index-column ordinal?  (i.e. $In)                               */
/*----------------------------------------------------------------------------*/
bool Card::bIsIndexColOrdinal(Stz* szValue)
{
  _static_

  char acFirst  = szValue->azChar()[0];
  char acSecond = szValue->azChar()[1];
  char acThird  = szValue->azChar()[2];

  if ('$' == acFirst
      && ('I' == acSecond || 'i' == acSecond)
      && StrLib::bIsDigit(acThird))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is value a key ordinal?  (i.e. $Kn)                                        */
/*----------------------------------------------------------------------------*/
bool Card::bIsKeyOrdinal(Stz* szValue)
{
  _static_

  char acFirst  = szValue->azChar()[0];
  char acSecond = szValue->azChar()[1];
  char acThird  = szValue->azChar()[2];

  if ('$' == acFirst
      && ('K' == acSecond || 'k' == acSecond)
      && StrLib::bIsDigit(acThird))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is value a parameter ordinal?  (i.e. $Pn)                                  */
/*----------------------------------------------------------------------------*/
bool Card::bIsParamOrdinal(Stz* szValue)
{
  _static_

  char acFirst  = szValue->azChar()[0];
  char acSecond = szValue->azChar()[1];

  if ('%' == acFirst
      && StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is value a result ordinal?  (i.e. #n)                                      */
/*----------------------------------------------------------------------------*/
bool Card::bIsResultOrdinal(Stz* szValue)
{
  _static_

  char acFirst  = szValue->azChar()[0];
  char acSecond = szValue->azChar()[1];

  if ('#' == acFirst && StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is value a subQuery name?                                                  */
/*----------------------------------------------------------------------------*/
bool Card::bIsSubQueryName(BASE::Stz *szValue)
{
  _static_

  char acFirst  = szValue->azChar()[0];
  char acSecond = szValue->azChar()[1];
  char acThird  = szValue->azChar()[2];
  char acFourth = szValue->azChar()[3];

  if ('_' == acFirst
      && '_' == acSecond
      && 's' == acThird
      && 'u' == acFourth)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is value a subQuery-column ordinal?  (i.e. __n)                            */
/*----------------------------------------------------------------------------*/
bool Card::bIsSqyColOrdinal(Stz* szValue)
{
  _static_

  char acFirst  = szValue->azChar()[0];
  char acSecond = szValue->azChar()[1];
  char acThird  = szValue->azChar()[2];
  char acFourth = szValue->azChar()[3];

  if ('_' == acFirst
      && '_' == acSecond
      && StrLib::bIsDigit(acThird))
    return true;

  if ('_' == acFirst
      && '_' == acSecond
      && 'S' == acThird
      && 'Q' == acFourth)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is value a stream-column ordinal?  (i.e. $n)                               */
/*----------------------------------------------------------------------------*/
bool Card::bIsStreamColOrdinal(Stz* szValue)
{
  _static_

  char acFirst  = szValue->azChar()[0];
  char acSecond = szValue->azChar()[1];

  if ('$' == acFirst && StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is value a table-column ordinal?  (i.e. $c.n)                              */
/*----------------------------------------------------------------------------*/
bool Card::bIsTableColOrdinal(Stz* szValue)
{
  _static_

  char acFirst  = szValue->azChar()[0];
  char acSecond = szValue->azChar()[1];
  char acThird  = szValue->azChar()[2];
  char acFourth = szValue->azChar()[3];

  if ('$' == acFirst
      && StrLib::bIsAlpha(acSecond)
      && '.' == acThird
      && StrLib::bIsDigit(acFourth))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get table column syn from $c.n                                             */
/*----------------------------------------------------------------------------*/
char Card::cTableColSyn(Stz* szValue)
{
  _static_

  char acFirst  = szValue->azChar()[0];
  char acSecond = szValue->azChar()[1];
  char acThird  = szValue->azChar()[2];
  char acFourth = szValue->azChar()[3];

  RASSERT('$' == acFirst);
  RASSERT('.' == acThird);

  return acSecond;
}

/*----------------------------------------------------------------------------*/
/* find eCK by card name                                                      */
/*----------------------------------------------------------------------------*/
eCK Card::eTo_eCK(Stz* szCardNm)
{
  _static_
  
  s32 sName;
  sName.vSprint("CK_%s"_Stz,szCardNm);
  
  for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++)
    if (aCK[i_Card]->bSame(sName))
      return (eCK)i_Card;
  
  return CK_NONE;
}

/*----------------------------------------------------------------------------*/
/* return typeShort                                                           */
/*----------------------------------------------------------------------------*/
char Card::cTypeShort(Stz* szType)
{
  _static_

  for (i8 i_CT = 0; i_CT < CT_ENUMS; i_CT++) {
    if (szType->bSame(aCT[i_CT]))
      return cCT_char[i_CT];
  }

  return cCT_char[CT_ENUMS];
}

/*----------------------------------------------------------------------------*/
/* convert eYG to eAL                                                         */
/*----------------------------------------------------------------------------*/
eAL Card::eStageTopologyToAggrLevel(eYG eStageTopo)
{
  _static_

  eAL eLevel;

  switch (eStageTopo) {
    case YG_ALL_PARTITIONS : eLevel = AL_PARTITION; break;
    case YG_ALL_NODES      : eLevel = AL_NODE;      break;
    case YG_ORIGIN_NODE    : eLevel = AL_ORIGIN;    break;
    default                : eLevel = AL_NONE;      break;
  }

  return eLevel;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vAddPredUnder_main(Card_PRED *p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Filter* p_filter = this->pSub_filter();
  if (! p_filter) {
    p_filter = _pCard_Filter(this->pAppendNewCardTail(CK_FILTER));
    p_filter->vAppendNewCardTail(CK_BOOLCALC);
  }

  p_filter->vAddPred(p_PRED);
}

/*----------------------------------------------------------------------------*/
/* duplicate the sub cards                                                    */
/*----------------------------------------------------------------------------*/
void Card::vAppendDupSubCards(Card* pFromCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nSubCards = pFromCard->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pSubCard = pFromCard->pGetChild(i_Child);

    Card* pSubCard_dup = pSubCard->pDupCardAndSubCards();

    if (pSubCard->m_pProperties) {
      pSubCard_dup->vPropertiesEnsure();
      pSubCard_dup->pProperties()->vCopy(pSubCard->m_pProperties);
    }

    this->vAppend(pSubCard_dup);
  }
}

/*----------------------------------------------------------------------------*/
/* append new main card at end                                                */
/*----------------------------------------------------------------------------*/
void Card::vAppendMain(Card* pNewMainCard, bool bRenumber)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAppend(pNewMainCard);

  if (bRenumber)
    vRenumberRecursive();
}

/*----------------------------------------------------------------------------*/
/* append new card to tail                                                    */
/*----------------------------------------------------------------------------*/
void Card::vAppendNewCardTail(eCK eCard, Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card* pCard = pAppendNewCardTail(eCard,szValue);
}

/*----------------------------------------------------------------------------*/
/* append new card to tail                                                    */
/*----------------------------------------------------------------------------*/
Card* Card::pAppendNewCardTail(eCK eCard, Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  Card* pCard;
  if ( szValue != nullptr )
    pCard = new Card(eCard, szValue);
  else
    pCard = new Card(eCard);

  vPushTail(pCard);

  return pCard;
}

/*----------------------------------------------------------------------------*/
/* append new card to top                                                     */
/*----------------------------------------------------------------------------*/
void Card::vAppendNewCardTop(eCK eCard, Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  Card* pCard;
  if(szValue != nullptr)
    pCard = new Card(eCard, szValue);
  else
    pCard = new Card(eCard);

  vPush(pCard);
}

/*----------------------------------------------------------------------------*/
/* visit all sub-cards of given eToSubs and apply lambda function               */
/*----------------------------------------------------------------------------*/
bool Card::bApplyRecursive(eCK eToSubs, std::function<bool(Card* pCard)> bfLambdaFn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (CK_NONE == eToSubs || eToSubs == m_eCard) {
    bool bContinue = bfLambdaFn(this);
    if (! bContinue)
      return C_TERMINATE_RECURSION;
  }

  CardIter xCI(this);
  while (Card* pChildCard = xCI.pEachChild()) {
    bool bContinue = pChildCard->bApplyRecursive(eToSubs, bfLambdaFn);
    if (! bContinue)
      return C_TERMINATE_RECURSION;
  }

  return C_CONTINUE_RECURSION;
}

/*----------------------------------------------------------------------------*/
/* visit all term sub-cards and apply lambda function                         */
/*----------------------------------------------------------------------------*/
void Card::vApplyRecursive_term(std::function<bool(Card_Term* p_term)> bfLambdaFn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (bIsCard(CK_TERM)) {
    bool bContinue = bfLambdaFn(_pCard_Term(this));
    if (! bContinue)
      return;
  }

  CardIter xCI(this);
  while (Card* pChildCard = xCI.pEachChild()) {
    pChildCard->vApplyRecursive_term(bfLambdaFn);
  }
}

/*----------------------------------------------------------------------------*/
/* get bearing                                                                */
/*----------------------------------------------------------------------------*/
Stz* Card::szBearing(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return (Stz*)md_Bearing;
}

/*----------------------------------------------------------------------------*/
/* set bearing                                                                */
/*----------------------------------------------------------------------------*/
void Card::vBearing(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  s4K sPretty;

  sPretty = aCK[m_eCard];

  if (m_pProperties) {
    s4K sProperties;
    m_pProperties->vToString(sProperties,true);
    sPretty.vConcat(": "_Stz);
    sPretty.vConcat(sProperties);
  }

  strcat(md_Bearing, " ");
  strcat(md_Bearing, sPretty.azChar());

  //  vTrace("%s\n"_Stz,md_Bearing);
}

/*----------------------------------------------------------------------------*/
/* set bearing                                                                */
/*----------------------------------------------------------------------------*/
void Card::vBearing(Card* pParentCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pParentCard) {
    sprintf(md_Bearing, "%s.%lld", pParentCard->md_Bearing, m_iCardNo);
  }
  else {
    sprintf(md_Bearing, "%lld", m_iCardNo);
  }

//  vTrace("%s\n"_Stz,md_Bearing);
}

/*----------------------------------------------------------------------------*/
/* get card kind                                                              */
/*----------------------------------------------------------------------------*/
eCK Card::eCard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_eCard;
}

/*----------------------------------------------------------------------------*/
/* set card kind                                                              */
/*----------------------------------------------------------------------------*/
void Card::vCard(eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_eCard = eCard;
}

/*----------------------------------------------------------------------------*/
/* clear all marks                                                            */
/*----------------------------------------------------------------------------*/
void Card::vClearMarksRecursive(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vMark1(' ');
  vMark2(' ');

  i8 nSubCards = this->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card *pSubCard = pGetChild(i_Sub);

    pSubCard->vClearMarksRecursive();
  }

}

/*----------------------------------------------------------------------------*/
/* get card no                                                                */
/*----------------------------------------------------------------------------*/
i8 Card::iCardNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_iCardNo;
}

/*----------------------------------------------------------------------------*/
/* set card no                                                                */
/*----------------------------------------------------------------------------*/
void Card::vCardNo(i8 iCardNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_iCardNo = iCardNo;
}

/*----------------------------------------------------------------------------*/
/* get card name                                                              */
/*----------------------------------------------------------------------------*/
Stz* Card::szCardNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return aCK_short[m_eCard];
}

/*----------------------------------------------------------------------------*/
/* copy properties from given card into this card                             */
/*----------------------------------------------------------------------------*/
void Card::vCopyProperties(Card* pFrom)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vPropertiesEnsure();
  m_pProperties->vCopy(pFrom->m_pProperties);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vDeleteFilterIfEmpty(Card_Filter* p_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! p_filter->bIsEmpty())
    return;

  this->vRemoveChild(p_filter);
  DELETE_CARD(p_filter);

}

/*----------------------------------------------------------------------------*/
/* dump card and its sub cards                                                */
/*----------------------------------------------------------------------------*/
void Card::vDump(i8 iDepth, Stz* szTitle)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (0 == iDepth)
    vDebugPoint();

  if (szTitle)
    vTrace("%s %p:\n"_Stz,szTitle,this);

  s48K sProperties;
  
  if (m_pProperties)
    m_pProperties->vToString(sProperties);
  else
    sProperties = "no properties"_Stz;
  
//  vTrace("%s%lld %s %s { %s }\n"_Stz
//         ,Stz::szTab(iDepth)
//         ,m_iCardNo
//         ,szCardNm()
//         ,aCK[m_eCard]
//         ,sProperties.szStz()
//         );

  if (*szValue()->azChar()) {
    vTrace("%s%s %s { %s }"_Stz
        , Stz::szTab(iDepth)
        , md_Bearing[0] ? md_Bearing : (char*)aCK[m_eCard]
        , szValue()
        , sProperties.szStz()
    );
  }
  else {
    vTrace("%s%s { %s }"_Stz
        , Stz::szTab(iDepth)
        , md_Bearing[0] ? md_Bearing : (char*)aCK[m_eCard]
        , sProperties.szStz()
    );
  }

  vTrace(" %p"_Stz,this);

  vTrace("\n"_Stz);

  i8 nSubCards = this->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++)
    pGetChild(i_Sub)->vDump(iDepth + 1);
}

/*----------------------------------------------------------------------------*/
/* duplicate this card, its value and properties                              */
/*----------------------------------------------------------------------------*/
Card* Card::pDup(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card* pCard_dup = new Card(m_eCard);
  pCard_dup->vValue(szValue());
  if (m_pProperties)
    pCard_dup->vCopyProperties(this);

  return pCard_dup;
}

/*----------------------------------------------------------------------------*/
/* duplicate this card and its sub cards                                      */
/*----------------------------------------------------------------------------*/
Card* Card::pDupCardAndSubCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card* pCard_dup = new Card(m_eCard);
  pCard_dup->vValue(szValue());
  if (m_pProperties)
    pCard_dup->vCopyProperties(this);

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pSubCard = pGetChild(i_Child);
    
    Card* pSubCard_dup = pSubCard->pDupCardAndSubCards();
    pCard_dup->vAppend(pSubCard_dup);
  }
  
  return pCard_dup;
}

/*----------------------------------------------------------------------------*/
/* duplicate the sub cards                                                    */
/*----------------------------------------------------------------------------*/
Card_SubSet* Card::pDupSubCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_SubSet* pCard_SubSet = new Card_SubSet;
  
  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pSubCard = pGetChild(i_Child);
    
    Card* pSubCard_dup = pSubCard->pDupCardAndSubCards();
    pCard_SubSet->vAppend(pSubCard_dup);
  }
  
  return pCard_SubSet;
}

/*----------------------------------------------------------------------------*/
/* duplicate the sub cards                                                    */
/*----------------------------------------------------------------------------*/
Card_SubSet* Card::pDupSubCards(i8 i_From, i8 i_Till)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);


  Card_SubSet* pCard_SubSet = new Card_SubSet;

  i8 nChildren = this->nChildCards();
  RASSERT(i_From >= 0 && i_From < nChildren);

  i8 i_Break = (C_UNKNOWN == i_Till) ? nChildren : i_Till + 1;
  if (i_Break > nChildren)
    i_Break = nChildren;
  RASSERT(i_Break >= 0 && i_Break <= nChildren);

  for (i8 i_Child = i_From; i_Child < i_Break; i_Child++) {
    Card* pChild = pGetChild(i_Child);

    Card* pChild_dup = pChild->pDupCardAndSubCards();
    pCard_SubSet->vAppend(pChild_dup);
  }

  return pCard_SubSet;
}

/*----------------------------------------------------------------------------*/
/* is this card equivalent to some other card?                                */
/*----------------------------------------------------------------------------*/
bool Card::bEquivalentTo(Card* pCard_OTHER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card* pCard_THIS = this;

  if (pCard_OTHER->eCard() != pCard_THIS->eCard())
    return false;

  TagSet* pProperties_THISCARD  = m_pProperties;
  TagSet* pProperties_OTHERCARD = pCard_OTHER->pProperties();

  if (pProperties_THISCARD) {
    if (pProperties_OTHERCARD) {
      if (! pProperties_THISCARD->bSame(pProperties_OTHERCARD))
        return false;
    }
    else {
      return false;
    }
  }
  else
  if (pProperties_OTHERCARD) {
    return false;
  }
  else {
    RASSERT(! m_pProperties);
    RASSERT(! pProperties_OTHERCARD);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  eCK eChild_THISCARD;
  i8 nChildren_THISCARD = pCard_THIS->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren_THISCARD; i_Child++) {
    Card* pChild_THISCARD = pCard_THIS->pGetChild(i_Child, eChild_THISCARD);
    
    Card* pChild_OTHERCARD = pCard_OTHER->pFindSubCard(eChild_THISCARD);
    if (! pChild_OTHERCARD)
      return false;

    if (!pChild_THISCARD->bEquivalentTo(pChild_OTHERCARD))
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* find parent of given sub-card                                              */
/*----------------------------------------------------------------------------*/
Card* Card::pFindParent(Card* pSubCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nSubCards = nItems();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card* pTestCard = pGetChild(i_Sub);

    if (pSubCard == pTestCard)
      return this;

    Card* pParent = pTestCard->pFindParent(pSubCard);
    if (pParent)
      return pParent;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* attempt find card                                                          */
/*----------------------------------------------------------------------------*/
Card* Card::pFindCardRecursive(eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bIsCard(eCard))
    return this;

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    if (Card *pFoundCard = pGetChild(i_Child)->pFindCardRecursive(eCard))
      return pFoundCard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* attempt find of value in this pile                                         */
/*----------------------------------------------------------------------------*/
Card* Card::pFindValueRecursive(Stz* szValue, Card*& rpParent, Card* _pWalkParent_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bSame(szValue)) {
    rpParent = _pWalkParent_;
    return this;
  }

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    if (Card *pFoundCard = pGetChild(i_Child)->pFindValueRecursive(szValue, rpParent, this))
      return pFoundCard;
  }

  rpParent = nullptr;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* attempt find of value in this pile                                         */
/*----------------------------------------------------------------------------*/
Card* Card::pFindValueRecursive(eCK eCard, char cWithValue, Card*& rpParent, Card* _pWalkParent_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //vTrace("%s `%s`?`%s` %s\n"_Stz,__func__,szValue,sz(),md_Bearing);

  if (eCard == m_eCard && cValue() == cWithValue) {
    rpParent = _pWalkParent_;
    return this;
  }

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    if (Card *pFoundCard = pGetChild(i_Child)->pFindValueRecursive(eCard,cWithValue, rpParent, this))
      return pFoundCard;
  }

  rpParent = nullptr;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* attempt find of value in this pile                                         */
/*----------------------------------------------------------------------------*/
Card* Card::pFindValueRecursive(eCK eCard, Stz* szValue, Card*& rpParent, Card* _pWalkParent_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //vTrace("%s `%s`?`%s` %s\n"_Stz,__func__,szValue,sz(),md_Bearing);

  if (eCard == m_eCard && bSame(szValue)) {
    rpParent = _pWalkParent_;
    return this;
  }

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    if (Card *pFoundCard = pGetChild(i_Child)->pFindValueRecursive(eCard,szValue, rpParent, this))
      return pFoundCard;
  }

  rpParent = nullptr;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find property recursively within this card                                 */
/*----------------------------------------------------------------------------*/
Card* Card::pFindPpyRecursive(Stz* szPpy, eCK eOfCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ( (CK_NONE == eOfCard || eOfCard == m_eCard)) {
    if (Y_bHas(szPpy))
      return this;
  }


  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    if (Card *pFoundCard = pGetChild(i_Child)->pFindPpyRecursive(szPpy,eOfCard))
      return pFoundCard;
  }

  return nullptr;
}



/*----------------------------------------------------------------------------*/
/* is there an immediate child with given value?                              */
/*----------------------------------------------------------------------------*/
bool Card::bHasChildWithValue(Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (i8 i_Child = 0; i_Child < nChildCards(); i_Child++) {
    Card* pChildCard = pGetChild(i_Child);

    if (pChildCard->bIsValue(szValue))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* is given card a sub card of this card?                                     */
/*----------------------------------------------------------------------------*/
bool Card::bHasSubRecursive(Card* pSubCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pTestCard = pGetChild(i_Child);

    if (pTestCard == pSubCard)
      return true;

    if (pTestCard->bHasSubRecursive(pSubCard))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* is given card a sub card of this card?                                     */
/*----------------------------------------------------------------------------*/
bool Card::bHasSubRecursive(Card* pSubCard, Card*& rpParent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pTestCard = pGetChild(i_Child);

    if (pTestCard == pSubCard) {
      rpParent = this;
      return true;
    }

    if (pTestCard->bHasSubRecursive(pSubCard, rpParent))
      return true;
  }

  rpParent = nullptr;
  return false;
}

/*----------------------------------------------------------------------------*/
/* is given card a sub card of this card?                                     */
/*----------------------------------------------------------------------------*/
bool Card::bHasSubWithValue(eCK eCard, Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bIsCard(eCard) && bIsValue(szValue))
    return true;

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *p_sub = pGetChild(i_Child);

    if (p_sub->bHasSubWithValue(eCard,szValue))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* does the card have a value?                                                */
/*----------------------------------------------------------------------------*/
bool Card::bHasValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (*m_sValue.azChar())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this atom an aggregate op?                                              */
/*----------------------------------------------------------------------------*/
bool Card::bIsAggOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = szValue();

       if (szTerm->bSame("avg"_Stz))      return true;
  else if (szTerm->bSame("count"_Stz))    return true;
  else if (szTerm->bSame("countAll"_Stz)) return true;
  else if (szTerm->bSame("max"_Stz))      return true;
  else if (szTerm->bSame("min"_Stz))      return true;
//else if (szTerm->bSame("stddev"_Stz))   return true;
  else if (szTerm->bSame("sum"_Stz))      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this card the given kind?                                               */
/*----------------------------------------------------------------------------*/
bool Card::bIsCard(eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (eCard == this->eCard())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this term a comma separator?                                            */
/*----------------------------------------------------------------------------*/
bool Card::bIsCommaSeparator(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (szValue()->bSame(","_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this an AND, NOT or an OR?                                              */
/*----------------------------------------------------------------------------*/
bool Card::bIsConjunction(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (   bIsCard(CK_ANDPRED)
      || bIsCard(CK_NOTPRED)
      || bIsCard(CK_ORPRED)
      )
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/* is this card a PRED ?                                                      */
/*----------------------------------------------------------------------------*/
bool Card::bIsPRED(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  switch (m_eCard){
    case CK_ANDPRED:
    case CK_BOLPRED:
    case CK_CMPPRED:
    case CK_ORPRED :
    case CK_ROPRED:
      return true;
    default:
      return false;
  }
}

/*----------------------------------------------------------------------------*/
/* does this card have same value as argument?                                */
/*----------------------------------------------------------------------------*/
bool Card::bIsValue(char cValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (this->cValue() == cValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* does this card have same value as argument?                                */
/*----------------------------------------------------------------------------*/
bool Card::bIsValue(Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (this->szValue()->bSame(szValue))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get last print parent                                                      */
/*----------------------------------------------------------------------------*/
Card* Card::pLastPrintParent(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_pLastPrintParent;
}

/*----------------------------------------------------------------------------*/
/* set last print parent                                                     */
/*----------------------------------------------------------------------------*/
void Card::vLastPrintParent(Card* pLastPrintParent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_pLastPrintParent = pLastPrintParent;
}

/*----------------------------------------------------------------------------*/
/* get mark 1                                                                 */
/*----------------------------------------------------------------------------*/
char Card::cMark1(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_cMark1;
}

/*----------------------------------------------------------------------------*/
/* set mark 1                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vMark1(char cMark1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_cMark1 = cMark1;
}

/*----------------------------------------------------------------------------*/
/* set mark1 conditionally recursive                                          */
/*----------------------------------------------------------------------------*/
void Card::vMark1SetIfRecursive(char cIfMark, char cSetTo)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (cIfMark == m_cMark1)
    m_cMark1 = cSetTo;

  CardIter xCI(this);
  while (xCI.bEachChild())
    xCI.pChild()->vMark1SetIfRecursive(cIfMark,cSetTo);
}

/*----------------------------------------------------------------------------*/
/* get mark 2                                                                 */
/*----------------------------------------------------------------------------*/
char Card::cMark2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_cMark2;
}

/*----------------------------------------------------------------------------*/
/* get mark 2                                                                 */
/*----------------------------------------------------------------------------*/
i8 Card::iMark2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_cMark2 - '0';
}

/*----------------------------------------------------------------------------*/
/* set mark 2                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vMark2(i8 iMark2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_cMark2 = iMark2 + '0';
}

/*----------------------------------------------------------------------------*/
/* set mark 2                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vMark2(char cMark2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_cMark2 = cMark2;
}

/*----------------------------------------------------------------------------*/
/* set mark2 conditionally recursive                                          */
/*----------------------------------------------------------------------------*/
void Card::vMark2SetIfRecursive(char cIfMark, char cSetTo)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (cIfMark == m_cMark2)
    m_cMark2 = cSetTo;

  CardIter xCI(this);
  while (xCI.bEachChild())
    xCI.pChild()->vMark2SetIfRecursive(cIfMark,cSetTo);
}

/*----------------------------------------------------------------------------*/
/* reset marks recursive                                                      */
/*----------------------------------------------------------------------------*/
void Card::vMarkResetRecursive(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  m_cMark1 = MARK_BLANK;
  m_cMark2 = MARK_BLANK;

  CardIter xCI(this);
  while (xCI.bEachChild())
    xCI.pChild()->vMarkResetRecursive();
}


/*----------------------------------------------------------------------------*/
/* Merge given card with this card only if they are of the same type          */
/*----------------------------------------------------------------------------*/
bool Card::bMerge(Card* pCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if(pCard->eCard() != m_eCard)
    return false;

  while(Card* pSubCard = pCard->pPop()) {
    vPush(pSubCard);
  }

  delete pCard;

  return true;
}

/*----------------------------------------------------------------------------*/
/* morph all children of eFrom to be eTo                                      */
/*----------------------------------------------------------------------------*/
void Card::vMorphChildrenTo(eCK eFrom, eCK eTo)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (Card* pChildCard = xCI.pEachChild())
    if (pChildCard->bIsCard(eFrom))
      pChildCard->vCard(eTo);
}

/*----------------------------------------------------------------------------*/
/* move given to child to be last child                                       */
/*----------------------------------------------------------------------------*/
void Card::vMoveToLastChild(Card* pChildCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vRemoveChild(pChildCard);
  vAppend(pChildCard);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card::bNeedsBrace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bSame("between"_Stz))
    return true;
  if (bSame("case"_Stz))
    return true;
  if (bSame("in"_Stz))
    return true;
  if (bSame("matchLeading"_Stz))
    return true;
  if (bSame("matchMiddle"_Stz))
    return true;
  if (bSame("matchMiddle1"_Stz))
    return true;
  if (bSame("matchTrailing"_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* verify there are no stream column ordinals                                 */
/*----------------------------------------------------------------------------*/
void Card::vNoTermsStmColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI(this);
  while (Card* p_child = xCI.pEachChild()) {
    if (p_child->bIsCard(CK_STREAM))
      continue;

    if (p_child->bIsCard(CK_TERM)) {
      if (Card::bIsStreamColOrdinal(this->szValue()))
        vDebugPoint();

      RASSERT(! Card::bIsStreamColOrdinal(this->szValue()));
      continue;
    }

    p_child->vNoTermsStmColOdn();
  }
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card::vPrintCardRecursive(Ribbon* pRibbon, i8 iDepth, Card* pParent,
                               bool& rbPrune, eAL eLevel, bool bPrintBelow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (CK_RPNCALC == m_eCard)
    vDebugPoint();

  if (0 == iDepth) {
    if (   bIsCard(CK_AGGREG)
        || bIsCard(CK_PROJECT)) {
      vDebugPoint();
    }

    vPrintHeader(pRibbon,eLevel);
  }

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCtl());

  if (m_pLastPrintParent != pParent) {
    if (MARK_BLANK == m_cMark1)
      m_cMark1 = MARK_CHANGED;
    m_pLastPrintParent = pParent;
  }

  if (pCardCtl->bPrintByKind(m_eCard))
    vPrintCard_byKind(pRibbon, iDepth);
  else
    vPrintCard_bySettings(pRibbon, iDepth);

  if (MARK_CHANGED == m_cMark1)
    m_cMark1 = MARK_BLANK;

  if (pCardCtl->bPrune(m_eCard))
    rbPrune = true;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (rbPrune)
    return;

  if (CK_RPNCALC == m_eCard)
    vDebugPoint();

  if (! bPrintBelow)
    bPrintBelow = pCardCtl->bPrintBelow(m_eCard);

  bool bPrintSubCards = pCardCtl->bPrintSubCards(m_eCard);
  if ( (! bPrintSubCards) && (! bPrintBelow))
    return;

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();
  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();
    pSubCard->vPrintCardRecursive(pRibbon, iDepth + 1, this,
                                  rbPrune, eLevel, bPrintBelow);
  }

  delete pSubCard_I;
}

/*----------------------------------------------------------------------------*/
/* print card onto given ribbon                                               */
/*----------------------------------------------------------------------------*/
void Card::vPrintCard_byCustomNm(Ribbon* pRibbon, i8 iDepth, Stz* szCustomNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCtl());

  if (pCardCtl->bPrintName(m_eCard))
    vPrintName(pRibbon, iDepth, szCustomNm);
  if (pCardCtl->bPrintValue(m_eCard))
    vPrintValue(pRibbon, iDepth);

  if (pCardCtl->bPrintProperties(m_eCard))
    vPrintProperties(pRibbon, iDepth);

  if (pCardCtl->bPrintSubShort(m_eCard))
    vPrintChildrenShort(pRibbon, iDepth);
}

/*----------------------------------------------------------------------------*/
/* print card onto given ribbon                                               */
/*----------------------------------------------------------------------------*/
void Card::vPrintCard_byKind(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  switch (m_eCard) {

    case CK_STREAM:
      _pCard_Stream(this)->vStream_PrintStream(_pCard_Stream(this), pRibbon, iDepth);
      break;

    case CK_TABLE:
      _pCard_Table(this)->vTable_PrintTable(_pCard_Table(this),pRibbon, iDepth);
      break;

    default:
      vPrintCard_bySettings(pRibbon, iDepth);
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* print card onto given ribbon                                               */
/*----------------------------------------------------------------------------*/
void Card::vPrintCard_bySettings(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCtl());

  if (pCardCtl->bPrintName(m_eCard) && pCardCtl->bPrintValue(m_eCard)) {
    vPrintNameValue(pRibbon, iDepth);
  }
  else {
    bool bPrintName = pCardCtl->bPrintName(m_eCard);
    if (bPrintName)
      vPrintName(pRibbon, iDepth);
    if (pCardCtl->bPrintValue(m_eCard))
      vPrintValue(pRibbon, iDepth, !bPrintName);
  }

  if (pCardCtl->bPrintProperties(m_eCard))
    vPrintProperties(pRibbon, iDepth);

  if (pCardCtl->bPrintSubShort(m_eCard))
    vPrintChildrenShort(pRibbon, iDepth);
}

/*----------------------------------------------------------------------------*/
/* print compressed children                                                  */
/*----------------------------------------------------------------------------*/
void Card::vPrintChildrenShort(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nChildren = nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChild = pGetChild(i_Child);

    i8 nSubs = pChild->nChildCards();
    if (nSubs == 0) {
      /*none*/;
    }
    else
    if (nSubs == 1) {
      Card *pSub_0 = pChild->pGetChild(0);
      pRibbon->vSprintRight("%s"_Stz,pSub_0->sz());
    }
    else {
      Card *pSub_0 = pChild->pGetChild(0);
      Card *pSub_1 = pChild->pGetChild(1);
      pRibbon->vSprintRight("%s == %s"_Stz,pSub_0->sz(),pSub_1->sz());
    }

  }
}

/*----------------------------------------------------------------------------*/
/* put header                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vPrintHeader(Ribbon* pRibbon, eAL eLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  char cLevel = ' ';
  switch (eLevel) {
    case AL_PARTITION : cLevel = 'P'; break;
    case AL_NODE      : cLevel = 'N'; break;
    case AL_ORIGIN    : cLevel = 'O'; break;
    default           : cLevel = '?'; break;
  }

  pRibbon->vSprint("==%02lld %c--------------"_Stz,m_iCardNo,cLevel);
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card::vPrintName(Ribbon* pRibbon, i8 iDepth, Stz* szCustomName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCtl());

  s32 sHiddenChildren;
  if (! pCardCtl->bPrintSubCards(m_eCard) && nChildCards())
    sHiddenChildren = " +"_Stz;

  Board* pBoard = _pBoard(pRibbon->pSurface());

  s32 sName;
  if (0 == iDepth) {
    if (szCustomName) {
      sName.vSprint("%s"_Stz,
                    szCustomName);
    }
    else {
      sName.vSprint("%s%s *%lld"_Stz,
                    szCardNm(),
                    sHiddenChildren.szStz(),
                    nItems());
      sName.vToUpper();
    }
  }
  else {
    if (pCardCtl->bPrintNo(m_eCard))
      sName.vSprint("%s%lld]"_Stz,
                    Stz::szDots(iDepth - 1),
                    m_iCardNo);
    if (szCustomName)
      sName.vSprintCat("%s"_Stz,
                    szCustomName);
    else
      sName.vSprintCat("%s%s"_Stz,
                       szCardNm(),
                       sHiddenChildren.szStz());
  }

  pRibbon->vSprint("%c%c%-18s"_Stz, m_cMark1, m_cMark2, sName.szStz());
}

/*----------------------------------------------------------------------------*/
/* print card name and value on single line if possible                       */
/*----------------------------------------------------------------------------*/
void Card::vPrintNameValue(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCtl());

  s32 sHiddenChildren;
  if (! pCardCtl->bPrintSubCards(m_eCard) && nChildCards())
    sHiddenChildren = " +"_Stz;

  s32 sName(szCardNm());
  if (0 == iDepth)
    sName.vToUpper();

  s256 sNameValue;

  if (0 == iDepth) {
    if (bHasValue())
      sNameValue.vSprint("%s%s %s *%lld"_Stz,
                         sName.szStz(),
                         sHiddenChildren.szStz(),
                         szValue(),
                         nItems());
    else
      sNameValue.vSprint("%s%s *%lld"_Stz,
                         sName.szStz(),
                         sHiddenChildren.szStz(),
                         nItems());

  }
  else {
    if (pCardCtl->bPrintNo(m_eCard))
      sNameValue.vSprint("%s%lld]"_Stz,
                    Stz::szDots(iDepth - 1),
                    m_iCardNo);
    if (bHasValue())
      sNameValue.vSprintCat("%s%s %s"_Stz,
                       sName.szStz(),
                       sHiddenChildren.szStz(),
                       szValue());
    else
      sNameValue.vSprintCat("%s%s"_Stz,
                         sName.szStz(),
                         sHiddenChildren.szStz());
  }

  if (sNameValue.nLength() <= 18) {
    pRibbon->vSprint("%c%c%-18s"_Stz, m_cMark1, m_cMark2, sNameValue.szStz());
  }
  else {
    vPrintName(pRibbon, iDepth);
    vPrintValue(pRibbon, iDepth);
  }
}

/*----------------------------------------------------------------------------*/
/* print card properties                                                      */
/*----------------------------------------------------------------------------*/
void Card::vPrintProperties(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return;

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCtl());

  bool bPrintPptName = pCardCtl->bPrintPptName(m_eCard);

  i8 nProperties = this->Y_nPpy();

  for (i8 i_Property = 0; i_Property < nProperties; i_Property++) {

    eTN eTkn = Y_ePpy(i_Property);

    switch (eTkn) {

      case TN_INTEGER: {
        Stz* szName = Y_szNm(i_Property);
        i8   iValue = Y_iGet(i_Property);

        Stz* szDecode = szDecodeEnum(szName,iValue);
        if (szDecode)
          pRibbon->vSprintRight("%s"_Stz,szDecode);
        else
        if (bPrintPptName)
          pRibbon->vSprintRight("%s=%lld"_Stz,szName,iValue);
        else
          pRibbon->vSprintRight("%lld"_Stz,iValue);
        break;
      }

      case TN_OBJECT: {
        Stz* szName = Y_szNm(i_Property);
        Object* pObject = Y_pGet(i_Property);
        pRibbon->vSprintRight("%s->  "_Stz,szName);
        pRibbon->vSprintRight("%p"_Stz,pObject);
        pRibbon->vSprintRight("%s"_Stz,aCN[pObject->eGetClassNo()]);
        break;
      }

      case TN_STRING: {
        Stz* szName  = Y_szNm(i_Property);
        Stz* szValue = Y_szGet(i_Property);
        if (bPrintPptName)
          pRibbon->vSprintRight("%s=%s"_Stz,szName,szValue);
        else
          pRibbon->vSprintRight("%s"_Stz,szValue);
        break;
      }

      default:
        break;
    }

  }
}

/*----------------------------------------------------------------------------*/
/* print value of card                                                        */
/*----------------------------------------------------------------------------*/
void Card::vPrintValue(Ribbon* pRibbon, i8 iDepth, bool bPrintMarks)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! bHasValue())
    return;

  if (bPrintMarks) {
    pRibbon->vSprintRight("%c%c%18s"_Stz, m_cMark1, m_cMark2, szValue());

  }
  else {
    pRibbon->vSprintRight("  %s"_Stz, szValue());
  }
}

/*----------------------------------------------------------------------------*/
/* get properties tagSet                                                      */
/*----------------------------------------------------------------------------*/
TagSet* Card::pProperties(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_pProperties;
}

/*----------------------------------------------------------------------------*/
/* ensure this card has a m_pProperties tagSet                                */
/*----------------------------------------------------------------------------*/
void Card::vPropertiesEnsure(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;
}

/*----------------------------------------------------------------------------*/
/* renumber subCard's                                                         */
/*----------------------------------------------------------------------------*/
void Card::vRenumberRecursive(Card* pParent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  vBearing(pParent);

  i8 iSubCardNo = 0;
  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();

    pSubCard->vCardNo(iSubCardNo++);
    pSubCard->vRenumberRecursive(this);
  }

  delete pSubCard_I;

  vBearing();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vRemove_filter_IfEmpty(Card_Filter* p_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  // todo: xt

  Card_BoolCalc* p_boolCalc = p_filter->pSub_boolCalc();

  if (p_boolCalc->nChildCards())
    return;

  this->vRemoveChild(p_filter);
  delete p_filter;
}

/*----------------------------------------------------------------------------*/
/* replace subCard                                                            */
/*----------------------------------------------------------------------------*/
void Card::vReplaceSubCard(Card* pSub, Card* pWith)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card* pParent = pFindParent(pSub);

  pParent->vReplaceChild(pSub,pWith);

}

/*----------------------------------------------------------------------------*/
/* is this card's value the same as given arg?                                */
/*----------------------------------------------------------------------------*/
bool Card::bSame(Stz* szArg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (szValue()->bSame(szArg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this card's value the same as given arg?                                */
/*----------------------------------------------------------------------------*/
bool Card::bSameValue(Card* pArg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (szValue()->bSame(pArg->szValue()))
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vSetIndexColOrdinal(i8 iIdxColOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValueSprint("$I%lld"_Stz, iIdxColOdn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vSetKeyOrdinal(i8 iKeyOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValueSprint("$K%lld"_Stz, iKeyOdn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vSetParamOrdinal(i8 iParamOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValueSprint("$P%lld"_Stz, iParamOdn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vSetStreamColOrdinal(i8 iStmColOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValueSprint("$%lld"_Stz, iStmColOdn);
}

/*----------------------------------------------------------------------------*/
/* replace c in $c.n                                                          */
/*----------------------------------------------------------------------------*/
void Card::vSetTableColSyn(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  ((char*)this->sz())[1] = cTblSyn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vSetTableColOrdinal(char cTblSyn, i8 iTblColOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValueSprint("$%c.%lld"_Stz, cTblSyn, iTblColOdn);
}

/*----------------------------------------------------------------------------*/
/* Take all subcards from given card and move under this card (top)           */
/*----------------------------------------------------------------------------*/
void Card::vTakeAllSubCards(Card* pCard, bool bDeleteCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  while(Card* pSubCard = pCard->pPop()) {
    vPush(pSubCard);
  }

  if(bDeleteCard)
    delete pCard;
}

/*----------------------------------------------------------------------------*/
/* get card value                                                             */
/*----------------------------------------------------------------------------*/
Stz* Card::sz(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return m_sValue.szStz();
}

/*----------------------------------------------------------------------------*/
/* get card value                                                             */
/*----------------------------------------------------------------------------*/
char Card::cValue(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  char c = * m_sValue.azChar();

  return c;
}

/*----------------------------------------------------------------------------*/
/* get card value                                                             */
/*----------------------------------------------------------------------------*/
i8 Card::iValue(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i = m_sValue.iToI8();

  return i;
}

/*----------------------------------------------------------------------------*/
/* get card value                                                             */
/*----------------------------------------------------------------------------*/
Stz* Card::szValue(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return m_sValue.szStz();
}

/*----------------------------------------------------------------------------*/
/* set card value                                                             */
/*----------------------------------------------------------------------------*/
void Card::vValue(char cValue)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValueSprint("%c"_Stz,cValue);
}

/*----------------------------------------------------------------------------*/
/* set card value                                                             */
/*----------------------------------------------------------------------------*/
void Card::vValue(i8 iValue)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValueSprint("%lld"_Stz,iValue);
}

/*----------------------------------------------------------------------------*/
/* set card value                                                             */
/*----------------------------------------------------------------------------*/
void Card::vValue(Stz* szValue)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  //RASSERT(! *m_sValue.azChar());

  m_sValue = szValue;
}

/*----------------------------------------------------------------------------*/
/* set card value and Mark1                                                   */
/*----------------------------------------------------------------------------*/
void Card::vSetValueMark1(Stz* szValue, char cMark1)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValue(szValue);
  vMark1(cMark1);
}

/*----------------------------------------------------------------------------*/
/* sprint card value                                                          */
/*----------------------------------------------------------------------------*/
void Card::vValueSprint(Stz* szFormat, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  s256 sValue;

  va_list xArgs;
  va_start(xArgs,szFormat);
  StrLib::vSprint(sValue.azChar(),xArgs,(char*)szFormat);
  va_end(xArgs);

  vValue(sValue);
}

/*----------------------------------------------------------------------------*/
/* are the two cards recursively the same                                     */
/*----------------------------------------------------------------------------*/
bool Card::bValuesSameRecursive(Card* pCard_b, i8 iDepth, bool bTestL0)
{
  _static_

  if (iDepth || (0 == iDepth && bTestL0)) {

    eCK eCard_a = this->eCard();
    eCK eCard_b = pCard_b->eCard();

    if (eCard_a != eCard_b)
      return false;

    //if (CK_TERM == eCard_a)
    if (! this->sz()->bSame(pCard_b->sz()))
      return false;
  }

  i8 nChildren_a = this->nChildCards();
  i8 nChildren_b = pCard_b->nChildCards();
  if (nChildren_a != nChildren_b)
    return false;

  for (i8 i_Child = 0; i_Child < nChildren_a; i_Child++) {
    Card* pChild_a = this->pGetChild(i_Child);
    Card* pChild_b = pCard_b->pGetChild(i_Child);
    if (! pChild_a->bValuesSameRecursive(pChild_b, iDepth+1, bTestL0))
      return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* remove given property                                                      */
/*----------------------------------------------------------------------------*/
void Card::Y_vCut(Stz* szPpyNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return;

  m_pProperties->vDelete(szPpyNm);
}

/*----------------------------------------------------------------------------*/
/* delete given property (recursively)                                        */
/*----------------------------------------------------------------------------*/
void Card::Y_vCutRecursive(Stz* szPpyNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Y_vCut(szPpyNm);

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pChildCard = pGetChild(i_Child);
    pChildCard->Y_vCutRecursive(szPpyNm);
  }
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
char Card::Y_cGet(Stz* szPpyNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return ' ';

  Stz* szValue = m_pProperties->szValue(szPpyNm);
  if (! szValue)
    return ' ';

  return *szValue->azChar();
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
char Card::Y_cGet(i8 i_Ppy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return ' ';

  Stz* szValue = m_pProperties->szValue(i_Ppy);
  if (! szValue)
    return ' ';

  return *szValue->azChar();
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
i8 Card::Y_iGet(Stz* szPpyNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return C_UNKNOWN;

  i8 iValue = m_pProperties->iValue(szPpyNm);

  return iValue;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
i8 Card::Y_iGet(i8 i_Ppy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return C_UNKNOWN;

  i8 iValue = m_pProperties->iValue(i_Ppy);

  return iValue;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
Object* Card::Y_pGet(Stz* szPptNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return nullptr;

  Object* pValue = m_pProperties->pValue(szPptNm);

  return pValue;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
Object* Card::Y_pGet(i8 i_Ppy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return nullptr;

  Object* pValue = m_pProperties->pValue(i_Ppy);

  return pValue;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
Stz* Card::Y_szGet(Stz* szPptNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return nullptr;

  Stz* szValue = m_pProperties->szValue(szPptNm);

  return szValue;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
Stz* Card::Y_szGet(i8 i_Ppy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return nullptr;

  Stz* szValue = m_pProperties->szValue(i_Ppy);

  return szValue;
}

/*----------------------------------------------------------------------------*/
/* has given property ?                                                       */
/*----------------------------------------------------------------------------*/
bool Card::Y_bHas(Stz* szPpyNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return false;

  return m_pProperties->bHasName(szPpyNm);
}

/*----------------------------------------------------------------------------*/
/* get given property name                                                    */
/*----------------------------------------------------------------------------*/
Stz* Card::Y_szNm(i8 i_Ppy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return nullptr;

  Stz* szName = m_pProperties->szName(i_Ppy);

  return szName;
}

/*----------------------------------------------------------------------------*/
/* get type of property                                                       */
/*----------------------------------------------------------------------------*/
eTN Card::Y_ePpy(i8 i_Ppy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return TN_NONE;

  eTN eToken = m_pProperties->eToken(i_Ppy);

  return eToken;
}

/*----------------------------------------------------------------------------*/
/* get number of properties                                                   */
/*----------------------------------------------------------------------------*/
i8 Card::Y_nPpy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return 0;

  i8 nProperties = m_pProperties->nItems();

  return nProperties;
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::Y_vSet(Stz* szPpyNm, char cPpy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;

  s32 sPptVal;
  sPptVal.vSprint("%c"_Stz, cPpy);
  m_pProperties->vReplace(szPpyNm, sPptVal);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::Y_vSet(Stz* szPpyNm, i8 iPpy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vReplace(szPpyNm, iPpy);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::Y_vSet(Stz* szPpyNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_)

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vReplace(szPpyNm, "y"_Stz);
}

/*----------------------------------------------------------------------------*/
/* sprint card property value                                                 */
/*----------------------------------------------------------------------------*/
void Card::Y_vSetSprint(Stz* szPpyNm, Stz* szFormat, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  s256 sPpyValue;

  va_list xArgs;
  va_start(xArgs,szFormat);
  StrLib::vSprint(sPpyValue.azChar(),xArgs,(char*)szFormat);
  va_end(xArgs);

  Y_vSet(szPpyNm,sPpyValue);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::Y_vSet(Stz* szPpyNm, Object* pPpy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vReplace(szPpyNm, pPpy);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::Y_vSet(Stz* szPpyNm, Stz* szPpy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vReplace(szPpyNm, szPpy);
}

/*----------------------------------------------------------------------------*/
/* yank some subcards                                                         */
/*----------------------------------------------------------------------------*/
Card_SubSet* Card::pYankSubCards(i8 i_From, i8 i_Till)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_SubSet* p_subSet = new Card_SubSet;

  while (i_From < nChildCards()
         && (C_UNKNOWN == i_Till || i_From <= i_Till)) {
    Card* p_child = pGetChild(i_From);

    vRemoveChild(p_child);

    p_subSet->vAppend(p_child);

    if (C_UNKNOWN != i_Till)
      i_Till--;
  }

  return p_subSet;
}



/*----------------------------------------------------------------------------*/
/* get number of stream columns                                               */
/*----------------------------------------------------------------------------*/
void Card::vStream_PrintStream(Card_Stream* p_stream, Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  s32 sName;
  sName.vSprint("%s%s"_Stz, szCardNm(), Y_nPpy() ? " +" : "");

  if (Y_bHas(P_OdnLo))
    sName.vCatSprint(" $%lld,%lld"_Stz,
                     Y_iGet(P_OdnLo), Y_iGet(P_OdnHi));

  vPrintCard_byCustomNm(pRibbon, iDepth, sName);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vTable_PrintTable(Card_Table* p_Table, Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  s32 sName;
  sName = aCK_short[m_eCard];
  if (0 == iDepth)
    sName.vToUpper();

  if (bHasValue())
    sName.vCatSprint(" %c"_Stz, cValue());

  vPrintCard_byCustomNm(pRibbon, iDepth, sName);
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Agg.hpp"
Card_Agg* Card::pSub_agg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Agg* p_agg =
      _pCard_Agg(pFindSubCard(CK_AGG));
  return p_agg;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Aggreg.hpp"
Card_Aggreg* Card::pSub_aggreg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Aggreg* p_aggreg =
      _pCard_Aggreg(pFindSubCard(CK_AGGREG));
  return p_aggreg;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_As.hpp"
Card_As* Card::pSub_as(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_As* p_as =
      _pCard_As(pFindSubCard(CK_AS));
  return p_as;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_BolPred.hpp"
Card_BolPred* Card::pSub_bolPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_BolPred* p_bolPred =
      _pCard_BolPred(pFindSubCard(CK_BOLPRED));
  return p_bolPred;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_BoolCalc.hpp"
Card_BoolCalc* Card::pSub_boolCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_BoolCalc* p_boolCalc =
      _pCard_BoolCalc(pFindSubCard(CK_BOOLCALC));
  return p_boolCalc;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Bound.hpp"
Card_Bound* Card::pSub_bound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Bound* p_bound =
      _pCard_Bound(pFindSubCard(CK_BOUND));
  return p_bound;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Bounds.hpp"
Card_Bounds* Card::pSub_bounds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Bounds* p_bounds =
      _pCard_Bounds(pFindSubCard(CK_BOUNDS));
  return p_bounds;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_CalcOp.hpp"
Card_CalcOp* Card::pSub_calcOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CalcOp* p_calcOp =
      _pCard_CalcOp(pFindSubCard(CK_CALCOP));
  return p_calcOp;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Cast.hpp"
Card_Cast* Card::pSub_cast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Cast* p_cast =
      _pCard_Cast(pFindSubCard(CK_CAST));
  return p_cast;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Clause.hpp"
Card_Clause* Card::pSub_clause(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Clause* p_clause =
      _pCard_Clause(pFindSubCard(CK_CLAUSE));
  return p_clause;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_ColNm.hpp"
Card_ColNm* Card::pSub_colNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_ColNm* p_colNm =
      _pCard_ColNm(pFindSubCard(CK_COLNM));
  return p_colNm;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_CmpPred.hpp"
Card_CmpPred* Card::pSub_cmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CmpPred* p_predCmp =
      _pCard_CmpPred(pFindSubCard(CK_CMPPRED));
  return p_predCmp;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Comment.hpp"
Card_Comment* Card::pSub_comment(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Comment* p_comment =
      _pCard_Comment(pFindSubCard(CK_COMMENT));
  return p_comment;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Constraint.hpp"
Card_Constraint* Card::pSub_constraint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Constraint* p_constraint =
      _pCard_Constraint(pFindSubCard(CK_CONSTRAINT));
  return p_constraint;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_CrossJoin.hpp"
Card_CrossJoin* Card::pSub_crossJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CrossJoin* p_crossJoin =
      _pCard_CrossJoin(pFindSubCard(CK_CROSSJOIN));
  return p_crossJoin;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Direction.hpp"
Card_Direction* Card::pSub_direction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Direction* p_direction =
      _pCard_Direction(pFindSubCard(CK_DIRECTION));
  return p_direction;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_EBound.hpp"
Card_EBound* Card::pSub_eBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_EBound* p_eBound =
      _pCard_EBound(pFindSubCard(CK_EBOUND));
  return p_eBound;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Expr.hpp"
Card_Expr* Card::pSub_expr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Expr* p_expr =
      _pCard_Expr(pFindSubCard(CK_EXPR));
  return p_expr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Filter.hpp"
Card_Filter* Card::pSub_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Filter* p_filter =
      _pCard_Filter(pFindSubCard(CK_FILTER));
  return p_filter;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Fold.hpp"
Card_Fold* Card::pSub_fold(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Fold* p_fold =
      _pCard_Fold(pFindSubCard(CK_FOLD));
  return p_fold;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_FoldElist.hpp"
Card_FoldElist* Card::pSub_foldElist(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_FoldElist* p_foldExpr =
      _pCard_FoldElist(pFindSubCard(CK_FOLDELIST));
  return p_foldExpr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_From.hpp"
Card_From* Card::pSub_from(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_From* p_from =
      _pCard_From(pFindSubCard(CK_FROM));
  return p_from;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_GbyAgg.hpp"
Card_GbyAgg* Card::pSub_gbyAgg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_GbyAgg* p_gbyAgg =
      _pCard_GbyAgg(pFindSubCard(CK_GBYAGG));
  return p_gbyAgg;
}

//*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Genesis.hpp"
Card_Genesis* Card::pSub_genesis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Genesis* p_genesis =
      _pCard_Genesis(pFindSubCard(CK_GENESIS));
  return p_genesis;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Group.hpp"
Card_Group* Card::pSub_group(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Group* p_group =
      _pCard_Group(pFindSubCard(CK_GROUP));
  return p_group;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_IdxColList.hpp"
Card_IdxColList* Card::pSub_idxColList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_IdxColList* p_idxColList =
      _pCard_IdxColList(pFindSubCard(CK_IDXCOLLIST));
  return p_idxColList;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_IdxColNm.hpp"
Card_IdxColNm* Card::pSub_idxColNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_IdxColNm* p_idxColNm =
      _pCard_IdxColNm(pFindSubCard(CK_IDXCOLNM));
  return p_idxColNm;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Index.hpp"
Card_Index* Card::pSub_index(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Index* p_index =
      _pCard_Index(pFindSubCard(CK_INDEX));
  return p_index;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_IndexJoin.hpp"
Card_IndexJoin* Card::pSub_indexJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_IndexJoin* p_indexJoin =
      _pCard_IndexJoin(pFindSubCard(CK_INDEXJOIN));
  return p_indexJoin;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_IndexList.hpp"
Card_IndexList* Card::pSub_indexList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_IndexList* p_indexList =
      _pCard_IndexList(pFindSubCard(CK_INDEXLIST));
  return p_indexList;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_IndexNm.hpp"
Card_IndexNm* Card::pSub_indexNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_IndexNm* p_indexNm =
      _pCard_IndexNm(pFindSubCard(CK_INDEXNM));
  return p_indexNm;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Key.hpp"
Card_Key* Card::pSub_key(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Key* p_key =
      _pCard_Key(pFindSubCard(CK_KEY));
  return p_key;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_KeyExists.hpp"
Card_KeyExists* Card::pSub_keyExists(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_KeyExists* p_keyExists =
      _pCard_KeyExists(pFindSubCard(CK_KEYEXISTS));
  return p_keyExists;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_KeyNotExists.hpp"
Card_KeyNotExists* Card::pSub_keyNotExists(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_KeyNotExists* p_keyNotExists =
      _pCard_KeyNotExists(pFindSubCard(CK_KEYNOTEXISTS));
  return p_keyNotExists;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_LBound.hpp"
Card_LBound* Card::pSub_lBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_LBound* p_lBound =
      _pCard_LBound(pFindSubCard(CK_LBOUND));
  return p_lBound;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Level.hpp"
Card_Level* Card::pSub_level(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Level* p_level =
      _pCard_Level(pFindSubCard(CK_LEVEL));
  return p_level;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_LoadJoin.hpp"
Card_LoadJoin* Card::pSub_loadJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_LoadJoin* p_loadJoin =
      _pCard_LoadJoin(pFindSubCard(CK_LOADJOIN));
  return p_loadJoin;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_LoadJoined.hpp"
Card_LoadJoined* Card::pSub_loadJoined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_LoadJoined* p_loadJoined =
      _pCard_LoadJoined(pFindSubCard(CK_LOADJOINED));
  return p_loadJoined;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_LoadNetwork.hpp"
Card_LoadNetwork* Card::pSub_loadNetwork(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_LoadNetwork* p_loadNetwork =
      _pCard_LoadNetwork(pFindSubCard(CK_LOADNETWORK));
  return p_loadNetwork;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_LoadPermSeq.hpp"
Card_LoadPermSeq* Card::pSub_loadPermSeq(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_LoadPermSeq* p_loadPermSeq =
      _pCard_LoadPermSeq(pFindSubCard(CK_LOADPERMSEQ));
  return p_loadPermSeq;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_LoadUnicast.hpp"
Card_LoadUnicast* Card::pSub_loadUnicast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_LoadUnicast* p_loadUnicast =
      _pCard_LoadUnicast(pFindSubCard(CK_LOADUNICAST));
  return p_loadUnicast;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Manner.hpp"
Card_Manner* Card::pSub_manner(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Manner* p_manner =
      _pCard_Manner(pFindSubCard(CK_MANNER));
  return p_manner;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_NCorrCols.hpp"
Card_NCorrCols* Card::pSub_nCorrCols(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_NCorrCols* p_nCorrCols =
      _pCard_NCorrCols(pFindSubCard(CK_NCORRCOLS));
  return p_nCorrCols;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Next.hpp"
Card_Next* Card::pSub_next(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Next* p_next =
      _pCard_Next(pFindSubCard(CK_NEXT));
  return p_next;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Note.hpp"
Card_Note* Card::pSub_note(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Note* p_note =
      _pCard_Note(pFindSubCard(CK_NOTE));
  return p_note;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_NotPred.hpp"
Card_NotPred* Card::pSub_notPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_NotPred* p_notPred =
      _pCard_NotPred(pFindSubCard(CK_NOTPRED));
  return p_notPred;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_NParmCols.hpp"
Card_NParmCols* Card::pSub_nParmCols(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_NParmCols* p_nParmCols =
      _pCard_NParmCols(pFindSubCard(CK_NPARMCOLS));
  return p_nParmCols;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_On.hpp"
Card_On* Card::pSub_on(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_On* p_On =
      _pCard_On(pFindSubCard(CK_ON));
  return p_On;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_OrdElmOdn.hpp"
Card_OrdElmOdn* Card::pSub_ordElmOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_OrdElmOdn* p_streamOrdOdn =
      _pCard_OrdElmOdn(pFindSubCard(CK_ORDELMODN));
  return p_streamOrdOdn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Order.hpp"
Card_Order* Card::pSub_order(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Order* p_order =
      _pCard_Order(pFindSubCard(CK_ORDER));
  return p_order;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_PmzWhere.hpp"
Card_PmzWhere* Card::pSub_pmzWhere(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_PmzWhere* p_pmzWhere =
      _pCard_PmzWhere(pFindSubCard(CK_PMZWHERE));
  return p_pmzWhere;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Projection.hpp"
Card_Projection* Card::pSub_projection(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Projection* p_projection =
      _pCard_Projection(pFindSubCard(CK_PROJECTION));
  return p_projection;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Reason.hpp"
Card_Reason* Card::pSub_reason(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Reason* p_reason =
      _pCard_Reason(pFindSubCard(CK_REASON));
  return p_reason;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_RoPred.hpp"
Card_RoPred* Card::pSub_roPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_RoPred* p_roPred =
      _pCard_RoPred(pFindSubCard(CK_ROPRED));
  return p_roPred;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_RpnCalc.hpp"
Card_RpnCalc* Card::pSub_rpnCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_RpnCalc* p_calc =
      _pCard_RpnCalc(pFindSubCard(CK_RPNCALC));
  return p_calc;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Scan.hpp"
Card_Scan* Card::pSub_scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Scan* p_scan =
      _pCard_Scan(pFindSubCard(CK_SCAN));
  return p_scan;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Section.hpp"
Card_Section* Card::pSub_section(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Section* p_section =
      _pCard_Section(pFindSubCard(CK_SECTION));
  return p_section;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Size.hpp"
Card_Size* Card::pSub_size(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Size* p_size =
      _pCard_Size(pFindSubCard(CK_SIZE));
  return p_size;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Stream.hpp"
Card_Stream* Card::pSub_stream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Stream* p_stream =
      _pCard_Stream(pFindSubCard(CK_STREAM));
  return p_stream;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_StmCol.hpp"
Card_StmCol* Card::pSub_stmCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StmCol* p_streamCol =
      _pCard_StmCol(pFindSubCard(CK_STMCOL));
  return p_streamCol;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_StmColList.hpp"
Card_StmColList* Card::pSub_stmColList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StmColList* p_streamColList =
      _pCard_StmColList(pFindSubCard(CK_STMCOLLIST));
  return p_streamColList;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_StmColNm.hpp"
Card_StmColNm* Card::pSub_stmColNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StmColNm* p_streamColNm =
      _pCard_StmColNm(pFindSubCard(CK_STMCOLNM));
  return p_streamColNm;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_StmColOdn.hpp"
Card_StmColOdn* Card::pSub_stmColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StmColOdn* p_streamColOrd =
      _pCard_StmColOdn(pFindSubCard(CK_STMCOLODN));
  return p_streamColOrd;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Table.hpp"
Card_Table* Card::pSub_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Table* p_table =
      _pCard_Table(pFindSubCard(CK_TABLE));
  return p_table;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_TableNm.hpp"
Card_TableNm* Card::pSub_tableNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_TableNm* p_tableNm =
      _pCard_TableNm(pFindSubCard(CK_TABLENM));
  return p_tableNm;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_TablePtn.hpp"
Card_TablePtn* Card::pSub_tablePtn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_TablePtn* p_tablePtn =
      _pCard_TablePtn(pFindSubCard(CK_TABLEPTN));
  return p_tablePtn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_TblColOdn.hpp"
Card_TblColOdn* Card::pSub_tblColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_TblColOdn* p_tblColOdn =
      _pCard_TblColOdn(pFindSubCard(CK_TBLCOLODN));
  return p_tblColOdn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_TblSyn.hpp"
Card_TblSyn* Card::pSub_tblSyn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_TblSyn* p_tblSyn =
      _pCard_TblSyn(pFindSubCard(CK_TBLSYN));
  return p_tblSyn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Term.hpp"
Card_Term* Card::pSub_term(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term =
      _pCard_Term(pFindSubCard(CK_TERM));
  return p_term;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Top.hpp"
Card_Top* Card::pSub_top(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Top* p_top =
      _pCard_Top(pFindSubCard(CK_TOP));
  return p_top;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Topology.hpp"
Card_Topology* Card::pSub_topology(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Topology* p_stageTopo =
      _pCard_Topology(pFindSubCard(CK_TOPOLOGY));
  return p_stageTopo;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Twist.hpp"
Card_Twist* Card::pSub_twist(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Twist* p_twist =
      _pCard_Twist(pFindSubCard(CK_TWIST));
  return p_twist;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Type.hpp"
Card_Type* Card::pSub_type(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Type* p_type =
      _pCard_Type(pFindSubCard(CK_TYPE));
  return p_type;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_UBound.hpp"
Card_UBound* Card::pSub_uBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_UBound* p_uBound =
      _pCard_UBound(pFindSubCard(CK_UBOUND));
  return p_uBound;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Unique.hpp"
Card_Unique* Card::pSub_unique(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Unique* p_unique =
      _pCard_Unique(pFindSubCard(CK_UNIQUE));
  return p_unique;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Where.hpp"
Card_Where* Card::pSub_where(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Where* p_where =
      _pCard_Where(pFindSubCard(CK_WHERE));
  return p_where;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_XBound.hpp"
Card_XBound* Card::pSub_xBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_XBound* p_xBound =
      _pCard_XBound(pFindSubCard(CK_XBOUND));
  return p_xBound;
}


/*----------------------------------------------------------------------------*/
/* Aggregate View                                                             */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Av_AggP.hpp"
Card_Av_AggP* Card::pSub_avAggP(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Av_AggP* p_avAggP =
      _pCard_Av_AggP(pFindSubCard(CK_AV_AGGP));
  return p_avAggP;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Av_AggPN.hpp"
Card_Av_AggPN* Card::pSub_avAggPN(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Av_AggPN* p_avAggPN =
      _pCard_Av_AggPN(pFindSubCard(CK_AV_AGGPN));
  return p_avAggPN;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Av_AggPNO.hpp"
Card_Av_AggPNO* Card::pSub_avAggPNO(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Av_AggPNO* p_avAggPNO =
      _pCard_Av_AggPNO(pFindSubCard(CK_AV_AGGPNO));
  return p_avAggPNO;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Av_AggregP.hpp"
Card_Av_AggregP* Card::pSub_avAggregP(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Av_AggregP* p_avAggregP =
      _pCard_Av_AggregP(pFindSubCard(CK_AV_AGGREGP));
  return p_avAggregP;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Av_AggregPN.hpp"
Card_Av_AggregPN* Card::pSub_avAggregPN(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Av_AggregPN* p_avAggregPN =
      _pCard_Av_AggregPN(pFindSubCard(CK_AV_AGGREGPN));
  return p_avAggregPN;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Av_AggregPNO.hpp"
Card_Av_AggregPNO* Card::pSub_avAggregPNO(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Av_AggregPNO* p_avAggregPNO =
      _pCard_Av_AggregPNO(pFindSubCard(CK_AV_AGGREGPNO));
  return p_avAggregPNO;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Av_Filter.hpp"
Card_Av_Filter* Card::pSub_avFilter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Av_Filter* p_avFilter =
      _pCard_Av_Filter(pFindSubCard(CK_AV_FILTER));
  return p_avFilter;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Av_FilterProject.hpp"
Card_Av_FilterProject* Card::pSub_avFilterProject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Av_FilterProject* p_avFilterProject =
      _pCard_Av_FilterProject(pFindSubCard(CK_AV_FILTERPROJECT));
  return p_avFilterProject;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Av_GbyAggP.hpp"
Card_Av_GbyAggP* Card::pSub_avGbyAggP(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Av_GbyAggP* p_avGbyAggP =
      _pCard_Av_GbyAggP(pFindSubCard(CK_AV_GBYAGGP));
  return p_avGbyAggP;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Av_GbyAggPN.hpp"
Card_Av_GbyAggPN* Card::pSub_avGbyAggPN(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Av_GbyAggPN* p_avGbyAggPN =
      _pCard_Av_GbyAggPN(pFindSubCard(CK_AV_GBYAGGPN));
  return p_avGbyAggPN;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Av_GbyPNO.hpp"
Card_Av_GbyPNO* Card::pSub_avGbyPNO(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Av_GbyPNO* p_avGbyPNO =
      _pCard_Av_GbyPNO(pFindSubCard(CK_AV_GBYPNO));
  return p_avGbyPNO;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Av_JoinSeries.hpp"
Card_Av_JoinSeries* Card::pSub_avJoinSeries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Av_JoinSeries* p_avJoinSeries =
      _pCard_Av_JoinSeries(pFindSubCard(CK_AV_JOINSERIES));
  return p_avJoinSeries;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Av_Limit.hpp"
Card_Av_Limit* Card::pSub_avLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Av_Limit* p_avLimit =
      _pCard_Av_Limit(pFindSubCard(CK_AV_LIMIT));
  return p_avLimit;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Av_Project.hpp"
Card_Av_Project* Card::pSub_avProject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Av_Project* p_avProject =
      _pCard_Av_Project(pFindSubCard(CK_AV_PROJECT));
  return p_avProject;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Av_ScanJoinSeries.hpp"
Card_Av_ScanJoinSeries* Card::pSub_avScanJoinSeries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Av_ScanJoinSeries* p_avScanJoinSeries =
      _pCard_Av_ScanJoinSeries(pFindSubCard(CK_AV_SCANJOINSERIES));
  return p_avScanJoinSeries;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Av_ScanOnly.hpp"
Card_Av_ScanOnly* Card::pSub_avScanOnly(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Av_ScanOnly* p_avScanOnly =
      _pCard_Av_ScanOnly(pFindSubCard(CK_AV_SCANONLY));
  return p_avScanOnly;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Av_Sort.hpp"
Card_Av_Sort* Card::pSub_avSort(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Av_Sort* p_avSort =
      _pCard_Av_Sort(pFindSubCard(CK_AV_SORT));
  return p_avSort;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* decode enum                                                                */
/*----------------------------------------------------------------------------*/
Stz* Card::szDecodeEnum(Stz* szEnum, i8 iEnum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  if (szEnum->bSame("eLevel"_Stz))      return aAL[iEnum];
  if (szEnum->bSame("eStageTopo"_Stz))  return aYG[iEnum];
  if (szEnum->bSame("eSendKind"_Stz))   return aXK[iEnum];

  return nullptr; //"**unknown**"_Stz;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
