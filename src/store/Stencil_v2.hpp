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

  Op* m_pDynamicOp;
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

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Stencil_v2(void);
   ~Stencil_v2(void) noexcept(false);

    static void vRepair(Object* pObject);

    static eEC eSynthesise(PipelineDef* pPipelineDef);

    void vCombine(bool bGenerateCCode);
    Stz* szOpSource(void);
    void vPrint(void);
    void vSetup(Dynamic* pDynamic, Stream* pStream);
    eEC  eSketch(Dynamic* pDynamic, Stream* pStream);
    Stz* szStageNm(void);

  private:

    eEC eCollectStage(Dynamic* pDynamic, Stream* pStream);

    void vGetNextLabel(Stz* sLabelNext, Stream* pStream, i8 i_Op, Stz* sDefault);
    u8   uGetSelectColumns(Stream* pStream);

    void vSetupAggreg(Dynamic* pDynamic, Stream* pStream, Op* pOp);
    void vSetupIndexJoin(Dynamic* pDynamic, Stream* pStream, Op* pOp);
    void vSetupIndexScan(Dynamic* pDynamic, Stream* pStream, Op* pOp);
    void vSetupKeyExists(Dynamic* pDynamic, Stream* pStream, Op* pOp);
    void vSetupOrdAgg(Dynamic* pDynamic, Stream* pStream, Op* pOp);
    void vSetupGbyAgg(Dynamic* pDynamic, Stream* pStream, Op* pOp);
    void vSetupProject(Dynamic* pDynamic, Stream* pStream, Op* pOp);
    void vSetupScan(Dynamic* pDynamic, Stream* pStream, Op* pOp);
    void vSetupSort(Dynamic* pDynamic, Stream* pStream, Op* pOp);
    void vSetupTop(Dynamic* pDynamic, Stream* pStream, Op* pOp);
    void vSetupTub(Dynamic* pDynamic, Op* pOp, eHK eHandlerKind);


    OpSketch* pSketchEos(Stream* pStream, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchFilter(Stream* pStream, Op* pOp, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchGbyAgg(Dynamic* pDynamic, Stream* pStream, Op* pOp, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchIndexScan(Stream* pStream, Op* pOp, eHK eHandlerKind, Stz* sLabelLoop);
    OpSketch* pSketchIndexJoin(Stream* pStream, Op* pOp, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchInflateCrw(Stream* pStream, Op* pOp, eHK eHandlerKind, Stz* sLabelLoop);
    OpSketch* pSketchInflateRow(Stream* pStream, Op* pOp, eHK eHandlerKind, Stz* sLabelLoop);
    OpSketch* pSketchProject(Stream* pStream, Op* pOp, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchAggreg(Stream* pStream, Op* pOp, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchSort(Dynamic* pDynamic, Stream* pStream, Op* pOp, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* szLabelLoop);
    OpSketch* pSketchTub(OpSketch* pLastOp, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop, TypedStack* pInitStack = nullptr);







    void vSketchEosSort(Dynamic* pDynamic, Stream* pStream, Op* pOp);
    void vSketchEosTop(Dynamic* pDynamic, Stream* pStream, Op* pOp);


    OpSketch* pSketchKeyExists(Dynamic* pDynamic, Stream* pStream, Op* pOp, eSR ePrevStencil_v2, bool bExists);
    OpSketch* pSketchLimit(Dynamic* pDynamic, Stream* pStream, Op* pOp, eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchOrdAgg(Dynamic* pDynamic, Stream* pStream, Op* pOp,
                         eSR ePrevStencil_v2, Stz* sLabelLoop);
    OpSketch* pSketchTop(Dynamic* pDynamic, Op* pOp, eSR ePrevStencil_v2, Stz* sLabelLoop);


    Footer* aFooter(void);
  };
  
  Stencil_v2* _pStencil_v2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
