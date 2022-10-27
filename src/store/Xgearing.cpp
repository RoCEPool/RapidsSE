/*============================================================================*/
/* Xgearing.cpp --                                                     */
/*============================================================================*/

#include "Xgearing.hpp"

#include "Job.hpp"
#include "Cue.hpp"

#include "Optimiser.hpp"
#include "OptimiserFun.hpp"
#include "OptimiserCatalog.hpp"

extern OptimiserCatalog gxOptimiserCatalog;


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xgearing::Xgearing(eCN eClass, Bench* pBench)
: Xsentinals(eClass)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pBench   = pBench;
  m_pCardCtl = m_pBench->pCardCtl();
  m_bTrace = false;
  m_iTrace   = 0;

  m_bPrintHand = false;
  m_bPrintXform = false;
  Job* pJob = MY::pJob();
  TagSet* pJobTags = pJob->pJobTags();
  m_bInteractive = pJobTags->bHasName("<interactive>"_Stz);
  m_bBeenCued = false;
  m_bCueingDisabled = false;

  m_pPermGen   = nullptr;
  m_cTableSyn  = 'a';

  m_pColumn    = nullptr;
  m_pIdxVec    = nullptr;
  m_pIndex     = nullptr;
  m_pIndexDef  = nullptr;
  m_pStreamDef = nullptr;
  m_pTable     = nullptr;

  m_pCard_child    = nullptr;
  m_pCard_dup      = nullptr;
  m_pCard_found    = nullptr;
  m_pCard_located  = nullptr;
  m_pCard_member   = nullptr;
  m_pCard_new      = nullptr;
  m_pCard_parent   = nullptr;
  m_pCard_popped   = nullptr;
  m_pCard_resolved = nullptr;
  m_pCard_temp     = nullptr;

  m_pTransformIterList = new TransformIterList;

  m_iOrdLo = 1;
  m_iOrdHi = C_UNKNOWN;
  m_nStreamCols = C_UNKNOWN;

  // the M iterator - - - - - - - - - - - - - - - - - - - - - -

  M.pBoard = nullptr;
  M.pDeck = nullptr;

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

  delete m_pTransformIterList;
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

  return M.pBoard;
}

/*----------------------------------------------------------------------------*/
/* cue user                                                                  */
/*----------------------------------------------------------------------------*/
eOA Xgearing::eCueUser(Stz* szHand, Stz* szTransformation)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  TokenVec* pTknVec = nullptr;

  if (m_bCueingDisabled)
    return OA_CONTINUE_TRANSFORMING;

  s256 sPrompt;
  if (szHand && szTransformation)
    sPrompt.vSprint("%s :: %s"_Stz, szHand, szTransformation);
  else if (szHand)
    sPrompt.vSprint("%s"_Stz, szHand, szTransformation);
  else
    sPrompt.vSprint(""_Stz, szHand);

  if (! m_bBeenCued && AM::JOB::bHasTag("<cue>"_Stz)) {
    Stz* szTagCue = AM::JOB::szTagValue("<cue>"_Stz);
    if (szTagCue->bSame(szHand) && szTransformation) {
      printf("%s\n", sPrompt.azChar());
      return OA_CONTINUE_TRANSFORMING;
    }
    if ( szTransformation && (! szTagCue->bSame(szTransformation))) {
      printf("%s\n", sPrompt.azChar());
      return OA_CONTINUE_TRANSFORMING;
    }
  }

  sPrompt.vConcat(" >> "_Stz);

  m_bBeenCued = true;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_Input:

  s4K sInput;
  Cue::vGetInput(sPrompt,sInput);

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
    goto L_Input;
  }

  eOA eOptAct = OA_CONTINUE_TRANSFORMING;
  bool bExecuted = pOptFun->m_fpOptFun(m_pBench->pOptimiser(),pTknVec,eOptAct);

  delete pTknVec;
  pTknVec = nullptr;

  if (OA_INPUT_AGAIN == eOptAct)
    goto L_Input;

  if (OA_DISABLE_CUEING == eOptAct)
    m_bCueingDisabled = true;

  return eOptAct;
}

/*----------------------------------------------------------------------------*/
/* get description of transformation                                          */
/*----------------------------------------------------------------------------*/
Stz* Xgearing::szDescription(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  return m_sDescription;
}

/*----------------------------------------------------------------------------*/
/* set description of transformation                                          */
/*----------------------------------------------------------------------------*/
void Xgearing::vDescription(Stz* szDescription)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_sDescription = szDescription;
}

/*----------------------------------------------------------------------------*/
/* end of transformation                                                      */
/*----------------------------------------------------------------------------*/
void Xgearing::vEndTransform(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_bTrace = false;

  M.pDeck->vRenumberRecursive();

  // UNDO: Card::vConsistencyCheck(m_pDeck, m_iTransformNo);
}

/*----------------------------------------------------------------------------*/
/* get transformation phase                                                   */
/*----------------------------------------------------------------------------*/
Stz* Xgearing::szHand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  return m_sHand;
}

