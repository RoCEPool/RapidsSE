

#include "Card.hpp"
#include "CardCtl.hpp"
#include "Board.hpp"
#include "PtnDef.hpp"

#include "eST.h"
#include "eXK.h"


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardSet::CardSet(void)
{
  //_precondition_(_ADDRESS_,_FOOTER_);
          
  //_postcondition_(_ADDRESS_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CardSet::~CardSet(void) noexcept(false)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  for (Card* pCard = pPop(); pCard; pCard = pPop())
    delete pCard;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardSet::pGetChild(i8 i_Child)
{
  Card* pCard = pGet(i_Child);
  return pCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardSet::pGetChildCard(i8 i_Child, eCK& reCard)
{
  Card* pCard = pGet(i_Child);
  
  if (pCard)
    reCard = pCard->eCard();
  else
    reCard = CK_NONE;
  
  return pCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardSet::vRemoveSubCards(eCK eCard)
{
  //_precondition_(_ADDRESS_,_FOOTER_);
  
  i8 nSubCards = this->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pSubCard = pGetChild(i_Child);
    if (! pSubCard->bIsCard(eCard))
      continue;
    
    vRemove(pSubCard);
    i_Child--;
    nSubCards--;
  }
}



/*============================================================================*/
/* Card.cpp -- Abstract Card                                                  */
/*============================================================================*/

Card* gapCard[C_10K];

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Card,CN_CARD)
__aNewFunction(Card,CN_CARD)
__vDeleteFunction(Card,CN_CARD)
__aFooterFunction(Card)

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
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void Card::vInit(eCK eCard)
{
  static i8 iSeqNo = 0;

  MemLib::vZero(m_caRef, sizeof(m_caRef), CN_CARD);

  m_eCard = eCard;
  m_iCardNo = C_UNKNOWN;

  m_iSeqNo = iSeqNo++;

  m_cMark1 = ' ';
  m_cMark2 = ' ';

  m_pProperties = nullptr;

  m_bPrintNm = true;

  gapCard[m_iSeqNo] = this;

//  if (35 == m_iSeqNo) {
//    vTrace("Card [%02lld] %p\n"_Stz, m_iSeqNo, this);
//    vDebugPoint();
//  }
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card::~Card(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //vTrace("Card [~%02lld] %p\n"_Stz,m_iSeqNo,this);
  gapCard[m_iSeqNo] = nullptr;

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
      vTrace("Card Leak ~%02lld %p\n"_Stz, i_Card, pLeaked);
      vDebugPoint();
    }
  }
}

/*----------------------------------------------------------------------------*/
/* are the two cards recursively the same                                     */
/*----------------------------------------------------------------------------*/
bool Card::bCardsSameRecursive(Card* pCard_a, Card* pCard_b, i8 iDepth, bool bTestL0)
{
  _static_

  if (iDepth || (0 == iDepth && bTestL0)) {

    eCK eCard_a = pCard_a->eCard();
    eCK eCard_b = pCard_b->eCard();

    if (eCard_a != eCard_b)
      return false;

    if (CK_TERM == eCard_a)
      if (!pCard_a->sz()->bSame(pCard_a->sz()))
        return false;
  }

  i8 nChildren_a = pCard_a->nChildCards();
  i8 nChildren_b = pCard_b->nChildCards();
  if (nChildren_a != nChildren_b)
    return false;

  for (i8 i_Child = 0; i_Child < nChildren_a; i_Child++) {
    Card* pChild_a = pCard_a->pGetChild(i_Child);
    Card* pChild_b = pCard_b->pGetChild(i_Child);
    if (! bCardsSameRecursive(pChild_a,pChild_b,iDepth,bTestL0))
      return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* find eCK by card name                                                      */
/*----------------------------------------------------------------------------*/
eCK Card::eToECard(Stz* szCardNm)
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::vAddProperty_nameOnly(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_)

  if (! m_pProperties)
    m_pProperties = new TagSet;
  
  m_pProperties->vReplace(szName,"yes"_Stz);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::vAddProperty_int(Stz* szName, i8 iValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;
  
  m_pProperties->vReplace(szName,iValue);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::vAddProperty_ptr(Stz* szName, Object* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;
  
  m_pProperties->vReplace(szName,pValue);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::vAddProperty_stz(Stz* szName, Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;
  
  m_pProperties->vReplace(szName,szValue);
}

/*----------------------------------------------------------------------------*/
/* duplicate the sub cards                                                    */
/*----------------------------------------------------------------------------*/
void Card::vAppendDupSubCards(Card* pCard_SubSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nSubCards = pCard_SubSet->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pSubCard = pCard_SubSet->pGetChild(i_Child);

    Card* pSubCard_dup = pSubCard->pDupCardAndSubCards();

    if (pSubCard->m_pProperties) {
      pSubCard_dup->vPropertiesEnsure();
      pSubCard_dup->pProperties()->vCopy(pSubCard->m_pProperties);
    }

    this->vAppend(pSubCard_dup);
  }
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
/* get number of child cards                                                  */
/*----------------------------------------------------------------------------*/
i8 Card::nChildCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = nItems();

  return nChildren;
}

/*----------------------------------------------------------------------------*/
/* get column ordinal of column ref                                           */
/*----------------------------------------------------------------------------*/
i8 Card::iColumnRef_columnOrd(void)
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
char Card::cColumnRef_tableSyn(void)
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
void Card::vCutSubCard(Card* pSubCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  this->vRemove(pSubCard);
}

/*----------------------------------------------------------------------------*/
/* delete child-card                                                          */
/*----------------------------------------------------------------------------*/
void Card::vDeleteChildCard(eCK eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i_Child;
  Card* pChildCard = pFindChildCard(eCard,i_Child);

  if (pChildCard) {
    vRemoveChild(pChildCard);
    delete pChildCard;
  }
}

/*----------------------------------------------------------------------------*/
/* delete given property                                                      */
/*----------------------------------------------------------------------------*/
void Card::vDeleteProperty(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return;

  m_pProperties->vDelete(szName);
}

/*----------------------------------------------------------------------------*/
/* duplicate this card and its sub cards                                      */
/*----------------------------------------------------------------------------*/
void Card::vDeletePropertyRecursively(Stz* szName)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vDeleteProperty(szName);

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pChildCard = pGetChild(i_Child);
    pChildCard->vDeletePropertyRecursively(szName);
  }
}

/*----------------------------------------------------------------------------*/
/* dum card and its sub cards                                                 */
/*----------------------------------------------------------------------------*/
void Card::vDump(i8 iDepth, Stz* szTitle)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (szTitle)
    vTrace("%s:\n"_Stz,szTitle);

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
        ,Stz::szTab(iDepth)
        ,m_caRef[0] ? m_caRef : (char*)aCK[m_eCard]
        ,szValue()
        ,sProperties.szStz()
    );
  }
  else {
    vTrace("%s%s { %s }\n"_Stz
        ,Stz::szTab(iDepth)
        ,m_caRef[0] ? m_caRef : (char*)aCK[m_eCard]
        ,sProperties.szStz()
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

  if (m_pProperties) {
    pCard_dup->vPropertiesEnsure();
    pCard_dup->pProperties()->vCopy(m_pProperties);
  }

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pSubCard = pGetChild(i_Child);
    
    Card* pSubCard_dup = pSubCard->pDupCardAndSubCards();
    
    if (pSubCard->m_pProperties) {
      pSubCard_dup->vPropertiesEnsure();
      pSubCard_dup->pProperties()->vCopy(pSubCard->m_pProperties);
    }
    
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
    
    if (pSubCard->m_pProperties) {
      pSubCard_dup->vPropertiesEnsure();
      pSubCard_dup->pProperties()->vCopy(pSubCard->m_pProperties);
    }
    
    pCard_SubSet->vAppend(pSubCard_dup);
  }
  
  return pCard_SubSet;
}

