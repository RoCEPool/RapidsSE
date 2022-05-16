/*============================================================================*/
/* Stencil.cpp -- Op Stencil                                                  */
/*============================================================================*/

#include "BASE.h"
#include "CmdLineArgs.hpp"

#include "Stencil.hpp"
#include "Stream.hpp"
#include "StreamDef.hpp"
#include "Op.hpp"
#include "Fragments.hpp"

#include "LogicUnit.hpp"
#include "TagConstants.h"

#include "Grid.hpp"
#include "Node.hpp"
#include "Index.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "IndexTree.hpp"
#include "Stage.hpp"
#include "RowSingle.hpp"
#include "IndexStream.hpp"

ShapeRef xaShapeRef[]
{
  { "none"_Stz         }, // LS_NONE
  { "LS_INFLATE_T"_Stz }, // LS_INFLATE_T
  { "LS_FILTER_T"_Stz  }, // LS_FILTER_T
  { "LS_PROJECT_T"_Stz }, // LS_PROJECT_T
  { "LS_JOIN_KEY"_Stz  }, // LS_JOIN_KEY
  { "LS_JOIN"_Stz      }, // LS_JOIN
  { "LS_INFLATE_J"_Stz }, // LS_INFLATE_J
  { "LS_PROJECT_J"_Stz }, // LS_PROJECT_J
  { "LS_AGG_KEY"_Stz   }, // LS_AGG_KEY
  { "LS_AGG"_Stz       }, // LS_AGG
  { "LS_fetch"_Stz     }, // LS_fetch
  { "LS_PROJECT_H"_Stz }, // LS_PROJECT_H
  { "LS_FILTER_H"_Stz  }, // LS_FILTER_H
  { "LS_OUT_H"_Stz     }, // LS_OUT_H
  
  { nullptr            },
};

