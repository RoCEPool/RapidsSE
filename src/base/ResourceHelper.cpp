/*============================================================================*/
/* ResourceHelper.cpp -- Resource Helper                                      */
/*============================================================================*/

#include "Base.h"

#include "ResourceTracker.hpp" //?

/*----------------------------------------------------------------------------*/
/* summarize resource usage                                                   */
/*----------------------------------------------------------------------------*/
void ResourceHelper::vResourceSummary(void)
{
  _static_

  printf("\n");
  printf("****************************************************************\n");
  printf((char*)"  BASE: dir %lld file %lld shm %lld socket %lld thread %lld\n"_Stz,
         AM::pResourceTracker()->nDirOpen(),
         AM::pResourceTracker()->nFileOpen(),
         AM::pResourceTracker()->nShmOpen(),
         AM::pResourceTracker()->nSocketOpen(),
         AM::pResourceTracker()->nThreadOpen());
  printf("****************************************************************\n");
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
