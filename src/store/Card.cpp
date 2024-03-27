/*============================================================================*/
/* Card.cpp -- Abstract Card                                                  */
/*============================================================================*/

#include "Card.hpp"
#include "CardCtl.hpp"
#include "Board.hpp"
#include "CardIter.hpp"
#include "Kid.hpp"

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

/*----------------------------------------------------------------------------*/
/* Constants and static variables                                             */
/*----------------------------------------------------------------------------*/
i8 Card::m__iSeqGen { 1 };

Stz* P_AggOp        = "bAggOp"_Stz;
Stz* P_BreakPt      = "bBreakPt"_Stz;
Stz* P_BumpedBy     = "nBumpedBy"_Stz;
Stz* P_Cell         = "i_Cell"_Stz;
Stz* P_ChosenBound  = "bChosenBound"_Stz;
Stz* P_ChosenIndex  = "bChosenIndex"_Stz;
Stz* P_Count        = "nCount"_Stz;
Stz* P_CrlKeyConj   = "bCrlKeyConj"_Stz;
Stz* P_CrlKeyDefn   = "bCrlKeyDefn"_Stz;
Stz* P_CrlKeyTerm   = "bCrlKeyTerm"_Stz;
Stz* P_CrlKeyTest   = "bCrlKeyTest"_Stz;
Stz* P_DebugOption  = "szDebugOption"_Stz;
Stz* P_Direction    = "szDirection"_Stz;
Stz* P_Early        = "bEarly"_Stz;
Stz* P_EmittedCol   = "i_EmittedCol"_Stz;
Stz* P_FixedPos     = "bFixedPos"_Stz;
Stz* P_FusedProject = "bFusedProject"_Stz;
Stz* P_HashOf       = "szHashOf"_Stz;
Stz* P_Injected     = "bInjected"_Stz;
Stz* P_InjCrlKey    = "bInjCrlKey"_Stz;
Stz* P_Invalid      = "bInvalid"_Stz;
Stz* P_Key          = "bKey"_Stz;
Stz* P_Level        = "eLevel"_Stz;
Stz* P_Payload      = "bPayload"_Stz;
Stz* P_PrmOdn       = "szPrmOdn"_Stz;
Stz* P_Permutation  = "i_Permutation"_Stz;
Stz* P_Pointer      = "pPointer"_Stz;
Stz* P_ProjBump     = "nProjBump"_Stz;
Stz* P_ProjBumped   = "nProjBumped"_Stz;
Stz* P_SendKind     = "eSendKind"_Stz;
Stz* P_Smoothed     = "bSmoothed"_Stz;
Stz* P_StageTopo    = "eStageTopo"_Stz;
Stz* P_SqyCall      = "bSqyCall"_Stz;
Stz* P_SqyRslTest   = "bSqyRslTest"_Stz;
Stz* P_Unproject    = "bUnproject"_Stz;

Stz* C_CARD_HISTORY = "<card_history>"_Stz;
Stz* C_CODE_BLOCK   = "<code_block>"_Stz;
Stz* C_FILTERPRED   = "<filterPred>"_Stz;
Stz* C_GLEAN_CUE_FROM = "<glean_cue_from>"_Stz;
Stz* C_JOWRITE      = "<joWrite>"_Stz;
Stz* C_JO_DISABLE   = "<jo_disable>"_Stz;
Stz* C_JO_PICK      = "<jo_pick>"_Stz;
Stz* C_NA                = "n/a"_Stz;
Stz* C_HASCODEFILE = "<hasCodeFile>"_Stz;
Stz* C_OPTIMISER_EP_DISABLE = "<optimiser_ep_disable>"_Stz;
Stz* C_OPTIMISER_TRACE = "<optimiser_trace>"_Stz;
Stz* C_OPTIMISE_PASSTHRU = "<optimise_passThru>"_Stz;
Stz* C_PIPELINE_DEF = "<pipeline_def>"_Stz;
Stz* C_WEAVED       = "<weaved>"_Stz;

d8 C_EXPANSION_COEFF = 1.60;  // crw -> row buffer
d8 C_FILTER_COEFF    = 0.30;
d8 C_JOIN_COEFF      = 2.00;  // bump the larger
d8 C_JOIN_LONGER     = 1.50;  // joined row gets longer

Stz* MARK_AND        = "&"_Stz;
Stz* MARK_AT         = "@"_Stz;
Stz* MARK_BLANK      = " "_Stz;
Stz* MARK_CHANGED    = "`"_Stz;
Stz* MARK_COMPARE    = "="_Stz;
Stz* MARK_ENUM       = ":"_Stz;
Stz* MARK_DELETED    = "Z"_Stz;
Stz* MARK_DUPLICATE  = "D"_Stz;
Stz* MARK_FOUND      = "+"_Stz;
Stz* MARK_NOT        = "!"_Stz;
Stz* MARK_OR         = "|"_Stz;
Stz* MARK_PREDICATE  = "?"_Stz;
Stz* MARK_UP         = "^"_Stz;
Stz* MARK_UNVALID    = "X"_Stz;
Stz* MARK_VALID      = "Y"_Stz;

Stz* C_CRLKEY      = "_CRLKEY_Q%lld_"_Stz;
Stz* C_SQYRSL      = "_SQYRSL_Q%lld_"_Stz;

Stz* C_ASC         = "ASC"_Stz;
Stz* C_DESC        = "DESC"_Stz;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card::Card(eCK eCard, i8 iFixedDepth)
: Object(CN_CARD), ChildCards(this)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  MemLib::vZero(md_Coord, sizeof(md_Coord), CN_CARD);

  if (CK_BOUNDS == eCard)
    vDebugPoint();

  m_eCard   = eCard;
  m_i_Child = C_UNKNOWN;

  m_pLastPrintParent = nullptr;
  m_pParent = nullptr;

  md__SeqNo = Card::m__iSeqGen++;
  //gapCard[md__SeqNo] = this;
  md__FirstDetected  = C_UNKNOWN;
  md__LeakDetected   = C_UNKNOWN;

  m_iFixedDepth = iFixedDepth;

  m_iFirstPrint = C_UNKNOWN;
  m_nPrintCount = 0;
  m_nFunctionCount = 0;
  m_eFirst = CK_NONE;

  m_bAddedCorrectly   = false;
  m_bRemovedCorrectly = false;

  m_iCounter = 0;

  vMark1(MARK_BLANK);
  vMark2(MARK_BLANK);

  s256 sCoord;
  sCoord.vSprint("((%s new %04lld))"_Stz, aCK[m_eCard], md__SeqNo);
  StrLib::vCopy(md_Coord, sizeof(md_Coord), sCoord.azChar());
//  if (11 == md__SeqNo) {
//    vTrace("Card [%02lld] %p\n"_Stz, md__SeqNo, this);
//    vDebugPoint();
//  }

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card::~Card(void) noexcept(false)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

//  vTrace("Card [~%02lld] %p %s %s : %s\n"_Stz,
//         md__SeqNo,this,aCK[m_eCard],m_xSymbol.m_sFacet.szStz(),md_Coord);
//
//  if (this == pPriorCard)
//    vDebugPoint();
//
//  if (678 == md__SeqNo) {
//    pPriorCard = this;
//    vDebugPoint();
//  }

  //gapCard[md__SeqNo] = nullptr;

  RASSERT(! bMark1(MARK_DELETED));

  vMark1(MARK_DELETED);

  RASSERT( (m_bAddedCorrectly && m_bRemovedCorrectly) ||
           (! m_bAddedCorrectly && ! m_bRemovedCorrectly));

  for (Card* pCard = pPopChild(); pCard; pCard = pPopChild())
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
//             i_Card, pLeaked, aCK[pLeaked->eCard()], pLeaked->szFacet());
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
  UNUSED(p_deck);
  UNUSED(iTransformNo);

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
//         p_cardInQuestion->szFacet());
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
/* test if eCard is a construction                                            */
/*----------------------------------------------------------------------------*/
bool Card::bIsConstruction(eCK eCard)
{
  _static_

  switch (eCard) {

    case CK_c_ABSJOIN:
    case CK_c_FLAT:
    case CK_c_FOLD:
    case CK_c_SEND:
    case CK_c_STAGE:
      return true;

    default:
      return false;
  }
}

/*----------------------------------------------------------------------------*/
/* test if eCard is a main                                                    */
/*----------------------------------------------------------------------------*/
bool Card::bIsMain(eCK eCard)
{
  _static_

  if (bIsOp(eCard))
    return true;

  if (bIsConstruction(eCard))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* test if eCard is a op                                                      */
/*----------------------------------------------------------------------------*/
bool Card::bIsOp(eCK eCard)
{
  _static_

  switch (eCard) {

    case CK_o_AGGREG:
    case CK_o_CALL:
    case CK_o_CAST:
    case CK_c_COMBINED:
    case CK_o_CORRELATION:
    case CK_o_CROSSJOIN:
    case CK_o_FILTER:
    case CK_o_FROM:
    case CK_o_GBYAGG:
    case CK_o_HASH:
    case CK_o_HAVING:
    case CK_o_INDEXSCAN:
    case CK_o_INDEXJOIN:
    case CK_o_INFLATE:
    case CK_o_KEYEXISTS:
    case CK_o_KEYNOTEXISTS:
    case CK_o_LIMIT:
    case CK_o_ORDAGG:
    case CK_o_PROJECT:
    case CK_o_SCAN:
    case CK_o_SORT:
    case CK_o_TOP:
    case CK_o_WHERE:
      return true;

    default:
      return false;
  }
}


/*------------------------------------ ----------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* card has been appended correctly                                           */
/*----------------------------------------------------------------------------*/
void Card::vAddedCorrectly(bool bAppended)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(CK_DECK != eCard());
  RASSERT(m_pParent)

  m_bAddedCorrectly = bAppended;
}

