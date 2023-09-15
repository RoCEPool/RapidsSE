/*============================================================================*/
/* Stencil.hpp -- Op Stencil                                                  */
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

  class Stencil : public VectorNL_t<CN_STENCIL,OpSketch,C_MAX_OPS_PER_STAGE>
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

    Stencil(void);
   ~Stencil(void) noexcept(false);

    static void vRepair(Object* pObject);

    static eEC eSynthesise(PipelineDef* pPipelineDef);

    Stz* szOpSource(void);
    void vSynthesise(bool bFirstStage, bool bLastStage);
    eEC  eSketch(Dynamic* pDynamic, Stream* pStream);
    Stz* szStageNm(void);

  private:

    eEC  eSynthesise(StageDef* pStageDef);
    void vGetNextLabel(String* psLabelNext, StageDef* pStageDef, i8 i_Op, Stz* sDefault);

    OpSketch* pSketchAggreg    (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchAntiJoin  (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchEos       (StageDef* pStageDef);
    OpSketch* pSketchFilter    (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchGbyAgg    (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchIndexJoin (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchIndexScan (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchInflateCrw(StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchInflateRow(StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchLimit     (StageDef* pStageDef, OpDef* pOpDefp);
    OpSketch* pSketchOrdAgg    (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchPotTub    (OpSketch* pLastOp, bool bGeneratePot, OpDef* pHashOpDef, Stz* sLimitExpr);
    OpSketch* pSketchProject   (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchSemiJoin  (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchSort      (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchStreamDef (OpSketch* pLastOp);
    OpSketch* pSketchTop       (StageDef* pStageDef, OpDef* pOpDef);


    Footer* aFooter(void);
  };
  
  Stencil* _pStencil(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
