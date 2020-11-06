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
    i8         m_i_Strand;    // which strand of stage
    StageSpec* m_pStageSpec;  // this is a strand of which stage
  
    i8         m_i_Ptn;       // which partition in grid
    i8         m_i_Nde;       // which node in grid
    i8         m_i_TablePtn;       // which slot in node

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Strand(StageSpec* pStageSpec, i8 i_Strand);
   ~Strand(void) noexcept(false);
  
    void   vDump(void);
  
    i8     i_Strand(void);
    i8     i_Nde(void);
    i8     i_Ptn(void);
    i8     i_TablePtn(void);
  
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
