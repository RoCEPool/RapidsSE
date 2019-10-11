/*============================================================================*/
/* StageNde.hpp -- Pipe Stage-Partition                                       */
/*============================================================================*/
#pragma once

namespace Operators
{
  class StageNde : public Object
  {
  public:
    StageSpec* m_pStageSpec; // specification of the stage
    i8         m_i_Nde;      // which node
    OpVec      m_oOpVec;     // set of operators
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    StageNde(StageSpec* pStageSpec, i8 i_Nde);
   ~StageNde(void) noexcept(false);
  
    void       vLinkOperators(void);
    StageSpec* pStageSpec(void);
  
    Footer* aFooter(void);
  };
  
  StageNde* _pStageNde(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
