/*============================================================================*/
/* PipeWork.hpp -- PipeWork                                                   */
/*============================================================================*/
#pragma once

namespace STORE { class Stage;    class Staging;
                  class PipeSpec; class SubPipeWorks; }

namespace STORE
{
  class PipeWork : public ListNL_t<PipeWork>
                 , public ListItem_t<PipeWork>


  {
  private:
    PipeWork* m_pParentPipeWork;
    PipeSpec* m_pPipeSpec; // specification of this pipework
    Pipeline* m_pPipeline; // which pipeline
  
    Staging* m_pStaging;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PipeWork(PipeWork* pParentPipeWork,
             PipeSpec* pPipeSpec,
             Pipeline* pPipeline);
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
    i8   nStages(void);
    Staging* pStageSet(void);
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
