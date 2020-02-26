/*============================================================================*/
/* Stage.hpp -- Pipe Stage                                                    */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class Stage : public Object
  {
  private:
    s32         m_sStageNm;        // stage name
    OpVec       m_oOpVec;          // set of operators
    StagePtnVec m_oStagePtnVec;    // set of stage-partitions
    StageNdeVec m_oStageNdeVec;    // set of stage-nodes
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Stage(StageSpec* pStageSpec, Pipeline* pPipeline);
   ~Stage(void) noexcept(false);
  
    void vDump(StageVec* pStageVec);
    
    void         vCancelOperators(i8 iThisNode);
    i8           nOpsOnNode(i8 iNode);
    OpVec*       pOpVec(void);
    Op*          pPipeOutOp(ePX eTopologyX, i8 iNde, i8 iPtn, i8 iOp);
    StageNde*    pStageNde(i8 i_Nde);
    StageNdeVec* pStageNdeVec(void);
    Stz*         szStageNm(void);
    StagePtn*    pStagePtn(i8 i_Ptn);
    StagePtnVec* pStagePtnVec(void);
    void         vStartPipeline(void);
    
    Footer* aFooter(void);
  };
  
  Stage* _pStage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