extern CmdLineArgs       gxCmdLineArgs;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Stencil,CN_STENCIL)
__aNewFunction(Stencil,CN_STENCIL)
__vDeleteFunction(Stencil,CN_STENCIL)
__aFooterFunction(Stencil)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stencil::Stencil(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);
    
  m_ePattern     = LP_NONE;
  m_szStageNm    = ""_Stz;
  m_bRowBuffer   = false;
  m_pPostStack   = nullptr;

  m_xStageOps.m_iAggregOp    = 0;
  m_xStageOps.m_iCombinedOp  = 0;
  m_xStageOps.m_iFilterOp    = 0;
  m_xStageOps.m_iHashOp      = 0;
  m_xStageOps.m_iIndexJoinOp = 0;
  m_xStageOps.m_iIndexScanOp = 0;
  m_xStageOps.m_iKeyExistsOp = 0;
  m_xStageOps.m_iLimitOp     = 0;
  m_xStageOps.m_iOrdAggOp    = 0;
  m_xStageOps.m_iProjectOp   = 0;
  m_xStageOps.m_iScanOp      = 0;
  m_xStageOps.m_iSortOp      = 0;
  m_xStageOps.m_iTopOp       = 0;

  m_xStageOps.m_bScan           = false;
  m_xStageOps.m_bIndexScan      = false;

  m_xStageOps.m_bPayloadOp      = false;

  m_xStageOps.m_bGeneratePotTub = true;

  m_xStageOps.m_pCombinedOp     = nullptr;
  m_xStageOps.m_pHavingFilterOp = nullptr;
  m_xStageOps.m_pPostProjectOp  = nullptr;
  m_xStageOps.m_pLimitOp        = nullptr;

  m_xStageOps.m_sLabelOrdAgg = ""_Stz;
  m_xStageOps.m_sHashExpr = ""_Stz;

  m_pBannerHead  = new CodeSynth;
  m_pBannerTail  = new CodeSynth;

  m_pPreliminary     = new CodeSynth;
  m_pPreliminaryFun0 = new CodeSynth;
  m_pPreliminaryFun1 = new CodeSynth;
  m_pPreliminaryFun2 = new CodeSynth;
  m_pPreliminaryEos = new CodeSynth;

  m_pConcluding     = new CodeSynth;
  m_pConcludingFun0 = new CodeSynth;
  m_pConcludingFun1 = new CodeSynth;
  m_pConcludingFun2 = new CodeSynth;
  m_pConcludingEos  = new CodeSynth;
  m_pOpHeader    = new CodeSynth;
  m_pOpSource    = new CodeSynth;
  m_pEmptyFun0   = new CodeSynth;
  m_pEmptyFun1   = new CodeSynth;
  m_pEmptyFun2   = new CodeSynth;

  m_pShapeCRW    = nullptr;
  m_pShapeROW    = nullptr;
  m_pShapeIDX    = nullptr;
  m_pShapeEOS    = new Shape;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append banner head
  Fragments::vAppendBannerHead(m_pBannerHead->azCode());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary defines etc
  Fragments::vAppendPreliminary(m_pPreliminary->azCode());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append banner tail
  Fragments::vAppendBannerTail(m_pBannerTail->azCode());

  _checkInitialised_(CN_STENCIL);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stencil::~Stencil(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  delete m_pPostStack;
  delete m_pBannerHead;
  delete m_pBannerTail;
  delete m_pPreliminary;
  delete m_pPreliminaryFun0;
  delete m_pPreliminaryFun1;
  delete m_pPreliminaryFun2;
  delete m_pPreliminaryEos;
  delete m_pConcluding;
  delete m_pConcludingFun0;
  delete m_pConcludingFun1;
  delete m_pConcludingFun2;
  delete m_pConcludingEos;
  delete m_pOpHeader;
  delete m_pOpSource;
  delete m_pEmptyFun0;
  delete m_pEmptyFun1;
  delete m_pEmptyFun2;

  delete m_pShapeCRW;
  delete m_pShapeROW;
  delete m_pShapeIDX;
  delete m_pShapeEOS;

}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair and delete the stencil                                              */
/*----------------------------------------------------------------------------*/
void Stencil::vRepair(Object* pObject)
{
  _static_

  Stencil* pStencil = _pStencil(pObject);

  // repair the stencil
  // ...
  
  // delete the stencil object
  delete pStencil;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* combine synthesized op code (put code for all shapes together)             */
/*----------------------------------------------------------------------------*/
void Stencil::vCombine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  i8 iLineNumber = 1;

  vAppendFuncHeader0();
  vAppendFuncHeader1();
  vAppendFuncHeader2();

  Fragments::vAppendConcludingFunc0(m_pConcludingFun0->azCode());
  Fragments::vAppendConcludingFunc1(m_pConcludingFun1->azCode());
  Fragments::vAppendConcludingFunc2(m_pConcludingFun2->azCode());

  vAppendFuncEmpty(m_pEmptyFun0, "CRWBUFFER"_Stz, "iFun0"_Stz);
  vAppendFuncEmpty(m_pEmptyFun1, "ROWBUFFER"_Stz, "iFun1"_Stz);
  vAppendFuncEmpty(m_pEmptyFun2, "IDXBUFFER"_Stz, "iFun2"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append banner head
  m_pOpSource->vAppendFragment(m_pBannerHead, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary
  m_pOpSource->vAppendFragment(m_pPreliminary, &iLineNumber);

  if (m_xStageOps.m_bScan || m_bRowBuffer) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    m_pOpSource->vAppendFragment(m_pPreliminaryFun0, &iLineNumber);

    if (m_pShapeCRW->pStructSynthA())
      m_pOpSource->vAppendFragment(m_pShapeCRW->pStructSynthA(), &iLineNumber);

    if (m_pShapeCRW->pStructSynthB())
      m_pOpSource->vAppendFragment(m_pShapeCRW->pStructSynthB(), &iLineNumber);

    if (m_pShapeCRW->pCodeSynthA())
      m_pOpSource->vAppendFragment(m_pShapeCRW->pCodeSynthA(), &iLineNumber);

    if (m_pShapeCRW->pCodeSynthB())
      m_pOpSource->vAppendFragment(m_pShapeCRW->pCodeSynthB(), &iLineNumber);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // append shapes
    for (i8 i_Shape = 0; i_Shape < nItems(); i_Shape++) {
      Shape *pShape = pGet(i_Shape);

      if (pShape->pStructSynthA())
        m_pOpSource->vAppendFragment(pShape->pStructSynthA(), &iLineNumber);

      if (pShape->pStructSynthB())
        m_pOpSource->vAppendFragment(pShape->pStructSynthB(), &iLineNumber);

      if (pShape->pCodeSynthA())
        m_pOpSource->vAppendFragment(pShape->pCodeSynthA(), &iLineNumber);

      if (pShape->pCodeSynthB())
        m_pOpSource->vAppendFragment(pShape->pCodeSynthB(), &iLineNumber);

      if (pShape->pCodeSynthC())
        m_pOpSource->vAppendFragment(pShape->pCodeSynthC(), &iLineNumber);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    m_pOpSource->vAppendFragment(m_pConcludingFun0, &iLineNumber);

    if (m_bRowBuffer) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      m_pOpSource->vAppendFragment(m_pPreliminaryFun1, &iLineNumber);

      if (m_pShapeROW->pStructSynthA())
        m_pOpSource->vAppendFragment(m_pShapeROW->pStructSynthA(), &iLineNumber);

      if (m_pShapeROW->pStructSynthB())
        m_pOpSource->vAppendFragment(m_pShapeROW->pStructSynthB(), &iLineNumber);

      if (m_pShapeROW->pCodeSynthA())
        m_pOpSource->vAppendFragment(m_pShapeROW->pCodeSynthA(), &iLineNumber);

      if (m_pShapeROW->pCodeSynthB())
        m_pOpSource->vAppendFragment(m_pShapeROW->pCodeSynthB(), &iLineNumber);

      if (m_pShapeROW->pCodeSynthC())
        m_pOpSource->vAppendFragment(m_pShapeROW->pCodeSynthC(), &iLineNumber);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // append shapes
      for (i8 i_Shape = 0; i_Shape < nItems(); i_Shape++) {
        Shape *pShape = pGet(i_Shape);

        if (pShape->pStructSynthA())
          m_pOpSource->vAppendFragment(pShape->pStructSynthA(), &iLineNumber);

        if (pShape->pStructSynthB())
          m_pOpSource->vAppendFragment(pShape->pStructSynthB(), &iLineNumber);

        if (pShape->pCodeSynthA())
          m_pOpSource->vAppendFragment(pShape->pCodeSynthA(), &iLineNumber);

        if (pShape->pCodeSynthB())
          m_pOpSource->vAppendFragment(pShape->pCodeSynthB(), &iLineNumber);

        if (pShape->pCodeSynthC())
          m_pOpSource->vAppendFragment(pShape->pCodeSynthC(), &iLineNumber);
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      m_pOpSource->vAppendFragment(m_pConcludingFun1, &iLineNumber);
    }
    else {
      m_pOpSource->vAppendFragment(m_pEmptyFun1, &iLineNumber);
    }
    m_pOpSource->vAppendFragment(m_pEmptyFun2, &iLineNumber);
  }

  else if (m_xStageOps.m_bIndexScan) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    m_pOpSource->vAppendFragment(m_pPreliminaryFun2, &iLineNumber);

    if (m_pShapeIDX->pStructSynthA())
      m_pOpSource->vAppendFragment(m_pShapeIDX->pStructSynthA(), &iLineNumber);

    if (m_pShapeIDX->pStructSynthB())
      m_pOpSource->vAppendFragment(m_pShapeIDX->pStructSynthB(), &iLineNumber);

    if (m_pShapeIDX->pCodeSynthA())
      m_pOpSource->vAppendFragment(m_pShapeIDX->pCodeSynthA(), &iLineNumber);

    if (m_pShapeIDX->pCodeSynthB())
      m_pOpSource->vAppendFragment(m_pShapeIDX->pCodeSynthB(), &iLineNumber);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // append shapes
    for (i8 i_Shape = 0; i_Shape < nItems(); i_Shape++) {
      Shape *pShape = pGet(i_Shape);

      if (pShape->pStructSynthA())
        m_pOpSource->vAppendFragment(pShape->pStructSynthA(), &iLineNumber);

      if (pShape->pStructSynthB())
        m_pOpSource->vAppendFragment(pShape->pStructSynthB(), &iLineNumber);

      if (pShape->pCodeSynthA())
        m_pOpSource->vAppendFragment(pShape->pCodeSynthA(), &iLineNumber);

      if (pShape->pCodeSynthB())
        m_pOpSource->vAppendFragment(pShape->pCodeSynthB(), &iLineNumber);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    m_pOpSource->vAppendFragment(m_pConcludingFun2, &iLineNumber);

    m_pOpSource->vAppendFragment(m_pEmptyFun0, &iLineNumber);
    m_pOpSource->vAppendFragment(m_pEmptyFun1, &iLineNumber);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append concluding
  m_pOpSource->vAppendFragment(m_pConcluding, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append eos shape

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary
  m_pOpSource->vAppendFragment(m_pPreliminaryEos, &iLineNumber);

  if (m_pShapeEOS->pStructSynthA())
    m_pOpSource->vAppendFragment(m_pShapeEOS->pStructSynthA(), &iLineNumber);

  if (m_pShapeEOS->pStructSynthB())
    m_pOpSource->vAppendFragment(m_pShapeEOS->pStructSynthB(), &iLineNumber);

  if (m_pShapeEOS->pCodeSynthA())
    m_pOpSource->vAppendFragment(m_pShapeEOS->pCodeSynthA(), &iLineNumber);

  if (m_pShapeEOS->pCodeSynthB())
    m_pOpSource->vAppendFragment(m_pShapeEOS->pCodeSynthB(), &iLineNumber);

  if (m_pShapeEOS->pCodeSynthC())
    m_pOpSource->vAppendFragment(m_pShapeEOS->pCodeSynthC(), &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary
  m_pOpSource->vAppendFragment(m_pConcludingEos, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append banner tail
  m_pOpSource->vAppendFragment(m_pBannerTail, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // write to a file

  s256 sFileName;
  s256 funcName;
  LocalFile oFile;

  i8 iQueryNo = AM::iQueryNoInc();

  if (gxCmdLineArgs.m_xSysArgs.m_bCombined) {
    sFileName.vSprint("../combined/FlatQuery.hpp"_Stz, iQueryNo,
                      AM::pGrid()->pThisNode()->szNodeNm());

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // write to a .h file
    bool bResult = oFile.bOpenAppend(sFileName);
    oFile.vPrint((Stz*)m_pOpHeader->azCode());
    oFile.vClose();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // write to a .c file

    sFileName.vSprint("../combined/FlatQuery%lld_%s.c"_Stz, iQueryNo,
                      AM::pGrid()->pThisNode()->szNodeNm());
    bResult = oFile.bOpenWrite(sFileName);
    oFile.vPrint((Stz*)m_pOpSource->azCode());
    oFile.vClose();

  }
  else {
#ifdef WITH_DEBUG_VERSION

    sFileName.vSprint("../tmp/FlatQuery.hpp"_Stz, iQueryNo,
                      AM::pGrid()->pThisNode()->szNodeNm());

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // write to a .c file

    sFileName.vSprint("../tmp/FlatQuery%lld_%s.c"_Stz, iQueryNo,
                      AM::pGrid()->pThisNode()->szNodeNm());
    bool bResult = oFile.bOpenWrite(sFileName);
    oFile.vPrint((Stz*)m_pOpSource->azCode());
    oFile.vClose();

#endif
  }

}

/*----------------------------------------------------------------------------*/
/* return op source code                                                      */
/*----------------------------------------------------------------------------*/
Stz* Stencil::szOpSource(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);
  
  char* azCode = m_pOpSource->azCode();
  
  return (Stz*)azCode;
}

/*----------------------------------------------------------------------------*/
/* get stencil pattern                                                        */
/*----------------------------------------------------------------------------*/
eLP Stencil::ePattern(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  return m_ePattern;
}

/*----------------------------------------------------------------------------*/
/* set stencil pattern                                                        */
/*----------------------------------------------------------------------------*/
void Stencil::vPattern(eLP ePattern)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  m_ePattern = ePattern;
}

/*----------------------------------------------------------------------------*/
/* print the synthesized code                                                 */
/*----------------------------------------------------------------------------*/
void Stencil::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  //vTrace("Synthesized Code:\n %s\n"_Stz,m_pOpSource->azCode());
}

/*----------------------------------------------------------------------------*/
/* sketch source from pattern                                                 */
/*----------------------------------------------------------------------------*/
eEC Stencil::eSketch(Combined* pCombined, Stream* pStream)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  CodeSynth *pCodeSynth;

  s4K sLabel;
  s4K sReturnLabel;
  s4K sLabelLoop;
  s4K sLabelNext;
  s4K sLabelOrdAgg;

  bool bInflateGenerated = false;

  Shape* pShape;
  IntStack *pPreExpStack = nullptr;

  eSR ePrevStencil = SR_NONE;
  eEK eExprKind = EK_NONE;

  m_szStageNm = pStream->pStage()->szStageNm();

  i8 iStage = pStream->pStage()->i_Stage();

  Tub* pTub = _pTub(pCombined->aCombinedT()->m_pTub);
  Pot* pPot = _pPot(pCombined->aCombinedT()->m_pPot);

  sReturnLabel = ""_Stz;
  sLabelLoop = "L_AdvanceNextRow"_Stz;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect the op information from the stage
  eEC eError = eCollectStage(pCombined, pStream);
  if (eError != EC_NONE)
    return eError;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each operator in the stream (skip consumeOP)
  for (i8 i_Op = 1; i_Op < pStream->nItems(); i_Op++) {

    // get the op
    Op *pOp = pStream->pGet(i_Op);

    // break when we reach the combined OP
    if (pOp->szOpNm()->bSame("combined"_Stz)) {
      break;
    }

    // skip scan OP
    if (pOp->szOpNm()->bSame("scan"_Stz))
      continue;

    // skip indexscan OP
    if (pOp->szOpNm()->bSame("indexScan"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupIndexScan(pCombined, pStream, pOp);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      m_pShapeIDX = pSketchIndexScan(pCombined, pStream, pOp);

      bInflateGenerated = true;
      ePrevStencil = SR_IT;
      continue;
    }

    // get the tags
    TagSet* pOpTags = pOp->pOpTags();

    // get the streamDefOut and streamDefLft
    LogicUnit *pLogicUnit = pOp->pOpLogicUnit();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for inflate (driving code) if not already inflated
    if ( ! bInflateGenerated ) {

      RASSERT( ! m_xStageOps.m_bIndexScan );

      if (m_xStageOps.m_bScan) {

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        vSetupScan(pCombined, pStream, pOp);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        m_pShapeCRW = pSketchInflateCrw(pCombined, pStream, pOp);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        m_pShapeROW = pSketchInflateRow(pStream, pOp);

        m_bRowBuffer = true;
      }
      else {

          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          vSetupScan(pCombined, pStream, pOp);

          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          m_pShapeCRW = pSketchInflateCrw(pCombined, pStream, pOp);

          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          m_pShapeROW = pSketchInflateRow(pStream, pOp);

          m_bRowBuffer = true;
      }
      bInflateGenerated = true;
      ePrevStencil = SR_IT;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the aggregOp
    if (pOp->szOpNm()->bSame("aggreg"_Stz)) {

      // get the project kind
      eExprKind = pLogicUnit->m_eExprKind;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupAggreg(pCombined, pStream, pOp);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pShape = pSketchProjectGby(pCombined, pStream, pOp, ePrevStencil, sLabelLoop);
      vAppend(pShape);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupEosPayload(pCombined, pStream, pOp);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSketchEosProjectGby(pCombined, pStream, pOp);
      m_xStageOps.m_bGeneratePotTub = false;

      ePrevStencil = SR_AT;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the filterOP
    if (pOp->szOpNm()->bSame("filter"_Stz)) {

      Op *pPrevOp = pStream->pGet(i_Op - 1);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if ( ! pPrevOp->szOpNm()->bSame("aggreg"_Stz) ) {
        pShape = pSketchFilter(pStream, pOp, ePrevStencil, sLabelLoop);
        vAppend(pShape);
      }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the hashOP
    if (pOp->szOpNm()->bSame("hash"_Stz)) {

      if (m_xStageOps.m_bPayloadOp)
        continue;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pShape = pSketchLabel(pCombined, pStream, pOp);
      vAppend(pShape);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the indexJoinOp
    if (pOp->szOpNm()->bSame("indexJoin"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupIndexJoin(pCombined, pStream, pOp);

      sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pShape = pSketchIndexJoin(pCombined, pStream, pOp, ePrevStencil, sLabel);
      vAppend(pShape);

      sLabelLoop = "L_AdvanceNextRowJ"_Stz;
      ePrevStencil = SR_TJ;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the keyExistsOP
    if (pOp->szOpNm()->bSame("keyExists"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupKeyExists(pCombined, pStream, pOp);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pShape = pSketchKeyExists(pCombined, pStream, pOp, ePrevStencil);
      vAppend(pShape);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the limitOP
    if (pOp->szOpNm()->bSame("limit"_Stz)) {

      pCombined->aCombinedT()->m_iLimitRows = 1;

      // limit after a payload will move to EOSHandler
      if (m_xStageOps.m_bPayloadOp) {
        continue;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pShape = pSketchLimit(pCombined, pStream, pOp, ePrevStencil, sLabelLoop);
      vAppend(pShape);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the orderByOp
    if (pOp->szOpNm()->bSame("sort"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupSort(pCombined, pStream, pOp);

      if (m_xStageOps.m_pPostProjectOp) {

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        pShape = pSketchSort(pCombined, pStream, pOp, ePrevStencil, sLabelLoop);
        vAppend(pShape);

      }
      else {

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        pShape = pSketchSort(pCombined, pStream, pOp, ePrevStencil, sLabelLoop);
        vAppend(pShape);
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupEosPayload(pCombined, pStream, pOp);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSketchEosSort(pCombined, pStream, pOp);
      m_xStageOps.m_bGeneratePotTub = false;

      ePrevStencil = SR_AT;

    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the projectOP
    if (pOp->szOpNm()->bSame("project"_Stz)) {

      Op *pPrevOp = pStream->pGet(i_Op - 1);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // project after sort will move to the EOS marker
      if ( ! m_xStageOps.m_pPostProjectOp ) {

        // get the aggregate type stack
        pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

        // get the project kind
        eExprKind = pLogicUnit->m_eExprKind;

        if (eExprKind == EK_PROJECT) {

          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          vSetupProjectExp(pCombined, pStream, pOp);

          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          pShape = pSketchPrjExp(pCombined, pStream, pOp, ePrevStencil, sLabelLoop);
          vAppend(pShape);

          ePrevStencil = SR_PT;
        } else {

          RASSERT(eExprKind == EK_AGGREG);

          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          vSetupProjectAgg(pCombined, pStream, pOp);

          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          pShape = pSketchPrjAgg(pCombined, pStream, pOp, ePrevStencil, sLabelLoop);
          vAppend(pShape);

          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          vSetupEosPayload(pCombined, pStream, pOp);

          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          vSketchEosProjectAgg(pCombined, pStream, pOp);
          m_xStageOps.m_bGeneratePotTub = false;

          ePrevStencil = SR_AT;
        }
      }

    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the ordAgg OP
    if (pOp->szOpNm()->bSame("ordAgg"_Stz)) {

      // get the project kind
      eExprKind = pLogicUnit->m_eExprKind;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupOrdAgg(pCombined, pStream, pOp);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pShape = pSketchOrdAgg(pCombined, pStream, pOp, ePrevStencil, sLabelLoop);
      vAppend(pShape);

      sLabelOrdAgg.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), pOp->i_Op());
      sReturnLabel = "L_ReturnFinal"_Stz;
      ePrevStencil = SR_AT;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the topOP
    if (pOp->szOpNm()->bSame("top"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupTop(pCombined, pStream, pOp);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pShape = pSketchTop(pCombined, pStream, pOp, ePrevStencil, sLabelLoop);
      vAppend(pShape);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupEosPayload(pCombined, pStream, pOp);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSketchEosTop(pCombined, pStream, pOp);
      m_xStageOps.m_bGeneratePotTub = false;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // synthesize code for allotting final tub row/ pot row
  Op* pLastOp = pStream->pGet(m_xStageOps.m_pCombinedOp->i_Op() - 1);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // synthesize code for allotting tub row

  LogicUnit *pLogicUnit = pLastOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  RASSERT(pStreamDefOut);

  // get the row
  RowPix* pRowPix = pStreamDefOut->pRowPix();

  if (pCombined->aCombinedT()->m_iHashRows) {
    pPot->vRowPix(pRowPix);
  } else {
    pTub->vRowPix(pRowPix);
    pCombined->aCombinedT()->m_iTubRowSize = Row::nRowProper(pTub->pRowPix()->nColumns());
  }

  if (m_xStageOps.m_bGeneratePotTub) {

    sLabel.vSprint("L_AddRowToTubPot"_Stz);

    if (pCombined->aCombinedT()->m_iHashRows)
      pShape = pSketchPot(pCombined, pLastOp, ePrevStencil, pPreExpStack, sLabel, sLabelLoop, sLabelOrdAgg);
    else
      pShape = pSketchTub(pCombined, pLastOp, ePrevStencil, pPreExpStack, sLabel, sLabelLoop, sLabelOrdAgg);

    // add the shape to the stencil
    vAppend(pShape);
  }
  else {
    pShape = pSketchReturn();

    // add the shape to the stencil
    vAppend(pShape);
  }

  // generate empty func3 if we have already generated pot/tub
  if ( m_xStageOps.m_bGeneratePotTub ) {

    pCodeSynth = new CodeSynth;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // append code for EOSMARKER handling
    vAppendFuncEmpty(pCodeSynth, "EOSMARKER"_Stz, "iFun3"_Stz);

    m_pShapeEOS->vCodeSynthA(pCodeSynth);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form entries in header
  vAppendHeaderEntries(iStage);

  //RASSERT( !pAggrStack );

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* setup the combinedOp                                                       */
/*----------------------------------------------------------------------------*/
void Stencil::vSetup(Combined* pCombined, Stream* pStream)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  bool bInflateGenerated = false;
  eEK  eExprKind = EK_NONE;

  Tub* pTub = _pTub(pCombined->aCombinedT()->m_pTub);
  Pot* pPot = _pPot(pCombined->aCombinedT()->m_pPot);

  // collect the op information from the stage
  eEC eError = eCollectStage(pCombined, pStream);
  RASSERT(eError == EC_NONE);

  // for each operator in the stream
  for (i8 i_Op = 0; i_Op < pStream->nItems(); i_Op++) {

    // get the op
    Op *pOp = pStream->pGet(i_Op);

    // skip consumeOP
    if (pOp->szOpNm()->bSame("consumeOp"_Stz))
      continue;

    // skip scan OP
    if (pOp->szOpNm()->bSame("scan"_Stz)) {
      continue;
    }

    // break when we reach the combined OP
    if (pOp->szOpNm()->bSame("combined"_Stz)) {
      break;
    }

    // get the streamDefOut
    LogicUnit *pLogicUnit = pOp->pOpLogicUnit();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for inflate (for first OP)
    if ( m_xStageOps.m_bScan && ! bInflateGenerated && ! m_xStageOps.m_bIndexScan) {
      vSetupScan(pCombined, pStream, pOp);
      bInflateGenerated = true;
    }

    if ( ! m_xStageOps.m_bScan && ! m_bRowBuffer && ! m_xStageOps.m_bIndexScan) {
      // row buffer processing
      m_bRowBuffer = true;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the filterOP
    if (pOp->szOpNm()->bSame("filter"_Stz)) {
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the aggregOp / projectOP
    if (pOp->szOpNm()->bSame("aggreg"_Stz)) {

      vSetupAggreg(pCombined, pStream, pOp);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupEosPayload(pCombined, pStream, pOp);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the aggregOp / projectOP
    if (pOp->szOpNm()->bSame("project"_Stz)) {

      // get the project kind
      eExprKind = pLogicUnit->m_eExprKind;

      if (eExprKind == EK_PROJECT) {
        vSetupProjectExp(pCombined, pStream, pOp);
      }
      else {
        RASSERT(eExprKind == EK_AGGREG);
        vSetupProjectAgg(pCombined, pStream, pOp);
      }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the ordAgg
    if (pOp->szOpNm()->bSame("ordAgg"_Stz)) {
      vSetupOrdAgg(pCombined, pStream, pOp);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the indexJoinOp
    if (pOp->szOpNm()->bSame("indexJoin"_Stz)) {
      vSetupIndexJoin(pCombined, pStream, pOp);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the indexScanOp
    if (pOp->szOpNm()->bSame("indexScan"_Stz)) {

      // there should be no RowBuffer or scan
      RASSERT( !m_xStageOps.m_bScan );
      RASSERT( !bInflateGenerated );
      RASSERT( !m_bRowBuffer );

      RASSERT(m_xStageOps.m_bIndexScan);

      // indexScan should be the second op after consume
      RASSERT(i_Op == 1);

      vSetupIndexScan(pCombined, pStream, pOp);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the keyExistsOp
    if (pOp->szOpNm()->bSame("keyExists"_Stz)) {
      vSetupKeyExists(pCombined, pStream, pOp);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the sortOP
    if (pOp->szOpNm()->bSame("sort"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupSort(pCombined, pStream, pOp);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupEosPayload(pCombined, pStream, pOp);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the topOP
    if (pOp->szOpNm()->bSame("top"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupTop(pCombined, pStream, pOp);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupEosPayload(pCombined, pStream, pOp);
    }

  }

  // get the lastOp
  Op* pLastOp = pStream->pGet(m_xStageOps.m_pCombinedOp->i_Op() - 1);
  LogicUnit *pLogicUnit = pLastOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  RASSERT(pStreamDefOut);

  // get the row
  RowPix* pRowPix = pStreamDefOut->pRowPix();
  if (pCombined->aCombinedT()->m_iHashRows) {
    pPot->vRowPix(pRowPix);
  }
  else {
    pTub->vRowPix(pRowPix);
    pCombined->aCombinedT()->m_iTubRowSize = Row::nRowProper(pTub->pRowPix()->nColumns());
  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the next label for Op                                                  */
/*----------------------------------------------------------------------------*/
void Stencil::vGetNextLabel(Stz* sLabelNext, Stream* pStream, i8 i_Op, Stz* sDefault) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  i8 iNextOp = i_Op + 1;
  Op *pOpNext = pStream->pGet(iNextOp);

  if (pOpNext->szOpNm()->bSame("combined"_Stz) || pOpNext->szOpNm()->bSame("hash"_Stz)) {
    // if nextOp is combined/hash, then next label is sDefault
    sLabelNext->vCopy(sDefault);
  }
  else {
    // form next label using next op
    sLabelNext->vSprint("L_%s_%d"_Stz, pOpNext->szOpNm(), iNextOp);
  }
}
/*----------------------------------------------------------------------------*/
/* get select columns                                                         */
/*----------------------------------------------------------------------------*/
u8 Stencil::uGetSelectColumns(Stream* pStream) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  bool bProject = false;

  u8 uSelectColumns = 0;

  // for each operator in the stream
  for (i8 i_Op = 0; i_Op < pStream->nItems(); i_Op++) {

    // get the op
    Op *pOp = pStream->pGet(i_Op);

    if (pOp->szOpNm()->bSame("indexJoin"_Stz)) {
      LogicUnit* pLogicUnit = pOp->pOpLogicUnit();
//      for (i8 i=0; i<64; i++) {
//        if (BitLib::bIsOn(pLogicUnit->m_uSelectColumns, i))
//          vTrace("OP: %s ; Column %lld\n"_Stz, pOp->szOpNm(), i);
//      }
      uSelectColumns |= pLogicUnit->m_uSelectColumns;
      continue;
    }

    if (pOp->szOpNm()->bSame("aggreg"_Stz) ||
        pOp->szOpNm()->bSame("project"_Stz) ||
        pOp->szOpNm()->bSame("ordagg"_Stz)) {
      LogicUnit* pLogicUnit = pOp->pOpLogicUnit();
      uSelectColumns |= pLogicUnit->m_uSelectColumns;
      bProject = true;
      break;
    }

    LogicUnit* pLogicUnit = pOp->pOpLogicUnit();
    uSelectColumns |= pLogicUnit->m_uSelectColumns;

    // break when we reach the combined OP
    if (pOp->szOpNm()->bSame("combined"_Stz)) {
      break;
    }

  }

  // if no projection, select all columns
  if (!bProject)
    uSelectColumns = 0xFFFFFFFFFFFFFFFFLL;

  //uSelectColumns = 0xFFFFFFFFFFFFFFFFLL;
  return uSelectColumns;
}

/*----------------------------------------------------------------------------*/
/* set up for indexJoin                                                       */
/*----------------------------------------------------------------------------*/
void Stencil::vSetupAggreg(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  // create the groupby index
  eKE eKind;
  bool bPayload = true;

  // get the indexDef
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  IndexDef *pIndexDef = _pIndexDef(*pLogicUnit->m_xpIndexDef);
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  pCombined->aCombinedT()->m_aIndexDefPL = new IndexDef(pIndexDef);
  pCombined->aCombinedT()->m_aIndexTreePL = IndexTree::aNewIndexTree(pIndexDef, &eKind);

  // do we have a payload ?
  if (pStreamDefOut->nColumns() == pIndexDef->nColumns())
    bPayload = false;

  EntWrk::vEntKind((EntWrk *) pCombined->aCombinedT()->m_aEntWrkPL, bPayload, pIndexDef->eKeyKind());

  // delete the indexdef in the LU of OP
  // not the correct place... will come back to it
  delete *pLogicUnit->m_xpIndexDef;
  pLogicUnit->m_xpIndexDef = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set up for EOS Payload                                                     */
/*----------------------------------------------------------------------------*/
void Stencil::vSetupEosPayload(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // create RowSingleAgg
  pCombined->aCombinedT()->m_aPayloadRow = new RowSingle(pStreamDefOut);
  pCombined->aCombinedT()->m_aPayloadCellArray =
      (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_aPayloadRow,
                                      &pCombined->aCombinedT()->m_racPayloadArea);
  pCombined->aCombinedT()->m_racPayloadAreaSav = pCombined->aCombinedT()->m_racPayloadArea;

}

/*----------------------------------------------------------------------------*/
/* set up for indexJoin                                                       */
/* set up for indexJoin                                                       */
/*----------------------------------------------------------------------------*/
void Stencil::vSetupIndexJoin(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // get the joinIndex from the indexJoinOp
  Index* pIndex = _pIndex(*pLogicUnit->m_xpIndex);
  pCombined->aCombinedT()->m_aIndexJ = pIndex;

  // get the index tree for the index
  Table* pTable = pIndex->pTable();

  LglPtn tLglPtn;

  StreamDef* pStreamDef = pTable->pStreamDef();
  if (pStreamDef->bReplicated()) {

    Stage*  pStage   = pOp->pStage();
    Stream* pStream0 = pStage->pGet(0);
    Op*     pOp0_0   = pStream0->pGet(0);

    tLglPtn = pOp0_0->tLglPtn();
  }
  else {
    tLglPtn = pOp->tLglPtn();
  }

  i8 i_TablePtn = pTable->iPtnNoToTablePtn(tLglPtn);
  TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);

  i8 iIndexNo = pIndex->iIndexNo();

  IndexTree* pIndexTree = pTablePtn->pIndexTree(iIndexNo);
  pCombined->aCombinedT()->m_aIndexTreeJ = pIndexTree;
  pCombined->aCombinedT()->m_aIndexDefJ  = pIndex->pIndexDef();

  // create join RowSingleTJ
  pCombined->aCombinedT()->m_aRowSingleTJ = new RowSingle(pStreamDefOut);
  pCombined->aCombinedT()->m_aRowCellArrayTJ =
      (byte *) RowSingle::aGetCellArray((RowSingle *) pCombined->aCombinedT()->m_aRowSingleTJ,
                                        &pCombined->aCombinedT()->m_racStrAreaTJ);
  pCombined->aCombinedT()->m_racStrAreaTJSav = pCombined->aCombinedT()->m_racStrAreaTJ;
}

/*----------------------------------------------------------------------------*/
/* set up for indexScan                                                       */
/*----------------------------------------------------------------------------*/
void Stencil::vSetupIndexScan(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();

  // get streamDefLft
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // get the scanIndex from the indexScanOp
  Index* pIndexScan = _pIndex(*pLogicUnit->m_xpIndex);
  pCombined->aCombinedT()->m_aIndexT = pIndexScan;

  // create RowSingleT
  pCombined->aCombinedT()->m_pRowSingleT = new RowSingle(pStreamDefLft);
  pCombined->aCombinedT()->m_aRowCellArrayT =
  (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_pRowSingleT,
  &pCombined->aCombinedT()->m_racStrAreaT);
  pCombined->aCombinedT()->m_racStrAreaTSav = pCombined->aCombinedT()->m_racStrAreaT;

  // get the index tree for the index
  Table* pTable = pIndexScan->pTable();

  LglPtn tLglPtn;

  StreamDef* pStreamDef = pTable->pStreamDef();
  if (pStreamDef->bReplicated()) {

    Stage*  pStage   = pOp->pStage();
    Stream* pStream0 = pStage->pGet(0);
    Op*     pOp0_0   = pStream0->pGet(0);

    tLglPtn = pOp0_0->tLglPtn();
  }
  else {
    tLglPtn = pOp->tLglPtn();
  }

  i8 i_TablePtn = pTable->iPtnNoToTablePtn(tLglPtn);
  TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);

  pCombined->aCombinedT()->m_nCols = pTable->pStreamDef()->nColumns();

  i8 iIndexNo = pIndexScan->iIndexNo();

  // COME BACK TO THIS
  IndexTree* pIndexTree = pTablePtn->pIndexTree(iIndexNo);
  IndexDef* pIndexDef = pIndexTree->pIndexDef();
  pCombined->aCombinedT()->m_aIndexTreeT = pIndexTree;
  pCombined->aCombinedT()->m_aIndexDefT  = pIndexDef;
  EntWrk::vEntKind((EntWrk*)pCombined->aCombinedT()->m_aEntWrkT, 0, pIndexDef->eKeyKind());
}

/*----------------------------------------------------------------------------*/
/* set up for key exists                                                      */
/*----------------------------------------------------------------------------*/
void Stencil::vSetupKeyExists(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefRht = _pStreamDef(*pLogicUnit->m_xpStreamDefRht);

  // create RowSingleJ
  pCombined->aCombinedT()->m_aRowSingleJ = new RowSingle(pStreamDefRht);
  pCombined->aCombinedT()->m_aRowCellArrayJ =
      (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_aRowSingleJ,
                                      &pCombined->aCombinedT()->m_racStrAreaJ);
  pCombined->aCombinedT()->m_racStrAreaJSav = pCombined->aCombinedT()->m_racStrAreaJ;

  // get the joinIndex from the indexJoinOp
  Index* pIndex = _pIndex(*pLogicUnit->m_xpIndex);
  pCombined->aCombinedT()->m_aIndexJ = pIndex;

  // get the index tree for the index
  Table* pTable = pIndex->pTable();

  LglPtn tLglPtn;
  StreamDef* pStreamDef = pTable->pStreamDef();
  if (pStreamDef->bReplicated()) {

    Stage*  pStage   = pOp->pStage();
    Stream* pStream0 = pStage->pGet(0);
    Op*     pOp0_0   = pStream0->pGet(0);

    tLglPtn = pOp0_0->tLglPtn();
  }
  else {
    tLglPtn = pOp->tLglPtn();
  }

  i8 i_TablePtn = pTable->iPtnNoToTablePtn(tLglPtn);
  TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);

  i8 iIndexNo = pIndex->iIndexNo();

  IndexTree* pIndexTree = pTablePtn->pIndexTree(iIndexNo);
  pCombined->aCombinedT()->m_aIndexTreeJ = pIndexTree;
  pCombined->aCombinedT()->m_aIndexDefJ  = pIndex->pIndexDef();
}

/*----------------------------------------------------------------------------*/
/* set up for ordered aggregate                                               */
/*----------------------------------------------------------------------------*/
void Stencil::vSetupOrdAgg(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  bool bPayload = true;

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // create RowSingleAgg
  pCombined->aCombinedT()->m_aRowSingleAgg = new RowSingle(pStreamDefOut);
  pCombined->aCombinedT()->m_aRowCellArrayAgg =
      (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_aRowSingleAgg,
                                      &pCombined->aCombinedT()->m_racStrAreaAgg);

  // get the indexDef
  IndexDef *pIndexDef = _pIndexDef(*pLogicUnit->m_xpIndexDef);

  // do we have a payload ?
  if (pStreamDefOut->nColumns() == pIndexDef->nColumns())
    bPayload = false;

  // create the groupby index
  eKE eKind;
  pCombined->aCombinedT()->m_aIndexDefPL = new IndexDef(pIndexDef);
  pCombined->aCombinedT()->m_aIndexTreePL = IndexTree::aNewIndexTree(pIndexDef, &eKind);
  EntWrk::vEntKind((EntWrk*)pCombined->aCombinedT()->m_aEntWrkPL, bPayload, pIndexDef->eKeyKind());

  // delete the indexdef in the LU of OP
  // not the correct place... will come back to it
  delete *pLogicUnit->m_xpIndexDef;
  pLogicUnit->m_xpIndexDef = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set up for project aggregate                                               */
/*----------------------------------------------------------------------------*/
void Stencil::vSetupProjectAgg(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // create RowSingleAgg
  pCombined->aCombinedT()->m_aRowSingleAgg = new RowSingle(pStreamDefOut);
  pCombined->aCombinedT()->m_aRowCellArrayAgg =
      (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_aRowSingleAgg,
                                      &pCombined->aCombinedT()->m_racStrAreaAgg);
  pCombined->aCombinedT()->m_racStrAreaSavAgg = pCombined->aCombinedT()->m_racStrAreaAgg;
}

/*----------------------------------------------------------------------------*/
/* set up for project expression                                              */
/*----------------------------------------------------------------------------*/
void Stencil::vSetupProjectExp(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // create RowSingleP
  pCombined->aCombinedT()->m_aRowSingleP = new RowSingle(pStreamDefOut);
  pCombined->aCombinedT()->m_aRowCellArrayP =
      (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_aRowSingleP,
                                      &pCombined->aCombinedT()->m_racStrAreaP);
  pCombined->aCombinedT()->m_racStrAreaSavP = pCombined->aCombinedT()->m_racStrAreaP;

}

/*----------------------------------------------------------------------------*/
/* set up for eos aggregate                                                   */
/*----------------------------------------------------------------------------*/
void Stencil::vSetupEosAgg(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // create RowSingleAgg
  pCombined->aCombinedT()->m_aPayloadRow = new RowSingle(pStreamDefOut);
  pCombined->aCombinedT()->m_aPayloadCellArray =
      (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_aPayloadRow,
                                      &pCombined->aCombinedT()->m_racPayloadArea);
  pCombined->aCombinedT()->m_racPayloadAreaSav = pCombined->aCombinedT()->m_racPayloadArea;
}

/*----------------------------------------------------------------------------*/
/* set up for sort                                                            */
/*----------------------------------------------------------------------------*/
void Stencil::vSetupSort(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  eKE eKind;
  bool bPayload = true;

  // get the indexDef
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  IndexDef *pIndexDef = _pIndexDef(*pLogicUnit->m_xpIndexDef);
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // do we have a payload ?
  if (pStreamDefOut->nColumns() == pIndexDef->nColumns())
    bPayload = false;

  pCombined->aCombinedT()->m_aIndexDefPL = new IndexDef(pIndexDef);
  pCombined->aCombinedT()->m_aIndexTreePL = IndexTree::aNewIndexTree(pIndexDef, &eKind);
  EntWrk::vEntKind((EntWrk *) pCombined->aCombinedT()->m_aEntWrkPL, bPayload, pIndexDef->eKeyKind());

  // create RowSingleP
  if (m_xStageOps.m_pPostProjectOp) {
    pCombined->aCombinedT()->m_aRowSingleP = new RowSingle(pStreamDefOut);
    pCombined->aCombinedT()->m_aRowCellArrayP =
        (byte *) RowSingle::aGetCellArray((RowSingle *) pCombined->aCombinedT()->m_aRowSingleP,
                                          &pCombined->aCombinedT()->m_racStrAreaP);
    pCombined->aCombinedT()->m_racStrAreaSavP = pCombined->aCombinedT()->m_racStrAreaP;
  }

  // delete the indexdef in the LU of OP
  // not the correct place... will come back to it
  delete *pLogicUnit->m_xpIndexDef;
  pLogicUnit->m_xpIndexDef = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set up for scan                                                            */
/*----------------------------------------------------------------------------*/
void Stencil::vSetupScan(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  // get the streamDefLft
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  pCombined->aCombinedT()->m_nCols = pStreamDefLft->nColumns();
  pCombined->aCombinedT()->m_pRowSingleT = new RowSingle(pStreamDefLft);
  pCombined->aCombinedT()->m_aRowCellArrayT =
      (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_pRowSingleT,
                                      &pCombined->aCombinedT()->m_racStrAreaT);
  pCombined->aCombinedT()->m_racStrAreaTSav = pCombined->aCombinedT()->m_racStrAreaT;

}

/*----------------------------------------------------------------------------*/
/* set up for top                                                             */
/*----------------------------------------------------------------------------*/
void Stencil::vSetupTop(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  bool bPayload = true;

  // get the LogicUnit
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();

  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // get the IndexDef from the TopOp
  IndexDef* pIndexDef = _pIndexDef(*pLogicUnit->m_xpIndexDef);
  pCombined->aCombinedT()->m_aIndexDefPL = new IndexDef(pIndexDef);

  // do we have a payload ?
  if (pStreamDefOut->nColumns() == pIndexDef->nColumns())
    bPayload = false;

  // create the indexTree
  eKE eKind;
  pCombined->aCombinedT()->m_aIndexTreePL = IndexTree::aNewIndexTree(pIndexDef, &eKind);
  EntWrk::vEntKind((EntWrk *) pCombined->aCombinedT()->m_aEntWrkPL, bPayload, pIndexDef->eKeyKind());

  // delete the indexdef in the LU of OP
  // not the correct place... will come back to it
  delete *pLogicUnit->m_xpIndexDef;
  pLogicUnit->m_xpIndexDef = nullptr;
}

/*----------------------------------------------------------------------------*/
/* sketch code for Eos fun3 for filter OP (having)                            */
/*----------------------------------------------------------------------------*/
void Stencil::vSketchEosFilter(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  CodeSynth *pCodeSynth;
  s256 szStageNm;
  s256 sLabel;


  i8 i_Op = pOp->i_Op();

  // get filter expr from OP
  TagSet* pOpTags = pOp->pOpTags();
  Stz* sFilterExpr = pOpTags->szValue(C_TAG_EXPR);

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the OP
  s256 sLabelPrev;
  s256 sLabelNext;
  pCodeSynth = new CodeSynth(pStreamDefLft, pStreamDefLft, nullptr, pPreExpStack, SR_PL, SR_NONE);
  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStream, i_Op, "L_AdvanceNextRow"_Stz);

  sLabelPrev = ""_Stz;
  pCodeSynth->vSynthFilter(sFilterExpr, sLabel, sLabelPrev, sLabelNext);

  m_pShapeEOS->vCodeSynthB(pCodeSynth);
}

/*----------------------------------------------------------------------------*/
/* sketch code for Eos fun3 for Project agg OP                                */
/*----------------------------------------------------------------------------*/
void Stencil::vSketchEosProjectAgg(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  CodeSynth *pCodeSynth;
  s256 szStageNm;
  s256 sLabel;
  s256 sLabelNext;

  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append function header
  vAppendFuncHeader3();

  pCodeSynth = new CodeSynth(pStreamDefOut, SR_AT);
  pCodeSynth->vSynthStruct(0);
  m_pShapeEOS->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payload to buffer
  pCodeSynth = new CodeSynth(nullptr, pStreamDefOut, nullptr, pPreExpStack, SR_NONE, SR_PL);

  vGetNextLabel(sLabelNext, pStream, pOp->i_Op(), "L_AddRowToTubPot"_Stz);

  pCodeSynth->vSynthEos(nullptr, m_pPostStack,sLabelNext);
  m_pShapeEOS->vCodeSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payload to Tub/Pot
  pCodeSynth = new CodeSynth(pStreamDefLft, pStreamDefOut, nullptr, pPreExpStack, SR_AT, SR_NONE);

  Stz* sHavingExpr = ""_Stz;

  if (m_xStageOps.m_pHavingFilterOp) {
    // get filter expr from OP
    TagSet *pOpTags = m_xStageOps.m_pHavingFilterOp->pOpTags();
    sHavingExpr = pOpTags->szValue(C_TAG_EXPR);

    sLabel.vSprint("L_%s_%d"_Stz, m_xStageOps.m_pHavingFilterOp->szOpNm(), m_xStageOps.m_pHavingFilterOp->i_Op());
    vGetNextLabel(sLabelNext, pStream, m_xStageOps.m_pHavingFilterOp->i_Op(), "L_AdvanceNextRow"_Stz);
    pCodeSynth->vSynthFilter(sHavingExpr, sLabel, ""_Stz, sLabelNext);
    m_pShapeEOS->vCodeSynthB(pCodeSynth);
  }

  if (m_xStageOps.m_pLimitOp) {
    // get limit from limitOp
    TagSet* pOpTags = m_xStageOps.m_pLimitOp->pOpTags();
    i8 iLimit = StrLib::iToI8((char*)(pOpTags->szValue(C_TAG_COUNT)));

    sLabel.vSprint("L_%s_%d"_Stz, m_xStageOps.m_pLimitOp->szOpNm(), m_xStageOps.m_pLimitOp->i_Op());
    vGetNextLabel(sLabelNext, pStream, m_xStageOps.m_pLimitOp->i_Op(), "L_AddRowToTubPot"_Stz);
    pCodeSynth->vSynthLimit(iLimit, sLabel, sLabelNext, "L_ReturnFinal"_Stz);
  }

  if (pCombined->aCombinedT()->m_iHashRows)
    pCodeSynth->vSynthAllocPotRow(m_pPostStack, m_xStageOps.m_sHashExpr, true, "L_AddRowToTubPot"_Stz, "L_ReturnFinal"_Stz, ""_Stz);
  else
    pCodeSynth->vSynthAllocTubRow(m_pPostStack, true, "L_AddRowToTubPot"_Stz, "L_ReturnFinal"_Stz,""_Stz);

  // delete the postStack. No longer needed
  delete m_pPostStack;
  m_pPostStack = nullptr;

  m_pShapeEOS->vCodeSynthB(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append concluding code
  Fragments::vAppendConcludingFunc3(m_pConcludingEos->azCode());
}

/*----------------------------------------------------------------------------*/
/* sketch code for Eos fun3 for Project agg OP                                */
/*----------------------------------------------------------------------------*/
void Stencil::vSketchEosProjectGby(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  CodeSynth *pCodeSynth;
  s256 szStageNm;
  s256 sLabel;
  s256 sLabelNext;

  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // get the indexDef
  IndexDef *pIndexDef = _pIndexDef(pCombined->aCombinedT()->m_aIndexDefPL);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append code for EOSMARKER handling
  vAppendFuncHeader3();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDef
  pCodeSynth = new CodeSynth(pStreamDefOut,SR_PL);
  pCodeSynth->vSynthStruct(0);
  m_pShapeEOS->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payload to buffer
  pCodeSynth = new CodeSynth(nullptr, pStreamDefOut, nullptr, pPreExpStack, SR_NONE, SR_PL);

  vGetNextLabel(sLabelNext, pStream, pOp->i_Op(), "L_AddRowToTubPot"_Stz);

  pCodeSynth->vSynthEos(pIndexDef, m_pPostStack,sLabelNext);
  m_pShapeEOS->vCodeSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payloadRow to Tub/Pot
  pCodeSynth = new CodeSynth(pStreamDefOut, pStreamDefOut, nullptr, pPreExpStack, SR_PL, SR_NONE);

  Stz* sHavingExpr = ""_Stz;

  if (m_xStageOps.m_pHavingFilterOp) {
    // get filter expr from OP
    TagSet *pOpTags = m_xStageOps.m_pHavingFilterOp->pOpTags();
    sHavingExpr = pOpTags->szValue(C_TAG_EXPR);

    sLabel.vSprint("L_%s_%d"_Stz, m_xStageOps.m_pHavingFilterOp->szOpNm(), m_xStageOps.m_pHavingFilterOp->i_Op());
    vGetNextLabel(sLabelNext, pStream, m_xStageOps.m_pHavingFilterOp->i_Op(), "L_AdvanceNextRow"_Stz);
    pCodeSynth->vSynthFilter(sHavingExpr, sLabel, ""_Stz, sLabelNext);
    m_pShapeEOS->vCodeSynthB(pCodeSynth);
  }

  if (m_xStageOps.m_pLimitOp) {
    // get limit from limitOp
    TagSet* pOpTags = m_xStageOps.m_pLimitOp->pOpTags();
    i8 iLimit = StrLib::iToI8((char*)(pOpTags->szValue(C_TAG_COUNT)));

    sLabel.vSprint("L_%s_%d"_Stz, m_xStageOps.m_pLimitOp->szOpNm(), m_xStageOps.m_pLimitOp->i_Op());
    vGetNextLabel(sLabelNext, pStream, m_xStageOps.m_pLimitOp->i_Op(), "L_AddRowToTubPot"_Stz);
    pCodeSynth->vSynthLimit(iLimit, sLabel, sLabelNext, "L_AdvanceNextRow"_Stz);
  }

  if (pCombined->aCombinedT()->m_iHashRows)
    pCodeSynth->vSynthAllocPotRow(m_pPostStack, m_xStageOps.m_sHashExpr, true, "L_AddRowToTubPot"_Stz, "L_AdvanceNextRow"_Stz, ""_Stz);
  else
    pCodeSynth->vSynthAllocTubRow(m_pPostStack, true, "L_AddRowToTubPot"_Stz, "L_AdvanceNextRow"_Stz,""_Stz);

  m_pShapeEOS->vCodeSynthB(pCodeSynth);

  // delete the postStack. No longer needed
  delete m_pPostStack;
  m_pPostStack = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append concluding code
  Fragments::vAppendConcludingFunc3(m_pConcludingEos->azCode());
}

/*----------------------------------------------------------------------------*/
/* sketch code for Eos fun3 for sort OP                                       */
/*----------------------------------------------------------------------------*/
void Stencil::vSketchEosSort(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  CodeSynth *pCodeSynth;
  s256 szStageNm;
  s256 sLabelNext;
  s256 sLabel;
  i8   i_Op;
  eSR ePrevStencil = SR_NONE;

  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // get the indexDef
  IndexDef *pIndexDef = _pIndexDef(pCombined->aCombinedT()->m_aIndexDefPL);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append code for EOSMARKER handling
  vAppendFuncHeader3();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDef
  pCodeSynth = new CodeSynth(pStreamDefOut,SR_PL);
  pCodeSynth->vSynthStruct(0, pIndexDef);
  m_pShapeEOS->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payload to buffer
  pCodeSynth = new CodeSynth(nullptr, pStreamDefOut, nullptr, pPreExpStack, SR_NONE, SR_PL);

  vGetNextLabel(sLabelNext, pStream, pOp->i_Op(), "L_AddRowToTubPot"_Stz);

  pCodeSynth->vSynthEos(pIndexDef, m_pPostStack, sLabelNext);
  m_pShapeEOS->vCodeSynthA(pCodeSynth);

  Stz* sProjectExpr = ""_Stz;

  ePrevStencil = SR_PL;

  if (m_xStageOps.m_pPostProjectOp) {

    pLogicUnit = m_xStageOps.m_pPostProjectOp->pOpLogicUnit();
    pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

    // get filter expr from OP
    TagSet *pOpTags = m_xStageOps.m_pPostProjectOp->pOpTags();
    sProjectExpr = pOpTags->szValue(C_TAG_EXPR);
    i_Op = m_xStageOps.m_pPostProjectOp->i_Op();

    // get the aggregate type stack
    IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the streamDef
    pCodeSynth = new CodeSynth(pStreamDefOut, SR_PT);
    pCodeSynth->vSynthStruct(0);
    m_pShapeEOS->vStructSynthB(pCodeSynth);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // payloadRow to Tub/Pot
    pCodeSynth = new CodeSynth(pStreamDefOut, pStreamDefOut, pIndexDef, pPreExpStack, ePrevStencil, SR_PT);

    sLabel.vSprint("L_%s_%d"_Stz, m_xStageOps.m_pPostProjectOp->szOpNm(), i_Op);
    vGetNextLabel(sLabelNext, pStream, i_Op, "L_AddRowToTubPot"_Stz);

    pCodeSynth->bSynthPrjExp(sProjectExpr, sLabel, sLabelNext, "L_AdvanceNextRow"_Stz) ;
    m_pShapeEOS->vCodeSynthB(pCodeSynth);

    ePrevStencil = SR_PT;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payloadRow to Tub/Pot
  pCodeSynth = new CodeSynth(pStreamDefOut, pStreamDefOut, nullptr, pPreExpStack, ePrevStencil, SR_NONE);

  if (m_xStageOps.m_pLimitOp) {
    // get limit from limitOp
    TagSet* pOpTags = m_xStageOps.m_pLimitOp->pOpTags();
    i8 iLimit = StrLib::iToI8((char*)(pOpTags->szValue(C_TAG_COUNT)));

    sLabel.vSprint("L_%s_%d"_Stz, m_xStageOps.m_pLimitOp->szOpNm(), m_xStageOps.m_pLimitOp->i_Op());
    vGetNextLabel(sLabelNext, pStream, m_xStageOps.m_pLimitOp->i_Op(), "L_AddRowToTubPot"_Stz);
    pCodeSynth->vSynthLimit(iLimit, sLabel, sLabelNext, "L_ReturnFinal"_Stz);
  }

  if (pCombined->aCombinedT()->m_iHashRows)
   // pCodeSynth->vSynthAllocPotRow(m_pPostStack, m_xStageOps.m_sHashExpr, true, sLabel, "L_ReturnFinal"_Stz, ""_Stz);
    pCodeSynth->vSynthAllocPotRow(m_pPostStack, m_xStageOps.m_sHashExpr, true, "L_AddRowToTubPot"_Stz, "L_AdvanceNextRow"_Stz, ""_Stz);
  else
    pCodeSynth->vSynthAllocTubRow(m_pPostStack, false, "L_AddRowToTubPot"_Stz, "L_AdvanceNextRow"_Stz,""_Stz);

  if (m_xStageOps.m_pPostProjectOp)
    m_pShapeEOS->vCodeSynthC(pCodeSynth);
  else
    m_pShapeEOS->vCodeSynthB(pCodeSynth);

  // delete the postStack. No longer needed
  delete m_pPostStack;
  m_pPostStack = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append concluding code
  Fragments::vAppendConcludingFunc3(m_pConcludingEos->azCode());
}

/*----------------------------------------------------------------------------*/
/* sketch code for Eos fun3 for top OP                                        */
/*----------------------------------------------------------------------------*/
void Stencil::vSketchEosTop(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s256 sLabelNext;
  CodeSynth *pCodeSynth;

  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // get the IndexDef from the TopOp
  IndexDef *pIndexDef = _pIndexDef(pCombined->aCombinedT()->m_aIndexDefPL);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append function header
  vAppendFuncHeader3();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDef
  pCodeSynth = new CodeSynth(pStreamDefLft, SR_PL);
  pCodeSynth->vSynthStruct(0);
  m_pShapeEOS->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payload to buffer
  pCodeSynth = new CodeSynth(nullptr, pStreamDefLft, nullptr, pPreExpStack, SR_NONE, SR_PL);

  vGetNextLabel(sLabelNext, pStream, pOp->i_Op(), "L_AddRowToTubPot"_Stz);

  pCodeSynth->vSynthEos(pIndexDef, nullptr, sLabelNext);
  m_pShapeEOS->vCodeSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payloadRow to Tub/Pot
  pCodeSynth = new CodeSynth(pStreamDefOut, pStreamDefOut, nullptr, pPreExpStack, SR_PL, SR_NONE);

  if (pCombined->aCombinedT()->m_iHashRows)
    pCodeSynth->vSynthAllocPotRow(m_pPostStack, m_xStageOps.m_sHashExpr, true, "L_AddRowToTubPot"_Stz, "L_AdvanceNextRow"_Stz, ""_Stz);
  else
    pCodeSynth->vSynthAllocTubRow(m_pPostStack, true, "L_AddRowToTubPot"_Stz, "L_AdvanceNextRow"_Stz, ""_Stz);

  m_pShapeEOS->vCodeSynthB(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append concluding code
  Fragments::vAppendConcludingFunc3(m_pConcludingEos->azCode());
}

/*----------------------------------------------------------------------------*/
/* sketch code for indexJoin                                                  */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchIndexJoin(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* sLabel)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  CodeSynth *pCodeSynth;

  s256 sLabelNext;
  s256 sReturnLabel;

  // get join expr from OP
  TagSet* pOpTags = pOp->pOpTags();
  Stz* sJoinExpr = pOpTags->szValue(C_TAG_EXPR);

  // get the streamDefs
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);
  StreamDef *pStreamDefRht = _pStreamDef(*pLogicUnit->m_xpStreamDefRht);

  // get the aggregate type stack
  //IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  Index* pIndex = _pIndex(*pLogicUnit->m_xpIndex);

  // get the select columns
  u8 uSelectColumns = uGetSelectColumns(pStream);

  // create a new shape for this OP
  Shape *pShape = new Shape;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for struct for streamDefRht
  pCodeSynth = new CodeSynth(pStreamDefRht, SR_IJ);
  pCodeSynth->vSynthStruct(0);
  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for struct for streamDefOut (join row)
  pCodeSynth = new CodeSynth(pStreamDefOut, SR_TJ);
  pCodeSynth->vSynthStruct(0);
  pShape->vStructSynthB(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the join
  pCodeSynth = new CodeSynth(pStreamDefLft, pStreamDefOut, pIndex->pIndexDef(), nullptr, ePrevStencil, SR_TJ);

  vGetNextLabel(sLabelNext, pStream, pOp->i_Op(), "L_AddRowToTubPot"_Stz);
  pCodeSynth->vSynthIndexJoin(pStreamDefRht, pIndex->pIndexDef(), sJoinExpr,
      sLabel, sLabelNext, uSelectColumns);
  pShape->vCodeSynthA(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* get the next label for Op                                                  */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchIndexScan(Combined* pCombined, Stream* pStream, Op* pOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  Shape* pShape;
  CodeSynth *pCodeSynth;

  s256 sLabel;
  s256 sLabelNext;
  s256 sOrdAggLabel;

  sOrdAggLabel = ""_Stz;

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // get the index from the combinedT
  Index* pIndexScan = _pIndex(pCombined->aCombinedT()->m_aIndexT);

  // get the tags
  TagSet* pOpTags = pOp->pOpTags();

  // get indexScan expr from OP
  Stz* sIndexScanExpr = pOpTags->szValue(C_TAG_EXPR);

  // create a new shape for this OP
  pShape = new Shape;

  u8 uSelectColumns = uGetSelectColumns(pStream);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for struct for streamDefOut (J)
  pCodeSynth = new CodeSynth(pStreamDefOut, SR_IT);
  pCodeSynth->vSynthStruct(uSelectColumns);
  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the index scan
  pCodeSynth = new CodeSynth(pStreamDefLft, pStreamDefOut, pIndexScan->pIndexDef(), nullptr, SR_IT, SR_NONE);
  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), pOp->i_Op());
  vGetNextLabel(sLabelNext, pStream, pOp->i_Op(), "L_AddRowToTubPot"_Stz);

  Op *pNextOp = pStream->pGet(pOp->i_Op()+1);
  if ( pNextOp->szOpNm()->bSame("ordAgg"_Stz) )
    sOrdAggLabel = sLabelNext;

  pCodeSynth->vSynthIndexScan(sIndexScanExpr, sLabel, sLabelNext, sOrdAggLabel, uSelectColumns);
  pShape->vCodeSynthA(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch scan (inflate crw of crwBuffer)                                     */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchInflateCrw(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  Shape* pShape;
  CodeSynth *pCodeSynth;

  s256 sLabel;
  s256 sReturnLabel;

  // get the streamDefLft
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // create a new shape for this OP
  pShape = new Shape;

  u8 uSelectColumns = uGetSelectColumns(pStream);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDefLft
  pCodeSynth = new CodeSynth(pStreamDefLft, SR_IT);
  pCodeSynth->vSynthStruct(0);
  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the row inflate
  pCodeSynth = new CodeSynth(pStreamDefLft, nullptr, nullptr, nullptr, SR_NONE, SR_NONE);
  sLabel.vSprint("L_InflateT"_Stz);
  sReturnLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), pOp->i_Op());
  pCodeSynth->vSynthInflateCrw("T"_Stz, sLabel, m_xStageOps.m_sLabelOrdAgg, sReturnLabel, &uSelectColumns);
  pShape->vCodeSynthA(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch inflate of row from rowBuffer                                       */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchInflateRow(Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  Shape *pShape;
  CodeSynth *pCodeSynth;

  s256 sLabel;

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // create a new shape for this OP
  pShape = new Shape;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDefLft
  pCodeSynth = new CodeSynth(pStreamDefLft, SR_IT);
  pCodeSynth->vSynthStruct(0);
  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the row inflate
  pCodeSynth = new CodeSynth(pStreamDefLft, nullptr, nullptr, nullptr, SR_NONE, SR_NONE);
  sLabel.vSprint("L_InflateT"_Stz);
  pCodeSynth->vSynthInflateRow(sLabel, m_xStageOps.m_sLabelOrdAgg);
  pShape->vCodeSynthA(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch filter                                                              */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchFilter(Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* sLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  s4K sLabelPrev;

  bool bLastFilter = false;

  i8 i_Op = pOp->i_Op();
  CodeSynth *pCodeSynth;
  //bool bLastFilter = false;

  // create a new shape for this OP
  Shape *pShape = new Shape;

  // get filter expr from OP
  TagSet* pOpTags = pOp->pOpTags();
  Stz* sFilterExpr = pOpTags->szValue(C_TAG_EXPR);

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  Op *pNextOp = pStream->pGet(i_Op + 1);
  if (pNextOp->szOpNm()->bSame("combined"_Stz))
    bLastFilter = true;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the OP
  pCodeSynth = new CodeSynth(pStreamDefLft, pStreamDefLft, nullptr, pPreExpStack, ePrevStencil, SR_NONE);
  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStream, i_Op, "L_AddRowToTubPot"_Stz);

  //Op *pPrevOp = pStream->pGet(i_Op - 1);
  sLabelPrev = ""_Stz;
  pCodeSynth->vSynthFilter(sFilterExpr, sLabel, sLabelPrev, sLabelLoop);

  pShape->vCodeSynthB(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch keyExists                                                           */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchKeyExists(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  s4K sLabelPrev;
  s4K sLabelLoop;

  i8 i_Op = pOp->i_Op();
  CodeSynth *pCodeSynth;
  //bool bLastFilter = false;

  // create a new shape for this OP
  Shape *pShape = new Shape;

  // get filter expr from OP
  TagSet* pOpTags = pOp->pOpTags();
  Stz* sFilterExpr = pOpTags->szValue(C_TAG_EXPR);

  // get key expr from OP
  Stz* sKeyExpr = pOpTags->szValue(C_TAG_KEYEXPR);

  // get the streamDefLft
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);
  StreamDef *pStreamDefRht = _pStreamDef(*pLogicUnit->m_xpStreamDefRht);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // get the index
  Index *pIndex = _pIndex(pCombined->aCombinedT()->m_aIndexJ);

  u8 uSelectColumns = pLogicUnit->m_uSelectColumns;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for struct for streamDefRht
  pCodeSynth = new CodeSynth(pStreamDefRht, SR_IJ);
  pCodeSynth->vSynthStruct(uSelectColumns);
  pShape->vStructSynthA(pCodeSynth);

  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStream, i_Op, "L_AdvanceNextRow"_Stz);

  pCodeSynth = new CodeSynth(pStreamDefLft, pStreamDefLft, pIndex->pIndexDef(), pPreExpStack, SR_IJ, ePrevStencil);
  pCodeSynth->vSynthKeyExists(pStreamDefRht, pIndex->pIndexDef(), sFilterExpr, sKeyExpr, sLabel, uSelectColumns);
  pShape->vCodeSynthB(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch label                                                               */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchLabel(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  CodeSynth *pCodeSynth;

  i8 i_Op = pOp->i_Op();

  // create a new shape for this OP
  Shape *pShape = new Shape;

  // form teh labels
  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStream, i_Op, "L_AddRowToTubPot"_Stz);

  // synthesize the code
  pCodeSynth = new CodeSynth();
  pCodeSynth->vSynthLabel(pOp->szOpNm(), sLabel, sLabelNext);
  pShape->vCodeSynthB(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch limit                                                               */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchLimit(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* sLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOp->i_Op();
  CodeSynth *pCodeSynth;

  // create a new shape for this OP
  Shape *pShape = new Shape;

  // get limit from limitOp
  TagSet* pOpTags = pOp->pOpTags();
  i8 iLimit = StrLib::iToI8((char*)(pOpTags->szValue(C_TAG_COUNT)));

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // new the aggregate type stack
  pLogicUnit->m_xpPreExpStack = new IntStack;
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the OP
  pCodeSynth = new CodeSynth(pStreamDefLft, pStreamDefLft, nullptr, pPreExpStack, ePrevStencil, SR_NONE);
  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStream, i_Op, "L_AddRowToTubPot"_Stz);

  pCodeSynth->vSynthLimit(iLimit, sLabel, sLabelNext, sLabelLoop);
  pShape->vCodeSynthB(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch ordered aggregate                                                   */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchOrdAgg(Combined* pCombined, Stream* pStream, Op* pOp,
                              eSR ePrevStencil, Stz* sLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  s4K sLabelPrev;

  i8 i_Op = pOp->i_Op();
  CodeSynth *pCodeSynth;

  // get filter expr from OP
  TagSet* pOpTags = pOp->pOpTags();
  Stz* sProjectExpr = pOpTags->szValue(C_TAG_EXPR);

  // get the streamDefs
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // get the indexDef
  IndexDef *pIndexDef = _pIndexDef(pCombined->aCombinedT()->m_aIndexDefPL);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // create a new shape for this OP
  Shape *pShape = new Shape;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDef (aggregate)
  pCodeSynth = new CodeSynth(pStreamDefOut, SR_AT);
  pCodeSynth->vSynthStruct(0);
  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the row projection
  pCodeSynth = new CodeSynth(pStreamDefLft, pStreamDefOut, pIndexDef, pPreExpStack, ePrevStencil, SR_AT);

  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStream, i_Op, "L_AddRowToTubPot"_Stz);
  pCodeSynth->vSynthOrdAgg( sProjectExpr,
                            sLabel,
                            sLabelNext,
                            sLabelLoop);
  pShape->vCodeSynthA(pCodeSynth);

  //m_pPostStack = pCodeSynth->pPostStack();
  //pCodeSynth->vPostStack(nullptr);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch project expressions                                                 */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchPrjAgg(Combined* pCombined, Stream* pStream, Op* pOp,
                                  eSR ePrevStencil, Stz* sLabelLoop)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  s256 szStageNm;

  i8 i_Op = pOp->i_Op();
  CodeSynth *pCodeSynth;

  // get project expr from OP
  TagSet* pOpTags = pOp->pOpTags();
  Stz* sProjectExpr = pOpTags->szValue(C_TAG_EXPR);

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // create a new shape for this OP
  Shape *pShape = new Shape;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDef
  pCodeSynth = new CodeSynth(pStreamDefOut, SR_AT);
  pCodeSynth->vSynthStruct(0);
  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the row projection
  pCodeSynth = new CodeSynth(pStreamDefLft, pStreamDefOut, nullptr, pPreExpStack, ePrevStencil, SR_AT);

  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStream, i_Op, "L_AdvanceNextRow"_Stz);

  m_pPostStack = pCodeSynth->pSynthPrjAgg(sProjectExpr,
                                                 sLabel,
                                                 sLabelLoop);

  pShape->vCodeSynthA(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch project expressions                                                 */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchPrjExp(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* sLabelLoop)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOp->i_Op();
  CodeSynth *pCodeSynth;

  // get project expr from OP
  TagSet* pOpTags = pOp->pOpTags();
  Stz* sProjectExpr = pOpTags->szValue(C_TAG_EXPR);

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // create a new shape for this OP
  Shape *pShape = new Shape;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDef
  pCodeSynth = new CodeSynth(pStreamDefOut, SR_PT);
  pCodeSynth->vSynthStruct(0);
  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the row projection
  pCodeSynth = new CodeSynth(pStreamDefLft, pStreamDefOut, nullptr, pPreExpStack, ePrevStencil, SR_PT);

  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStream, i_Op, "L_AddRowToTubPot"_Stz);

  bool bSynthResult = pCodeSynth->bSynthPrjExp(sProjectExpr,
                                                sLabel,
                                                sLabelNext,
                                                sLabelLoop);

  RASSERT (bSynthResult);

  pShape->vCodeSynthA(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch project group by expressions                                        */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchProjectGby(Combined* pCombined, Stream* pStream, Op* pOp,
                                  eSR ePrevStencil, Stz* sLabelLoop)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOp->i_Op();
  CodeSynth *pCodeSynth;

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // get the indexDef
  IndexDef *pIndexDef = _pIndexDef(pCombined->aCombinedT()->m_aIndexDefPL);

  // get project expr from OP
  TagSet* pOpTags = pOp->pOpTags();
  Stz* sProjectExpr = pOpTags->szValue(C_TAG_EXPR);

  // create a new shape for this OP
  Shape *pShape = new Shape;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDef
  pCodeSynth = new CodeSynth(pStreamDefOut, SR_AT);
  pCodeSynth->vSynthStruct(0);
  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the row projection
  pCodeSynth = new CodeSynth(pStreamDefLft, pStreamDefOut, pIndexDef, pPreExpStack, ePrevStencil, SR_AT);

  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);

  m_pPostStack = pCodeSynth->pSynthAggGby(sProjectExpr,
                                          sLabel,
                                          sLabelLoop);

  pShape->vCodeSynthA(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch sort                                                                */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchSort(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* szLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  s4K sLabelPrev;

  i8 i_Op = pOp->i_Op();
  CodeSynth *pCodeSynth;

  // get filter expr from OP
  TagSet* pOpTags = pOp->pOpTags();
  Stz* sProjectExpr = pOpTags->szValue(C_TAG_EXPR);

  // get the streamDefs
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // get the indexDef
  IndexDef *pIndexDef = _pIndexDef(pCombined->aCombinedT()->m_aIndexDefPL);

  // get the sort type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // create a new shape for this OP
  Shape *pShape = new Shape;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDef
//  pCodeSynth = new CodeSynth(pStreamDefOut, ePrevStencil);
//  pCodeSynth->vSynthStruct(0);
//  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the row projection
  pCodeSynth = new CodeSynth(pStreamDefLft, pStreamDefOut, pIndexDef, pPreExpStack, ePrevStencil, ePrevStencil);

  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
  pCodeSynth->vSynthSort( sProjectExpr,
                            sLabel,
                            szLabelLoop);
  pShape->vCodeSynthA(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch top                                                                 */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchTop(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* sLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOp->i_Op();
  CodeSynth *pCodeSynth;

  // create a new shape for this OP
  Shape *pShape = new Shape;

  // get opTags from OP
  TagSet* pOpTags = pOp->pOpTags();
  Stz* sKeyExpr = pOpTags->szValue(C_TAG_EXPR);

  i8 iTopCount = StrLib::iToI8((char*)(pOpTags->szValue(C_TAG_COUNT)));

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // get the IndexDef from the TopOp
  IndexDef* pIndexDef = _pIndexDef(pCombined->aCombinedT()->m_aIndexDefPL);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the OP
  pCodeSynth = new CodeSynth(pStreamDefLft, pStreamDefLft, pIndexDef, nullptr, ePrevStencil, SR_NONE);
  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);

  pCodeSynth->vSynthTop(sKeyExpr, iTopCount, sLabel, sLabelLoop);
  pShape->vCodeSynthB(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch pot                                                                 */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchPot(Combined* pCombined, Op* pLastOp, eSR ePrevStencil,
                           IntStack* pPreExpStack, Stz* sLabel, Stz* sLabelLoop, Stz* sLabelOrdAgg) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  // get the streamDefOut
  LogicUnit *pLogicUnit = pLastOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);
  RASSERT(pStreamDefOut);

  // create a new shape
  Shape* pShape = new Shape;

  CodeSynth* pCodeSynth = new CodeSynth(pStreamDefOut, pStreamDefOut, nullptr, pPreExpStack, ePrevStencil, SR_NONE);

  pCodeSynth->vSynthAllocPotRow(m_pPostStack, m_xStageOps.m_sHashExpr, false, sLabel, sLabelLoop, sLabelOrdAgg);
  pShape->vCodeSynthB(pCodeSynth);

  // delete the postStack. No longer needed
  delete m_pPostStack;
  m_pPostStack = nullptr;

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch return                                                              */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchReturn(void) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  // create a new shape
  Shape* pShape = new Shape;

  CodeSynth* pCodeSynth = new CodeSynth();

  pCodeSynth->vSynthReturn();
  pShape->vCodeSynthB(pCodeSynth);
  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch tub                                                                 */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchTub(Combined* pCombined, Op* pLastOp, eSR ePrevStencil,
                           IntStack* pPreExpStack, Stz* sLabel, Stz* sLabelLoop, Stz* sLabelOrdAgg) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  // get the streamDefOut
  LogicUnit *pLogicUnit = pLastOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);
  RASSERT(pStreamDefOut);

  // create a new shape
  Shape* pShape = new Shape;

  CodeSynth* pCodeSynth = new CodeSynth(pStreamDefOut, pStreamDefOut, nullptr, pPreExpStack, ePrevStencil, SR_NONE);

  pCodeSynth->vSynthAllocTubRow(m_pPostStack, false, sLabel, sLabelLoop, sLabelOrdAgg);
  pShape->vCodeSynthB(pCodeSynth);

  // delete the postStack. No longer needed
  delete m_pPostStack;
  m_pPostStack = nullptr;

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* Func header                                                                */
/*----------------------------------------------------------------------------*/
void Stencil::vAppendFuncHeader0(void) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sFunSignature;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form signature for fun0
  sFunSignature.vSprint("  int iFun0_%s(struct Combined_T* aCombinedT)"_Stz, m_szStageNm.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary code
  Fragments::vAppendPreliminaryFunc0(m_pPreliminaryFun0->azCode(), sFunSignature);

}

/*----------------------------------------------------------------------------*/
/* Func header                                                                */
/*----------------------------------------------------------------------------*/
void Stencil::vAppendFuncHeader1(void) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sFunSignature;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form signature for fun0
  sFunSignature.vSprint("  int iFun1_%s(struct Combined_T* aCombinedT)"_Stz, m_szStageNm.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary code
  Fragments::vAppendPreliminaryFunc1(m_pPreliminaryFun1->azCode(), sFunSignature);
}

/*----------------------------------------------------------------------------*/
/* Func header                                                                */
/*----------------------------------------------------------------------------*/
void Stencil::vAppendFuncHeader2(void) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sFunSignature;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form signature for fun0
  sFunSignature.vSprint("  int iFun2_%s(struct Combined_T* aCombinedT)"_Stz, m_szStageNm.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary code
  Fragments::vAppendPreliminaryFunc2(m_pPreliminaryFun2->azCode(), sFunSignature);
}

/*----------------------------------------------------------------------------*/
/* Func header                                                                */
/*----------------------------------------------------------------------------*/
void Stencil::vAppendFuncHeader3(void) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sFunSignature;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form signature for fun0
  sFunSignature.vSprint("  int iFun3_%s(struct Combined_T* aCombinedT)"_Stz, m_szStageNm.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary code
  Fragments::vAppendPreliminaryFunc3(m_pPreliminaryEos->azCode(), sFunSignature);
}

/*----------------------------------------------------------------------------*/
/* Func header                                                                */
/*----------------------------------------------------------------------------*/
void Stencil::vAppendFuncEmpty(CodeSynth* pCodeSynth, Stz* szDesc, Stz* szFunNm) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sFunSignature;

  sFunSignature.vSprint("\n  int %s_%s(struct Combined_T* aCombinedT)"_Stz, szFunNm, m_szStageNm.azChar());
  Fragments::vAppendEmptyFunc(pCodeSynth->azCode(), szDesc, sFunSignature);
}

/*----------------------------------------------------------------------------*/
/* Func header                                                                */
/*----------------------------------------------------------------------------*/
void Stencil::vAppendHeaderEntries(i8 iStage) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sFunSignature;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append banner head
  m_pOpHeader->vAppendFragment((Stz*)m_pBannerHead->azCode());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form signature for fun0
  sFunSignature.vSprint("\n\n  int iFun0_%s(struct Combined_T* aCombinedT);"_Stz, m_szStageNm.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append function signatures
  m_pOpHeader->vAppendFragment(sFunSignature);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form signature for fun1
  sFunSignature.vSprint("\n  int iFun1_%s(struct Combined_T* aCombinedT);"_Stz, m_szStageNm.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append function signatures
  m_pOpHeader->vAppendFragment(sFunSignature);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form signature for fun2
  sFunSignature.vSprint("\n  int iFun2_%s(struct Combined_T* aCombinedT);"_Stz, m_szStageNm.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append function signatures
  m_pOpHeader->vAppendFragment(sFunSignature);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form signature for fun3
  sFunSignature.vSprint("\n  int iFun3_%s(struct Combined_T* aCombinedT);"_Stz, m_szStageNm.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append function signatures
  m_pOpHeader->vAppendFragment(sFunSignature);

  sFunSignature.vSprint("\n\n  int (*stage%lld[4])(struct Combined_T* aCombinedT) = {"_Stz, iStage);
  m_pOpHeader->vAppendFragment(sFunSignature);

  sFunSignature.vSprint("\n    iFun0_%s,"_Stz, m_szStageNm.azChar());
  m_pOpHeader->vAppendFragment(sFunSignature);

  sFunSignature.vSprint("\n    iFun1_%s,"_Stz, m_szStageNm.azChar());
  m_pOpHeader->vAppendFragment(sFunSignature);

  sFunSignature.vSprint("\n    iFun2_%s,"_Stz, m_szStageNm.azChar());
  m_pOpHeader->vAppendFragment(sFunSignature);

  sFunSignature.vSprint("\n    iFun3_%s,"_Stz, m_szStageNm.azChar());
  m_pOpHeader->vAppendFragment(sFunSignature);

  sFunSignature.vSprint("\n  };"_Stz);
  m_pOpHeader->vAppendFragment(sFunSignature);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append banner tail
  m_pOpHeader->vAppendFragment(m_pBannerTail);
}

/*----------------------------------------------------------------------------*/
/* collect and validate the stage information of stage being combined         */
/*----------------------------------------------------------------------------*/
eEC Stencil::eCollectStage(Combined* pCombined, Stream* pStream) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  i8 iOpsToCombine = 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate the operators (skipping consumeOP and sendSE)
  for (i8 i_Op = 1; i_Op < pStream->nItems() - 1; i_Op++) {

    // get the op
    Op *pOp = pStream->pGet(i_Op);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // OP after CombinedOp ?
    if ( m_xStageOps.m_iCombinedOp ) {
      vTrace("Error: Cannot have OP's after combinedOp (%s)!!!\n"_Stz,
             m_szStageNm.azChar());
      return EC_INVALID_PIPELINE;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // scanOp
    if (pOp->szOpNm()->bSame("scan"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // Should be the first OP after consumeOp
      if (i_Op != 1) {
        vTrace("Error: scanOP should be the first OP in the stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE SCAN-OP's
      if (m_xStageOps.m_iScanOp) {
        vTrace("Error: Cannot have multiple scan operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_bScan = true;
      m_xStageOps.m_iScanOp++;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // combined OP
    if (pOp->szOpNm()->bSame("combined"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE COMBINED OP's
      if (m_xStageOps.m_iCombinedOp) {
        vTrace("Error: Cannot have multiple combinedOp's (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_iCombinedOp++;
      m_xStageOps.m_pCombinedOp = pOp;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // skip scanOP and combinedOP and count no of OPS to combine
    iOpsToCombine++;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // aggregate Op
    if (pOp->szOpNm()->bSame("aggreg"_Stz)) {

      m_xStageOps.m_iAggregOp++;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE PAYLOAD OP's
      if (m_xStageOps.m_bPayloadOp) {
        vTrace("Error: Cannot have multiple combination of payload operators (aggregate/sort/top)"
            "in the same stage (%s)!!!\n"_Stz,
            m_szStageNm.azChar());

        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_bPayloadOp = true;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // filterOp
    if (pOp->szOpNm()->bSame("filter"_Stz)) {
      if (m_xStageOps.m_bPayloadOp)
        m_xStageOps.m_pHavingFilterOp = pOp;
      m_xStageOps.m_iFilterOp++;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // hashOp
    if (pOp->szOpNm()->bSame("hash"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE HASH OP's
      if (m_xStageOps.m_iHashOp) {
        vTrace("Error: Cannot have multiple hashOp's (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // Should be the last OP before CombinedOp
      if (i_Op != pStream->nItems() - 3) {
        vTrace("Error: HashOP should be the last OP before combinedOP (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_iHashOp++;

      // set hashRows in combined (needed at runtime)
      pCombined->aCombinedT()->m_iHashRows = 1;

      // get hash expr from OP
      TagSet *pOpTags = pOp->pOpTags();
      m_xStageOps.m_sHashExpr = pOpTags->szValue(C_TAG_EXPR);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // indexJoinOp
    if (pOp->szOpNm()->bSame("indexJoin"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE INDEXJOIN OP's
      if (m_xStageOps.m_iIndexJoinOp) {
        vTrace("Error: Cannot have multiple indexJoinOP's (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_iIndexJoinOp++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // indexScanOp
    if (pOp->szOpNm()->bSame("indexScan"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // Should be the first OP after consumeOp
      if (i_Op != 1) {
        vTrace("Error: indexScanOP should be the first OP in the stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE INDEXSCAN-OP's
      if (m_xStageOps.m_iIndexScanOp) {
        vTrace("Error: Cannot have multiple indexScan operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      // set indexScan in combined (needed at runtime)
      pCombined->aCombinedT()->m_iIndexScan = 1;

      m_xStageOps.m_bIndexScan = true;
      m_xStageOps.m_iIndexScanOp++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // keyExistsOp
    if (pOp->szOpNm()->bSame("keyExists"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE LIMIT-OP's
      if (m_xStageOps.m_iKeyExistsOp) {
        vTrace("Error: Cannot have multiple keyExists operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_iKeyExistsOp++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // limitOp
    if (pOp->szOpNm()->bSame("limit"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE LIMIT-OP's
      if (m_xStageOps.m_iLimitOp) {
        vTrace("Error: Cannot have multiple limit operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_pLimitOp = pOp;
      m_xStageOps.m_iLimitOp++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ordAggOp
    if (pOp->szOpNm()->bSame("ordAgg"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE LIMIT-OP's
      if (m_xStageOps.m_iOrdAggOp) {
        vTrace("Error: Cannot have multiple ordAgg operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }


      m_xStageOps.m_sLabelOrdAgg.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), pOp->i_Op());

      m_xStageOps.m_iOrdAggOp++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // projectOp
    if (pOp->szOpNm()->bSame("project"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE PROJECT-OP's
      if (m_xStageOps.m_iProjectOp) {
        vTrace("Error: Cannot have multiple project operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // PROJECT-OP after AGGREG-OP
      if (m_xStageOps.m_iAggregOp) {
        vTrace("Error: Cannot have project operator after an aggregate operator in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      eEK eExprKind = pOp->pOpLogicUnit()->m_eExprKind;

      if (eExprKind == EK_AGGREG) {

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // MULTIPLE PAYLOAD OP's
        if (m_xStageOps.m_bPayloadOp) {
          vTrace("Error: Cannot have multiple combination of payload operators (aggregate/sort/top) in the same stage (%s)!!!\n"_Stz,
                 m_szStageNm.azChar());

          return EC_INVALID_PIPELINE;
        }

        m_xStageOps.m_bPayloadOp = true;
      }

      // is this project after the sort ?
      if (m_xStageOps.m_iSortOp) {
        m_xStageOps.m_pPostProjectOp = pOp;
      }
      m_xStageOps.m_iProjectOp++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // sortOp
    if (pOp->szOpNm()->bSame("sort"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE SORT-OP's
      if (m_xStageOps.m_iSortOp) {
        vTrace("Error: Cannot have multiple sort operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE PAYLOAD OP's
      if (m_xStageOps.m_bPayloadOp) {
        vTrace("Error: Cannot have multiple combination of payload operators (aggregate/sort/top) in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());

        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_bPayloadOp = true;
      m_xStageOps.m_iSortOp++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // topOp
    if (pOp->szOpNm()->bSame("top"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE TOP-OP's
      if (m_xStageOps.m_iTopOp) {
        vTrace("Error: Cannot have multiple top operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // TOP with ORDAGG
      if (m_xStageOps.m_iOrdAggOp) {
        vTrace("Error: Cannot have top with OrdAggOP the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());

        return EC_INVALID_PIPELINE;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE PAYLOAD OP's
      if (m_xStageOps.m_bPayloadOp) {
        vTrace("Error: Cannot have multiple combination of payload operators (aggregate/sort/top) in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());

        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_bPayloadOp = true;
      m_xStageOps.m_iTopOp++;
    }
  }

  // we should have seen a combinedOP ?
  if ( !m_xStageOps.m_pCombinedOp ) {
    vTrace("Error: Missing combinedOp (%s)!!!\n"_Stz,
           m_szStageNm.azChar());
    return EC_INVALID_PIPELINE;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Should have a hashCast in the SendSE if hashOP
  if (m_xStageOps.m_iHashOp) {

    // get the lastOp
    Op *pOp = pStream->pGet(pStream->nItems() - 1);

    // should be a SendSE OP
    RASSERT(pOp->szOpNm()->bSame("sendSE"_Stz));

    if (pOp->eSendCast() != XK_HASHCAST) {
      vTrace("Error: Missing hashcast (hashOP) (%s)!!!\n"_Stz,
             m_szStageNm.azChar());
      return EC_INVALID_PIPELINE;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // any OP's to combine ?
  if (iOpsToCombine == 0) {
    vTrace("Error: Zero OP's to combine (%s)!!!\n"_Stz,
           m_szStageNm.azChar());
    return EC_INVALID_PIPELINE;
  }

  return EC_NONE;
}

//*============================================================================*/
/* end                                                                         */
//*============================================================================*/
