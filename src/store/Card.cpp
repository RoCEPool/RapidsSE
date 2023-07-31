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
Card::Card(eCK eCard, Symbol* pSymbol)
    : Object(CN_CARD)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vInit(eCard);

  m_xSymbol.vSet(pSymbol);

  m_xSymbol.Y_vAll(pSymbol);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card::Card(eCK eCard, Stz* szSymbol, eSF eSymbol)
    : Object(CN_CARD)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vInit(eCard);

  vSymbol(szSymbol,eSymbol);
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void Card::vInit(eCK eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (CK_CROSSJOIN == eCard)
    vDebugPoint();

  if (CK_BOUNDS == eCard)
    vDebugPoint();

  MemLib::vZero(md_Bearing, sizeof(md_Bearing), CN_CARD);

  if (CK_BOUNDS == eCard)
    vDebugPoint();

  m_eCard   = eCard;
  m_i_Child = C_UNKNOWN;

  //m_xSymbol.m_sSymbol = ""_Stz;
  m_cMark1 = MARK_BLANK;
  m_cMark2 = MARK_BLANK;

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
//         md__SeqNo,this,aCK[m_eCard],m_xSymbol.m_sSymbol.szStz(),md_Bearing);
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

//  for (i8 i_Card = 0; i_Card < C_10K; i_Card++) {
//    if (gapCard[i_Card]) {
//      Card *pLeaked = gapCard[i_Card];
//      vTrace("Card Leak ~%02lld %p %s:%s\n"_Stz,
//             i_Card, pLeaked, aCK[pLeaked->eCard()], pLeaked->szSF__print());
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
//    if (p_deck->pFindParentRecursive(p_cardInQuestion))
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
//         p_cardInQuestion->szSF__print());
//    vDebugPoint();
//  }
//
}

/*----------------------------------------------------------------------------*/
/* convert eAL to eYG                                                         */
/*----------------------------------------------------------------------------*/
eYG Card::e_aggLevel_To_stageTopo(eAL eLevel)
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
/* convert eYG to eAL                                                         */
/*----------------------------------------------------------------------------*/
eAL Card::e_stageTopo_To_aggLevel(eYG eStageTopo)
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
/* append new card to tail                                                    */
/*----------------------------------------------------------------------------*/
Card* Card::pAppendNewCard(eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card* pCard = new Card(eCard);

  vPushTail(pCard);

  return pCard;
}

/*----------------------------------------------------------------------------*/
/* visit all sub-cards of given eToSubs and apply lambda function             */
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
  while (xCI.G_bEachMain()) {
    bool bContinue = xCI.G.p_MainCard->bApplyRecursive(eToSubs, bfLambdaFn);
    if (! bContinue)
      return C_TERMINATE_RECURSION;
  }

  return C_CONTINUE_RECURSION;
}

/*----------------------------------------------------------------------------*/
/* visit all term sub-cards and apply lambda function                         */
/*----------------------------------------------------------------------------*/
bool Card::bApplyRecursive_term(std::function<bool(Card_Term* p_term)> bfLambdaFn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (bIsCard(CK_TERM)) {
    bool bContinue = bfLambdaFn(_pCard_Term(this));
    if (! bContinue)
      return false;
  }

  CardIter xCI(this);
  while (xCI.G_bEachMain()) {
    if (! xCI.G.p_MainCard->bApplyRecursive_term(bfLambdaFn))
      return false;
  }

  return true;
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

//  if (m_xSymbol.m_pProperties) {
//    s4K sProperties;
//    m_xSymbol.m_pProperties->vToString(sProperties, true);
//    sPretty.vConcat(": "_Stz);
//    sPretty.vConcat(sProperties);
//  }

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
    sprintf(md_Bearing, "%s.%lld", pParentCard->md_Bearing, m_i_Child);
  }
  else {
    sprintf(md_Bearing, "%lld", m_i_Child);
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
/* get card no                                                                */
/*----------------------------------------------------------------------------*/
i8 Card::i_Child(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_i_Child;
}

/*----------------------------------------------------------------------------*/
/* set card no                                                                */
/*----------------------------------------------------------------------------*/
void Card::v_Child(i8 i_Child)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_i_Child = i_Child;
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
/* set type of symbol                                                         */
/*----------------------------------------------------------------------------*/
void Card::vColType(Card* pCard_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  m_xSymbol.Y_vSet(P_Type, pCard_arg->pSymbol()->szType());
}

/*----------------------------------------------------------------------------*/
/* dump card and its sub cards                                                */
/*----------------------------------------------------------------------------*/
void Card::vDump(const char* azFunc, i8 iDepth, Stz* szTitle)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (0 == iDepth) {
    if (szTitle)
      vTrace("^^%s [%s]\n"_Stz,szTitle,azFunc);
    else
      vTrace("^^Card Dump [%s]\n"_Stz,azFunc);
  }

  s4K sProperties;
  m_xSymbol.vPpyStr(sProperties);

  vTrace("  %p %s%s : %s {%s} %s\n"_Stz
        , this
        , Stz::szTab(iDepth)
        , md_Bearing[0] ? md_Bearing : (char*)aCK[m_eCard]
        , *szSymbol()->azChar() ? szSymbol() : "??Symbol"_Stz
        , sProperties.szStz()
        , pSymbol()->szWhatIs()
  );

  i8 nSubCards = this->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++)
    pGetChild(i_Sub)->vDump(azFunc, iDepth + 1);

  if (0 == iDepth)
    vTrace("$$\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* duplicate this card, along with its symbolic value and properties          */
/*----------------------------------------------------------------------------*/
Card* Card::pDupCard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card* pCard_dup = new Card(m_eCard);
  pCard_dup->pSymbol()->vSet(pSymbol());

  return pCard_dup;
}

