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
#include "JobMetrics.hpp"


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
  private:
    Lock m_okLock;
  
  public:
  
    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // universally immutable information                    UNIVERSAL

    // -- id, name and kind
    JobId       m_tJobId;     // this job's id
    s32         m_sJobNm;     // name of job
    eJK         m_eJobKind;   // job kind: (needs work)

    Atomic      m_noActiveOps; // number of active ops



    // -- command and tags
  private:  
    TagSet m_oJobTags;  // tags for the job

  public:
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

  
  private:
    // -- tasking
    Task* m_pTask;  // task executing this job                         POST_RECV
  
    s4K m_sEntry;   // sen entry code
  
    i8  m_iTscParticipant;

    i8  m_iTscBeginning;
    i8  m_iTscBegun;
    i8  m_iTscExecuting;
    i8  m_iTscExecuted;
    i8  m_iTscEnding;
    i8  m_iTscEnded;
    i8  m_iTscCanceling;
    i8  m_iTscCanceled;

    Timer m_oProgressTimer;
 
    b8  m_bReapable;  // can job be reaped?
  
    JobMetrics* m_pJobMetricsChain;                                // POST_RECV

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
  
    Stz* szCmd(void);
    void vCmd(Stz* szCmd);

    void vDecActiveOps(void);
    void vDotPrint(Stz* szText);
    void vDropLock(void);

    Stz* szEntry(void);
    void vEntry(Stz* szEntry);
  
    void vGrabLock(void);
  
    bool bHasTag(Stz* szTagNm);
  
    void vIncActiveOps(void);
    bool bIsOriginator(void);
    bool bIsParticipant(void);
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
    void vJobMetricsAdd(JobMetrics* pJobMetrics);
    JobMetrics* pJobMetricsChain(void);
    Stz* szJobNm(void);
    void vJobNm(Stz* szJobNm);
    eJP  eJobPhase(void);
    void vJobPhase(eJP eJobPhase);
    eJS  eJobState(void);
    void vJobState(eJS eJobState);
    TagSet* pJobTags(void);

    Stz* szNodeNm(void);
  
    i8   iOriginatorNode(void);
  
    i8   iParticipantNode(void);
    void vParticipantNode(i8 i_Node);
    Pipeline* pPipeline_job(void);
    void vPrintJobContents(void);
    void vPrintJobLine(void);
    void vPostRecv(void);
    Timer*  pProgressTimer(void);
  
    bool bReapable(void);
    void vReapable(bool bReapable);
    void vResetForCascade(Session* pSession);

    Session* pSession(void);
    Op*  pSingularOp(void);
    void vSingularOp(Op* pSingularOp);
    void vSetFromCascade(Stz* szCascade);
    void vSignalJobMutex(void);
    void vSpawnTask(void);

    Task* pTask(void);
    void vTask(Task* pTask);

    i8   iTimerMsec(void);
    i8   iTimerStart(void);
    void vTimerStart(void);
    void vTimerUpdate(void);
  
    i8   iTscBeginning(void);
    void vTscBeginning(i8 iTsc);

    i8   iTscBegun(void);
    void vTscBegun(i8 iTsc);

    i8   iTscExecuting(void);
    void vTscExecuting(i8 iTsc);

    i8   iTscExecuted(void);
    void vTscExecuted(i8 iTsc);

    i8   iTscEnding(void);
    void vTscEnding(i8 iTsc);

    i8   iTscEnded(void);
    void vTscEnded(i8 iTsc);

    i8   iTscCanceling(void);
    void vTscCanceling(i8 iTsc);
  
    i8   iTscCanceled(void);
    void vTscCanceled(i8 iTsc);

    i8   iTscParticipant(void);
    void vTscParticipant(i8 iTsc);
    
    void vVerifyJobComposition(void);
  
    void vWaitJobMutex(void);
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
