/*============================================================================*/
/* Pipeline.hpp -- Pipeline                                                   */
/*============================================================================*/
#pragma once

#include "LifeGuard_t.hpp"


namespace STORE { class Stage; class PipelineSpec; }

namespace STORE
{
  class Pipeline : public VectorNL_t<CN_PIPELINE,Stage,C_MAX_STAGES>,
                   public LifeGuard_t<Pipeline>

  {
  private:
    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // a)

    // -- id, name and kind
    Gid<CN_PIPELINE> m_xPipelineId;  // pipeline id in grid
    s32              m_sPipelineNm;  // name of this pipeline
    // n/a                           // kind

    // -- belongs to
    Job* m_pPipelineJob;  // which job
  
    // -- refers to
    PipelineSpec* m_pPipelineSpec; // specification of this pipeline

    // -- state
    // n/a

    // -- tags, seeding and tasking
    // n/a

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // b)

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // c)

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // d)


    // ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?

  
    i8   m_i_Origin;    // originating node

  
    // -- -- -- -- -- --

  
    bool m_bPulsed;
    bool m_bEarlyExit;
    bool m_bErrored;
    
    bool m_bPulsedLimit;
    Op*  m_pOpToPulse;
    bool m_bCancelled;
        
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Pipeline(Job* pPipelineJob, PipelineSpec* pPipelineSpec, i8 i_Origin);
   ~Pipeline(void) noexcept(false);
  
    bool bCancel(Task* pTriggeringTask);
    void vCancelled(bool bCancelled);
    bool bCancelled(void);
    bool bEarlyExit(void);
    void vEarlyExit(bool bEarlyExit);
    bool bErrored(void);
    void vErrored(bool bErrored);
    i8   i_Origin(void);
    Op*  pOp0(void);
    i8   nOpsOnNode(i8 iNode);
    Op*  pOpToPulse(void);
    Job* pPipelineJob(void);
    Stz* szPipelineNm(void);
    PipelineSpec* pPipelineSpec(void);
    bool bPulsed(void);
    void vPulsed(bool bPulsed);
    bool bPulsedLimit(void);
    void vSetupJob(void);
    void vSpawnOpTasks(i8 iSessionId, i8 iNode);

  private:
    void vCreateStages();

    Footer* aFooter(void);
  };
  
  Pipeline* _pPipeline(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