/*----------------------------------------------------------------------------*/
/* duplicate this card and its sub cards                                      */
/*----------------------------------------------------------------------------*/
Card* Card::pDupCardRecursive(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card* pCard_dup = new Card(m_eCard);
  pCard_dup->pSymbol()->vSet(pSymbol());

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pSubCard = pGetChild(i_Child);
    
    Card* pSubCard_dup = pSubCard->pDupCardRecursive();
    pCard_dup->vAppend(pSubCard_dup);
  }
  
  return pCard_dup;
}

/*----------------------------------------------------------------------------*/
/* duplicate the sub cards                                                    */
/*----------------------------------------------------------------------------*/
Card_SubSet* Card::pDupSubCardsRecursive(i8 i_From, i8 i_Till)
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

    Card* pChild_dup = pChild->pDupCardRecursive();
    pCard_SubSet->vAppend(pChild_dup);
  }

  return pCard_SubSet;
}

/*----------------------------------------------------------------------------*/
/* find parent of given sub-card                                              */
/*----------------------------------------------------------------------------*/
Card* Card::pFindParentRecursive(Card* pSubCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nSubCards = nItems();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card* pTestCard = pGetChild(i_Sub);

    if (pSubCard == pTestCard)
      return this;

    Card* pParent = pTestCard->pFindParentRecursive(pSubCard);
    if (pParent)
      return pParent;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* attempt find card                                                          */
/*----------------------------------------------------------------------------*/
Card* Card::pFindCardRecursive(eCK eOfCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bIsCard(eOfCard))
    return this;

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    if (Card *pFoundCard = pGetChild(i_Child)->pFindCardRecursive(eOfCard))
      return pFoundCard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* attempt find of symbolic value                                             */
/*----------------------------------------------------------------------------*/
Card* Card::pFindSymbolRecursive(eCK eOfCard, Symbol* pSymbol, Card*& rpParent, Card* _pWalkParent_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //vTrace("%s `%s`?`%s` %s\n"_Stz,__func__,szSymbol,szSF__print(),md_Bearing);

  if (eOfCard == m_eCard && bSameSymbol(pSymbol)) {
    rpParent = _pWalkParent_;
    return this;
  }

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    if (Card *pFoundCard = pGetChild(i_Child)->pFindSymbolRecursive(eOfCard, pSymbol, rpParent, this))
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
    if (pSymbol()->Y_bHas(szPpy))
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
/* is this card the given kind?                                               */
/*----------------------------------------------------------------------------*/
bool Card::bIsOne(eCK eCard, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  va_list xArgs;

  va_start(xArgs, eCard);
  eCK eCard_mayBe = eCard;
  while (CK_NONE != eCard_mayBe) {

    if (this->bIsCard(eCard_mayBe))
      return true;

    eCard_mayBe = (eCK) va_arg(xArgs, i8);
  }

  va_end(xArgs);

  return false;
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
/* get mark 2                                                                 */
/*----------------------------------------------------------------------------*/
char Card::cMark2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_cMark2;
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
/* clear all marks                                                            */
/*----------------------------------------------------------------------------*/
void Card::vMarksClearRecursive(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vMark1(MARK_BLANK);
  vMark2(MARK_BLANK);

  i8 nSubCards = this->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card *pSubCard = pGetChild(i_Sub);

    pSubCard->vMarksClearRecursive();
  }

}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card::vPrintCardRecursive(Ribbon* pRibbon, i8 iDepth, Card* pParent,
                               bool& rbPrune, eAL eLevel,
                               bool bPrintBelow, i8 iXform, Stz* szXfunc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (CK_RPNCALC == m_eCard)
    vDebugPoint();

  if (0 == iDepth) {
    if (   bIsCard(CK_AGGREG)
        || bIsCard(CK_PROJECT)) {
      vDebugPoint();
    }

    if (pRibbon->pCardCtl()->pCardHistory())
      pRibbon->pCardCtl()->pCardHistory()->vAddImage(this,iXform,szXfunc);
    vPrintHeader(pRibbon,eLevel);
  }

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCardCtl());

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
    pSubCard->vPrintCardRecursive(pRibbon,
                                  iDepth + 1,
                                  this,
                                  rbPrune,
                                  eLevel,
                                  bPrintBelow,
                                  iXform,
                                  szXfunc);
  }

  delete pSubCard_I;
}

