

#include "Card.hpp"
#include "CardCtl.hpp"
#include "Board.hpp"
#include "ShardDef.hpp"

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
Card* CardSet::pGetCard(i8 i_Card)
{
  Card* pCard = pGet(i_Card);
  return pCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardSet::pGetCard(i8 i_Card, eCK& reCard)
{
  Card* pCard = pGet(i_Card);
  
  if (pCard)
    reCard = pCard->eCard();
  else
    reCard = CK_NONE;
  
  return pCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardSet::vRemoveCards(eCK eCard)
{
  //_precondition_(_ADDRESS_,_FOOTER_);
  
  i8 nCards = nItems();
  for (i8 i_Card = 0; i_Card < nCards; i_Card++) {
    Card* pCard = pGet(i_Card);
    if (! pCard->bIsCard(eCard))
      continue;
    
    vRemove(pCard);
    i_Card--;
    nCards--;
  }
}



/*============================================================================*/
/* Card.cpp -- Abstract Card                                                  */
/*============================================================================*/


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
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
        
  m_eCard    = eCard;
  m_iCardNo  = -1;
  
  m_cMark1   = ' ';
  m_cMark2   = ' ';

  m_pProperties = nullptr;
  
  m_bPrintNm = true;
  
  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card::~Card(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Card* pCard = pPop(); pCard; pCard = pPop())
    delete pCard;
  
  delete m_pProperties;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* is eCK a main card?                                                        */
/*----------------------------------------------------------------------------*/
bool Card::bIsMain(eCK eCard)
{
  _static_
  
  switch (eCard) {
    case CK_AGGREG:
    case CK_CANDINDEX:
    case CK_COMBINE:
    case CK_FILTER:
    case CK_HASH:
    case CK_INDEXJOIN:
    case CK_INDEXSCAN:
    case CK_LIMIT:
    case CK_ORDAGG:
    case CK_PROJECT:
    case CK_SCAN:
    case CK_SEND:
    case CK_SORT:
    case CK_STAGE:
      return true;
      
    default:
      return false;
  }
}

/*----------------------------------------------------------------------------*/
/* find eCK by name                                                           */
/*----------------------------------------------------------------------------*/
eCK Card::eToECard(Stz* szName)
{
  _static_
  
  s32 sName;
  sName.vSprint("CK_%s"_Stz,szName);
  
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
void Card::vAddProperty(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;
  
  m_pProperties->vReplace(szName,"yes"_Stz);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::vAddProperty(Stz* szName, i8 iValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;
  
  m_pProperties->vReplace(szName,iValue);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::vAddProperty(Stz* szName, Object* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;
  
  m_pProperties->vReplace(szName,pValue);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::vAddProperty(Stz* szName, Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;
  
  m_pProperties->vReplace(szName,szValue);
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
/* cut given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::vCutProperty(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return;
  
  m_pProperties->vDelete(szName);
}

/*----------------------------------------------------------------------------*/
/* dum card and its sub cards                                                 */
/*----------------------------------------------------------------------------*/
void Card::vDump(i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  s48K sProperties;
  
  if (m_pProperties)
    m_pProperties->vToString(sProperties);
  else
    sProperties = "no properties"_Stz;
  
  vTrace("%s%lld %s %s { %s }\n"_Stz
         ,Stz::szTab(iDepth)
         ,m_iCardNo
         ,szCardNm()
         ,aCK[m_eCard]
         ,sProperties.szStz()
         );
  
  i8 nSubCards = nItems();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++)
    pGet(i_Sub)->vDump(iDepth+1);
}

/*----------------------------------------------------------------------------*/
/* duplicate this card and its sub cards                                      */
/*----------------------------------------------------------------------------*/
Card* Card::pDuplicate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card* pCard_dup = new Card(m_eCard);
  
  i8 nSubCards = this->nItems();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card* pSubCard = pGetCard(i_SubCard);
    
    Card* pSubCard_dup = pSubCard->pDuplicate();
    
    if (pSubCard->m_pProperties) {
      pSubCard_dup->vPropertiesEnsure();
      pSubCard_dup->pProperties()->vCopy(pSubCard->m_pProperties);
    }
    
    pCard_dup->vAppend(pSubCard_dup);
  }
  
  return pCard_dup;
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
/* get card name                                                              */
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
    vAddProperty("EBound"_Stz);
    return;
  }
  
  vCutProperty("EBound"_Stz);
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
    vAddProperty("LBound"_Stz);
    return;
  }
  
  vCutProperty("LBound"_Stz);
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
    vAddProperty("UBound"_Stz);
    return;
  }
  
  vCutProperty("UBound"_Stz);
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
/* renumber cards                                                             */
/*----------------------------------------------------------------------------*/
void Card::vRenumberSubCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  ListExoIter2_t<Card>* pSubCard_I = pNewIter();
  
  i8 i_SubCard = 0;
  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();
    
    pSubCard->vCardNo(i_SubCard++);
    
    pSubCard->vRenumberSubCards();
   }
  
  delete pSubCard_I;
}

