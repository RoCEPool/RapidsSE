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
  m_bPrintXformForce = false;
  Job* pJob = MY::pJob();
  TagSet* pJobTags = pJob->pJobTags();
  m_bInteractive = pJobTags->bHasName("<interactive>"_Stz);
  m_bBeenCued = false;
  m_bCueingDisabled = false;

  m_pPermGen   = nullptr;

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
  m_pCard_yanked   = nullptr;

  m_pTransformIterList = new TransformIterList;


  // the M iterator - - - - - - - - - - - - - - - - - - - - - -

  G.pBoard = nullptr;
  G.pDeck = nullptr;

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

  return G.pBoard;
}

/*----------------------------------------------------------------------------*/
/* cue user                                                                  */
/*----------------------------------------------------------------------------*/
eOA Xgearing::eCueUser(Stz* szHand, Stz* szTransformation, i8 iLineNo)
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

  if (C_UNKNOWN != iLineNo)
    sPrompt.vCatSprint(" [%lld]"_Stz,iLineNo);

  if (! m_bPrintXformForce) {
    if (!m_bBeenCued && AM::JOB::bHasTag(C_GLEAN_CUE_FROM)) {
      Stz *szTagCue = AM::JOB::szTagValue(C_GLEAN_CUE_FROM);
      if (szTagCue->bSame(szHand) && szTransformation) {
        printf("%s\n", sPrompt.azChar());
        return OA_CONTINUE_TRANSFORMING;
      }
      if (szTransformation && (!szTagCue->bSame(szTransformation))) {
        printf("%s\n", sPrompt.azChar());
        return OA_CONTINUE_TRANSFORMING;
      }
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

  G.pDeck->vRenumberRecursive();

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
void Xgearing::vPrintBoard(bool bCue, i8 iLineNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((C_UNKNOWN == iLineNo) && (! m_bPrintXform) && (! m_bPrintXformForce))
    return;

  for (;;) {
    G.pBoard->vPrintBoard(szHand(),
                          szTransformation(),
                          szDescription());

    if (! bCue)
      break;

    if (OA_REPRINT_BOARD != eCueUser(szHand(),
                                     szTransformation(),
                                     iLineNo))
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
/* set print phase force                                                      */
/*----------------------------------------------------------------------------*/
void Xgearing::vPrintXformForce(bool bPrintXformForce)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_bPrintXformForce = bPrintXformForce;
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

  G.pBoard->vTransformNoInc();

  G.xBI.vRestart();
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

  G.pBoard = m_pBench->pGetBoard(eBoard,i_SubBoard);
  G.pDeck  = G.pBoard->pDeck();

  G.xBI.vChangeBoard(G.pBoard);
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

  if ((G.p_MainCard = G.xBI.pEachMain())) {
    G.e_MainCard = G.p_MainCard->eCard();
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

  if ((G.p_MainCard = G.xBI.pEachMain(eOfMainKind))) {
    G.e_MainCard = G.p_MainCard->eCard();
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

  while ((G.p_MainCard = G.xBI.pEachMain(eOfMainKind))) {
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

  while ((G.p_MainCard = G.xBI.pEachMain(eOfMainKind))) {
    if (! (G.p_SubOfMain = G.p_MainCard->pFindSubCard(eAndSubKind1)))
      continue;
    if (! (G.p_Sub2OfMain = G.p_MainCard->pFindSubCard(eAndSubKind2)))
      continue;

    G.e_MainCard = G.p_MainCard->eCard();
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
/* skip over next main ; optionally clear M members and registers             */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vSkipNextMain(bool bClearMembers, bool bClearRegisters)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.xBI.vForward();

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

  if (G.xBI.bMore())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* remove main card from deck (do not clear M members or registers)           */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vRemoveMain(eCK eCrossCheck)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.xBI.vRemoveCurr(eCrossCheck);
}

/*----------------------------------------------------------------------------*/
/* remove ain card from deck and delete it; clear M member to null            */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vRemoveDeleteMain(eCK eCrossCheck)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.xBI.vRemoveDeleteCurr(eCrossCheck);

  G.p_MainCard = nullptr;
  G.e_MainCard = CK_NONE;

  G_vUpdateMember(eCrossCheck, nullptr, __func__);
}

/*----------------------------------------------------------------------------*/
/* is there a card immediately after current main card?                       */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bSubsequentToMain(bool bUpdateMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if ((G.p_SubsequentToMain = G.xBI.pNextMain())) {
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
/* fetch M member by decoding given eMember                                   */
/*----------------------------------------------------------------------------*/
Card* Xgearing::G_pGetMember(eCK eMember)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  switch (eMember) {

    case CK_ABSJOIN:          return G.p_absJoin;
    case CK_AGGREG:           return G.p_aggreg;
    case CK_BOOLCALC:         return G.p_boolCalc;
    case CK_BOUNDS:           return G.p_bounds;
    case CK_CALL:             return G.p_call;
    case CK_CMPPRED:          return G.p_cmpPred;
    case CK_COMBINED:         return G.p_combined;
    case CK_COMMENT:          return G.p_comment;
    case CK_CONSTRAINT:       return G.p_constraint;
    case CK_COPTNJOIN:        return G.p_coPtnJoin;
    case CK_COPTNJOINLIST:    return G.p_coPtnJoinList;
    case CK_COPTNPRED:        return G.p_coPtnPred;
    case CK_COPTNPREDLIST:    return G.p_coPtnPredList;
    case CK_CROSSPTNJOIN:     return G.p_crossPtnJoin;
    case CK_CROSSPTNJOINLIST: return G.p_crossPtnJoinList;
    case CK_EXEPRED:          return G.p_exePred;
    case CK_EXPR:             return G.p_expr;
    case CK_FILTER:           return G.p_filter;
    case CK_GBYELIST:         return G.p_gbyElist;
    case CK_IDXCOL:           return G.p_idxCol;
    case CK_IDXCOLLIST:       return G.p_idxColList;
    case CK_IDXCOLNM:         return G.p_idxColNm;
    case CK_INDEX:            return G.p_index;
    case CK_INDEXJOIN:        return G.p_indexJoin;
    case CK_INDEXLIST:        return G.p_indexList;
    case CK_INDEXNM:          return G.p_indexNm;
    case CK_INDEXSCAN:        return G.p_indexScan;
    case CK_JOINORDER:        return G.p_joinOrder;
    case CK_JOINORDERSEQ:     return G.p_joinOrderSeq;
    case CK_JOINPREDLIST:     return G.p_joinPredList;
    case CK_KEYEXISTS:        return G.p_keyExists;
    case CK_LBOUND:           return G.p_lBound;
    case CK_PARAM:            return G.p_param;
    case CK_PARAMLIST:        return G.p_paramList;
    case CK_PARTITION:        return G.p_partition;
    case CK_PERMUTATION:      return G.p_permutation;
    case CK_PROJECT:          return G.p_project;
    case CK_PTNCOL:           return G.p_ptnCol;
    case CK_PTNCOLLIST:       return G.p_ptnColList;
    case CK_PTNCOLNM:         return G.p_ptnColNm;
    case CK_PTNDJOIN:         return G.p_ptndJoin;
    case CK_PTNDJOINSET:      return G.p_ptndJoinSet;
    case CK_RPNCALC:          return G.p_rpnCalc;
    case CK_SBYELIST:         return G.p_sbyElist;
    case CK_SCAN:             return G.p_scan;
    case CK_SEND:             return G.p_send;
    case CK_SIZE:             return G.p_size;
    case CK_SORT:             return G.p_sort;
    case CK_STAGE:            return G.p_stage;
    case CK_STMCOL:           return G.p_stmCol;
    case CK_STMCOLLIST:       return G.p_stmColList;
    case CK_STMCOLNM:         return G.p_stmColNm;
    case CK_STMCOLODN:        return G.p_stmColOdn;
    case CK_STREAM:           return G.p_stream;
    case CK_SUBSET:           return G.p_subSet;
    case CK_TABLE:            return G.p_table;
    case CK_TABLELIST:        return G.p_tableList;
    case CK_TABLENM:          return G.p_tableNm;
    case CK_TABLEPTN:         return G.p_tablePtn;
    case CK_TBLCOLODN:        return G.p_tblColOdn;
    case CK_TERM:             return G.p_term;
    case CK_TOPOLOGY:         return G.p_topology;
    case CK_TSTPRED:          return G.p_tstPred;
    case CK_UBOUND:           return G.p_uBound;
    case CK_WHERE:            return G.p_where;

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

  if ((G.p_PriorToMain = G.xBI.pPrevMain())) {
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

  if ((G.p_PriorToMain = G.xBI.pPrevMainIf(eOfPriorKind))) {
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

  if ((G.p_SubsequentToMain = G.xBI.pNextMain())) {
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

  if ((G.p_SubsequentToMain = G.xBI.pNextMainIf(eOfSubsequentKind))) {
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
/* locate card (left-right) after given i_After                               */
/*----------------------------------------------------------------------------*/
bool Xgearing::G_bLocateCard(eCK eCard, Stz* szWithValue, i8 i_After)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

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
/* clear the M members                                                        */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vClearMembers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.p_absJoin          = nullptr;
  G.p_aggreg           = nullptr;
  G.p_boolCalc         = nullptr;
  G.p_bounds           = nullptr;
  G.p_call             = nullptr;
  G.p_cmpPred          = nullptr;
  G.p_combined         = nullptr;
  G.p_comment          = nullptr;
  G.p_constraint       = nullptr;
  G.p_coPtnJoin        = nullptr;
  G.p_coPtnJoinList    = nullptr;
  G.p_coPtnPred        = nullptr;
  G.p_coPtnPredList    = nullptr;
  G.p_crossPtnJoin     = nullptr;
  G.p_crossPtnJoinList = nullptr;
  G.p_exePred          = nullptr;
  G.p_expr             = nullptr;
  G.p_filter           = nullptr;
  G.p_gbyElist         = nullptr;
  G.p_idxCol           = nullptr;
  G.p_idxColList       = nullptr;
  G.p_idxColNm         = nullptr;
  G.p_index            = nullptr;
  G.p_indexJoin        = nullptr;
  G.p_indexList        = nullptr;
  G.p_indexNm          = nullptr;
  G.p_indexScan        = nullptr;
  G.p_joinOrder        = nullptr;
  G.p_joinOrderSeq     = nullptr;
  G.p_joinPredList     = nullptr;
  G.p_keyExists        = nullptr;
  G.p_lBound           = nullptr;
  G.p_param            = nullptr;
  G.p_paramList        = nullptr;
  G.p_partition        = nullptr;
  G.p_permutation      = nullptr;
  G.p_permutation_low  = nullptr;
  G.p_project          = nullptr;
  G.p_ptnCol           = nullptr;
  G.p_ptnColList       = nullptr;
  G.p_ptnColNm         = nullptr;
  G.p_ptndJoin         = nullptr;
  G.p_ptndJoinSet      = nullptr;
  G.p_rpnCalc          = nullptr;
  G.p_sbyElist         = nullptr;
  G.p_scan             = nullptr;
  G.p_send             = nullptr;
  G.p_size             = nullptr;
  G.p_sort             = nullptr;
  G.p_stage            = nullptr;
  G.p_stmCol           = nullptr;
  G.p_stmColList       = nullptr;
  G.p_stmColNm         = nullptr;
  G.p_stmColOdn        = nullptr;
  G.p_stream           = nullptr;
  G.p_subSet           = nullptr;
  G.p_table            = nullptr;
  G.p_tableList        = nullptr;
  G.p_tableNm          = nullptr;
  G.p_tablePtn         = nullptr;
  G.p_tblColOdn        = nullptr;
  G.p_term             = nullptr;
  G.p_topology         = nullptr;
  G.p_tstPred          = nullptr;
  G.p_uBound           = nullptr;
  G.p_where            = nullptr;
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

  G.p_Located          = nullptr;
  G.e_Located          = CK_NONE;
  G.p_PriorToMain      = nullptr;
  G.e_PriorToMain      = CK_NONE;
  G.p_MainCard         = nullptr;
  G.e_MainCard         = CK_NONE;
  G.p_SubLocated       = nullptr;
  G.e_SubLocated       = CK_NONE;
  G.p_SubLocatedParent = nullptr;
  G.e_SubLocatedParent = CK_NONE;
  G.p_SubOfMain        = nullptr;
  G.e_SubOfMain        = CK_NONE;
  G.p_Sub2OfMain       = nullptr;
  G.e_Sub2OfMain       = CK_NONE;
  G.p_SubsequentToMain = nullptr;
  G.e_SubsequentToMain = CK_NONE;

  G.p_SubMember        = nullptr;
  G.e_SubMember        = CK_NONE;

  m_pTerm_0            = nullptr;
  m_pTerm_1            = nullptr;

  G.sz_SubQueryNm      = nullptr;
}

/*----------------------------------------------------------------------------*/
/* clear the general M registers if they match given kind                     */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vClearRegistersIf(eCK eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (eCard == G.e_PriorToMain ||
       (G.p_PriorToMain && eCard == G.p_PriorToMain->eCard())) {
    G.p_PriorToMain = nullptr;
    G.e_PriorToMain = CK_NONE;
  }
  if (eCard == G.e_MainCard ||
       (G.p_MainCard && eCard == G.p_MainCard->eCard())) {
    G.p_MainCard = nullptr;
    G.e_MainCard = CK_NONE;
  }
  if (eCard == G.e_SubOfMain ||
       (G.p_SubOfMain && eCard == G.p_SubOfMain->eCard())) {
    G.p_SubOfMain = nullptr;
    G.e_SubOfMain = CK_NONE;
  }
  if (eCard == G.e_Sub2OfMain ||
      (G.p_Sub2OfMain && eCard == G.p_Sub2OfMain->eCard())) {
    G.p_Sub2OfMain = nullptr;
    G.e_Sub2OfMain = CK_NONE;
  }
  if (eCard == G.e_SubsequentToMain ||
       (G.p_SubsequentToMain && eCard == G.p_SubsequentToMain->eCard())) {
    G.p_SubsequentToMain = nullptr;
    G.e_SubsequentToMain = CK_NONE;
  }
  if (eCard == G.e_SubMember ||
       (G.p_SubMember && eCard == G.p_SubMember->eCard())) {
    G.p_SubMember = nullptr;
    G.e_SubMember = CK_NONE;
  }
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
/* push current M iterator onto iter stack                                    */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vPushIter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Xsentinals* pTransformIterSave = new Xsentinals;

  pTransformIterSave->vCopyFrom(this->aXsentinels());

  m_pTransformIterList->vPush(pTransformIterSave);
}

/*----------------------------------------------------------------------------*/
/* push current M iterator onto iter stack                                    */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vPushBoard(Board* pBoard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Xsentinals* pTransformIterSave = new Xsentinals;

  pTransformIterSave->vCopyFrom(this->aXsentinels());

  m_pTransformIterList->vPush(pTransformIterSave);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.pBoard = pBoard;
  G.pDeck  = G.pBoard->pDeck();

  G.xBI.vChangeBoard(G.pBoard);
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

  G_vPushIter(pSubQueryBoard);

  return true;
}

/*----------------------------------------------------------------------------*/
/* push current M iterator onto iter stack                                    */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vPushIter(Board* pBoard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Xsentinals* pTransformIterSave = new Xsentinals;

  pTransformIterSave->vCopyFrom(this->aXsentinels());

  m_pTransformIterList->vPush(pTransformIterSave);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.pBoard = pBoard;
  G.pDeck  = G.pBoard->pDeck();

  G.xBI.vChangeBoard(G.pBoard);
}


/*----------------------------------------------------------------------------*/
/* pop/restore M iterator from iter stack                                     */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vPopBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Xsentinals* pTransformIterRestore = m_pTransformIterList->pPop();

  this->vCopyFrom(pTransformIterRestore->aXsentinels());

  delete pTransformIterRestore;
}

/*----------------------------------------------------------------------------*/
/* pop/restore M iterator from iter stack                                     */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vPopIter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Xsentinals* pTransformIterRestore = m_pTransformIterList->pPop();

  this->vCopyFrom(pTransformIterRestore->aXsentinels());

  delete pTransformIterRestore;
}


/*----------------------------------------------------------------------------*/
/* restart the M iterator                                                     */
/*----------------------------------------------------------------------------*/
void Xgearing::G_vRestart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.xBI.vRestart();

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

    case CK_ABSJOIN:          *((Card**)&G.p_absJoin)          = pCard; break;
    case CK_AGGREG:           *((Card**)&G.p_aggreg)           = pCard; break;
    case CK_BOOLCALC:         *((Card**)&G.p_boolCalc)         = pCard; break;
    case CK_BOUNDS:           *((Card**)&G.p_bounds)           = pCard; break;
    case CK_CALL:             *((Card**)&G.p_call)             = pCard; break;
    case CK_CMPPRED:          *((Card**)&G.p_cmpPred)          = pCard; break;
    case CK_COMBINED:         *((Card**)&G.p_combined)         = pCard; break;
    case CK_COMMENT:          *((Card**)&G.p_comment)          = pCard; break;
    case CK_CONSTRAINT:       *((Card**)&G.p_constraint)       = pCard; break;
    case CK_COPTNJOIN:        *((Card**)&G.p_coPtnJoin)        = pCard; break;
    case CK_COPTNJOINLIST:    *((Card**)&G.p_coPtnJoinList)    = pCard; break;
    case CK_COPTNPRED:        *((Card**)&G.p_coPtnPred)        = pCard; break;
    case CK_COPTNPREDLIST:    *((Card**)&G.p_coPtnPredList)    = pCard; break;
    case CK_CROSSPTNJOIN:     *((Card**)&G.p_crossPtnJoin)     = pCard; break;
    case CK_CROSSPTNJOINLIST: *((Card**)&G.p_crossPtnJoinList) = pCard; break;
    case CK_EXEPRED:          *((Card**)&G.p_exePred)          = pCard; break;
    case CK_EXPR:             *((Card**)&G.p_expr)             = pCard; break;
    case CK_FILTER:           *((Card**)&G.p_filter)           = pCard; break;
    case CK_GBYELIST:         *((Card**)&G.p_gbyElist)         = pCard; break;
    case CK_IDXCOL:           *((Card**)&G.p_idxCol)           = pCard; break;
    case CK_IDXCOLLIST:       *((Card**)&G.p_idxColList)       = pCard; break;
    case CK_IDXCOLNM:         *((Card**)&G.p_idxColNm)         = pCard; break;
    case CK_INDEX:            *((Card**)&G.p_index)            = pCard; break;
    case CK_INDEXJOIN:        *((Card**)&G.p_indexJoin)        = pCard; break;
    case CK_INDEXLIST:        *((Card**)&G.p_indexList)        = pCard; break;
    case CK_INDEXNM:          *((Card**)&G.p_indexNm)          = pCard; break;
    case CK_INDEXSCAN:        *((Card**)&G.p_indexScan)        = pCard; break;
    case CK_JOINORDER:        *((Card**)&G.p_joinOrder)        = pCard; break;
    case CK_JOINORDERSEQ:     *((Card**)&G.p_joinOrderSeq)     = pCard; break;
    case CK_JOINPREDLIST:     *((Card**)&G.p_joinPredList)     = pCard; break;
    case CK_KEYEXISTS:        *((Card**)&G.p_keyExists)        = pCard; break;
    case CK_LBOUND:           *((Card**)&G.p_lBound)           = pCard; break;
    case CK_PARAM:            *((Card**)&G.p_param)            = pCard; break;
    case CK_PARAMLIST:        *((Card**)&G.p_paramList)        = pCard; break;
    case CK_PARTITION:        *((Card**)&G.p_partition)        = pCard; break;
    case CK_PERMUTATION:      *((Card**)&G.p_permutation)      = pCard; break;
    case CK_PROJECT:          *((Card**)&G.p_project)          = pCard; break;
    case CK_PTNCOL:           *((Card**)&G.p_ptnCol)           = pCard; break;
    case CK_PTNCOLLIST:       *((Card**)&G.p_ptnColList)       = pCard; break;
    case CK_PTNCOLNM:         *((Card**)&G.p_ptnColNm)         = pCard; break;
    case CK_PTNDJOIN:         *((Card**)&G.p_ptndJoin)         = pCard; break;
    case CK_PTNDJOINSET:      *((Card**)&G.p_ptndJoinSet)      = pCard; break;
    case CK_RPNCALC:          *((Card**)&G.p_rpnCalc)          = pCard; break;
    case CK_SBYELIST:         *((Card**)&G.p_sbyElist)         = pCard; break;
    case CK_SCAN:             *((Card**)&G.p_scan)             = pCard; break;
    case CK_SEND:             *((Card**)&G.p_send)             = pCard; break;
    case CK_SIZE:             *((Card**)&G.p_size)             = pCard; break;
    case CK_SORT:             *((Card**)&G.p_sort)             = pCard; break;
    case CK_STAGE:            *((Card**)&G.p_stage)            = pCard; break;
    case CK_STMCOL:           *((Card**)&G.p_stmCol)           = pCard; break;
    case CK_STMCOLLIST:       *((Card**)&G.p_stmColList)       = pCard; break;
    case CK_STMCOLNM:         *((Card**)&G.p_stmColNm)         = pCard; break;
    case CK_STMCOLODN:        *((Card**)&G.p_stmColOdn)        = pCard; break;
    case CK_STREAM:           *((Card**)&G.p_stream)           = pCard; break;
    case CK_SUBSET:           *((Card**)&G.p_subSet)           = pCard; break;
    case CK_TABLE:            *((Card**)&G.p_table)            = pCard; break;
    case CK_TABLELIST:        *((Card**)&G.p_tableList)        = pCard; break;
    case CK_TABLENM:          *((Card**)&G.p_tableNm)          = pCard; break;
    case CK_TABLEPTN:         *((Card**)&G.p_tablePtn)         = pCard; break;
    case CK_TBLCOLODN:        *((Card**)&G.p_tblColOdn)        = pCard; break;
    case CK_TERM:             *((Card**)&G.p_term)             = pCard; break;
    case CK_TOPOLOGY:         *((Card**)&G.p_topology)         = pCard; break;
    case CK_TSTPRED:          *((Card**)&G.p_tstPred)          = pCard; break;
    case CK_UBOUND:           *((Card**)&G.p_uBound)           = pCard; break;
    case CK_WHERE:            *((Card**)&G.p_where)            = pCard; break;

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
        G_vUpdateMember(eCard, m_pCard_resolved, __func__);
        return true;
      }
    }
  }

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

    case CK_ABSJOIN:          return "G.p_absJoin"_Stz;
    case CK_AGGREG:           return "G.p_aggreg"_Stz;
    case CK_BOOLCALC:         return "G.p_boolCalc"_Stz;
    case CK_BOUNDS:           return "G.p_bounds"_Stz;
    case CK_CALL:             return "G.p_call"_Stz;
    case CK_CMPPRED:          return "G.p_cmpPred"_Stz;
    case CK_COMBINED:         return "G.p_combined"_Stz;
    case CK_COMMENT:          return "G.p_comment"_Stz;
    case CK_CONSTRAINT:       return "G.p_constraint"_Stz;
    case CK_COPTNJOIN:        return "G.p_coPtnJoin"_Stz;
    case CK_COPTNJOINLIST:    return "G.p_coPtnJoinList"_Stz;
    case CK_COPTNPRED:        return "G.p_coPtnPred"_Stz;
    case CK_COPTNPREDLIST:    return "G.p_coPtnPredList"_Stz;
    case CK_CROSSPTNJOIN:     return "G.p_crossPtnJoin"_Stz;
    case CK_CROSSPTNJOINLIST: return "G.p_crossPtnJoinList"_Stz;
    case CK_EXEPRED:          return "G.p_exePred"_Stz;
    case CK_EXPR:             return "G.p_expr"_Stz;
    case CK_FILTER:           return "G.p_filter"_Stz;
    case CK_GBYELIST:         return "G.p_gbyElist"_Stz;
    case CK_IDXCOLLIST:       return "G.p_idxColList"_Stz;
    case CK_IDXCOLNM:         return "G.p_idxColNm"_Stz;
    case CK_INDEX:            return "G.p_index"_Stz;
    case CK_INDEXJOIN:        return "G.p_indexJoin"_Stz;
    case CK_INDEXLIST:        return "G.p_indexList"_Stz;
    case CK_INDEXNM:          return "G.p_indexNm"_Stz;
    case CK_INDEXSCAN:        return "G.p_indexScan"_Stz;
    case CK_JOINORDER:        return "G.p_joinOrder"_Stz;
    case CK_JOINORDERSEQ:     return "G.p_joinOrderSeq"_Stz;
    case CK_JOINPREDLIST:     return "G.p_joinPredList"_Stz;
    case CK_KEYEXISTS:        return "G.p_keyExists"_Stz;
    case CK_LBOUND:           return "G.p_lBound"_Stz;
    case CK_PARAM:            return "G.p_param"_Stz;
    case CK_PARAMLIST:        return "G.p_paramList"_Stz;
    case CK_PARTITION:        return "G.p_partition"_Stz;
    case CK_PERMUTATION:      return "G.p_permutation"_Stz;
    case CK_PROJECT:          return "G.p_project"_Stz;
    case CK_PTNCOL:           return "G.p_ptnCol"_Stz;
    case CK_PTNCOLLIST:       return "G.p_ptnColList"_Stz;
    case CK_PTNCOLNM:         return "G.p_ptnColNm"_Stz;
    case CK_PTNDJOIN:         return "G.p_ptndJoin"_Stz;
    case CK_PTNDJOINSET:      return "G.p_ptndJoinSet"_Stz;
    case CK_RPNCALC:          return "G.p_rpnCalc"_Stz;
    case CK_SBYELIST:         return "G.p_sbyElist"_Stz;
    case CK_SCAN:             return "G.p_scan"_Stz;
    case CK_SEND:             return "G.p_send"_Stz;
    case CK_SIZE:             return "G.p_size"_Stz;
    case CK_SORT:             return "G.p_sort"_Stz;
    case CK_STAGE:            return "G.p_stage"_Stz;
    case CK_STMCOL:           return "G.p_stmCol"_Stz;
    case CK_STMCOLLIST:       return "G.p_stmColList"_Stz;
    case CK_STMCOLNM:         return "G.p_stmColNm"_Stz;
    case CK_STMCOLODN:        return "G.p_stmColOdn"_Stz;
    case CK_STREAM:           return "G.p_stream"_Stz;
    case CK_SUBSET:           return "G.p_subSet"_Stz;
    case CK_TABLE:            return "G.p_table"_Stz;
    case CK_TABLELIST:        return "G.p_tableList"_Stz;
    case CK_TABLENM:          return "G.p_tableNm"_Stz;
    case CK_TABLEPTN:         return "G.p_tablePtn"_Stz;
    case CK_TBLCOLODN:        return "G.p_tblColOdn"_Stz;
    case CK_TERM:             return "G.p_term"_Stz;
    case CK_TOPOLOGY:         return "G.p_topology"_Stz;
    case CK_TSTPRED:          return "G.p_tstPred"_Stz;
    case CK_UBOUND:           return "G.p_uBound"_Stz;
    case CK_WHERE:            return "G.p_where"_Stz;

    default:
      NEVER_REACH
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
