/*============================================================================*/
/* Stencil_v2.hpp -- Op Stencil_v2                                            */
/*============================================================================*/
#pragma once

#include "eSK.h"
#include "CombinedSrc.hpp"
#include "OpSketch.hpp"
#include "Dynamic.hpp"

#include "PipelineDef.hpp"

namespace STORE
{
static const i8 C_MAX_OPS_PER_STAGE = 32;

  class Stencil_v2 : public VectorNL_t<CN_STENCIL_V2,OpSketch,C_MAX_OPS_PER_STAGE>
  {
  private:

    s256         m_szStageNm;
    CombinedSrc* m_pOpSource;

    eHK          m_eHandlerKind;
    eSR          m_ePreviousStencil;
    s256         m_sLabelLoop;

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

    OpSketch* pSketchAggreg    (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchAntiJoin  (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchEos       (StageDef* pStageDef);
    OpSketch* pSketchFilter    (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchGbyAgg    (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchIndexJoin (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchIndexScan (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchSemiJoin  (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchInflateCrw(StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchInflateRow(StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchLimit     (StageDef* pStageDef, OpDef* pOpDefp);
    OpSketch* pSketchPotTub    (OpSketch* pLastOp, bool bGeneratePot, Stz* sHashExpr, Stz* sLimitExpr);
    OpSketch* pSketchProject   (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchSort      (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchStreamDef (OpSketch* pLastOp);
    OpSketch* pSketchTop       (StageDef* pStageDef, OpDef* pOpDef);

    void vSetupOrdAgg(Dynamic* pDynamic, Stream* pStream, Op* pOp);
    OpSketch* pSketchOrdAgg(Dynamic* pDynamic, Stream* pStream, Op* pOp,
                         eSR ePrevStencil_v2, Stz* sLabelLoop);


    Footer* aFooter(void);
  };
  
  Stencil_v2* _pStencil_v2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
