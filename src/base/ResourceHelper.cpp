/*============================================================================*/
/* ResourceHelper.cpp -- Resource Helper                                      */
/*============================================================================*/

#include "BASE.h"

#include "Resources.hpp" //?

/*----------------------------------------------------------------------------*/
/* summarize resource usage                                                   */
/*----------------------------------------------------------------------------*/
void ResourceHelper::vResourceSummary(void)
{
  _static_

  printf("\n");
  printf("****************************************************************\n");
  printf((char*)"  BASE: dir %lld file %lld shm %lld socket %lld thread %lld\n"_Stz,
         AM::pResources()->nDirOpen(),
         AM::pResources()->nFileOpen(),
         AM::pResources()->nShmOpen(),
         AM::pResources()->nSocketOpen(),
         AM::pResources()->nThreadOpen());
  printf("****************************************************************\n");
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
