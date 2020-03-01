/*============================================================================*/
/* Division.hpp -- Pipe Stage-Partition                                       */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class Division : public VectorNL_t<CN_DIVISION,Op,C_MAX_OPS>
  {
  private:
    s32        m_sDivisionNm; // synthesised name of this division
    i8         m_i_Div;       // which division of stage
    StageSpec* m_pStageSpec;  // this is a division of which stage
  
    i8         m_i_Ptn;       // which partition in grid
    i8         m_i_Nde;       // which node in grid
    i8         m_i_Slt;       // which slot in node

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Division(StageSpec* pStageSpec, i8 i_Div);
   ~Division(void) noexcept(false);
  
    void   vDump(void);
  
    i8     i_Div(void);
    i8     i_Nde(void);
    i8     i_Ptn(void);
    i8     i_Slt(void);
  
    StageSpec* pStageSpec(void);
    void   vSpawnOperators(i8 iSessionId, i8 iNode);
    void   vSynthesiseName(void);

    Footer* aFooter(void);
  };
  
  Division* _pDivision(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
