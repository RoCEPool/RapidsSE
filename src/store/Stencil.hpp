/*============================================================================*/
/* Stencil.hpp -- Op Stencil                                                  */
/*============================================================================*/
#pragma once

#include "eLP.h"
#include "eLS.h"
#include "CodeSynth.hpp"
#include "Shape.hpp"
#include "Combined.hpp"

// information about the stage being combined and its Operators
// this will help in adding in the rules
struct stageOps {

  i8 m_iAggregOp;
  i8 m_iCombinedOp;
  i8 m_iFilterOp;
  i8 m_iHashOp;
  i8 m_iIndexJoinOp;
  i8 m_iIndexScanOp;
  i8 m_iKeyExistsOp;
  i8 m_iLimitOp;
  i8 m_iOrdAggOp;
  i8 m_iProjectOp;
  i8 m_iScanOp;
  i8 m_iSortOp;
  i8 m_iTopOp;

  bool m_bScan;
  bool m_bIndexScan;

  bool m_bPayloadOp;

  bool m_bGeneratePotTub;

  Op* m_pCombinedOp;
  Op* m_pHavingFilterOp;
  Op* m_pPostProjectOp;
  Op* m_pLimitOp;

  s256 m_sHashExpr;
};

namespace STORE
{
static const i8 C_MAX_SHAPES = 32;

  class Stencil : public VectorNL_t<CN_STENCIL,Shape,C_MAX_SHAPES>
  {
  private:
  
    eLP         m_ePattern;

    s256        m_szStageNm;
    bool        m_bScan;
    bool        m_bRowBuffer;

    TypedStack* m_pPostStack;

    stageOps   m_xStageOps;
    CodeSynth* m_pBannerHead;
    CodeSynth* m_pBannerTail;
    CodeSynth* m_pPreliminary;
    CodeSynth* m_pPreliminaryFun0;
    CodeSynth* m_pPreliminaryFun1;
    CodeSynth* m_pPreliminaryFun2;
    CodeSynth* m_pPreliminaryEos;
    CodeSynth* m_pConcluding;
    CodeSynth* m_pConcludingFun0;
    CodeSynth* m_pConcludingFun1;
    CodeSynth* m_pConcludingFun2;
    CodeSynth* m_pConcludingEos;
    CodeSynth* m_pOpHeader;
    CodeSynth* m_pOpSource;
    CodeSynth* m_pEmptyFun0;
    CodeSynth* m_pEmptyFun1;
    CodeSynth* m_pEmptyFun2;

    Shape*     m_pShapeCRW;
    Shape*     m_pShapeROW;
    Shape*     m_pShapeIDX;
    Shape*     m_pShapeEOS;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Stencil(void);
   ~Stencil(void) noexcept(false);

    static void vRepair(Object* pObject);

    void vCombine(void);
  
    Stz* szOpSource(void);

    eLP  ePattern(void);
    void vPattern(eLP ePattern);
    void vPrint(void);
    void vSetup(Combined* pCombined, Stream* pStream);
    eEC  eSketch(Combined* pCombined, Stream* pStream);

  private:
    void vAppendFuncEmpty(CodeSynth* pCodeSynth, Stz* szDesc, Stz* szFunNm);
    void vAppendFuncHeader0(void);
    void vAppendFuncHeader1(void);
    void vAppendFuncHeader2(void);
    void vAppendFuncHeader3(void);
    void vAppendHeaderEntries(i8 iStage);

    eEC eCollectStage(Combined* pCombined, Stream* pStream);

    void vGetNextLabel(Stz* sLabelNext, Stream* pStream, i8 i_Op, Stz* sDefault);
    u8   uGetSelectColumns(Stream* pStream);

    void vSetupAggreg(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupEosAgg(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupEosPayload(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupIndexJoin(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupIndexScan(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupKeyExists(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupOrdAgg(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupProjectAgg(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupProjectExp(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupScan(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupSmlAgg(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupSort(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupTop(Combined* pCombined, Stream* pStream, Op* pOp);

    void vSketchEosFilter(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSketchEosProjectAgg(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSketchEosProjectGby(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSketchEosSort(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSketchEosTop(Combined* pCombined, Stream* pStream, Op* pOp);

    Shape* pSketchFilter(Stream* pStream, Op* pOp, eSR ePrevStencil);
    Shape* pSketchIndexScan(Combined* pCombined, Stream* pStream, Op* pOp);
    Shape* pSketchIndexJoin(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* sLabel);

    Shape* pSketchInflateCrw(Combined* pCombined, Stream* pStream, Op* pOp);
    Shape* pSketchInflateRow(Stream* pStream, Op* pOp);
    Shape* pSketchKeyExists(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil);
    Shape* pSketchLabel(Combined* pCombined, Stream* pStream, Op* pOp);
    Shape* pSketchLimit(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* sLabelLoop);
    Shape* pSketchSort(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* szLabelLoop);
    Shape* pSketchOrdAgg(Combined* pCombined, Stream* pStream, Op* pOp,
                         eSR ePrevStencil, Stz* sLabelLoop);
    Shape* pSketchOrderBy(Stream* pStream, Op* pOp, eSR ePrevStencil);
    Shape* pSketchPrjAgg(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* sLabelLoop);
    Shape* pSketchPrjExp(Combined* pCombined, Stream* pStream, Op* pOp,
                                      eSR ePrevStencil, Stz* sLabelLoop);
    Shape* pSketchProjectGby(Combined* pCombined, Stream* pStream, Op* pOp,
                             eSR ePrevStencil, Stz* sLabelLoop);
    Shape* pSketchPot(Combined* pCombined, Op* pLastOp, eSR ePrevStencil, IntStack* pPreExpStack,
                      Stz* szLabel, Stz* sLabelLoop, Stz* sLabelOrdAgg);
    Shape* pSketchReturn(void);
    Shape* pSketchTub(Combined* pCombined, Op* pLastOp, eSR ePrevStencil, IntStack* pPreExpStack,
                      Stz* szLabel, Stz* sLabelLoop, Stz* sLabelOrdAgg);
    Shape* pSketchTop(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil, Stz* sLabelLoop);


    Footer* aFooter(void);
  };
  
  Stencil* _pStencil(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
