/*============================================================================*/
/* Job.hpp -- a job in the system                                             */
/*============================================================================*/
#pragma once

#include "eJK.h"
#include "eJL.h"
#include "eJS.h"
#include "eJP.h"
#include "eRS.h"

#include "LifeGuard_t.hpp"


/*                      IMPORTANT NOTE
 
    If you add a pointer member into this class then modify
      JOBS_fn(vJob_postRecv) to set it to null when the job is received
 
 */

namespace STORE
{
  class Job : public Operand,
              public CRM<Job>,
              public LifeGuard_t<Job>
  {
  public:
  
    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // universally immutable information                    UNIVERSAL

    // -- id, name and kind
    JobId       m_tJobId;     // this job's id
    s32         m_sJobNm;     // name of job
    eJK         m_eJobKind;   // job kind: (needs work)

    b8          m_bReapable;  // can job be reaped?
    Atomic      m_noActiveOps; // number of active ops

  public:


    // -- command and tags
    s4K    m_sCmd;      // the command
    TagSet m_oJobTags;  // tags for the job

    // -- home is
    i8 m_i_Origin;   // which node
    i8 m_i_Session;  // which session in node's session array
    i8 m_iCmdNo;     // sequence no of the command from that session
  
    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // information depending on job instance/location           MODAL
  
    // -- postage
    bool m_bWasReceived;   // was job received over a socket?          POST_RECV
    i8   m_i_Participant;  // participant node no (FromNodeNo?)
  
    // -- state
    eJS m_eJobState;  // current state: JS_ACTIVE, _LOCKED, ...
    eJP m_eJobPhase;  // current phase: JP_BEGINNING, begun, ...
    eJL m_eJobList;   // current list : JL_ORIGINATING, ...            POST_RECV

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // job running as originator on origin                 ORIGINATOR
  
    // -- timer
    Timer   m_oTimer;        // timer (on origin only)
    Decider m_oAcksRmg_job;  // acks remaining for coordinating 2pc job
    
    eEC     m_eError_job;    // error when starting job
                             // DOUBLE CHECK THIS BELONGS HERE

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // job running on participant node                    PARTICIPANT

  private:
    // -- participating job maintains
    Op*       m_pSingularOp;    //   or op (api input op )             POST_RECV
  public:


    // -- logic unit
    LogicUnit* m_pJobLogicUnit;  // set of registers for task steps    POST_RECV

  private:
    // -- tasking
    Task* m_pTask;  // task executing this job                         POST_RECV
  
    s4K m_sEntry;   // sen entry code
  
    i8  m_iParticipantTsc;

    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Job(Stz* szJobNm,
        i8 i_Origin,   // originated on which node
        i8 i_Session,  // which session in session array
        i8 iCmdNo      // the sequence no of the request from that session
       );

   ~Job(void) noexcept(false);
  
    static void vPostRecv(void* aJob);
    static void vPrintJobNode(SkipNode* pNode, int i_Level);

    i8   iAcksOriginal(void);
    i8   iAcksRmg_printOnlyValue(void);
    void vAcksRmg(i8 nAcks);
    bool bAcksRmgDec(i8& nAcksWereRemaining);
    i8   nActiveOps(void);
    bool bByPromptTag(void);
    void vDecActiveOps(void);
    void vDotPrint(Stz* szText);
    Stz* szEntry(void);
    void vEntry(Stz* szEntry);
    bool bHasTag(Stz* szTagNm);
    void vIncActiveOps(void);
    bool bIsPipeline(void);
    bool bIsPipelineLocal(void);
    JobId tJobId(void);
    void vJobError(eEC eErrorCode);
    eEC  eJobError(void);
    bool bJobHasError(void);
    eJK  eJobKind(void);
    void vJobKind(eJK eJobKind);
    eJL  eJobList(void);
    void vJobList(eJL eJobList);
    Stz* szJobNm(void);
    void vJobNm(Stz* szJobNm);
    eJP  eJobPhase(void);
    void vJobPhase(eJP eJobPhase);
    eJS  eJobState(void);
    void vJobState(eJS eJobState);
    TagSet* pJobTags(void);
    LogicUnit* pJobLogicUnit(void);
    void vLockJobMutex(void);
    void vNewJobLogicUnit(void);
    void vNullJobLogicUnit(void);
    Stz* szNodeNm(void);
    i8   iOriginatorNode(void);
    i8   iParticipantNode(void);
    void vParticipantNode(i8 i_Node);
    i8   iParticipantTsc(void);
    void vParticipantTsc(i8 iTsc);
    Pipeline* pPipeline_job(void);
    void vPrintJobContents(void);
    void vPrintJobLine(void);
    bool bReapable(void);
    void vReapable(bool bReapable);
    void vResetForCascade(Session* pSession);
    Session* pSession(void);
    Op*  pSingularOp(void);
    void vSingularOp(Op* pSingularOp);
    void vSetFromCascade(Stz* szCascade);
    void vSignalJobMutex(void);
    Task* pJobTask(void);
    void vJobTask(Task* pTask);
    i8   iTimerMsec(void);
    void vTimerStart(void);
    void vTimerUpdate(void);
    void vUnlockJobMutex(void);
    void vVerifyJobComposition(void);
    void vWaitJobMutex(void);
    void vWalk(void* aArg);
    void vWalkLambda(
           i8 iDepth,
           std::function<void(Object* pObject, i8 iDepth)> f_vClosure);
    bool bWasReceived(void);
    void vWasReceived(bool bWasReceived);
  
  private:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job* _pJob(void* aAddr);

  void vJob_postRecv(Job* pJob);
}


typedef void* (*pFUN)(void*);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
