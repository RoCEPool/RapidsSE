/*============================================================================*/
/* PipeWork.hpp -- PipeWork                                                   */
/*============================================================================*/
#pragma once

namespace STORE { class Stage;    class StageSet;
                  class PipeSpec; class SubPipeWorks; }

namespace STORE
{
  class PipeWork : public VectorNL_t<CN_PIPEWORK,Stage,C_MAX_STAGES>
                 , public ListItem_t<PipeWork>


  {
  private:

    Pipeline* m_pPipeline; // which pipeline
    PipeSpec* m_pPipeSpec; // specification of this pipework
  
    SubPipeWorks* m_pSubPipeWorks;
    StageSet* m_pStageSet;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PipeWork(Pipeline* pPipeline, PipeSpec* pPipeSpec);
   ~PipeWork(void) noexcept(false);
  
    bool bCancel(Task* pTriggeringTask);

    PipeSpec* pPipeSpec(void);
    Op*  pOp0(void);
    i8   nOpsOnNode(i8 iNode);
    Pipeline* pPipeline(void);
    void vSetInvoker(Task* pInvoker);
    void vSetupJob(void);
    void vSpawnOpTasks(void);
    Stage* pStage0(void);
    StageSet* pStageSet(void);
    Stream* pStream0(void);
    SubPipeWorks* pSubPipeWorks(void);

    void vCreatePipework();

  private:
    Footer* aFooter(void);
  };
  
  PipeWork* _pPipeWork(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
