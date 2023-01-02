/*============================================================================*/
/* Xgearing.cpp --                                                            */
/*============================================================================*/

#include "Xgearing.hpp"

#include "Job.hpp"
#include "Cue.hpp"

#include "Optimiser.hpp"
#include "OptimiserFun.hpp"
#include "OptimiserCatalog.hpp"

extern OptimiserCatalog gxOptimiserCatalog;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Xgearing)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xgearing::Xgearing(eCN eClass, Bench* pBench)
: CardIter(eClass)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pBench   = pBench;
  m_pCardCtl = m_pBench->pCardCtl();

  Job* pJob = MY::pJob();
  TagSet* pJobTags = pJob->pJobTags();

  m_iXform     = 0;
  m_iTrace     = 0;

  m_pPermGen   = nullptr;

  m_pColumn    = nullptr;
  m_pIdxVec    = nullptr;
  m_pIndex     = nullptr;
  m_pIndexDef  = nullptr;
  m_pStreamDef = nullptr;
  m_pTable     = nullptr;

  m_pCard_child    = nullptr;
  m_pCard_copied   = nullptr;
  m_pCard_dup      = nullptr;
  m_pCard_found    = nullptr;
  m_pCard_located  = nullptr;
  m_pCard_member   = nullptr;
  m_pCard_new      = nullptr;
  m_pCard_parent   = nullptr;
  m_pCard_popped   = nullptr;
  m_pCard_resolved = nullptr;
  m_pCard_temp     = nullptr;
  m_pCard_yanked   = nullptr;


  // the M iterator - - - - - - - - - - - - - - - - - - - - - -

  G.pBoard    = nullptr;
  G.pDeck     = nullptr;
  G.pSubQuery = nullptr;
  G_vClearMembers();
  G_vClearRegisters();

  _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xgearing::~Xgearing(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  //delete m_pM;
  delete m_pPermGen;

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get board                                                                  */
/*----------------------------------------------------------------------------*/
Board* Xgearing::pBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  return G.pBoard;
}

/*----------------------------------------------------------------------------*/
/* cue user                                                                   */
/*----------------------------------------------------------------------------*/
eOA Xgearing::eCueUser(Stz* szPrompt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  TokenVec* pTknVec = nullptr;

  if (m_pCardCtl->bCueDisabled())
    return OA_CONTINUE_TRANSFORMING;

  szPrompt->vConcat(" >> "_Stz);

  m_pCardCtl->vCued(true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_Input:

  s4K sInput;
  Cue::vGetInput(szPrompt,sInput);

  if (0 == sInput.nLength())
    return OA_CONTINUE_TRANSFORMING;

  if (sInput.nLength() > 1)
    vDebugPoint();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(! pTknVec);
  pTknVec = AM::TOKENISE::pTokeniseSen(sInput);
  i8 nTokens = pTknVec->nTokens();

  if (0 == nTokens)
    return OA_CONTINUE_TRANSFORMING;

  OptimiserFun* pOptFun = nullptr;
  if (1 == nTokens)
    pOptFun = gxOptimiserCatalog.pFindFun(pTknVec->pGet(0)->szChars(),
                                          nullptr);
  else
    pOptFun = gxOptimiserCatalog.pFindFun(pTknVec->pGet(0)->szChars(),
                                          pTknVec->pGet(1)->szChars());
  if (! pOptFun) {
    vTrace("Unknown command\n"_Stz);
    delete pTknVec;
    pTknVec = nullptr;
    goto L_Input;
  }

  eOA eOptAct = OA_CONTINUE_TRANSFORMING;
  bool bExecuted = pOptFun->m_fpOptFun(m_pBench->pOptimiser(),pTknVec,eOptAct);

  delete pTknVec;
  pTknVec = nullptr;

  if (OA_INPUT_AGAIN == eOptAct)
    goto L_Input;

  if (OA_DISABLE_CUEING == eOptAct)
    m_pCardCtl->vCueDisabled(true);

  return eOptAct;
}

/*----------------------------------------------------------------------------*/
/* end of reshape                                                             */
/*----------------------------------------------------------------------------*/
void Xgearing::vEndReshape(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* end of transformation                                                      */
/*----------------------------------------------------------------------------*/
void Xgearing::vEndTransform(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.pDeck->vRenumberRecursive();

  //m_pCardCtl->vUseXform();
  // UNDO: Card::vConsistencyCheck(m_pDeck, m_iXform);

  vPrintBoard();

  //m_pCardCtl->vUseDefault();


  CardIter xCI(G.pDeck);
  while ((G.p_MainCard = xCI.pEachChild()))
    G.p_MainCard->Y_vCut(P_Focus);
}

/*----------------------------------------------------------------------------*/
/* focus on main                                                              */
/*----------------------------------------------------------------------------*/
void Xgearing::vFocusOnMain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.e_MainCard = G.p_MainCard->eCard();
  G.i_MainCard = this->i_Child();

  G.p_MainCard->Y_vSet(P_Focus);
}

/*----------------------------------------------------------------------------*/
/* print all bards of bench                                                   */
/*----------------------------------------------------------------------------*/
void Xgearing::vPrintBench(eBK eBoard, i8 i_Board)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! m_pCardCtl->bBenchPrint())
    return;

  char cPrefix = m_pBench->cPrefix();

  s256 sPrompt;

  i8 nBoards = m_pBench->nItems();

  for (;;) {
    if (C_UNKNOWN != i_Board) {
      Board *pBoard = m_pBench->pGetBoard(eBoard, i_Board);
      pBoard->vPrintBoard(szXhand());
    }
    else {
      for (i8 i_Each = 0; m_pBench->bBoardExists(eBoard, i_Each); i_Each++) {
        Board *pBoard = m_pBench->pGetBoard(eBoard, i_Each);
        pBoard->vPrintBoard(szXhand());
      }
    }

    if (! m_pCardCtl->bCueUser(szXhand(), C_NA)) {
      m_pCardCtl->vFormPrompt(sPrompt,
                              cPrefix, szXhand(), C_NA,
                              i_Board, nBoards, C_UNKNOWN);
      printf("%s\n", sPrompt.azChar());
      break;
    }

    m_pCardCtl->vFormPrompt(sPrompt,
                            cPrefix, szXhand(), C_NA,
                            i_Board, nBoards, C_UNKNOWN);
    if (OA_REPRINT_BOARD != eCueUser(sPrompt))
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* print the board                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::vPrintBoard(i8 iLineNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! m_pCardCtl->bBoardPrint())
    return;

  char cPrefix = m_pBench->cPrefix();

  s256 sPrompt;

  i8 nBoards = m_pBench->nItems();

  for (;;) {
    G.pBoard->vPrintBoard(szXdesc());

    if (! m_pCardCtl->bCueUser(szXhand(), szXform())) {
      m_pCardCtl->vFormPrompt(sPrompt,
                              cPrefix, szXhand(), szXform(),
                              G.pBoard->i_Board(), nBoards, iLineNo);
      printf("%s\n", sPrompt.azChar());
      break;
    }

    m_pCardCtl->vFormPrompt(sPrompt,
                            cPrefix, szXhand(), szXform(),
                            G.pBoard->i_Board(), nBoards, iLineNo);
    if (OA_REPRINT_BOARD != eCueUser(sPrompt))
        break;
  }
}