/*----------------------------------------------------------------------------*/
/* print card onto given ribbon                                               */
/*----------------------------------------------------------------------------*/
void Card::vPrintCard_byCustomNm(Ribbon* pRibbon, i8 iDepth, Stz* szCustomNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCardCtl());

  if (pCardCtl->bPrintName(m_eCard))
    vPrintName(pRibbon, iDepth, szCustomNm);
  if (pCardCtl->bPrintValue(m_eCard))
    vPrintSymbol(pRibbon, iDepth);

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

    case CK_KEY:
      _pCard_Key(this)->vPrint(pRibbon, iDepth);
      break;

    case CK_PROJECTION:
      _pCard_Projection(this)->vPrint(pRibbon, iDepth);
      break;

    case CK_TABLE:
      _pCard_Table(this)->vPrint(pRibbon, iDepth);
      break;

    case CK_TERM:
      _pCard_Term(this)->vPrint(pRibbon, iDepth);
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

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCardCtl());

  if (pCardCtl->bPrintName(m_eCard) && pCardCtl->bPrintValue(m_eCard)) {
    vPrintNameSymbol(pRibbon, iDepth);
  }
  else {
    bool bPrintName = pCardCtl->bPrintName(m_eCard);
    if (bPrintName)
      vPrintName(pRibbon, iDepth);
    if (pCardCtl->bPrintValue(m_eCard))
      vPrintSymbol(pRibbon, iDepth, !bPrintName);
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
      pRibbon->vSprintDataRight("%s"_Stz, pSub_0->szSymbol());
    }
    else {
      Card *pSub_0 = pChild->pGetChild(0);
      Card *pSub_1 = pChild->pGetChild(1);
      pRibbon->vSprintDataRight("%s == %s"_Stz, pSub_0->szSymbol(), pSub_1->szSymbol());
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

  pRibbon->vSprintData("==%02lld %c--%10p--"_Stz,
                       m_i_Child,
                       cLevel,
                       ((char*)this) - (char*)AM::SYSARGS::aRegionAddr());
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card::vPrintName(Ribbon* pRibbon, i8 iDepth, Stz* szCustomName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCardCtl());

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
                    m_i_Child);
    if (szCustomName)
      sName.vSprintCat("%s"_Stz,
                    szCustomName);
    else
      sName.vSprintCat("%s%s"_Stz,
                       szCardNm(),
                       sHiddenChildren.szStz());
  }

  pRibbon->vSprintData("%c%c%-18s"_Stz, m_cMark1, m_cMark2, sName.szStz());
}

