/*============================================================================*/
/* Formwork.hpp -- Formwork                                                   */
/*============================================================================*/
#pragma once

#include "LifeGuard_t.hpp"

namespace STORE { class Stage; class FormworkSpec; }

#ifdef WITH_FORMWORK

namespace STORE
{
  class Formwork : public VectorNL_t<CN_PIPELINE,Stage,C_MAX_STAGES>,
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
    void vSpawnOpTasks(i8 iSessionId, i8 iNode);

    void vCreateFormwork();

  private:
    Footer* aFooter(void);
  };
  
  Formwork* _pFormwork(void* aAddr);
}

// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#else

namespace STORE
{
  class Formwork : public VectorNL_t<CN_PIPELINE,Stage,C_MAX_STAGES>,
                   public LifeGuard_t<Formwork>

  {
  private:

    // -- id, name and kind
    Gid_t<CN_PIPELINE> m_xFormworkId;  // pipeline id in grid
    s32              m_sFormworkNm;  // name of this pipeline
    // n/a                           // kind

    // -- belongs to
    Job* m_pFormworkJob;  // which job
  
    // -- refers to
    FormworkSpec* m_pFormworkSpec; // specification of this pipeline
  
    i8   m_i_Origin;    // originating node
  
    // -- -- -- -- -- --

    Atomic m_ixAccum0;
    Atomic m_ixAccum1;
    Atomic m_ixAccum2;
    Lock   m_okDebugLock;

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
    
    Formwork(Job* pFormworkJob, FormworkSpec* pFormworkSpec, i8 i_Origin);
   ~Formwork(void) noexcept(false);
  
    void vAddAccum(i8 i_Accum, i8 iValue);
    bool bCancel(Task* pTriggeringTask);
    void vCancelled(bool bCancelled);
    bool bCancelled(void);
    void vDropDebugLock(void);
    bool bEarlyExit(void);
    void vEarlyExit(bool bEarlyExit);
    bool bErrored(void);
    void vErrored(bool bErrored);
    i8   iGetAccum(i8 i_Accum);
    void vGrabDebugLock(void);
    i8   i_Origin(void);
    Op*  pOp0(void);
    i8   nOpsOnNode(i8 iNode);
    Op*  pOpToPulse(void);
    Job* pFormworkJob(void);
    Stz* szFormworkNm(void);
    FormworkSpec* pFormworkSpec(void);
    bool bPulsed(void);
    void vPulsed(bool bPulsed);
    bool bPulsedLimit(void);
    void vSetupJob(void);
    void vSpawnOpTasks(i8 iSessionId, i8 iNode);

  private:
    void vCreateStages();

    Footer* aFooter(void);
  };
  
  Formwork* _pFormwork(void* aAddr);
}

#endif // WITH_FORMWORK

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
