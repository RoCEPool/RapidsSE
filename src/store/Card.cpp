/*============================================================================*/
/* Card.cpp -- Abstract Card                                                  */
/*============================================================================*/

#include "Card.hpp"
#include "Board.hpp"
#include "ShardDef.hpp"

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
        
  m_iCardNo  = -1;
  m_eCard    = eCard;
  m_sCardNm  = aCK_short[m_eCard];
  
  m_pProperties = nullptr;
  
  m_bPrintNm = true;
  
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
/* put header                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vCardHeader(Ribbon* pRibbon)
{
  _static_

  pRibbon->vSput(FR_HEAD,pRibbon->iLogicalColumn());
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

  return m_sCardNm;
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
/* get i8 of text                                                             */
/*----------------------------------------------------------------------------*/
i8 Card::iOfText(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_sText.iToI8();
}

/*----------------------------------------------------------------------------*/
/* get i8 of text                                                             */
/*----------------------------------------------------------------------------*/
i8 Card::nOfText(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_sText.iToI8();
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
/* get text                                                                   */
/*----------------------------------------------------------------------------*/
//Stz* Card::sz(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//  return m_sText;
//}

/*----------------------------------------------------------------------------*/
/* get text                                                                   */
/*----------------------------------------------------------------------------*/
Stz* Card::szText(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return m_sText;
}

/*----------------------------------------------------------------------------*/
/* set text                                                                   */
/*----------------------------------------------------------------------------*/
void Card::vText(Stz* szText)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_sText = szText;
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
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Card::vDeleteThis_(Card* pCard)
{
  _static_
  
  delete pCard;
}





/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get aggregate level                                                        */
/*----------------------------------------------------------------------------*/
eAL Card::eAggLvl(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  eAL eAggLvl = (eAL)iGetProperty("eAggLvl"_Stz);
  
  return eAggLvl;
}

/*----------------------------------------------------------------------------*/
/* set aggregate level                                                        */
/*----------------------------------------------------------------------------*/
void Card::vAggLvl(eAL eAggLvl)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vAddProperty("eAggLvl"_Stz,eAggLvl);
}

/*----------------------------------------------------------------------------*/
/* get as name                                                                */
/*----------------------------------------------------------------------------*/
Stz* Card::szAsNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Stz* szAsNm = szGetProperty("szAsNm"_Stz);

  return szAsNm;
}

/*----------------------------------------------------------------------------*/
/* set as name                                                                */
/*----------------------------------------------------------------------------*/
void Card::vAsNm(Stz* szAsNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vAddProperty("szAsNm"_Stz,szAsNm);
}

/*----------------------------------------------------------------------------*/
/* get atom name                                                              */
/*----------------------------------------------------------------------------*/
Stz* Card::szAtom(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Stz* szAsNm = szGetProperty("szAtom"_Stz);

  return szAsNm;
}

/*----------------------------------------------------------------------------*/
/* set atom name                                                              */
/*----------------------------------------------------------------------------*/
void Card::vAtom(Stz* szAtom)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vAddProperty("szAtom"_Stz,szAtom);
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

  Stz* szAtom = szGetProperty("atom"_Stz);

       if (szAtom->bSame("avg"_Stz))      return true;
  else if (szAtom->bSame("count"_Stz))    return true;
  else if (szAtom->bSame("countAll"_Stz)) return true;
  else if (szAtom->bSame("max"_Stz))      return true;
  else if (szAtom->bSame("min"_Stz))      return true;
