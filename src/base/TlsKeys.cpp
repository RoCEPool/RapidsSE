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
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
