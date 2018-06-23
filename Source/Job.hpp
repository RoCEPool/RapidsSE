/*============================================================================*/
/* Job.hpp -- a job in the system                                             */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  class Job;
  
  static const i8 C_MAX_COLUMNS = 128;
  static const Job* C_JOB_MARKER = (Job*)0xFFFFFFFFFFFFFFFD;
  
  enum eJS // job state
  {
    /*  0 */ JS_NONE=0,
    /*  1 */ JS_BEGUN,
    /*  2 */ JS_EXECUTED,
    /*  3 */ JS_ENDED,
    /*  4 */ JS_ENUMS
  };
  
  static Stz* aTH[]
  {
    /*  0 */ "NONE"_Stz,
    /*  1 */ "BEGUN"_Stz,
    /*  2 */ "EXECUTED"_Stz,
    /*  3 */ "ENDED"_Stz,
    /*  4 */ "ENUMS"_Stz
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job : public Object
  {
  public:
    i8  m_iJobId;              // job id
    eJS m_eState;              // job state
    s32 m_sExecute;            // word to be executed
    i8  m_iParticipantNode;    // participant node no
    Atomic m_ziAcksRemaining;  // acks remaining
  
  public:
    Job(eCN eClassNo);
    ~Job(void);
    
    static i8 iOriginatorNode(i8 iJobId);
    
    Stz* sJobExecute(void);
    void vJobExecute(Stz* sExecute);
    i8   iJobId(void);
    void vJobId(i8 iJobId);
    i8   iOriginatorNode(void);
    i8   iParticipantNode(void);
    void vParticipantNode(i8 iNode);
    eJS  eJobState(void);
    void vJobState(eJS eJobState);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job* _pJob(void* aAddr);
}

bool bJobCompare(void* pObjectInsert, void* pObjectScan, bool& bMatch);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