/*----------------------------------------------------------------------------*/
/* call closure for each sub-card                                             */
/*----------------------------------------------------------------------------*/
void Card::vEachSubCard(std::function<void(Card* pCard)> f_vClosure,
                        eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  ListExoIter2_t<Card>* pCard_I = pNewIter();

  while (pCard_I->bHasNext()) {
    Card* pSubCard = pCard_I->pNext();
    
    if (CK_NONE == eCard || eCard == pSubCard->eCard())
      f_vClosure(pSubCard);
  }
  
  delete pCard_I;
}

/*----------------------------------------------------------------------------*/
/* is this card equivalent to given arg?                                      */
/*----------------------------------------------------------------------------*/
bool Card::bEquivalent(Card* pCard_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  TagSet* pProperties_arg = pCard_arg->pProperties();
  if (m_pProperties) {
    if (pProperties_arg) {
      if (! m_pProperties->bSame(pProperties_arg))
        return false;
    }
    else {
      return false;
    }
  }
  else
  if (pProperties_arg) {
    return false;
  }
  else {
    RASSERT(! m_pProperties);
    RASSERT(! pProperties_arg);
  }
  
  eCK eSubCard;
  i8 nSubCards = this->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card* pSubCard = pGetChildCard(i_Sub, eSubCard);
    
    Card* pSubCard_arg = pCard_arg->pFindSubCard(eSubCard);
    if (! pSubCard_arg)
      return false;

    if (! pSubCard->bEquivalent(pSubCard_arg))
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* find immediate child-card                                                  */
/*----------------------------------------------------------------------------*/
Card* Card::pFindChildCard(eCK eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i_Child;

  Card* pFoundCard = pFindChildCard(eCard,i_Child);

  return pFoundCard;
}

/*----------------------------------------------------------------------------*/
/* find immediate child-card                                                  */
/*----------------------------------------------------------------------------*/
Card* Card::pFindChildCard(eCK eCard, i8& i_Child)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card* pFoundCard = nullptr;

  eCK eSubCard;
  i8 nSubCards = this->nChildCards();
  for (i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pSubCard = pGetChildCard(i_Child, eSubCard);

    if (eCard == pSubCard->eCard()) {
      pFoundCard = pSubCard;
      break;
    }
  }

  return pFoundCard;
}

/*----------------------------------------------------------------------------*/
/* find parent of given arg                                                   */
/*----------------------------------------------------------------------------*/
Card* Card::pFindParent(Card* pSubCard_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 nSubCards = this->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card* pSubCard = pGetChild(i_Sub);
    
    if (pSubCard_arg == pSubCard)
      return this;
    
    Card* pParent = pSubCard->pFindParent(pSubCard_arg);
    if (pParent)
      return pParent;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find sub-card                                                              */
/*----------------------------------------------------------------------------*/
Card* Card::pFindSubCard(eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();
  
  Card* pFoundCard = nullptr;
  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();
    
    if (eCard == pSubCard->eCard()) {
      pFoundCard = pSubCard;
      break;
    }
    
    pFoundCard = pSubCard->pFindSubCard(eCard);
    if (pFoundCard)
      break;
  }
  
  delete pSubCard_I;
  
  return pFoundCard;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
i8 Card::iGetProperty(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return C_UNKNOWN;
  
  i8 iValue = m_pProperties->iValue(szName);
  
  return iValue;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
i8 Card::iGetProperty(i8 i_Property)
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
Object* Card::pGetProperty(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return nullptr;
  
  Object* pValue = m_pProperties->pValue(szName);
  
  return pValue;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
Object* Card::pGetProperty(i8 i_Property)
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
Stz* Card::szGetProperty(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return nullptr;
  
  Stz* szValue = m_pProperties->szValue(szName);
  
  return szValue;
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
Stz* Card::szGetProperty(i8 i_Property)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return nullptr;
  
  Stz* szValue = m_pProperties->szValue(i_Property);
  
  return szValue;
}

/*----------------------------------------------------------------------------*/
/* get given property name                                                    */
/*----------------------------------------------------------------------------*/
Stz* Card::szGetPropertyNm(i8 i_Property)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return nullptr;
  
  Stz* szName = m_pProperties->szName(i_Property);
  
  return szName;
}

/*----------------------------------------------------------------------------*/
/* does card have the EBound property ?                                       */
/*----------------------------------------------------------------------------*/
bool Card::bHasEBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return false;

  bool bHas = m_pProperties->bHasName("EBound"_Stz);

  return bHas;
}

/*----------------------------------------------------------------------------*/
/* set/reset the EBound property                                              */
/*----------------------------------------------------------------------------*/
void Card::vHasEBound(bool bHasEBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bHasEBound) {
    vAddProperty_nameOnly("EBound"_Stz);
    return;
  }

  vDeleteProperty("EBound"_Stz);
}

/*----------------------------------------------------------------------------*/
/* does card have the LBound property ?                                       */
/*----------------------------------------------------------------------------*/
bool Card::bHasLBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return false;

  bool bHas = m_pProperties->bHasName("LBound"_Stz);

  return bHas;
}

/*----------------------------------------------------------------------------*/
/* set/reset the LBound property                                              */
/*----------------------------------------------------------------------------*/
void Card::vHasLBound(bool bHasLBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bHasLBound) {
    vAddProperty_nameOnly("LBound"_Stz);
    return;
  }

  vDeleteProperty("LBound"_Stz);
}

/*----------------------------------------------------------------------------*/
/* is given card a sub card of this card?                                     */
/*----------------------------------------------------------------------------*/
bool Card::bHasSubCard(Card* pGivenCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pGetChild(i_Child);

    if (pSubCard == pGivenCard)
      return true;

    if (pSubCard->bHasSubCard(pGivenCard))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* is given card a sub card of this card?                                     */
/*----------------------------------------------------------------------------*/
bool Card::bHasSubCard(Card* pGivenCard, Card*& rpParentCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pGetChild(i_Child);

    if (pSubCard == pGivenCard) {
      rpParentCard = this;
      return true;
    }

    if (pSubCard->bHasSubCard(pGivenCard,rpParentCard))
      return true;
  }

  rpParentCard = nullptr;
  return false;
}

