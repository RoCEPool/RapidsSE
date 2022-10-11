

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
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card::Card(eCK eCard, Stz* szValue, Stz* szPptName, i8 iPptValue)
    : Object(CN_CARD)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vInit(eCard);

  vValue(szValue);

  vAddProperty_int(szPptName,iPptValue);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card::Card(eCK eCard, Stz* szValue, Stz* szPptName, Object* pPptValue)
    : Object(CN_CARD)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vInit(eCard);

  vValue(szValue);

  vAddProperty_ptr(szPptName,pPptValue);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card::Card(eCK eCard, Stz* szValue, Stz* szPptName, Stz* szPptValue)
    : Object(CN_CARD)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vInit(eCard);

  vValue(szValue);

  vAddProperty_stz(szPptName,szPptValue);
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void Card::vInit(eCK eCard)
{
  static i8 iSeqNo = 0;

  MemLib::vZero(md_Bearing, sizeof(md_Bearing), CN_CARD);

  m_eCard   = eCard;
  m_iCardNo = C_UNKNOWN;

  m_cMark1 = ' ';
  m_cMark2 = ' ';

  m_pProperties = nullptr;

  md__SeqNo = iSeqNo++;
  gapCard[md__SeqNo] = this;

//  if (35 == md__SeqNo) {
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::vAddProperty_chr(Stz* szPpyName, char cPpyValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;

  s32 sPptValue;
  sPptValue.vSprint("%c"_Stz, cPpyValue);
  m_pProperties->vReplace(szPpyName, sPptValue);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::vAddProperty_int(Stz* szPpyName, i8 iPpyValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;
  
  m_pProperties->vReplace(szPpyName, iPpyValue);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::vAddProperty_nameOnly(Stz* szPpyName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_)

  if (! m_pProperties)
    m_pProperties = new TagSet;

  m_pProperties->vReplace(szPpyName, "yes"_Stz);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::vAddProperty_ptr(Stz* szPpyName, Object* pPpyValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;
  
  m_pProperties->vReplace(szPpyName, pPpyValue);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::vAddProperty_stz(Stz* szPpyName, Stz* szPpyValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;
  
  m_pProperties->vReplace(szPpyName, szPpyValue);
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
/* delete child-card                                                          */
/*----------------------------------------------------------------------------*/
void Card::vDeleteChildCard(eCK eCard)
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
void Card::vDeleteProperty(Stz* szPptName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return;

  m_pProperties->vDelete(szPptName);
}

/*----------------------------------------------------------------------------*/
/* delete given property (recursively)                                        */
/*----------------------------------------------------------------------------*/
void Card::vDeletePropertyRecursively(Stz* szPptName)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vDeleteProperty(szPptName);

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pChildCard = pGetChild(i_Child);
    pChildCard->vDeletePropertyRecursively(szPptName);
  }
}

/*----------------------------------------------------------------------------*/
/* dump card and its sub cards                                                */
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
/* is this card equivalent to given arg?                                      */
/*----------------------------------------------------------------------------*/
bool Card::bEquivalent(Card* pToCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  TagSet* pProperties_arg = pToCard->pProperties();
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
    Card* pSubCard = pGetChild(i_Sub, eSubCard);
    
    Card* pSubCard_arg = pToCard->pFindSubCard(eSubCard);
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
i8 Card::iFindChild(Card* pChild)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nSubCards = this->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card* pTestCard = pGetChild(i_Sub);

    if (pChild == pTestCard)
      return i_Sub;
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* find immediate child-card                                                  */
/*----------------------------------------------------------------------------*/
Card* Card::pFindChild(eCK eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i_Child;

  Card* pFoundCard = pFindChild(eCard, i_Child);

  return pFoundCard;
}

/*----------------------------------------------------------------------------*/
/* find immediate child-card                                                  */
/*----------------------------------------------------------------------------*/
Card* Card::pFindChild(eCK eCard, i8& i_Child)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card* pFoundCard = nullptr;

  eCK eSubCard;
  i8 nSubCards = this->nChildCards();
  for (i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pSubCard = pGetChild(i_Child, eSubCard);

    if (eCard == pSubCard->eCard()) {
      pFoundCard = pSubCard;
      break;
    }
  }

  return pFoundCard;
}

/*----------------------------------------------------------------------------*/
/* find parent of given sub-card                                              */
/*----------------------------------------------------------------------------*/
Card* Card::pFindParent(Card* pSubCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 nSubCards = this->nChildCards();
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
char Card::cGetProperty(Stz* szPptName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return ' ';

  Stz* szValue = m_pProperties->szValue(szPptName);
  if (! szValue)
    return ' ';

  return *szValue->azChar();
}

/*----------------------------------------------------------------------------*/
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
char Card::cGetProperty(i8 i_Property)
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
i8 Card::iGetProperty(Stz* szPptName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return C_UNKNOWN;
  
  i8 iValue = m_pProperties->iValue(szPptName);
  
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
Object* Card::pGetProperty(Stz* szPptName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return nullptr;
  
  Object* pValue = m_pProperties->pValue(szPptName);
  
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
Stz* Card::szGetProperty(Stz* szPptName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return nullptr;
  
  Stz* szValue = m_pProperties->szValue(szPptName);
  
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Card::pGetChild(i8 i_Child)
{
  Card* pCard = pGet(i_Child);
  return pCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* Card::pGetChild(i8 i_Child, eCK& reCard)
{
  Card* pCard = pGet(i_Child);

  if (pCard)
    reCard = pCard->eCard();
  else
    reCard = CK_NONE;

  return pCard;
}

/*----------------------------------------------------------------------------*/
/* set guage                                                                  */
/*----------------------------------------------------------------------------*/
void Card::vGuage(void)
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
/* set guage                                                                  */
/*----------------------------------------------------------------------------*/
void Card::vGuage(Card* pParentCard)
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
bool Card::bHasSubCard(Card* pSubCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pTestCard = pGetChild(i_Child);

    if (pTestCard == pSubCard)
      return true;

    if (pTestCard->bHasSubCard(pSubCard))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* is given card a sub card of this card?                                     */
/*----------------------------------------------------------------------------*/
bool Card::bHasSubCard(Card* pSubCard, Card*& rpParentCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nSubCards = this->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pTestCard = pGetChild(i_Child);

    if (pTestCard == pSubCard) {
      rpParentCard = this;
      return true;
    }

    if (pTestCard->bHasSubCard(pSubCard, rpParentCard))
      return true;
  }

  rpParentCard = nullptr;
  return false;
}

/*----------------------------------------------------------------------------*/
/* does card have given property ?                                            */
/*----------------------------------------------------------------------------*/
bool Card::bHasProperty(Stz* szPpyName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return false;

  bool bHas = m_pProperties->bHasName(szPpyName);

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
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card::vPrintCard(Ribbon* pRibbon, i8 iDepth, bool& rbPrune)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (CK_CMPPRED == m_eCard)
    vDebugPoint();

  if (0 == iDepth)
    vPrintHeader(pRibbon);

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCtl());

  if (pCardCtl->bPrintByKind(m_eCard))
    vPrintCard_byKind(pRibbon, iDepth);
  else
    vPrintCard_bySettings(pRibbon, iDepth);

  if (pCardCtl->bPrune(m_eCard))
    rbPrune = true;
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
void Card::vPrintCard_byCustom(Ribbon* pRibbon, i8 iDepth, Stz* szCustomName)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCtl());

  if (pCardCtl->bPrintName(m_eCard))
    vPrintName(pRibbon, iDepth, szCustomName);
  if (pCardCtl->bPrintValue(m_eCard))
    vPrintValue(pRibbon, iDepth);

  if (pCardCtl->bPrintProperties(m_eCard))
    vPrintProperties(pRibbon, iDepth);

  if (pCardCtl->bPrintSubShort(m_eCard))
    vPrintSubShort(pRibbon, iDepth);
}

/*----------------------------------------------------------------------------*/
/* print card onto given ribbon                                               */
/*----------------------------------------------------------------------------*/
void Card::vPrintCard_byKind(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  switch (m_eCard) {

    case CK_CARDPTR:
      _pCard_CardPtr(this)->vCardPtrPrint(pRibbon, iDepth);
      break;

    case CK_INDEXPTR:
      _pCard_IndexPtr(this)->vIndexPtrPrint(pRibbon, iDepth);
      break;

    case CK_STREAM:
      _pCard_Stream(this)->vStreamPrint(pRibbon, iDepth);
      break;

    case CK_STREAMELIST:
      _pCard_StreamElist(this)->vStreamElistPrint(pRibbon,iDepth);
      break;

    case CK_TABLE:
      _pCard_Table(this)->vTablePrint(pRibbon, iDepth);
      break;

    case CK_TABLEPTR:
      _pCard_TablePtr(this)->vTablePtrPrint(pRibbon, iDepth);
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
    if (pCardCtl->bPrintName(m_eCard))
      vPrintName(pRibbon, iDepth);
    if (pCardCtl->bPrintValue(m_eCard))
      vPrintValue(pRibbon, iDepth);
  }

  if (pCardCtl->bPrintProperties(m_eCard))
    vPrintProperties(pRibbon, iDepth);

  if (pCardCtl->bPrintSubShort(m_eCard))
    vPrintSubShort(pRibbon, iDepth);
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
    if (szCustomName)
      sName.vSprint("%s%lld]%s"_Stz,
                    Stz::szDots(iDepth - 1),
                    m_iCardNo,
                    szCustomName);
    else
      sName.vSprint("%s%lld]%s%s"_Stz,
                    Stz::szDots(iDepth - 1),
                    m_iCardNo,
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
    if (bHasValue())
      sNameValue.vSprint("%s%lld]%s%s %s"_Stz,
                       Stz::szDots(iDepth - 1),
                       m_iCardNo,
                       sName.szStz(),
                       sHiddenChildren.szStz(),
                       szValue());
    else
      sNameValue.vSprint("%s%lld]%s%s"_Stz,
                         Stz::szDots(iDepth - 1),
                         m_iCardNo,
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
        Stz* szName = szGetPropertyNm(i_Property);
        i8   iValue = iGetProperty(i_Property);

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
/* print compressed children                                                  */
/*----------------------------------------------------------------------------*/
void Card::vPrintSubShort(Ribbon* pRibbon, i8 iDepth)
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
/* print value of card                                                        */
/*----------------------------------------------------------------------------*/
void Card::vPrintValue(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! bHasValue())
    return;

  pRibbon->vSprintRight("  %s"_Stz,szValue());

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
/* remove given immediate child card (non-recursive)                          */
/*----------------------------------------------------------------------------*/
void Card::vRemoveChild(Card* pChildCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vRemove(pChildCard);
}

/*----------------------------------------------------------------------------*/
/* remove given subCard (recursive)                                           */
/*----------------------------------------------------------------------------*/
void Card::vRemoveSubCard(Card* pSubCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  ListExoIter2_t<Card>* pSubCard_I = pNewIter();
  
  while (pSubCard_I->bHasNext()) {
    Card* pTestCard = pSubCard_I->pNext();
    
    if (pTestCard == pSubCard) {
      vRemove(pTestCard);
      break;
    }
    pTestCard->vRemoveSubCard(pSubCard);
   }
  
  delete pSubCard_I;
}

/*----------------------------------------------------------------------------*/
/* renumber subCard's                                                         */
/*----------------------------------------------------------------------------*/
void Card::vRenumberSubCards(Card* pParentCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  vGuage(pParentCard);

  i8 iSubCardNo = 0;
  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();

    pSubCard->vCardNo(iSubCardNo++);
    pSubCard->vRenumberSubCards(this);
  }

  delete pSubCard_I;

  vGuage();
}

/*----------------------------------------------------------------------------*/
/* remove given child card from this card and replace it with its children    */
/*----------------------------------------------------------------------------*/
void Card::vReplaceChildWithItsSubs(Card* pChildCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card* pAfter = pChildCard;

  while (Card* pSub = pChildCard->pPop()) {
    vInsertAfter(pSub, pAfter);
    pAfter = pSub;
  }

  vRemoveChild(pChildCard);
}

/*----------------------------------------------------------------------------*/
/* replace immediate child with given card                                    */
/*----------------------------------------------------------------------------*/
void Card::vReplaceChild(Card* pChildCard, Card* pWithCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vReplace(pChildCard, pWithCard);
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
/* transfer all child card (incl. sub-cards) from given card set to this card */
/*----------------------------------------------------------------------------*/
void Card::vTransferChildren(Card* pFromCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  while (Card* pPoppedChildCard = pFromCard->pPop())
    vAppend(pPoppedChildCard);
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
/* visit all sub-cards of given eCard and apply lambda function               */
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
#include "Card_IndexPtr.hpp"
Card_IndexPtr* Card::pSub_indexPtr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_IndexPtr* p_indexPtr =
      _pCard_IndexPtr(pFindSubCard(CK_INDEXPTR));
  return p_indexPtr;
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
