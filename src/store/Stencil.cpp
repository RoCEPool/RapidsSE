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
    
  m_ePattern    = LP_NONE;

  m_pBannerHead  = new CodeSynth;
  m_pBannerTail  = new CodeSynth;
  m_pPreliminary = new CodeSynth;
  m_pPreliminaryEos = new CodeSynth;
  m_pConcluding     = new CodeSynth;
  m_pConcludingEos  = new CodeSynth;
  m_pOpHeader    = new CodeSynth;
  m_pOpSource    = new CodeSynth;
  m_pEOSShape    = new Shape;
  m_pEmptyFun0   = new CodeSynth;
  m_pEmptyFun1   = new CodeSynth;
  m_pEmptyFun2   = new CodeSynth;


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append banner head
  Fragments::vAppendBannerHead(m_pBannerHead->azCode());

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

  delete m_pBannerHead;
  delete m_pBannerTail;
  delete m_pPreliminary;
  delete m_pPreliminaryEos;
  delete m_pConcluding;
  delete m_pConcludingEos;
  delete m_pOpHeader;
  delete m_pOpSource;
  delete m_pEOSShape;
  delete m_pEmptyFun0;
  delete m_pEmptyFun1;
  delete m_pEmptyFun2;
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append banner head
  m_pOpSource->vAppendFragment(m_pBannerHead, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary
  m_pOpSource->vAppendFragment(m_pPreliminary, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append shapes
  for(i8 i_Shape = 0; i_Shape < nItems(); i_Shape++) {
    Shape* pShape = pGet(i_Shape);

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
  // append concluding
  m_pOpSource->vAppendFragment(m_pConcluding, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append empty Func0
  m_pOpSource->vAppendFragment(m_pEmptyFun0, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append empty Func1
  m_pOpSource->vAppendFragment(m_pEmptyFun1, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append empty Func2
  m_pOpSource->vAppendFragment(m_pEmptyFun2, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append eos shape

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary
  m_pOpSource->vAppendFragment(m_pPreliminaryEos, &iLineNumber);

  if (m_pEOSShape->pStructSynthA())
    m_pOpSource->vAppendFragment(m_pEOSShape->pStructSynthA(), &iLineNumber);

  if (m_pEOSShape->pStructSynthB())
    m_pOpSource->vAppendFragment(m_pEOSShape->pStructSynthB(), &iLineNumber);

  if (m_pEOSShape->pCodeSynthA())
    m_pOpSource->vAppendFragment(m_pEOSShape->pCodeSynthA(), &iLineNumber);

  if (m_pEOSShape->pCodeSynthB())
    m_pOpSource->vAppendFragment(m_pEOSShape->pCodeSynthB(), &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary
  m_pOpSource->vAppendFragment(m_pConcludingEos, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append banner tail
  m_pOpSource->vAppendFragment(m_pBannerTail, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append banner tail
  m_pOpHeader->vAppendFragment(m_pBannerTail);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // write to a file

  s256 sFileName;
  s256 funcName;
  LocalFile oFile;

  i8 iQueryNo = AM::iQueryNoInc();

  if (gxCmdLineArgs.m_xSysArgs.m_bCombined) {
    sFileName.vSprint("../combined/FlatQuery.h"_Stz, iQueryNo,
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

    sFileName.vSprint("../tmp/FlatQuery.h"_Stz, iQueryNo,
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
  s4K sLabelNext;
  s4K sLabelPrev;
  s4K sLabelLoop;

  s4K sFunSignature;

  bool bScan = false;
  Index* pIndexScan = nullptr;
  bool bInflateGenerated = false;
  bool bRowIteration = false;
  bool bIndexScan = false;
  bool bLastFilter = false;
  ePJ  eProjectKind = PJ_NONE;
  bool bHasOrdAgg = false;

  Op* pCombinedOp = nullptr;

  s256 szStageNm;
  s256 sHashExpr;
  szStageNm = pStream->szStageNm();
  Shape* pShape;

  i8 iStage = pStream->pStage()->iStage();
  i8 iOpsCombined = 0;

  eSR ePrevStencil;

  sLabelLoop = "L_AdvanceNextRow"_Stz;

  Tub* pTub = _pTub(pCombined->aCombinedT()->m_pTub);
  Pot* pPot = _pPot(pCombined->aCombinedT()->m_pPot);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // driving op indexScan ?
  if (pStream->pGet(1)->szOpNm()->bSame("indexScan"_Stz)) {
    bIndexScan = true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append banner head
  m_pOpHeader->vAppendFragment((Stz*)m_pBannerHead->azCode());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each operator in the stream
  for (i8 i_Op = 0; i_Op < pStream->nItems(); i_Op++) {

    // get the op
    Op *pOp = pStream->pGet(i_Op);

    // skip consumeOP
    if (pOp->szOpNm()->bSame("consumeOp"_Stz))
      continue;

    // skip scan OP
    if (pOp->szOpNm()->bSame("scan"_Stz)) {
      bScan = true;
      continue;
    }

    // break when we reach the combined OP
    if (pOp->szOpNm()->bSame("combined"_Stz)) {
      pCombinedOp = pOp;

      // driving op indexScan ?
      // set index in combinedOP LU
      if (pIndexScan) {
        LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
        pLogicUnit->m_xpIndex = pIndexScan;
      }
      break;
    }

    // get the tags
    TagSet* pOpTags = pOp->pOpTags();

    // get the streamDefOut and streamDefLft
    LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
    StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);
    StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for inflate (driving code)
    if ( bScan && ! bInflateGenerated ) { //&& ! bIndexScan) {

      pShape = pSketchInflateCrw(pCombined, pStream, pOp, szStageNm);

      // add the shape to the stencil
      vAppend(pShape);

      bInflateGenerated = true;
      ePrevStencil = SR_IT;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the rowBuffer inflate (driving code)
    if ( ! bScan && ! bRowIteration && ! bIndexScan) {

      pShape = pSketchInflateRow(pCombined, pOp, szStageNm);

      // add the shape to the stencil
      vAppend(pShape);

      bRowIteration = true;
      ePrevStencil = SR_IT;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the indexScanOp (driving code)
    if (pOp->szOpNm()->bSame("indexScan"_Stz)) {

      // there should be no RowBuffer or scan
      RASSERT( !bScan );
      RASSERT( !bInflateGenerated );
      RASSERT( !bRowIteration );

      RASSERT(bIndexScan);

      pShape = pSketchIndexScan(pCombined, pStream, pOp, szStageNm);

      // add the shape to the stencil
      vAppend(pShape);

      ePrevStencil = SR_IT;
    }

    iOpsCombined++;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the filterOP
    if (pOp->szOpNm()->bSame("filter"_Stz)) {

      // get filter expr from OP
      Stz* sFilterExpr = pOpTags->szValue(C_TAG_EXPR);

      // create a new shape for this OP
      Shape *pShape = new Shape;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the OP
      pCodeSynth = new CodeSynth;
      sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
      vGetNextLabel(sLabelNext, pStream, i_Op, "L_AdvanceNextRow"_Stz);

      Op* pNextOp = pStream->pGet(i_Op+1);
      if (pNextOp->szOpNm()->bSame("combined"_Stz) )
        bLastFilter = true;

      bool bPrevOp = false;
      Op* pPrevOp = pStream->pGet(i_Op-1);
      if (pPrevOp->szOpNm()->bSame("ordAgg"_Stz) ) {
        RASSERT(bLastFilter);
        sLabelPrev.vSprint("L_%s_%d"_Stz, pPrevOp->szOpNm(), i_Op-1);
        pCodeSynth->vSynthFilterOrdAgg(pStreamDefLft, sFilterExpr, ePrevStencil, sLabel, sLabelNext, sLabelPrev);
      }
      else {
        //RASSERT( ! bLastFilter );
        pCodeSynth->vSynthFilter(pStreamDefLft, sFilterExpr, ePrevStencil, sLabel, sLabelNext);
      }
      pShape->vCodeSynthB(pCodeSynth);

      // add the shape to the stencil
      vAppend(pShape);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the hashOP
    if (pOp->szOpNm()->bSame("hash"_Stz)) {
      pCombined->aCombinedT()->m_iHashRows = 1;

      // get hash expr from OP
      sHashExpr = pOpTags->szValue(C_TAG_EXPR);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the aggregOp / projectOP
    if (pOp->szOpNm()->bSame("aggreg"_Stz) ||
        pOp->szOpNm()->bSame("project"_Stz)) {

      // get the indexDef
      IndexDef *pIndexDef = _pIndexDef(*pLogicUnit->m_xpIndexDef);

      // get the aggregate type stack
      IntStack *pAggTypeStack = _pIntStack(*pLogicUnit->m_xpAggTypeStack);

      // get the project kind
      eProjectKind = pLogicUnit->m_eProjectKind;

      // get project expr from OP
      Stz* sProjectExpr = pOpTags->szValue(C_TAG_EXPR);

      // create a new shape for this OP
      Shape *pShape = new Shape;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the streamDef
      pCodeSynth = new CodeSynth;
      pCodeSynth->vSynthStruct(pStreamDefOut, 0, SR_PT);
      pShape->vStructSynthA(pCodeSynth);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the row projection
      pCodeSynth = new CodeSynth;

      // create the groupby index
      eKE eKind;
      pCombined->aCombinedT()->m_aIndexDefGby = new IndexDef(pIndexDef);
      pCombined->aCombinedT()->m_aIndexTreeGby = IndexTree::aNewIndexTree(pIndexDef, &eKind);
      EntWrk::vEntKind((EntWrk*)pCombined->aCombinedT()->m_aEntWrkPL,1,pIndexDef->eKeyKind());

      sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
      if (eProjectKind == PJ_AGG || eProjectKind == PJ_GBY)
        vGetNextLabel(sLabelNext, pStream, i_Op, "L_AdvanceNextRow"_Stz);
      else
        vGetNextLabel(sLabelNext, pStream, i_Op, "L_AddRowToTub"_Stz);

      pCodeSynth->vSynthProject(pStreamDefLft, pStreamDefOut, pIndexDef,
                                ePrevStencil, SR_PT, eProjectKind,
                                pAggTypeStack, sProjectExpr,
                                sLabel,
                                sLabelNext);
      pShape->vCodeSynthA(pCodeSynth);

      if (eProjectKind == PJ_EXP) {
        // create RowSingleP
        pCombined->aCombinedT()->m_aRowSingleP = new RowSingle(pStreamDefOut);
        pCombined->aCombinedT()->m_aRowCellArrayP =
            (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_aRowSingleP,
                                            &pCombined->aCombinedT()->m_racStrAreaP);
        pCombined->aCombinedT()->m_racStrAreaSavP = pCombined->aCombinedT()->m_racStrAreaP;
      }
      else if (eProjectKind == PJ_AGG) {
        // create RowSingleP
        pCombined->aCombinedT()->m_aRowSingleAgg = new RowSingle(pStreamDefOut);
        pCombined->aCombinedT()->m_aRowCellArrayAgg =
            (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_aRowSingleAgg,
                                            &pCombined->aCombinedT()->m_racStrAreaAgg);
      }
      if (eProjectKind == PJ_GBY || eProjectKind == PJ_AGG) {

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // append code for EOSMARKER handling

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // form signature for fun0
        sFunSignature.vSprint("\n  int iFun3_%s(struct Combined_T* aCombinedT)"_Stz, szStageNm.azChar());

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // append function signatures
        //m_pOpHeader->vAppendFragmentSemiColon(sFunSignature);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // append preliminary code
        Fragments::vAppendPreliminaryFunc3(m_pPreliminaryEos->azCode(), sFunSignature);

        if (eProjectKind == PJ_GBY) {
          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          // generate the code for the streamDef
          pCodeSynth = new CodeSynth;
          pCodeSynth->vSynthStruct(pStreamDefOut, 0, SR_PT);
          m_pEOSShape->vStructSynthA(pCodeSynth);

          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          // payload to buffer
          pCodeSynth = new CodeSynth;
          pCodeSynth->vSynthEos(pStreamDefOut, pIndexDef, SR_PT, pAggTypeStack);
          m_pEOSShape->vCodeSynthA(pCodeSynth);
        }

        else if (eProjectKind == PJ_AGG) {

          pCodeSynth = new CodeSynth;
          pCodeSynth->vSynthStruct(pStreamDefOut, 0, SR_PT);
          m_pEOSShape->vStructSynthA(pCodeSynth);

          pCodeSynth = new CodeSynth;
          if (pCombined->aCombinedT()->m_iHashRows) {
            pCodeSynth->vSynthAllocPotRow(pStreamDefOut, sHashExpr, eProjectKind, SR_PT, true, sLabel, "L_ReturnFinal"_Stz, sReturnLabel);
          }
          else
            pCodeSynth->vSynthAllocTubRow(pStreamDefOut, eProjectKind, SR_PT, true, sLabel, "L_ReturnFinal"_Stz, sReturnLabel);
          m_pEOSShape->vCodeSynthA(pCodeSynth);
        }

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // append concluding code
        Fragments::vAppendConcludingFunc3(m_pConcludingEos->azCode());

      }

      // delete the indexdef in the LU of OP
      // not the correct place... will come back to it
      delete *pLogicUnit->m_xpIndexDef;
      pLogicUnit->m_xpIndexDef = nullptr;

      // delete the aggregate type stack in the LU of OP
      // not the correct place... will come back to it
      delete pAggTypeStack;
      pLogicUnit->m_xpAggTypeStack = nullptr;

      // add the shape to the stencil
      vAppend(pShape);

      ePrevStencil = SR_PT;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the aggOrdOp
    if (pOp->szOpNm()->bSame("ordAgg"_Stz)) {

      // get the indexDef
      IndexDef *pIndexDef = _pIndexDef(*pLogicUnit->m_xpIndexDef);

      // get the aggregate type stack
      IntStack *pAggTypeStack = _pIntStack(*pLogicUnit->m_xpAggTypeStack);

      // get the project kind
      // not to set eProjectKind else fun3 for eos marker gets created
      // for payload traversal.
      // not needed for ordagg
      eProjectKind = pLogicUnit->m_eProjectKind;

      bHasOrdAgg = true;

      // get project expr from OP
      Stz* sProjectExpr = pOpTags->szValue(C_TAG_EXPR);

      // create a new shape for this OP
      Shape *pShape = new Shape;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the streamDef
      pCodeSynth = new CodeSynth;
      pCodeSynth->vSynthStruct(pStreamDefOut, 0, SR_PT);
      pShape->vStructSynthA(pCodeSynth);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the row projection
      pCodeSynth = new CodeSynth;

      // create RowSingleAgg
      pCombined->aCombinedT()->m_aRowSingleAgg = new RowSingle(pStreamDefOut);
      pCombined->aCombinedT()->m_aRowCellArrayAgg =
          (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_aRowSingleAgg,
                                          &pCombined->aCombinedT()->m_racStrAreaAgg);
      // create the groupby index
      eKE eKind;
      pCombined->aCombinedT()->m_aIndexDefGby = new IndexDef(pIndexDef);
      pCombined->aCombinedT()->m_aIndexTreeGby = IndexTree::aNewIndexTree(pIndexDef, &eKind);
      EntWrk::vEntKind((EntWrk*)pCombined->aCombinedT()->m_aEntWrkPL,1,pIndexDef->eKeyKind());

      sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
      vGetNextLabel(sLabelNext, pStream, i_Op, "L_AddRowToTub"_Stz);
      pCodeSynth->vSynthOrdAgg(pStreamDefLft, pStreamDefOut, pIndexDef,
                                SR_IT, SR_PT, pLogicUnit->m_eProjectKind,
                                pAggTypeStack, sProjectExpr,
                                sLabel,
                                sLabelNext);
      pShape->vCodeSynthA(pCodeSynth);

      sLabelLoop = sLabel;
      sReturnLabel = sLabelNext;

      // delete the indexdef in the LU of OP
      // not the correct place... will come back to it
      delete *pLogicUnit->m_xpIndexDef;
      pLogicUnit->m_xpIndexDef = nullptr;

      // delete the aggregate type stack in the LU of OP
      // not the correct place... will come back to it
      delete pAggTypeStack;
      pLogicUnit->m_xpAggTypeStack = nullptr;

      // add the shape to the stencil
      vAppend(pShape);

      ePrevStencil = SR_PT;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the indexJoinOp
    if (pOp->szOpNm()->bSame("indexJoin"_Stz)) {

      StreamDef *pStreamDefRht = _pStreamDef(*pLogicUnit->m_xpStreamDefRht);

      // get the joinIndex from the indexJoinOp
      Index* pIndex = _pIndex(*pLogicUnit->m_xpIndex);
      pCombined->aCombinedT()->m_aIndexJ = pIndex;

      // create RowSingleJ
      pCombined->aCombinedT()->m_aRowSingleJ = new RowSingle(pStreamDefRht);

      // get the index tree for the index
      Table* pTable = pIndex->pTable();

      PtnId tPtn = pOp->tPtnId();
      i8 i_TablePtn = pTable->iPtnNoToTablePtn(tPtn);
      TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);

      i8 iIndexNo = pIndex->iIndexNo();

      IndexTree* pIndexTree = pTablePtn->pIndexTree(iIndexNo);
      pCombined->aCombinedT()->m_aIndexTreeJ = pIndexTree;
      pCombined->aCombinedT()->m_aIndexDefJ  = pIndex->pIndexDef();

      // get join expr from OP
      Stz* sJoinExpr = pOpTags->szValue(C_TAG_EXPR);

      // create a new shape for this OP
      Shape *pShape = new Shape;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for struct for streamDefRht
      pCodeSynth = new CodeSynth;
      pCodeSynth->vSynthStruct(pStreamDefRht, 0, SR_IJ);
      pShape->vStructSynthA(pCodeSynth);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for struct for streamDefOut (join row)
      pCodeSynth = new CodeSynth;
      pCodeSynth->vSynthStruct(pStreamDefOut, 0, SR_TJ);
      pShape->vStructSynthB(pCodeSynth);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the join
      pCodeSynth = new CodeSynth;
      sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
      vGetNextLabel(sLabelNext, pStream, pOp->iOp(), "L_AdvanceNextRowJ"_Stz);
      pCodeSynth->vSynthIndexJoin(pStreamDefOut, pStreamDefLft, pStreamDefRht,
                                  ePrevStencil, SR_IJ, SR_TJ,
                                  pIndex->pIndexDef(), sJoinExpr, sLabel, sLabelNext);
      pShape->vCodeSynthA(pCodeSynth);

      sLabelLoop = sLabelNext;

      // add the shape to the stencil
      vAppend(pShape);


      // create RowSingleJoin
      pCombined->aCombinedT()->m_aRowSingleTJ = new RowSingle(pStreamDefOut);
      pCombined->aCombinedT()->m_aRowCellArrayTJ =
          (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_aRowSingleTJ,
                                          &pCombined->aCombinedT()->m_racStrAreaTJ);
      pCombined->aCombinedT()->m_racStrAreaTJSav = pCombined->aCombinedT()->m_racStrAreaTJ;

      ePrevStencil = SR_TJ;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // any ops combined ?
  if (iOpsCombined == 0) {
    vTrace("No Operators to combine!!!\n"_Stz);
    return EC_INVALID_COMMAND;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // synthesize code for allotting final tub row/ pot row
  Op* pLastOp = pStream->pGet(pCombinedOp->iOp() - 1);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // synthesize code for allotting tub row

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the tub

    LogicUnit *pLogicUnit = pLastOp->pOpLogicUnit();
    StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

    RASSERT(pStreamDefOut);

    // get the row
    RowPix* pRowPix = pStreamDefOut->pRowPix();

    if (pCombined->aCombinedT()->m_iHashRows) {
      pPot->vRowPix(pRowPix);
    } else {
      pTub->vRowPix(pRowPix);
    }

    if ( ! (pLastOp->szOpNm()->bSame("project"_Stz) && eProjectKind == PJ_AGG) &&
       ! (pLastOp->szOpNm()->bSame("aggreg"_Stz) && eProjectKind == PJ_GBY) ) {

      // create a new shape
      pShape = new Shape;

      pCodeSynth = new CodeSynth;

      sLabel.vSprint("L_AddRowToTub"_Stz);
      sLabelNext.vSprint(sLabelLoop);

      if (pCombined->aCombinedT()->m_iHashRows) {
        pCodeSynth->vSynthAllocPotRow(pStreamDefOut, sHashExpr, eProjectKind, ePrevStencil, false, sLabel, sLabelNext,
                                      sReturnLabel);
      } else {
        pCodeSynth->vSynthAllocTubRow(pStreamDefOut, eProjectKind, ePrevStencil, false, sLabel, sLabelNext, sReturnLabel);
      }

      pShape->vCodeSynthB(pCodeSynth);

      // add the shape to the stencil
      vAppend(pShape);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append concluding code
  if (bIndexScan) {
    if ( bHasOrdAgg)
      Fragments::vAppendConcludingFunc2_OrdAgg(m_pConcluding->azCode());
    else
      Fragments::vAppendConcludingFunc2(m_pConcluding->azCode());

    sFunSignature.vSprint("\n  int iFun0_%s(struct Combined_T* aCombinedT)"_Stz, szStageNm.azChar());
    Fragments::vAppendEmptyFunc(m_pEmptyFun0->azCode(), sFunSignature);
    m_pOpHeader->vAppendFragmentSemiColon(sFunSignature);

    sFunSignature.vSprint("\n  int iFun1_%s(struct Combined_T* aCombinedT)"_Stz, szStageNm.azChar());
    Fragments::vAppendEmptyFunc(m_pEmptyFun1->azCode(), sFunSignature);
    m_pOpHeader->vAppendFragmentSemiColon(sFunSignature);
  }
  else if (bInflateGenerated) {

//    if ( (pLastOp->szOpNm()->bSame("project"_Stz) && eProjectKind == PJ_AGG) ||
//         (pLastOp->szOpNm()->bSame("aggreg"_Stz) && eProjectKind == PJ_GBY) ) {
//      Fragments::vAppendConcludingFunc0_agg(m_pConcluding->azCode());
//    }
    //else if ( !bLastFilter)
    //  Fragments::vAppendConcludingFunc0(m_pConcluding->azCode());

    if (bHasOrdAgg)
      Fragments::vAppendConcludingFunc0_OrdAgg(m_pConcluding->azCode());
    else {
      Fragments::vAppendConcludingFunc0(m_pConcluding->azCode());
    }

    sFunSignature.vSprint("\n  int iFun1_%s(struct Combined_T* aCombinedT)"_Stz, szStageNm.azChar());
    Fragments::vAppendEmptyFunc(m_pEmptyFun1->azCode(), sFunSignature);
    m_pOpHeader->vAppendFragmentSemiColon(sFunSignature);

    sFunSignature.vSprint("\n  int iFun2_%s(struct Combined_T* aCombinedT)"_Stz, szStageNm.azChar());
    Fragments::vAppendEmptyFunc(m_pEmptyFun2->azCode(), sFunSignature);
    m_pOpHeader->vAppendFragmentSemiColon(sFunSignature);
  }
  else {
//    if ( (pLastOp->szOpNm()->bSame("project"_Stz) && eProjectKind == PJ_AGG) ||
//         (pLastOp->szOpNm()->bSame("aggreg"_Stz) && eProjectKind == PJ_GBY) ) {
//      Fragments::vAppendConcludingFunc1_Agg(m_pConcluding->azCode());
//    }
//    else if ( !bLastFilter)

    if (bHasOrdAgg)
      Fragments::vAppendConcludingFunc1_OrdAgg(m_pConcluding->azCode());
    else {
      Fragments::vAppendConcludingFunc1(m_pConcluding->azCode());
    }

    sFunSignature.vSprint("\n  int iFun0_%s(struct Combined_T* aCombinedT)"_Stz, szStageNm.azChar());
    Fragments::vAppendEmptyFunc(m_pEmptyFun0->azCode(), sFunSignature);
    m_pOpHeader->vAppendFragmentSemiColon(sFunSignature);

    sFunSignature.vSprint("\n  int iFun2_%s(struct Combined_T* aCombinedT)"_Stz, szStageNm.azChar());
    Fragments::vAppendEmptyFunc(m_pEmptyFun2->azCode(), sFunSignature);
    m_pOpHeader->vAppendFragmentSemiColon(sFunSignature);
  }

  if ( (eProjectKind != PJ_GBY && eProjectKind != PJ_AGG) || bHasOrdAgg) {

    pCodeSynth = new CodeSynth;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // append code for EOSMARKER handling

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // form signature for fun3
    sFunSignature.vSprint("\n  int iFun3_%s(struct Combined_T* aCombinedT)"_Stz, szStageNm.azChar());

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // append function signatures
    m_pOpHeader->vAppendFragmentSemiColon(sFunSignature);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // append preliminary code
    Fragments::vAppendPreliminaryFunc3(m_pPreliminaryEos->azCode(), sFunSignature);


    Fragments::vAppendConcludingFunc3(m_pConcludingEos->azCode());

    m_pEOSShape->vCodeSynthA(pCodeSynth);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form signature for fun0
  sFunSignature.vSprint("\n\n  int (*stage%lld[4])(struct Combined_T* aCombinedT) = {"_Stz, iStage);
  m_pOpHeader->vAppendFragment(sFunSignature);

  sFunSignature.vSprint("\n    iFun0_%s,"_Stz, szStageNm.azChar());
  m_pOpHeader->vAppendFragment(sFunSignature);

  sFunSignature.vSprint("\n    iFun1_%s,"_Stz, szStageNm.azChar());
  m_pOpHeader->vAppendFragment(sFunSignature);

  sFunSignature.vSprint("\n    iFun2_%s,"_Stz, szStageNm.azChar());
  m_pOpHeader->vAppendFragment(sFunSignature);

  sFunSignature.vSprint("\n    iFun3_%s,"_Stz, szStageNm.azChar());
  m_pOpHeader->vAppendFragment(sFunSignature);

  sFunSignature.vSprint("\n  };"_Stz);
  m_pOpHeader->vAppendFragment(sFunSignature);

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* setup the combinedOp                                                       */
/*----------------------------------------------------------------------------*/
void Stencil::vSetup(Combined* pCombined, Stream* pStream)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  CodeSynth *pCodeSynth;

  bool bScan = false;
  Index* pIndexScan = nullptr;
  bool bInflateGenerated = false;
  bool bRowIteration = false;
  bool bIndexScan = false;
  ePJ  eProjectKind = PJ_NONE;

  Op* pCombinedOp = nullptr;

  Tub* pTub = _pTub(pCombined->aCombinedT()->m_pTub);
  Pot* pPot = _pPot(pCombined->aCombinedT()->m_pPot);

  if (pStream->pGet(1)->szOpNm()->bSame("indexScan"_Stz)) {
    bIndexScan = true;
  }

  // for each operator in the stream
  for (i8 i_Op = 0; i_Op < pStream->nItems(); i_Op++) {

    // get the op
    Op *pOp = pStream->pGet(i_Op);

    // skip consumeOP
    if (pOp->szOpNm()->bSame("consumeOp"_Stz))
      continue;

    // skip scan OP
    if (pOp->szOpNm()->bSame("scan"_Stz)) {
      bScan = true;
      continue;
    }

    // break when we reach the combined OP
    if (pOp->szOpNm()->bSame("combined"_Stz)) {
      pCombinedOp = pOp;
      if (pIndexScan) {
        LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
        pLogicUnit->m_xpIndex = pIndexScan;
      }
      break;
    }

    // get the streamDefOut
    LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
    StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);
    StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for inflate (for first OP)
    if ( bScan && ! bInflateGenerated && ! bIndexScan) {

      pCombined->aCombinedT()->m_nCols = pStreamDefLft->nColumns();
      pCombined->aCombinedT()->m_pRowSingleT = new RowSingle(pStreamDefLft);
      pCombined->aCombinedT()->m_aRowCellArrayT =
          (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_pRowSingleT,
                                         &pCombined->aCombinedT()->m_racStrAreaT);
      pCombined->aCombinedT()->m_racStrAreaTSav = pCombined->aCombinedT()->m_racStrAreaT;

      bInflateGenerated = true;
    }

    if ( ! bScan && ! bRowIteration && ! bIndexScan) {
      // func1
      // row buffer processing

      bRowIteration = true;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the filterOP
    if (pOp->szOpNm()->bSame("filter"_Stz)) {
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the hashOP
    if (pOp->szOpNm()->bSame("hash"_Stz)) {
      pCombined->aCombinedT()->m_iHashRows = 1;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the aggregOp / projectOP
    if (pOp->szOpNm()->bSame("aggreg"_Stz) ||
        pOp->szOpNm()->bSame("project"_Stz)) {

      // get the project kind
      eProjectKind = pLogicUnit->m_eProjectKind;

      if (eProjectKind == PJ_EXP) {
        // create RowSingleP
        pCombined->aCombinedT()->m_aRowSingleP = new RowSingle(pStreamDefOut);
        pCombined->aCombinedT()->m_aRowCellArrayP =
            (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_aRowSingleP,
                                            &pCombined->aCombinedT()->m_racStrAreaP);
        pCombined->aCombinedT()->m_racStrAreaSavP = pCombined->aCombinedT()->m_racStrAreaP;
      }
      else if (eProjectKind == PJ_AGG) {
        // create RowSingleAgg
        pCombined->aCombinedT()->m_aRowSingleAgg = new RowSingle(pStreamDefOut);
        pCombined->aCombinedT()->m_aRowCellArrayAgg =
            (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_aRowSingleAgg,
                                            &pCombined->aCombinedT()->m_racStrAreaAgg);
      }
      // get the indexDef
      IndexDef *pIndexDef = _pIndexDef(*pLogicUnit->m_xpIndexDef);

      // create the groupby index
      eKE eKind;
      pCombined->aCombinedT()->m_aIndexDefGby = new IndexDef(pIndexDef);
      pCombined->aCombinedT()->m_aIndexTreeGby = IndexTree::aNewIndexTree(pIndexDef, &eKind);
      EntWrk::vEntKind((EntWrk*)pCombined->aCombinedT()->m_aEntWrkPL,1,pIndexDef->eKeyKind());

      // delete the indexdef in the LU of OP
      // not the correct place... will come back to it
      delete *pLogicUnit->m_xpIndexDef;
      pLogicUnit->m_xpIndexDef = nullptr;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the ordAgg
    if (pOp->szOpNm()->bSame("ordAgg"_Stz)) {

      // create RowSingleAgg
      pCombined->aCombinedT()->m_aRowSingleAgg = new RowSingle(pStreamDefOut);
      pCombined->aCombinedT()->m_aRowCellArrayAgg =
          (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_aRowSingleAgg,
                                          &pCombined->aCombinedT()->m_racStrAreaAgg);

      // get the indexDef
      IndexDef *pIndexDef = _pIndexDef(*pLogicUnit->m_xpIndexDef);

      // create the groupby index
      eKE eKind;
      pCombined->aCombinedT()->m_aIndexDefGby = new IndexDef(pIndexDef);
      pCombined->aCombinedT()->m_aIndexTreeGby = IndexTree::aNewIndexTree(pIndexDef, &eKind);
      EntWrk::vEntKind((EntWrk*)pCombined->aCombinedT()->m_aEntWrkPL,1,pIndexDef->eKeyKind());

      // delete the indexdef in the LU of OP
      // not the correct place... will come back to it
      delete *pLogicUnit->m_xpIndexDef;
      pLogicUnit->m_xpIndexDef = nullptr;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the indexJoinOp
    if (pOp->szOpNm()->bSame("indexJoin"_Stz)) {

      StreamDef *pStreamDefRht = _pStreamDef(*pLogicUnit->m_xpStreamDefRht);

      // get the joinIndex from the indexJoinOp
      Index* pIndex = _pIndex(*pLogicUnit->m_xpIndex);
      pCombined->aCombinedT()->m_aIndexJ = pIndex;

      // create RowSingleJ
      pCombined->aCombinedT()->m_aRowSingleJ = new RowSingle(pStreamDefRht);

      // get the index tree for the index
      Table* pTable = pIndex->pTable();

      PtnId tPtn = pOp->tPtnId();
      i8 i_TablePtn = pTable->iPtnNoToTablePtn(tPtn);
      TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);

      i8 iIndexNo = pIndex->iIndexNo();

      IndexTree* pIndexTree = pTablePtn->pIndexTree(iIndexNo);
      pCombined->aCombinedT()->m_aIndexTreeJ = pIndexTree;
      pCombined->aCombinedT()->m_aIndexDefJ  = pIndex->pIndexDef();

      // create RowSingleJoin
      pCombined->aCombinedT()->m_aRowSingleTJ = new RowSingle(pStreamDefOut);
      pCombined->aCombinedT()->m_aRowCellArrayTJ =
          (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_aRowSingleTJ,
                                          &pCombined->aCombinedT()->m_racStrAreaTJ);
      pCombined->aCombinedT()->m_racStrAreaTJSav = pCombined->aCombinedT()->m_racStrAreaTJ;

    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the indexScanOp
    if (pOp->szOpNm()->bSame("indexScan"_Stz)) {

      // there should be no RowBuffer or scan
      RASSERT( !bScan );
      RASSERT( !bInflateGenerated );
      RASSERT( !bRowIteration );

      RASSERT(bIndexScan);

      // indexScan should be the second op after consume
      RASSERT(i_Op == 1);

      // get the scanIndex from the indexScanOp
      pIndexScan = _pIndex(*pLogicUnit->m_xpIndex);
      pCombined->aCombinedT()->m_aIndexT = pIndexScan;

      // create RowSingleT
      pCombined->aCombinedT()->m_pRowSingleT = new RowSingle(pStreamDefLft);
      pCombined->aCombinedT()->m_aRowCellArrayT =
          (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_pRowSingleT,
                                          &pCombined->aCombinedT()->m_racStrAreaT);
      pCombined->aCombinedT()->m_racStrAreaTSav = pCombined->aCombinedT()->m_racStrAreaT;

      // get the index tree for the index
      Table* pTable = pIndexScan->pTable();

      PtnId tPtn = pOp->tPtnId();
      i8 i_TablePtn = pTable->iPtnNoToTablePtn(tPtn);
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

  }

  Op* pLastOp = pStream->pGet(pCombinedOp->iOp() - 1);
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

  if (iNextOp == pStream->nItems() - 2 || pOpNext->szOpNm()->bSame("hash"_Stz)) {
    // if nextOp is combined, then next label is sDefault
    //ASSERT(pStream->pGet(iNextOp)->szOpNm()->bSame("combined"_Stz));
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
u8 Stencil::uGetSelectColumns(Combined* pCombined, Stream* pStream, Op* pOp) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  bool bProject = false;
  u8 uSelectColumns = 0;

  // for each operator in the stream
  for (i8 i_Op = 0; i_Op < pStream->nItems(); i_Op++) {

    // get the op
    Op *pOp = pStream->pGet(i_Op);

    if (pOp->szOpNm()->bSame("aggreg"_Stz) ||
        pOp->szOpNm()->bSame("project"_Stz) ||
        pOp->szOpNm()->bSame("ordagg"_Stz) ||
        pOp->szOpNm()->bSame("indexJoin"_Stz)) {
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

  return uSelectColumns;
}

/*----------------------------------------------------------------------------*/
/* get the next label for Op                                                  */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchIndexScan(Combined* pCombined, Stream* pStream, Op* pOp, Stz* szStageNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  Shape* pShape;
  CodeSynth *pCodeSynth;
  Index* pIndexScan;

  s256 sFunSignature;
  s256 sLabel;
  s256 sLabelNext;
  s256 sReturnLabel;

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // get the tags
  TagSet* pOpTags = pOp->pOpTags();

  Tub* pTub = _pTub(pCombined->aCombinedT()->m_pTub);
  Pot* pPot = _pPot(pCombined->aCombinedT()->m_pPot);

  sReturnLabel = ""_Stz;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form signature for fun0
  sFunSignature.vSprint("\n  int iFun2_%s(struct Combined_T* aCombinedT)"_Stz, szStageNm->azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append function signatures
  m_pOpHeader->vAppendFragmentSemiColon(sFunSignature);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary code
  Fragments::vAppendPreliminaryFunc2(m_pPreliminary->azCode(), sFunSignature);

  // get the scanIndex from the indexScanOp
  pIndexScan = _pIndex(*pLogicUnit->m_xpIndex);
  pCombined->aCombinedT()->m_aIndexT = pIndexScan;

  // create RowSingleT
  pCombined->aCombinedT()->m_pRowSingleT = new RowSingle(pStreamDefLft);
  pCombined->aCombinedT()->m_aRowCellArrayT =
      (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_pRowSingleT,
                                     &pCombined->aCombinedT()->m_racStrAreaT);
  pCombined->aCombinedT()->m_racStrAreaTSav = pCombined->aCombinedT()->m_racStrAreaT;

  // get the index tree for the index
  Table* pTable = pIndexScan->pTable();

  PtnId tPtn = pOp->tPtnId();
  i8 i_TablePtn = pTable->iPtnNoToTablePtn(tPtn);
  TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);

  pCombined->aCombinedT()->m_nCols = pTable->pStreamDef()->nColumns();

  i8 iIndexNo = pIndexScan->iIndexNo();

  // COME BACK TO THIS
  IndexTree* pIndexTree = pTablePtn->pIndexTree(iIndexNo);
  IndexDef* pIndexDef = pIndexTree->pIndexDef();
  pCombined->aCombinedT()->m_aIndexTreeT = pIndexTree;
  pCombined->aCombinedT()->m_aIndexDefT  = pIndexDef;
  EntWrk::vEntKind((EntWrk*)pCombined->aCombinedT()->m_aEntWrkT, 0, pIndexDef->eKeyKind());

  // get indexScan expr from OP
  Stz* sIndexScanExpr = pOpTags->szValue(C_TAG_EXPR);

  // create a new shape for this OP
  pShape = new Shape;

  u8 uSelectColumns = uGetSelectColumns(pCombined, pStream, pOp);
/*
  u8 uSelectColumns = 0;

  // for each operator in the stream
  for (i8 i_Op = 0; i_Op < pStream->nItems(); i_Op++) {

    // get the op
    Op *pOp = pStream->pGet(i_Op);

    if (pOp->szOpNm()->bSame("indexJoin"_Stz)) {
      uSelectColumns = 0xFFFFFFFFFFFFFFFFLL;
    }

    // break when we reach the combined OP
    if (pOp->szOpNm()->bSame("combined"_Stz)) {
      break;
    }
    pLogicUnit = pOp->pOpLogicUnit();
    uSelectColumns |= pLogicUnit->m_uSelectColumns;
  }
*/
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for struct for streamDefOut (J)
  pCodeSynth = new CodeSynth;
  pCodeSynth->vSynthStruct(pStreamDefOut, uSelectColumns, SR_IT);
  pShape->vStructSynthA(pCodeSynth);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the index scan
  pCodeSynth = new CodeSynth;
  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), pOp->iOp());
  vGetNextLabel(sLabelNext, pStream, pOp->iOp(), "L_AdvanceNextRow"_Stz);
  pCodeSynth->vSynthIndexScan(pStreamDefOut, pStreamDefLft, pIndexScan->pIndexDef(),
      SR_IT, sIndexScanExpr, sLabel, sLabelNext, &uSelectColumns);
  pShape->vCodeSynthA(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch inflate crw of crwBuffer                                            */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchInflateCrw(Combined* pCombined, Stream* pStream, Op* pOp, Stz* szStageNm) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  Shape* pShape;
  CodeSynth *pCodeSynth;

  s256 sFunSignature;
  s256 sLabel;
  s256 sReturnLabel;

  // get the streamDefLft
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  pCombined->aCombinedT()->m_nCols = pStreamDefLft->nColumns();
  pCombined->aCombinedT()->m_pRowSingleT = new RowSingle(pStreamDefLft);
  pCombined->aCombinedT()->m_aRowCellArrayT =
      (byte*)RowSingle::aGetCellArray((RowSingle*)pCombined->aCombinedT()->m_pRowSingleT,
                                     &pCombined->aCombinedT()->m_racStrAreaT);
  pCombined->aCombinedT()->m_racStrAreaTSav = pCombined->aCombinedT()->m_racStrAreaT;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form signature for fun0
  sFunSignature.vSprint("\n  int iFun0_%s(struct Combined_T* aCombinedT)"_Stz, szStageNm->azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append function signatures
  m_pOpHeader->vAppendFragmentSemiColon(sFunSignature);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary code
  Fragments::vAppendPreliminaryFunc0(m_pPreliminary->azCode(), sFunSignature);

  // create a new shape for this OP
  pShape = new Shape;

//  u8 uSelectColumns = pLogicUnit->m_uSelectColumns;
//
//  // get streamDefIn for next op as streamDefOut for this one
//  Op* pNextOp = pStream->pGet(pOp->iOp());
//
//  // HACK
//  if (pNextOp->szOpNm()->bSame("indexJoin"_Stz)) {
//    uSelectColumns = 0xFFFFFFFFFFFFFFFFLL;
//  }
//  // if next op is project/agg, check project/aggreg what columns they need
//  else if (pNextOp->szOpNm()->bSame("project"_Stz) ||
//           pNextOp->szOpNm()->bSame("aggreg"_Stz)) {
//    pLogicUnit = pNextOp->pOpLogicUnit();
//    uSelectColumns |= pLogicUnit->m_uSelectColumns;
//  }
  u8 uSelectColumns = uGetSelectColumns(pCombined, pStream, pOp);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDefLft
  pCodeSynth = new CodeSynth;
  pCodeSynth->vSynthStruct(pStreamDefLft, 0, SR_IT);
  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the row inflate
  pCodeSynth = new CodeSynth;
  sLabel.vSprint("L_InflateT"_Stz);
  sReturnLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), pOp->iOp());
  pCodeSynth->vSynthInflate(pStreamDefLft, "T"_Stz, sLabel, sReturnLabel, &uSelectColumns);
  pShape->vCodeSynthA(pCodeSynth);

  return pShape;
}

/*----------------------------------------------------------------------------*/
/* sketch inflate of row from rowBuffer                                       */
/*----------------------------------------------------------------------------*/
Shape* Stencil::pSketchInflateRow(Combined* pCombined, Op* pOp, Stz* szStageNm) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  Shape *pShape;
  CodeSynth *pCodeSynth;

  s256 sFunSignature;
  s256 sLabel;

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form signature for fun0
  sFunSignature.vSprint("\n  int iFun1_%s(struct Combined_T* aCombinedT)"_Stz, szStageNm->azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append function signatures
  m_pOpHeader->vAppendFragmentSemiColon(sFunSignature);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary code
  Fragments::vAppendPreliminaryFunc1(m_pPreliminary->azCode(), sFunSignature);

  // create a new shape for this OP
  pShape = new Shape;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDefLft
  pCodeSynth = new CodeSynth;
  pCodeSynth->vSynthStruct(pStreamDefLft, 0, SR_IT);
  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the row inflate
  pCodeSynth = new CodeSynth;
  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), pOp->iOp());
  pCodeSynth->vSynthRowInflate(pStreamDefLft, sLabel);
  pShape->vCodeSynthA(pCodeSynth);

  return pShape;
}

//*============================================================================*/
/* end                                                                         */
//*============================================================================*/