/*----------------------------------------------------------------------------*/
/* does card have given property ?                                            */
/*----------------------------------------------------------------------------*/
bool Card::bHasProperty(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return false;

  bool bHas = m_pProperties->bHasName(szName);

  return bHas;
}

/*----------------------------------------------------------------------------*/
/* does card have the UBound property ?                                       */
/*----------------------------------------------------------------------------*/
bool Card::bHasUBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return false;

  bool bHas = m_pProperties->bHasName("UBound"_Stz);

  return bHas;
}

/*----------------------------------------------------------------------------*/
/* set/reset the UBound property                                              */
/*----------------------------------------------------------------------------*/
void Card::vHasUBound(bool bHasUBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (bHasUBound) {
    vAddProperty_nameOnly("UBound"_Stz);
    return;
  }

  vDeleteProperty("UBound"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card::vPrintCard(Ribbon* pRibbon, i8 iDepth, bool& rbPrune)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (0 == iDepth)
    vPrintHeader(pRibbon);

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCtl());

  eCP ePrintForm = pCardCtl->ePrintForm(m_eCard);

  switch (ePrintForm) {
    case CP_DEFAULT:
      vPrintCard_default(pRibbon, iDepth,rbPrune, ePrintForm);
      break;
    case CP_CARDFORM:
      vPrintCard_byForm(pRibbon, iDepth,rbPrune, ePrintForm);
      break;
    case CP_COMPRESSED:
      vPrintCard_compressed(pRibbon, iDepth,rbPrune, ePrintForm);
      break;
    default:
      NEVER_REACH
  }

  //vMark2(' ');

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (rbPrune)
    return;

  if (! pCardCtl->bPrintSubCards(m_eCard))
    return;

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();
  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();
    pSubCard->vPrintCard(pRibbon, iDepth + 1, rbPrune);
  }

  delete pSubCard_I;
}

/*----------------------------------------------------------------------------*/
/* print card onto given ribbon                                               */
/*----------------------------------------------------------------------------*/
void Card::vPrintCard_byForm(Ribbon* pRibbon, i8 iDepth, bool& rbPrune, eCP ePinForme)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);


  switch (m_eCard) {

    case CK_PREDAND:
      _pCard_PredAnd(this)->vPrintCardName(pRibbon, iDepth, rbPrune);
      break;

    case CK_PREDCMP:
      _pCard_PredCmp(this)->vPrintCardName(pRibbon, iDepth, rbPrune);
      break;

    case CK_PREDOR:
      _pCard_PredOr(this)->vPrintCardName(pRibbon, iDepth, rbPrune);
      break;

    case CK_STREAM:
      _pCard_Stream(this)->vRibbonPrint(pRibbon,iDepth);
      break;

    case CK_TERM:
      _pCard_Term(this)->vPrintCardProperties(pRibbon, iDepth, rbPrune, false);
      break;

    default:
      vTrace("SWITCH %s: Missing case for eCard %lld %s\n"_Stz
          ,__func__
          ,(i8)m_eCard
          ,aCK[m_eCard]
      );
      RASSERT(0);
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* print card onto given ribbon                                               */
/*----------------------------------------------------------------------------*/
void Card::vPrintCard_compressed(Ribbon* pRibbon, i8 iDepth, bool& rbPrune, eCP ePinForm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  eCK eCard = this->eCard();

  switch (eCard) {
    case CK_COLUMNREF:
    case CK_COMMENT:
    case CK_PREDCMP:
    case CK_TERM:
      if (*sz()->azChar())
        pRibbon->vSprintRight("%s"_Stz,sz());
      break;

    case CK_COPTNJOIN:
    case CK_CROSSPTNJOIN:
      vPrintCardName(pRibbon, iDepth, rbPrune);
      vPrintCardProperties(pRibbon, iDepth, rbPrune);
      vPrintCompressed_childPairs(pRibbon, iDepth, rbPrune);
      rbPrune = true;
      break;

    default:
      NEVER_REACH;
      break;
  }

}


/*----------------------------------------------------------------------------*/
/* print card onto given ribbon                                               */
/*----------------------------------------------------------------------------*/
void Card::vPrintCard_default(Ribbon* pRibbon, i8 iDepth, bool& rbPrune, eCP ePinForm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  eCK eCard = this->eCard();

  switch (eCard) {
    case CK_COMMENT:
      vPrintCardDetails(pRibbon, iDepth, rbPrune);
      break;

    case CK_TERM:
      vPrintCardName(pRibbon, iDepth, rbPrune);
      vPrintCardDetails(pRibbon, iDepth, rbPrune);
      break;

    default:
      vPrintCardName(pRibbon, iDepth, rbPrune);
      vPrintCardValue(pRibbon, iDepth, rbPrune);
      vPrintCardDetails(pRibbon, iDepth, rbPrune);
      break;
  }

}

/*----------------------------------------------------------------------------*/
/* print card onto given ribbon                                               */
/*----------------------------------------------------------------------------*/
void Card::vPrintCardDetails(Ribbon* pRibbon, i8 iDepth, bool& rbPrune)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eCard = this->eCard();

  switch (eCard) {

    case CK_CARDPTR:     vCardPrint     (pRibbon, iDepth, rbPrune); break;
    case CK_COMMENT:     vCommentPrint  (pRibbon, iDepth); break;
    case CK_INDEXPTR:    vIndexPrint    (pRibbon, iDepth); break;
    case CK_TABLEPTR:    vTablePrint    (pRibbon, iDepth); break;
    case CK_TERM:        vPrintCardValue(pRibbon, iDepth, rbPrune); break;

    case CK_STREAMELIST: vStreamElistPrint(pRibbon,iDepth);
      break;

    default:
      vPrintCardProperties(pRibbon, iDepth, rbPrune);
      break;
  }

}

/*----------------------------------------------------------------------------*/
/* print compressed children                                                  */
/*----------------------------------------------------------------------------*/
void Card::vPrintCompressed_childPairs(Ribbon* pRibbon, i8 iDepth, bool& rbPrune)
{
  _static_

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
  _static_

  pRibbon->vSprint("--%02lld--------------=="_Stz
      ,m_iCardNo
  );
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card::vPrintCardName(Ribbon* pRibbon, i8 iDepth, bool& rbPrune)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_bPrintNm)
    return;

  s32 sName;

  Board* pBoard = _pBoard(pRibbon->pSurface());
  CardRegs* pCardRegs = pBoard->pCardRegs();

  if (0 == iDepth)
    sName.vSprint("%s%s x%lld"_Stz,
                  pCardRegs->bInCardReg(this) ? "R "_Stz : ""_Stz,
                  szCardNm(),
                  nItems());
  else
    sName.vSprint("%s%lld]%s%s"_Stz,
                  Stz::szDots(iDepth-1),
                  m_iCardNo,
                  pCardRegs->bInCardReg(this) ? "R "_Stz : ""_Stz,
                  szCardNm());

  pRibbon->vSprint("%-18s%c%c"_Stz, sName.szStz(), m_cMark1, m_cMark2);
}

