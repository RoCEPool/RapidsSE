/*============================================================================*/
/* SysCall.hpp - Library Call                                                  */
/*============================================================================*/
#pragma once

#include "eSC.h"

namespace Resources
{
  class SysCall : public Object
  {
  public:
    eSC  m_eStatus;  // return status
    i8   m_iErrNo;   // error number
    
    // must be last
    Footer m_xFooter;
    
  public:
    SysCall(void);
    ~SysCall(void) noexcept(false);
    
    void vClear(void);
    void vError(i8 iErrno);
    i8   iError(void);
    void vFailure(i8 iErrno);
    void vSuccess(void);
    
    bool bHadError(void);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