/*----------------------------------------------------------------------------*/
/* are all children given kind?                                               */
/*----------------------------------------------------------------------------*/
bool Card::bAllChildren(eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xCard(this); xCard.bEach(); /**/) {
    if (! xCard.pChild()->bIsCard(eCard))
      return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* append child card                                                          */
/*----------------------------------------------------------------------------*/
void Card::vAppendChild(Card* pChild)
{
  RASSERT(pChild);

  ChildCards::vAppendChild(pChild,this);
}

/*----------------------------------------------------------------------------*/
/* append new card to tail                                                    */
/*----------------------------------------------------------------------------*/
Card* Card::pAppendNewCard(eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card* pCard = new Card(eCard,C_UNKNOWN);

  vAppendChild(pCard);

  return pCard;
}

/*----------------------------------------------------------------------------*/
/* set type of symbol                                                         */
/*----------------------------------------------------------------------------*/
void Card::vApplyCast_p(Card* pCard_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  //pCard_arg->vDump(__func__);

  m_xSymbol.vApplyCast(pCard_arg->xCast());
}

/*----------------------------------------------------------------------------*/
/* set type of symbol                                                         */
/*----------------------------------------------------------------------------*/
void Card::vApplyCast_x(Cast xCast_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  //pCard_arg->vDump(__func__);

  m_xSymbol.vApplyCast(xCast_arg);
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
Card* Card::pCard(eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_eCard = eCard;

  return this;
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
/* get cast information                                                       */
/*----------------------------------------------------------------------------*/
Cast Card::xCast(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Cast xCast = pSymbol()->xCast();

  return xCast;
}

/*----------------------------------------------------------------------------*/
/* visit all sub-cards and check correctness                                   */
/*----------------------------------------------------------------------------*/
void Card::vCheckCorrectness(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(CK_DECK == eCard() || m_bAddedCorrectly);

  for (Kid xCard(this); xCard.bEach(); /**/) {
    xCard.pChild()->vCheckCorrectness();
  }
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
/* get coord                                                                  */
/*----------------------------------------------------------------------------*/
Stz* Card::szCoord(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return (Stz*)md_Coord;
}

/*----------------------------------------------------------------------------*/
/* return counter                                                             */
/*----------------------------------------------------------------------------*/
i8 Card::iCounter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_iCounter;
}

/*----------------------------------------------------------------------------*/
/* return next counter                                                        */
/*----------------------------------------------------------------------------*/
void Card::vCounterInc(i8 iInc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_iCounter += iInc;
}

/*----------------------------------------------------------------------------*/
/* reset counter                                                              */
/*----------------------------------------------------------------------------*/
void Card::vCounterReset(i8 iTo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_iCounter = iTo;
}

/*----------------------------------------------------------------------------*/
/* get eCT                                                                    */
/*----------------------------------------------------------------------------*/
eCT Card::eCTVal(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return xCast().m_xTypeId.eCTVal();
}

/*----------------------------------------------------------------------------*/
/* is given card a sub card of this card?                                     */
/*----------------------------------------------------------------------------*/
void Card::vDeleteFacetRecursive(eSF eFacet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pSymbol()->vDeleteFacet(eFacet);

  for (Kid xCard(this); xCard.bEach(); /**/) {
    xCard.pChild()->vDeleteFacetRecursive(eFacet);
  }
}

/*----------------------------------------------------------------------------*/
/* dump card and its sub cards                                                */
/*----------------------------------------------------------------------------*/
void Card::vDump(const char* azFunc, Stz* szTitle, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (0 == iDepth) {
    if (szTitle)
      vTrace("^^%s [%s] %p /P %p\n"_Stz,szTitle,azFunc,this,m_pParent);
    else
      vTrace("^^Card Dump [%s] %p /P %p\n"_Stz,azFunc,this,m_pParent);
  }

  s4K sFacets;
  m_xSymbol.vFacets_To_String(sFacets);

  s4K sProperties;
  m_xSymbol.Y_vTo_String(sProperties);

  vTrace("  %p /P %p %18s %s%s : %s%s <%s> {%s} (%s:%lld.%lld)\n"_Stz
        , this
        , m_pParent
        , aCK_short[m_eCard]
        , Stz::szTab(iDepth)
        , md_Coord[0] ? md_Coord : (char*)aCK[m_eCard]

        , m_xMark1.szChrSeq()
        , m_xMark2.szChrSeq()

        , m_xSymbol.bHasFacets() ? sFacets.szStz() : "noFacets"_Stz

        , m_xSymbol.bHasProperties() ? sProperties.szStz() : "noPpy"_Stz

        , m_xSymbol.xTypeId().bIsValid() ? m_xSymbol.xTypeId().szTypeName() : "noType"_Stz
        , m_xSymbol.xCast().m_nTypeLength
        , m_xSymbol.xCast().m_nTypeScale
  );

  for (Kid xCard(this); xCard.bEach(); /**/)
    xCard.pChild()->vDump(azFunc, nullptr, iDepth + 1);

  if (0 == iDepth)
    vTrace("$$\n"_Stz);

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* dump card (short form)                                                     */
/*----------------------------------------------------------------------------*/
void Card::vDumpShort(const char* azFunc, Stz* szTitle)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vTrace("--%s [%s] %p %s\n"_Stz,
         szTitle,
         azFunc,
         this,
         md_Coord);
}

/*----------------------------------------------------------------------------*/
/* duplicate this card, along with its symbolic value and properties          */
/*----------------------------------------------------------------------------*/
Card* Card::pDupCard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card* pCard_dup = new Card(m_eCard,m_iFixedDepth);
  pCard_dup->pSymbol()->vApplyCastFacets(pSymbol());

  return pCard_dup;
}

/*----------------------------------------------------------------------------*/
/* duplicate this card and its sub cards                                      */
/*----------------------------------------------------------------------------*/
Card* Card::pDupCardRecursive(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card* pCard_dup = new Card(m_eCard,m_iFixedDepth);
  pCard_dup->pSymbol()->vApplyCastFacetsPpy(pSymbol());

  for (Kid xCard(this); xCard.bEach(); /**/) {
    Card* pSubCard = xCard.pChild();
    
    Card* pSubCard_dup = pSubCard->pDupCardRecursive();
    pCard_dup->vAppendChild(pSubCard_dup);
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
    pCard_SubSet->vAppendChild(pChild_dup);
  }

  return pCard_SubSet;
}

/*----------------------------------------------------------------------------*/
/* test if card is one of given kind list                                     */
/*----------------------------------------------------------------------------*/
bool Card::bEither(i8 /*eCK*/ iKind_first, ...)
{
  // The first parameter before the varargs (...) must not undergo default
  // argument promotion. This will occur if the enum type is smaller than
  // a 64 bit integer. If that is the case then the result is undefined
  // in va_start(). So let's make the first parameter an integer to avoid this.

  va_list xArgs;

  bool bFound = false;

  va_start(xArgs, iKind_first);

  eCK eKind_mustHave = (eCK) iKind_first;
  while (CK_NONE != eKind_mustHave) {
    if (eCard() == eKind_mustHave) {
      bFound = true;
      break;
    }
    eKind_mustHave = (eCK)va_arg(xArgs, i8);
  }

  va_end(xArgs);

  return bFound;
}

/*----------------------------------------------------------------------------*/
/* get facet symbol                                                           */
/*----------------------------------------------------------------------------*/
Stz* Card::szFacet(eSF eFacet_xchk)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! m_xSymbol.nFacets())
    return nullptr;

  Stz* szChrSeq = m_xSymbol.szFacet(eFacet_xchk);

  return szChrSeq;
}

