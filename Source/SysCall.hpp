/*============================================================================*/
/* SysCall.hpp - System Call                                                  */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common
{
  enum eSysCall
  {
    /* -2 */ eSysCall_NOTSET  = -2, // status code not yet set
    /* -1 */ eSysCall_ERROR   = -1, // error status
    /* 0  */ eSysCall_FAILURE = 0,  // failure status
    /* 1  */ eSysCall_SUCCESS = 1,  // success status
  };
  
  static const char* const pSysCall[]
  {
    /* -2 */ "NOTSET",
    /* -1 */ "ERROR",
    /* 0  */ "FAILURE",
    /* 1  */ "SUCCESS"
  };

  class SysCall : public Header
  {
  public:
    cn       m_nClassNo; // class number
    eSysCall m_eStatus;  // return status
    i8       m_iErrNo;   // error number
    
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