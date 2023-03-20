/*============================================================================*/
/* Stencil_v2.hpp -- Op Stencil_v2                                            */
/*============================================================================*/
#pragma once

#include "eLP.h"
#include "eLS.h"
#include "CodeSynth_v2.hpp"
#include "CombinedSrc.hpp"
#include "OpSketch.hpp"
#include "Combined.hpp"

// information about the stage being combined and its Operators
// this will help in adding in the rules
struct stageOps_V2 {

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
  s256 m_sLabelOrdAgg;

};

namespace STORE
{
static const i8 C_MAX_OPS_PER_STAGE = 32;

  class Stencil_v2 : public VectorNL_t<CN_STENCIL_V2,OpSketch,C_MAX_OPS_PER_STAGE>
  {
  private:
  
    eLP         m_ePattern;

    s256        m_szStageNm;
    bool        m_bScan;
    bool        m_bRowBuffer;

    TypedStack* m_pPostStack;

    stageOps_V2 m_xStageOps;

    OpSketch     m_xOpSketch[C_MAX_OPS_PER_STAGE];

    CombinedSrc* m_pOpSource;

    CodeSynth_v2* m_pBannerHead;
    CodeSynth_v2* m_pBannerTail;
    CodeSynth_v2* m_pPreliminary;
    CodeSynth_v2* m_pPreliminaryFun0;
    CodeSynth_v2* m_pPreliminaryFun1;
    CodeSynth_v2* m_pPreliminaryFun2;
    CodeSynth_v2* m_pPreliminaryEos;
    CodeSynth_v2* m_pConcluding;
    CodeSynth_v2* m_pConcludingFun0;
    CodeSynth_v2* m_pConcludingFun1;
    CodeSynth_v2* m_pConcludingFun2;
    CodeSynth_v2* m_pConcludingEos;
    CodeSynth_v2* m_pOpHeader;
    CodeSynth_v2* m_pEmptyFun0;
    CodeSynth_v2* m_pEmptyFun1;
    CodeSynth_v2* m_pEmptyFun2;

    OpSketch*     m_pShapeCRW;
    OpSketch*     m_pShapeROW;
    OpSketch*     m_pShapeIDX;
    OpSketch*     m_pShapeEOS;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Stencil_v2(void);
   ~Stencil_v2(void) noexcept(false);

    static void vRepair(Object* pObject);

    void vCombine(bool bGenerateCCode);
  
    Stz* szOpSource(void);

    eLP  ePattern(void);
    void vPattern(eLP ePattern);
    void vPrint(void);
    void vSetup(Combined* pCombined, Stream* pStream);
    eEC  eSketch(Combined* pCombined, Stream* pStream);

  private:
    void vAppendFuncEmpty(CodeSynth_v2* pCodeSynth, Stz* szDesc, Stz* szFunNm);
    void vAppendFuncHeader0(void);
    void vAppendFuncHeader1(void);
    void vAppendFuncHeader2(void);
    void vAppendFuncHeader3(void);
    void vAppendHeaderEntries(i8 iStage);

    eEC eCollectStage(Combined* pCombined, Stream* pStream);

    void vGetNextLabel(Stz* sLabelNext, Stream* pStream, i8 i_Op, Stz* sDefault);
    u8   uGetSelectColumns(Stream* pStream);

    void vSetupAggreg(Combined* pCombined, Op* pOp);
    void vSetupEosPayload(Combined* pCombined, Op* pOp);
    void vSetupIndexJoin(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupIndexScan(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupKeyExists(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupOrdAgg(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupProjectAgg(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupProjectExp(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupScan(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupSort(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupTop(Combined* pCombined, Stream* pStream, Op* pOp);


    OpSketch* pSketchProject(Combined* pCombined, Stream* pStream, Op* pOp,
                             eSR ePrevStencil_v2, Stz* sLabelLoop);



    void vSketchEosProjectAgg(Combined* pCombined, Stream* pStream, Op* pOp, TypedStack* pInitStack);
    void vSketchEosProjectGby(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSketchEosSort(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSketchEosTop(Combined* pCombined, Stream* pStream, Op* pOp);

    OpSketch* pSketchFilter(Stream* pStream, Op* pOp, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchIndexScan(Combined* pCombined, Stream* pStream, Op* pOp);
    OpSketch* pSketchIndexJoin(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil_v2, Stz* sLabel);

    OpSketch* pSketchInflateCrw(Combined* pCombined, Stream* pStream, Op* pOp);
    OpSketch* pSketchInflateRow(Stream* pStream, Op* pOp);
    OpSketch* pSketchKeyExists(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil_v2, bool bExists);
    OpSketch* pSketchLabel(Combined* pCombined, Stream* pStream, Op* pOp);
    OpSketch* pSketchLimit(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchSort(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil_v2, Stz* szLabelLoop);
    OpSketch* pSketchOrdAgg(Combined* pCombined, Stream* pStream, Op* pOp,
                         eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchPrjAgg(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchProjectGby(Combined* pCombined, Stream* pStream, Op* pOp,
                             eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchPot(Op* pLastOp, eSR ePrevStencil_v2, IntStack* pPreExpStack,
                      Stz* szLabel, Stz* sLabelLoop, Stz* sLabelOrdAgg);
    OpSketch* pSketchReturn(void);
    OpSketch* pSketchTub(Op* pLastOp, eSR ePrevStencil_v2, IntStack* pPreExpStack,
                      Stz* szLabel, Stz* sLabelLoop, Stz* sLabelOrdAgg);
    OpSketch* pSketchTop(Combined* pCombined, Op* pOp, eSR ePrevStencil_v2, Stz* sLabelLoop);


    Footer* aFooter(void);
  };
  
  Stencil_v2* _pStencil_v2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
