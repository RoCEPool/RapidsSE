/*============================================================================*/
/* Drafter.cpp --                                                             */
/*============================================================================*/

#include "Drafter.hpp"
#include "Optimiser.hpp"
#include "Dynamo.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Drafter,CN_DRAFTER)
__aNewFunction(Drafter,CN_DRAFTER)
__vDeleteFunction(Drafter,CN_DRAFTER)
__aFooterFunction(Drafter)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Drafter::Drafter(Optimiser* pOptimiser)
: Object(CN_DRAFTER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);
    
  m_pOptimiser = pOptimiser;
  m_pCardCtl   = m_pOptimiser->pCardCtl();

  _checkInitialised_(CN_DRAFTER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Drafter::~Drafter(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the ruler                                                           */
/*----------------------------------------------------------------------------*/
void Drafter::vRepair(Object* pObject)
{
  _static_

  Drafter* pDrafter = _pDrafter(pObject);

  delete pDrafter;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add                                                                        */
/*----------------------------------------------------------------------------*/
bool Drafter::bAdd(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2)
    return false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
   if (pTknVec->pGet(1)->bSame("*"_Stz)) {
    for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {
      m_pCardCtl->vPrint(i_Card,1);
    }
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Tkn = 1; i_Tkn < nTkns; i_Tkn++) {
    Stz* szName = pTknVec->pGet(i_Tkn)->szChars();
    eCK eCard = Card::eToECard(szName);
    if (CK_NONE == eCard)
      continue;
    
    m_pCardCtl->vPrint(eCard,1);
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* below                                                                      */
/*----------------------------------------------------------------------------*/
bool Drafter::bBelow(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  i8 nTkns = pTknVec->nItems();
  if (pTknVec->nItems() < 2)
    return false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pTknVec->pGet(1)->bSame("*"_Stz)) {
    for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {
      m_pCardCtl->vBelow(i_Card,1);
    }
    
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Tkn = 1; i_Tkn < nTkns; i_Tkn++) {
    Stz* szName = pTknVec->pGet(i_Tkn)->szChars();
    eCK eCard = Card::eToECard(szName);
    if (CK_NONE == eCard)
      continue;
    
    m_pCardCtl->vBelow(eCard,1);
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* clear                                                                      */
/*----------------------------------------------------------------------------*/
bool Drafter::bClear(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  Board* pBoard = m_pOptimiser->pHeadBoard();
  
  pBoard->vClear();

  riAction = 1;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* compare                                                                    */
/*----------------------------------------------------------------------------*/
bool Drafter::bCmp(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  i8 nTkns = pTknVec->nItems();
  if (nTkns < 3) {
    vTrace("Error -- usage is: cmp <card> <card>\n"_Stz);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Board* pBoard = m_pOptimiser->pHeadBoard();

  Token* pTkn_A = pTknVec->pGet(1);
  Stz* szCard_A = pTkn_A->szChars();

  Card* pCard_A = pBoard->pSearch(szCard_A);
  if (! pCard_A) {
    vTrace("Error -- no such card: %s\n"_Stz,szCard_A);
    return false;
  }

  Token* pTkn_B = pTknVec->pGet(2);
  Stz* szCard_B = pTkn_B->szChars();

  Card* pCard_B = pBoard->pSearch(szCard_B);
  if (! pCard_B) {
    vTrace("Error -- no such card: %s\n"_Stz,szCard_B);
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pCard_A->bEquivalent(pCard_B)) {
    pCard_A->vMark1('E');
    pCard_A->vMark2('Q');
    pCard_B->vMark1('E');
    pCard_B->vMark2('Q');
  }
  else {
    pCard_A->vMark1('N');
    pCard_A->vMark2('E');
    pCard_B->vMark1('N');
    pCard_B->vMark2('E');
  }
  return true;
}

/*----------------------------------------------------------------------------*/
/* cut                                                                        */
/*----------------------------------------------------------------------------*/
bool Drafter::bCut(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2) {
    vTrace("Error -- usage is: cut <card>\n"_Stz);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Card* pMainCard = nullptr;
  Card* pSubCard = nullptr;
  Board* pBoard = m_pOptimiser->pHeadBoard();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Tkn = 1; i_Tkn < nTkns; i_Tkn++) {
    Token* pTkn = pTknVec->pGet(i_Tkn);
    Stz* szCard = pTkn->szChars();

    pBoard->vSearch(szCard,pMainCard,pSubCard);
    
    if (pMainCard) {
      pBoard->vCutSubCard(pMainCard);
      delete pMainCard; //TEMP
    }
    else
    if (pSubCard) {
      Card* pParentCard = pBoard->pFindParent(pSubCard);
      pParentCard->vCutSubCard(pSubCard);
      
      delete pSubCard; //TEMP
    }
  }
  
  pBoard->vRenumberCards();
  
  return true;
  
}

/*----------------------------------------------------------------------------*/
/* depth                                                                      */
/*----------------------------------------------------------------------------*/
bool Drafter::bDepth(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  i8 iDepth = m_pCardCtl->iDepth();
  
  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2) {
    vTrace("depth : %lld\n"_Stz,iDepth);
    riAction = 1;
    return true;
  }
  
  iDepth = pTknVec->pGet(1)->iToI8();
  //if (iDepth < 1)
  //  iDepth = 99;
  
  m_pCardCtl->vDepth(iDepth);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* form                                                                       */
/*----------------------------------------------------------------------------*/
bool Drafter::bForm(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  i8 nTkns = pTknVec->nItems();
  if (nTkns < 3)
    return false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iForm = pTknVec->pGet(1)->iToI8();
  
  if (pTknVec->pGet(2)->bSame("*"_Stz)) {
    for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {
      m_pCardCtl->vForm(i_Card,iForm);
    }
    
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Tkn = 2; i_Tkn < nTkns; i_Tkn++) {
    Stz* szName = pTknVec->pGet(i_Tkn)->szChars();
    eCK eCard = Card::eToECard(szName);
    if (CK_NONE == eCard)
      continue;
    
    m_pCardCtl->vForm(eCard,iForm);
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* add                                                                        */
/*----------------------------------------------------------------------------*/
bool Drafter::bHide(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pTknVec->pGet(1)->bSame("*"_Stz)) {
    for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {
      m_pCardCtl->vBelow(i_Card,0);
    }
    
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Tkn = 1; i_Tkn < nTkns; i_Tkn++) {
    Stz* szName = pTknVec->pGet(i_Tkn)->szChars();
    eCK eCard = Card::eToECard(szName);
    if (CK_NONE == eCard)
      return false;
    
    m_pCardCtl->vBelow(eCard,0);
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* load                                                                       */
/*----------------------------------------------------------------------------*/
bool Drafter::bLoad(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  Board* pBoard = m_pOptimiser->pHeadBoard();
  pBoard->vClear();

  s256 sPath;
  sPath.vSprint("/tmp/%s"_Stz,"unamed.txt"_Stz);
  
  File* pFile = new File(m_pOptimiser);

  bool bOpened = pFile->bOpenRead(sPath);
  if (! bOpened) {
    pFile->vClose();
    delete pFile;
    return false;
  }

  s48K sPipelineStmt;
  i8 nBytesRead = pFile->nReadFile(sPipelineStmt);
  if (! nBytesRead) {
    pFile->vClose();
    delete pFile;
    return false;
  }

  vTrace("Loaded (%lld bytes):\n%s\n"_Stz
         ,nBytesRead
         ,sPipelineStmt.szStz());

  pFile->vClose();
  delete pFile;
  
  PipelineSpec* pPipelineSpec =
    Dynamo::pToPipelineSpec(sPipelineStmt,
                            m_pOptimiser->pJobTags());

  m_pOptimiser->vToPipelineSpec(pPipelineSpec);

  return true;
}

/*----------------------------------------------------------------------------*/
/* name                                                                       */
/*----------------------------------------------------------------------------*/
bool Drafter::bName(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  Board* pBoard = m_pOptimiser->pHeadBoard();

  i8 nTkns = pTknVec->nItems();
  
  if (nTkns < 2) {
    vTrace("QueryNm : %s\n"_Stz,pBoard->szQueryNm());
    return true;
  }
  
  Stz* szQueryNm = pTknVec->pGet(1)->szChars();
  pBoard->vQueryNm(szQueryNm);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* reprint                                                                    */
/*----------------------------------------------------------------------------*/
bool Drafter::bReprint(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);
  
  riAction = 0;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set                                                                        */
/*----------------------------------------------------------------------------*/
bool Drafter::bSet(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2) {
    vTrace("Error -- usage is: set <card>\n"_Stz);
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card* pMainCard = nullptr;
  Card* pSubCard = nullptr;
  Board* pBoard = m_pOptimiser->pHeadBoard();
  CardRegs* pCardRegs = pBoard->pCardRegs();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Tkn = 1; i_Tkn < nTkns; i_Tkn++) {
    Token* pTkn = pTknVec->pGet(i_Tkn);
    Stz* szCard = pTkn->szChars();

    pBoard->vSearch(szCard,pMainCard,pSubCard);
      
    if (pMainCard) {
      pCardRegs->vCardReg(pMainCard);
    }
    else
    if (pSubCard) {
      pCardRegs->vCardReg(pSubCard);
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* sub                                                                        */
/*----------------------------------------------------------------------------*/
bool Drafter::bSub(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pTknVec->pGet(1)->bSame("*"_Stz)) {
    for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {
      if (Card::bIsMain((eCK)i_Card))
        continue;
      m_pCardCtl->vPrint(i_Card,0);
    }
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Tkn = 1; i_Tkn < nTkns; i_Tkn++) {
    Stz* szName = pTknVec->pGet(i_Tkn)->szChars();
    eCK eCard = Card::eToECard(szName);
    if (CK_NONE == eCard)
      continue;
    
    m_pCardCtl->vPrint(eCard,0);
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* weave                                                                      */
/*----------------------------------------------------------------------------*/
bool Drafter::bWeave(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  vTrace("%s\n"_Stz
         ,m_pOptimiser->szWeave()
         );
  
  riAction = 1;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* write                                                                      */
/*----------------------------------------------------------------------------*/
bool Drafter::bWrite(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  Stz* szStmt = m_pOptimiser->szWeave();
  Stz* szQueryNm = m_pOptimiser->pHeadBoard()->szQueryNm();

  s256 sPath;
  sPath.vSprint("/tmp/%s.txt"_Stz,szQueryNm);
  
  File* pFile = new File(m_pOptimiser);
  
  pFile->bOpenWrite(sPath);
  
  pFile->vPrint("%s"_Stz,szStmt);

  pFile->vClose();
  
  delete pFile;
  
  vTrace("%s written\n"_Stz,sPath.szStz());
  
  riAction = 1;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
