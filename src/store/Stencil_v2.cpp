/*============================================================================*/
/* Stencil_v2.cpp -- Op Stencil_v2                                                  */
/*============================================================================*/

#include "BASE.h"
#include "CmdLineArgs.hpp"

#include "Stencil_v2.hpp"
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

#include "CodeSynthUtils.hpp"
#include "eSK.h"

extern CmdLineArgs       gxCmdLineArgs;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Stencil_v2,CN_STENCIL_V2)
__aNewFunction(Stencil_v2,CN_STENCIL_V2)
__vDeleteFunction(Stencil_v2,CN_STENCIL_V2)
__aFooterFunction(Stencil_v2)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stencil_v2::Stencil_v2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL_V2,_FOOTER_);

  m_szStageNm    = ""_Stz;
  m_bRowBuffer   = false;

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
  m_xStageOps.m_pLimitOp        = nullptr;

  m_xStageOps.m_sLabelOrdAgg = ""_Stz;
  m_xStageOps.m_sHashExpr = ""_Stz;

  m_pBannerHead  = nullptr;
  m_pBannerTail  = nullptr;

  m_pPreliminary     = nullptr;
  m_pPreliminaryFun0 = nullptr;
  m_pPreliminaryFun1 = nullptr;
  m_pPreliminaryFun2 = nullptr;
  m_pPreliminaryEos = nullptr;

  m_pConcluding     = nullptr;
  m_pConcludingFun0 = nullptr;
  m_pConcludingFun1 = nullptr;
  m_pConcludingFun2 = nullptr;
  m_pConcludingEos  = nullptr;
  m_pOpHeader    = nullptr;
  m_pOpSource    = nullptr;
  m_pEmptyFun0   = nullptr;
  m_pEmptyFun1   = nullptr;
  m_pEmptyFun2   = nullptr;

  m_pShapeCRW    = nullptr;
  m_pShapeROW    = nullptr;
  m_pShapeIDX    = nullptr;
  m_pShapeEOS    = nullptr;

  try {

    m_pOpSource    = new CombinedSrc;

    m_pBannerHead  = new CodeSynth_v2;
    m_pBannerTail  = new CodeSynth_v2;

    m_pPreliminary     = new CodeSynth_v2;
    m_pPreliminaryFun0 = new CodeSynth_v2;
    m_pPreliminaryFun1 = new CodeSynth_v2;
    m_pPreliminaryFun2 = new CodeSynth_v2;
    m_pPreliminaryEos = new CodeSynth_v2;

    m_pConcluding     = new CodeSynth_v2;
    m_pConcludingFun0 = new CodeSynth_v2;
    m_pConcludingFun1 = new CodeSynth_v2;
    m_pConcludingFun2 = new CodeSynth_v2;
    m_pConcludingEos  = new CodeSynth_v2;
    m_pOpHeader    = new CodeSynth_v2;
    m_pEmptyFun0   = new CodeSynth_v2;
    m_pEmptyFun1   = new CodeSynth_v2;
    m_pEmptyFun2   = new CodeSynth_v2;
    m_pShapeEOS    = new OpSketch;
  }
  catch (LowMemory xLowMemory) {

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

    delete m_pShapeEOS;

    throw xLowMemory;
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append banner head
  Fragments::vAppendBannerHead(m_pBannerHead->azCode());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary defines etc
  Fragments::vAppendPreliminary(m_pPreliminary->azCode());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append banner tail
  Fragments::vAppendBannerTail(m_pBannerTail->azCode());

  _checkInitialised_(CN_STENCIL_V2);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STENCIL_V2,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stencil_v2::~Stencil_v2(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL_V2,_FOOTER_);

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
void Stencil_v2::vRepair(Object* pObject)
{
  _static_

  Stencil_v2* pStencil_v2 = _pStencil_v2(pObject);

  // repair the stencil
  // ...
  
  // delete the stencil object
  delete pStencil_v2;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* combine synthesized op code (put code for all shapes together)             */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vCombine(bool bGenerateCCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL_V2,_FOOTER_);

  /*
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
      OpSketch *pShape = pGet(i_Shape);

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
        OpSketch *pShape = pGet(i_Shape);

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
      OpSketch *pShape = pGet(i_Shape);

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

 */

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // synthesize the code for the combined operators
  m_pOpSource->vSynthesizeCode(this);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // write to a file

  if ( ! gxCmdLineArgs.m_xSysArgs.m_bCombined && ! bGenerateCCode)
    return;

  s256 sFileHpp;
  s256 sFileC;
  s256 funcName;
  LocalFile oFile;

  i8 iQueryNo = AM::iQueryNoInc();

  if (gxCmdLineArgs.m_xSysArgs.m_bCombined) {
    if (!Dir::bExists("../combined"_Stz)) {
      if (!Dir::bMake("../combined"_Stz)) {
        vTrace("Unable to make dir ../combined"_Stz);
        return;
      }
    }
    sFileHpp.vSprint("../combined/FlatQuery.hpp"_Stz, iQueryNo,
                      AM::pGrid()->pThisNode()->szNodeNm());
    sFileC.vSprint("../combined/FlatQuery%lld_%s.c"_Stz, iQueryNo,
                      AM::pGrid()->pThisNode()->szNodeNm());
  }
  else if (bGenerateCCode) {
    if (! Dir::bExists("../ccode"_Stz)) {
      if (! Dir::bMake("../ccode"_Stz)) {
        vTrace("Unable to make dir ../ccode"_Stz);
        return;
      }
    }
    sFileHpp.vSprint("../ccode/FlatQuery.hpp"_Stz, iQueryNo,
                      AM::pGrid()->pThisNode()->szNodeNm());
    sFileC.vSprint("../ccode/FlatQuery%lld_%s.c"_Stz, iQueryNo,
                      AM::pGrid()->pThisNode()->szNodeNm());
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // write to the .hpp file
  bool bResult = oFile.bOpenAppend(sFileHpp);
  oFile.vPrint((Stz*)m_pOpHeader->azCode());
  oFile.vClose();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // write to the .c file
  bResult = oFile.bOpenWrite(sFileC);
  oFile.vPrint((Stz*)m_pOpSource->azCode());
  oFile.vClose();

}

/*----------------------------------------------------------------------------*/
/* return op source code                                                      */
/*----------------------------------------------------------------------------*/
Stz* Stencil_v2::szOpSource(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL_V2,_FOOTER_);
  
  char* azCode = m_pOpSource->azCode();
  
  return (Stz*)azCode;
}

/*----------------------------------------------------------------------------*/
/* print the synthesized code                                                 */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL_V2,_FOOTER_);

  //vTrace("Synthesized Code:\n %s\n"_Stz,m_pOpSource->azCode());
}

