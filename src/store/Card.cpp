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

  if (Y_bHas(P_Focus))
    pRibbon->vSprint("==%02lld %c\\/\\/\\/\\/\\/\\/\\/"_Stz,m_iCardNo,cLevel);
  else
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
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_AggElist.hpp"
Card_AggElist* Card::pSub_aggElist(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_AggElist* p_aggElist =
      _pCard_AggElist(pFindSubCard(CK_AGGELIST));
  return p_aggElist;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_AggGby.hpp"
Card_AggGby* Card::pSub_aggGby(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_AggGby* p_aggGby =
      _pCard_AggGby(pFindSubCard(CK_AGGGBY));
  return p_aggGby;
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
#include "Card_MaybeIndexJoin.hpp"
Card_MaybeIndexJoin* Card::pSub_maybeIndexJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_MaybeIndexJoin* p_maybeIndexJoin =
      _pCard_MaybeIndexJoin(pFindSubCard(CK_MAYBEINDEXJOIN));
  return p_maybeIndexJoin;
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
#include "Card_ProjElist.hpp"
Card_ProjElist* Card::pSub_projElist(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_ProjElist* p_projElist =
      _pCard_ProjElist(pFindSubCard(CK_PROJELIST));
  return p_projElist;
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
#include "Card_TstPred.hpp"
Card_TstPred* Card::pSub_tstPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_TstPred* p_tstPred =
      _pCard_TstPred(pFindSubCard(CK_TSTPRED));
  return p_tstPred;
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
