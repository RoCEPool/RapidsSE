/*============================================================================*/
/* TlsKeys.cpp -- Thread Local Storage Keys                                   */
/*============================================================================*/

#include "BASE.h"

#include "TlsKeys.hpp" //?
#include "PsxThd.hpp"  //?

extern pthread_key_t giTLS_ThreadKey;
extern pthread_key_t giTLS_TaskKey;
extern pthread_key_t giTLS_HandlerKey;
extern pthread_key_t giTLS_StepKey;
extern pthread_key_t giTLS_StepAccum;

extern pthread_key_t gbTLS_FaultInjection;
extern pthread_key_t giTLS_FaultPoint;
extern pthread_key_t giTLS_CurrentPoint;
extern pthread_key_t giTLS_PDSCrwTablePtn;

/*----------------------------------------------------------------------------*/
/* initialise tls                                                             */
/*----------------------------------------------------------------------------*/
void TlsKeys::vInit(void)
{
  bool b;
  
  b = PsxThd::bTlsCreate(&giTLS_ThreadKey);
  RASSERT(b);
  
  b = PsxThd::bTlsCreate(&giTLS_TaskKey);
  RASSERT(b);

  b = PsxThd::bTlsCreate(&giTLS_HandlerKey);
  RASSERT(b);

  b = PsxThd::bTlsCreate(&giTLS_StepKey);
  RASSERT(b);


  b = PsxThd::bTlsCreate(&giTLS_StepAccum);
  RASSERT(b);

  b = PsxThd::bTlsCreate(&gbTLS_FaultInjection);
  RASSERT(b);

  b = PsxThd::bTlsCreate(&giTLS_FaultPoint);
  RASSERT(b);

  b = PsxThd::bTlsCreate(&giTLS_CurrentPoint);
  RASSERT(b);

  b = PsxThd::bTlsCreate(&giTLS_PDSCrwTablePtn);
  RASSERT(b);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
