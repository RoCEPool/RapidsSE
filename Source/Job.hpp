/*============================================================================*/
/* Job.hpp -- a job in the system                                             */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  
  static const i8 C_MAX_FOLDERS = 16;
  
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
  
  class Job : public Object
  {
  private:
    i8     m_iJobId;      // job id
    eJS    m_eState;      // job state
    s32    m_sExecute;    // word to be executed
  
  public:
    Job(eCN eClassNo);
    ~Job(void);
    
    Stz* sJobExecute(void);
    void vJobExecute(Stz* sExecute);
    i8   iJobId(void);
    void vJobId(i8 iJobId);
    i8   iJobNode(void);
    eJS  eJobState(void);
    void vJobState(eJS eJobState);
      
  };
  
  Job* _pJob(void* aAddr);
}

bool bJobCompare(void* pObjectInsert, void* pObjectScan, bool& bMatch);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