/*----------------------------------------------------------------------------*/
/* set transformation phase                                                   */
/*----------------------------------------------------------------------------*/
void Xgearing::vHand(const char* asHand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_sHand = asHand;
}

/*----------------------------------------------------------------------------*/
/* print all bards of bench                                                   */
/*----------------------------------------------------------------------------*/
void Xgearing::vPrintBench(eBK eBoard, i8 i_SubBoard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  for (;;) {
    if (C_UNKNOWN != i_SubBoard) {
      Board *pBoard = m_pBench->pGetBoard(eBoard, i_SubBoard);
      pBoard->vPrintBoard(szHand());
    }
    else {
      for (i8 i_Sub = 0; m_pBench->bBoardExists(eBoard, i_Sub); i_Sub++) {
        Board *pBoard = m_pBench->pGetBoard(eBoard, i_Sub);
        pBoard->vPrintBoard(szHand());
      }
    }

    if (OA_REPRINT_BOARD != eCueUser(szHand()))
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* print the board                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::vPrintBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! m_bPrintXform)
    return;

  for (;;) {
    M.pBoard->vPrintBoard(szHand(),
                          szTransformation(),
                          szDescription());

    if (OA_REPRINT_BOARD != eCueUser(szHand(), szTransformation()))
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* set control of card printing by kind                                       */
/*----------------------------------------------------------------------------*/
void Xgearing::vSetPrintByKind(eCK eCard, bool bPrintByKind)
{
  m_pCardCtl->vPrintByKind(eCard,bPrintByKind);
}

/*----------------------------------------------------------------------------*/
/* get print phase                                                            */
/*----------------------------------------------------------------------------*/
bool Xgearing::bPrintHand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  return m_bPrintHand;
}

/*----------------------------------------------------------------------------*/
/* set print phase                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::vPrintHand(bool bPrintHand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_bPrintHand = bPrintHand;
}

/*----------------------------------------------------------------------------*/
/* get print phase                                                            */
/*----------------------------------------------------------------------------*/
bool Xgearing::bPrintXform(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  return m_bPrintHand;
}

/*----------------------------------------------------------------------------*/
/* set print phase                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::vPrintXform(bool bPrintXform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_bPrintXform = bPrintXform;
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
void Xgearing::vTransform(const char* azTransformation,
                          bool bPrintTransformation)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_sTransformation = azTransformation;
  m_bPrintXform = bPrintTransformation;

  M.pBoard->vTransformNoInc();

  M.xBI.vRestart();
}

/*----------------------------------------------------------------------------*/
/* get transformation                                                         */
/*----------------------------------------------------------------------------*/
Stz* Xgearing::szTransformation(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  return m_sTransformation;
}

/*----------------------------------------------------------------------------*/
/* get transformation                                                         */
/*----------------------------------------------------------------------------*/
void Xgearing::vTransformation(Stz* szTransformation)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_sTransformation = szTransformation;
}

/*----------------------------------------------------------------------------*/
/* set up which board to use                                                  */
/*----------------------------------------------------------------------------*/
void Xgearing::vUsingBoard(eBK eBoard, i8 i_SubBoard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.pBoard = m_pBench->pGetBoard(eBoard,i_SubBoard);
  M.pDeck  = M.pBoard->pDeck();

  M.xBI.vChangeBoard(M.pBoard);
}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is tracing enabled?                                                        */
/*----------------------------------------------------------------------------*/
bool Xgearing::bIsTracing(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  return m_bTrace;
}