/*----------------------------------------------------------------------------*/
/* print card name and its symbolic value on single line if possible          */
/*----------------------------------------------------------------------------*/
void Card::vPrintNameSymbol(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCardCtl());

  s32 sHiddenChildren;
  if (! pCardCtl->bPrintSubCards(m_eCard) && nChildCards())
    sHiddenChildren = " +"_Stz;

  s32 sName(szCardNm());
  if (0 == iDepth)
    sName.vToUpper();

  s256 sNameSymbol;

  if (0 == iDepth) {
    if (m_xSymbol.bHasSymbolicValue())
      sNameSymbol.vSprint("%s%s %s *%lld"_Stz,
                          sName.szStz(),
                          sHiddenChildren.szStz(),
                          szSymbol(),
                          nItems());
    else
      sNameSymbol.vSprint("%s%s *%lld"_Stz,
                          sName.szStz(),
                          sHiddenChildren.szStz(),
                          nItems());

  }
  else {
    if (pCardCtl->bPrintNo(m_eCard))
      sNameSymbol.vSprint("%s%lld]"_Stz,
                          Stz::szDots(iDepth - 1),
                          m_i_Child);
    if (m_xSymbol.bHasSymbolicValue())
      sNameSymbol.vSprintCat("%s%s %s"_Stz,
                             sName.szStz(),
                             sHiddenChildren.szStz(),
                             szSymbol());
    else
      sNameSymbol.vSprintCat("%s%s"_Stz,
                             sName.szStz(),
                             sHiddenChildren.szStz());
  }

  if (sNameSymbol.nLength() <= 18) {
    pRibbon->vSprintData("%c%c%-18s"_Stz, m_cMark1, m_cMark2, sNameSymbol.szStz());
  }
  else {
    vPrintName(pRibbon, iDepth);
    vPrintSymbol(pRibbon, iDepth);
  }
}

/*----------------------------------------------------------------------------*/
/* print card properties                                                      */
/*----------------------------------------------------------------------------*/
void Card::vPrintProperties(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_xSymbol.m_pProperties)
    return;

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCardCtl());

  bool bPrintPptName = pCardCtl->bPrintPptName(m_eCard);

  i8 nProperties = pSymbol()->Y_nPpy();

  for (i8 i_Property = 0; i_Property < nProperties; i_Property++) {

    eTN eTkn = pSymbol()->Y_ePpyTkn(i_Property);

    switch (eTkn) {

      case TN_INTEGER: {
        Stz* szName = pSymbol()->Y_szPpyNm(i_Property);
        i8   iPpy = pSymbol()->Y_iGet(i_Property);

        Stz* szDecode = szDecodeEnum(szName, iPpy);
        if (szDecode)
          pRibbon->vSprintDataRight("%s"_Stz,szDecode);
        else
        if (bPrintPptName)
          pRibbon->vSprintDataRight("%s=%lld"_Stz, szName, iPpy);
        else
          pRibbon->vSprintDataRight("%lld"_Stz, iPpy);
        break;
      }

      case TN_STRING: {
        Stz* szName  = pSymbol()->Y_szPpyNm(i_Property);
        Stz* szPpy = pSymbol()->Y_szGet(i_Property);
        if (bPrintPptName)
          pRibbon->vSprintDataRight("%s=%s"_Stz, szName, szPpy);
        else
          pRibbon->vSprintDataRight("%s"_Stz, szPpy);
        break;
      }

      default:
        break;
    }

  }
}

/*----------------------------------------------------------------------------*/
/* print symbolic value of card                                               */
/*----------------------------------------------------------------------------*/
void Card::vPrintSymbol(Ribbon* pRibbon, i8 iDepth, bool bPrintMarks)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! m_xSymbol.bHasSymbolicValue())
    return;

  if (bPrintMarks)
    pRibbon->vSprintDataRight("%c%c%18s"_Stz, m_cMark1, m_cMark2, szSymbol());
  else
    pRibbon->vSprintDataRight("  %s"_Stz, szSymbol());
}

/*----------------------------------------------------------------------------*/
/* renumber subCard's                                                         */
/*----------------------------------------------------------------------------*/
void Card::vRenumberRoot(i8 i_Board)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  m_i_Child = i_Board;

  vBearing(nullptr);

  i8 iSubCardNo = 0;
  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();

    pSubCard->v_Child(iSubCardNo++);
    pSubCard->vRenumberRecursive(this);
  }

  delete pSubCard_I;

  vBearing();
}

