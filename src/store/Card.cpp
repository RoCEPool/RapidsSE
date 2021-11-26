/*============================================================================*/
/* Card.cpp -- Card of cards                                                  */
/*============================================================================*/

#include "Card.hpp"

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
Card::Card(eCN eClass,  Stz* szCardNm, Stz* szText)
: Object(eClass)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
      
  m_iCardNo  = -1;
  m_sCardNm  = szCardNm;
  m_eKind    = CK_NONE;
  m_sText    = szText;
  
  m_pProperties = nullptr;
  
  m_bPrintNm = true;
  
  m_fpDeleteThis = nullptr;
  m_fpBoardPrint = nullptr;
  m_fpCardHeader = Card::vCardHeader_;

  _postcondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card::~Card(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  for (Card* pCard = pPop(); pCard; pCard = pPop())
    pCard->vDelete();
  
  delete m_pProperties;
}



/*----------------------------------------------------------------------------*/
/* put header                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vCardHeader_(Card* pCard, Ribbon* pRibbon)
{
  _static_

  pRibbon->vSput(FR_HEAD,pRibbon->iLogicalColumn());
}



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Card::vDelete(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  m_fpDeleteThis(this);
}


/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Card::vBoardPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
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
  
  if (m_fpBoardPrint)
    m_fpBoardPrint(this,pRibbon,iDepth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ListExoIter2_t<Card>* pCard_I = pNewIter();
  while (pCard_I->bHasNext()) {
    Card* pCard = pCard_I->pNext();

    pCard->vBoardPrint(pRibbon,iDepth + 1);
  }

  delete pCard_I;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::vAddProperty(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;
  
  m_pProperties->vReplace(szName,"yes"_Stz);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::vAddProperty(Stz* szName, Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;
  
  m_pProperties->vReplace(szName,szValue);
}

/*----------------------------------------------------------------------------*/
/* get card no                                                                */
/*----------------------------------------------------------------------------*/
i8 Card::iCardNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  return m_iCardNo;
}

/*----------------------------------------------------------------------------*/
/* set card no                                                                */
/*----------------------------------------------------------------------------*/
void Card::vCardNo(i8 iCardNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  m_iCardNo = iCardNo;
}

/*----------------------------------------------------------------------------*/
/* get card name                                                              */
/*----------------------------------------------------------------------------*/
Stz* Card::szCardNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  return m_sCardNm;
}

/*----------------------------------------------------------------------------*/
/* cut given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::vCutProperty(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  if (! m_pProperties)
    return;
  
  m_pProperties->vDelete(szName);
}

/*----------------------------------------------------------------------------*/
/* call closure for each sub-card                                             */
/*----------------------------------------------------------------------------*/
void Card::vEachSubCard(std::function<void(Card* pCard)> f_vClosure,
                        eCN eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ListExoIter2_t<Card>* pCard_I = pNewIter();

  while (pCard_I->bHasNext()) {
    Card* pSubCard = pCard_I->pNext();
    
    if (CN_NONE == eClassNo || eClassNo == pSubCard->eGetClassNo())
      f_vClosure(pSubCard);
  }
  
  delete pCard_I;
}

/*----------------------------------------------------------------------------*/
/* get card name                                                              */
/*----------------------------------------------------------------------------*/
Card* Card::pFindSubCard(eCN eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();
  
  Card* pFoundCard = nullptr;
  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();
    
    if (eClassNo == pSubCard->eGetClassNo()) {
      pFoundCard = pSubCard;
      break;
    }
  }
  
  delete pSubCard_I;
  
  return pFoundCard;
}

/*----------------------------------------------------------------------------*/
/* does card have given property ?                                            */
/*----------------------------------------------------------------------------*/
bool Card::bHasProperty(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  if (! m_pProperties)
    return false;

  bool bHas = m_pProperties->bHasName(szName);

  return bHas;
}

/*----------------------------------------------------------------------------*/
/* get card kind                                                              */
/*----------------------------------------------------------------------------*/
eCK Card::eKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  return m_eKind;
}

/*----------------------------------------------------------------------------*/
/* get i8 of text                                                             */
/*----------------------------------------------------------------------------*/
i8 Card::iOfText(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  return m_sText.iToI8();
}

/*----------------------------------------------------------------------------*/
/* get i8 of text                                                             */
/*----------------------------------------------------------------------------*/
i8 Card::nOfText(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  return m_sText.iToI8();
}

/*----------------------------------------------------------------------------*/
/* renumber cards                                                             */
/*----------------------------------------------------------------------------*/
void Card::vRenumberSubCards(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
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
Stz* Card::sz(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  return m_sText;
}

/*----------------------------------------------------------------------------*/
/* get text                                                                   */
/*----------------------------------------------------------------------------*/
Stz* Card::szText(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  return m_sText;
}

/*----------------------------------------------------------------------------*/
/* set text                                                                   */
/*----------------------------------------------------------------------------*/
void Card::vText(Stz* szText)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  m_sText = szText;
}

/*----------------------------------------------------------------------------*/
/* transfer all sub-cards from given card set to this card                    */
/*----------------------------------------------------------------------------*/
void Card::vTransferAll(Card* pCards)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  while (Card* pCard = pCards->pPop())
    vAppend(pCard);
}

/*----------------------------------------------------------------------------*/
/* transfer cards to this card from given card onwards                        */
/*----------------------------------------------------------------------------*/
void Card::vTransferFrom(Card* pCards, Card* pFrom,
                         Card* pAt, bool bAfter)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  f_vClosure(this);
  
  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();
    
    pSubCard->vWalkAllCards(f_vClosure);
  }
  delete pSubCard_I;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