/*----------------------------------------------------------------------------*/
/* print by piece                                                             */
/*----------------------------------------------------------------------------*/
void Card::vPrintCardProperties(Ribbon* pRibbon, i8 iDepth, bool& rbPrune, bool bPrintPtyName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return;

  i8 nProperties = this->nProperties();

  for (i8 i_Property = 0; i_Property < nProperties; i_Property++) {

    eTN eTkn = eProperty(i_Property);

    switch (eTkn) {

      case TN_INTEGER: {
        Stz* szName = szGetPropertyNm(i_Property);
        i8   iValue = iGetProperty(i_Property);

        Stz* szDecode = szDecodeEnum(szName,iValue);
        if (szDecode)
          pRibbon->vSprintRight("%s"_Stz,szDecode);
        else
        if (bPrintPtyName)
          pRibbon->vSprintRight("%s=%lld"_Stz,szName,iValue);
        else
          pRibbon->vSprintRight("%lld"_Stz,iValue);
        break;
      }

      case TN_OBJECT: {
        Stz* szName = szGetPropertyNm(i_Property);
        Object* pObject = pGetProperty(i_Property);
        pRibbon->vSprintRight("%s->  "_Stz,szName);
        pRibbon->vSprintRight("%p"_Stz,pObject);
        pRibbon->vSprintRight("%s"_Stz,aCN[pObject->eGetClassNo()]);
        break;
      }

      case TN_STRING: {
        Stz* szName  = szGetPropertyNm(i_Property);
        Stz* szValue = szGetProperty(i_Property);
        if (bPrintPtyName)
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
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card::vPrintCardValue(Ribbon* pRibbon, i8 iDepth, bool& rbPrune)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! *sz()->azChar())
    return;

  pRibbon->vSprintRight("%s"_Stz,szValue());

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
/* get properties                                                             */
/*----------------------------------------------------------------------------*/
TagSet* Card::pProperties(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_pProperties;
}

/*----------------------------------------------------------------------------*/
/* ensure this card has a m_pProperties                                       */
/*----------------------------------------------------------------------------*/
void Card::vPropertiesEnsure(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;
}

/*----------------------------------------------------------------------------*/
/* get name of property                                                       */
/*----------------------------------------------------------------------------*/
Stz* Card::szPropertyNm(i8 i_Property)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return nullptr;
  
  Stz* szName = m_pProperties->szName(i_Property);
  
  return szName;
}

/*----------------------------------------------------------------------------*/
/* remove given child card (non-recursive)                                    */
/*----------------------------------------------------------------------------*/
void Card::vRemoveChild(Card* pCardRemove)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vRemove(pCardRemove);
}

/*----------------------------------------------------------------------------*/
/* remove given subcard (recursive)                                           */
/*----------------------------------------------------------------------------*/
void Card::vRemoveSubCard(Card* pCardRemove)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  ListExoIter2_t<Card>* pSubCard_I = pNewIter();
  
  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();
    
    if (pSubCard == pCardRemove) {
      vRemove(pCardRemove);
      break;
    }
    pSubCard->vRemoveSubCard(pCardRemove);
   }
  
  delete pSubCard_I;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vRenumberSubCards(Card* pParentCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  vRef(pParentCard);

  i8 iSubCardNo = 0;
  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();

    pSubCard->vCardNo(iSubCardNo++);
    pSubCard->vRenumberSubCards(this);
  }

  delete pSubCard_I;

  vRef();
}
/*----------------------------------------------------------------------------*/
//void Card::vRenumberMainCards(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//  ListExoIter2_t<Card>* pBoard_I = pNewIter();
//
//  i8 iCardNo = 0;
//  while (pBoard_I->bHasNext()) {
//    Card* pCard = pBoard_I->pNext();
//
//    pCard->vCardNo(iCardNo++);
//    pCard->vRenumberMainCards();
//  }
//
//  delete pBoard_I;
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vReplaceChildWithKids(Card* pChildCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card* pAfter = pChildCard;

  while (Card* pKid = pChildCard->pPop()) {
    vInsertAfter(pKid,pAfter);
    pAfter = pKid;
  }

  vRemoveChild(pChildCard);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vReplaceSubCard(Card* pSubCard, Card* pWithCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vReplace(pSubCard,pWithCard);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* Card::sz(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return m_sValue.szStz();
}

/*----------------------------------------------------------------------------*/
/* set table synonym                                                          */
/*----------------------------------------------------------------------------*/
char Card::cTableSyn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //Stz* szTableSyn = szGetProperty("szTableSyn"_Stz);

  //RASSERT(szValue()->bSame(szTableSyn));

  char cTableSyn = *(szValue()->azChar());

  return cTableSyn;
}


/*----------------------------------------------------------------------------*/
/* set table synonym                                                          */
/*----------------------------------------------------------------------------*/
void Card::vTableSyn(char cTableSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  s32 sSyn;
  sSyn.vSprint("%c"_Stz,cTableSyn);

  vValue(sSyn);
}

/*----------------------------------------------------------------------------*/
/* transfer all sub-cards from given card set to this card                    */
/*----------------------------------------------------------------------------*/
void Card::vTransferSubCards(Card* pCards)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  while (Card* pCard = pCards->pPop())
    vAppend(pCard);
}

/*----------------------------------------------------------------------------*/
/* transfer cards to this card from given card onwards                        */
/*----------------------------------------------------------------------------*/
void Card::vTransferFrom(Card* pCards, Card* pFrom,
                         Card* pAt, bool bAfter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  ListExoIter2_t<Card>* pCard_I = pNewIter();
  
  bool b = pCard_I->bPositionBefore(pFrom);
  if (! b) {
    delete pCard_I;
    return;
  }
  
  while (pCard_I->bHasNext()) {
    Card* pCard = pCard_I->pNext();
    
    pCard_I->vRemove();
    
    if (! pAt)
      vAppend(pCard);
    else
    if (bAfter)
      vInsertAfter(pAt,pCard);
    else
      vInsertBefore(pAt,pCard);
  }

  delete pCard_I;
}

