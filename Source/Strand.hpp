/*============================================================================*/
/* Strand.hpp -- Strand of Stage of Pipeline                                  */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class Strand : public VectorNL_t<CN_STRAND,Op,C_MAX_OPS>
  {
  private:
    s32  m_sPipelineNm; // pipeline name
    s32  m_sStageNm;    // stage name within pipeline
    s32  m_sStrandNm;   // name of this strand
    i8   m_i_Strand;    // [strand] in stage

    StageSpec* m_pStageSpec;  // specification of this stage strand
    Stage*     m_pStage;      // which stage of pipeline
    Pipeline*  m_pPipeline;   // which pipeline

    tPN  m_tPtnNo; // this strand processes m_tPtnNo in grid
                   //   will be C_UNKNOWN for stage with node topology
    i8   m_i_Node; // this strand processes m_i_Node in grid
                   //   is PtnMap::i_PtnNoToNode(m_tPtnNo);
 
    // -- -- -- -- -- --

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Strand(StageSpec* pStageSpec, Stage* pStage, i8 i_Strand,
           tPN tPtnNo, i8 i_Node);
   ~Strand(void) noexcept(false);
  
    void   vDump(void);
  
    i8     i_Node(void);
  
    Pipeline* pPipeline(void);
    tPN    tPtnNo(void);
  
    Stage* pStage(void);
    StageSpec* pStageSpec(void);
    void   vSpawnOperatorJobs(i8 iSessionId, i8 iNode);
  
    Stz* szStrandNm(void);

    void   vSynthesiseName(void);

    Footer* aFooter(void);
  };
  
  Strand* _pStrand(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