/*----------------------------------------------------------------------------*/
/* is this card same as arg?                                                  */
/*----------------------------------------------------------------------------*/
bool Card::bSame(Card* pCard_arg)
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
  i8 nSubs = this->nItems();
  for (i8 i_Sub = 0; i_Sub < nSubs; i_Sub++) {
    Card* pSubCard = pGetCard(i_Sub,eSubCard);
    
    Card* pSubCard_arg = pCard_arg->pFindSubCard(eSubCard);
    if (! pSubCard_arg)
      return false;

    if (! pSubCard->bSame(pSubCard_arg))
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* transfer all sub-cards from given card set to this card                    */
/*----------------------------------------------------------------------------*/
void Card::vTransferAll(Card* pCards)
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
/* call closure for this card and each sub-card                               */
/*----------------------------------------------------------------------------*/
void Card::vWalkAllCards(std::function<void(Card* pCard)> f_vClosure)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  f_vClosure(this);
  
  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();
    
    pSubCard->vWalkAllCards(f_vClosure);
  }
  delete pSubCard_I;
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
  
  vAddProperty("eAggLevel"_Stz,eAggLvl);
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
  
  vAddProperty("bAggOp"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get as name                                                                */
/*----------------------------------------------------------------------------*/
Stz* Card::szAs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Stz* szAs = szGetProperty("szAs"_Stz);

  return szAs;
}

/*----------------------------------------------------------------------------*/
/* set as name                                                                */
/*----------------------------------------------------------------------------*/
void Card::vAs(Stz* szAs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vAddProperty("szAs"_Stz,szAs);
}

/*----------------------------------------------------------------------------*/
/* evaluate bound                                                              */
/*----------------------------------------------------------------------------*/
eBK Card::eBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eRO eRelOp = this->eRelOp();
  
  switch (eRelOp) {
    case RO_LESS_THAN:     return BK_UBOUND;
    case RO_LESS_EQUAL:    return BK_UBOUND;
    case RO_EQUAL_TO:      return BK_EBOUND;
    case RO_GREATER_EQUAL: return BK_LBOUND;
    case RO_GREATER_THAN:  return BK_LBOUND;
      
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
  
  vAddProperty("pColumn"_Stz,pColumn);
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
  
  vAddProperty("pColumn1"_Stz,pColumn1);
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
  
  vAddProperty("pColumn2"_Stz,pColumn2);
}

/*----------------------------------------------------------------------------*/
/* get sort direction                                                         */
/*----------------------------------------------------------------------------*/
Stz* Card::szDirn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Stz* szDirn = szGetProperty("szDirn"_Stz);

  return szDirn;
}