/*----------------------------------------------------------------------------*/
/* transfer subset of cards from given card up till (and including) given card*/
/*----------------------------------------------------------------------------*/
void Card::vTransferSubset(Card* pCards, Card* pFrom, Card* pTill,
                           Card* pAt, bool bAfter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  ListExoIter2_t<Card>* pCard_I = pNewIter();
  
  bool b = pCard_I->bPositionBefore(pFrom);
  if (! b) {
    delete pCard_I;
    return;
  }
  
  while (pCard_I->bHasNext()) {
    Card* pCard = pCard_I->pNext();
    
    pCard_I->vRemove();
    
    if (! pAt)
      vAppend(pCard);
    else
    if (bAfter)
      vInsertAfter(pAt,pCard);
    else
      vInsertBefore(pAt,pCard);

    if (pTill == pCard)
      break;
  }

  delete pCard_I;
}

/*----------------------------------------------------------------------------*/
/* transfer cards to this card up till (and including) given card             */
/*----------------------------------------------------------------------------*/
void Card::vTransferTill(Card* pCards, Card* pTill,
                         Card* pAt, bool bAfter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  while (Card* pCard = pCards->pPop()) {
    
    if (! pAt)
      vAppend(pCard);
    else
    if (bAfter)
      vInsertAfter(pAt,pCard);
    else
      vInsertBefore(pAt,pCard);

    if (pTill == pCard)
      break;
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* Card::szValue(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return m_sValue.szStz();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vValue(Stz* szValue)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  //RASSERT(! *m_sValue.azChar());

  m_sValue = szValue;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card::vVisitAllSub(eCK eCard, std::function<bool(Card* pCard)> bfApply)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  eCK eTest = this->eCard();
  if (eTest == eCard) {
    bool b = bfApply(this);
  }

  i8 nChildren = nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = pGetChild(i_Child);
    pChildCard->vVisitAllSub(eCard,bfApply);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get aggregate level                                                        */
/*----------------------------------------------------------------------------*/
eAL Card::eAggLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  eAL eAggLevel = (eAL)iGetProperty("eAggLevel"_Stz);
  
  return eAggLevel;
}

/*----------------------------------------------------------------------------*/
/* set aggregate level                                                        */
/*----------------------------------------------------------------------------*/
void Card::vAggLevel(eAL eAggLvl)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_int("eAggLevel"_Stz,eAggLvl);
}

/*----------------------------------------------------------------------------*/
/* get agg op                                                                 */
/*----------------------------------------------------------------------------*/
bool Card::bAggOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  if (bHasProperty("bAggOp"_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set agg op                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vAggOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_nameOnly("bAggOp"_Stz);
}


/*----------------------------------------------------------------------------*/
/* evaluate bound                                                              */
/*----------------------------------------------------------------------------*/
ePB Card::eBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eRO eRelOp = this->eRelOp();
  
  switch (eRelOp) {
    case RO_LESS_THAN:     return PB_UBOUND;
    case RO_LESS_EQUAL:    return PB_UBOUND;
    case RO_EQUAL_TO:      return PB_EBOUND;
    case RO_GREATER_EQUAL: return PB_LBOUND;
    case RO_GREATER_THAN:  return PB_LBOUND;
      
    default:
      vTrace("%s(%lld): unexpected eRelOp %lld %s\n"_Stz
             ,__func__
             ,(i8)__LINE__
             ,(i8)eRelOp
             ,aRO[eRelOp]);
      RASSERT(0);
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* return pCard                                                               */
/*----------------------------------------------------------------------------*/
Card* Card::pCard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card* pCard = _pCard(pGetProperty("pCard"_Stz));

  return pCard;
}

/*----------------------------------------------------------------------------*/
/* set pCard                                                                  */
/*----------------------------------------------------------------------------*/
void Card::vCard(Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_ptr("pCard"_Stz,pCard);
}

/*----------------------------------------------------------------------------*/
/* print {card}                                                               */
/*----------------------------------------------------------------------------*/
void Card::vCardPrint(Ribbon* pRibbon, i8 iDepth, bool rbPrune)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CardPtr* pCard_CardPtr = _pCard_CardPtr(this);
  Card* pCard = _pCard(pCard_CardPtr->pCard());

  pRibbon->vSput(FR_NAME,pCard->szCardNm());

  if (pRibbon->iArea())
    return;

  pCard->vPrintCardProperties(pRibbon, iDepth,rbPrune);

  CardIter* pSubCardIter = pCard->pNewSubCardIter();
  while (pSubCardIter->bHasNext()) {
    Card *pSubCard = pSubCardIter->pNext();
    pSubCard->vPrintCardProperties(pRibbon, iDepth + 1,rbPrune);
  }
  delete pSubCardIter;
}

/*----------------------------------------------------------------------------*/
/* clear all marks                                                            */
/*----------------------------------------------------------------------------*/
void Card::vClearAllMarks(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vMark1(' ');
  vMark2(' ');

  i8 nSubCards = this->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card *pSubCard = pGetChild(i_Sub);

    pSubCard->vClearAllMarks();
  }

}

/*----------------------------------------------------------------------------*/
/* get column                                                                 */
/*----------------------------------------------------------------------------*/
Column* Card::pColumn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
    
  Column* pColumn = _pColumn(pGetProperty("pColumn"_Stz));

  return pColumn;
}

/*----------------------------------------------------------------------------*/
/* set column                                                                */
/*----------------------------------------------------------------------------*/
void Card::vColumn(Column* pColumn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_ptr("pColumn"_Stz,pColumn);
}

/*----------------------------------------------------------------------------*/
/* return column 1                                                            */
/*----------------------------------------------------------------------------*/
Column* Card::pColumn1(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
    
  Column* pColumn1 = _pColumn(pGetProperty("pColumn1"_Stz));

  return pColumn1;
}

/*----------------------------------------------------------------------------*/
/* set column 1                                                               */
/*----------------------------------------------------------------------------*/
void Card::vColumn1(Column* pColumn1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_ptr("pColumn1"_Stz,pColumn1);
}

/*----------------------------------------------------------------------------*/
/* return column 2                                                            */
/*----------------------------------------------------------------------------*/
Column* Card::pColumn2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Column* pColumn2 = _pColumn(pGetProperty("pColumn2"_Stz));

  return pColumn2;
}

/*----------------------------------------------------------------------------*/
/* set column 2                                                               */
/*----------------------------------------------------------------------------*/
void Card::vColumn2(Column* pColumn2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_ptr("pColumn2"_Stz,pColumn2);
}

/*----------------------------------------------------------------------------*/
/* concat terms                                                               */
/*----------------------------------------------------------------------------*/
void Card::vConcatTerms(Stz* szTerms)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vConcatTerms(szTerms,0);
}

/*----------------------------------------------------------------------------*/
/* concat terms                                                               */
/*----------------------------------------------------------------------------*/
void Card::vConcatTerms(Stz* szTerms, i8 i_From, i8 i_Till)
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
/* test if the expr has an aggregate op                                       */
/*----------------------------------------------------------------------------*/
bool Card::bHasAggOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool bHasAggOp = false;

  ListExoIter2_t<Card>* pExpr_I = pNewIter();

  while (pExpr_I->bHasNext()) {
    Card_Term* pCardTerm = _pCard_Term(pExpr_I->pNext());
    
    if (pCardTerm->bIsAggOp()) {
      bHasAggOp = true;
      break;
    }
  }
  
  delete pExpr_I;

  return bHasAggOp;
}