/*----------------------------------------------------------------------------*/
/* is tracing enabled?                                                        */
/*----------------------------------------------------------------------------*/
void Xgearing::vTracing(bool bPrintXform, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_bPrintXform = bPrintXform;

  m_bTrace = bTrace;

  if (m_bTrace) {
    vTrace("\n\nTracing %s\n"_Stz, m_sTransformation.szStz());
    m_iTrace = 0;
  }
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

  if (! m_bTrace)
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

  if (! m_bTrace)
    return;

  vTrace("  %s\n"_Stz,__func__);
  vTrace("    parent %s\n"_Stz,pParent->szBearing());
  vTrace("    child  %s\n"_Stz,pChild->szBearing());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vDuplicateMember(eCK eMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card* pMember  = G_pGetMember(eMember);

  m_pCard_dup = pMember->pDupCardAndSubCards();

  if (! m_bTrace)
    return;

  vTrace("  %s %s from %s\n"_Stz,
         __func__,m_pCard_dup->szBearing(),pMember->szBearing());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vRemoveChild(eCK eParent, eCK eChild)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vRemoveDeleteChild(eCK eParent, eCK eChild)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vRemoveDeleteMain(eCK eMain)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vPromoteChildren(eCK eParent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

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
/* step main register next main and update M member                           */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bEachMain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((M.p_MainCard = M.xBI.pEachMain())) {
    M.e_MainCard = M.p_MainCard->eCard();
    G_vUpdateMember(M.e_MainCard, M.p_MainCard, __func__);
    return true;
  }

  M.e_MainCard = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main and update M member          */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bEachMain(eCK eOfMainKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((M.p_MainCard = M.xBI.pEachMain(eOfMainKind))) {
    M.e_MainCard = M.p_MainCard->eCard();
    G_vUpdateMember(M.e_MainCard, M.p_MainCard, __func__);
    return true;
  }

  M.e_MainCard = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* conditionally step main register to next main which has sub and update M   */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  while ((M.p_MainCard = M.xBI.pEachMain(eOfMainKind))) {
    if (! (M.p_SubOfMain = M.p_MainCard->pFindSubCard(eAndSubKind)))
      continue;

    M.e_MainCard = M.p_MainCard->eCard();
    G_vUpdateMember(M.e_MainCard, M.p_MainCard, __func__);
    M.e_SubOfMain = M.p_SubOfMain->eCard();
    G_vUpdateMember(M.e_SubOfMain, M.p_SubOfMain, __func__);
    return true;
  }

  M.p_MainCard  = nullptr;
  M.e_MainCard  = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  M.p_SubOfMain = nullptr;
  M.e_SubOfMain = CK_NONE;
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
    if (M.p_SubOfMain->szValue()->bSame(szSubValue))
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

  while ((M.p_MainCard = M.xBI.pEachMain(eOfMainKind))) {
    if (! (M.p_SubOfMain = M.p_MainCard->pFindSubCard(eAndSubKind1)))
      continue;
    if (! (M.p_Sub2OfMain = M.p_MainCard->pFindSubCard(eAndSubKind2)))
      continue;

    M.e_MainCard = M.p_MainCard->eCard();
    G_vUpdateMember(M.e_MainCard, M.p_MainCard, __func__);
    M.e_SubOfMain = M.p_SubOfMain->eCard();
    G_vUpdateMember(M.e_SubOfMain, M.p_SubOfMain, __func__);
    M.e_Sub2OfMain = M.p_Sub2OfMain->eCard();
    G_vUpdateMember(M.e_Sub2OfMain, M.p_Sub2OfMain, __func__);
    return true;
  }

  M.p_MainCard  = nullptr;
  M.e_MainCard  = CK_NONE;
  G_vUpdateMember(eOfMainKind, nullptr, __func__);
  M.p_SubOfMain = nullptr;
  M.e_SubOfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind1, nullptr, __func__);
  M.p_Sub2OfMain = nullptr;
  M.e_Sub2OfMain = CK_NONE;
  G_vUpdateMember(eAndSubKind2, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* skip over next main ; optionally clear M members and registers             */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vSkipNextMain(bool bClearMembers, bool bClearRegisters)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.xBI.vForward();

  if (bClearMembers)
    G_vClearMembers();

  if (bClearRegisters)
    G_vClearRegisters();
}

/*----------------------------------------------------------------------------*/
/* are there more main cards?                                                 */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bMoreMain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (M.xBI.bMore())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* remove main card from deck (do not clear M members or registers)           */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vRemoveMain(eCK eCrossCheck)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.xBI.vRemoveCurr(eCrossCheck);
}

/*----------------------------------------------------------------------------*/
/* remove ain card from deck and delete it; clear M member to null            */
/*----------------------------------------------------------------------------*/
void Xgearing::E_vRemoveDeleteMain(eCK eCrossCheck)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.xBI.vRemoveDeleteCurr(eCrossCheck);

  M.p_MainCard = nullptr;
  M.e_MainCard = CK_NONE;

  G_vUpdateMember(eCrossCheck, nullptr, __func__);
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately after current main card?                       */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bSubsequentToMain(bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((M.p_SubsequentToMain = M.xBI.pNextMain())) {
    M.e_SubsequentToMain = M.p_SubsequentToMain->eCard();
    if (bUpdateMember)
      G_vUpdateMember(M.e_SubsequentToMain, M.p_SubsequentToMain, __func__);
    return true;
  }

  M.e_SubsequentToMain = CK_NONE;
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
/* fetch M member by decoding given eMember                                   */
/*----------------------------------------------------------------------------*/
Card* Xgearing::G_pGetMember(eCK eMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  switch (eMember) {

    case CK_ABSJOIN:          return M.p_absJoin;
    case CK_AGGREG:           return M.p_aggreg;
    case CK_BOOLCALC:         return M.p_boolCalc;
    case CK_BOUNDS:           return M.p_bounds;
    case CK_CMPPRED:          return M.p_cmpPred;
    case CK_COMBINED:         return M.p_combined;
    case CK_COMMENT:          return M.p_comment;
    case CK_COPTNJOIN:        return M.p_coPtnJoin;
    case CK_COPTNJOINLIST:    return M.p_coPtnJoinList;
    case CK_COPTNPRED:        return M.p_coPtnPred;
    case CK_COPTNPREDLIST:    return M.p_coPtnPredList;
    case CK_CROSSPTNJOIN:     return M.p_crossPtnJoin;
    case CK_CROSSPTNJOINLIST: return M.p_crossPtnJoinList;
    case CK_EXPR:             return M.p_expr;
    case CK_FILTER:           return M.p_filter;
    case CK_GBYELIST:         return M.p_gbyElist;
    case CK_IDXCOL:           return M.p_idxCol;
    case CK_IDXCOLLIST:       return M.p_idxColList;
    case CK_IDXCOLNM:         return M.p_idxColNm;
    case CK_INDEX:            return M.p_index;
    case CK_INDEXJOIN:        return M.p_indexJoin;
    case CK_INDEXLIST:        return M.p_indexList;
    case CK_INDEXNM:          return M.p_indexNm;
    case CK_INDEXSCAN:        return M.p_indexScan;
    case CK_JOINORDER:        return M.p_joinOrder;
    case CK_JOINORDERSEQ:     return M.p_joinOrderSeq;
    case CK_JOINPREDLIST:     return M.p_joinPredList;
    case CK_KEYEXISTS:        return M.p_keyExists;
    case CK_LBOUND:           return M.p_lBound;
    case CK_PARAM:            return M.p_param;
    case CK_PARAMLIST:        return M.p_paramList;
    case CK_PARTITION:        return M.p_partition;
    case CK_PERMUTATION:      return M.p_permutation;
    case CK_PROJECT:          return M.p_project;
    case CK_PTNCOL:           return M.p_ptnCol;
    case CK_PTNCOLLIST:       return M.p_ptnColList;
    case CK_PTNCOLNM:         return M.p_ptnColNm;
    case CK_PTNDJOIN:         return M.p_ptndJoin;
    case CK_PTNDJOINSET:      return M.p_ptndJoinSet;
    case CK_RPNCALC:          return M.p_rpnCalc;
    case CK_SBYELIST:         return M.p_sbyElist;
    case CK_SCAN:             return M.p_scan;
    case CK_SEND:             return M.p_send;
    case CK_SIZE:             return M.p_size;
    case CK_SORT:             return M.p_sort;
    case CK_STAGE:            return M.p_stage;
    case CK_STMCOL:           return M.p_stmCol;
    case CK_STMCOLLIST:       return M.p_stmColList;
    case CK_STMCOLNM:         return M.p_stmColNm;
    case CK_STMCOLORD:        return M.p_stmColOrd;
    case CK_STREAM:           return M.p_stream;
    case CK_SUBSET:           return M.p_subSet;
    case CK_TABLE:            return M.p_table;
    case CK_TABLELIST:        return M.p_tableList;
    case CK_TABLENM:          return M.p_tableNm;
    case CK_TABLEPTN:         return M.p_tablePtn;
    case CK_TERM:             return M.p_term;
    case CK_TOPOLOGY:         return M.p_topology;
    case CK_TSTPRED:          return M.p_tstPred;
    case CK_UBOUND:           return M.p_uBound;
    case CK_WHERE:            return M.p_where;

   default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately before current main card?                      */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bHasPriorToMain(bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((M.p_PriorToMain = M.xBI.pPrevMain())) {
    M.e_PriorToMain = M.p_PriorToMain->eCard();
    if (bUpdateMember)
      G_vUpdateMember(M.e_PriorToMain, M.p_PriorToMain, __func__);
    return true;
  }

  M.e_PriorToMain = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately before current main which is of given kind?    */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bHasPriorToMainIf(eCK eOfPriorKind, bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((M.p_PriorToMain = M.xBI.pPrevMainIf(eOfPriorKind))) {
    M.e_PriorToMain = M.p_PriorToMain->eCard();
    if (bUpdateMember)
      G_vUpdateMember(M.e_PriorToMain, M.p_PriorToMain, __func__);
    return true;
  }

  M.e_PriorToMain = CK_NONE;
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

  if ((M.p_SubsequentToMain = M.xBI.pNextMain())) {
    M.e_SubsequentToMain = M.p_SubsequentToMain->eCard();
    if (bUpdateMember)
      G_vUpdateMember(M.e_SubsequentToMain, M.p_SubsequentToMain, __func__);
    return true;
  }

  M.e_SubsequentToMain = CK_NONE;
  return false;
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately after current main which is of given kind?     */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bHasSubsequentToMainIf(eCK eOfSubsequentKind, bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((M.p_SubsequentToMain = M.xBI.pNextMainIf(eOfSubsequentKind))) {
    M.e_SubsequentToMain = M.p_SubsequentToMain->eCard();
    if (bUpdateMember)
      G_vUpdateMember(M.e_SubsequentToMain, M.p_SubsequentToMain, __func__);
    return true;
  }

  M.e_SubsequentToMain = CK_NONE;
  if (bUpdateMember)
    G_vUpdateMember(eOfSubsequentKind, nullptr, __func__);
  return false;
}

/*----------------------------------------------------------------------------*/
/* locate card (left-right) after given i_After                               */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bLocateCard(eCK eCard, Stz* szWithValue, i8 i_After)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

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
/* clear the M members                                                        */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vClearMembers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_absJoin          = nullptr;
  M.p_aggreg           = nullptr;
  M.p_boolCalc         = nullptr;
  M.p_bounds           = nullptr;
  M.p_cmpPred          = nullptr;
  M.p_combined         = nullptr;
  M.p_comment          = nullptr;
  M.p_coPtnJoin        = nullptr;
  M.p_coPtnJoinList    = nullptr;
  M.p_coPtnPred        = nullptr;
  M.p_coPtnPredList    = nullptr;
  M.p_crossPtnJoin     = nullptr;
  M.p_crossPtnJoinList = nullptr;
  M.p_expr             = nullptr;
  M.p_filter           = nullptr;
  M.p_gbyElist         = nullptr;
  M.p_idxCol           = nullptr;
  M.p_idxColList       = nullptr;
  M.p_idxColNm         = nullptr;
  M.p_index            = nullptr;
  M.p_indexJoin        = nullptr;
  M.p_indexList        = nullptr;
  M.p_indexNm          = nullptr;
  M.p_indexScan        = nullptr;
  M.p_joinOrder        = nullptr;
  M.p_joinOrderSeq     = nullptr;
  M.p_joinPredList     = nullptr;
  M.p_keyExists        = nullptr;
  M.p_lBound           = nullptr;
  M.p_param            = nullptr;
  M.p_paramList        = nullptr;
  M.p_partition        = nullptr;
  M.p_permutation      = nullptr;
  M.p_permutation_low  = nullptr;
  M.p_project          = nullptr;
  M.p_ptnCol           = nullptr;
  M.p_ptnColList       = nullptr;
  M.p_ptnColNm         = nullptr;
  M.p_ptndJoin         = nullptr;
  M.p_ptndJoinSet      = nullptr;
  M.p_rpnCalc          = nullptr;
  M.p_sbyElist         = nullptr;
  M.p_scan             = nullptr;
  M.p_send             = nullptr;
  M.p_size             = nullptr;
  M.p_sort             = nullptr;
  M.p_stage            = nullptr;
  M.p_stmCol           = nullptr;
  M.p_stmColList       = nullptr;
  M.p_stmColNm         = nullptr;
  M.p_stmColOrd        = nullptr;
  M.p_stream           = nullptr;
  M.p_subSet           = nullptr;
  M.p_table            = nullptr;
  M.p_tableList        = nullptr;
  M.p_tableNm          = nullptr;
  M.p_tablePtn         = nullptr;
  M.p_term             = nullptr;
  M.p_topology         = nullptr;
  M.p_tstPred          = nullptr;
  M.p_uBound           = nullptr;
  M.p_where            = nullptr;
}

/*----------------------------------------------------------------------------*/
/* clear the M members and registers                                          */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vClearMembersRegisters(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G_vClearMembers();
  G_vClearRegisters();
}

/*----------------------------------------------------------------------------*/
/* clear the general M registers                                              */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vClearRegisters(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_PriorToMain      = nullptr;
  M.e_PriorToMain      = CK_NONE;
  M.p_MainCard         = nullptr;
  M.e_MainCard         = CK_NONE;
  M.p_SubOfMain        = nullptr;
  M.e_SubOfMain        = CK_NONE;
  M.p_Sub2OfMain       = nullptr;
  M.e_Sub2OfMain       = CK_NONE;
  M.p_SubsequentToMain = nullptr;
  M.e_SubsequentToMain = CK_NONE;

  M.p_SubMember        = nullptr;
  M.e_SubMember        = CK_NONE;

  m_pTerm_0            = nullptr;
  m_pTerm_1            = nullptr;
}

/*----------------------------------------------------------------------------*/
/* clear the general M registers if they match given kind                     */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vClearRegistersIf(eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (eCard == M.e_PriorToMain ||
       (M.p_PriorToMain && eCard == M.p_PriorToMain->eCard())) {
    M.p_PriorToMain = nullptr;
    M.e_PriorToMain = CK_NONE;
  }
  if (eCard == M.e_MainCard ||
       (M.p_MainCard && eCard == M.p_MainCard->eCard())) {
    M.p_MainCard = nullptr;
    M.e_MainCard = CK_NONE;
  }
  if (eCard == M.e_SubOfMain ||
       (M.p_SubOfMain && eCard == M.p_SubOfMain->eCard())) {
    M.p_SubOfMain = nullptr;
    M.e_SubOfMain = CK_NONE;
  }
  if (eCard == M.e_Sub2OfMain ||
      (M.p_Sub2OfMain && eCard == M.p_Sub2OfMain->eCard())) {
    M.p_Sub2OfMain = nullptr;
    M.e_Sub2OfMain = CK_NONE;
  }
  if (eCard == M.e_SubsequentToMain ||
       (M.p_SubsequentToMain && eCard == M.p_SubsequentToMain->eCard())) {
    M.p_SubsequentToMain = nullptr;
    M.e_SubsequentToMain = CK_NONE;
  }
  if (eCard == M.e_SubMember ||
       (M.p_SubMember && eCard == M.p_SubMember->eCard())) {
    M.p_SubMember = nullptr;
    M.e_SubMember = CK_NONE;
  }
}

/*----------------------------------------------------------------------------*/
/* clear the general M registers if they match given pointer                  */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vClearRegistersIf(Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (pCard == M.p_PriorToMain) {
    M.p_PriorToMain = nullptr;
    M.e_PriorToMain = CK_NONE;
  }
  if (pCard == M.p_MainCard) {
    M.p_MainCard = nullptr;
    M.e_MainCard = CK_NONE;
  }
  if (pCard == M.p_SubOfMain) {
    M.p_SubOfMain = nullptr;
    M.e_SubOfMain = CK_NONE;
  }
  if (pCard == M.p_SubsequentToMain) {
    M.p_SubsequentToMain = nullptr;
    M.e_SubsequentToMain = CK_NONE;
  }
  if (pCard == M.p_SubMember) {
    M.p_SubMember = nullptr;
    M.e_SubMember = CK_NONE;
  }
}

/*----------------------------------------------------------------------------*/
/* push current M iterator onto iter stack                                    */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vPushIter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Xsentinals* pTransformIterSave = new Xsentinals;

  pTransformIterSave->vCopyFrom(this->aXregs());

  m_pTransformIterList->vPush(pTransformIterSave);
}

/*----------------------------------------------------------------------------*/
/* push current M iterator onto iter stack                                    */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vPushIter(Board* pBoard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Xsentinals* pTransformIterSave = new Xsentinals;

  pTransformIterSave->vCopyFrom(this->aXregs());

  m_pTransformIterList->vPush(pTransformIterSave);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  M.pBoard = pBoard;
  M.pDeck  = M.pBoard->pDeck();

  M.xBI.vChangeBoard(M.pBoard);
}


/*----------------------------------------------------------------------------*/
/* pop/restore M iterator from iter stack                                     */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vPopIter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Xsentinals* pTransformIterRestore = m_pTransformIterList->pPop();

  this->vCopyFrom(pTransformIterRestore->aXregs());

  delete pTransformIterRestore;
}


/*----------------------------------------------------------------------------*/
/* restart the M iterator                                                     */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vRestart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.xBI.vRestart();

  G_vClearMembers();
  G_vClearRegisters();
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
/* update member card                                                         */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vUpdateMember(eCK eMember, Card* pCard, const char* asFuncNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  switch (eMember) {

    case CK_ABSJOIN:          *((Card**)&M.p_absJoin)          = pCard; break;
    case CK_AGGREG:           *((Card**)&M.p_aggreg)           = pCard; break;
    case CK_BOOLCALC:         *((Card**)&M.p_boolCalc)         = pCard; break;
    case CK_BOUNDS:           *((Card**)&M.p_bounds)           = pCard; break;
    case CK_CMPPRED:          *((Card**)&M.p_cmpPred)          = pCard; break;
    case CK_COMBINED:         *((Card**)&M.p_combined)         = pCard; break;
    case CK_COMMENT:          *((Card**)&M.p_comment)          = pCard; break;
    case CK_COPTNJOIN:        *((Card**)&M.p_coPtnJoin)        = pCard; break;
    case CK_COPTNJOINLIST:    *((Card**)&M.p_coPtnJoinList)    = pCard; break;
    case CK_COPTNPRED:        *((Card**)&M.p_coPtnPred)        = pCard; break;
    case CK_COPTNPREDLIST:    *((Card**)&M.p_coPtnPredList)    = pCard; break;
    case CK_CROSSPTNJOIN:     *((Card**)&M.p_crossPtnJoin)     = pCard; break;
    case CK_CROSSPTNJOINLIST: *((Card**)&M.p_crossPtnJoinList) = pCard; break;
    case CK_EXPR:             *((Card**)&M.p_expr)             = pCard; break;
    case CK_FILTER:           *((Card**)&M.p_filter)           = pCard; break;
    case CK_GBYELIST:         *((Card**)&M.p_gbyElist)         = pCard; break;
    case CK_IDXCOL:           *((Card**)&M.p_idxCol)           = pCard; break;
    case CK_IDXCOLLIST:       *((Card**)&M.p_idxColList)       = pCard; break;
    case CK_IDXCOLNM:         *((Card**)&M.p_idxColNm)         = pCard; break;
    case CK_INDEX:            *((Card**)&M.p_index)            = pCard; break;
    case CK_INDEXJOIN:        *((Card**)&M.p_indexJoin)        = pCard; break;
    case CK_INDEXLIST:        *((Card**)&M.p_indexList)        = pCard; break;
    case CK_INDEXNM:          *((Card**)&M.p_indexNm)          = pCard; break;
    case CK_INDEXSCAN:        *((Card**)&M.p_indexScan)        = pCard; break;
    case CK_JOINORDER:        *((Card**)&M.p_joinOrder)        = pCard; break;
    case CK_JOINORDERSEQ:     *((Card**)&M.p_joinOrderSeq)     = pCard; break;
    case CK_JOINPREDLIST:     *((Card**)&M.p_joinPredList)     = pCard; break;
    case CK_KEYEXISTS:        *((Card**)&M.p_keyExists)        = pCard; break;
    case CK_LBOUND:           *((Card**)&M.p_lBound)           = pCard; break;
    case CK_PARAM:            *((Card**)&M.p_param)            = pCard; break;
    case CK_PARAMLIST:        *((Card**)&M.p_paramList)        = pCard; break;
    case CK_PARTITION:        *((Card**)&M.p_partition)        = pCard; break;
    case CK_PERMUTATION:      *((Card**)&M.p_permutation)      = pCard; break;
    case CK_PROJECT:          *((Card**)&M.p_project)          = pCard; break;
    case CK_PTNCOL:           *((Card**)&M.p_ptnCol)           = pCard; break;
    case CK_PTNCOLLIST:       *((Card**)&M.p_ptnColList)       = pCard; break;
    case CK_PTNCOLNM:         *((Card**)&M.p_ptnColNm)         = pCard; break;
    case CK_PTNDJOIN:         *((Card**)&M.p_ptndJoin)         = pCard; break;
    case CK_PTNDJOINSET:      *((Card**)&M.p_ptndJoinSet)      = pCard; break;
    case CK_RPNCALC:          *((Card**)&M.p_rpnCalc)          = pCard; break;
    case CK_SBYELIST:         *((Card**)&M.p_sbyElist)         = pCard; break;
    case CK_SCAN:             *((Card**)&M.p_scan)             = pCard; break;
    case CK_SEND:             *((Card**)&M.p_send)             = pCard; break;
    case CK_SIZE:             *((Card**)&M.p_size)             = pCard; break;
    case CK_SORT:             *((Card**)&M.p_sort)             = pCard; break;
    case CK_STAGE:            *((Card**)&M.p_stage)            = pCard; break;
    case CK_STMCOL:           *((Card**)&M.p_stmCol)           = pCard; break;
    case CK_STMCOLLIST:       *((Card**)&M.p_stmColList)       = pCard; break;
    case CK_STMCOLNM:         *((Card**)&M.p_stmColNm)         = pCard; break;
    case CK_STMCOLORD:        *((Card**)&M.p_stmColOrd)        = pCard; break;
    case CK_STREAM:           *((Card**)&M.p_stream)           = pCard; break;
    case CK_SUBSET:           *((Card**)&M.p_subSet)           = pCard; break;
    case CK_TABLE:            *((Card**)&M.p_table)            = pCard; break;
    case CK_TABLELIST:        *((Card**)&M.p_tableList)        = pCard; break;
    case CK_TABLENM:          *((Card**)&M.p_tableNm)          = pCard; break;
    case CK_TABLEPTN:         *((Card**)&M.p_tablePtn)         = pCard; break;
    case CK_TERM:             *((Card**)&M.p_term)             = pCard; break;
    case CK_TOPOLOGY:         *((Card**)&M.p_topology)         = pCard; break;
    case CK_TSTPRED:          *((Card**)&M.p_tstPred)          = pCard; break;
    case CK_UBOUND:           *((Card**)&M.p_uBound)           = pCard; break;
    case CK_WHERE:            *((Card**)&M.p_where)            = pCard; break;

    default:
      NEVER_REACH
  }

  if (! m_bTrace)
    return;

  vTrace("  %s %s\n"_Stz,asFuncNm,szT(eMember));
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

  if ((M.p_SubMember = pMember->pFindSubCard(eSubCard))) {
    M.e_SubMember = M.p_SubMember->eCard();
    if (bUpdateMember)
      G_vUpdateMember(M.e_SubMember, M.p_SubMember, __func__);
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

  if ((M.p_SubMember = pMember->pFindSubCard(eSub))) {
    if (M.p_SubMember->bSame(szValue)) {
      M.e_SubMember = M.p_SubMember->eCard();
      if (bUpdateMember)
        G_vUpdateMember(M.e_SubMember, M.p_SubMember, __func__);
      return true;
    }
  }

  if (bUpdateMember)
    G_vUpdateMember(eSub, nullptr, __func__);
  return false;
}


/*----------------------------------------------------------------------------*/
/* resolve card (right-left before given i_Before                             */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bResolveCard(eCK eCard, Stz* szWithValue, i8 i_Before)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  for (i8 i_Main = i_Before - 1; i_Main >= 0; i_Main--) {
    Card* pMain = M.pDeck->pGetChild(i_Main);

    if (m_pCard_resolved = pMain->pFindValueRecursive(eCard,szWithValue,m_pCard_parent)) {
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

  i8 i_Before = M.pDeck->iFindChild(pBefore);
  if (C_UNKNOWN == i_Before)
    return false;

  bool b = G_bResolveCard(eCard, szWithValue, i_Before);

  return b;
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
/* update member card                                                         */
/*----------------------------------------------------------------------------*/
Stz* Xgearing::szMemberNm(eCK eMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  switch (eMember) {

    case CK_ABSJOIN:          return "M.p_absJoin"_Stz;
    case CK_AGGREG:           return "M.p_aggreg"_Stz;
    case CK_BOOLCALC:         return "M.p_boolCalc"_Stz;
    case CK_BOUNDS:           return "M.p_bounds"_Stz;
    case CK_CMPPRED:          return "M.p_cmpPred"_Stz;
    case CK_COMBINED:         return "M.p_combined"_Stz;
    case CK_COMMENT:          return "M.p_comment"_Stz;
    case CK_COPTNJOIN:        return "M.p_coPtnJoin"_Stz;
    case CK_COPTNJOINLIST:    return "M.p_coPtnJoinList"_Stz;
    case CK_COPTNPRED:        return "M.p_coPtnPred"_Stz;
    case CK_COPTNPREDLIST:    return "M.p_coPtnPredList"_Stz;
    case CK_CROSSPTNJOIN:     return "M.p_crossPtnJoin"_Stz;
    case CK_CROSSPTNJOINLIST: return "M.p_crossPtnJoinList"_Stz;
    case CK_EXPR:             return "M.p_expr"_Stz;
    case CK_FILTER:           return "M.p_filter"_Stz;
    case CK_GBYELIST:         return "M.p_gbyElist"_Stz;
    case CK_IDXCOLLIST:       return "M.p_idxColList"_Stz;
    case CK_IDXCOLNM:         return "M.p_idxColNm"_Stz;
    case CK_INDEX:            return "M.p_index"_Stz;
    case CK_INDEXJOIN:        return "M.p_indexJoin"_Stz;
    case CK_INDEXLIST:        return "M.p_indexList"_Stz;
    case CK_INDEXNM:          return "M.p_indexNm"_Stz;
    case CK_INDEXSCAN:        return "M.p_indexScan"_Stz;
    case CK_JOINORDER:        return "M.p_joinOrder"_Stz;
    case CK_JOINORDERSEQ:     return "M.p_joinOrderSeq"_Stz;
    case CK_JOINPREDLIST:     return "M.p_joinPredList"_Stz;
    case CK_KEYEXISTS:        return "M.p_keyExists"_Stz;
    case CK_LBOUND:           return "M.p_lBound"_Stz;
    case CK_PARAM:            return "M.p_param"_Stz;
    case CK_PARAMLIST:        return "M.p_paramList"_Stz;
    case CK_PARTITION:        return "M.p_partition"_Stz;
    case CK_PERMUTATION:      return "M.p_permutation"_Stz;
    case CK_PROJECT:          return "M.p_project"_Stz;
    case CK_PTNCOL:           return "M.p_ptnCol"_Stz;
    case CK_PTNCOLLIST:       return "M.p_ptnColList"_Stz;
    case CK_PTNCOLNM:         return "M.p_ptnColNm"_Stz;
    case CK_PTNDJOIN:         return "M.p_ptndJoin"_Stz;
    case CK_PTNDJOINSET:      return "M.p_ptndJoinSet"_Stz;
    case CK_RPNCALC:          return "M.p_rpnCalc"_Stz;
    case CK_SBYELIST:         return "M.p_sbyElist"_Stz;
    case CK_SCAN:             return "M.p_scan"_Stz;
    case CK_SEND:             return "M.p_send"_Stz;
    case CK_SIZE:             return "M.p_size"_Stz;
    case CK_SORT:             return "M.p_sort"_Stz;
    case CK_STAGE:            return "M.p_stage"_Stz;
    case CK_STMCOL:           return "M.p_stmCol"_Stz;
    case CK_STMCOLLIST:       return "M.p_stmColList"_Stz;
    case CK_STMCOLNM:         return "M.p_stmColNm"_Stz;
    case CK_STMCOLORD:        return "M.p_stmColOrd"_Stz;
    case CK_STREAM:           return "M.p_stream"_Stz;
    case CK_SUBSET:           return "M.p_subSet"_Stz;
    case CK_TABLE:            return "M.p_table"_Stz;
    case CK_TABLELIST:        return "M.p_tableList"_Stz;
    case CK_TABLENM:          return "M.p_tableNm"_Stz;
    case CK_TABLEPTN:         return "M.p_tablePtn"_Stz;
    case CK_TERM:             return "M.p_term"_Stz;
    case CK_TOPOLOGY:         return "M.p_topology"_Stz;
    case CK_TSTPRED:          return "M.p_tstPred"_Stz;
    case CK_UBOUND:           return "M.p_uBound"_Stz;
    case CK_WHERE:            return "M.p_where"_Stz;

    default:
      NEVER_REACH
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