/*----------------------------------------------------------------------------*/
/* set sort dirn                                                              */
/*----------------------------------------------------------------------------*/
void Card::vDirn(Stz* szDirn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vAddProperty("szDirn"_Stz,szDirn);
}

/*----------------------------------------------------------------------------*/
/* get generate                                                               */
/*----------------------------------------------------------------------------*/
i8 Card::iGenerate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 iGenerate = iGetProperty("iGenerate"_Stz);
  
  return iGenerate;
}

/*----------------------------------------------------------------------------*/
/* set generate                                                               */
/*----------------------------------------------------------------------------*/
void Card::vGenerate(i8 iGenerate)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vAddProperty("iGenerate"_Stz,iGenerate);
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
  
  vAddProperty("pIndex"_Stz,pIndex);
}

/*----------------------------------------------------------------------------*/
/* is this atom an aggregate op?                                              */
/*----------------------------------------------------------------------------*/
bool Card::bIsAggOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = szGetProperty("szTerm"_Stz);

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
bool Card::bIsOrdinalColumnAtom(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = szGetProperty("szTerm"_Stz);

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

  Stz* szTerm = szGetProperty("szTerm"_Stz);

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

  Stz* szTerm = szGetProperty("szTerm"_Stz);
  //vTrace("bIsOrdinalColumnTerm: %s\n"_Stz,szTerm);
  
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

  Stz* szTerm = szGetProperty("szTerm"_Stz);

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

  Stz* szTerm = szGetProperty("szTerm"_Stz);

  if (szTerm->bSame(szArg))
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

  Stz* szTerm = szGetProperty("szTerm"_Stz);

  if (szTerm->bSame(","_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set level                                                                  */
/*----------------------------------------------------------------------------*/
void Card::vLevel(Stz* szLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vAddProperty("szLevel"_Stz,szLevel);
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
/* get mode                                                                   */
/*----------------------------------------------------------------------------*/
Stz* Card::szMode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Stz* szMode = szGetProperty("szMode"_Stz);

  return szMode;
}

/*----------------------------------------------------------------------------*/
/* set mode                                                                   */
/*----------------------------------------------------------------------------*/
void Card::vMode(Stz* szMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vAddProperty("szMode"_Stz,szMode);
}

/*----------------------------------------------------------------------------*/
/* get mode                                                                   */
/*----------------------------------------------------------------------------*/
Stz* Card::szName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Stz* szName = szGetProperty("szName"_Stz);

  return szName;
}

/*----------------------------------------------------------------------------*/
/* set mode                                                                   */
/*----------------------------------------------------------------------------*/
void Card::vName(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vAddProperty("szName"_Stz,szName);
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
  
  vAddProperty("iOption"_Stz,iOption);
}

/*----------------------------------------------------------------------------*/
/* get option                                                                 */
/*----------------------------------------------------------------------------*/
Stz* Card::szOption(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Stz* szOption = szGetProperty("szOption"_Stz);

  return szOption;
}

/*----------------------------------------------------------------------------*/
/* set option                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vOption(Stz* szOption)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vAddProperty("szOption"_Stz,szOption);
}

/*----------------------------------------------------------------------------*/
/* get ordinal column value                                                   */
/*----------------------------------------------------------------------------*/
i8 Card::iOrdinalStreamColNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = szGetProperty("szTerm"_Stz);
  i8 i_Ord = StrLib::iGleanInt(szTerm->azChar());
  
  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* get ordinal index value                                                    */
/*----------------------------------------------------------------------------*/
i8 Card::iOrdinalIndexColNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = szGetProperty("szTerm"_Stz);
  i8 i_Ord = StrLib::iGleanInt(szTerm->azChar());
  
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
Stz* Card::szQueryNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Stz* szQueryNm = szGetProperty("szQuerysNm"_Stz);

  return szQueryNm;
}

/*----------------------------------------------------------------------------*/
/* set query name                                                             */
/*----------------------------------------------------------------------------*/
void Card::vQueryNm(Stz* szQueryNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vAddProperty("szQueryNm"_Stz,szQueryNm);
}

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
  
  vAddProperty("iRank"_Stz,iRank);
}