//  else if (szAtom->bSame("stddev"_Stz))   return true;
  else if (szAtom->bSame("sum"_Stz))      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is atom a column ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Card::bIsOrdinalColumnAtom(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szAtom = szGetProperty("atom"_Stz);

  char acFirst  = szAtom->azChar()[0];
  char acSecond = szAtom->azChar()[1];

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

  Stz* szAtom = szGetProperty("atom"_Stz);

  char acFirst  = szAtom->azChar()[0];
  char acSecond = szAtom->azChar()[1];

  if ('$' == acFirst && ('I' == acSecond || 'i' == acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is atom a column ordinal?                                                  */
/*----------------------------------------------------------------------------*/
bool Card::bIsOrdinalColumnTerm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szTerm = szGetProperty("term"_Stz);
  
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

  Stz* szTerm = szGetProperty("term"_Stz);

  char acFirst  = szTerm->azChar()[0];
  char acSecond = szTerm->azChar()[1];

  if ('$' == acFirst && ('I' == acSecond || 'i' == acSecond))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this atom same as argument?                                             */
/*----------------------------------------------------------------------------*/
bool Card::bIsSame(Stz* szArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szAtom = szGetProperty("atom"_Stz);

  if (szAtom->bSame(szArg))
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

  Stz* szAtom = szGetProperty("atom"_Stz);

  if (szAtom->bSame(","_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get level                                                                  */
/*----------------------------------------------------------------------------*/
Stz* Card::szLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Stz* szLevel = szGetProperty("szLevel"_Stz);

  return szLevel;
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
i8 Card::iOrdinalColumnValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szAtom = szGetProperty("atom"_Stz);
  i8 i_Ord = StrLib::iGleanInt(szAtom->azChar());
  
  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* get ordinal index value                                                    */
/*----------------------------------------------------------------------------*/
i8 Card::iOrdinalIndexValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Stz* szAtom = szGetProperty("atom"_Stz);
  i8 i_Ord = StrLib::iGleanInt(szAtom->azChar());
  
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
/* get query name                                                             */
/*----------------------------------------------------------------------------*/
Stz* Card::szRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Stz* szRows = szGetProperty("szRows"_Stz);

  return szRows;
}

/*----------------------------------------------------------------------------*/
/* set query name                                                             */
/*----------------------------------------------------------------------------*/
void Card::vRows(Stz* szRows)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vAddProperty("szRows"_Stz,szRows);
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
eYG Card::eStageTopology(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  eYG eStageTopology = (eYG)iGetProperty("eStageTopology"_Stz);

  return eStageTopology;
}

/*----------------------------------------------------------------------------*/
/* set stage topology                                                         */
/*----------------------------------------------------------------------------*/
void Card::vStageTopology(eYG  eStageTopology)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  vAddProperty("eStageTopology"_Stz,eStageTopology);
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
/* return type                                                                */
/*----------------------------------------------------------------------------*/
Stz* Card::szType(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Stz* szType = szGetProperty("szType"_Stz);

  return szType;
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
void Card::vPrintCard(Ribbon* pRibbon, i8 iDepth, Board* pBoard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  s32 sName;
    
  if (m_bPrintNm && m_sCardNm.nLength()) {
    
    if (-1 == m_iCardNo)
      sName.vSprint("%s%s x%lld"_Stz
                    ,Stz::szDots(iDepth)
                    ,m_sCardNm.szStz()
                    ,nItems()
                    );
    else
      sName.vSprint("%s%lld)%s"_Stz
                    ,Stz::szDots(iDepth)
                    ,m_iCardNo
                    ,m_sCardNm.szStz()
                    );

    pRibbon->vSprint("%s"_Stz,sName.szStz());
  }
  
  vPrintDetails(pRibbon,iDepth);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ListExoIter2_t<Card>* pCard_I = pNewIter();
  while (pCard_I->bHasNext()) {
    Card* pCard = pCard_I->pNext();

    pCard->vPrintCard(pRibbon,iDepth + 1,pBoard);
  }

  delete pCard_I;
}


/*----------------------------------------------------------------------------*/
/* print card onto given ribbon                                               */
/*----------------------------------------------------------------------------*/
void Card::vPrintDetails(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  eCK eCard = this->eCard();
  
  switch (eCard) {

    case CK_AGGREG:        vPrintProperties(pRibbon,iDepth); break;
    case CK_AGGREGPRED:    vPrintProperties(pRibbon,iDepth); break;
    case CK_ANDPRED:       vPrintProperties(pRibbon,iDepth); break;
    case CK_AS:            vPrintProperties(pRibbon,iDepth); break;
      
    case CK_BY:            vPrintProperties(pRibbon,iDepth); break;
    case CK_BYLIST:        vPrintProperties(pRibbon,iDepth); break;
    case CK_BYPRED:        vPrintProperties(pRibbon,iDepth); break;
      
    case CK_COMBINE:       vPrintProperties(pRibbon,iDepth); break;
    case CK_COMPAREPRED:   vPrintProperties(pRibbon,iDepth); break;
    case CK_CORRSUBQUERY:  vPrintProperties(pRibbon,iDepth); break;
    case CK_CREATETABLE:   vPrintProperties(pRibbon,iDepth); break;
      
    case CK_DEBUG:         vPrintProperties(pRibbon,iDepth); break;
    case CK_DROPTABLE:     vPrintProperties(pRibbon,iDepth); break;
      
    case CK_EXISTS:        vPrintProperties(pRibbon,iDepth); break;
    case CK_EXPR:          vPrintProperties(pRibbon,iDepth); break;
    case CK_FILTER:        vPrintProperties(pRibbon,iDepth); break;
      
    case CK_HASH:          vPrintProperties(pRibbon,iDepth); break;
    case CK_HASHCAST:      vPrintProperties(pRibbon,iDepth); break;
    case CK_HAVING:        vPrintProperties(pRibbon,iDepth); break;
       
    case CK_INDEX:         vPrintProperties(pRibbon,iDepth); break;
    case CK_INDEXJOIN:     vPrintProperties(pRibbon,iDepth); break;
    case CK_INDEXPRED:     vPrintProperties(pRibbon,iDepth); break;
    case CK_INDEXSCAN:     vPrintProperties(pRibbon,iDepth); break;
      
    case CK_LIMIT:         vPrintProperties(pRibbon,iDepth); break;
      
    case CK_MAINSUBQUERY:  vPrintProperties(pRibbon,iDepth); break;
      
    case CK_NODECAST:      vPrintProperties(pRibbon,iDepth); break;
    case CK_NONCSUBQUERY:  vPrintProperties(pRibbon,iDepth); break;
      
    case CK_ON:            vPrintProperties(pRibbon,iDepth); break;
    case CK_ORDAGG:        vPrintProperties(pRibbon,iDepth); break;
    case CK_ORIGINCAST:    vPrintProperties(pRibbon,iDepth); break;
    case CK_ORPRED:        vPrintProperties(pRibbon,iDepth); break;
      
    case CK_PREDICATE:     vPrintProperties(pRibbon,iDepth); break;
    case CK_PROJECT:       vPrintProperties(pRibbon,iDepth); break;
    case CK_PROJECTION:    vPrintProperties(pRibbon,iDepth); break;
    case CK_PROJECTPRED:   vPrintProperties(pRibbon,iDepth); break;
      
    case CK_SCAN:          vPrintProperties(pRibbon,iDepth); break;
    case CK_SEND:          vPrintProperties(pRibbon,iDepth); break;
    case CK_SORT:          vPrintProperties(pRibbon,iDepth); break;
    case CK_STAGE:         vPrintProperties(pRibbon,iDepth); break;
    case CK_STREAMDEF:     vPrintProperties(pRibbon,iDepth); break;
      
    case CK_TABLE:         vPrintProperties(pRibbon,iDepth); break;
    case CK_TERM:          vPrintProperties(pRibbon,iDepth); break;
    case CK_TERMLIST:      vPrintProperties(pRibbon,iDepth); break;
    case CK_TYPE:          vPrintProperties(pRibbon,iDepth); break;

    default:
      vTrace("%s: Unknown enum %lld\n"_Stz,__func__,(i8)eCard);
      break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* print by piece                                                             */
/*----------------------------------------------------------------------------*/
void Card::vPrintProperties(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! m_pProperties)
    return;
  
  eTN eTkn = eProperty(0);

  switch (eTkn) {
      
    case TN_INTEGER: {
      pRibbon->vSput(FR_TEXT,"TN_INTEGER"_Stz);
      break;
    }
      
    case TN_OBJECT: {
      pRibbon->vSput(FR_TEXT,"TN_OBJECT"_Stz);
      break;
    }
      
    case TN_STRING: {
      Stz* szProperty = szGetProperty((i8)0);
      pRibbon->vSput(FR_TEXT,szProperty);
      break;
    }
      
    default:
      break;
  }
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* print table                                                                */
/*----------------------------------------------------------------------------*/
void Card::vIndexPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Card_Index* pCardIndex = _pCard_Index(this);
  
  pRibbon->vSput(FR_NAME,pCardIndex->pIndex()->szIndexNm());
  
  IndexDef* pIndexDef = pCardIndex->pIndex()->pIndexDef();
  pRibbon->vSput(FR_TEXT
                 ,pIndexDef->bIsUnique()
                    ? "unique"_Stz : "not unique"_Stz
                 );
  
  i8 nColumns = pIndexDef->nColumns();
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    Column* pColumn = pIndexDef->pGetColumn(i_Col);
    
    pRibbon->vSput(FR_NAME,pColumn->szColNm());
  }
}

/*----------------------------------------------------------------------------*/
/* print on                                                                   */
/*----------------------------------------------------------------------------*/
void Card::vOnPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Card_On* pCardOn = _pCard_On(this);
  
  //pRibbon->vSprint("%15s"_Stz,"ON="_Stz);
  pRibbon->vSprint("%15s"_Stz,pCardOn->pColumn1()->szColNm());
  pRibbon->vSprint("%15s"_Stz,pCardOn->pColumn2()->szColNm());
}

/*----------------------------------------------------------------------------*/
/* print predicate                                                            */
/*----------------------------------------------------------------------------*/
void Card::vPredicatePrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Card_Predicate* pCard_Predicate = _pCard_Predicate(this);

  pRibbon->vSput(FR_DOTS
                 ,Stz::szDots(iDepth)
                 ,pCard_Predicate->m_iCardNo
                 ,pCard_Predicate->m_sCardNm.szStz()
                 );

}

/*----------------------------------------------------------------------------*/
/* print sort                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vSortPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Card_Sort* pCard_Sort = _pCard_Sort(this);
  
  s32 sBy;

  sBy.vSprint("+%s+"_Stz,pCard_Sort->pColumn()->szColNm());

  pRibbon->vSprint("%15s"_Stz,sBy.szStz());
}

/*----------------------------------------------------------------------------*/
/* print stage                                                                */
/*----------------------------------------------------------------------------*/
void Card::vStagePrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Card_Stage* pCard_Stage = _pCard_Stage(this);
  
  pRibbon->vSput(FR_NAME,pCard_Stage->szStageNm());
  pRibbon->vSput(FR_TEXT,aYG_15s[pCard_Stage->eStageTopology()]);

}

/*----------------------------------------------------------------------------*/
/* print stream def piece                                                     */
/*----------------------------------------------------------------------------*/
void Card::vStreamDefPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Card_StreamDef* pCard_StreamDef = _pCard_StreamDef(this);
  StreamDef* pStreamDef = pCard_StreamDef->pStreamDef();
  if (! pStreamDef)
    return;

  s32 sOrder;
  
  i8 nOrders = pStreamDef->nOrders();
  for (i8 i_Ord = 0; i_Ord < nOrders; i_Ord++) {
    Column* pColumn = pStreamDef->pGetOrder(i_Ord);
  
//    sOrder.vSprint("[%s]"_Stz,pColumn->szColNm());
//
//    pRibbon->vSprint("%15s"_Stz
//                     ,sOrder.szStz()
//                     );
    
    pRibbon->vSput(FR_ATOM_SQUARE
                     ,pColumn->szColNm()
                     );
  }

  
  i8 nColumns = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    Column* pColumn = pStreamDef->pGetColumn(i_Col);
    
    pRibbon->vSput(FR_SDEF_COLUMN
                     ,pColumn->szColNm()
                     ,pColumn->cTypeShort()
                     );
  }

}

/*----------------------------------------------------------------------------*/
/* print table card onto given ribbon                                         */
/*----------------------------------------------------------------------------*/
void Card::vTablePrint(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
  
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

  pRibbon->vSput(FR_SIZE,pStreamDef->jTableSize());

  
  //pCardTable->vPrintPrimary(pRibbon);
  IdxVec*   pIdxVec    = pTable->pIdxVec();
  Index*    pIndex     = pIdxVec->pGet(0);
  IndexDef* pIndexDef  = pIndex->pIndexDef();
  Column*   pIdxColumn = pIndexDef->pGetColumn(0);
  
  pRibbon->vSput(FR_ATOM_SQUARE,pIdxColumn->szColNm());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
