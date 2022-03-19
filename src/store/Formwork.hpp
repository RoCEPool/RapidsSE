/*============================================================================*/
/* Formwork.hpp -- Formwork                                                   */
/*============================================================================*/
#pragma once

#include "LifeGuard_t.hpp"

namespace STORE { class Stage; class FormworkSpec; }

namespace STORE
{
  class Formwork : public VectorNL_t<CN_FORMWORK,Stage,C_MAX_STAGES>,
                   public LifeGuard_t<Formwork>

  {
  private:

    Pipeline*     m_pPipeline;     // which pipeline
    FormworkSpec* m_pFormworkSpec; // specification of this formwork
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Formwork(Pipeline* pPipeline, FormworkSpec* pFormworkSpec);
   ~Formwork(void) noexcept(false);
  
    bool bCancel(Task* pTriggeringTask);

    FormworkSpec* pFormworkSpec(void);
    Op*  pOp0(void);
    i8   nOpsOnNode(i8 iNode);
    Pipeline* pPipeline(void);
    void vSetupJob(void);
    void vSpawnOpTasks(i8 iNode);
    Stage* pStage0(void);
    Stream* pStream0(void);

    void vCreateFormwork();

  private:
    Footer* aFooter(void);
  };
  
  Formwork* _pFormwork(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
