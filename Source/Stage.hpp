/*============================================================================*/
/* Stage.hpp -- Pipe Stage                                                    */
/*============================================================================*/
#pragma once

namespace Operators
{
  class Stage : public Object
  {
  private:
    s32         m_sName;           // stage name
    eSK         m_eStageKind;      // stage kind
    OpVec       m_oOpVec;          // set of operators
    StagePtnVec m_oStagePtnVec;    // set of stage-partitions
    StageNdeVec m_oStageNdeVec;    // set of stage-nodes
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Stage(StageSpec* pStageSpec, Pipeline* pPileLine);
   ~Stage(void) noexcept(false);
  
    void vDump(StageVec* pStageVec);
    
    void         vCancelOperators(i8 iThisNode);
    Stz*         szName(void);
    i8           nOpsOnNode(i8 iNode);
    OpVec*       pOpVec(void);
    Op*          pPipeOutOp(ePX eTopologyX, i8 iNde, i8 iPtn, i8 iOp);
    eSK          eStageKind(void);
    void         vStageKind(eSK eStageKind);
    StageNde*    pStageNde(i8 i_Nde);
    StageNdeVec* pStageNdeVec(void);
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