/*----------------------------------------------------------------------------*/
/* get head subcard                                                           */
/*----------------------------------------------------------------------------*/
Card_Term* Card::pHeadTerm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Term* pCard_Term = _pCard_Term(pHead());
  
  return pCard_Term;
}

/*----------------------------------------------------------------------------*/
/* return table                                                               */
/*----------------------------------------------------------------------------*/
Index* Card::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Index* pIndex = _pIndex(pGetProperty("pIndex"_Stz));

  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* set index                                                                  */
/*----------------------------------------------------------------------------*/
void Card::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_ptr("pIndex"_Stz,pIndex);
}

/*----------------------------------------------------------------------------*/
/* is this atom an aggregate op?                                              */
/*----------------------------------------------------------------------------*/
bool Card::bIsAggOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //Stz* szTerm = szGetProperty("szTerm"_Stz);

  //RASSERT(szValue()->bSame(szTerm));

  Stz* szTerm = szValue();
  if (szTerm->bSame("avg"_Stz))      return true;
  else if (szTerm->bSame("count"_Stz))    return true;
  else if (szTerm->bSame("countAll"_Stz)) return true;
  else if (szTerm->bSame("max"_Stz))      return true;
  else if (szTerm->bSame("min"_Stz))      return true;
//  else if (szTerm->bSame("stddev"_Stz))   return true;
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
/* is atom a column ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Card::bIsColumnRef(void)
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
/* is atom a column ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Card::bIsOrdinalColumnAtom(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //Stz* szTerm = szGetProperty("szTerm"_Stz);

  //RASSERT(szValue()->bSame(szTerm));

  Stz* szTerm = szValue();
  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];

  if ('$' == acFirst && StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is atom an index ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Card::bIsOrdinalIndexAtom(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //Stz* szTerm = szGetProperty("szTerm"_Stz);

  //RASSERT(szValue()->bSame(szTerm));

  Stz* szTerm = szValue();
  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];

  if ('$' == acFirst && ('I' == acSecond || 'i' == acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is atom a column ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Card::bIsOrdinalStreamTerm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //Stz* szTerm = szGetProperty("szTerm"_Stz);
  //vTrace("bIsOrdinalColumnTerm: %s\n"_Stz,szTerm);

  //RASSERT(szValue()->bSame(szTerm));

  Stz* szTerm = szValue();
  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];

  if ('$' == acFirst && StrLib::bIsDigit(acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is atom an index ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Card::bIsOrdinalIndexTerm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //Stz* szTerm = szGetProperty("szTerm"_Stz);

  //RASSERT(szValue()->bSame(szTerm));

  Stz* szTerm = szValue();
  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];

  if ('$' == acFirst && ('I' == acSecond || 'i' == acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this card the given kind?                                               */
/*----------------------------------------------------------------------------*/
bool Card::bIsPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eCard = this->eCard();
  
  switch (eCard) {
    case CK_EXPRBOOL:
    case CK_PREDOR:
    case CK_PREDAND:
      return true;
      
    default: return false;
  }
}

/*----------------------------------------------------------------------------*/
/* is this atom same as argument?                                             */
/*----------------------------------------------------------------------------*/
bool Card::bIsSame(Stz* szArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //Stz* szTerm = szGetProperty("szTerm"_Stz);

  //RASSERT(szValue()->bSame(szTerm));

  if (szValue()->bSame(szArg))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this atom an separator?                                                 */
/*----------------------------------------------------------------------------*/
bool Card::bIsSeparator(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //Stz* szTerm = szGetProperty("szTerm"_Stz);

  //RASSERT(szValue()->bSame(szTerm));

  if (szValue()->bSame(","_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get map back                                                                */
/*----------------------------------------------------------------------------*/
Stz* Card::szMapBack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szMapBack = szGetProperty("szMapBack"_Stz);

  return szMapBack;
}

/*----------------------------------------------------------------------------*/
/* set map back                                                               */
/*----------------------------------------------------------------------------*/
void Card::vMapBack(Stz* szMapBack)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_stz("szMapBack"_Stz,szMapBack);
}

/*----------------------------------------------------------------------------*/
/* get map from                                                               */
/*----------------------------------------------------------------------------*/
Stz* Card::szMapFrom(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szMapFrom = szGetProperty("szMapFrom"_Stz);

  return szMapFrom;
}

/*----------------------------------------------------------------------------*/
/* set map from                                                               */
/*----------------------------------------------------------------------------*/
void Card::vMapFrom(Stz* szMapFrom)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_stz("szMapFrom"_Stz,szMapFrom);
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
/* move given to child to be last child                                       */
/*----------------------------------------------------------------------------*/
void Card::vMoveToLastChild(Card* pChildCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vRemoveChild(pChildCard);
  vAppend(pChildCard);
}

/*----------------------------------------------------------------------------*/
/* new a card iterator                                                        */
/*----------------------------------------------------------------------------*/
CardIter* Card::pNewSubCardIter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter* pCardIter = (CardIter*)pNewIter();

  return pCardIter;
}

/*----------------------------------------------------------------------------*/
/* get ordHi                                                                  */
/*----------------------------------------------------------------------------*/
i8 Card::iOrdHi(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 iOrdHi = iGetProperty(C_CARD_OrdHi);

  return iOrdHi;
}

/*----------------------------------------------------------------------------*/
/* set ordHi                                                                  */
/*----------------------------------------------------------------------------*/
void Card::vOrdHi(i8 iOrdHi)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_int(C_CARD_OrdHi,iOrdHi);
}

/*----------------------------------------------------------------------------*/
/* get ordHi                                                                  */
/*----------------------------------------------------------------------------*/
i8 Card::iOrdLo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 iOrdLo = iGetProperty(C_CARD_OrdLo);

  return iOrdLo;
}

/*----------------------------------------------------------------------------*/
/* set ordLo                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vOrdLo(i8 iOrdLo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_int(C_CARD_OrdLo,iOrdLo);
}

/*----------------------------------------------------------------------------*/
/* get option                                                                 */
/*----------------------------------------------------------------------------*/
i8 Card::iOption(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 iOption = iGetProperty("iOption"_Stz);
  
  return iOption;
}

/*----------------------------------------------------------------------------*/
/* set option                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vOption(i8 iOption)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_int("iOption"_Stz,iOption);
}

/*----------------------------------------------------------------------------*/
/* get ordinal column value                                                   */
/*----------------------------------------------------------------------------*/
i8 Card::iOrdinalStreamColNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //Stz* szTerm = szGetProperty("szTerm"_Stz);

  //RASSERT(szValue()->bSame(szTerm));

  i8 i_Ord = StrLib::iGleanInt(szValue()->azChar());
  
  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* get ordinal index value                                                    */
