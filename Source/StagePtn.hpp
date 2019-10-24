/*============================================================================*/
/* StagePtn.hpp -- Pipe Stage-Partition                                       */
/*============================================================================*/
#pragma once

namespace Operators
{
  class StagePtn : public Object
  {
  private:
    StageSpec* m_pStageSpec; // specification of the stage
    OpVec      m_oOpVec;     // set of operators
    i8         m_i_Ptn;      // which partition
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    StagePtn(StageSpec* pStageSpec, i8 i_Ptn);
   ~StagePtn(void) noexcept(false);
  
    void       vLinkOperators(StageVec* pStageVec);
    OpVec*     pOpVec(void);
    i8         iPartition(void);
    void       vSpawnOperatorJobs(Session* pSession, i8 iNode);
    StageSpec* pStageSpec(void);
  
    Footer* aFooter(void);
  };
  
  StagePtn* _pStagePtn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
