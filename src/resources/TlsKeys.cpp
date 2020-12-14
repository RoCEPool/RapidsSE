/*============================================================================*/
/* TlsKeys.cpp -- Thread Local Storage Keys                                   */
/*============================================================================*/

#include "Resources.h"

#include "TlsKeys.hpp" //?
#include "PsxThd.hpp"  //?

extern pthread_key_t giTLS_TaskKey;
extern pthread_key_t giTLS_ThreadKey;

/*----------------------------------------------------------------------------*/
/* initialise tls                                                             */
/*----------------------------------------------------------------------------*/
void TlsKeys::vInit(void)
{
  bool b;
  
  //vTrace(" TlsKeys::vInit A\n"_Stz);
  //ASSERT(C_UNKNOWN == (i8)giTLS_TaskKey);
  //vTrace(" TlsKeys::vInit B\n"_Stz);
  b = PsxThd::bTlsCreate(&giTLS_TaskKey);
  //vTrace(" TlsKeys::vInit C\n"_Stz);
  RASSERT(b);
  //vTrace(" TlsKeys::vInit D\n"_Stz);

  //ASSERT(C_UNKNOWN == (i8)giTLS_ThreadKey);
  //vTrace(" TlsKeys::vInit E\n"_Stz);
  b = PsxThd::bTlsCreate(&giTLS_ThreadKey);
  //vTrace(" TlsKeys::vInit F\n"_Stz);
  RASSERT(b);
  //vTrace(" TlsKeys::vInit G\n"_Stz);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