/*----------------------------------------------------------------------------*/
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO Card::eRelOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = this->szTerm();
  
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
  
  vAddProperty("nRows"_Stz,nRows);
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
  
  vAddProperty("eSendKind"_Stz,eSendKind);
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
  
  vAddProperty("iSends"_Stz,iSends);
}

/*----------------------------------------------------------------------------*/
/* get stage name                                                             */
/*----------------------------------------------------------------------------*/
Stz* Card::szStageNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Stz* szStageNm = szGetProperty("szStageNm"_Stz);

  return szStageNm;
}

/*----------------------------------------------------------------------------*/
/* set stage name                                                             */
/*----------------------------------------------------------------------------*/
void Card::vStageNm(Stz* szStageNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vAddProperty("szStageNm"_Stz,szStageNm);
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
  
  vAddProperty("eStageTopo"_Stz,eStageTopo);
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
  
  vAddProperty("pStreamDef"_Stz,pStreamDef);
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
  
  vAddProperty("pTable"_Stz,pTable);
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
  
  vAddProperty("pTable1"_Stz,pTable1);
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
  
  vAddProperty("pTable2"_Stz,pTable2);
}

/*----------------------------------------------------------------------------*/
/* get term                                                                   */
/*----------------------------------------------------------------------------*/
Stz* Card::szTerm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Stz* szTerm = szGetProperty("szTerm"_Stz);

  return szTerm;
}

/*----------------------------------------------------------------------------*/
/* set term                                                                   */
/*----------------------------------------------------------------------------*/
void Card::vTerm(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vAddProperty("szTerm"_Stz,szTerm);
}

