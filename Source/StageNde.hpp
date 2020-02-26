/*============================================================================*/
/* StageNde.hpp -- Pipe Stage-Partition                                       */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class StageNde : public Object
  {
  private:
    OpVec      m_oOpVec;     // set of operators
    i8         m_i_Nde;      // which node
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    StageNde(StageSpec* pStageSpec, i8 i_Nde);
   ~StageNde(void) noexcept(false);
  
    i8         iNode(void);
    OpVec*     pOpVec(void);
    void       vSpawnOperatorJobs(i8 iSessionId, i8 iNode);
  
    Footer* aFooter(void);
  };
  
  StageNde* _pStageNde(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
