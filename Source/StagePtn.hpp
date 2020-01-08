/*============================================================================*/
/* StagePtn.hpp -- Pipe Stage-Partition                                       */
/*============================================================================*/
#pragma once

namespace Operators
{
  class StagePtn : public Object
  {
  private:
    OpVec      m_oOpVec;     // set of operators
    i8         m_i_Ptn;      // which partition
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    StagePtn(StageSpec* pStageSpec, i8 i_Ptn);
   ~StagePtn(void) noexcept(false);
  
    OpVec*     pOpVec(void);
    i8         iPartition(void);
    void       vSpawnOperatorJobs(i8 iSessionId, i8 iNode);
  
    Footer* aFooter(void);
  };
  
  StagePtn* _pStagePtn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
