/*============================================================================*/
/* Division.hpp -- Pipe Stage-Partition                                       */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class Division : public VectorNL_t<CN_DIVISION,Op,C_MAX_OPS>
  {
  private:
    OpVec m_oOpVec;  // set of operators
    union {
      i8  m_i_Div;   // which division
      i8  m_i_Nde;   // which node
      i8  m_i_Ptn;   // which partition
    };
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Division(StageSpec* pStageSpec, i8 i_Div);
   ~Division(void) noexcept(false);
  
    OpVec* pOpVec(void);
    i8     i_Div(void);
    i8     i_Nde(void);
    i8     i_Ptn(void);
    void   vSpawnOperators(i8 iSessionId, i8 iNode);
  
    Footer* aFooter(void);
  };
  
  Division* _pDivision(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