/*----------------------------------------------------------------------------*/
/* evaluate term                                                              */
/*----------------------------------------------------------------------------*/
eTE Card::eTermEval(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = this->szTerm();
  
       if (szTerm->bSame("and"_Stz))     return TE_AND;
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
/* return type                                                                */
/*----------------------------------------------------------------------------*/
Stz* Card::szType(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Stz* szType = szGetProperty("szType"_Stz);

  return szType;
}

/*----------------------------------------------------------------------------*/
/* return typeShort                                                           */
/*----------------------------------------------------------------------------*/
char Card::cTypeShort(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Stz* szType = szGetProperty("szType"_Stz);
  
  for (i8 i_CT = 0; i_CT < CT_ENUMS; i_CT++) {
    if (szType->bSame(aCT[i_CT]))
      return cCT_char[i_CT];
  }

  return cCT_char[CT_ENUMS];
}

/*----------------------------------------------------------------------------*/
/* set type                                                                   */
/*----------------------------------------------------------------------------*/
void Card::vType(Stz* szType)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vAddProperty("szType"_Stz,szType);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card::vPrintCard(Ribbon* pRibbon, i8 iDepth, i8 iBelow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool bPrune = false;
  CardCtl* pCardCtl = _pCardCtl(pRibbon->pCtl());
  
  if (pCardCtl->iBelow(m_eCard))
    iBelow = pCardCtl->iBelow(m_eCard);

  if (iDepth <= pCardCtl->iDepth() || iBelow || pCardCtl->iPrint(m_eCard)) {
    
    i8 iForm = pCardCtl->iForm(m_eCard);
    
    if (iForm) {
      if (! m_pProperties)
        vPrintName(pRibbon,iDepth);
    }
    else {
      vPrintName(pRibbon,iDepth);
    }
    vPrintDetails(pRibbon,iDepth,bPrune);
  }
  
  if (bPrune)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ListExoIter2_t<Card>* pCard_I = pNewIter();
  while (pCard_I->bHasNext()) {
    Card* pCard = pCard_I->pNext();

    pCard->vPrintCard(pRibbon,iDepth + 1,iBelow);
  }

  delete pCard_I;
}


/*----------------------------------------------------------------------------*/
/* print card onto given ribbon                                               */
/*----------------------------------------------------------------------------*/
void Card::vPrintDetails(Ribbon* pRibbon, i8 iDepth, bool& bPrune)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  eCK eCard = this->eCard();
  
  switch (eCard) {

    case CK_AGGELIST:                   vPrintProperties(pRibbon,iDepth); break;
    case CK_AGGLEVEL:                   vPrintProperties(pRibbon,iDepth); break;
    case CK_AGGREG:                     vPrintProperties(pRibbon,iDepth); break;
    case CK_AS:                         vPrintProperties(pRibbon,iDepth); break;
                        
    case CK_CANDINDEX:                  vPrintProperties(pRibbon,iDepth); break;
    case CK_COMBINE:                    vPrintProperties(pRibbon,iDepth); break;
                        
    case CK_DEBUG:                      vPrintProperties(pRibbon,iDepth); break;
                        
    case CK_EBOUND:                     vPrintProperties(pRibbon,iDepth); break;
    case CK_EXISTS:                     vPrintProperties(pRibbon,iDepth); break;
    case CK_EXPR:                       vPrintProperties(pRibbon,iDepth); break;
    case CK_EXPRBOOL:                   vPrintProperties(pRibbon,iDepth); break;
    case CK_EXPRCALC:                   vPrintProperties(pRibbon,iDepth); break;

    case CK_FILTER:                     vPrintProperties(pRibbon,iDepth); break;
              
    case CK_GBYELIST:                   vPrintProperties(pRibbon,iDepth); break;
    case CK_GENERATE:                   vPrintProperties(pRibbon,iDepth); break;

    case CK_HASH:                       vPrintProperties(pRibbon,iDepth); break;
    case CK_HAVING:                     vPrintProperties(pRibbon,iDepth); break;
           
    case CK_INDEX:      vIndexPrint                     (pRibbon,iDepth); break;
    case CK_INDEXJOIN:                  vPrintProperties(pRibbon,iDepth); break;
    case CK_INDEXPRED:                  vPrintProperties(pRibbon,iDepth); break;
    case CK_INDEXSCAN:                  vPrintProperties(pRibbon,iDepth); break;
          
    case CK_LBOUND:                     vPrintProperties(pRibbon,iDepth); break;
    case CK_LIMIT:                      vPrintProperties(pRibbon,iDepth); break;
                                                
    case CK_MODE:                       vPrintProperties(pRibbon,iDepth); break;

    case CK_NAME:                       vPrintProperties(pRibbon,iDepth); break;

    case CK_ON:                         vPrintProperties(pRibbon,iDepth); break;
    case CK_OPTION:                     vPrintProperties(pRibbon,iDepth); break;
    case CK_ORDAGG:                     vPrintProperties(pRibbon,iDepth); break;
    case CK_ORDERELIST:                 vPrintProperties(pRibbon,iDepth); break;

    case CK_PREDAND:                    vPrintProperties(pRibbon,iDepth); break;
    case CK_PREDCMP:                    vPrintProperties(pRibbon,iDepth); break;
    case CK_PREDOR:                     vPrintProperties(pRibbon,iDepth); break;
    case CK_PROJECT:                    vPrintProperties(pRibbon,iDepth); break;
    case CK_PROJELIST:                  vPrintProperties(pRibbon,iDepth); break;
              
    case CK_RBOUND:                     vPrintProperties(pRibbon,iDepth); break;
    case CK_ROWS:                       vPrintProperties(pRibbon,iDepth); break;

    case CK_SBYELIST:                   vPrintProperties(pRibbon,iDepth); break;
    case CK_SCAN:                       vPrintProperties(pRibbon,iDepth); break;
    case CK_SEND:                       vPrintProperties(pRibbon,iDepth); break;
    case CK_SENDKIND:                   vPrintProperties(pRibbon,iDepth); break;
    case CK_SENDS:                      vPrintProperties(pRibbon,iDepth); break;
    case CK_SORT:                       vPrintProperties(pRibbon,iDepth); break;
    case CK_DIRECTION:                  vPrintProperties(pRibbon,iDepth); break;
    case CK_STAGE:                      vPrintProperties(pRibbon,iDepth); break;
    case CK_STAGETOPO:                  vPrintProperties(pRibbon,iDepth); break;
    case CK_STREAM:                     vPrintProperties(pRibbon,iDepth); break;
    case CK_STREAMDEF:  vStreamDefPrint                 (pRibbon,iDepth); break;
    case CK_TABLE:      vTablePrint                     (pRibbon,iDepth); break;
    case CK_TERM:       vTermPrint                      (pRibbon,iDepth); break;
    case CK_TYPE:                       vPrintProperties(pRibbon,iDepth); break;

    case CK_UBOUND:                     vPrintProperties(pRibbon,iDepth); break;
    case CK_UNIQUE:                     vPrintProperties(pRibbon,iDepth); break;
    
    case CK_STREAMELIST:
      vStreamElistPrint(pRibbon,iDepth,bPrune);
      break;

    default:
      vTrace("SWITCH %s: Missing case for eCard %lld %s\n"_Stz
             ,__func__
             ,(i8)eCard
             ,aCK[eCard]
             );
      RASSERT(0);
      break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* put header                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vPrintHeader(Ribbon* pRibbon)
{
  _static_

  //pRibbon->vSput(FR_HEAD,pRibbon->iLogicalColumn());
  
  Board* pBoard = _pBoard(pRibbon->pSurface());
  pRibbon->vSprint("%c-%02lld--------------=="_Stz
                   ,pBoard->bInRegister(this) ? 'R' : '-'
                   ,pRibbon->iLogicalColumn()
                   );

}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card::vPrintName(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  s32 sName;
    
  if (m_bPrintNm) {
    
    if (-1 == m_iCardNo) {
      sName.vSprint("%s%s x%lld"_Stz
                    ,Stz::szDots(iDepth)
                    ,szCardNm()
                    ,nItems()
                    );
//      pRibbon->vSprint("%-18s%c%c"_Stz
//                       ,sName.szStz()
//                       ,m_cMark1
//                       ,m_cMark2
//                       );
    }
    else {
      sName.vSprint("%s%lld)%s"_Stz
                    ,Stz::szDots(iDepth)
                    ,m_iCardNo
                    ,szCardNm()
                    );

      //pRibbon->vSprint("%s"_Stz,sName.szStz());
    }
    pRibbon->vSprint("%-18s%c%c"_Stz
                     ,sName.szStz()
                     ,m_cMark1
                     ,m_cMark2
                     );
  }
  
}



/*----------------------------------------------------------------------------*/
/* print by piece                                                             */
/*----------------------------------------------------------------------------*/
void Card::vPrintProperties(Ribbon* pRibbon, i8 iDepth, bool bPrintName)
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
        if (bPrintName)
          pRibbon->vSprintRight("%s %lld"_Stz,szName,iValue);
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
        if (bPrintName)
          pRibbon->vSprintRight("%s %s"_Stz,szName,szValue);
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
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* print {index}                                                              */
/*----------------------------------------------------------------------------*/
void Card::vIndexPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Index* pCard_Index = _pCard_Index(this);
  Index* pIndex = pCard_Index->pIndex();
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

  //vPrintProperties(pRibbon,iDepth);
}


/*----------------------------------------------------------------------------*/
/* print {streamdef}                                                          */
/*----------------------------------------------------------------------------*/
void Card::vStreamDefPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_StreamDef* pCard_StreamDef = _pCard_StreamDef(this);
  StreamDef* pStreamDef = pCard_StreamDef->pStreamDef();
  if (! pStreamDef)
    return;
  
  i8 nColumns = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    Column* pColumn = pStreamDef->pGetColumn(i_Col);
    
    pRibbon->vSput(FR_SDEF_COLUMN,pColumn->szColNm(),pColumn->cTypeShort());
  }

  i8 nOrders = pStreamDef->nOrders();
  for (i8 i_Ord = 0; i_Ord < nOrders; i_Ord++) {
    Column* pOrdCol = pStreamDef->pGetOrder(i_Ord);
      
    //pRibbon->vSput(FR_ATOM_SQUARE,pOrdCol->szColNm());
    
    pRibbon->vSprintRight("[%s %s]"_Stz
                          ,pOrdCol->szColNm()
                          ,aSD_short[pStreamDef->eDirection(i_Ord)]
                          );

  }
  
  pRibbon->vSput(FR_TEXT
                 ,pStreamDef->bUnique()
                    ? "unique"_Stz : "not unique"_Stz
                 );
}