/*----------------------------------------------------------------------------*/
/* find parent of given sub-card                                              */
/*----------------------------------------------------------------------------*/
Card* Card::pFindParentRecursive(Card* pChild)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xCard(this); xCard.bEach(); /**/) {
    Card* pTestCard = xCard.pChild();
    if (pChild == pTestCard)
      return this;

    Card* pParent = pTestCard->pFindParentRecursive(pChild);
    if (pParent) {
      RASSERT(pChild->pParent() == pParent);
      return pParent;
    }
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

  for (Kid xCard(this); xCard.bEach(); /**/) {
    if (Card *pFoundCard = xCard.pChild()->pFindCardRecursive(eCard))
      return pFoundCard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find mark1 recursively within this card                                    */
/*----------------------------------------------------------------------------*/
Card* Card::pFindMark1Recursive(char cMark1, eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ( (CK_NONE == eCard || eCard == m_eCard)) {
    if (cMark1 == this->cMark1())
      return this;
  }

  for (Kid xCard(this); xCard.bEach(); /**/) {
    if (Card *pFoundCard = xCard.pChild()->pFindMark1Recursive(cMark1, eCard))
      return pFoundCard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find mark1 recursively within this card                                    */
/*----------------------------------------------------------------------------*/
Card* Card::pFindMark1Recursive(Stz* szMark1, eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ( (CK_NONE == eCard || eCard == m_eCard)) {
    if (this->bMark1(szMark1))
      return this;
  }

  for (Kid xCard(this); xCard.bEach(); /**/) {
    if (Card *pFoundCard = xCard.pChild()->pFindMark1Recursive(szMark1, eCard))
      return pFoundCard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find mark2 recursively within this card                                    */
/*----------------------------------------------------------------------------*/
Card* Card::pFindMark2Recursive(char cMark2, eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ( (CK_NONE == eCard || eCard == m_eCard)) {
    if (cMark2 == this->cMark2())
      return this;
  }

  for (Kid xCard(this); xCard.bEach(); /**/) {
    if (Card *pFoundCard = xCard.pChild()->pFindMark2Recursive(cMark2, eCard))
      return pFoundCard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find mark2 recursively within this card                                    */
/*----------------------------------------------------------------------------*/
Card* Card::pFindMark2Recursive(Stz* szMark2, eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ( (CK_NONE == eCard || eCard == m_eCard)) {
    if (this->bMark2(szMark2))
      return this;
  }

  for (Kid xCard(this); xCard.bEach(); /**/) {
    if (Card *pFoundCard = xCard.pChild()->pFindMark2Recursive(szMark2, eCard))
      return pFoundCard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find sub card with given facet                                             */
/*----------------------------------------------------------------------------*/
Card* Card::pFindCard_WithSubFacet(eCK eCard, eSF eFacet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (eCard == this->eCard() && bHasSubFacet(eFacet))
    return this;

  for (Kid xCard(this); xCard.bEach(); /**/) {
    if (Card *pFoundCard = xCard.pChild()->pFindCard_WithSubFacet(eCard,eFacet))
      return pFoundCard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find property recursively within this card                                 */
/*----------------------------------------------------------------------------*/
Card* Card::pFindPpyRecursive(Stz* szPpy, eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ( (CK_NONE == eCard || eCard == m_eCard)) {
    if (pSymbol()->Y_bHas(szPpy))
      return this;
  }

  for (Kid xCard(this); xCard.bEach(); /**/) {
    if (Card *pFoundCard = xCard.pChild()->pFindPpyRecursive(szPpy, eCard))
      return pFoundCard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* attempt find of symbolic value                                             */
/*----------------------------------------------------------------------------*/
Card* Card::pFindSymbolRecursive(eCK eCard, Symbol* pSymbol, Card*& rpParent, Card* _pWalkParent_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (eCard == m_eCard && bSymbolMatches(pSymbol)) {
    rpParent = _pWalkParent_;
    return this;
  }

  for (Kid xCard(this); xCard.bEach(); /**/) {
    if (Card *pFoundCard = xCard.pChild()->pFindSymbolRecursive(eCard, pSymbol, rpParent, this))
      return pFoundCard;
  }

  rpParent = nullptr;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* test fixed depth                                                           */
/*----------------------------------------------------------------------------*/
bool Card::bFixedDepth(i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (C_UNKNOWN == iDepth || C_UNKNOWN == m_iFixedDepth)
    return true;

  if (iDepth == m_iFixedDepth)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get fixed depth                                                            */
/*----------------------------------------------------------------------------*/
i8 Card::iFixedDepth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_iFixedDepth;
}

/*----------------------------------------------------------------------------*/
/* set fixed depth                                                            */
/*----------------------------------------------------------------------------*/
void Card::vFixedDepth(i8 iFixedDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_iFixedDepth = iFixedDepth;
}

/*----------------------------------------------------------------------------*/
/* fetch function count                                                       */
/*----------------------------------------------------------------------------*/
i8 Card::nFunctionCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_nFunctionCount;
}

/*----------------------------------------------------------------------------*/
/* is there a sub card with given facet?                                      */
/*----------------------------------------------------------------------------*/
bool Card::bHasChildFacet(eSF eFacet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xCard(this); xCard.bEach(); /**/) {
    if (xCard.pChild()->pSymbol()->bHasFacet(eFacet))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* is given card a sub card of this card?                                     */
/*----------------------------------------------------------------------------*/
bool Card::bHasSub(Card* pSubCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (this == pSubCard)
    return true;

  for (Kid xCard(this); xCard.bEach(); /**/) {
     if (xCard.pChild()->bHasSub(pSubCard))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* is there a sub card with given facet?                                      */
/*----------------------------------------------------------------------------*/
bool Card::bHasSubFacet(eSF eFacet, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (iDepth && pSymbol()->bHasFacet(eFacet))
    return true;

  for (Kid xCard(this); xCard.bEach(); /**/) {
    if (xCard.pChild()->bHasSubFacet(eFacet,++iDepth))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* increment function count                                                   */
/*----------------------------------------------------------------------------*/
void Card::vIncFunctionCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_nFunctionCount++;
}

/*----------------------------------------------------------------------------*/
/* insert child card after given card                                         */
/*----------------------------------------------------------------------------*/
//void Card::vInsertChildAfter(Card* pChild, Card* pAfter)
//{
//  RASSERT(pChild);
//  RASSERT(pAfter);
//
//  ChildCards::vInsertAfter(pChild,pAfter);
//}

/*----------------------------------------------------------------------------*/
/* insert child card before given card                                        */
/*----------------------------------------------------------------------------*/
//void Card::vInsertChildBefore(Card* pChild, Card* pBefore)
//{
//  RASSERT(pChild);
//  RASSERT(pBefore);
//
//  ChildCards::vInsertBefore(pChild,pBefore);
//}

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
/* is this card a construction card                                           */
/*----------------------------------------------------------------------------*/
bool Card::bIsConstruction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bIsConstruction(eCard()))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this card a main card                                                   */
/*----------------------------------------------------------------------------*/
bool Card::bIsMain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bIsMain(eCard()))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this card a op card                                                     */
/*----------------------------------------------------------------------------*/
bool Card::bIsOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bIsOp(eCard()))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is child card the given kind?                                              */
/*----------------------------------------------------------------------------*/
bool Card::bIsPeek(eCK eChild0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (eChild0 == this->ePeekChild(0))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* are child cards the given kinds?                                           */
/*----------------------------------------------------------------------------*/
bool Card::bIsPeek(eCK eChild0, eCK eChild1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (eChild0 == this->ePeekChild(0) && eChild1 == this->ePeekChild(1))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this card the given kind?                                               */
/*----------------------------------------------------------------------------*/
bool Card::bIsPeek(i8 i_Child, eCK eChild)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (eChild == this->ePeekChild(i_Child))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* visit all sub-cards of given eToSubs and apply lambda function             */
/*----------------------------------------------------------------------------*/
bool Card::bLambdaWalk(eCK eToSubs, std::function<bool(Card* pCard)> bfLambdaFn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (CK_NONE == eToSubs || eToSubs == m_eCard) {
    bool bContinue = bfLambdaFn(this);
    if (! bContinue)
      return C_TERMINATE_RECURSION;
  }

  for (Kid xCard(this); xCard.bEach(); /**/) {
    bool bContinue = xCard.pChild()->bLambdaWalk(eToSubs, bfLambdaFn);
    if (! bContinue)
      return C_TERMINATE_RECURSION;
  }

  return C_CONTINUE_RECURSION;
}

/*----------------------------------------------------------------------------*/
/* visit all term sub-cards and apply lambda function                         */
/*----------------------------------------------------------------------------*/
bool Card::bLambdaWalk_term(std::function<bool(Card_Term* p_term)> bfLambdaFn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (bIsCard(CK_TERM)) {
    bool bContinue = bfLambdaFn(_pCard_Term(this));
    if (! bContinue)
      return false;
  }

  for (Kid xCard(this); xCard.bEach(); /**/) {
    if (!xCard.pChild()->bLambdaWalk_term(bfLambdaFn))
      return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* set mark 1                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vMark(Stz* szMark1, Stz* szMark2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vMark1(szMark1);
  vMark2(szMark2);
}

/*----------------------------------------------------------------------------*/
/* test mark 1                                                                */
/*----------------------------------------------------------------------------*/
bool Card::bMark1(Stz* szArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (m_xMark1.bSame(szArg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get mark 1                                                                 */
/*----------------------------------------------------------------------------*/
char Card::cMark1(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_xMark1.cChr();
}

/*----------------------------------------------------------------------------*/
/* get mark 1                                                                 */
/*----------------------------------------------------------------------------*/
Stz* Card::szMark1(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_xMark1.szChrSeq();
}

/*----------------------------------------------------------------------------*/
/* set mark 1                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vMark1(char cMark1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_xMark1.vChr(cMark1);
}

/*----------------------------------------------------------------------------*/
/* set mark 1                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vMark1(Stz* szMark1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_xMark1.vChrSeq(szMark1);
}

/*----------------------------------------------------------------------------*/
/* set mark 1 recursively                                                     */
/*----------------------------------------------------------------------------*/
void Card::vMark1Recursive(char cMark1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vMark1(cMark1);

  for (Kid xCard(this); xCard.bEach(); /**/) {
    Card *pSubCard = xCard.pChild();

    pSubCard->vMark1Recursive(cMark1);
  }
}

/*----------------------------------------------------------------------------*/
/* set mark 1 recursively                                                     */
/*----------------------------------------------------------------------------*/
void Card::vMark1Recursive(Stz* szMark1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vMark1(szMark1);

  for (Kid xCard(this); xCard.bEach(); /**/) {
    Card *pSubCard = xCard.pChild();

    pSubCard->vMark1Recursive(szMark1);
  }
}

/*----------------------------------------------------------------------------*/
/* set mark 1 recursively                                                     */
/*----------------------------------------------------------------------------*/
void Card::vMark1RecursiveIf(char cMark1, char cIf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (cIf == this->cMark1()) {
    vMark1(cMark1);
  }

  for (Kid xCard(this); xCard.bEach(); /**/) {
    Card *pSubCard = xCard.pChild();

    pSubCard->vMark1RecursiveIf(cMark1,cIf);
  }
}

/*----------------------------------------------------------------------------*/
/* set mark 1 recursively                                                     */
/*----------------------------------------------------------------------------*/
void Card::vMark1RecursiveIf(Stz* szMark1, Stz* szIf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bMark1(szIf)) {
    vMark1(szMark1);
  }

  for (Kid xCard(this); xCard.bEach(); /**/) {
    Card *pSubCard = xCard.pChild();

    pSubCard->vMark1RecursiveIf(szMark1,szIf);
  }
}

/*----------------------------------------------------------------------------*/
/* test mark 2                                                                */
/*----------------------------------------------------------------------------*/
bool Card::bMark2(Stz* szArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (m_xMark2.bSame(szArg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get mark 2                                                                 */
/*----------------------------------------------------------------------------*/
char Card::cMark2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_xMark2.cChr();
}

/*----------------------------------------------------------------------------*/
/* get mark 2                                                                 */
/*----------------------------------------------------------------------------*/
Stz* Card::szMark2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_xMark2.szChrSeq();
}

/*----------------------------------------------------------------------------*/
/* set mark 2                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vMark2(char cMark2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_xMark2.vChr(cMark2);
}

/*----------------------------------------------------------------------------*/
/* set mark 1                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vMark2(Stz* szMark2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_xMark2.vChrSeq(szMark2);
}

/*----------------------------------------------------------------------------*/
/* set mark 2 recursively                                                     */
/*----------------------------------------------------------------------------*/
void Card::vMark2Recursive(char cMark2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vMark2(cMark2);

  for (Kid xCard(this); xCard.bEach(); /**/) {
    Card *pSubCard = xCard.pChild();

    pSubCard->vMark2Recursive(cMark2);
  }
}

/*----------------------------------------------------------------------------*/
/* set mark 2 recursively                                                     */
/*----------------------------------------------------------------------------*/
void Card::vMark2Recursive(Stz* szMark2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vMark2(szMark2);

  for (Kid xCard(this); xCard.bEach(); /**/) {
    Card *pSubCard = xCard.pChild();

    pSubCard->vMark2Recursive(szMark2);
  }
}

/*----------------------------------------------------------------------------*/
/* set mark 2 recursively                                                     */
/*----------------------------------------------------------------------------*/
void Card::vMark2RecursiveIf(char cMark2, char cIf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (cIf == this->cMark2()) {
    vMark2(cMark2);
  }

  for (Kid xCard(this); xCard.bEach(); /**/) {
    Card *pSubCard = xCard.pChild();

    pSubCard->vMark2RecursiveIf(cMark2,cIf);
  }
}

/*----------------------------------------------------------------------------*/
/* set mark 2 recursively                                                     */
/*----------------------------------------------------------------------------*/
void Card::vMark2RecursiveIf(Stz* szMark2, Stz* szIf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bMark2(szIf)) {
    vMark2(szMark2);
  }

  for (Kid xCard(this); xCard.bEach(); /**/) {
    Card *pSubCard = xCard.pChild();

    pSubCard->vMark2RecursiveIf(szMark2,szIf);
  }
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
/* clear all marks                                                            */
/*----------------------------------------------------------------------------*/
void Card::vMarksTestRecursive(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(szMark1()->bSame(MARK_BLANK));
  RASSERT(szMark2()->bSame(MARK_BLANK));

  i8 nSubCards = this->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card *pSubCard = pGetChild(i_Sub);

    pSubCard->vMarksTestRecursive();
  }

}

/*----------------------------------------------------------------------------*/
/* this child has just been added to given parent                             */
/*----------------------------------------------------------------------------*/
void Card::vOnAdd_child(Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);
}

/*----------------------------------------------------------------------------*/
/* this parent has just had given child added to it                           */
/*----------------------------------------------------------------------------*/
void Card::vOnAdd_parent(Card* pChild)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pChild);
}

/*----------------------------------------------------------------------------*/
/* this child has just been removed from given parent                         */
/*----------------------------------------------------------------------------*/
void Card::vOnRemove_child(Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);
}

/*----------------------------------------------------------------------------*/
/* this parent has just had given child fremoved from it                      */
/*----------------------------------------------------------------------------*/
void Card::vOnRemove_parent(Card* pChild)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pChild);
}

/*----------------------------------------------------------------------------*/
/* parent card                                                                */
/*----------------------------------------------------------------------------*/
Card* Card::pParent(void)
{
  return m_pParent;
}

/*----------------------------------------------------------------------------*/
/* parent card                                                                */
/*----------------------------------------------------------------------------*/
void Card::vParent(Card* pParent)
{
  RASSERT(! pParent || ! m_pParent);
  m_pParent = pParent;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card::vPrintRecursive(CardIter* pCI,
                           Ribbon* pRibbon, i8 iDepth, Card* pParent,
                           bool& rbPrune, eAL eLevel,
                           bool bPrintBelow, i8 iXform, Stz* szXfunc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (CK_TERM == m_eCard)
    vDebugPoint();

  if (C_UNKNOWN == m_iFirstPrint) {
    m_iFirstPrint = pCI->m_pBench->nPrintBoard();
    m_eFirst = m_eCard;
  }
  m_nPrintCount++;

  if (0 == iDepth) {
    if (   bIsCard(CK_o_AGGREG)
        || bIsCard(CK_o_PROJECT)) {
      vDebugPoint();
    }

    if (pRibbon->pCardCtl()->pCardHistory())
      pRibbon->pCardCtl()->pCardHistory()->vAddImage(this,iXform,szXfunc);
    vPrintHeader(pRibbon,eLevel);
  }

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCardCtl());

  if (m_pLastPrintParent != pParent) {
    if (bMark1(MARK_BLANK))
      vMark1(MARK_CHANGED);
    m_pLastPrintParent = pParent;
  }

  if (pCardCtl->bPrintByKind(m_eCard))
    vPrintCard_byKind(pRibbon, iDepth);
  else
    vPrintCard_bySettings(pRibbon, iDepth);

  if (this->bMark1(MARK_CHANGED) || this->bMark1(MARK_FOUND))
    vMark1(MARK_BLANK);

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
    pSubCard->vPrintRecursive(pCI,
                              pRibbon,
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vPushChild(Card* pChild)
{
  RASSERT(pChild);

  ChildCards::vPushChild(pChild,this);
}

/*----------------------------------------------------------------------------*/
/* card has been removed correctly                                            */
/*----------------------------------------------------------------------------*/
void Card::vRemovedCorrectly(bool bRemoved)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(!m_pParent)
  m_bRemovedCorrectly = bRemoved;
}

/*----------------------------------------------------------------------------*/
/* renumber subCard's                                                         */
/*----------------------------------------------------------------------------*/
void Card::vRenumberRoot(i8 i_Board)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  m_i_Child = i_Board;

  vCoord(nullptr);

  i8 iSubCardNo = 0;
  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();

    pSubCard->v_Child(iSubCardNo++);
    pSubCard->vRenumberRecursive(this);
  }

  delete pSubCard_I;

  vCoord();
}

/*----------------------------------------------------------------------------*/
/* renumber subCard's                                                         */
/*----------------------------------------------------------------------------*/
void Card::vRenumberRecursive(Card* pParent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(pParent);
  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  vCoord(pParent);

  i8 iSubCardNo = 0;
  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();

    pSubCard->v_Child(iSubCardNo++);
    pSubCard->vRenumberRecursive(this);
  }

  delete pSubCard_I;

  vCoord();
}

/*----------------------------------------------------------------------------*/
/* replace and delete subCard                                                 */
/*----------------------------------------------------------------------------*/
void Card::vReplaceSubDelete(Card* pSub, Card* pWith)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card* pParent = pFindParentRecursive(pSub);
  RASSERT(pSub->pParent() == pParent);
  pParent->vReplaceChild(pSub,pWith);

  DELETE_CARD(pSub);
}

/*----------------------------------------------------------------------------*/
/* replace subCard                                                            */
/*----------------------------------------------------------------------------*/
void Card::vReplaceSub(Card* pSub, Card* pWith)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card* pParent = pFindParentRecursive(pSub);

  pParent->vReplaceChild(pSub,pWith);
}

/*----------------------------------------------------------------------------*/
/* find immediate child-card                                                  */
/*----------------------------------------------------------------------------*/
i8 Card::nSubCards(eCK eSub_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nFound = 0;
  if (eSub_arg == eCard())
    nFound++;

  i8 nChildren = nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChild = pGetChild(i_Child);
    nFound += pChild->nSubCards(eSub_arg);
  }

  return nFound;
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
/* is this card's symbolic value the same as given arg?                       */
/*----------------------------------------------------------------------------*/
bool Card::bSymbolMatches(Card* pCard_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (m_xSymbol.bMatches(pCard_arg->pSymbol()))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does this card have same symbol as given argument?                         */
/*----------------------------------------------------------------------------*/
bool Card::bSymbolMatches(Symbol* pSymbol_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (m_xSymbol.bMatches(pSymbol_arg))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* transfer all child card (incl. sub-cards) from given card set to this card */
/*----------------------------------------------------------------------------*/
void Card::vTransferChildren(Card* pFrom)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  while (Card* pPoppedChild = pFrom->pPopChild())
    vAppendChild(pPoppedChild);
}

/*----------------------------------------------------------------------------*/
/* transfer all child card (incl. sub-cards) from given card set to this card */
/*----------------------------------------------------------------------------*/
void Card::vTransferChildren(Card* pFrom, eSF eFacet_stopAfter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  while (Card *pPoppedChild = pFrom->pPopChild()) {
    vAppendChild(pPoppedChild);

    if (pPoppedChild->pSymbol()->bHasFacet(eFacet_stopAfter))
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* get typeId information                                                     */
/*----------------------------------------------------------------------------*/
TypeId Card::xTypeId(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  TypeId xTypeId = m_xSymbol.xTypeId();

  return xTypeId;
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
    if (!pCard->bSymbolMatches(pSymbol))
      return C_CONTINUE_RECURSION;

    pCard->m_xSymbol.vApplyCastFacetsPpy(pWith);

    bReplaced = true;

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bLambdaWalk(eSub, bfUpdateSymbol);

  return bReplaced;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);
  UNUSED(pParent);

  RASSERT(m_bAddedCorrectly);

  RASSERT(bFixedDepth(iDepth));

  if (m_pParent) {
    if (m_pParent->pFindParentRecursive(this) != m_pParent) {
      pCI->PRINTBOARD_dbg();
      vDump(__func__);
      vValidationFailed("Invalid Parent : %p"_Stz, this);
    }
  }
  else {
    if (CK_DECK != m_eCard) {
      pCI->PRINTBOARD_dbg();
      vDump(__func__);
      vValidationFailed("Missing Parent : %p"_Stz, this);
    }
  }

  if (! m_xSymbol.bValidate()) {
    pCI->PRINTBOARD_dbg();
    vDump(__func__);
    vValidationFailed("Invalid Symbol : %p"_Stz, this);
  }
}

/*----------------------------------------------------------------------------*/
/* validate this card has child cards                                         */
/*----------------------------------------------------------------------------*/
void Card::vValidate_hasChildren(void)
{
  if (! nChildCards()) {
    vDump(__func__,"CARD FAILED: HASCHILDREN"_Stz);
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* validate this card has child cards                                         */
/*----------------------------------------------------------------------------*/
void Card::vValidate_hasType(void)
{
  if (!pSymbol()->bValidType()) {
    vDump(__func__);
    NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* validate set of child cards                                                 */
/*----------------------------------------------------------------------------*/
void Card::vValidate_mayHave(i8 /*eCK*/ iCard_first, ...)
{
  // The first parameter before the varargs (...) must not undergo default
  // argument promotion. This will occur if the enum type is smaller than
  // a 64 bit integer. If that is the case then the result is undefined
  // in va_start(). So let's make the first parameter an integer to avoid this.

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

    va_start(xArgs, iCard_first);
    eCK eCard_mayHave = (eCK) iCard_first;
    while (CK_NONE != eCard_mayHave) {
      if (eCard_test == eCard_mayHave) {
        bMayHave = true;
        break;
      }
      eCard_mayHave = (eCK)va_arg(xArgs, i8);
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
void Card::vValidate_mustHave(i8 /*eCK*/ iCard_first, ...)
{
  // The first parameter before the varargs (...) must not undergo default
  // argument promotion. This will occur if the enum type is smaller than
  // a 64 bit integer. If that is the case then the result is undefined
  // in va_start(). So let's make the first parameter an integer to avoid this.

  va_list xArgs;

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  va_start(xArgs, iCard_first);
  eCK eCard_mustHave = (eCK) iCard_first;
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
      vValidationFailed("vValidate_mustHave: missing %lld %lls"_Stz,(i8)eCard_mustHave,aCK[eCard_mustHave]);
    }

    eCard_mustHave = (eCK)va_arg(xArgs, i8);
  }

  va_end(xArgs);

  delete pSubCard_I;
}

/*----------------------------------------------------------------------------*/
/* validate set of child cards                                                */
/*----------------------------------------------------------------------------*/
eCK Card::eValidate_mustEither(i8 /*eCK*/ iCard_first, ...)
{
  // The first parameter before the varargs (...) must not undergo default
  // argument promotion. This will occur if the enum type is smaller than
  // a 64 bit integer. If that is the case then the result is undefined
  // in va_start(). So let's make the first parameter an integer to avoid this.

  va_list xArgs;

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  i8 nHas = 0;
  eCK eFound = CK_NONE;

  va_start(xArgs, iCard_first);
  eCK eCard_mustHave = (eCK) iCard_first;
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
eCK Card::eValidate_mustSome(i8 /*eCK*/ iCard_first, ...)
{
  // The first parameter before the varargs (...) must not undergo default
  // argument promotion. This will occur if the enum type is smaller than
  // a 64 bit integer. If that is the case then the result is undefined
  // in va_start(). So let's make the first parameter an integer to avoid this.

  va_list xArgs;

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  i8 nHas = 0;
  eCK eFound = CK_NONE;

  va_start(xArgs, iCard_first);
  eCK eCard_mustHave = (eCK) iCard_first;
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

  va_end(xArgs);

  if (! nHas)
    vDump(__func__);
  RASSERT(nHas);

  delete pSubCard_I;

  return eFound;
}

/*----------------------------------------------------------------------------*/
/* validate no child cards                                                    */
/*----------------------------------------------------------------------------*/
void Card::vValidate_noChildren(void)
{
  if (nChildCards()) {
    vDump(__func__,"CARD FAILED: NOCHILDREN"_Stz);
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* validate set of child cards                                                */
/*----------------------------------------------------------------------------*/
void Card::vValidate_notHave(i8 /*eCK*/ iCard, ...)
{
  // The first parameter before the varargs (...) must not undergo default
  // argument promotion. This will occur if the enum type is smaller than
  // a 64 bit integer. If that is the case then the result is undefined
  // in va_start(). So let's make the first parameter an integer to avoid this.

  va_list xArgs;

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  va_start(xArgs, iCard);
  eCK eCard_mustHave = (eCK) iCard;
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
/* validate set of child cards                                                */
/*----------------------------------------------------------------------------*/
eCK Card::eValidate_parentEither(CardIter* pCI, i8 /*eCK*/ iCard_first, ...)
{
  // The first parameter before the varargs (...) must not undergo default
  // argument promotion. This will occur if the enum type is smaller than
  // a 64 bit integer. If that is the case then the result is undefined
  // in va_start(). So let's make the first parameter an integer to avoid this.

  va_list xArgs;

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  eCK eFound = CK_NONE;

  va_start(xArgs, iCard_first);
  eCK eCard_either = (eCK) iCard_first;
  while (CK_NONE != eCard_either) {

    if (Card* p_parent = pCI->pAncestorParent()) {
      if (eCard_either == p_parent->eCard()) {
        eFound = eCard_either;
        break;
      }
    }

    eCard_either = (eCK)va_arg(xArgs, i8);
  }

  if (CK_NONE == eFound) {
    pCI->vAncestorDump(this);
    vValidationFailed("%s"_Stz,__func__);
  }

  va_end(xArgs);

  delete pSubCard_I;

  return eFound;
}

/*----------------------------------------------------------------------------*/
/* validate set of child cards                                                */
/*----------------------------------------------------------------------------*/
void Card::vValidate_parentMust(CardIter* pCI, eCK eParent)
{

  if (CK_NONE == pCI->bAncestor(eParent)) {
    pCI->vAncestorDump(this);
    vValidationFailed("%s"_Stz,__func__);
  }

}

/*----------------------------------------------------------------------------*/
/* validation has failed                                                      */
/*----------------------------------------------------------------------------*/
void Card::vValidationFailed(Stz *szFormat, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  s4K sError;

  va_list xArgs;
  va_start(xArgs,szFormat);
  sError.vSprintVA(szFormat,xArgs);
  va_end(xArgs);

  vDump(__func__,sError);

  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* validation has passed                                                      */
/*----------------------------------------------------------------------------*/
void Card::vValidationPassed(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (bMark1(MARK_VALID))
    vMark1(MARK_DUPLICATE);
  else
  if (bMark1(MARK_UNVALID))
    vMark1(MARK_VALID);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set bearing                                                                */
/*----------------------------------------------------------------------------*/
void Card::vCoord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  StrLib::vConcat(md_Coord,sizeof(md_Coord), "]");
}

/*----------------------------------------------------------------------------*/
/* set bearing                                                                */
/*----------------------------------------------------------------------------*/
void Card::vCoord(Card* pParentCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);


  if (pParentCard) {
    MemLib::vZero(md_Coord, sizeof(md_Coord), CN_CARD);

    StrLib::vSprint(md_Coord, sizeof(md_Coord), nullptr,
                    "%s.%lld"_Stz, pParentCard->md_Coord, m_i_Child);
  }
  else {
    MemLib::vZero(md_Coord, sizeof(md_Coord), CN_CARD);

    StrLib::vSprint(md_Coord, sizeof(md_Coord), nullptr,
                    "SQ%lld"_Stz, m_i_Child);
  }
}

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
/* print card onto given ribbon                                               */
/*----------------------------------------------------------------------------*/
void Card::vPrintCard_byKind(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  switch (m_eCard) {

    case CK_t_EMIT:   _pCard_Emit(this)->vPrint(pRibbon, iDepth);    break;
    case CK_EXPR:     _pCard_Expr(this)->vPrint(pRibbon, iDepth);    break;
    case CK_s_KEY:    _pCard_Key(this)->vPrint(pRibbon, iDepth);     break;
    case CK_RPNCALC:  _pCard_RpnCalc(this)->vPrint(pRibbon, iDepth); break;
    case CK_s_TABLE:  _pCard_Table(this)->vPrint(pRibbon, iDepth);   break;
    case CK_TERM:     _pCard_Term(this)->vPrint(pRibbon, iDepth);    break;
    case CK_o_FILTER: _pCard_Filter(this)->vPrint(pRibbon, iDepth);  break;

    case CK_ANDCONJ:
    case CK_NOTCONJ:
    case CK_ORCONJ:
    case CK_RLNCONJ:
       vPrintCard_bySettings(pRibbon, iDepth);
      break;

    default:
      vPrintCard_bySettings(pRibbon, iDepth);
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* print compressed children                                                  */
/*----------------------------------------------------------------------------*/
void Card::vPrintChildrenShort(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(iDepth);

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
      pRibbon->vSprintSnippet(true,"%s"_Stz,
                              pSub_0->pSymbol()->szFacet((i8)0));
    }
    else {
      Card *pSub_0 = pChild->pGetChild(0);
      Card *pSub_1 = pChild->pGetChild(1);
      pRibbon->vSprintSnippet(true, "%s == %s"_Stz,
                              pSub_0->pSymbol()->szFacet((i8)0),
                              pSub_1->pSymbol()->szFacet((i8)0));
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

  pRibbon->vSprintSnippet(false,"==%02lld %c--%10p--"_Stz,
                           m_i_Child,
                           cLevel,
                           ((char *) this) - (char *) AM::SYSARGS::aRegionAddr());
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

  s32 sName;
  if (0 == iDepth) {
    if (szCustomName) {
      sName.vSprint("%s"_Stz,
                    szCustomName);
    }
    else {
      sName.vSprint("%s%s *%lld:%lld"_Stz,
                    szCardNm(),
                    sHiddenChildren.szStz(),
                    m_nPrintCount,
                    m_iFirstPrint);
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

  pRibbon->vSprintSnippet(false,"%c%c%-18s"_Stz, this->cMark1(), this->cMark2(), sName.szStz());
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
    if (m_xSymbol.bHasFacets())
      sNameSymbol.vSprint("%s%s %s *%lld:%lld"_Stz,
                          sName.szStz(),
                          sHiddenChildren.szStz(),
                          m_xSymbol.szFacet((i8)0),
                          m_nPrintCount,
                          m_iFirstPrint);
    else
      sNameSymbol.vSprint("%s%s *%lld:%lld"_Stz,
                          sName.szStz(),
                          sHiddenChildren.szStz(),
                          m_nPrintCount,
                          m_iFirstPrint);

  }
  else {
    if (pCardCtl->bPrintNo(m_eCard))
      sNameSymbol.vSprint("%s%lld]"_Stz,
                          Stz::szDots(iDepth - 1),
                          m_i_Child);
    if (m_xSymbol.bHasFacets())
      sNameSymbol.vSprintCat("%s%s %s"_Stz,
                             sName.szStz(),
                             sHiddenChildren.szStz(),
                             m_xSymbol.szFacet((i8)0));
    else
      sNameSymbol.vSprintCat("%s%s"_Stz,
                             sName.szStz(),
                             sHiddenChildren.szStz());
  }

  if (sNameSymbol.nLength() <= 18) {
    pRibbon->vSprintSnippet(false,"%c%c%-18s"_Stz, this->cMark1(), this->cMark2(), sNameSymbol.szStz());
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
  UNUSED(iDepth);

  if (! m_xSymbol.bHasProperties())
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
          pRibbon->vSprintSnippet(true,"}%s"_Stz,szDecode);
        else
        if (bPrintPptName)
          pRibbon->vSprintSnippet(true,"}%s=%lld"_Stz, szName, iPpy);
        else
          pRibbon->vSprintSnippet(true,"}%lld"_Stz, iPpy);
        break;
      }

      case TN_STRING: {
        Stz* szName  = pSymbol()->Y_szPpyNm(i_Property);
        Stz* szPpy = pSymbol()->Y_szGet(i_Property);
        if (szPpy->bSame("y"_Stz))
          pRibbon->vSprintSnippet(true,"}%s"_Stz, szName);
        else
        if (bPrintPptName)
          pRibbon->vSprintSnippet(true,"}%s=%s"_Stz, szName, szPpy);
        else
          pRibbon->vSprintSnippet(true,"}%s"_Stz, szPpy);
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
  UNUSED(iDepth);

  if (!m_xSymbol.bHasFacets())
    return;

  if (bPrintMarks)
    pRibbon->vSprintSnippet(true, "%s%s%18s"_Stz,
                            this->szMark1(),
                            this->szMark2(),
                            m_xSymbol.szFacet((i8)0));
  else
    pRibbon->vSprintSnippet(true, "  %s"_Stz, m_xSymbol.szFacet((i8)0));

  if (m_xSymbol.nFacets()) {
    for (i8 i_Facet = 0; i_Facet < m_xSymbol.nFacets(); i_Facet++) {
      Facet* aFacet = m_xSymbol.aFacet(i_Facet);
      pRibbon->vSprintSnippet(true, "%18s~%lld"_Stz, aFacet->m_sFacet.azChar(), i_Facet);
    }
  }
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Direction.hpp"
Card_Direction* Card::pGet_direction_z(i8 i_Element)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Direction* p_direction =
      _pCard_Direction(pGetChild(i_Element));

  return p_direction;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Expr.hpp"
Card_Expr* Card::pGet_expr_z(i8 i_Element)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Expr* p_expr =
      _pCard_Expr(pGetChild(i_Element));

  return p_expr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Term.hpp"
Card_Term* Card::pGet_term_z(i8 i_Element)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term =
      _pCard_Term(pGetChild(i_Element));

  return p_term;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Aggreg.hpp"
Card_Aggreg* Card::pSub_aggreg(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Aggreg* p_aggreg =
      _pCard_Aggreg(pFindSubCard(CK_o_AGGREG));

  if (bNotNull && !p_aggreg) {
    NEVER_REACH
  }

  return p_aggreg;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_BoolPred.hpp"
Card_BoolPred* Card::pSub_boolPred(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_BoolPred* p_boolPred =
      _pCard_BoolPred(pFindSubCard(CK_BOOLPRED));

  if (bNotNull && !p_boolPred) {
    NEVER_REACH
  }

  return p_boolPred;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Bound.hpp"
Card_Bound* Card::pSub_bound(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Bound* p_bound =
      _pCard_Bound(pFindSubCard(CK_BOUND));

  if (bNotNull && !p_bound) {
    NEVER_REACH
  }

  return p_bound;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Bounds.hpp"
Card_Bounds* Card::pSub_bounds(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Bounds* p_bounds =
      _pCard_Bounds(pFindSubCard(CK_BOUNDS));

  if (bNotNull && !p_bounds) {
    NEVER_REACH
  }

  return p_bounds;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Caller.hpp"
Card_Caller* Card::pSub_caller(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Caller* p_caller =
      _pCard_Caller(pFindSubCard(CK_CALLER));

  if (bNotNull && !p_caller) {
    NEVER_REACH
  }

  return p_caller;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_CallOp.hpp"
Card_CallOp* Card::pSub_callOp(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CallOp* p_callOp =
      _pCard_CallOp(pFindSubCard(CK_CALLOP));

  if (bNotNull && !p_callOp) {
    NEVER_REACH
  }

  return p_callOp;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Cast.hpp"
Card_Cast* Card::pSub_cast(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Cast* p_cast =
      _pCard_Cast(pFindSubCard(CK_o_CAST));

  if (bNotNull && !p_cast) {
    NEVER_REACH
  }

  return p_cast;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Clause.hpp"
Card_Clause* Card::pSub_clause(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Clause* p_clause =
      _pCard_Clause(pFindSubCard(CK_CLAUSE));

  if (bNotNull && !p_clause) {
    NEVER_REACH
  }

  return p_clause;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_RlnConj.hpp"
Card_RlnConj* Card::pSub_rlnConj(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_RlnConj* p_rlnConj =
      _pCard_RlnConj(pFindSubCard(CK_RLNCONJ));

  if (bNotNull && !p_rlnConj) {
    NEVER_REACH
  }

  return p_rlnConj;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Constraint.hpp"
Card_Constraint* Card::pSub_constraint(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Constraint* p_constraint =
      _pCard_Constraint(pFindSubCard(CK_s_CONSTRAINT));

  if (bNotNull && !p_constraint) {
    NEVER_REACH
  }

  return p_constraint;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Correlation.hpp"
Card_Correlation* Card::pSub_correlation(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Correlation* p_correlation =
      _pCard_Correlation(pFindSubCard(CK_o_CORRELATION));

  if (bNotNull && !p_correlation) {
    NEVER_REACH
  }

  return p_correlation;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_CostNetwork.hpp"
Card_CostNetwork* Card::pSub_costNetwork(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CostNetwork* p_costNetwork =
      _pCard_CostNetwork(pFindSubCard(CK_COSTNETWORK));

  if (bNotNull && !p_costNetwork) {
    NEVER_REACH
  }

  return p_costNetwork;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_CostJoining.hpp"
Card_CostJoining* Card::pSub_costJoining(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CostJoining* p_costJoining =
      _pCard_CostJoining(pFindSubCard(CK_COSTJOINING));

  if (bNotNull && !p_costJoining) {
    NEVER_REACH
  }

  return p_costJoining;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_CostStream.hpp"
Card_CostStream* Card::pSub_costQuery(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CostStream* p_costQuery =
      _pCard_CostStream(pFindSubCard(CK_COSTSTREAM));

  if (bNotNull && !p_costQuery) {
    NEVER_REACH
  }

  return p_costQuery;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_CostUnicast.hpp"
Card_CostUnicast* Card::pSub_costUnicast(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CostUnicast* p_costUnicast =
      _pCard_CostUnicast(pFindSubCard(CK_COSTUNICAST));

  if (bNotNull && !p_costUnicast) {
    NEVER_REACH
  }

  return p_costUnicast;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Count.hpp"
Card_Count* Card::pSub_count(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Count* p_count =
      _pCard_Count(pFindSubCard(CK_COUNT));

  if (bNotNull && !p_count) {
    NEVER_REACH
  }

  return p_count;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_CorrCols.hpp"
Card_CorrCols* Card::pSub_corrCols(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CorrCols* p_corrCols =
      _pCard_CorrCols(pFindSubCard(CK_CORRCOLS));

  if (bNotNull && !p_corrCols) {
    NEVER_REACH
  }

  return p_corrCols;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_CrossJoin.hpp"
Card_CrossJoin* Card::pSub_crossJoin(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CrossJoin* p_crossJoin =
      _pCard_CrossJoin(pFindSubCard(CK_o_CROSSJOIN));

  if (bNotNull && !p_crossJoin) {
    NEVER_REACH
  }

  return p_crossJoin;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Decision.hpp"
Card_Decision* Card::pSub_decision(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Decision* p_decision =
      _pCard_Decision(pFindSubCard(CK_DECISION));

  if (bNotNull && !p_decision) {
    NEVER_REACH
  }

  return p_decision;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Direction.hpp"
Card_Direction* Card::pSub_direction(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Direction* p_direction =
      _pCard_Direction(pFindSubCard(CK_DIRECTION));

  if (bNotNull && !p_direction) {
    NEVER_REACH
  }

  return p_direction;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Emit.hpp"
Card_Emit* Card::pSub_emit(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Emit* p_emit =
      _pCard_Emit(pFindSubCard(CK_t_EMIT));

  if (bNotNull && !p_emit) {
    NEVER_REACH
  }

  return p_emit;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Expr.hpp"
Card_Expr* Card::pSub_expr(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Expr* p_expr =
      _pCard_Expr(pFindSubCard(CK_EXPR));

  if (bNotNull && !p_expr) {
    NEVER_REACH
  }

  return p_expr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Filter.hpp"
Card_Filter* Card::pSub_filter(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Filter* p_filter =
      _pCard_Filter(pFindSubCard(CK_o_FILTER));

  if (bNotNull && !p_filter) {
    NEVER_REACH
  }

  return p_filter;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_FiltPred.hpp"
Card_FiltPred* Card::pSub_filtPred(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_FiltPred* p_filtPred =
      _pCard_FiltPred(pFindSubCard(CK_FILTPRED));

  if (bNotNull && !p_filtPred) {
    NEVER_REACH
  }

  return p_filtPred;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_FiltPredList.hpp"
Card_FiltPredList* Card::pSub_filtPredList(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_FiltPredList* p_filtPredList =
      _pCard_FiltPredList(pFindSubCard(CK_FILTPREDLIST));

  if (bNotNull && !p_filtPredList) {
    NEVER_REACH
  }

  return p_filtPredList;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Flat.hpp"
Card_Flat* Card::pSub_flat(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Flat* p_flat =
      _pCard_Flat(pFindSubCard(CK_c_FOLD));

  if (bNotNull && !p_flat) {
    NEVER_REACH
  }

  return p_flat;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Fold.hpp"
Card_Fold* Card::pSub_fold(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Fold* p_fold =
      _pCard_Fold(pFindSubCard(CK_c_FOLD));

  if (bNotNull && !p_fold) {
    NEVER_REACH
  }

  return p_fold;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_From.hpp"
Card_From* Card::pSub_from(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_From* p_from =
      _pCard_From(pFindSubCard(CK_o_FROM));

  if (bNotNull && !p_from) {
    NEVER_REACH
  }

  return p_from;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_GbyAgg.hpp"
Card_GbyAgg* Card::pSub_gbyAgg(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_GbyAgg* p_gbyAgg =
      _pCard_GbyAgg(pFindSubCard(CK_o_GBYAGG));

  if (bNotNull && !p_gbyAgg) {
    NEVER_REACH
  }

  return p_gbyAgg;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Hashing.hpp"
Card_Hashing* Card::pSub_hashing(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Hashing* p_hashing =
      _pCard_Hashing(pFindSubCard(CK_s_HASHING));

  if (bNotNull && !p_hashing) {
    NEVER_REACH
  }

  return p_hashing;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_HshCol.hpp"
Card_HshCol* Card::pSub_hshCol(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_HshCol* p_hshCol =
      _pCard_HshCol(pFindSubCard(CK_t_HSHCOL));

  if (bNotNull && !p_hshCol) {
    NEVER_REACH
  }

  return p_hshCol;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_HshColList.hpp"
Card_HshColList* Card::pSub_hshColList(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_HshColList* p_hshColList =
      _pCard_HshColList(pFindSubCard(CK_s_HSHCOLLIST));

  if (bNotNull && !p_hshColList) {
    NEVER_REACH
  }

  return p_hshColList;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_IdxCol.hpp"
Card_IdxCol* Card::pSub_idxCol(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_IdxCol* p_idxCol =
      _pCard_IdxCol(pFindSubCard(CK_IDXCOL));

  if (bNotNull && !p_idxCol) {
    NEVER_REACH
  }

  return p_idxCol;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_IdxColList.hpp"
Card_IdxColList* Card::pSub_idxColList(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_IdxColList* p_idxColList =
      _pCard_IdxColList(pFindSubCard(CK_IDXCOLLIST));

  if (bNotNull && !p_idxColList) {
    NEVER_REACH
  }

  return p_idxColList;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Index.hpp"
Card_Index* Card::pSub_index(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Index* p_index =
      _pCard_Index(pFindSubCard(CK_INDEX));

  if (bNotNull && !p_index) {
    NEVER_REACH
  }

  return p_index;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_IndexJoin.hpp"
Card_IndexJoin* Card::pSub_indexJoin(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_IndexJoin* p_indexJoin =
      _pCard_IndexJoin(pFindSubCard(CK_o_INDEXJOIN));

  if (bNotNull && !p_indexJoin) {
    NEVER_REACH
  }

  return p_indexJoin;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_IndexList.hpp"
Card_IndexList* Card::pSub_indexList(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_IndexList* p_indexList =
      _pCard_IndexList(pFindSubCard(CK_t_INDEXLIST));

  if (bNotNull && !p_indexList) {
    NEVER_REACH
  }

  return p_indexList;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_IndexNm.hpp"
Card_IndexNm* Card::pSub_indexNm(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_IndexNm* p_indexNm =
      _pCard_IndexNm(pFindSubCard(CK_INDEXNM));

  if (bNotNull && !p_indexNm) {
    NEVER_REACH
  }

  return p_indexNm;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Injectable.hpp"
Card_Injectable* Card::pSub_injectable(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Injectable* p_injected =
      _pCard_Injectable(pFindSubCard(CK_s_INJECTABLE));

  if (bNotNull && !p_injected) {
    NEVER_REACH
  }

  return p_injected;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Inflate.hpp"
Card_Inflate* Card::pSub_inflate(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Inflate* p_inflate =
      _pCard_Inflate(pFindSubCard(CK_o_INFLATE));

  if (bNotNull && !p_inflate) {
    NEVER_REACH
  }

  return p_inflate;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_JoinCast.hpp"
Card_JoinCast* Card::pSub_joinCast(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_JoinCast* p_castNext =
      _pCard_JoinCast(pFindSubCard(CK_JOINCAST));

  if (bNotNull && !p_castNext) {
    NEVER_REACH
  }

  return p_castNext;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Key.hpp"
Card_Key* Card::pSub_key(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Key* p_key =
      _pCard_Key(pFindSubCard(CK_s_KEY));

  if (bNotNull && !p_key) {
    NEVER_REACH
  }

  return p_key;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_KeyExists.hpp"
Card_KeyExists* Card::pSub_keyExists(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_KeyExists* p_keyExists =
      _pCard_KeyExists(pFindSubCard(CK_o_KEYEXISTS));

  if (bNotNull && !p_keyExists) {
    NEVER_REACH
  }

  return p_keyExists;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_KeyNotExists.hpp"
Card_KeyNotExists* Card::pSub_keyNotExists(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_KeyNotExists* p_keyNotExists =
      _pCard_KeyNotExists(pFindSubCard(CK_o_KEYNOTEXISTS));

  if (bNotNull && !p_keyNotExists) {
    NEVER_REACH
  }

  return p_keyNotExists;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_JoinEstimate.hpp"
Card_JoinEstimate* Card::pSub_joinEstimate(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_JoinEstimate* p_joinEstimate =
      _pCard_JoinEstimate(pFindSubCard(CK_JOINESTIMATE));

  if (bNotNull && !p_joinEstimate) {
    NEVER_REACH
  }

  return p_joinEstimate;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_JoinOrder.hpp"
Card_JoinOrder* Card::pSub_joinOrder(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_JoinOrder* p_joinOrder =
      _pCard_JoinOrder(pFindSubCard(CK_JOINORDER));

  if (bNotNull && !p_joinOrder) {
    NEVER_REACH
  }

  return p_joinOrder;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_KeyCells.hpp"
Card_KeyCells* Card::pSub_keyCells(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_KeyCells* p_keyCells =
      _pCard_KeyCells(pFindSubCard(CK_KEYCELLS));

  if (bNotNull && !p_keyCells) {
    NEVER_REACH
  }

  return p_keyCells;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Level.hpp"
Card_Level* Card::pSub_level(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Level* p_level =
      _pCard_Level(pFindSubCard(CK_s_LEVEL));

  if (bNotNull && !p_level) {
    NEVER_REACH
  }

  return p_level;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Map.hpp"
Card_Map* Card::pSub_map(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Map* p_map =
      _pCard_Map(pFindSubCard(CK_MAP));

  if (bNotNull && !p_map) {
    NEVER_REACH
  }

  return p_map;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Mapping.hpp"
Card_Mapping* Card::pSub_mapping(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Mapping* p_mapping =
      _pCard_Mapping(pFindSubCard(CK_s_MAPPING));

  if (bNotNull && !p_mapping) {
    NEVER_REACH
  }

  return p_mapping;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Needed.hpp"
Card_Needed* Card::pSub_needed(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Needed* p_needed =
      _pCard_Needed(pFindSubCard(CK_s_NEEDED));

  if (bNotNull && !p_needed) {
    NEVER_REACH
  }

  return p_needed;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Note.hpp"
Card_Note* Card::pSub_note(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Note* p_note =
      _pCard_Note(pFindSubCard(CK_NOTE));

  if (bNotNull && !p_note) {
    NEVER_REACH
  }

  return p_note;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_NotConj.hpp"
Card_NotConj* Card::pSub_notConj(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_NotConj* p_notPred =
      _pCard_NotConj(pFindSubCard(CK_NOTCONJ));

  if (bNotNull && !p_notPred) {
    NEVER_REACH
  }

  return p_notPred;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_On.hpp"
Card_On* Card::pSub_on(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_On* p_on =
      _pCard_On(pFindSubCard(CK_ON));

  if (bNotNull && !p_on) {
    NEVER_REACH
  }

  return p_on;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_OuterColNm.hpp"
Card_OuterColNm* Card::pSub_outerColNm(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_OuterColNm* p_outerColNm =
      _pCard_OuterColNm(pFindSubCard(CK_OUTERCOLNM));

  if (bNotNull && !p_outerColNm) {
    NEVER_REACH
  }

  return p_outerColNm;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_OuterJoin.hpp"
Card_OuterJoin* Card::pSub_outerJoin(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_OuterJoin* p_outerJoin =
      _pCard_OuterJoin(pFindSubCard(CK_OUTERJOIN));

  if (bNotNull && !p_outerJoin) {
    NEVER_REACH
  }

  return p_outerJoin;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_PayloadCells.hpp"
Card_PayloadCells* Card::pSub_payloadCells(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_PayloadCells* p_payloadCells =
      _pCard_PayloadCells(pFindSubCard(CK_PAYLOADCELLS));

  if (bNotNull && !p_payloadCells) {
    NEVER_REACH
  }

  return p_payloadCells;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Required.hpp"
Card_Required* Card::pSub_required(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Required* p_required =
      _pCard_Required(pFindSubCard(CK_REQUIRED));

  if (bNotNull && !p_required) {
    NEVER_REACH
  }

  return p_required;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_RowCount.hpp"
Card_RowCount* Card::pSub_rowCount(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_RowCount* p_rowCount =
      _pCard_RowCount(pFindSubCard(CK_ROWCOUNT));

  if (bNotNull && !p_rowCount) {
    NEVER_REACH
  }

  return p_rowCount;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_RowSize.hpp"
Card_RowSize* Card::pSub_rowSize(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_RowSize* p_rowSize =
      _pCard_RowSize(pFindSubCard(CK_ROWSIZE));

  if (bNotNull && !p_rowSize) {
    NEVER_REACH
  }

  return p_rowSize;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_RpnCalc.hpp"
Card_RpnCalc* Card::pSub_rpnCalc(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_RpnCalc* p_rpnCalc =
      _pCard_RpnCalc(pFindSubCard(CK_RPNCALC));

  if (bNotNull && !p_rpnCalc) {
    NEVER_REACH
  }

  return p_rpnCalc;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Scan.hpp"
Card_Scan* Card::pSub_scan(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Scan* p_scan =
      _pCard_Scan(pFindSubCard(CK_o_SCAN));

  if (bNotNull && !p_scan) {
    NEVER_REACH
  }

  return p_scan;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Section.hpp"
Card_Section* Card::pSub_section(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Section* p_section =
      _pCard_Section(pFindSubCard(CK_SECTION));

  if (bNotNull && !p_section) {
    NEVER_REACH
  }

  return p_section;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Spare.hpp"
Card_Spare* Card::pSub_spare(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Spare* p_spare =
      _pCard_Spare(pFindSubCard(CK_SPARE));

  if (bNotNull && !p_spare) {
    NEVER_REACH
  }

  return p_spare;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_SqyCalls.hpp"
Card_SqyCalls* Card::pSub_sqyCalls(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_SqyCalls* p_sqyCalls =
      _pCard_SqyCalls(pFindSubCard(CK_SQYCALLS));

  if (bNotNull && !p_sqyCalls) {
    NEVER_REACH
  }

  return p_sqyCalls;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_SqyInfo.hpp"
Card_SqyInfo* Card::pSub_sqyInfo(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_SqyInfo* p_sqyInfo =
      _pCard_SqyInfo(pFindSubCard(CK_SQYINFO));

  if (bNotNull && !p_sqyInfo) {
    NEVER_REACH
  }

  return p_sqyInfo;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_SqyNo.hpp"
Card_SqyNo* Card::pSub_sqyNo(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_SqyNo* p_sqyNo =
      _pCard_SqyNo(pFindSubCard(CK_SQYNO));

  if (bNotNull && !p_sqyNo) {
    NEVER_REACH
  }

  return p_sqyNo;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_IdxCol.hpp"
Card_Stage* Card::pSub_stage(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Stage* p_stage =
      _pCard_Stage(pFindSubCard(CK_IDXCOL));

  if (bNotNull && !p_stage) {
    NEVER_REACH
  }

  return p_stage;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Stream.hpp"
Card_Stream* Card::pSub_stream(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Stream* p_stream =
      _pCard_Stream(pFindSubCard(CK_s_STREAM));

  if (bNotNull && !p_stream) {
    NEVER_REACH
  }

  return p_stream;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_StreamHead.hpp"
Card_StreamHead* Card::pSub_streamHead(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StreamHead* p_streamHead =
      _pCard_StreamHead(pFindSubCard(CK_STREAMHEAD));

  if (bNotNull && !p_streamHead) {
    NEVER_REACH
  }

  return p_streamHead;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_StreamJoin.hpp"
Card_StreamJoin* Card::pSub_streamJoin(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StreamJoin* p_streamJoin =
      _pCard_StreamJoin(pFindSubCard(CK_STREAMJOIN));

  if (bNotNull && !p_streamJoin) {
    NEVER_REACH
  }

  return p_streamJoin;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_StreamKey.hpp"
Card_StreamKey* Card::pSub_streamKey(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StreamKey* p_streamKey =
      _pCard_StreamKey(pFindSubCard(CK_STREAMKEY));

  if (bNotNull && !p_streamKey) {
    NEVER_REACH
  }

  return p_streamKey;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_StreamPer.hpp"
Card_StreamPer* Card::pSub_streamPer(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StreamPer* p_streamPer =
      _pCard_StreamPer(pFindSubCard(CK_STREAMPER));

  if (bNotNull && !p_streamPer) {
    NEVER_REACH
  }

  return p_streamPer;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_StreamPtn.hpp"
Card_StreamPtn* Card::pSub_streamPtn(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StreamPtn* p_streamPtn =
      _pCard_StreamPtn(pFindSubCard(CK_STREAMPTN));

  if (bNotNull && !p_streamPtn) {
    NEVER_REACH
  }

  return p_streamPtn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_SubQuery.hpp"
Card_SubQuery* Card::pSub_subQuery(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_SubQuery* p_subQuery =
      _pCard_SubQuery(pFindSubCard(CK_SUBQUERY));

  if (bNotNull && !p_subQuery) {
    NEVER_REACH
  }

  return p_subQuery;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Synthesis.hpp"
Card_Synthesis* Card::pSub_synthesis(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Synthesis* pSub_synthesis =
      _pCard_Synthesis(pFindSubCard(CK_SYNTHESIS));

  if (bNotNull && !pSub_synthesis) {
    NEVER_REACH
  }

  return pSub_synthesis;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Table.hpp"
Card_Table* Card::pSub_table(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Table* p_table =
      _pCard_Table(pFindSubCard(CK_s_TABLE));

  if (bNotNull && !p_table) {
    NEVER_REACH
  }

  return p_table;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_TableInfo.hpp"
Card_TableInfo* Card::pSub_tableInfo(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_TableInfo* p_tableInfo =
      _pCard_TableInfo(pFindSubCard(CK_t_TABLEINFO));

  if (bNotNull && !p_tableInfo) {
    NEVER_REACH
  }

  return p_tableInfo;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_TableNm.hpp"
Card_TableNm* Card::pSub_tableNm(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_TableNm* p_tableNm =
      _pCard_TableNm(pFindSubCard(CK_t_TABLENM));

  if (bNotNull && !p_tableNm) {
    NEVER_REACH
  }

  return p_tableNm;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_TablePtn.hpp"
Card_TablePtn* Card::pSub_tablePtn(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_TablePtn* p_tablePtn =
      _pCard_TablePtn(pFindSubCard(CK_TABLEPTN));

  if (bNotNull && !p_tablePtn) {
    NEVER_REACH
  }

  return p_tablePtn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_TableSize.hpp"
Card_TableSize* Card::pSub_tableSize(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_TableSize* p_tableSize =
      _pCard_TableSize(pFindSubCard(CK_TABLESIZE));

  if (bNotNull && !p_tableSize) {
    NEVER_REACH
  }

  return p_tableSize;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_TblSyn.hpp"
Card_TblSyn* Card::pSub_tblSyn(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_TblSyn* p_tblSyn =
      _pCard_TblSyn(pFindSubCard(CK_TBLSYN));

  if (bNotNull && !p_tblSyn) {
    NEVER_REACH
  }

  return p_tblSyn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Term.hpp"
Card_Term* Card::pSub_term(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term =
      _pCard_Term(pFindSubCard(CK_TERM));

  if (bNotNull && !p_term) {
    NEVER_REACH
  }

  return p_term;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Top.hpp"
Card_Top* Card::pSub_top(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Top* p_top =
      _pCard_Top(pFindSubCard(CK_o_TOP));

  if (bNotNull && !p_top) {
    NEVER_REACH
  }

  return p_top;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Topology.hpp"
Card_Topology* Card::pSub_topology(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Topology* p_topology =
      _pCard_Topology(pFindSubCard(CK_TOPOLOGY));

  if (bNotNull && !p_topology) {
    NEVER_REACH
  }

  return p_topology;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Unique.hpp"
Card_Unique* Card::pSub_unique(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Unique* p_unique =
      _pCard_Unique(pFindSubCard(CK_UNIQUE));

  if (bNotNull && !p_unique) {
    NEVER_REACH
  }

  return p_unique;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_Where.hpp"
Card_Where* Card::pSub_where(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Where* p_where =
      _pCard_Where(pFindSubCard(CK_o_WHERE));

  if (bNotNull && !p_where) {
    NEVER_REACH
  }

  return p_where;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_XCard.hpp"
Card_XCard* Card::pSub_xcard(bool bNotNull)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_XCard* p_xcard =
      _pCard_XCard(pFindSubCard(CK_XCARD));

  if (bNotNull && !p_xcard) {
    NEVER_REACH
  }

  return p_xcard;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