/*----------------------------------------------------------------------------*/
i8 Card::iOrdinalIndexColNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //Stz* szTerm = szGetProperty("szTerm"_Stz);

  //RASSERT(szValue()->bSame(szTerm));

  i8 i_Ord = StrLib::iGleanInt(szValue()->azChar());
  
  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* pop subcard                                                                */
/*----------------------------------------------------------------------------*/
Card_Term* Card::pPopTerm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Term* pCard_Term = _pCard_Term(pPop());
  
  return pCard_Term;
}

/*----------------------------------------------------------------------------*/
/* get query name                                                             */
/*----------------------------------------------------------------------------*/
//Stz* Card::szQueryNm(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//  Stz* szQueryNm = szGetProperty("szQuerysNm"_Stz);
//
//  RASSERT(szValue()->bSame(szQueryNm));
//
//  return szValue();
//}

/*----------------------------------------------------------------------------*/
/* get rank                                                                   */
/*----------------------------------------------------------------------------*/
i8 Card::iRank(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 iRank = iGetProperty("iRank"_Stz);
  
  return iRank;
}

/*----------------------------------------------------------------------------*/
/* set rank                                                                   */
/*----------------------------------------------------------------------------*/
void Card::vRank(i8 iRank)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_int("iRank"_Stz,iRank);
}

/*----------------------------------------------------------------------------*/
/* set ref                                                                    */
/*----------------------------------------------------------------------------*/
void Card::vRef(void)
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

  strcat(m_caRef," ");
  strcat(m_caRef,sPretty.azChar());

  //  vTrace("%s\n"_Stz,m_caRef);
}

/*----------------------------------------------------------------------------*/
/* set ref                                                                    */
/*----------------------------------------------------------------------------*/
void Card::vRef(Card* pParentCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pParentCard) {
    sprintf(m_caRef,"%s.%lld", pParentCard->m_caRef, m_iCardNo);
  }
  else {
    sprintf(m_caRef,"%lld",m_iCardNo);
  }

//  vTrace("%s\n"_Stz,m_caRef);
}

/*----------------------------------------------------------------------------*/
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO Card::eRelOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = this->sz();
  
       if (szTerm->bSame("<"_Stz))       return RO_LESS_THAN;
  else if (szTerm->bSame("<="_Stz))      return RO_LESS_EQUAL;
  else if (szTerm->bSame("="_Stz))       return RO_EQUAL_TO;
  else if (szTerm->bSame("=="_Stz))      return RO_EQUAL_TO;
  else if (szTerm->bSame("!="_Stz))      return RO_NOT_EQUAL;
  else if (szTerm->bSame(">="_Stz))      return RO_GREATER_EQUAL;
  else if (szTerm->bSame(">"_Stz))       return RO_GREATER_THAN;
  
  else return RO_NONE;
}

/*----------------------------------------------------------------------------*/
/* get row limit                                                              */
/*----------------------------------------------------------------------------*/
i8 Card::nRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 nRows = iGetProperty("nRows"_Stz);

  return nRows;
}

/*----------------------------------------------------------------------------*/
/* set query name                                                             */
/*----------------------------------------------------------------------------*/
void Card::vRows(i8 nRows)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_int("nRows"_Stz,nRows);
}

/*----------------------------------------------------------------------------*/
/* get send kind                                                              */
/*----------------------------------------------------------------------------*/
eXK Card::eSendKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  eXK eSendKind = (eXK)iGetProperty("eSendKind"_Stz);

  return eSendKind;
}

/*----------------------------------------------------------------------------*/
/* set send kind                                                              */
/*----------------------------------------------------------------------------*/
void Card::vSendKind(eXK eSendKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_int("eSendKind"_Stz,eSendKind);
}

/*----------------------------------------------------------------------------*/
/* get sends                                                                  */
/*----------------------------------------------------------------------------*/
i8 Card::iSends(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 iSends = iGetProperty("iSends"_Stz);
  
  return iSends;
}

/*----------------------------------------------------------------------------*/
/* set sends                                                                  */
/*----------------------------------------------------------------------------*/
void Card::vSends(i8 iSends)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_int("iSends"_Stz,iSends);
}


/*----------------------------------------------------------------------------*/
/* get stage topology                                                         */
/*----------------------------------------------------------------------------*/
eYG Card::eStageTopo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  eYG eStageTopo = (eYG)iGetProperty("eStageTopo"_Stz);

  return eStageTopo;
}

/*----------------------------------------------------------------------------*/
/* set stage topology                                                         */
/*----------------------------------------------------------------------------*/
void Card::vStageTopo(eYG  eStageTopo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_int("eStageTopo"_Stz,eStageTopo);
}

/*----------------------------------------------------------------------------*/
/* get stream definition                                                      */
/*----------------------------------------------------------------------------*/
StreamDef* Card::pStreamDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  StreamDef* pStreamDef = _pStreamDef(pGetProperty("pStreamDef"_Stz));
  
  return pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* set stream definition                                                      */
/*----------------------------------------------------------------------------*/
void Card::vStreamDef(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_ptr("pStreamDef"_Stz,pStreamDef);
}

/*----------------------------------------------------------------------------*/
/* return table                                                               */
/*----------------------------------------------------------------------------*/
Table* Card::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Table* pTable = _pTable(pGetProperty("pTable"_Stz));

  return pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Card::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_ptr("pTable"_Stz,pTable);
}

/*----------------------------------------------------------------------------*/
/* return table 1                                                             */
/*----------------------------------------------------------------------------*/
Table* Card::pTable1(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Table* pTable1 = _pTable(pGetProperty("pTable1"_Stz));

  return pTable1;
}

/*----------------------------------------------------------------------------*/
/* set table 1                                                                */
/*----------------------------------------------------------------------------*/
void Card::vTable1(Table* pTable1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_ptr("pTable1"_Stz,pTable1);
}

/*----------------------------------------------------------------------------*/
/* return table 2                                                             */
/*----------------------------------------------------------------------------*/
Table* Card::pTable2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Table* pTable2 = _pTable(pGetProperty("pTable2"_Stz));

  return pTable2;
}

/*----------------------------------------------------------------------------*/
/* set table 2                                                                */
/*----------------------------------------------------------------------------*/
void Card::vTable2(Table* pTable2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_ptr("pTable2"_Stz,pTable2);
}

