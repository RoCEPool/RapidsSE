/*============================================================================*/
/* Card.cpp -- Card of cards                                                  */
/*============================================================================*/

#include "Card.hpp"
#include "Board.hpp"

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
Card::Card(eCK eKind, eCN eClass)
: Object(eClass)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
        
  vInitialise(eKind,eClass);
  
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
/* initialisor                                                                */
/*----------------------------------------------------------------------------*/
void Card::vInitialise(eCK eKind, eCN eClass)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
      
  m_iCardNo  = -1;
  m_eCard    = eKind;
  m_sCardNm  = aCK_short[m_eCard];
  
  m_pProperties = nullptr;
  
  m_bPrintNm = true;
  
  m_fpDeleteThis = Card::vDeleteThis_;
  m_fpBoardPrint = nullptr;
  m_fpCardHeader = Card::vCardHeader_;
  
  switch (eKind) {
      
    case CK_INDEX:
      m_fpBoardPrint = &Card::vIndexPrint_;     break;
    case CK_ON:        m_fpBoardPrint = &Card::vOnPrint_;        break;
    case CK_PREDICATE: m_fpBoardPrint = &Card::vPredicatePrint_; break;
    case CK_SORT:      m_fpBoardPrint = &Card::vSortPrint_;      break;
    case CK_STAGE:     m_fpBoardPrint = &Card::vStagePrint_;     break;
    case CK_STREAMDEF: m_fpBoardPrint = &Card::vStreamDefPrint_; break;

    default:
      m_fpBoardPrint = &Card::vCardPrint_;
  }
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
/* print by piece                                                             */
/*----------------------------------------------------------------------------*/
void Card::vCardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_
  
  if (! pCard->m_pProperties)
    return;
  
  eTN eTkn = pCard->eProperty(0);

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
      Stz* szProperty = pCard->szGetProperty((i8)0);
      pRibbon->vSput(FR_TEXT,szProperty);
      break;
    }
      
    default:
      break;
  }
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
void Card::vBoardPrint(Ribbon* pRibbon, i8 iDepth, Board* pBoard)
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
  
  if (CK_TABLE == m_eCard)
    pBoard->vPrintTable(this,pRibbon);
  else
  if (m_fpBoardPrint)
    m_fpBoardPrint(this,pRibbon,iDepth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ListExoIter2_t<Card>* pCard_I = pNewIter();
  while (pCard_I->bHasNext()) {
    Card* pCard = pCard_I->pNext();

    pCard->vBoardPrint(pRibbon,iDepth + 1,pBoard);
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
void Card::vAddProperty(Stz* szName, i8 iValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;
  
  m_pProperties->vReplace(szName,iValue);
}

/*----------------------------------------------------------------------------*/
/* add given property                                                         */
/*----------------------------------------------------------------------------*/
void Card::vAddProperty(Stz* szName, Object* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  if (! m_pProperties)
    m_pProperties = new TagSet;
  
  m_pProperties->vReplace(szName,pValue);
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
/* get card kind                                                              */
/*----------------------------------------------------------------------------*/
eCK Card::eCard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  return m_eCard;
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
/* get given property                                                         */
/*----------------------------------------------------------------------------*/
i8 Card::iGetProperty(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

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
/* get number of properties                                                   */
/*----------------------------------------------------------------------------*/
i8 Card::nProperties(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

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
/* print table                                                                */
/*----------------------------------------------------------------------------*/
void Card::vIndexPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_

  Card_Index* pCardIndex = _pCard_Index(pCard);
  
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
void Card::vOnPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_

  Card_On* pCardOn = _pCard_On(pCard);
  
  //pRibbon->vSprint("%15s"_Stz,"ON="_Stz);
  pRibbon->vSprint("%15s"_Stz,pCardOn->pColumn1()->szColNm());
  pRibbon->vSprint("%15s"_Stz,pCardOn->pColumn2()->szColNm());
}

/*----------------------------------------------------------------------------*/
/* print predicate                                                            */
/*----------------------------------------------------------------------------*/
void Card::vPredicatePrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_
  
  Card_Predicate* pCard_Predicate = _pCard_Predicate(pCard);

  pRibbon->vSput(FR_DOTS
                 ,Stz::szDots(iDepth)
                 ,pCard_Predicate->m_iCardNo
                 ,pCard_Predicate->m_sCardNm.szStz()
                 );

}

/*----------------------------------------------------------------------------*/
/* print sort                                                                 */
/*----------------------------------------------------------------------------*/
void Card::vSortPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_

  Card_Sort* pCard_Sort = _pCard_Sort(pCard);
  
  s32 sBy;

  sBy.vSprint("+%s+"_Stz,pCard_Sort->pColumn()->szColNm());

  pRibbon->vSprint("%15s"_Stz,sBy.szStz());
}

/*----------------------------------------------------------------------------*/
/* print stage                                                                */
/*----------------------------------------------------------------------------*/
void Card::vStagePrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_
  
  Card_Stage* pCard_Stage = _pCard_Stage(pCard);
  
  pRibbon->vSput(FR_NAME,pCard_Stage->szStageNm());
  pRibbon->vSput(FR_TEXT,aYG_15s[pCard_Stage->eStageTopology()]);

}

/*----------------------------------------------------------------------------*/
/* print stream def piece                                                     */
/*----------------------------------------------------------------------------*/
void Card::vStreamDefPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
{
  _static_

  Card_StreamDef* pCard_StreamDef = _pCard_StreamDef(pCard);
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
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get aggregate level                                                        */
/*----------------------------------------------------------------------------*/
eAL Card::eAggLvl(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  eAL eAggLvl = (eAL)iGetProperty("AggLvl"_Stz);
  
  return eAggLvl;
}


/*----------------------------------------------------------------------------*/
/* test if the expr has an aggregate op                                       */
/*----------------------------------------------------------------------------*/
bool Card::bHasAggOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  Index* pIndex = _pIndex(pGetProperty("pIndex"_Stz));

  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* set index                                                                  */
/*----------------------------------------------------------------------------*/
void Card::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  vAddProperty("pIndex"_Stz,pIndex);
}

/*----------------------------------------------------------------------------*/
/* is this atom an aggregate op?                                              */
/*----------------------------------------------------------------------------*/
bool Card::bIsAggOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  Stz* szAtom = szGetProperty("atom"_Stz);

  char acFirst  = szAtom->azChar()[0];
  char acSecond = szAtom->azChar()[1];

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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

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
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD_ATOM,_FOOTER_);

  Stz* szAtom = szGetProperty("atom"_Stz);

  if (szAtom->bSame(","_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get ordinal column value                                                   */
/*----------------------------------------------------------------------------*/
i8 Card::iOrdinalColumnValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  Stz* szAtom = szGetProperty("atom"_Stz);
  i8 i_Ord = StrLib::iGleanInt(szAtom->azChar());
  
  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* get ordinal index value                                                    */
/*----------------------------------------------------------------------------*/
i8 Card::iOrdinalIndexValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  Stz* szAtom = szGetProperty("atom"_Stz);
  i8 i_Ord = StrLib::iGleanInt(szAtom->azChar());
  
  return i_Ord;
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
