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
Drafter::Drafter(Bench* pBench)
: OptimiserTool(CN_DRAFTER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  m_pBench        = pBench;
  m_pOptimiser    = m_pBench->pOptimiser();

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
/* clear                                                                      */
/*----------------------------------------------------------------------------*/
bool Drafter::bClear(TokenVec* pTknVec, eOA& reOptAct)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  Board* pBoard = m_pOptimiser->pHeadBoard();
  
  pBoard->vClear();

  reOptAct = OA_ENTRY;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* compare                                                                    */
/*----------------------------------------------------------------------------*/
bool Drafter::bCmp(TokenVec* pTknVec, eOA& reOptAct)
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
bool Drafter::bCut(TokenVec* pTknVec, eOA& reOptAct)
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
      pBoard->pCard_SubSet()->vCutSubCard(pMainCard);
      delete pMainCard; //TEMP
    }
    else
    if (pSubCard) {
      Card* pParentCard = pBoard->pCard_SubSet()->pFindParent(pSubCard);
      pParentCard->vCutSubCard(pSubCard);
      
      delete pSubCard; //TEMP
    }
  }
  
  //pBoard->m_pCard_SubSet->vRenumberMainCards();
  
  return true;
  
}

/*----------------------------------------------------------------------------*/
/* load                                                                       */
/*----------------------------------------------------------------------------*/
bool Drafter::bLoad(TokenVec* pTknVec, eOA& reOptAct)
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
  
  VertexSpec* pPipeSpec =
    Dynamo::pToVertexSpec(sPipelineStmt,
                            m_pOptimiser->pJobTags());

  m_pOptimiser->vToPipeSpec(pPipeSpec);

  return true;
}

/*----------------------------------------------------------------------------*/
/* name                                                                       */
/*----------------------------------------------------------------------------*/
bool Drafter::bName(TokenVec* pTknVec, eOA& reOptAct)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  Board* pBoard = m_pOptimiser->pHeadBoard();

  i8 nTkns = pTknVec->nItems();
  
  if (nTkns < 2) {
    vTrace("QueryNm : %s\n"_Stz,m_pOptimiser->szQueryNm());
    return true;
  }
  
  Stz* szQueryNm = pTknVec->pGet(1)->szChars();
  m_pOptimiser->vQueryNm(szQueryNm);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* reprint                                                                    */
/*----------------------------------------------------------------------------*/
bool Drafter::bReprint(TokenVec* pTknVec, eOA& reOptAct)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);
  
  reOptAct = OA_ENTRY;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set                                                                        */
/*----------------------------------------------------------------------------*/
bool Drafter::bSet(TokenVec* pTknVec, eOA& reOptAct)
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
/* weave                                                                      */
/*----------------------------------------------------------------------------*/
bool Drafter::bWeave(TokenVec* pTknVec, eOA& reOptAct)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  vTrace("%s\n"_Stz
         ,m_pOptimiser->szWeave()
         );
  
  reOptAct = OA_INPUT;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* write                                                                      */
/*----------------------------------------------------------------------------*/
bool Drafter::bWrite(TokenVec* pTknVec, eOA& reOptAct)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  Stz* szStmt = m_pOptimiser->szWeave();
  Stz* szQueryNm = m_pOptimiser->szQueryNm();

  s256 sPath;
  sPath.vSprint("/tmp/%s.txt"_Stz,szQueryNm);
  
  File* pFile = new File(m_pOptimiser);
  
  pFile->bOpenWrite(sPath);
  
  pFile->vPrint("%s"_Stz,szStmt);

  pFile->vClose();
  
  delete pFile;
  
  vTrace("%s written\n"_Stz,sPath.szStz());
  
  reOptAct = OA_INPUT;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
