/*============================================================================*/
/* Stage.hpp -- Stage (of Deck of Vertex of Pipeline)                         */
/*============================================================================*/
#pragma once

#include "eYS.h"

namespace STORE { class Deck; class Vertex;
                  class Stream; class StageSpec; }

namespace STORE
{
  class Stage : public VectorNL_t<CN_STAGE,Stream,C_MAX_STREAMS>
  {
  private:
    Deck*      m_pDeck;
    StageSpec* m_pStageSpec; // specification of this stage
  
    i8   m_i_Stage;     // [stage] in pipeline

  
    // -- -- -- -- -- --

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Stage(Deck*      pDeck,
          StageSpec* pStageSpec);
   ~Stage(void) noexcept(false);
      
    void vCancelPipeline(Task* pTriggeringTask);
    Deck* pDeck(void);
    bool bEnqueueInvoke(void);
    Stream* pGetStream_byNodeNo(i8 i_Node);
    Stream* pGetStream_byPtnNo(i8 i_Ptn);
    Stream* pGetStream_byStreamNo(i8 i_Stream);
    Op*  pOp0(void);
    i8   nOpsExeOnNode(i8 iNode, i8 iOrigin);
    Pipeline* pPipeline(void);
    Vertex* pVertex(void);
    Op*  pPipeDstOp(PtnId tDstPtnId, i8 i_DstOp);
    i8   i_Stage(void);
    Stz* szStageNm(void);
    Stream* pStream0(void);
    StageSpec* pStageSpec(void);


  private:
    void    vCreateStreams(void);

    Footer* aFooter(void);
  };
  
  Stage* _pStage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