/*----------------------------------------------------------------------------*/
/* evaluate term                                                              */
/*----------------------------------------------------------------------------*/
eTE Card::eTermEval(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = this->sz();
  
       if (szTerm->bSame("and"_Stz))     return TE_AND;
  else if (szTerm->bSame("as"_Stz))      return TE_AS;
  else if (szTerm->bSame("boolean"_Stz)) return TE_BOOLEAN;
  else if (szTerm->bSame("or"_Stz))      return TE_OR;
  else if (szTerm->bSame("type"_Stz))    return TE_TYPE;
  
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

/*----------------------------------------------------------------------------*/
/* return typeShort                                                           */
/*----------------------------------------------------------------------------*/
char Card::cTypeShort(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Stz* szType = szGetProperty("szType"_Stz);

  RASSERT(szValue()->bSame(szType));

  for (i8 i_CT = 0; i_CT < CT_ENUMS; i_CT++) {
    if (szValue()->bSame(aCT[i_CT]))
      return cCT_char[i_CT];
  }

  return cCT_char[CT_ENUMS];
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print comment                                                              */
/*----------------------------------------------------------------------------*/
void Card::vCommentPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szComment = this->sz();

  pRibbon->vSprint("%s"_Stz,szComment);
}

/*----------------------------------------------------------------------------*/
/* print {index}                                                              */
/*----------------------------------------------------------------------------*/
void Card::vIndexPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_IndexPtr* pCard_IndexPtr = _pCard_IndexPtr(this);
  Index* pIndex = pCard_IndexPtr->pIndex();
  IndexDef* pIndexDef = pIndex->pIndexDef();
  Table* pTable = pIndex->pTable();

  pRibbon->vSput(FR_NAME,pIndex->szIndexNm());
  
  pRibbon->vSput(FR_TEXT
                 ,pIndexDef->bIsUnique()
                    ? "unique"_Stz : "not unique"_Stz
                 );
  
  pRibbon->vSput(FR_NAME,pTable->szTableNm());

  i8 nIdxCols = pIndexDef->nColumns();
  for (i8 i_Col = 0; i_Col < nIdxCols; i_Col++) {
    Column* pIdxCol = pIndexDef->pGetColumn(i_Col);
    
    pRibbon->vSput(FR_ATOM_SQUARE,pIdxCol->szColNm());
  }
  
  i8 nBuffers = 0;
  i8 nEntries = 0;
  pIndex->vAnalyze(nBuffers,nEntries);
  pRibbon->vSprintRight(" %lld buffers"_Stz,nBuffers);
  pRibbon->vSprintRight(" %lld entries"_Stz,nEntries);

  //vPrintCardProperties(pRibbon,iDepth);
}


/*----------------------------------------------------------------------------*/
/* print StreamElist                                                          */
/*----------------------------------------------------------------------------*/
void Card::vStreamElistPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_StreamElist* pCard_StreamElist = _pCard_StreamElist(this);

  if (! pCard_StreamElist->bSimpleDef()) {
    bool bPrune = false;
    vPrintCardProperties(pRibbon, iDepth,bPrune);
    return;
  }

  // expr
    // type
    // as
    // exprCalc
      // term
  
  eCK eSubCard;
  i8 nSubs = this->nItems();
  for (i8 i_Sub = 0; i_Sub < nSubs; i_Sub++) {
    Card* pSubCard = pGetChildCard(i_Sub, eSubCard);
    if (CK_EXPR != eSubCard)
      continue;
    
    Card_Expr* pCard_Expr = _pCard_Expr(pSubCard);

    Card_Type* pCard_Type = _pCard_Type(pCard_Expr->pFindSubCard(CK_TYPE));
    Card_As*   pCard_As   = _pCard_As  (pCard_Expr->pFindSubCard(CK_AS));

    pRibbon->vSprintRight("%s %c"_Stz
                          ,pCard_As ? pCard_As->sz() : "??AS?"_Stz
                          ,pCard_Type->cTypeShort()
                          );

  }
}

/*----------------------------------------------------------------------------*/
/* print {table}                                                              */
/*----------------------------------------------------------------------------*/
void Card::vTablePrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_TablePtr* pCardTablePtr = _pCard_TablePtr(this);
  Table* pTable = _pTable(pCardTablePtr->pTable());

  pRibbon->vSput(FR_NAME,pTable->szTableNm());

  if (pRibbon->iArea())
    return;
  
  //pCardTablePtr->vPrintPartition(pRibbon);
  PtnDef* pPtnDef = pTable->pPtnDef();
  Column* pColumn = pPtnDef->pGetColumn(0);
  
  pRibbon->vSput(FR_ATOM_SLASH,pColumn->szColNm());

  
  //pCardTablePtr->vPrintSize(pRibbon);
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  //pCardTablePtr->vPrintPrimary(pRibbon);
  IdxVec*   pIdxVec    = pTable->pIdxVec();
//  Index*    pIndex     = pIdxVec->pGet(0);
//  IndexDef* pIndexDef  = pIndex->pIndexDef();
//  Column*   pIdxColumn = pIndexDef->pGetColumn(0);
//   pRibbon->vSput(FR_ATOM_SQUARE,pIdxColumn->szColNm());

  //pRibbon->vSput(FR_SIZE,pStreamDef->jTableSize());
  pRibbon->vSprintRight(" tableSize %lld"_Stz,pStreamDef->jTableSize());
  pRibbon->vSprintRight(" %lld %s"_Stz,pIdxVec->nItems(),
                        1 == pIdxVec->nItems() ? "index" : "indexes");
}

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
#include "Card_CalcBool.hpp"
Card_CalcBool* Card::pSub_calcBool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CalcBool* p_calcBool =
      _pCard_CalcBool(pFindSubCard(CK_CALCBOOL));
  return p_calcBool;
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
#include "Card_JoinPred.hpp"
Card_JoinPred* Card::pSub_joinPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_JoinPred* p_joinPred =
      _pCard_JoinPred(pFindSubCard(CK_JOINPRED));
  return p_joinPred;
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
#include "Card_PredCmp.hpp"
Card_PredCmp* Card::pSub_predCmp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_PredCmp* p_predCmp =
      _pCard_PredCmp(pFindSubCard(CK_PREDCMP));
  return p_predCmp;
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
#include "Card_ScanPred.hpp"
Card_ScanPred* Card::pSub_scanPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_ScanPred* p_scanPred =
      _pCard_ScanPred(pFindSubCard(CK_SCANPRED));
  return p_scanPred;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "Card_StageTopo.hpp"
Card_StageTopo* Card::pSub_stageTopo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StageTopo* p_stageTopo =
      _pCard_StageTopo(pFindSubCard(CK_STAGETOPO));
  return p_stageTopo;
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
#include "Card_StreamColList.hpp"
Card_StreamColList* Card::pSub_streamColList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StreamColList* p_streamColList =
      _pCard_StreamColList(pFindSubCard(CK_STREAMCOLLIST));
  return p_streamColList;
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
#include "Card_TablePtr.hpp"
Card_TablePtr* Card::pSub_tablePtr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_TablePtr* p_tablePtr =
      _pCard_TablePtr(pFindSubCard(CK_TABLEPTR));
  return p_tablePtr;
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