/*----------------------------------------------------------------------------*/
/* renumber subCard's                                                         */
/*----------------------------------------------------------------------------*/
void Card::vRenumberRecursive(Card* pParent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(pParent);
  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  vBearing(pParent);

  i8 iSubCardNo = 0;
  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();

    pSubCard->v_Child(iSubCardNo++);
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

  Card* pParent = pFindParentRecursive(pSub);

  pParent->vReplaceChild(pSub,pWith);
}

/*----------------------------------------------------------------------------*/
/* does this card have same symbol as given argument?                         */
/*----------------------------------------------------------------------------*/
bool Card::bSameSymbol(char cSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (m_xSymbol.bSame(cSymbol))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* does this card have same symbol as given argument?                         */
/*----------------------------------------------------------------------------*/
bool Card::bSameSymbol(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bSameSymbol(pSymbol->szSF__print()))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* does this card have same symbol as given argument?                         */
/*----------------------------------------------------------------------------*/
bool Card::bSameSymbol(Stz* szSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (m_xSymbol.bSame(szSymbol))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this card's symbolic value the same as given arg?                       */
/*----------------------------------------------------------------------------*/
bool Card::bSameSymbol(Card* pArg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (szSymbol()->bSame(pArg->szSymbol()))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get symbol                                                                 */
/*----------------------------------------------------------------------------*/
Symbol* Card::pSymbol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return &m_xSymbol;
}

/*----------------------------------------------------------------------------*/
/* get card symbol                                                            */
/*----------------------------------------------------------------------------*/
char Card::cSymbol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  char c = m_xSymbol.cSF_char();

  return c;
}

/*----------------------------------------------------------------------------*/
/* get card symbol                                                            */
/*----------------------------------------------------------------------------*/
i8 Card::iSymbol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i = m_xSymbol.iSF_number();

  return i;
}

/*----------------------------------------------------------------------------*/
/* get card symbol                                                            */
/*----------------------------------------------------------------------------*/
Stz* Card::szSymbol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* sz = m_xSymbol.szSF_string();

  return sz;
}

/*----------------------------------------------------------------------------*/
/* set card symbol                                                            */
/*----------------------------------------------------------------------------*/
void Card::vSymbol(Stz* szSymbol, eSF eSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  //RASSERT(! *m_xSymbol.m_sSymbol.azChar());

  m_xSymbol.vSet(eSymbol,szSymbol);
}

/*----------------------------------------------------------------------------*/
/* find sub card with given symbol and replace                                */
/*----------------------------------------------------------------------------*/
bool Card::bUpdateSymbolRecursive(eCK eSub, Symbol* pSymbol, Symbol* pWith)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bReplaced = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfUpdateSymbol = [pSymbol, pWith, &bReplaced](Card *pCard)
  {
    if (! pCard->bSameSymbol(pSymbol))
      return C_CONTINUE_RECURSION;

    pCard->m_xSymbol.vSet(pWith);

    bReplaced = true;

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bApplyRecursive(eSub, bfUpdateSymbol);

  return bReplaced;
}

/*----------------------------------------------------------------------------*/
/* validate set of child cards                                                 */
/*----------------------------------------------------------------------------*/
void Card::vValidate_mayHave(eCK eCard_first, ...)
{
  va_list xArgs;

//  va_start(xArgs,eCard_first);
//
//  while (CK_NONE != eCard_first) {
//    eCard_first = va_arg(xArgs, eCK);
//  }
//
//  va_end(xArgs);

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  while (pSubCard_I->bHasNext()) {
    Card* pTestCard = pSubCard_I->pNext();
    eCK eCard_test = pTestCard->eCard();

    bool bMayHave = false;

    va_start(xArgs, eCard_first);
    eCK eCard_mayHave = eCard_first;
    while (CK_NONE != eCard_mayHave) {
      eCard_mayHave = (eCK)va_arg(xArgs, i8);
      if (eCard_test == eCard_mayHave) {
        bMayHave = true;
        break;
      }
    }
    va_end(xArgs);

    if (! bMayHave)
      vDump(__func__);
    RASSERT(bMayHave);
  }

  delete pSubCard_I;
}