/*----------------------------------------------------------------------------*/
/* begin and record reshape information                                       */
/*----------------------------------------------------------------------------*/
void Xgearing::vReshape(const char* azFunc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* set control of card printing by kind                                       */
/*----------------------------------------------------------------------------*/
void Xgearing::vSetPrintByKind(eCK eCard, bool bPrintByKind)
{
  m_pCardCtl->vPrintByKind(eCard,bPrintByKind);
}

/*----------------------------------------------------------------------------*/
/* set card print properties                                                  */
/*----------------------------------------------------------------------------*/
void Xgearing::vSetPrintProperties(eCK eCard, bool bPrintProperties)
{
  m_pCardCtl->vPrintProperties(eCard,bPrintProperties);
}

/*----------------------------------------------------------------------------*/
/* set card print subCards                                                     */
/*----------------------------------------------------------------------------*/
void Xgearing::vSetPrintSubCards(eCK eCard, bool bPrintSubCards)
{
  m_pCardCtl->vPrintSubCards(eCard,bPrintSubCards);
}

/*----------------------------------------------------------------------------*/
/* begin and record transformation information                                */
/*----------------------------------------------------------------------------*/
void Xgearing::vTransform(const char* azFunc, Stz* szXdesc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vXform((Stz*)azFunc);
  G.pBoard->vXform((Stz*)azFunc);
  vXdesc(szXdesc);

  m_iXform++;
  G.pBoard->vXform(m_iXform);

  //m_pCardCtl->vUseXform();

  this->vRestart();

  if (m_pCardCtl->bTracing(szXhand(), szXform())) {
    vTrace("\n\nTracing %s::%s\n"_Stz, szXhand(), szXform());
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Xgearing::iTransformDepth(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  return iSentinalsDepth();
}

/*----------------------------------------------------------------------------*/
/* set up which board to use                                                  */
/*----------------------------------------------------------------------------*/
void Xgearing::vUsingBoard(eBK eBoard, i8 i_Board)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.pBoard = m_pBench->pGetBoard(eBoard, i_Board);
  G.pDeck  = G.pBoard->pDeck();

  this->vChangeCard(G.pBoard->pDeck());
}

/*----------------------------------------------------------------------------*/
/* get description of transformation                                          */
/*----------------------------------------------------------------------------*/
Stz* Xgearing::szXdesc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  return m_sXdesc;
}

/*----------------------------------------------------------------------------*/
/* set description of transformation                                          */
/*----------------------------------------------------------------------------*/
void Xgearing::vXdesc(Stz* szDescription)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_sXdesc = szDescription;
  G.pBoard->vXdesc(szDescription);
}

