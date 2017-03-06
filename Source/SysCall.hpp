/*============================================================================*/
/* SysCall.hpp - System Call                                                  */
/*============================================================================*/
#pragma once

namespace System
{
  enum eSC // system call
  {
    /* -2 */ SC_NOTSET  = -2, // status code not yet set
    /* -1 */ SC_ERROR   = -1, // error status
    /* 0  */ SC_FAILURE = 0,  // failure status
    /* 1  */ SC_SUCCESS = 1,  // success status
  };
  
  static Str* aSC[]
  {
    /* -2 */ "NOTSET"_Str,
    /* -1 */ "ERROR"_Str,
    /* 0  */ "FAILURE"_Str,
    /* 1  */ "SUCCESS"_Str
  };

  class SysCall : public Object
  {
  public:
    eSC  m_eStatus;  // return status
    i8   m_iErrNo;   // error number
    
    // must be last
    Footer m_xFooter;
    
  public:
    SysCall(void);
    ~SysCall(void);
    
    void vClear(void);
    void vError(i8 iErrno);
    i8   iError(void);
    void vFailure(i8 iErrno);
    void vSuccess(void);
    
    bool bHadError(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
