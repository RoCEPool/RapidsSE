/*============================================================================*/
/* Strand.hpp -- Strand of Stage of Pipeline                                  */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class Strand : public VectorNL_t<CN_STRAND,Op,C_MAX_OPS>
  {
  private:
    s32        m_sStrandNm;   // synthesised name of this strand
    StageSpec* m_pStageSpec;  // this is a strand of which stage
  
    tPN        m_tPtnNo;       // which partition in grid
    i8         m_i_Node;       // which node in grid
    i8         m_i_Strand;    // which strand of stage

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Strand(StageSpec* pStageSpec, i8 i_Strand);
   ~Strand(void) noexcept(false);
  
    void   vDump(void);
  
    i8     i_Strand(void);
    i8     i_Node(void);
    tPN    tPtnNo(void);
  
    StageSpec* pStageSpec(void);
    void   vSpawnOperatorJobs(i8 iSessionId, i8 iNode);
    void   vSynthesiseName(void);

    Footer* aFooter(void);
  };
  
  Strand* _pStrand(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
