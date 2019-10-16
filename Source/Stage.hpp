/*============================================================================*/
/* Stage.hpp -- Pipe Stage                                                    */
/*============================================================================*/
#pragma once

namespace Operators
{
  class Stage : public Object
  {
  private:
    StageSpec*  m_pStageSpec;      // specification of the stage
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
  
    void vDump(void);
    
    i8           nOpsOnNode(i8 iNode);
    OpVec*       pOpVec(void);
    Op*          pPipeOutOp(ePX eToloplogy, i8 iNdeVec);
    StageSpec*   pStageSpec(void);
    StageNde*    pStageNde(i8 i_Nde, i8& iNdeVec);
    StageNdeVec* pStageNdeVec(void);
    StagePtnVec* pStagePtnVec(void);
    
    Footer* aFooter(void);
  };
  
  Stage* _pStage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
