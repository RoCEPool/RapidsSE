/*============================================================================*/
/* PipeWork.hpp -- PipeWork                                                   */
/*============================================================================*/
#pragma once

#include "LifeGuard_t.hpp"

namespace STORE { class Stage; class PipeSpec; }

namespace STORE
{
  class PipeWork : public VectorNL_t<CN_PIPEWORK,Stage,C_MAX_STAGES>,
                   public LifeGuard_t<PipeWork>

  {
  private:

    Pipeline*     m_pPipeline;     // which pipeline
    PipeSpec* m_pPipeWorkSpec; // specification of this formwork
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PipeWork(Pipeline* pPipeline, PipeSpec* pPipeWorkSpec);
   ~PipeWork(void) noexcept(false);
  
    bool bCancel(Task* pTriggeringTask);

    PipeSpec* pPipeWorkSpec(void);
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
  
  PipeWork* _pPipeWork(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