/*----------------------------------------------------------------------------*/
/* sketch source from pattern                                                 */
/*----------------------------------------------------------------------------*/
eEC Stencil_v2::eSketch(Combined* pCombined, Stream* pStream)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL_V2,_FOOTER_);

  CodeSynth_v2 *pCodeSynth;

  s4K sLabel;
  s4K sReturnLabel;
  s4K sLabelLoop;
  s4K sLabelNext;
  s4K sLabelOrdAgg;

  bool bInflateGenerated = false;

  OpSketch *pOpSketch;
  OpSketch *pPotTubSketch;

  eSR ePrevStencil_v2 = SR_NONE;

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
    if (pOp->szOpNm()->bSame("combined_v2"_Stz)) {
      break;
    }

    // skip scan OP
    if (pOp->szOpNm()->bSame("scan"_Stz))
      continue;

    // generate code for indexScan OP
    if (pOp->szOpNm()->bSame("indexScan"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupIndexScan(pCombined, pStream, pOp);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      m_pShapeIDX = pSketchIndexScan(pCombined, pStream, pOp);

      bInflateGenerated = true;
      ePrevStencil_v2 = SR_IT;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for inflate (driving code) if not already inflated
    if (!bInflateGenerated) {

      RASSERT(!m_xStageOps.m_bIndexScan);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupScan(pCombined, pStream, pOp);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchInflateCrw(pCombined, pStream, pOp);
      vAppend(pOpSketch);

      m_bRowBuffer = true;

      bInflateGenerated = true;
      ePrevStencil_v2 = SR_IT;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the projectOP
    if (pOp->szOpNm()->bSame("project_v2"_Stz)) {

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        vSetupProjectExp(pCombined, pStream, pOp);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        pOpSketch = pSketchProject(pCombined, pStream, pOp, ePrevStencil_v2, sLabelLoop);
        vAppend(pOpSketch);

        ePrevStencil_v2 = SR_PT;

    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the filterOP
    if (pOp->szOpNm()->bSame("filter_v2"_Stz)) {

      Op *pPrevOp = pStream->pGet(i_Op - 1);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchFilter(pStream, pOp, ePrevStencil_v2, sLabelLoop);
      vAppend(pOpSketch);
    }

  }

  if (m_xStageOps.m_bGeneratePotTub) {

    sLabel.vSprint("L_AddRowToTubPot"_Stz);

    if (pCombined->aCombinedT()->m_iHashRows) {
      RASSERT(0);
      //pPotTubSketch = pSketchPot(pOpSketch, ePrevStencil_v2);
    }
    else {
      pPotTubSketch = pSketchTub(pOpSketch, sLabelLoop, ePrevStencil_v2);
    }

    // add the op sketch to the stencil
    vAppend(pPotTubSketch);
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
void Stencil_v2::vSetup(Combined* pCombined, Stream* pStream)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL_V2,_FOOTER_);

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
    if (pOp->szOpNm()->bSame("combined_v2"_Stz)) {
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
    else {
      vSetupScan(pCombined, pStream, pOp);
      bInflateGenerated = true;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the filterOP
    if (pOp->szOpNm()->bSame("filter"_Stz)) {
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the aggregOp / projectOP
    if (pOp->szOpNm()->bSame("aggreg"_Stz)) {

      vSetupAggreg(pCombined, pOp);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupEosPayload(pCombined, pOp);
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
    // generate the code for the keyNotExistsOP
    if (pOp->szOpNm()->bSame("keyNotExists"_Stz)) {
      vSetupKeyExists(pCombined, pStream, pOp);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the sortOP
    if (pOp->szOpNm()->bSame("sort"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupSort(pCombined, pStream, pOp);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupEosPayload(pCombined, pOp);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the topOP
    if (pOp->szOpNm()->bSame("top"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupTop(pCombined, pStream, pOp);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vSetupEosPayload(pCombined, pOp);
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
/* return op stage name                                                       */
/*----------------------------------------------------------------------------*/
Stz* Stencil_v2::szStageNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL_V2,_FOOTER_);

  return (Stz*)m_szStageNm;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the next label for Op                                                  */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vGetNextLabel(Stz* sLabelNext, Stream* pStream, i8 i_Op, Stz* sDefault) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  i8 iNextOp = i_Op + 1;
  Op *pOpNext = pStream->pGet(iNextOp);

  if (pOpNext->szOpNm()->bSame("combined_v2"_Stz) || pOpNext->szOpNm()->bSame("hash"_Stz)) {
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
u8 Stencil_v2::uGetSelectColumns(Stream* pStream) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

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

    if (pOp->szOpNm()->bSame("aggreg_v2"_Stz) ||
        pOp->szOpNm()->bSame("project_v2"_Stz) ||
        pOp->szOpNm()->bSame("ordagg_v2"_Stz)) {
      LogicUnit* pLogicUnit = pOp->pOpLogicUnit();
      uSelectColumns |= pLogicUnit->m_uSelectColumns;
      bProject = true;
      break;
    }

    LogicUnit* pLogicUnit = pOp->pOpLogicUnit();
    uSelectColumns |= pLogicUnit->m_uSelectColumns;

    // break when we reach the combined OP
    if (pOp->szOpNm()->bSame("combined_v2"_Stz)) {
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
void Stencil_v2::vSetupAggreg(Combined* pCombined, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

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
void Stencil_v2::vSetupEosPayload(Combined* pCombined, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

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
/*----------------------------------------------------------------------------*/
void Stencil_v2::vSetupIndexJoin(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

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
void Stencil_v2::vSetupIndexScan(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

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
void Stencil_v2::vSetupKeyExists(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

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
void Stencil_v2::vSetupOrdAgg(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  bool bPayload = true;

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // create RowSingleAgg
  pCombined->aCombinedT()->m_aRowSingleAgg = new RowSingle(pStreamDefOut);
  pCombined->aCombinedT()->m_aRowCellArrayAgg =
      (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_aRowSingleAgg,
                                      &pCombined->aCombinedT()->m_racStrAreaAgg);
  pCombined->aCombinedT()->m_racStrAreaAggSav = pCombined->aCombinedT()->m_racStrAreaAgg;

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
void Stencil_v2::vSetupProjectAgg(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // create RowSingleAgg
  pCombined->aCombinedT()->m_aRowSingleAgg = new RowSingle(pStreamDefOut);
  pCombined->aCombinedT()->m_aRowCellArrayAgg =
      (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_aRowSingleAgg,
                                      &pCombined->aCombinedT()->m_racStrAreaAgg);
  pCombined->aCombinedT()->m_racStrAreaAggSav = pCombined->aCombinedT()->m_racStrAreaAgg;
}

/*----------------------------------------------------------------------------*/
/* set up for project expression                                              */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vSetupProjectExp(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // create RowSingleP
  pCombined->aCombinedT()->m_aRowSingleP = new RowSingle(pStreamDefOut);
  pCombined->aCombinedT()->m_aRowCellArrayP =
      (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_aRowSingleP,
                                      &pCombined->aCombinedT()->m_racStrAreaP);
  pCombined->aCombinedT()->m_racStrAreaPSav = pCombined->aCombinedT()->m_racStrAreaP;

}

/*----------------------------------------------------------------------------*/
/* set up for sort                                                            */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vSetupSort(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

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
  EntWrk::vEntDesc((EntWrk *) pCombined->aCombinedT()->m_aEntWrkPL,pIndexDef);

  // delete the indexdef in the LU of OP
  // not the correct place... will come back to it
  delete *pLogicUnit->m_xpIndexDef;
  pLogicUnit->m_xpIndexDef = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set up for scan                                                            */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vSetupScan(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

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
void Stencil_v2::vSetupTop(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

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
/* sketch code for Eos fun3 for Project agg OP                                */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vSketchEosProjectAgg(Combined* pCombined, Stream* pStream, Op* pOp, TypedStack* pInitStack) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  CodeSynth_v2 *pCodeSynth;
  s256 szStageNm;
  s256 sLabel;
  s256 sLabelNext;

  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append function header
  vAppendFuncHeader3();

  pCodeSynth = new CodeSynth_v2(pStreamDefOut, SR_AT);
  pCodeSynth->vSynthStruct(0);
  m_pShapeEOS->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payload to buffer
  pCodeSynth = new CodeSynth_v2(nullptr, pStreamDefOut, nullptr, pPreExpStack, SR_NONE, SR_PL);

  vGetNextLabel(sLabelNext, pStream, pOp->i_Op(), "L_AddRowToTubPot"_Stz);

  pCodeSynth->vSynthEos(nullptr, nullptr,sLabelNext);
  m_pShapeEOS->vCodeSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payload to Tub/Pot
  pCodeSynth = new CodeSynth_v2(pStreamDefOut, pStreamDefOut, nullptr, pPreExpStack, SR_AT, SR_NONE);

  Stz* sHavingExpr = ""_Stz;

  if (m_xStageOps.m_pHavingFilterOp) {
    // get filter expr from OP
    TagSet *pOpTags = m_xStageOps.m_pHavingFilterOp->pOpTags();
    sHavingExpr = pOpTags->szValue(C_TAG_EXPR);

    sLabel.vSprint("L_%s_%d"_Stz, m_xStageOps.m_pHavingFilterOp->szOpNm(), m_xStageOps.m_pHavingFilterOp->i_Op());
    vGetNextLabel(sLabelNext, pStream, m_xStageOps.m_pHavingFilterOp->i_Op(), "L_AddRowToTubPot"_Stz);
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
    pCodeSynth->vSynthAllocPotRow(nullptr, m_xStageOps.m_sHashExpr, true,
                                  "L_AddRowToTubPot"_Stz, "L_ReturnFinal"_Stz, ""_Stz,pInitStack);
  else
    pCodeSynth->vSynthAllocTubRow(nullptr, true, "L_AddRowToTubPot"_Stz,
                                  "L_ReturnFinal"_Stz,""_Stz, pInitStack);

  m_pShapeEOS->vCodeSynthB(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append concluding code
  Fragments::vAppendConcludingFunc3(m_pConcludingEos->azCode());
}

/*----------------------------------------------------------------------------*/
/* sketch code for Eos fun3 for Project agg OP                                */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vSketchEosProjectGby(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  CodeSynth_v2 *pCodeSynth;
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
  pCodeSynth = new CodeSynth_v2(pStreamDefOut,SR_PL);
  pCodeSynth->vSynthStruct(0);
  m_pShapeEOS->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payload to buffer
  pCodeSynth = new CodeSynth_v2(nullptr, pStreamDefOut, nullptr, pPreExpStack, SR_NONE, SR_PL);

  vGetNextLabel(sLabelNext, pStream, pOp->i_Op(), "L_AddRowToTubPot"_Stz);

  pCodeSynth->vSynthEos(pIndexDef, nullptr,sLabelNext);
  m_pShapeEOS->vCodeSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payloadRow to Tub/Pot
  pCodeSynth = new CodeSynth_v2(pStreamDefOut, pStreamDefOut, nullptr, pPreExpStack, SR_PL, SR_NONE);

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
    pCodeSynth->vSynthAllocPotRow(nullptr, m_xStageOps.m_sHashExpr, true, "L_AddRowToTubPot"_Stz, "L_AdvanceNextRow"_Stz, ""_Stz);
  else
    pCodeSynth->vSynthAllocTubRow(nullptr, true, "L_AddRowToTubPot"_Stz, "L_AdvanceNextRow"_Stz,""_Stz);

  m_pShapeEOS->vCodeSynthB(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append concluding code
  Fragments::vAppendConcludingFunc3(m_pConcludingEos->azCode());
}

/*----------------------------------------------------------------------------*/
/* sketch code for Eos fun3 for sort OP                                       */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vSketchEosSort(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  CodeSynth_v2 *pCodeSynth;
  s256 szStageNm;
  s256 sLabelNext;
  s256 sLabel;
  i8   i_Op;
  eSR ePrevStencil_v2 = SR_NONE;

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
  pCodeSynth = new CodeSynth_v2(pStreamDefOut,SR_PL);
  pCodeSynth->vSynthStruct(0);
  m_pShapeEOS->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payload to buffer
  pCodeSynth = new CodeSynth_v2(nullptr, pStreamDefOut, nullptr, pPreExpStack, SR_NONE, SR_PL);

  vGetNextLabel(sLabelNext, pStream, pOp->i_Op(), "L_AddRowToTubPot"_Stz);

  pCodeSynth->vSynthEos(pIndexDef, nullptr, sLabelNext);
  m_pShapeEOS->vCodeSynthA(pCodeSynth);

  Stz* sProjectExpr = ""_Stz;

  ePrevStencil_v2 = SR_PL;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payloadRow to Tub/Pot
  pCodeSynth = new CodeSynth_v2(pStreamDefOut, pStreamDefOut, nullptr, pPreExpStack, ePrevStencil_v2, SR_NONE);

  if (m_xStageOps.m_pLimitOp) {
    // get limit from limitOp
    TagSet* pOpTags = m_xStageOps.m_pLimitOp->pOpTags();
    i8 iLimit = StrLib::iToI8((char*)(pOpTags->szValue(C_TAG_COUNT)));

    sLabel.vSprint("L_%s_%d"_Stz, m_xStageOps.m_pLimitOp->szOpNm(), m_xStageOps.m_pLimitOp->i_Op());
    vGetNextLabel(sLabelNext, pStream, m_xStageOps.m_pLimitOp->i_Op(), "L_AddRowToTubPot"_Stz);
    pCodeSynth->vSynthLimit(iLimit, sLabel, sLabelNext, "L_ReturnFinal"_Stz);
  }

  if (pCombined->aCombinedT()->m_iHashRows)
   pCodeSynth->vSynthAllocPotRow(nullptr, m_xStageOps.m_sHashExpr, true, "L_AddRowToTubPot"_Stz, "L_AdvanceNextRow"_Stz, ""_Stz);
  else
    pCodeSynth->vSynthAllocTubRow(nullptr, false, "L_AddRowToTubPot"_Stz, "L_AdvanceNextRow"_Stz,""_Stz);

  m_pShapeEOS->vCodeSynthB(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append concluding code
  Fragments::vAppendConcludingFunc3(m_pConcludingEos->azCode());
}

/*----------------------------------------------------------------------------*/
/* sketch code for Eos fun3 for top OP                                        */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vSketchEosTop(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s256 sLabelNext;
  CodeSynth_v2 *pCodeSynth;

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
  pCodeSynth = new CodeSynth_v2(pStreamDefLft, SR_PL);
  pCodeSynth->vSynthStruct(0);
  m_pShapeEOS->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payload to buffer
  pCodeSynth = new CodeSynth_v2(nullptr, pStreamDefLft, nullptr, pPreExpStack, SR_NONE, SR_PL);

  vGetNextLabel(sLabelNext, pStream, pOp->i_Op(), "L_AddRowToTubPot"_Stz);

  pCodeSynth->vSynthEos(pIndexDef, nullptr, sLabelNext);
  m_pShapeEOS->vCodeSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payloadRow to Tub/Pot
  pCodeSynth = new CodeSynth_v2(pStreamDefOut, pStreamDefOut, nullptr, pPreExpStack, SR_PL, SR_NONE);

  if (pCombined->aCombinedT()->m_iHashRows)
    pCodeSynth->vSynthAllocPotRow(nullptr, m_xStageOps.m_sHashExpr, true, "L_AddRowToTubPot"_Stz, "L_AdvanceNextRow"_Stz, ""_Stz);
  else
    pCodeSynth->vSynthAllocTubRow(nullptr, true, "L_AddRowToTubPot"_Stz, "L_AdvanceNextRow"_Stz, ""_Stz);

  m_pShapeEOS->vCodeSynthB(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append concluding code
  Fragments::vAppendConcludingFunc3(m_pConcludingEos->azCode());
}

/*----------------------------------------------------------------------------*/
/* sketch code for indexJoin                                                  */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchIndexJoin(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil_v2, Stz* sLabel)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  CodeSynth_v2 *pCodeSynth;

  s256 sLabelNext;
  s256 sReturnLabel;

  Stz* sFilterExpr = ""_Stz;

  // get join expr from OP
  TagSet* pOpTags = pOp->pOpTags();
  Stz* sJoinExpr = pOpTags->szValue(C_TAG_KEYEXPR);

  // get filter expr if any from OP
  if (pOpTags->bHasName(C_TAG_EXPR)) {
    sFilterExpr = pOpTags->szValue(C_TAG_EXPR);
  }

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
  OpSketch *pShape = new OpSketch;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for struct for streamDefRht
  pCodeSynth = new CodeSynth_v2(pStreamDefRht, SR_IJ);
  pCodeSynth->vSynthStruct(0);
  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for struct for streamDefOut (join row)
  pCodeSynth = new CodeSynth_v2(pStreamDefOut, SR_TJ);
  pCodeSynth->vSynthStruct(0);
  pShape->vStructSynthB(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the join
  pCodeSynth = new CodeSynth_v2(pStreamDefLft, pStreamDefOut, pIndex->pIndexDef(), nullptr, ePrevStencil_v2, SR_TJ);

  vGetNextLabel(sLabelNext, pStream, pOp->i_Op(), "L_AddRowToTubPot"_Stz);
  pCodeSynth->vSynthIndexJoin(pStreamDefRht, pIndex->pIndexDef(), sFilterExpr, sJoinExpr,
      sLabel, sLabelNext, uSelectColumns);
  pShape->vCodeSynthA(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* get the next label for Op                                                  */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchIndexScan(Combined* pCombined, Stream* pStream, Op* pOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  OpSketch* pShape;
  CodeSynth_v2 *pCodeSynth;

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
  pShape = new OpSketch;

  u8 uSelectColumns = uGetSelectColumns(pStream);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for struct for streamDefOut (J)
  pCodeSynth = new CodeSynth_v2(pStreamDefOut, SR_IT);
  pCodeSynth->vSynthStruct(uSelectColumns);
  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the index scan
  pCodeSynth = new CodeSynth_v2(pStreamDefLft, pStreamDefOut, pIndexScan->pIndexDef(), nullptr, SR_IT, SR_NONE);
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
/* sketch inflate compact rows of crwBuffer)                                  */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchInflateCrw(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  OpSketch* pOpSketch;
  CodeSynth_v2 *pCodeSynth;

  s256 sLabel;
  s256 sLabelNext;
  s256 sLabelLoop;

  // get the streamDefLft
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // create a new shape for this OP
  pOpSketch = new OpSketch;

  // get the columns to be selected
  u8 uSelectColumns = uGetSelectColumns(pStream);

  // create a new opSketch for this OP
  pOpSketch = new OpSketch(""_Stz, SK_INFLATE_CRW, SR_NONE, SR_IT, pStreamDefLft, pStreamDefLft);

  sLabel.vSprint("L_InflateT"_Stz);
  sLabelNext.vSprint("L_InflateCrwRet"_Stz);
  sLabelLoop.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), pOp->i_Op());

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, sLabelLoop);

  // generate the stencil structure for this OP
  pOpSketch->vGenerateStencilToStruct();

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch inflate of row from rowBuffer                                       */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchInflateRow(Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  OpSketch *pShape;
  CodeSynth_v2 *pCodeSynth;

  s256 sLabel;

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // create a new shape for this OP
  pShape = new OpSketch;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDefLft
  pCodeSynth = new CodeSynth_v2(pStreamDefLft, SR_IT);
  pCodeSynth->vSynthStruct(0);
  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the row inflate
  pCodeSynth = new CodeSynth_v2(pStreamDefLft, nullptr, nullptr, nullptr, SR_NONE, SR_NONE);
  sLabel.vSprint("L_InflateT"_Stz);
  pCodeSynth->vSynthInflateRow(sLabel, m_xStageOps.m_sLabelOrdAgg);
  pShape->vCodeSynthA(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch filter                                                              */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchFilter(Stream* pStream, Op* pOp, eSR ePrevStencil_v2, Stz* sLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOp->i_Op();

  // create a new shape for this OP
  OpSketch *pShape = new OpSketch;

  // get filter expr from OP
  TagSet* pOpTags = pOp->pOpTags();
  Stz* sFilterExpr = pOpTags->szValue(C_TAG_EXPR);

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  Op *pNextOp = pStream->pGet(i_Op + 1);

  // create a new opSketch for this OP
  OpSketch *pOpSketch = new OpSketch(sFilterExpr, SK_FILTER, ePrevStencil_v2, SR_NONE, pStreamDefLft, pStreamDefOut);

  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStream, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, sLabelLoop);

  // generate the stencil structure for this OP
  pOpSketch->vGenerateStencilToStruct();

  // generate the expression stacks
  pOpSketch->vGenerateExprStacks(pPreExpStack);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch keyExists                                                           */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchKeyExists(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil_v2, bool bExists) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelEnd;
  s4K sLabelNext;
  s4K sLabelPrev;
  s4K sLabelLoop;

  i8 i_Op = pOp->i_Op();
  CodeSynth_v2 *pCodeSynth;
  //bool bLastFilter = false;

  // create a new shape for this OP
  OpSketch *pShape = new OpSketch;

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
  pCodeSynth = new CodeSynth_v2(pStreamDefRht, SR_IJ);
  pCodeSynth->vSynthStruct(uSelectColumns);
  pShape->vStructSynthA(pCodeSynth);

  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
  sLabelEnd.vSprint("L_%s_END_%d"_Stz, pOp->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStream, i_Op, "L_AdvanceNextRow"_Stz);

  pCodeSynth = new CodeSynth_v2(pStreamDefLft, pStreamDefLft, pIndex->pIndexDef(), pPreExpStack, SR_IJ, ePrevStencil_v2);
  pCodeSynth->vSynthKeyExists(pStreamDefRht, pIndex->pIndexDef(), sFilterExpr, sKeyExpr, sLabel, sLabelEnd, sLabelNext, uSelectColumns, bExists);
  pShape->vCodeSynthB(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch label                                                               */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchLabel(Combined* pCombined, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  CodeSynth_v2 *pCodeSynth;

  i8 i_Op = pOp->i_Op();

  // create a new shape for this OP
  OpSketch *pShape = new OpSketch;

  // form teh labels
  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStream, i_Op, "L_AddRowToTubPot"_Stz);

  // synthesize the code
  pCodeSynth = new CodeSynth_v2();
  pCodeSynth->vSynthLabel(pOp->szOpNm(), sLabel, sLabelNext);
  pShape->vCodeSynthB(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch limit                                                               */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchLimit(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil_v2, Stz* sLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOp->i_Op();
  CodeSynth_v2 *pCodeSynth;

  // create a new shape for this OP
  OpSketch *pShape = new OpSketch;

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
  pCodeSynth = new CodeSynth_v2(pStreamDefLft, pStreamDefLft, nullptr, pPreExpStack, ePrevStencil_v2, SR_NONE);
  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStream, i_Op, "L_AddRowToTubPot"_Stz);

  pCodeSynth->vSynthLimit(iLimit, sLabel, sLabelNext, sLabelLoop);
  pShape->vCodeSynthB(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch ordered aggregate                                                   */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchOrdAgg(Combined* pCombined, Stream* pStream, Op* pOp,
                              eSR ePrevStencil_v2, Stz* sLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  s4K sLabelPrev;

  i8 i_Op = pOp->i_Op();
  CodeSynth_v2 *pCodeSynth;

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
  OpSketch *pShape = new OpSketch;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDef (aggregate)
  pCodeSynth = new CodeSynth_v2(pStreamDefOut, SR_AT);
  pCodeSynth->vSynthStruct(0);
  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the row projection
  pCodeSynth = new CodeSynth_v2(pStreamDefLft, pStreamDefOut, pIndexDef, pPreExpStack, ePrevStencil_v2, SR_AT);

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
OpSketch* Stencil_v2::pSketchPrjAgg(Combined* pCombined, Stream* pStream, Op* pOp,
                                  eSR ePrevStencil_v2, Stz* sLabelLoop)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  s256 szStageNm;

  i8 i_Op = pOp->i_Op();
  CodeSynth_v2 *pCodeSynth;

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
  OpSketch *pShape = new OpSketch;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDef
  pCodeSynth = new CodeSynth_v2(pStreamDefOut, SR_AT);
  pCodeSynth->vSynthStruct(0);
  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the row projection
  pCodeSynth = new CodeSynth_v2(pStreamDefLft, pStreamDefOut, nullptr, pPreExpStack, ePrevStencil_v2, SR_AT);

  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStream, i_Op, "L_AdvanceNextRow"_Stz);

  TypedStack* pPostStack = pCodeSynth->pSynthPrjAgg(sProjectExpr,
                                                 sLabel,
                                                 sLabelLoop);

  pShape->vCodeSynthA(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch project expressions                                                 */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchProject(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil_v2, Stz* sLabelLoop)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOp->i_Op();

  // get project expr from OP
  TagSet* pOpTags = pOp->pOpTags();
  Stz* sProjectExpr = pOpTags->szValue(C_TAG_EXPR);

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // get the predicate expression stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // create a new opSketch for this OP
  OpSketch *pOpSketch = new OpSketch(sProjectExpr, SK_PROJECT, ePrevStencil_v2, SR_PT, pStreamDefLft, pStreamDefOut);

  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStream, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, sLabelLoop);

  // generate the expression stacks
  pOpSketch->vGenerateExprStacks(pPreExpStack);

  // generate the stencil structure for this OP
  pOpSketch->vGenerateStencilToStruct();

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch project group by expressions                                        */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchProjectGby(Combined* pCombined, Stream* pStream, Op* pOp,
                                  eSR ePrevStencil_v2, Stz* sLabelLoop)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOp->i_Op();
  CodeSynth_v2 *pCodeSynth;

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
  OpSketch *pShape = new OpSketch;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDef
  pCodeSynth = new CodeSynth_v2(pStreamDefOut, SR_AT);
  pCodeSynth->vSynthStruct(0);
  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the row projection
  pCodeSynth = new CodeSynth_v2(pStreamDefLft, pStreamDefOut, pIndexDef, pPreExpStack, ePrevStencil_v2, SR_AT);

  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);

  TypedStack* pPostStack = pCodeSynth->pSynthAggGby(sProjectExpr,
                                          sLabel,
                                          sLabelLoop);

  pShape->vCodeSynthA(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch sort                                                                */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchSort(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil_v2, Stz* szLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  s4K sLabelPrev;

  i8 i_Op = pOp->i_Op();
  CodeSynth_v2 *pCodeSynth;

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
  OpSketch *pShape = new OpSketch;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDef
//  pCodeSynth = new CodeSynth_v2(pStreamDefOut, ePrevStencil_v2);
//  pCodeSynth->vSynthStruct(0);
//  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the row projection
  pCodeSynth = new CodeSynth_v2(pStreamDefLft, pStreamDefOut, pIndexDef, pPreExpStack, ePrevStencil_v2, ePrevStencil_v2);

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
OpSketch* Stencil_v2::pSketchTop(Combined* pCombined, Op* pOp, eSR ePrevStencil_v2, Stz* sLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOp->i_Op();
  CodeSynth_v2 *pCodeSynth;

  // create a new shape for this OP
  OpSketch *pShape = new OpSketch;

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
  pCodeSynth = new CodeSynth_v2(pStreamDefLft, pStreamDefLft, pIndexDef, nullptr, ePrevStencil_v2, SR_NONE);
  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);

  pCodeSynth->vSynthTop(sKeyExpr, iTopCount, sLabel, sLabelLoop);
  pShape->vCodeSynthB(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch tub                                                                 */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchTub(OpSketch* pLastOp, Stz* sLabelLoop, eSR ePrevStencil_v2) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  // get the streamDefOut
  StreamDef *pStreamDefOut = pLastOp->m_pStreamDefOut;
  RASSERT(pStreamDefOut);

  // create a new opSketch
  OpSketch* pOpSketch = new OpSketch(""_Stz, SK_TUB, ePrevStencil_v2, SR_TB, pStreamDefOut, pStreamDefOut);

  sLabel = "L_AddRowToTubPot"_Stz;
  sLabelNext = ""_Stz;

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, sLabelLoop);

  // generate the stencil structure for this OP
  pOpSketch->vGenerateStencilToStruct();

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch return                                                              */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchReturn(void) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  // create a new shape
  OpSketch* pShape = new OpSketch;

  CodeSynth_v2* pCodeSynth = new CodeSynth_v2();

  //pCodeSynth->vSynthReturn();
  pShape->vCodeSynthB(pCodeSynth);
  return pShape;
}


/*----------------------------------------------------------------------------*/
/* Func header                                                                */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vAppendFuncHeader0(void) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

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
void Stencil_v2::vAppendFuncHeader1(void) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

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
void Stencil_v2::vAppendFuncHeader2(void) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

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
void Stencil_v2::vAppendFuncHeader3(void) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

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
void Stencil_v2::vAppendFuncEmpty(CodeSynth_v2* pCodeSynth, Stz* szDesc, Stz* szFunNm) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sFunSignature;

  sFunSignature.vSprint("\n  int %s_%s(struct Combined_T* aCombinedT)"_Stz, szFunNm, m_szStageNm.azChar());
  Fragments::vAppendEmptyFunc(pCodeSynth->azCode(), szDesc, sFunSignature);
}

/*----------------------------------------------------------------------------*/
/* Func header                                                                */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vAppendHeaderEntries(i8 iStage) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

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
eEC Stencil_v2::eCollectStage(Combined* pCombined, Stream* pStream) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

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
    if (pOp->szOpNm()->bSame("combined_v2"_Stz)) {

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

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // CANNOT HAVE INDEXJOIN OP wITH A KEYEXISTS OP
      if (m_xStageOps.m_iKeyExistsOp) {
        vTrace("Error: Cannot have indexJoinOP with an keyExistsOP (%s)!!!\n"_Stz,
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
      // MULTIPLE KEYEXISTS-OP's
      if (m_xStageOps.m_iKeyExistsOp) {
        vTrace("Error: Cannot have multiple keyExists/keyNotExists operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // CANNOT HAVE KEYEXISTS WITH AN INDEXJOIN OP
      if (m_xStageOps.m_iIndexJoinOp) {
        vTrace("Error: Cannot have keyExists with an indexJoinOP (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_iKeyExistsOp++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // keyNotExistsOp
    if (pOp->szOpNm()->bSame("keyNotExists"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE LIMIT-OP's
      if (m_xStageOps.m_iKeyExistsOp) {
        vTrace("Error: Cannot have multiple keyExists/keyNotExists operators in the same stage (%s)!!!\n"_Stz,
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
