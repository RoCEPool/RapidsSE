/*============================================================================*/
/* Card.cpp -- Abstract Card                                                  */
/*============================================================================*/

#include "Card.hpp"
#include "CardCtl.hpp"
#include "Board.hpp"
#include "CardIter.hpp"

#include "Card_Index.hpp"  // TODO: remove
#include "Index.hpp"       // TODO: remove

Card* gapCard[C_10K];

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

  m_eCard   = eCard;
  m_iCardNo = C_UNKNOWN;

  m_cMark1 = ' ';
  m_cMark2 = ' ';

  m_pProperties = nullptr;

  m_pLastPrintParent = nullptr;

  md__SeqNo = Card::m__iSeqGen++;
  gapCard[md__SeqNo] = this;
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //vTrace("Card [~%02lld] %p\n"_Stz,md__SeqNo,this);
  gapCard[md__SeqNo] = nullptr;

  for (Card* pCard = pPop(); pCard; pCard = pPop())
    delete pCard;
  
  delete m_pProperties;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* check for leaks                                                            */
/*----------------------------------------------------------------------------*/
void Card::vCardLeaks(void)
{
  _static_

  for (i8 i_Card = 0; i_Card < C_10K; i_Card++) {
    if (gapCard[i_Card]) {
      Card *pLeaked = gapCard[i_Card];
      vTrace("Card Leak ~%02lld %p %s:%s\n"_Stz,
             i_Card, pLeaked, aCK[pLeaked->eCard()], pLeaked->sz());
      vDebugPoint();
    }
  }
}

/*----------------------------------------------------------------------------*/
/* check for leaks                                                            */
/*----------------------------------------------------------------------------*/
void Card::vConsistencyCheck(Card* p_deck, i8 iTransformNo)
{
  _static_

  for (i8 i_Card = 0; i_Card < m__iSeqGen; i_Card++) {
    if (! gapCard[i_Card])
      continue;

    Card* p_cardInQuestion = gapCard[i_Card];

    // is this the first time a transformation has seen this card?
    if (C_UNKNOWN == p_cardInQuestion->md__FirstDetected)
      p_cardInQuestion->md__FirstDetected = iTransformNo;

    // is the card the entire deck?
    if (p_cardInQuestion == p_deck)
      continue;

    // is the card in the deck?
    if (p_deck->pFindParent(p_cardInQuestion))
      continue;

    // **Leak Detected**

    // is this the first time this card has been detected?
    if (C_UNKNOWN != p_cardInQuestion->md__LeakDetected)
      continue;

    // report the leak
    p_cardInQuestion->md__LeakDetected = iTransformNo;
    vTrace("Card leak detected : %04lld %p %s:%s\n"_Stz,
         i_Card,
         p_cardInQuestion,
         aCK[p_cardInQuestion->eCard()],
         p_cardInQuestion->sz());
    vDebugPoint();
  }

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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  vAppend(pNewMainCard);

  if (bRenumber)
    vRenumberRecursive();
}

