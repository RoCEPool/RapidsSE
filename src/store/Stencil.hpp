/*============================================================================*/
/* Stencil.hpp -- Op Stencil                                                  */
/*============================================================================*/
#pragma once

#include "eSK.h"
#include "eTP.h"
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
    i8           m_iPrevOpNo;
    i8           m_iPrevDupOpNo;
    s256         m_sLabelLoop;

    i8           m_iOpSketchCount[SK_ENUMS];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Stencil(void);
   ~Stencil(void) noexcept(false);

    static eEC eSynthesise(PipelineDef* pPipelineDef);

    Stz* szOpSource(void);
    Stz* szStageNm(void);

  private:

		void vSynthesise(bool bFirstStage, bool bLastStage);
    eEC  eSynthesise(StageDef* pStageDef);
    void vGetNextLabel(String* psLabelNext, StageDef* pStageDef, i8 i_Op, Stz* sDefault);

    OpSketch* pSketchAggreg    (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchAntiJoin  (StageDef* pStageDef, OpDef* pOpDef, OpSketch* pLastOpSketch);
    OpSketch* pSketchEos       (StageDef* pStageDef, OpSketch* pLastOpSketch);
    OpSketch* pSketchFilter    (StageDef* pStageDef, OpDef* pOpDef, OpSketch* pLastOpSketch);
    OpSketch* pSketchGbyAgg    (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchIndexJoin (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchIndexScan (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchInflateCrw(StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchPdsRow    (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchLimit     (StageDef* pStageDef, OpDef* pOpDefp, OpSketch* pLastOpSketch);
    OpSketch* pSketchOrdAgg    (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchPotTub    (OpSketch* pLastOp, eTP eTubPot, OpDef* pHashOpDef, Stz* sLimitExpr);
		OpSketch* pSketchPrjPotTub(StageDef* pStageDef, OpDef* pOpDef, eTP eTubPot, OpDef* pHashOpDef, Stz* sLimitExpr);
		OpSketch* pSketchProject   (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchRrfJoin   (StageDef* pStageDef, OpDef* pOpDef);
    OpSketch* pSketchSemiJoin  (StageDef* pStageDef, OpDef* pOpDef, OpSketch* pLastOpSketch);
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
