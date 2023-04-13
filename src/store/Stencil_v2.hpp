/*============================================================================*/
/* Stencil_v2.hpp -- Op Stencil_v2                                            */
/*============================================================================*/
#pragma once

#include "eSK.h"
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

    s256        m_szStageNm;
    bool        m_bRowBuffer;

    stageOps_V2 m_xStageOps;

    CombinedSrc* m_pOpSource;
    CodeSynth_v2* m_pOpHeader;

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
    void vPrint(void);
    void vSetup(Combined* pCombined, Stream* pStream);
    eEC  eSketch(Combined* pCombined, Stream* pStream);
    Stz* szStageNm(void);

  private:

    eEC eCollectStage(Combined* pCombined, Stream* pStream);

    void vGetNextLabel(Stz* sLabelNext, Stream* pStream, i8 i_Op, Stz* sDefault);
    u8   uGetSelectColumns(Stream* pStream);

    void vSetupAggreg(Combined* pCombined, Op* pOp);
    void vSetupIndexJoin(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupIndexScan(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupKeyExists(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupOrdAgg(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupAggreg(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupProject(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupScan(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupSort(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupTop(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSetupTub(Combined* pCombined, Op* pOp, eHK eHandlerKind);


    OpSketch* pSketchEos(Stream* pStream, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchFilter(Stream* pStream, Op* pOp, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchInflateCrw(Stream* pStream, Op* pOp, eHK eHandlerKind);
    OpSketch* pSketchProject(Stream* pStream, Op* pOp, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchAggreg(Stream* pStream, Op* pOp, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchTub(OpSketch* pLastOp, eHK eHandlerKind, Stz* sLabelLoop, eSR ePrevStencil_v2, TypedStack* pInitStack = nullptr);






    void vSketchEosProjectAgg(Combined* pCombined, Stream* pStream, Op* pOp, TypedStack* pInitStack);
    void vSketchEosProjectGby(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSketchEosSort(Combined* pCombined, Stream* pStream, Op* pOp);
    void vSketchEosTop(Combined* pCombined, Stream* pStream, Op* pOp);

    OpSketch* pSketchIndexScan(Combined* pCombined, Stream* pStream, Op* pOp);
    OpSketch* pSketchIndexJoin(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil_v2, Stz* sLabel);

    OpSketch* pSketchInflateRow(Stream* pStream, Op* pOp);
    OpSketch* pSketchKeyExists(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil_v2, bool bExists);
    OpSketch* pSketchLimit(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchSort(Combined* pCombined, Stream* pStream, Op* pOp, eSR ePrevStencil_v2, Stz* szLabelLoop);
    OpSketch* pSketchOrdAgg(Combined* pCombined, Stream* pStream, Op* pOp,
                         eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchProjectGby(Combined* pCombined, Stream* pStream, Op* pOp,
                             eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchTop(Combined* pCombined, Op* pOp, eSR ePrevStencil_v2, Stz* sLabelLoop);


    Footer* aFooter(void);
  };
  
  Stencil_v2* _pStencil_v2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
