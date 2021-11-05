/*============================================================================*/
/* Stencil.cpp -- Op Stencil                                                  */
/*============================================================================*/

#include "BASE.h"

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
  m_pConcluding  = new CodeSynth;
  m_pConcludingEos  = new CodeSynth;
  m_pOpSource    = new CodeSynth;
  m_pEOSShape    = new Shape;

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
  delete m_pOpSource;
  delete m_pEOSShape;
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
  // append preliminary
  m_pOpSource->vAppendFragment(m_pConcluding, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary
  m_pOpSource->vAppendFragment(m_pBannerTail, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append preliminary
  m_pOpSource->vAppendFragment(m_pPreliminaryEos, &iLineNumber);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append eos shape

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

#ifdef WITH_DEBUG_VERSION

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // write to a file

  s256 sFileName;
  LocalFile oFile;

  i8 iQueryNo = AM::iQueryNoInc();
  sFileName.vSprint("../tmp/FlatQuery%lld_%s.txt"_Stz, iQueryNo,
                    AM::pGrid()->pThisNode()->szNodeNm());
  bool bResult = oFile.bOpenWrite(sFileName);
  oFile.vPrint((Stz*)m_pOpSource->azCode());
  oFile.vClose();

#endif


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
void Stencil::vSketch(Combined* pCombined, Tub* pTub, Stream* pStream)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  CodeSynth *pCodeSynth;

  s256 sLabel;
  s256 sReturnLabel;
  s256 sLabelNext;

  bool bScan = false;
  Index* pIndexScan = nullptr;
  bool bInflateGenerated = false;
  bool bRowIteration = false;
  bool bIndexScan = false;
  ePJ  eProjectKind = PJ_NONE;

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
      if (pIndexScan) {
        LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
        pLogicUnit->m_xpIndex = pIndexScan;
      }
      break;
    }

    // get the tags
    TagSet* pOpTags = pOp->pOpTags();

    // get the streamDefOut
    LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
    StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);
    StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for inflate (for first OP)
    if ( bScan && ! bInflateGenerated && ! bIndexScan) {

      pCombined->aCombinedT()->m_pRowSingleT = new RowSingle(pStreamDefLft);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // append banner head
      Fragments::vAppendBannerHead(m_pBannerHead->azCode(), "Func0"_Stz);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // append preliminary code
      Fragments::vAppendPreliminaryFunc0(m_pPreliminary->azCode());

      // create a new shape for this OP
      Shape *pShape = new Shape;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the streamDefLft
      pCodeSynth = new CodeSynth;
      pCodeSynth->vSynthStruct(pStreamDefLft, SR_IT);
      pShape->vStructSynthA(pCodeSynth);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the row inflate
      pCodeSynth = new CodeSynth;
      sLabel.vSprint("L_InflateT"_Stz);
      sReturnLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
      pCodeSynth->vSynthInflate(pStreamDefLft, "T"_Stz, sLabel, sReturnLabel);
      pShape->vCodeSynthA(pCodeSynth);

      // add the shape to the stencil
      vAppend(pShape);

      bInflateGenerated = true;
    }

    if ( ! bScan && ! bRowIteration && ! bIndexScan) {
      // func1
      // row buffer processing
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // append banner head
      Fragments::vAppendBannerHead(m_pBannerHead->azCode(), "Func1"_Stz);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // append preliminary code
      Fragments::vAppendPreliminaryFunc1(m_pPreliminary->azCode());

      // create a new shape for this OP
      Shape *pShape = new Shape;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the streamDefLft
      pCodeSynth = new CodeSynth;
      pCodeSynth->vSynthStruct(pStreamDefLft, SR_IT);
      pShape->vStructSynthA(pCodeSynth);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the row inflate
      pCodeSynth = new CodeSynth;
      sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
      pCodeSynth->vSynthRowInflate(sLabel);
      pShape->vCodeSynthA(pCodeSynth);

      // add the shape to the stencil
      vAppend(pShape);

      bRowIteration = true;
    }

    // create a new shape for this OP
    Shape *pShape = new Shape;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the filterOP
    if (pOp->szOpNm()->bSame("filter"_Stz)) {

      // get filter expr from OP
      Stz* sFilterExpr = pOpTags->szValue(C_TAG_EXPR);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the OP
      pCodeSynth = new CodeSynth;
      sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
      sLabelNext = sGetNextLabel(pStream, i_Op);
      pCodeSynth->vSynthFilter(pStreamDefLft, sFilterExpr, SR_IT, sLabel, sLabelNext);
      pShape->vCodeSynthB(pCodeSynth);
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

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // create rowPix for tub from the streamDefOut of project
      RowPix* pRowPix = pStreamDefOut->pRowPix();
      pTub->vRowPix(pRowPix);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the streamDef
      pCodeSynth = new CodeSynth;
      pCodeSynth->vSynthStruct(pStreamDefOut, SR_PT);
      pShape->vStructSynthA(pCodeSynth);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the row projection
      pCodeSynth = new CodeSynth;
      sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
      pCodeSynth->vSynthProject(pStreamDefLft, pStreamDefOut, pIndexDef,
                                SR_IT, SR_PT, eProjectKind,
                                pAggTypeStack, sProjectExpr,
                                sLabel);
      pShape->vCodeSynthA(pCodeSynth);

      if (eProjectKind == PJ_GBY) {

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // append code for EOSMARKER handling

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // append preliminary code
        Fragments::vAppendPreliminaryFunc3(m_pPreliminaryEos->azCode());

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // generate the code for the streamDef
        pCodeSynth = new CodeSynth;
        pCodeSynth->vSynthStruct(pStreamDefOut, SR_PT);
        m_pEOSShape->vStructSynthA(pCodeSynth);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // payload to buffer
        pCodeSynth = new CodeSynth;
        pCodeSynth->vSynthEos(pStreamDefOut, pIndexDef, SR_PT, pAggTypeStack);
        m_pEOSShape->vCodeSynthA(pCodeSynth);

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

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // create rowPix for tub from the streamDefOut of the indexJoinOP
      RowPix* pRowPix = pStreamDefOut->pRowPix();
      pTub->vRowPix(pRowPix);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for struct for streamDefRht
      pCodeSynth = new CodeSynth;
      pCodeSynth->vSynthStruct(pStreamDefRht, SR_IJ);
      pShape->vStructSynthA(pCodeSynth);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for struct for streamDefOut (join row)
      pCodeSynth = new CodeSynth;
      pCodeSynth->vSynthStruct(pStreamDefOut, SR_TJ);
      pShape->vStructSynthB(pCodeSynth);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the join
      pCodeSynth = new CodeSynth;
      sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
      pCodeSynth->vSynthIndexJoin(pStreamDefOut, pStreamDefLft, pStreamDefRht,
                                  SR_IT, SR_IJ, SR_TJ,
                                  pIndex->pIndexDef(), sJoinExpr, sLabel);
      pShape->vCodeSynthA(pCodeSynth);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the indexScanOp
    if (pOp->szOpNm()->bSame("indexScan"_Stz)) {

      // there should be no RowBuffer or scan
      RASSERT( !bScan );
      RASSERT( !bInflateGenerated );
      RASSERT( !bRowIteration );

      RASSERT(bIndexScan);

      // func2
      // row buffer processing
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // append banner head
      Fragments::vAppendBannerHead(m_pBannerHead->azCode(), "Func2"_Stz);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // append preliminary code
      Fragments::vAppendPreliminaryFunc2(m_pPreliminary->azCode());

      // indexScan should be the second op after consume
      RASSERT(i_Op == 1);

      // get the scanIndex from the indexScanOp
      pIndexScan = _pIndex(*pLogicUnit->m_xpIndex);
      pCombined->aCombinedT()->m_aIndexT = pIndexScan;

      // create RowSingleT
      pCombined->aCombinedT()->m_pRowSingleT = new RowSingle(pStreamDefLft);

      // get the index tree for the index
      Table* pTable = pIndexScan->pTable();

      PtnId tPtn = pOp->tPtnId();
      i8 i_TablePtn = pTable->iPtnNoToTablePtn(tPtn);
      TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);

      i8 iIndexNo = pIndexScan->iIndexNo();

      // COME BACK TO THIS
      IndexTree* pIndexTree = pTablePtn->pIndexTree(iIndexNo);
      pCombined->aCombinedT()->m_aIndexTreeT = pIndexTree;
      pCombined->aCombinedT()->m_aIndexDefT  = pIndexScan->pIndexDef();

      // get indexScan expr from OP
      Stz* sIndexScanExpr = pOpTags->szValue(C_TAG_EXPR);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // create rowPix for tub from the streamDefOut of the indexScanOP
      RowPix* pRowPix = pStreamDefOut->pRowPix();
      pTub->vRowPix(pRowPix);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for struct for streamDefOut (J)
      pCodeSynth = new CodeSynth;
      pCodeSynth->vSynthStruct(pStreamDefOut, SR_IT);
      pShape->vStructSynthA(pCodeSynth);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the index scan
      pCodeSynth = new CodeSynth;
      sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
      sLabelNext = sGetNextLabel(pStream, i_Op);
      pCodeSynth->vSynthIndexScan(pStreamDefOut, pStreamDefLft, pIndexScan->pIndexDef(),
                                  SR_IT, sIndexScanExpr, sLabel, sLabelNext);
      pShape->vCodeSynthA(pCodeSynth);
    }

    // add the shape to the stencil
    vAppend(pShape);

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append concluding code
  if (bIndexScan)
    Fragments::vAppendConcludingFunc2(m_pConcluding->azCode());
  else if (bInflateGenerated)
    Fragments::vAppendConcludingFunc0(m_pConcluding->azCode());
  else
    Fragments::vAppendConcludingFunc1(m_pConcluding->azCode());

  if (eProjectKind != PJ_GBY) {

    pCodeSynth = new CodeSynth;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // append code for EOSMARKER handling
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // append preliminary code
    Fragments::vAppendPreliminaryFunc3(m_pPreliminaryEos->azCode());

    Fragments::vAppendConcludingFunc3(m_pConcludingEos->azCode());

    m_pEOSShape->vCodeSynthA(pCodeSynth);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append banner tail
  Fragments::vAppendBannerTail(m_pBannerTail->azCode());

}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the next label for Op                                                  */
/*----------------------------------------------------------------------------*/
Stz* Stencil::sGetNextLabel(Stream* pStream, i8 i_Op) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s256 sLabelNext;
  i8 iNextOp = i_Op + 1;

  if (iNextOp == pStream->nItems() - 2) {
    // if nextOp is combined, then next label is L_Return
    ASSERT(pStream->pGet(iNextOp)->szOpNm()->bSame("combined"_Stz));
    sLabelNext.vSprint("L_AdvanceNextRow"_Stz);
  }
  else {
    // form next label using next op
    Op *pOpNext = pStream->pGet(iNextOp);
    sLabelNext.vSprint("L_%s_%d"_Stz, pOpNext->szOpNm(), iNextOp);
  }

  return sLabelNext;
}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
