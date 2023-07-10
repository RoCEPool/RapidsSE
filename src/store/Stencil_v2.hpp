/*============================================================================*/
/* Stencil_v2.hpp -- Op Stencil_v2                                            */
/*============================================================================*/
#pragma once

#include "eSK.h"
#include "CombinedSrc.hpp"
#include "OpSketch.hpp"
#include "Dynamic.hpp"

#include "PipelineDef.hpp"


// information about the stage being combined and its Operators
// this will help in adding in the rules
struct stageOps_V2 {

  i8 m_iAggregOp;
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

  Op* m_pHavingFilterOp;
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

    s256         m_szStageNm;
    stageOps_V2  m_xStageOps;
    CombinedSrc* m_pOpSource;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Stencil_v2(void);
   ~Stencil_v2(void) noexcept(false);

    static void vRepair(Object* pObject);

    static eEC eSynthesise(PipelineDef* pPipelineDef);

    void vSynthesise(i8 i_Stage, bool bLastStage);
    Stz* szOpSource(void);
    eEC  eSketch(Dynamic* pDynamic, Stream* pStream);
    Stz* szStageNm(void);

  private:

    eEC  eSynthesise(StageDef* pStageDef);
    void vGetNextLabel(Stz* sLabelNext, StageDef* pStageDef, i8 i_Op, Stz* sDefault);

    OpSketch* pSketchAggreg(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchAntiJoin(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchEos(StageDef* pStageDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchFilter(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchGbyAgg(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchIndexJoin(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchIndexScan(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, Stz* sLabelLoop);
    OpSketch* pSketchSemiJoin(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchInflateCrw(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, Stz* sLabelLoop);
    OpSketch* pSketchInflateRow(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, Stz* sLabelLoop);
    OpSketch* pSketchLimit(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchProject(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchSort(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* szLabelLoop);
    OpSketch* pSketchTop(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* szLabelLoop);
    OpSketch* pSketchStreamDef(OpSketch* pLastOp, eHK eHandlerKind);
    OpSketch* pSketchPotTub(OpSketch* pLastOp, eHK eHandlerKind, eSR ePrevStencil_v2,
                            Stz* sLabelLoop, bool bGeneratePot, Stz* sHashExpr, Stz* sLimitExpr);

    eEC eValidateStage(StageDef* pStageDef);

    void vSetupOrdAgg(Dynamic* pDynamic, Stream* pStream, Op* pOp);

    void vSketchEosSort(Dynamic* pDynamic, Stream* pStream, Op* pOp);
    void vSketchEosTop(Dynamic* pDynamic, Stream* pStream, Op* pOp);

    OpSketch* pSketchOrdAgg(Dynamic* pDynamic, Stream* pStream, Op* pOp,
                         eSR ePrevStencil_v2, Stz* sLabelLoop);


    Footer* aFooter(void);
  };
  
  Stencil_v2* _pStencil_v2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