/*----------------------------------------------------------------------------*/
/* validate set of child cards                                                */
/*----------------------------------------------------------------------------*/
void Card::vValidate_mustHave(eCK eCard_first, ...)
{
  va_list xArgs;

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  va_start(xArgs, eCard_first);
  eCK eCard_mustHave = eCard_first;
  while (CK_NONE != eCard_mustHave) {

    bool bMustHave = false;

    pSubCard_I->vBeforeFirst();
    while (pSubCard_I->bHasNext()) {
      Card* pTestCard = pSubCard_I->pNext();
      eCK eCard_test = pTestCard->eCard();
      if (eCard_test == eCard_mustHave) {
        bMustHave = true;
        break;
      }
    }

    if (! bMustHave) {
      vTrace("vValidate_mustHave: missing %lld %lls\n"_Stz,(i8)eCard_mustHave,aCK[eCard_mustHave]);
      vDump(__func__);
      RASSERT(bMustHave);
    }

    eCard_mustHave = (eCK)va_arg(xArgs, i8);
  }

  va_end(xArgs);

  delete pSubCard_I;
}

/*----------------------------------------------------------------------------*/
/* validate set of child cards                                                */
/*----------------------------------------------------------------------------*/
eCK Card::eValidate_mustEither(eCK eCard_first, ...)
{
  va_list xArgs;

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  i8 nHas = 0;
  eCK eFound = CK_NONE;

  va_start(xArgs, eCard_first);
  eCK eCard_mustHave = eCard_first;
  while (CK_NONE != eCard_mustHave) {

    pSubCard_I->vBeforeFirst();
    while (pSubCard_I->bHasNext()) {
      Card* pTestCard = pSubCard_I->pNext();
      eCK eCard_test = pTestCard->eCard();
      if (eCard_test == eCard_mustHave) {
        nHas++;
        eFound = eCard_test;
      }
    }


    eCard_mustHave = (eCK)va_arg(xArgs, i8);
  }

  if (1 != nHas)
    vDump(__func__);
  RASSERT(1 == nHas);

  va_end(xArgs);

  delete pSubCard_I;

  return eFound;
}

/*----------------------------------------------------------------------------*/
/* validate set of child cards                                                */
/*----------------------------------------------------------------------------*/
void Card::vValidate_notHave(eCK eCard, ...)
{
  va_list xArgs;

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  va_start(xArgs, eCard);
  eCK eCard_mustHave = eCard;
  while (CK_NONE != eCard_mustHave) {

    bool bNotHave = false;

    pSubCard_I->vBeforeFirst();
    while (pSubCard_I->bHasNext()) {
      Card* pTestCard = pSubCard_I->pNext();
      eCK eCard_test = pTestCard->eCard();
      if (eCard_test == eCard_mustHave) {
        bNotHave = true;
        break;
      }
    }

    if (bNotHave)
      vDump(__func__);
    RASSERT(! bNotHave);

    eCard_mustHave = (eCK)va_arg(xArgs, i8);
  }

  va_end(xArgs);

  delete pSubCard_I;
}




