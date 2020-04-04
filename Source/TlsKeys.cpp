/*============================================================================*/
/* TlsKeys.hpp -- Thread Local Storage Keys                                   */
/*============================================================================*/

#include "Resources.h"

extern pthread_key_t giTLS_TaskKey;
extern pthread_key_t giTLS_ThreadKey;

/*----------------------------------------------------------------------------*/
/* initialise tls                                                             */
/*----------------------------------------------------------------------------*/
void TlsKeys::vInit(void)
{
  bool b;
  
  ASSERT(C_UNKNOWN == (i8)giTLS_TaskKey);
  b = PsxThd::bTlsCreate(&giTLS_TaskKey);
  ASSERT(b);
  
  ASSERT(C_UNKNOWN == (i8)giTLS_ThreadKey);
  b = PsxThd::bTlsCreate(&giTLS_ThreadKey);
  ASSERT(b);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
