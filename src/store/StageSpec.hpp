/*============================================================================*/
/* StageSpec.hpp -- Stage Specification                                       */
/*============================================================================*/
#pragma once

#include "eYG.h"
#include "eYX.h"

#include "OpSpec.hpp"
#include "LifeGuard_t.hpp"

namespace STORE
{
  class StageSpec
          : public ListNL_t<OpSpec>
          , public LifeGuard_t<StageSpec>
          , public ListItem_t<StageSpec>

  {
  private:
    s32  m_sStageNm;    // stage name

    PipelineSpec* m_pPipelineSpec; // which pipeline spec

    eYG  m_eStageTopology;  // stage topoplogy
    bool m_bNodeTopo;       // is node-level topology (or partition)?
    i8   m_i_SpecificNode;  // run on specific node
    PtnId m_tSpecificPtnNo; // run for specific ptn
    //i8  m_nThreads;       // threads
  
  
    // -- -- -- -- -- --

    eYX m_eCrossTopo;

    s32 m_sNextStageNm;  // next stage name
    i8  m_i_NextStage;
    i8  m_i_NextOp;

    s32 m_sOtherStageNm; // next stage name
    i8  m_i_OtherStage;  // other stage in the cross topology
    i8  m_i_OtherOp;     // next Op in the other stage

    eXK m_eSendKind;     // {normal, broadcast, hashcast, robin, local robin}
                         //   if send op

  
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StageSpec(Stz* szStageNm, PipelineSpec* pPipelineSpec);
   ~StageSpec(void) noexcept(false);
   
    static eYX eCrossTopoCalc(StageSpec* pFromStageSpec,
                              StageSpec* pToStageSpec,
                              eYO&       reOpTopology);

    static eYO eOpTopoCalc(StageSpec* pStageSpec);

    void vAddOpSpec(OpSpec* pOpSpec);
    void vCopyOps(StageSpec* pStageSpec);
  
    void vCrossTopo(eYX eCrossTopo,
                    Stz* szNextStageNm, i8 i_NextStage, i8 i_NextOp);

    OpSpec* pFindOpSpec(Stz* szFindNm);
    bool bNodeTopo(void);
    OpSpec* pOpSpec(i8 i_Op);
  
    void vOtherStageOp(Stz* szOtherStageNm, i8 i_OtherStage,i8 i_OtherOp);

    PipelineSpec* pPipelineSpec(void);
    void vSpecificNode(i8 i_SpecificNode);
    i8   iSpecificNode(void);

    PtnId tSpecificPtnNo(void);
    void vSpecificPtnNo(PtnId tSpecificPtnNo);

    Stz* szStageNm(void);
    i8   i_StageSpec(void);
    //void vThreads(i8 iThreads);
    //i8   nThreads(void);
    eYG  eStageTopology(void);
    void vStageTopology(eYG eStageTopology);
    void vToPipeStr(Stz* szPipeStr);
  
    Footer* aFooter(void);
  
  
  
    eYX  eCrossTopo(void);
  
    i8   iNextOp(void);
    i8   iNextStage(void);
    Stz* szNextStageNm(void);

    i8   iOtherOp(void);
    i8   iOtherStage(void);
    Stz* szOtherStageNm(void);

    eXK  eSendKind(void);
    void vSendKind(eXK eStageKind);
  };

  StageSpec* _pStageSpec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