/*----------------------------------------------------------------------------*/
/* print StreamElist                                                          */
/*----------------------------------------------------------------------------*/
void Card::vStreamElistPrint(Ribbon* pRibbon, i8 iDepth, bool& bPrune)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_StreamElist* pCard_StreamElist = _pCard_StreamElist(this);

  if (! pCard_StreamElist->bSimpleDef()) {
    vPrintProperties(pRibbon,iDepth);
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
    Card* pSubCard = pGetCard(i_Sub,eSubCard);
    if (CK_EXPR != eSubCard)
      continue;
    
    Card_Expr* pCard_Expr = _pCard_Expr(pSubCard);

    Card_Type* pCard_Type = _pCard_Type(pCard_Expr->pFindSubCard(CK_TYPE));
    Card_As*   pCard_As   = _pCard_As  (pCard_Expr->pFindSubCard(CK_AS));

    pRibbon->vSprintRight("%s %c"_Stz
                          ,pCard_As->szAs()
                          ,pCard_Type->cTypeShort()
                          );

  }
  
  bPrune = true;
}

/*----------------------------------------------------------------------------*/
/* print {table}                                                              */
/*----------------------------------------------------------------------------*/
void Card::vTablePrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Table* pCardTable = _pCard_Table(this);
  Table* pTable = _pTable(pCardTable->pTable());

  pRibbon->vSput(FR_NAME,pTable->szTableNm());

  if (pRibbon->iArea())
    return;
  
  //pCardTable->vPrintPartition(pRibbon);
  ShardDef* pShardDef = pTable->pShardDef();
  Column* pColumn = pShardDef->pGetColumn(0);
  
  pRibbon->vSput(FR_ATOM_SLASH,pColumn->szColNm());

  
  //pCardTable->vPrintSize(pRibbon);
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  //pCardTable->vPrintPrimary(pRibbon);
  IdxVec*   pIdxVec    = pTable->pIdxVec();
//  Index*    pIndex     = pIdxVec->pGet(0);
//  IndexDef* pIndexDef  = pIndex->pIndexDef();
//  Column*   pIdxColumn = pIndexDef->pGetColumn(0);
//   pRibbon->vSput(FR_ATOM_SQUARE,pIdxColumn->szColNm());

  //pRibbon->vSput(FR_SIZE,pStreamDef->jTableSize());
  pRibbon->vSprintRight(" %lld buffers"_Stz,pStreamDef->jTableSize());
  pRibbon->vSprintRight(" %lld indexes"_Stz,pIdxVec->nItems());
}

/*----------------------------------------------------------------------------*/
/* print term card onto given ribbon                                          */
/*----------------------------------------------------------------------------*/
void Card::vTermPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  //vTrace("vTermPrint\n"_Stz);
  
  //Card_Term* pCardTerm = _pCard_Term(this);

  vPrintProperties(pRibbon,iDepth,true);
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
