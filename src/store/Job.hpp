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

#include "DeleteHelper.hpp"


/*                      IMPORTANT NOTE
 
    If you add a pointer member into this class then modify
      JOBS_fn(vJob_postRecv) to set it to null when the job is received
 
 */

namespace STORE
{
  class Job : public Object,
              public CRM<Job>,
              public DeleteHelper
  {
  public:
  
    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // universally immutable information                    UNIVERSAL

    // -- id, name and kind
    Gid<CN_JOB> m_xJobId;    // this job's id
    s32         m_sJobNm;    // name of job
    eJK         m_eJobKind;  // job kind: (needs work)

    eSS         m_eSeed;  // sen word to start execution
    Word*       m_pSeedWord; // seed word of this job                  POST_RECV

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
    bool m_bNormalSides;   // normal side semantics?
  
    // -- state
    eJS m_eJobState;  // current state: JS_ACTIVE, _LOCKED, ...
    eJP m_eJobPhase;  // current phase: JP_BEGINNING, _BEGUN, ...
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
    Pipeline* m_pPipeline_job;  // pipeline the job represents         POST_RECV
    Op*       m_pSingularOp;    //   or op (api input op )             POST_RECV
  public:
  
    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // job running as responder on origin                   RESPONDER
  
    // -- responding to which originator
    Task*  m__dpOriginatingTask; // dist ptr of originating task of the request
    i8     m_iOriginatingId;     // job id of originating job

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // job execution                                        execution

    // -- tasking
    Task* m_pTask;  // task executing this job                         POST_RECV

    // -- logic unit
    LogicUnit* m_pJobLogicUnit;  // set of registers for task steps    POST_RECV

  private:
    
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
    void vDeleteThis(Object* pDeleter, Stz* szFunc);
    bool bDiaryTag(void);
    void vDotPrint(Stz* szText);
    void vOutline(s4K& sOutline);
    bool bHasTag(Stz* szTagNm);
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
    void vNewJobLogicUnit(void);
    void vNullJobLogicUnit(void);
    Stz* szNodeNm(void);
    bool bNormalSides(void);
    void vNormalSides(bool bNormalSides);
    void vOperandQueueReport(File* pFile, bool bStdOut);
    i8   iOriginatorNode(void);
    i8   iParticipantNode(void);
    void vParticipantNode(i8 iNode);
    Pipeline* pPipeline_job(void);
    void vPipeline_job(Pipeline* pPipeline_job);
    void vReport(void);
    void vResetForCascade(Session* pSession);
    void vSee(void);
    eSS  eSeed(void);
    Stz* szSeed(void);
    void vSeed(eSS eSeed);
    Word* pSeedWord(void);
    void vSeedWord(Word* pSeedWord);
    Session* pSession(void);
    void vSetSeedWord(void);
    void vShow(void);
    Op*  pSingularOp(void);
    void vSingularOp(Op* pSingularOp);
    bool bSummaryTag(void);
    void vSetFromCascade(Stz* szCascade);
    Stz* szSide(void);
    void vSignalJobMutex(void);
    Task* pTask(void);
    void vTask(Task* pTask);
    i8   iTimerMsec(void);
    void vTimerStart(void);
    void vTimerUpdate(void);
    void vUnlockJobMutex(void);
    void vVerifyJobComposition(void);
    void vWaitJobMutex(void);
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

  bool bJobCompare(void* aJobInsert, void* aJobScan, bool& bMatch);
}


typedef void* (*pFUN)(void*);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