/*----------------------------------------------------------------------------*/
/* get transformation                                                         */
/*----------------------------------------------------------------------------*/
Stz* Xgearing::szXform(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  return m_sXform;
}

/*----------------------------------------------------------------------------*/
/* get transformation                                                         */
/*----------------------------------------------------------------------------*/
void Xgearing::vXform(Stz* szTransformation)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_sXform = szTransformation;
}

/*----------------------------------------------------------------------------*/
/* get transformation phase                                                   */
/*----------------------------------------------------------------------------*/
Stz* Xgearing::szXhand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  return m_sXhand;
}

/*----------------------------------------------------------------------------*/
/* set transformation phase                                                   */
/*----------------------------------------------------------------------------*/
void Xgearing::vXhand(const char* asXhand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_sXhand = asXhand;

  G.pBoard->vXhand(asXhand);
}

/*----------------------------------------------------------------------------*/
/*-- Protected ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is tracing enabled?                                                        */
/*----------------------------------------------------------------------------*/
bool Xgearing::bIsTracing(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  return m_pCardCtl->bTracing(szXhand(),szXform());
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vAppendChild(eCK eParent, eCK eChild)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card* pParent = G_pGetMember(eParent);
  Card* pChild  = G_pGetMember(eChild);

  pParent->vAppend(pChild);

  if (! bIsTracing())
    return;

  vTrace("  %s\n"_Stz,__func__);
  vTrace("    parent %s\n"_Stz,pParent->szBearing());
  vTrace("    child  %s\n"_Stz,pChild->szBearing());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vAppendChild(eCK eParent, Card* pChild)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card* pParent = G_pGetMember(eParent);

  pParent->vAppend(pChild);

  if (! bIsTracing())
    return;

  vTrace("  %s\n"_Stz,__func__);
  vTrace("    parent %s\n"_Stz,pParent->szBearing());
  vTrace("    child  %s\n"_Stz,pChild->szBearing());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vAppendNewChild(Card* pParent, eCK eChild)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card* pChild = new Card(eChild);

  pParent->vAppend(pChild);
  G_vUpdateMember(eChild,pChild,__func__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vAppendNewChild(Card* pParent, eCK eChild, Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card* pChild = new Card(eChild, szValue);

  pParent->vAppend(pChild);
  G_vUpdateMember(eChild,pChild,__func__);
}

/*----------------------------------------------------------------------------*/
/* clear the general M registers                                              */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vClearRegisters(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.p_Located          = nullptr;
  G.e_Located          = CK_NONE;
  G.p_PriorToMain      = nullptr;
  G.e_PriorToMain      = CK_NONE;
  G.p_MainCard         = nullptr;
  G.e_MainCard         = CK_NONE;
  G.i_MainCard         = C_UNKNOWN;
  G.p_SubLocated       = nullptr;
  G.e_SubLocated       = CK_NONE;
  G.p_SubLocatedParent = nullptr;
  G.e_SubLocatedParent = CK_NONE;
  G.p_SubOfMain        = nullptr;
  G.e_SubOfMain        = CK_NONE;
  G.p_Sub2OfMain       = nullptr;
  G.e_Sub2OfMain       = CK_NONE;
  G.p_Sub3OfMain       = nullptr;
  G.e_Sub3OfMain       = CK_NONE;
  G.p_SubsequentToMain = nullptr;
  G.e_SubsequentToMain = CK_NONE;

  G.p_SubMember        = nullptr;
  G.e_SubMember        = CK_NONE;

  m_pTerm_0            = nullptr;
  m_pTerm_1            = nullptr;
  m_pTerm_2            = nullptr;
  m_pTerm_corrPred       = nullptr;

  G.sz_SubQueryNm      = nullptr;

  G.e_Level = AL_NONE;

  G.i_stmColOdn = C_UNKNOWN;
  G.i_FoldCard = C_UNKNOWN;

  G.p_PRED = nullptr;
}

/*----------------------------------------------------------------------------*/
/* clear the general M registers if they match given pointer                  */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vClearRegistersIf(Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (pCard == G.p_PriorToMain) {
    G.p_PriorToMain = nullptr;
    G.e_PriorToMain = CK_NONE;
  }
  if (pCard == G.p_MainCard) {
    G.p_MainCard = nullptr;
    G.e_MainCard = CK_NONE;
  }
  if (pCard == G.p_SubOfMain) {
    G.p_SubOfMain = nullptr;
    G.e_SubOfMain = CK_NONE;
  }
  if (pCard == G.p_Sub2OfMain) {
    G.p_Sub2OfMain = nullptr;
    G.e_Sub2OfMain = CK_NONE;
  }
  if (pCard == G.p_Sub3OfMain) {
    G.p_Sub3OfMain = nullptr;
    G.e_Sub3OfMain = CK_NONE;
  }
  if (pCard == G.p_SubsequentToMain) {
    G.p_SubsequentToMain = nullptr;
    G.e_SubsequentToMain = CK_NONE;
  }
  if (pCard == G.p_SubMember) {
    G.p_SubMember = nullptr;
    G.e_SubMember = CK_NONE;
  }
}

/*----------------------------------------------------------------------------*/
/* clear the general M registers if they match given pointer                  */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vCopyCard(Card* pCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! pCard) {
    m_pCard_copied = nullptr;
    return;
  }

  m_pCard_copied = pCard->pDupCardAndSubCards();
}

/*----------------------------------------------------------------------------*/
/* delete member of given kind; clear-if registers; update M member to null   */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vDeleteMember(eCK eMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card* pMember = G_pGetMember(eMember);

  G_vClearRegistersIf(pMember);
  G_vUpdateMember(eMember, nullptr, __func__);

  DELETE_CARD(pMember);
}

/*----------------------------------------------------------------------------*/
/* delete given member; clear-if registers; update M member to null           */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vDeleteMember(Card* pMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  eCK eMember = pMember->eCard();

  G_vClearRegistersIf(pMember);
  G_vUpdateMember(eMember, nullptr, __func__);

  DELETE_CARD(pMember);
}

/*----------------------------------------------------------------------------*/
/* delete main when given sub is empty                                        */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vDeleteWhenEmptySub(eCK eMain, eCK eSub)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  RASSERT(this->bJustRestarted());

  if (! G_bEachMainWithSub(eMain,eSub))
    return;

  G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
  G.e_SubOfMain = G.p_SubOfMain->eCard();


  if (G.p_SubOfMain->nChildCards())
    return;

  G_vRemoveDeleteMain(eMain);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vDuplicateMember(eCK eMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card* pMember  = G_pGetMember(eMember);

  m_pCard_dup = pMember->pDupCardAndSubCards();

  if (! bIsTracing())
    return;

  vTrace("  %s %s from %s\n"_Stz,
         __func__,m_pCard_dup->szBearing(),pMember->szBearing());
}

/*----------------------------------------------------------------------------*/
/* step main register next main and update M member                           */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bEachMain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((G.p_MainCard = this->pEachChild())) {
    vFocusOnMain();
    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    return true;
  }

  G.e_MainCard = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bEachMain(eCK eOfMainKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((G.p_MainCard = this->pEachChild(eOfMainKind))) {
    vFocusOnMain();
    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    return true;
  }

  G.e_MainCard = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  while ((G.p_MainCard = this->pEachChild(eOfMainKind))) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eAndSubKind)))
      continue;

    vFocusOnMain();

    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vUpdateMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bEachMainWithSub_reverse(eCK eOfMainKind, eCK eAndSubKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  while ((G.p_MainCard = this->pEachChild_reverse(eOfMainKind))) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eAndSubKind)))
      continue;

    vFocusOnMain();

    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vUpdateMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind, nullptr, __func__);
  return false;
}


