/*============================================================================*/
/* StageNde.hpp -- Pipe Stage-Partition                                       */
/*============================================================================*/
#pragma once

namespace Operators
{
  class StageNde : public Object
  {
  private:
    StageSpec* m_pStageSpec; // specification of the stage
    OpVec      m_oOpVec;     // set of operators
    i8         m_i_Nde;      // which node
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    StageNde(StageSpec* pStageSpec, i8 i_Nde);
   ~StageNde(void) noexcept(false);
  
    void       vLinkOperators(void);
    i8         iNode(void);
    OpVec*     pOpVec(void);
    void       vSpawnOperators(Session* pSession, i8 iNode);
    StageSpec* pStageSpec(void);
  
    Footer* aFooter(void);
  };
  
  StageNde* _pStageNde(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
