/*============================================================================*/
/* Job.hpp -- a job in the system                                             */
/*============================================================================*/
#pragma once

#include "eJK.h"
#include "eJL.h"
#include "eJS.h"
#include "eJP.h"
#include "eRS.h"
#include "eSS.h"

namespace STORE
{
  class Job : public Object,
              public CRM<Job>
  {
  
  /*                      IMPORTANT NOTE
   
      If you add a pointer member into this class then modify
        JOBS_fn(vJob_postRecv) to set it to null when the job is received
   
   */
  
  public:
    Gid<CN_JOB> m_xJobId;    // this job's id


    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
  
    Pipeline* m_pPipeline_job; // which pipeline
    Op*       m_pSingularOp;   // e.g. api input op for a pipeline

    // -- a -------------------
  
    i8 m_i_Origin;     // originated on which node
    i8 m_i_Session;    // which session in session array
    i8 m_iCmdNo;       // the sequence no of the request from that session

  
  
    s32      m_sJobNm;       // name of job
    Timer    m_oTimer;       // timer

    // -- b -------------------
    bool m_bWasReceived;     // job was received over a socket
    i8   m_iParticipantNode; // participant node no (FromNodeNo?)
    bool m_bNormalSides;     // side semantics

    // -- c -------------------
    eJK m_eJobKind;          // kind of job
    eJL m_eJobList;          // which job list the job is currently in
    eJP m_eJobPhase;         // which job phase the job is currently in
    eJS m_eJobState;         // current state of the job
    eEC m_eError;            // error when starting job
  
    // -- d -------------------
  private:
    LogicUnit* m_pJobLogicUnit;  // set of registers for the task steps
  
  public:
    TagSet     m_oJobTags;   // tags for the job
    
    // -- e -------------------

    // -- f -------------------
    Atomic m_ixErrors;
    Task*  m_pOriginatingTask; // originating task of the request
    i8     m_iOriginatingId;   // job id of originating job
    
    // -- g -------------------
    Decider m_oAcksRmg_2pc;  // acks remaining for coordinating 2pc job
    eSS     m_eSeed;         // seed to spawn
    Task*   m_pTask;         // task executing this job

    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
    
    Job(i8 i_Origin,     // originated on which node
        i8 i_Session,    // which session in session array
        i8 iCmdNo  // the sequence no of the request from that session
       );

    // FIND CASE -- TO BE REMOVED
    Job(eCN eClass);

   ~Job(void) noexcept(false);
  
    static Job* pThisTLSJob(void);

    static bool bIsForeign(i8 iJobId);
    static i8   iJobNo(i8 iJobId);
    static i8   iNodeNo(i8 iJobId);
    static i8   iOriginatorNode(i8 iJobId);
    static i8   iAsNodeNo_Bits(i8 iNodeNo);
  
    static void vPostRecv(void* aJob);

    i8   iAcksOriginal(void);
    i8   iAcksRmg_printOnlyValue(void);
    void vAcksRmg(i8 nAcks);
    bool bAcksRmgDec(i8& nAcksWereRemaining);
    bool bByPromptTag(void);
    void vContentDump(void);
    bool bDiaryTag(void);
    void vDotPrint(Stz* szText);
    void vOutline(s4K& sOutline);
    bool bHasTag(Stz* szEnameTag);
    bool bHasTag_bg(void);
    bool bHasTag_time(void);
    bool bHasTag_noWait(void);
    bool bIsPipeline(void);
    i8   iJobId(void);
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
    void vOperandQueueReport(File* pFile, bool bStdOut);
    Pipeline* pPipeline(void);
    eEC ePopulateJob(TagSet* pTagSet);
    void vResetForReuse(Session* pSession);
    void vSetFromCascade(Stz* szCascade);
    Stz* szSide(void);
    void vSignalJobMutex(void);
    void vUnlockJobMutex(void);
    void vWaitJobMutex(void);
    void vNewJobLogicUnit(void);
    void vNullJobLogicUnit(void);
    Stz* szNodeNm(void);
    bool bNormalSides(void);
    void vNormalSides(bool bNormalSides);
    i8   iOriginatorNode(void);
    i8   iParticipantNode(void);
    void vParticipantNode(i8 iNode);
    void vReport(void);
    eSS  eSeed(void);
    Stz* szSeed(void);
    void vSeed(eSS eSeed);
    Session* pSession(void);
    void vShow(void);
    Op*  pSingularOp(void);
    void vSingularOp(Op* pSingularOp);
    bool bSummaryTag(void);
    Task* pTask(void);
    void vTask(Task* pTask);
    i8   iTimerMsec(void);
    void vTimerStart(void);
    void vTimerUpdate(void);
    void vVerifyJobComposition(void);
    bool bWasReceived(void);
    void vWasReceived(bool bWasReceived);

  protected:
    //void vShow(s48K& s);
  
  private:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job* _pJob(void* aAddr);
  Job* _pJob(i8 iAddr);
  bool _bIsValidJob(eCN eClass);

  ////////////////////////////////////////////////////////////////////////////////
  eEC  eJob_checkCluster(void);
  void vJob_postRecv(Job* pJob);
  void vJob_preSend(Job* pJob);

  bool bJobCompare(void* aJobInsert, void* aJobScan, bool& bMatch);
}


typedef void* (*pFUN)(void*);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