/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind, Stz* szSubValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  while (G_bEachMainWithSub(eOfMainKind, eAndSubKind)) {
    if (G.p_SubOfMain->szValue()->bSame(szSubValue))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bEachMainWith2Sub(eCK eOfMainKind,
                                   eCK eAndSubKind1,
                                   eCK eAndSubKind2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  while ((G.p_MainCard = this->pEachChild(eOfMainKind))) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eAndSubKind1)))
      continue;
    if (! (G.p_Sub2OfMain = G.p_MainCard->pFindSubCard(eAndSubKind2)))
      continue;

    vFocusOnMain();

    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vUpdateMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    G.e_Sub2OfMain = G.p_Sub2OfMain->eCard();
    G_vUpdateMember(G.e_Sub2OfMain, G.p_Sub2OfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind1, nullptr, __func__);
  G.p_Sub2OfMain = nullptr;
  G.e_Sub2OfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind2, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bEachMainWith3Sub(eCK eOfMainKind,
                                   eCK eAndSubKind1,
                                   eCK eAndSubKind2,
                                   eCK eAndSubKind3)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  while ((G.p_MainCard = this->pEachChild(eOfMainKind))) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eAndSubKind1)))
      continue;
    if (! (G.p_Sub2OfMain = G.p_MainCard->pFindSubCard(eAndSubKind2)))
      continue;
    if (! (G.p_Sub3OfMain = G.p_MainCard->pFindSubCard(eAndSubKind3)))
      continue;

    vFocusOnMain();

    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vUpdateMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    G.e_Sub2OfMain = G.p_Sub2OfMain->eCard();
    G_vUpdateMember(G.e_Sub2OfMain, G.p_Sub2OfMain, __func__);
    G.e_Sub3OfMain = G.p_Sub3OfMain->eCard();
    G_vUpdateMember(G.e_Sub3OfMain, G.p_Sub3OfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind1, nullptr, __func__);
  G.p_Sub2OfMain = nullptr;
  G.e_Sub2OfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind2, nullptr, __func__);
  G.p_Sub3OfMain = nullptr;
  G.e_Sub3OfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind3, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* step main register to next main which has sub and update M member          */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bEachSubOfMain(eCK eSubKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  while ((G.p_MainCard = this->pEachChild())) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eSubKind)))
      continue;

    vFocusOnMain();

    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vUpdateMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eSubKind, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* step main register to next main which has sub and update M member          */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bEach2SubOfMain(eCK eSubKind1, eCK eSubKind2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  while ((G.p_MainCard = this->pEachChild())) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eSubKind1)))
      continue;

    if (! (G.p_Sub2OfMain = G.p_MainCard->pFindSubCard(eSubKind2)))
      continue;

    vFocusOnMain();

    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vUpdateMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    G.e_Sub2OfMain = G.p_Sub2OfMain->eCard();
    G_vUpdateMember(G.e_Sub2OfMain, G.p_Sub2OfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eSubKind1, nullptr, __func__);
  G.p_Sub2OfMain = nullptr;
  G.e_Sub2OfMain = CK_NONE;
  G_vUpdateMember(eSubKind2, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* step main register to next main which has sub and update M member          */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bEach3SubOfMain(eCK eSubKind1, eCK eSubKind2, eCK eSubKind3)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  while ((G.p_MainCard = this->pEachChild())) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eSubKind1)))
      continue;

    if (! (G.p_Sub2OfMain = G.p_MainCard->pFindSubCard(eSubKind2)))
      continue;

    if (! (G.p_Sub3OfMain = G.p_MainCard->pFindSubCard(eSubKind3)))
      continue;

    vFocusOnMain();

    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vUpdateMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    G.e_Sub2OfMain = G.p_Sub2OfMain->eCard();
    G_vUpdateMember(G.e_Sub2OfMain, G.p_Sub2OfMain, __func__);
    G.e_Sub3OfMain = G.p_Sub3OfMain->eCard();
    G_vUpdateMember(G.e_Sub3OfMain, G.p_Sub3OfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eSubKind1, nullptr, __func__);
  G.p_Sub2OfMain = nullptr;
  G.e_Sub2OfMain = CK_NONE;
  G_vUpdateMember(eSubKind2, nullptr, __func__);
  G.p_Sub3OfMain = nullptr;
  G.e_Sub3OfMain = CK_NONE;
  G_vUpdateMember(eSubKind3, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* find sub of member with given property                                     */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bFindPpy(eCK eMember, Stz* szPpy, eCK eOfCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! (m_pCard_member = G_pGetMember(eMember))) {
    m_pCard_found = nullptr;
    return false;
  }

  if (! (m_pCard_found = m_pCard_member->pFindPpyRecursive(szPpy)))
    return false;

  G_vUpdateMember(m_pCard_found->eCard(), m_pCard_found, __func__);
  return true;
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately before current main card?                      */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bHasPriorToMain(bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((G.p_PriorToMain = this->pPrevChild())) {
    G.e_PriorToMain = G.p_PriorToMain->eCard();
    if (bUpdateMember)
      G_vUpdateMember(G.e_PriorToMain, G.p_PriorToMain, __func__);
    return true;
  }

  G.e_PriorToMain = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately before current main which is of given kind?    */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bHasPriorToMainIf(eCK eOfPriorKind, bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((G.p_PriorToMain = this->pPrevChildIf(eOfPriorKind))) {
    G.e_PriorToMain = G.p_PriorToMain->eCard();
    if (bUpdateMember)
      G_vUpdateMember(G.e_PriorToMain, G.p_PriorToMain, __func__);
    return true;
  }

  G.e_PriorToMain = CK_NONE;
  if (bUpdateMember)
    G_vUpdateMember(eOfPriorKind, nullptr, __func__);

  return false;
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately after current main card?                       */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bHasSubsequentToMain(bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((G.p_SubsequentToMain = this->pNextChild())) {
    G.e_SubsequentToMain = G.p_SubsequentToMain->eCard();
    if (bUpdateMember)
      G_vUpdateMember(G.e_SubsequentToMain, G.p_SubsequentToMain, __func__);
    return true;
  }

  G.e_SubsequentToMain = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately after current main which is of given kind?     */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bHasSubsequentToMainIf(eCK eOfSubsequentKind, bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((G.p_SubsequentToMain = this->pNextChildIf(eOfSubsequentKind))) {
    G.e_SubsequentToMain = G.p_SubsequentToMain->eCard();
    if (bUpdateMember)
      G_vUpdateMember(G.e_SubsequentToMain, G.p_SubsequentToMain, __func__);
    return true;
  }

  G.e_SubsequentToMain = CK_NONE;
  if (bUpdateMember)
    G_vUpdateMember(eOfSubsequentKind, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vInsertAfter(Card* pCard, Card* pAfter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G.pDeck->vInsertAfter(pCard,pAfter);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vInsertBefore(Card* pCard, Card* pBefore)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G.pDeck->vInsertBefore(pCard,pBefore);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bIsBoard(Stz* szBoardNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (m_pBench->pGetBoard(szBoardNm))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bIsNextMain(eCK eMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (this->pNextChildIf(eMember))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* locate card (left-right) after given i_After                               */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bLocateCard(eCK eLocate, Stz* szWithValue, i8 i_After)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nChildren = G.pDeck->nChildCards();
  for (i8 i_Main = i_After + 1; i_Main < nChildren; i_Main++) {
    Card *pMain = G.pDeck->pGetChild(i_Main);

    G.p_Located = pMain->pFindValueRecursive(eLocate,szWithValue,G.p_Located);
    if (!G.p_Located)
      continue;

    G.e_Located = eLocate;
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* locate card (left-right) after given i_After                               */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bLocateCard(eCK eLocate, eCK eSubLocate, Stz* szWithValue, i8 i_After)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nChildren = G.pDeck->nChildCards();
  for (i8 i_Main = i_After + 1; i_Main < nChildren; i_Main++) {
    Card *pMain = G.pDeck->pGetChild(i_Main);

    G.p_Located = pMain->pFindCardRecursive(eLocate);
    if (! G.p_Located)
      continue;

    G.e_Located = eLocate;

    G.p_SubLocated = G.p_Located->pFindValueRecursive(eSubLocate,szWithValue,G.p_SubLocatedParent);
    if (G.p_SubLocated) {
      G.e_SubLocated = eSubLocate;
      G.e_SubLocatedParent = G.p_SubLocatedParent ? G.p_SubLocatedParent->eCard() : CK_NONE;
      return true;
    }

  }

  G.p_Located = nullptr;
  G.e_Located = CK_NONE;
  G.p_SubLocated = nullptr;
  G.e_SubLocated = CK_NONE;
  G.p_SubLocatedParent = nullptr;
  G.e_SubLocatedParent = CK_NONE;

  return false;
}

/*----------------------------------------------------------------------------*/
/* locate card (left-right) after given card                                  */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bLocateCard(eCK eCard, Stz* szWithValue, Card* pAfter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* are there more main cards?                                                 */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bMoreMain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (this->bMore())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* morph card                                                                 */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vMorphCard(Card* pCard, eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  pCard->vCard(eCard);
}

/*----------------------------------------------------------------------------*/
/* create a new card and optionally set member                                */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vNewMember(eCK eMember, bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pCard_new = new Card(eMember);

  if (bUpdateMember)
    G_vUpdateMember(eMember, m_pCard_new, __func__);
}

/*----------------------------------------------------------------------------*/
/* step to next main and set member                                           */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vNewMember(eCK eMember, Stz* szValue, bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G_vNewMember(eMember, bUpdateMember);

  m_pCard_new->vValue(szValue);
}

/*----------------------------------------------------------------------------*/
/* step to next main and set member                                           */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vNewMemberBefore(eCK eMember, Stz* szValue,
                                  eCK eBefore, bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G_vNewMember(eMember, bUpdateMember);

  m_pCard_new->vValue(szValue);

  Card* p_before = G_pGetMember(eBefore);

  G.pDeck->vInsertBefore(m_pCard_new,p_before);
}

/*----------------------------------------------------------------------------*/
/* step to next main and set member                                           */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vNewMemberOf(eCK eMember, Stz* szValue,
                              eCK eOf, bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G_vNewMember(eMember, bUpdateMember);

  m_pCard_new->vValue(szValue);

  G_vAppendChild(eOf,eMember);
}

/*----------------------------------------------------------------------------*/
/* search for sub of member                                                   */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bPlumbForSubMember(eCK eMember, eCK eSubCard, bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card* pMember = G_pGetMember(eMember);

  bool b = G_bPlumbForSubMember(pMember, eSubCard, bUpdateMember);

  return b;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bPlumbForSubMember(Card* pMember, eCK eSubCard, bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((G.p_SubMember = pMember->pFindSubCard(eSubCard))) {
    G.e_SubMember = G.p_SubMember->eCard();
    if (bUpdateMember)
      G_vUpdateMember(G.e_SubMember, G.p_SubMember, __func__);
    return true;
  }

  if (bUpdateMember)
    G_vUpdateMember(eSubCard, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bPlumbForSubValue(eCK eMember, eCK eSub, Stz* szValue, bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card* pMember = G_pGetMember(eMember);

  bool b = G_bPlumbForSubValue(pMember, eSub, szValue, bUpdateMember);

  return b;

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bPlumbForSubValue(Card* pMember, eCK eSub, Stz* szValue, bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((G.p_SubMember = pMember->pFindSubCard(eSub))) {
    if (G.p_SubMember->bSame(szValue)) {
      G.e_SubMember = G.p_SubMember->eCard();
      if (bUpdateMember)
        G_vUpdateMember(G.e_SubMember, G.p_SubMember, __func__);
      return true;
    }
  }

  if (bUpdateMember)
    G_vUpdateMember(eSub, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* push current M iterator onto iter stack                                    */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vPushBoard(Board* pBoard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G_vPushIter();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  G.pBoard = pBoard;
//  G.pDeck  = G.pBoard->pDeck();
//
//  this->vChangeBoard(G.pBoard);
  G_vSetBoard(pBoard);
}

/*----------------------------------------------------------------------------*/
/* push board                                                                 */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bPushBoard(i8 i_Board)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Board* pBoard = m_pBench->pGetBoard(i_Board);
  RASSERT(pBoard);

  if (pBoard) {
    G_vPushBoard(pBoard);
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* push current M iterator onto iter stack                                    */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bPushBoard(Stz* szBoardNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Board* pSubQueryBoard = m_pBench->pGetBoard(szBoardNm);
  if (! pSubQueryBoard) {
    vTracer("DID NOT FIND Board: %s\n"_Stz,szBoardNm);
    return false;
  }

  vTracer("Found SubQuery Board: %s\n"_Stz,pSubQueryBoard->szBoardNm());

  G_vPushBoard(pSubQueryBoard);

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vPushNewChild(Card* pParent, eCK eChild)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card* pChild = new Card(eChild);

  pParent->vPush(pChild);
  G_vUpdateMember(eChild,pChild,__func__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vPushNewChild(Card* pParent, eCK eChild, Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card* pChild = new Card(eChild, szValue);

  pParent->vPush(pChild);
  G_vUpdateMember(eChild,pChild,__func__);
}

/*----------------------------------------------------------------------------*/
/* push current M iterator onto iter stack                                    */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vPushIter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

//  Xsentinals* pTransformIterSave = new Xsentinals;
//
//  pTransformIterSave->vCopyFrom(this->aXsentinels());
//
//  m_pTransformIterList->vPush(pTransformIterSave);

  this->vSave();
}

/*----------------------------------------------------------------------------*/
/* pop/restore M iterator from iter stack                                     */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vPopIter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

//  Xsentinals* pTransformIterRestore = m_pTransformIterList->pPop();
//
//  this->vCopyFrom(pTransformIterRestore->aXsentinels());
//
//  delete pTransformIterRestore;

  this->vRestore();
}

/*----------------------------------------------------------------------------*/
/* remove main card from deck (do not clear M members or registers)           */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vRemoveMain(eCK eCrossCheck)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  this->vRemoveCurr(eCrossCheck);
}

/*----------------------------------------------------------------------------*/
/* remove ain card from deck and delete it; clear M member to null            */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vRemoveDeleteMain(eCK eCrossCheck)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  this->vRemoveDeleteCurr(eCrossCheck);

  G.p_MainCard = nullptr;
  G.e_MainCard = CK_NONE;

  G_vUpdateMember(eCrossCheck, nullptr, __func__);
}

/*----------------------------------------------------------------------------*/
/* resolve card (right-left before given i_Before                             */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bResolveCard(eCK eCard, i8 i_Before)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (C_UNKNOWN == i_Before)
    i_Before = G.pDeck->nChildCards();

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card* pMain = G.pDeck->pGetChild(i_Main);

    if (pMain->bIsCard(eCard)) {
      G_vUpdateMember(eCard, pMain, __func__);
      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* resolve card (right-left before given i_Before                             */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bResolveCard(eCK eCard, char cWithValue, i8 i_Before)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card* pMain = G.pDeck->pGetChild(i_Main);

    if ((m_pCard_resolved = pMain->pFindValueRecursive(eCard,cWithValue,m_pCard_parent))) {
      G_vUpdateMember(eCard, m_pCard_resolved, __func__);
      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* resolve card (right-left before given i_Before                             */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bResolveCard(eCK eCard, Stz* szWithValue, i8 i_Before)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card* pMain = G.pDeck->pGetChild(i_Main);

    if ((m_pCard_resolved = pMain->pFindValueRecursive(eCard,szWithValue,m_pCard_parent))) {
      G_vUpdateMember(eCard, m_pCard_resolved, __func__);
      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* resolve card (right-left) before given card                                */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bResolveCard(eCK eCard, Stz* szWithValue, Card* pBefore)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 i_Before = G.pDeck->iFindChild(pBefore);
  if (C_UNKNOWN == i_Before)
    return false;

  bool b = G_bResolveCard(eCard, szWithValue, i_Before);

  return b;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bResolveCardWithSub(eCK eOfMainKind, eCK eAndSubKind, i8 i_Before)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    G.p_MainCard = G.pDeck->pGetChild(i_Main);

    if (! G.p_MainCard->bIsCard(eOfMainKind))
      continue;

    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eAndSubKind)))
      continue;

    G.e_MainCard = G.p_MainCard->eCard();
    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    G.e_SubOfMain = G.p_SubOfMain->eCard();
    G_vUpdateMember(G.e_SubOfMain, G.p_SubOfMain, __func__);
    return true;
  }

  G.p_MainCard  = nullptr;
  G.e_MainCard  = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  G.p_SubOfMain = nullptr;
  G.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind, nullptr, __func__);
  return false;
}


/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bResolveCardWithSub(eCK eOfMainKind, eCK eAndSubKind, Card* pBefore)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 i_Before = G.pDeck->iFindChild(pBefore);
  if (C_UNKNOWN == i_Before)
    return false;

  bool b = G_bResolveCardWithSub(eOfMainKind, eAndSubKind, i_Before);

  return b;
}

/*----------------------------------------------------------------------------*/
/* resolve card (r-l) with grand-sub with given value before given i_Before   */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bResolveSubCard(eCK eCard, eCK eGrandSub, Stz* szWithValue, i8 i_Before)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card* pMain = G.pDeck->pGetChild(i_Main);

    if ((m_pCard_resolved = pMain->pFindValueRecursive(eGrandSub,szWithValue,m_pCard_parent))) {

      if ((m_pCard_resolved = pMain->pFindParent(m_pCard_resolved))) {
        G_vUpdateMember(m_pCard_resolved->eCard(), m_pCard_resolved, __func__);
        return true;
      }
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* resolve card (r-l) with grand-sub with given value before given i_Before   */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bResolveSubCard(eCK eCard, i8 i_Before)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card* pMain = G.pDeck->pGetChild(i_Main);

    if ((m_pCard_resolved = pMain->pFindSubCard(eCard))) {
      G_vUpdateMember(eCard, m_pCard_resolved, __func__);
      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* restart the M iterator                                                     */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vRestart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  this->vRestart();

  G_vClearMembers();
  G_vClearRegisters();
}

/*----------------------------------------------------------------------------*/
/* step main register next main and update M member                           */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bRewindMain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((G.p_MainCard = this->pRewindChild())) {
    vFocusOnMain();
    G_vUpdateMember(G.e_MainCard, G.p_MainCard, __func__);
    return true;
  }

  G.e_MainCard = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vSetBoard(Board* pBoard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.pBoard = pBoard;
  G.pDeck  = G.pBoard->pDeck();

  this->vChangeCard(G.pBoard->pDeck());
}

/*----------------------------------------------------------------------------*/
/* skip over next main ; optionally clear M members and registers             */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vSkipNextMain(bool bClearMembers, bool bClearRegisters)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  this->vForward();

  if (bClearMembers)
    G_vClearMembers();

  if (bClearRegisters)
    G_vClearRegisters();
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately after current main card?                       */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bSubsequentToMain(bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((G.p_SubsequentToMain = this->pNextChild())) {
    G.e_SubsequentToMain = G.p_SubsequentToMain->eCard();
    if (bUpdateMember)
      G_vUpdateMember(G.e_SubsequentToMain, G.p_SubsequentToMain, __func__);
    return true;
  }

  G.e_SubsequentToMain = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately after current main card?                       */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vSubsequentToMain(bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  bool b = G_bSubsequentToMain(bUpdateMember);
  UNUSED(b);
}

/*----------------------------------------------------------------------------*/
/* update member card                                                         */
/*----------------------------------------------------------------------------*/
Stz* Xgearing::szT(eCK eMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card* pMember = G_pGetMember(eMember);
  Stz* szMemberNm = this->szMemberNm(eMember);
  if (pMember)
    m_sT.vSprint("%s=%s %s"_Stz,
                    szMemberNm,pMember->szValue(), pMember->szBearing());
  else
    m_sT.vSprint("%s=null"_Stz,szMemberNm);

  return m_sT;
}

/*----------------------------------------------------------------------------*/
/* conditionally trace                                                        */
/*----------------------------------------------------------------------------*/
void Xgearing::vTracer(Stz* szFormat, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! bIsTracing())
    return;

  s256 sValue;

  va_list xArgs;
  va_start(xArgs,szFormat);
  StrLib::vSprint(sValue.azChar(),xArgs,(char*)szFormat);
  va_end(xArgs);

  vTrace("  [%02lld] %s"_Stz,m_iTrace++,sValue.szStz());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
