/*============================================================================*/
/* Pipework.hpp -- Pipework                                                   */
/*============================================================================*/
#pragma once

#include "LifeGuard_t.hpp"

namespace STORE { class Stage; class PipeworkSpec; }

namespace STORE
{
  class Pipework : public VectorNL_t<CN_PIPEWORK,Stage,C_MAX_STAGES>,
                   public LifeGuard_t<Pipework>

  {
  private:

    Pipeline*     m_pPipeline;     // which pipeline
    PipeworkSpec* m_pPipeworkSpec; // specification of this formwork
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Pipework(Pipeline* pPipeline, PipeworkSpec* pPipeworkSpec);
   ~Pipework(void) noexcept(false);
  
    bool bCancel(Task* pTriggeringTask);

    PipeworkSpec* pPipeworkSpec(void);
    Op*  pOp0(void);
    i8   nOpsOnNode(i8 iNode);
    Pipeline* pPipeline(void);
    void vSetupJob(void);
    void vSpawnOpTasks(void);
    Stage* pStage0(void);
    Stream* pStream0(void);

    void vCreatePipework();

  private:
    Footer* aFooter(void);
  };
  
  Pipework* _pPipework(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