/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode enum                                                                */
/*----------------------------------------------------------------------------*/
Stz* Card::szDecodeEnum(Stz* szEnum, i8 iEnum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (szEnum->bSame("eLevel"_Stz))      return aAL[iEnum];
  if (szEnum->bSame("eSendKind"_Stz))   return aXK[iEnum];
  if (szEnum->bSame("eStageTopo"_Stz))  return aYG[iEnum];
  if (szEnum->bSame("eType"_Stz))       return aCT[iEnum];

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


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
#include "Card_JoinCast.hpp"
Card_JoinCast* Card::pSub_locusCast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_JoinCast* p_castNext =
      _pCard_JoinCast(pFindSubCard(CK_JOINCAST));
  return p_castNext;
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
#include "Card_FiltPred.hpp"
Card_FiltPred* Card::pSub_filtPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_FiltPred* p_filtPred =
      _pCard_FiltPred(pFindSubCard(CK_FILTPRED));
  return p_filtPred;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_FiltPredList.hpp"
Card_FiltPredList* Card::pSub_filtPredList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_FiltPredList* p_filtPredList =
      _pCard_FiltPredList(pFindSubCard(CK_FILTPREDLIST));
  return p_filtPredList;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Flat.hpp"
Card_Flat* Card::pSub_flat(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Flat* p_flat =
      _pCard_Flat(pFindSubCard(CK_FOLD));
  return p_flat;
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

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_HshCol.hpp"
Card_HshCol* Card::pSub_hshCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_HshCol* p_hshCol =
      _pCard_HshCol(pFindSubCard(CK_HSHCOL));
  return p_hshCol;
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
#include "Card_JoinEstimate.hpp"
Card_JoinEstimate* Card::pSub_joinEstimate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_JoinEstimate* p_joinCost =
      _pCard_JoinEstimate(pFindSubCard(CK_JOINESTIMATE));
  return p_joinCost;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_CostJoining.hpp"
Card_CostJoining* Card::pSub_costJoining(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CostJoining* p_costJoining =
      _pCard_CostJoining(pFindSubCard(CK_COSTJOINING));
  return p_costJoining;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_CostNetwork.hpp"
Card_CostNetwork* Card::pSub_costNetwork(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CostNetwork* p_costNetwork =
      _pCard_CostNetwork(pFindSubCard(CK_COSTNETWORK));
  return p_costNetwork;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_CostStream.hpp"
Card_CostStream* Card::pSub_costQuery(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CostStream* p_costQuery =
      _pCard_CostStream(pFindSubCard(CK_COSTSTREAM));
  return p_costQuery;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_CostUnicast.hpp"
Card_CostUnicast* Card::pSub_costUnicast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CostUnicast* p_loadUnicast =
      _pCard_CostUnicast(pFindSubCard(CK_COSTUNICAST));
  return p_loadUnicast;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_KeyCells.hpp"
Card_KeyCells* Card::pSub_keyCells(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_KeyCells* p_keyCells =
      _pCard_KeyCells(pFindSubCard(CK_KEYCELLS));
  return p_keyCells;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Map.hpp"
Card_Map* Card::pSub_map(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Map* p_map =
      _pCard_Map(pFindSubCard(CK_MAP));
  return p_map;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Mapping.hpp"
Card_Mapping* Card::pSub_mapping(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Mapping* p_mapping =
      _pCard_Mapping(pFindSubCard(CK_MAPPING));
  return p_mapping;
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
#include "Card_Needed.hpp"
Card_Needed* Card::pSub_needed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Needed* p_needed =
      _pCard_Needed(pFindSubCard(CK_NEEDED));
  return p_needed;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Cast.hpp"
Card_Cast* Card::pSub_next(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Cast* p_next =
      _pCard_Cast(pFindSubCard(CK_CAST));
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
#include "Card_OuterColNm.hpp"
Card_OuterColNm* Card::pSub_outerColNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_OuterColNm* p_outerColNm =
      _pCard_OuterColNm(pFindSubCard(CK_OUTERCOLNM));
  return p_outerColNm;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_OuterJoin.hpp"
Card_OuterJoin* Card::pSub_outerJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_OuterJoin* p_outerJoin =
      _pCard_OuterJoin(pFindSubCard(CK_OUTERJOIN));
  return p_outerJoin;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_PayloadCells.hpp"
Card_PayloadCells* Card::pSub_payloadCells(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_PayloadCells* p_payloadCells =
      _pCard_PayloadCells(pFindSubCard(CK_PAYLOADCELLS));
  return p_payloadCells;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Correlation.hpp"
Card_Correlation* Card::pSub_correlation(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Correlation* p_paramList =
      _pCard_Correlation(pFindSubCard(CK_CORRELATION));
  return p_paramList;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Count.hpp"
Card_Count* Card::pSub_count(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Count* p_count =
      _pCard_Count(pFindSubCard(CK_COUNT));
  return p_count;
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
#include "Card_RowCount.hpp"
Card_RowCount* Card::pSub_rowCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_RowCount* p_rowCount =
      _pCard_RowCount(pFindSubCard(CK_ROWCOUNT));
  return p_rowCount;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_RowSize.hpp"
Card_RowSize* Card::pSub_rowSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_RowSize* p_rowSize =
      _pCard_RowSize(pFindSubCard(CK_ROWSIZE));
  return p_rowSize;
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
#include "Card_StreamPtn.hpp"
Card_StreamPtn* Card::pSub_streamPtn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StreamPtn* p_streamPtn =
      _pCard_StreamPtn(pFindSubCard(CK_SECTION));
  return p_streamPtn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_TableSize.hpp"
Card_TableSize* Card::pSub_tableSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_TableSize* p_size =
      _pCard_TableSize(pFindSubCard(CK_TABLESIZE));
  return p_size;
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
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