/*----------------------------------------------------------------------------*/
/* visit all sub-cards of given eToSubs and apply lambda function               */
/*----------------------------------------------------------------------------*/
void Card::vApplyRecursive(eCK eToSubs, std::function<bool(Card* pCard)> bfApply)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (eToSubs == m_eCard) {
    bool bContinue = bfApply(this);
    if (! bContinue)
      return;
  }

  CardIter xCI(this);
  while (Card* pChildCard = xCI.pEachChild()) {
    pChildCard->vApplyRecursive(eToSubs, bfApply);
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
/* delete child-card                                                          */
/*----------------------------------------------------------------------------*/
void Card::vDeleteChild(eCK eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i_Child;
  Card* pChildCard = pFindChild(eCard, i_Child);

  if (pChildCard) {
    vRemoveChild(pChildCard);
    delete pChildCard;
  }
}

/*----------------------------------------------------------------------------*/
/* delete given property                                                      */
/*----------------------------------------------------------------------------*/
void Card::vDeleteProperty(Stz* szPpyNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return;

  m_pProperties->vDelete(szPpyNm);
}

/*----------------------------------------------------------------------------*/
/* delete given property (recursively)                                        */
/*----------------------------------------------------------------------------*/
void Card::vDeletePropertyRecursive(Stz* szPpyNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vDeleteProperty(szPpyNm);

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pChildCard = pGetChild(i_Child);
    pChildCard->vDeletePropertyRecursive(szPpyNm);
  }
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
    vTrace("%s%s %s { %s }\n"_Stz
        , Stz::szTab(iDepth)
        , md_Bearing[0] ? md_Bearing : (char*)aCK[m_eCard]
        , szValue()
        , sProperties.szStz()
    );
  }
  else {
    vTrace("%s%s { %s }\n"_Stz
        , Stz::szTab(iDepth)
        , md_Bearing[0] ? md_Bearing : (char*)aCK[m_eCard]
        , sProperties.szStz()
    );
  }

  i8 nSubCards = this->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++)
    pGetChild(i_Sub)->vDump(iDepth + 1);
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
    if (bHasProperty(szPpy))
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
/* does card have given property ?                                            */
/*----------------------------------------------------------------------------*/
bool Card::bHasProperty(Stz* szPpyNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return false;

  bool bHas = m_pProperties->bHasName(szPpyNm);

  return bHas;
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
/* move given to child to be last child                                       */
/*----------------------------------------------------------------------------*/
void Card::vMoveToLastChild(Card* pChildCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vRemoveChild(pChildCard);
  vAppend(pChildCard);
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card::vPrintCardRecursive(Ribbon* pRibbon, i8 iDepth, Card* pParent, bool& rbPrune)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (CK_CMPPRED == m_eCard)
    vDebugPoint();

  if (0 == iDepth)
    vPrintHeader(pRibbon);

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

  if (! pCardCtl->bPrintSubCards(m_eCard))
    return;

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();
  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();
    pSubCard->vPrintCardRecursive(pRibbon, iDepth + 1, this, rbPrune);
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
void Card::vPrintHeader(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pRibbon->vSprint("==%02lld----------------"_Stz,m_iCardNo);
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

  i8 nProperties = this->nProperties();

  for (i8 i_Property = 0; i_Property < nProperties; i_Property++) {

    eTN eTkn = eProperty(i_Property);

    switch (eTkn) {

      case TN_INTEGER: {
        Stz* szName = Y_szNm(i_Property);
        i8   iValue = Y_iVal(i_Property);

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
        Object* pObject = Y_pVal(i_Property);
        pRibbon->vSprintRight("%s->  "_Stz,szName);
        pRibbon->vSprintRight("%p"_Stz,pObject);
        pRibbon->vSprintRight("%s"_Stz,aCN[pObject->eGetClassNo()]);
        break;
      }

      case TN_STRING: {
        Stz* szName  = Y_szNm(i_Property);
        Stz* szValue = Y_szVal(i_Property);
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
/* get number of properties                                                   */
/*----------------------------------------------------------------------------*/
i8 Card::nProperties(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return 0;
  
  i8 nProperties = m_pProperties->nItems();
  
  return nProperties;
}

/*----------------------------------------------------------------------------*/
/* get type of property                                                       */
/*----------------------------------------------------------------------------*/
eTN Card::eProperty(i8 i_Property)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return TN_NONE;
  
  eTN eToken = m_pProperties->eToken(i_Property);
  
  return eToken;
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
Stz* Card::szValue(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return m_sValue.szStz();
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

    if (CK_TERM == eCard_a)
      if (!this->sz()->bSame(pCard_b->sz()))
        return false;
  }

  i8 nChildren_a = this->nChildCards();
  i8 nChildren_b = pCard_b->nChildCards();
  if (nChildren_a != nChildren_b)
    return false;

  for (i8 i_Child = 0; i_Child < nChildren_a; i_Child++) {
    Card* pChild_a = this->pGetChild(i_Child);
    Card* pChild_b = pCard_b->pGetChild(i_Child);
    if (!pChild_a->bValuesSameRecursive(pChild_b, iDepth, bTestL0))
      return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* get given property name                                                    */
/*----------------------------------------------------------------------------*/
Stz* Card::Y_szNm(i8 i_Property)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return nullptr;

  Stz* szName = m_pProperties->szName(i_Property);

  return szName;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
char Card::Y_cVal(Stz* szPpyNm)
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
char Card::Y_cVal(i8 i_Property)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return ' ';

  Stz* szValue = m_pProperties->szValue(i_Property);
  if (! szValue)
    return ' ';

  return *szValue->azChar();
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
i8 Card::Y_iVal(Stz* szPpyNm)
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
i8 Card::Y_iVal(i8 i_Property)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return C_UNKNOWN;

  i8 iValue = m_pProperties->iValue(i_Property);

  return iValue;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
Object* Card::Y_pVal(Stz* szPptNm)
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
Object* Card::Y_pVal(i8 i_Property)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return nullptr;

  Object* pValue = m_pProperties->pValue(i_Property);

  return pValue;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
Stz* Card::Y_szVal(Stz* szPptNm)
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
Stz* Card::Y_szVal(i8 i_Property)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return nullptr;

  Stz* szValue = m_pProperties->szValue(i_Property);

  return szValue;
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::Y_vSet(Stz* szPpyNm, char cPpyVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;

  s32 sPptVal;
  sPptVal.vSprint("%c"_Stz, cPpyVal);
  m_pProperties->vReplace(szPpyNm, sPptVal);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::Y_vSet(Stz* szPpyNm, i8 iPpyVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vReplace(szPpyNm, iPpyVal);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::Y_vSet(Stz* szPpyNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_)

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vReplace(szPpyNm, "yes"_Stz);
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
void Card::Y_vSet(Stz* szPpyNm, Object* pPpyVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vReplace(szPpyNm, pPpyVal);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::Y_vSet(Stz* szPpyNm, Stz* szPpyVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vReplace(szPpyNm, szPpyVal);
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


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
#include "Card_CmpPred.hpp"
Card_CmpPred* Card::pSub_predCmp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CmpPred* p_predCmp =
      _pCard_CmpPred(pFindSubCard(CK_CMPPRED));
  return p_predCmp;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_ColumnRef.hpp"
Card_ColumnRef* Card::pSub_columnRef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_ColumnRef* p_columnRef =
      _pCard_ColumnRef(pFindSubCard(CK_COLUMNREF));
  return p_columnRef;
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
#include "Card_GbyElist.hpp"
Card_GbyElist* Card::pSub_gbyElist(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_GbyElist* p_gbyElist =
      _pCard_GbyElist(pFindSubCard(CK_GBYELIST));
  return p_gbyElist;
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
#include "Card_SbyElist.hpp"
Card_SbyElist* Card::pSub_sbyElist(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_SbyElist* p_sbyElist =
      _pCard_SbyElist(pFindSubCard(CK_SBYELIST));
  return p_sbyElist;
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
#include "Card_StmColOrd.hpp"
Card_StmColOrd* Card::pSub_stmColOrd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StmColOrd* p_streamColOrd =
      _pCard_StmColOrd(pFindSubCard(CK_STMCOLORD));
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
#include "Card_UBound.hpp"
Card_UBound* Card::pSub_uBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_UBound* p_uBound =
      _pCard_UBound(pFindSubCard(CK_UBOUND));
  return p_uBound;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* decode enum                                                                */
/*----------------------------------------------------------------------------*/
Stz* Card::szDecodeEnum(Stz* szEnum, i8 iEnum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  if (szEnum->bSame("eAggLevel"_Stz))   return aAL[iEnum];
  if (szEnum->bSame("eStageTopo"_Stz))  return aYG[iEnum];
  if (szEnum->bSame("eSendKind"_Stz))   return aXK[iEnum];

  return nullptr; //"**unknown**"_Stz;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eTerm_Bound of given index                      */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card::pAndPred_FindBound(Card_AndPred* p_andPred, Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_PRED* pSubPred0 = (Card_PRED*)pGet(0);
  if (Card_CmpPred* pFound = pSubPred0->pPRED_FindBound(pSubPred0, pIndex, eBound))
    return pFound;

  Card_PRED* pSubPred1 = (Card_PRED*)pGet(1);
  if (Card_CmpPred* pFound = pSubPred1->pPRED_FindBound(pSubPred1, pIndex, eBound))
    return pFound;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does this andPred have some bound on index?                                */
/*----------------------------------------------------------------------------*/
bool Card::bAndPred_HasBounds(Card_AndPred* p_andPred, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_PRED* pSubPred0 = (Card_PRED*)pGet(0);
  if (pSubPred0->bPRED_HasBounds(pSubPred0, pIndex))
    return true;

  Card_PRED* pSubPred1 = (Card_PRED*)pGet(1);
  if (pSubPred1->bPRED_HasBounds(pSubPred1, pIndex))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
bool Card::bBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred, char cWithMark)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_)

  eCK eCard_pred = p_pred->eCard();

  if (CK_ANDPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      vBoolCalc_RemovePred(p_boolCalc,p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_ORPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      vBoolCalc_RemovePred(p_boolCalc,p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_CMPPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      vBoolCalc_RemovePred(p_boolCalc,p_pred);
      delete p_pred;
      return true;
    }
    return false;
  }

  RASSERT (CK_ANDPRED == eCard_pred || CK_ORPRED == eCard_pred);

  i8 nChildren = p_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_PRED* pCard_pred_child = static_cast<Card_PRED*>(p_pred->pGetChild(i_Child));
    if (bBoolCalc_CullPred(p_boolCalc,pCard_pred_child, cWithMark))
      return true;
  }

  return  false;
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Card::vBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, char cWithMark)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_)

  if (! nChildCards())
    return;

  Card_PRED* pCard_pred = static_cast<Card_PRED*>(pGetChild(0));

  while (pCard_pred && bBoolCalc_CullPred(p_boolCalc, pCard_pred, cWithMark) && nChildCards()) {
    pCard_pred = static_cast<Card_PRED*>(pGetChild(0));
  }
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Card::vBoolCalc_RemovePred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_)

  Card* pParentCard = pFindParent(p_pred);

  pParentCard->vRemoveSubCard(p_pred);

  if (CK_ANDPRED == pParentCard->eCard() || CK_ORPRED == pParentCard->eCard()) {

    if (0 == pParentCard->nChildCards()) {
      vRemoveSubCard(pParentCard);
    }
    else {
      Card* pSiblingCard = pParentCard->pGetChild(0);
      Card* pGrandCard = pFindParent((pParentCard));
      pParentCard->vRemoveSubCard(pSiblingCard);
      pGrandCard->vReplaceChild(pParentCard, pSiblingCard);
    }

    delete pParentCard;
  }
}

/*----------------------------------------------------------------------------*/
/* get rank                                                                   */
/*----------------------------------------------------------------------------*/
i8 Card::iCandIndex_Rank(Card_CandIndex* p_candIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 iRank = Y_iVal(P_Rank);

  return iRank;
}

/*----------------------------------------------------------------------------*/
/* set rank                                                                   */
/*----------------------------------------------------------------------------*/
void Card::vCandIndex_Rank(Card_CandIndex* p_candIndex, i8 iRank)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Y_vSet(P_Rank, iRank);
}

/*----------------------------------------------------------------------------*/
/* is this predCmp co-partitionable with given arg?                           */
/*----------------------------------------------------------------------------*/
bool Card::bCmpPred_CoPtnWith(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_ColumnRef* p_columnRef_this_0 =
      _pCard_ColumnRef(this->pGetChild(0));
  Stz* szColumnRef_this_0 = p_columnRef_this_0->sz();
  char cTableSyn_this_0 = szColumnRef_this_0->cTableSyn();

  Card_ColumnRef* p_columnRef_this_1 =
      _pCard_ColumnRef(this->pGetChild(1));
  Stz* szColumnRef_this_1 = p_columnRef_this_1->sz();
  char cTableSyn_this_1 = szColumnRef_this_1->cTableSyn();


  Card_ColumnRef* p_columnRef_arg_0 =
      _pCard_ColumnRef(p_PredCmp_arg->pGetChild(0));
  Stz* szColumnRef_arg_0 = p_columnRef_arg_0->sz();
  char cTableSyn_arg_0 = szColumnRef_arg_0->cTableSyn();

  Card_ColumnRef* p_columnRef_arg_1 =
      _pCard_ColumnRef(p_PredCmp_arg->pGetChild(1));
  Stz* szColumnRef_arg_1 = p_columnRef_arg_1->sz();
  char cTableSyn_arg_1 = szColumnRef_arg_1->cTableSyn();

  bool bAreCoPtn = false;

  if (   cTableSyn_arg_0 == cTableSyn_this_0
         || cTableSyn_arg_0 == cTableSyn_this_1
         || cTableSyn_arg_1 == cTableSyn_this_0
         || cTableSyn_arg_1 == cTableSyn_this_1
      )
    bAreCoPtn = true;

  vTrace("(%c,%c) - (%c,%c) are%s co-partitioned\n"_Stz,
         cTableSyn_this_0,cTableSyn_this_1,
         cTableSyn_arg_0,cTableSyn_arg_1,
         bAreCoPtn ? "" : " NOT");

  return bAreCoPtn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card::bCmpPred_IsEq(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_ColumnRef* p_columnRef_this_0 = _pCard_ColumnRef(this->pGetChild(0));
  Stz* szColumnRef_this_0 = p_columnRef_this_0->sz();
  Card_ColumnRef* p_columnRef_this_1 = _pCard_ColumnRef(this->pGetChild(1));
  Stz* szColumnRef_this_1 = p_columnRef_this_1->sz();

  Card_ColumnRef* p_columnRef_arg_0 = _pCard_ColumnRef(p_PredCmp_arg->pGetChild(0));
  Stz* szColumnRef_arg_0 = p_columnRef_arg_0->sz();
  Card_ColumnRef* p_columnRef_arg_1 = _pCard_ColumnRef(p_PredCmp_arg->pGetChild(1));
  Stz* szColumnRef_arg_1 = p_columnRef_arg_1->sz();

  if (szColumnRef_this_0->bSame(szColumnRef_arg_0) &&
      szColumnRef_this_1->bSame(szColumnRef_arg_1))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card::bCmpPred_OverlapsWith(Card_CmpPred* p_cmpPred, Card_CmpPred* p_PredCmp_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  //this->vDump();
  //p_PredCmp_arg->vDump();

  Card_ColumnRef* p_columnRef_this_0 = _pCard_ColumnRef(this->pGetChild(0));
  char cTableSyn_this_0 = p_columnRef_this_0->cColumnRef_TableSyn(p_columnRef_this_0);
  Card_ColumnRef* p_columnRef_this_1 = _pCard_ColumnRef(this->pGetChild(1));
  char cTableSyn_this_1 = p_columnRef_this_1->cColumnRef_TableSyn(p_columnRef_this_1);

  Card_ColumnRef* p_columnRef_arg_0 = _pCard_ColumnRef(p_PredCmp_arg->pGetChild(0));
  char cTableSyn_arg_0 = p_columnRef_arg_0->cColumnRef_TableSyn(p_columnRef_arg_0);
  Card_ColumnRef* p_columnRef_arg_1 = _pCard_ColumnRef(p_PredCmp_arg->pGetChild(1));
  char cTableSyn_arg_1 = p_columnRef_arg_1->cColumnRef_TableSyn(p_columnRef_arg_1);

  if (   cTableSyn_this_0 == cTableSyn_arg_0
         || cTableSyn_this_0 == cTableSyn_arg_1
         || cTableSyn_this_1 == cTableSyn_arg_0
         || cTableSyn_this_1 == cTableSyn_arg_1
      )
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* mark the bounds of this <predCmp>                                          */
/*----------------------------------------------------------------------------*/
void Card::vCmpPred_MarkBound(Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szCmp = this->sz();

  char cBound = '?';

  if (szCmp->bSame("<"_Stz))  cBound = 'U';
  else if (szCmp->bSame("<="_Stz)) cBound = 'U';
  else if (szCmp->bSame("="_Stz))  cBound = 'E';
  else if (szCmp->bSame("=="_Stz)) cBound = 'E';
  else if (szCmp->bSame("!="_Stz)) cBound = 'N';
  else if (szCmp->bSame(">="_Stz)) cBound = 'L';
  else if (szCmp->bSame(">"_Stz))  cBound = 'L';

  vMark1(cBound);
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eTerm_Bound of given index                      */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card::pCmpPred_FindBound(Card_CmpPred* p_cmpPred, Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* pTerm2 = _pCard_Term(pGet(2));
  if (eBound != pTerm2->eTerm_Bound(pTerm2))
    return nullptr;

  Card_Term* pTerm0 = _pCard_Term(pGet(0));
  Stz* szTerm0 = pTerm0->sz();
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return nullptr;

  return _pCard_CmpPred(this);
}

/*----------------------------------------------------------------------------*/
/* is this comparePred a bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool Card::bCmpPred_HasBounds(Card_CmpPred* p_cmpPred, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //Card_Term* pTerm1 = _pCard_Term(pCard_PredCmp->pGet(1));

  Card_Term* pTerm2 = _pCard_Term(pGet(2));
  if (PB_NONE == pTerm2->eTerm_Bound(pTerm2))
    return false;

  Card_Term* pTerm0 = _pCard_Term(pGet(0));
  Stz* szTerm0 = pTerm0->sz();
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card::bCmpPred_UsesTable(Card_CmpPred* p_cmpPred, char cTableSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_ColumnRef* p_columnRef_this_0 = _pCard_ColumnRef(this->pGetChild(0));
  char cTableSyn_this_0 = p_columnRef_this_0->cColumnRef_TableSyn(p_columnRef_this_0);
  Card_ColumnRef* p_columnRef_this_1 = _pCard_ColumnRef(this->pGetChild(1));
  char cTableSyn_this_1 = p_columnRef_this_1->cColumnRef_TableSyn(p_columnRef_this_1);

  if (cTableSyn_this_0 == cTableSyn || cTableSyn_this_1 == cTableSyn)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is atom a column ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Card::bColumnRef_IsColumnRef(Card_ColumnRef* p_columnRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szColumnRef = this->sz();

  char cFirst  = szColumnRef->azChar()[0];
  char cSecond = szColumnRef->azChar()[1];
  char cThird  = szColumnRef->azChar()[2];
  char cFourth = szColumnRef->azChar()[3];

  if ('$' == cFirst && '$' == cSecond && '.' == cFourth)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get column ordinal of column ref                                           */
/*----------------------------------------------------------------------------*/
i8 Card::iColumnRef_ColumnOrd(Card_ColumnRef* p_columnRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szColumnRef = this->sz();

  char* azColumnOrd = (szColumnRef->azChar() + 3);

  i8 iColumnOrd = StrLib::iGleanInt(azColumnOrd);

  return iColumnOrd;
}

/*----------------------------------------------------------------------------*/
/* get table syn of column ref                                                */
/*----------------------------------------------------------------------------*/
char Card::cColumnRef_TableSyn(Card_ColumnRef* p_columnRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szColumnRef = this->sz();

  char cFirst  = szColumnRef->azChar()[0]; // $
  char cSecond = szColumnRef->azChar()[1]; // $
  char cThird  = szColumnRef->azChar()[2]; // c

  return cThird;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
char Card::cColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  Stz* szColumnRef = this->sz();

  char cFirst  = szColumnRef->azChar()[0]; // $
  char cSecond = szColumnRef->azChar()[1]; // $
  char cThird  = szColumnRef->azChar()[2]; // c

  return cThird;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef, String* psTableSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  psTableSyn->vSprint("%c"_Stz, cColumnRef_TableSyn_x(p_columnRef));
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card::bCoPtnJoin_CoPtnWith(Card_CoPtnJoin* p_coPtnJoin, Card_CoPtnPred* p_CoPtnPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CmpPred* p_predCmp_this = _pCard_CmpPred(this);
  Card_CmpPred* p_predCmp_arg  = _pCard_CmpPred(p_CoPtnPred);

  if (p_predCmp_this->bCmpPred_CoPtnWith(p_predCmp_this, p_predCmp_arg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_CoPtnJoin* Card::pCoPtnJoinList_Find_coPtn_for(Card_CoPtnJoinList* p_coPtnJoinList, Card_CoPtnPred* p_coPtnPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_CoPtnJoin* p_coPtnJoin = _pCard_CoPtnJoin(pGetChild(i_Child));

    if (p_coPtnJoin->bCoPtnJoin_CoPtnWith(p_coPtnJoin, p_coPtnPred))
      return p_coPtnJoin;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card::bCoPtnJoinList_HasEqPredCmp(Card_CoPtnJoinList* p_coPtnJoinList, Card_CmpPred* p_predCmp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nChildren_coPtn = this->nChildCards();
  for (i8 i_Child_coPtn = 0; i_Child_coPtn < nChildren_coPtn; i_Child_coPtn++) {
    Card_CoPtnJoin* p_coPtnJoin = _pCard_CoPtnJoin(this->pGetChild(i_Child_coPtn));

    i8 nChildren_predCmp = p_coPtnJoin->nChildCards();
    for (i8 i_Child_predCmp = 0; i_Child_predCmp < nChildren_predCmp; i_Child_predCmp++) {
      Card_CmpPred *p_predCmp = _pCard_CmpPred(p_coPtnJoin->pGetChild(i_Child_predCmp));


      if (p_predCmp->bCmpPred_IsEq(p_predCmp, p_predCmp))
        return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_StmColList* Card::pELIST_DeriveStreamColList(Card_ELIST* p_ELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StmColList* pCard_StreamColList = new Card_StmColList;

  eCK eSubCard;
  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pGetChild(i_Child, eSubCard);

    Card *pCard_Type = pSubCard->pFindSubCard(CK_TYPE);
    Card *pCard_As = pSubCard->pFindSubCard(CK_AS);

    Card_StmCol* pCard_StreamCol = new Card_StmCol;
    pCard_StreamColList->vAppend(pCard_StreamCol);

    if (pCard_As)
      pCard_StreamCol->vAppend(pCard_As->pDupCardAndSubCards());
    else {
      s32 sAs;
      sAs.vSprint("$%lld"_Stz,i_Child + 1);
      pCard_As = new Card_As(sAs);
      pCard_StreamCol->vAppend(pCard_As);
    }
    pCard_StreamCol->vAppend(pCard_Type->pDupCardAndSubCards());
  }

  return pCard_StreamColList;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vExpr_AsEnsure(Card_Expr* p_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_As* pCard_As = _pCard_As(pFindSubCard(CK_AS));
  if (pCard_As)
    return;

  s32 sAs;
  sAs.vSprint("COL_%lld"_Stz,m_iCardNo);

  pCard_As = new Card_As(sAs);
  vAppend(pCard_As);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vFilter_CullPred(Card_Filter* p_filter, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = pGetChild(i_Child);

    eCK eCard = pChildCard->eCard();
    if (eCard != CK_BOOLCALC)
      continue;

    Card_BoolCalc* p_boolCalc = _pCard_BoolCalc(pChildCard);

    p_boolCalc->vBoolCalc_CullPred(p_boolCalc,cWithMark);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card::bFilter_IsEmptyFilter(Card_Filter* p_filter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_BoolCalc* pCard_calcBool =
      _pCard_BoolCalc(pFindSubCard(CK_BOOLCALC));

  if (0 == pCard_calcBool->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* add table and index                                                        */
/*----------------------------------------------------------------------------*/
void Card::vIdxColList_Ordinate(Card_IdxColList* p_idxColList, Card_Stream* pCard_Stream)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 iOrdLo = pCard_Stream->Y_iVal(P_OrdLo);

  Card_StmColList* pCard_StreamColList = pCard_Stream->pSub_stmColList();

  i8 nSubCards = this->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card_IdxCol* pCard_idxCol = _pCard_IdxCol(pGetChild(i_Sub));

    i8 i_StreamColNo = C_UNKNOWN;
    Stz* szColNm = pCard_idxCol->sz();
    Card_StmCol* pCard_streamCol =
        pCard_StreamColList->pStmColList_Find_streamCol(pCard_StreamColList, szColNm, i_StreamColNo);

    s32 sOrdinal;
    sOrdinal.vSprint("$%lld"_Stz,iOrdLo+i_StreamColNo);
    pCard_idxCol->vValue(sOrdinal);

    pCard_idxCol->vMark1(MARK_CHANGED);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vJoinOrderSeq_ExtractJoinOrder(Card_PtndJoin* p_PtndJoin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eChild;

  for (i8 i_Child = 0; i_Child < p_PtndJoin->nChildCards(); i_Child++) {
    Card_CmpPred* p_predCmp = _pCard_CmpPred(p_PtndJoin->pGetChild(i_Child));

    for (i8 i_Kid = p_predCmp->nChildCards() - 1; i_Kid >= 0; i_Kid--) {
      Card *pChild = p_predCmp->pGetChild(i_Kid, eChild);
      if (CK_COLUMNREF != eChild)
        continue;

      Card_ColumnRef* p_columnRef = _pCard_ColumnRef(pChild);
      s32 sTableSyn;
      p_columnRef->vColumnRef_TableSyn_x(p_columnRef,sTableSyn);

      if (bHasChildWithValue(sTableSyn))
        continue;

      Card_JoinOrder* p_joinOrder = new Card_JoinOrder(sTableSyn);
      vAppend(p_joinOrder);
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vJoinPredList_Extract(Card_JoinPredList* p_joinPredList, Card_BoolCalc* p_boolCalc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  auto bfExtract = [this,p_boolCalc](Card *pCard) {

    Card_CmpPred* p_predCmp = _pCard_CmpPred(pCard);

    if (! p_predCmp->sz()->bSame("=="_Stz)) //RAYMOND CAN DO BETTER!
      return true;

    Card_Term* p_term_0 = _pCard_Term(p_predCmp->pGetChild(0));
    Card_ColumnRef* p_columnRef_0 = p_term_0->pSub_columnRef();
    if (! p_columnRef_0)
      return true;
    Card_Term* p_term_1 = _pCard_Term(p_predCmp->pGetChild(1));
    Card_ColumnRef* p_columnRef_1= p_term_1->pSub_columnRef();
    if (!p_columnRef_1->bColumnRef_IsColumnRef(p_columnRef_1))
      return true;

    Card_CmpPred* p_predCmp_new = new Card_CmpPred(p_predCmp->sz());
    p_predCmp_new->vAppend(p_columnRef_0->pDupCardAndSubCards());
    p_predCmp_new->vAppend(p_columnRef_1->pDupCardAndSubCards());

    this->vAppend(p_predCmp_new);

    return true;
  };

  // - - - - - - - - -

  p_boolCalc->vApplyRecursive(CK_CMPPRED, bfExtract);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vObyElist_Populate(Card_ObyElist* p_obyElist, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  IndexDef* pIndexDef = pIndex->pIndexDef();
  i8 nIdxCols = pIndexDef->nColumns();

  for (i8 i_IdxCol = 0; i_IdxCol < nIdxCols; i_IdxCol++) {

    Column* pIdxCol = pIndexDef->pGetColumn(i_IdxCol);

    Card_Expr* pCard_Expr = new Card_Expr;
    vAppend(pCard_Expr);


    Stz* szType = aCT[pIdxCol->eColumnType()];
    Card_Type* pCard_Type = new Card_Type(szType);
    pCard_Expr->vAppend(pCard_Type);

    // as
    Card_As* pCard_As = new Card_As(pIdxCol->szColNm());
    pCard_Expr->vAppend(pCard_As);

    // exprCalc
    Card_RpnCalc* pCard_RpnCalc = new Card_RpnCalc;
    pCard_Expr->vAppend(pCard_RpnCalc);

    Card_Term* pCard_Term = new Card_Term(pIdxCol->szColNm());
    pCard_RpnCalc->vAppend(pCard_Term);

  }
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eTerm_Bound of given index                      */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card::pOrPred_FindBound(Card_OrPred* p_orPred, Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_PRED* pSubPred0 = (Card_PRED*)pGet(0);
  if (Card_CmpPred* pFound = pSubPred0->pPRED_FindBound(pSubPred0, pIndex, eBound))
    return pFound;

  Card_PRED* pSubPred1 = (Card_PRED*)pGet(1);
  if (Card_CmpPred* pFound = pSubPred1->pPRED_FindBound(pSubPred1, pIndex, eBound))
    return pFound;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does orPred have some bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool Card::bOrPred_HasBounds(Card_OrPred* p_orPred, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_PRED* pSubPred0 = (Card_PRED*)pGet(0);
  if (pSubPred0->bPRED_HasBounds(pSubPred0,pIndex))
    return true;

  Card_PRED* pSubPred1 = (Card_PRED*)pGet(1);
  if (pSubPred1->bPRED_HasBounds(pSubPred1,pIndex))
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/* concat terms                                                               */
/*----------------------------------------------------------------------------*/
void Card::vPRED_ConcatTerms(Card_PRED* p_PRED, Stz* szTerms, i8 i_From, i8 i_Till)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (C_UNKNOWN == i_Till)
    i_Till = nChildCards() - 1;

  for (i8 i_Child = i_From; i_Child <= i_Till; i_Child++) {
    Card* pChildCard = pGetChild(i_Child);
    if (CK_TERM != pChildCard->eCard())
      continue;

    szTerms->vCatSprint(" %s"_Stz, pChildCard->sz());
  }
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vPRED_CullByKids(Card_PRED* p_PRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildCards = nItems();
  for (i8 i_ChildCard = 0; i_ChildCard < nChildCards; i_ChildCard++) {
    Card* pCard_Child = pGet(i_ChildCard);
    if (!bPRED_IsPred(pCard_Child))
      continue;

    Card_PRED* pCard_ChildPRED = (Card_PRED*)pCard_Child;

    pCard_ChildPRED->vPRED_CullByKids(pCard_ChildPRED);

    if (0 == pCard_ChildPRED->nItems()) {

      vRemove(pCard_ChildPRED);
      delete pCard_ChildPRED;

      i_ChildCard--;
      nChildCards--;

      continue;
    }

    if (1 == pCard_ChildPRED->nItems()) {

      Card_PRED* pCard_KidPRED = (Card_PRED*)pCard_Child->pPop();

      vReplace(pCard_ChildPRED,pCard_KidPRED);
      delete pCard_ChildPRED;

      continue;
    }


  }
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eTerm_Bound of given index                      */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card::pPRED_FindBound(Card_PRED* p_PRED, Index* pIndex,
                                    ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eCard = this->eCard();

  switch (eCard) {

    case CK_ORPRED:
      return _pCard_OrPred(this)->pOrPred_FindBound(_pCard_OrPred(this), pIndex, eBound);

    case CK_ANDPRED:
      return _pCard_AndPred(this)->pAndPred_FindBound(_pCard_AndPred(this), pIndex, eBound);

    case CK_CMPPRED:
      return _pCard_CmpPred(this)->pCmpPred_FindBound(_pCard_CmpPred(this), pIndex, eBound);

    default:
      vTrace("pPRED_FindBound: unexpected eCard %lld %s\n"_Stz
          ,(i8)eCard
          ,aCK[eCard]);
      RASSERT(0);
      break;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does predicate have some bound wrt given index?                            */
/*----------------------------------------------------------------------------*/
bool Card::bPRED_HasBounds(Card_PRED* p_PRED, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eCard = this->eCard();

  switch (eCard) {

    case CK_ORPRED:
      return _pCard_OrPred(this)->bOrPred_HasBounds(_pCard_OrPred(this),pIndex);

    case CK_ANDPRED:
      return _pCard_AndPred(this)->bAndPred_HasBounds(_pCard_AndPred(this),pIndex);

    case CK_CMPPRED:
      return _pCard_CmpPred(this)->bCmpPred_HasBounds(_pCard_CmpPred(this),pIndex);

    default:
      vTrace("bPRED_HasBounds: unexpected eCard %lld %s\n"_Stz
          ,(i8)eCard
          ,aCK[eCard]);
      RASSERT(0);
      break;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this card the given kind?                                               */
/*----------------------------------------------------------------------------*/
bool Card::bPRED_IsPred(Card* pArgCard)
{
  _static_;

  eCK eCard = pArgCard->eCard();

  switch (eCard) {
    case CK_EXPRBOOL:
    case CK_ORPRED:
    case CK_ANDPRED:
      return true;

    default: return false;
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card::bPtndJoin_MayFollowAfter(Card_PtndJoin* p_ptndJoin, Card_SubSet* pPriorPtnJoins_sofar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nPtnJoins_sofar = pPriorPtnJoins_sofar->nChildCards();
  if (! nPtnJoins_sofar)
    return true;

  for (i8 i_PtnJoin_sofar = 0; i_PtnJoin_sofar < nPtnJoins_sofar; i_PtnJoin_sofar++) {
    Card_PtndJoin* p_ptndJoin_sofar =
        _pCard_PtndJoin(pPriorPtnJoins_sofar->pGetChild(i_PtnJoin_sofar));

    i8 nPredCmps_sofar = p_ptndJoin_sofar->nChildCards();
    for (i8 i_PredCmp_sofar = 0; i_PredCmp_sofar < nPredCmps_sofar; i_PredCmp_sofar++) {
      Card_CmpPred* p_predCmp_sofar =
          _pCard_CmpPred(p_ptndJoin_sofar->pGetChild(i_PredCmp_sofar));

      i8 nPredCmps_this = this->nChildCards();
      for (i8 i_PredCmp_this = 0; i_PredCmp_this < nPredCmps_this; i_PredCmp_this++) {
        Card_CmpPred *p_predCmp_this =
            _pCard_CmpPred(this->pGetChild(i_PredCmp_this));

        if (p_predCmp_this->bCmpPred_OverlapsWith(p_predCmp_this,p_predCmp_sofar))
          return true;
      }

    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vPtndJoin_JoinEst(Card_PtndJoin* p_ptndJoin, Card_TableList* p_tableList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nJoinedBuff = 0;

  i8 nTables = p_tableList->nChildCards();
  for (i8 i_Table = 0; i_Table < nTables; i_Table++) {
    Card_Table *p_table =
        _pCard_Table(p_tableList->pGetChild(i_Table));

    if (p_table->bHasProperty(P_TableUsed))
      continue;

    i8 nPredCmps = this->nChildCards();
    for (i8 i_PredCmp = 0; i_PredCmp < nPredCmps; i_PredCmp++) {
      Card_CmpPred *p_predCmp =
          _pCard_CmpPred(this->pGetChild(i_PredCmp));

      if (p_predCmp->bCmpPred_UsesTable(p_predCmp,p_table->cValue())) {
        nJoinedBuff += p_table->pSub_size()->szValue()->iToI8();
        p_table->Y_vSet(P_TableUsed);
      }
    }

  }

  Y_vSet(P_JoinedBuff, nJoinedBuff);
}

/*----------------------------------------------------------------------------*/
/* get head term                                                              */
/*----------------------------------------------------------------------------*/
Card_Term* Card::pRpnTerms_HeadTerm(Card_RpnTerms* p_rpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* pCard_Term = _pCard_Term(pHead());

  return pCard_Term;
}

/*----------------------------------------------------------------------------*/
/* pop subcard                                                                */
/*----------------------------------------------------------------------------*/
Card_Term* Card::pRpnTerms_PopTerm(Card_RpnTerms* p_rpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* pCard_Term = _pCard_Term(pPop());

  return pCard_Term;
}

/*----------------------------------------------------------------------------*/
/* get number of stream columns                                               */
/*----------------------------------------------------------------------------*/
void Card::vStream_PrintStream(Card_Stream* p_stream, Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  s32 sName;
  sName.vSprint("%s%s"_Stz,szCardNm(),nProperties()?" +":"");

  if (bHasProperty(P_OrdLo))
    sName.vCatSprint(" $%lld,%lld"_Stz,
                     Y_iVal(P_OrdLo), Y_iVal(P_OrdHi));

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
/* evaluate bound                                                             */
/*----------------------------------------------------------------------------*/
ePB Card::eTerm_Bound(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eRO eRelOp = this->eTerm_RelOp(p_term);

  switch (eRelOp) {
    case RO_LESS_THAN:     return PB_UBOUND;
    case RO_LESS_EQUAL:    return PB_UBOUND;
    case RO_EQUAL_TO:      return PB_EBOUND;
    case RO_GREATER_EQUAL: return PB_LBOUND;
    case RO_GREATER_THAN:  return PB_LBOUND;

    default:
      vTrace("%s(%lld): unexpected eTerm_RelOp %lld %s\n"_Stz
          ,__func__
          ,(i8)__LINE__
          ,(i8)eRelOp
          ,aRO[eRelOp]);
      RASSERT(0);
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* get ordinal index value  (i.e. $In)                                        */
/*----------------------------------------------------------------------------*/
i8 Card::iTerm_IndexColNo(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bTerm_IsIndexColOrdinal(p_term));

  i8 i_Ord = StrLib::iGleanInt(szValue()->azChar());

  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* is this atom an aggregate op?                                              */
/*----------------------------------------------------------------------------*/
bool Card::bTerm_IsAggOp(Card_Term* p_term)
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
/* is this term a comma separator?                                            */
/*----------------------------------------------------------------------------*/
bool Card::bTerm_IsCommaSeparator(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (szValue()->bSame(","_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is term an index-column ordinal?  (i.e. $In)                               */
/*----------------------------------------------------------------------------*/
bool Card::bTerm_IsIndexColOrdinal(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = szValue();

  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];
  char acThird  = szTerm->azChar()[2];

  if ('$' == acFirst && ('I' == acSecond || 'i' == acSecond)
      && StrLib::bIsDigit(acThird))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is term a parameter ordinal?  (i.e. $Pn)                                   */
/*----------------------------------------------------------------------------*/
bool Card::bTerm_IsParamOrdinal(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = szValue();

  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];
  char acThird  = szTerm->azChar()[2];

  if ('$' == acFirst && ('P' == acSecond || 'p' == acSecond)
      && StrLib::bIsDigit(acThird))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is term a stream-column ordinal?  (i.e. $n)                                */
/*----------------------------------------------------------------------------*/
bool Card::bTerm_IsStreamColOrdinal(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = szValue();

  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];

  if ('$' == acFirst && StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is term an table-column ordinal?  (i.e. $$c.n)                             */
/*----------------------------------------------------------------------------*/
bool Card::bTerm_IsTableColOrdinal(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = szValue();

  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];
  char acThird  = szTerm->azChar()[2];
  char acFourth = szTerm->azChar()[3];
  char acFifth  = szTerm->azChar()[4];

  if ('$' == acFirst && '$' == acSecond
      && StrLib::bIsAlpha(acThird)
      && '.' == acFourth
      && StrLib::bIsDigit(acFifth))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO Card::eTerm_RelOp(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = this->sz();

  if (szTerm->bSame("<"_Stz))   return RO_LESS_THAN;
  else if (szTerm->bSame("<="_Stz))  return RO_LESS_EQUAL;
  else if (szTerm->bSame("="_Stz))   return RO_EQUAL_TO;
  else if (szTerm->bSame("=="_Stz))  return RO_EQUAL_TO;
  else if (szTerm->bSame("!="_Stz))  return RO_NOT_EQUAL;
  else if (szTerm->bSame(">="_Stz))  return RO_GREATER_EQUAL;
  else if (szTerm->bSame(">"_Stz))   return RO_GREATER_THAN;

  else return RO_NONE;
}

/*----------------------------------------------------------------------------*/
/* get stream column number n  (i.e. $n)                                      */
/*----------------------------------------------------------------------------*/
i8 Card::iTerm_StreamColNo(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bTerm_IsStreamColOrdinal(p_term));

  i8 i_Col = StrLib::iGleanInt(szValue()->azChar());

  return i_Col;
}

/*----------------------------------------------------------------------------*/
/* get table column number n (i.e. $$c.n)                                     */
/*----------------------------------------------------------------------------*/
i8 Card::iTerm_TableColNo(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bTerm_IsTableColOrdinal(p_term));

  i8 i_Col = StrLib::iGleanInt(szValue()->azChar());

  return i_Col;
}

/*----------------------------------------------------------------------------*/
/* get table synonym c (i.e. $$c.n)                                           */
/*----------------------------------------------------------------------------*/
char Card::cTerm_TableSyn(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bTerm_IsTableColOrdinal(p_term));

  Stz* szTerm = this->sz();

  char cSyn = szTerm->azChar()[2];

  return cSyn;
}

/*----------------------------------------------------------------------------*/
/* evaluate term                                                              */
/*----------------------------------------------------------------------------*/
eTE Card::eTerm_TermEval(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = this->sz();

  if (szTerm->bSame("and"_Stz))     return TE_AND;
  else if (szTerm->bSame("exists"_Stz))  return TE_EXISTS;
  else if (szTerm->bSame("or"_Stz))      return TE_OR;

  else if (szTerm->bSame("<"_Stz))       return TE_COMPARE;
  else if (szTerm->bSame("<="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame("="_Stz))       return TE_COMPARE;
  else if (szTerm->bSame("=="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame("!="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame("<>"_Stz))      return TE_COMPARE;
  else if (szTerm->bSame(">="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame(">"_Stz))       return TE_COMPARE;

  else return TE_OTHER;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
