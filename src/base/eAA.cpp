/*============================================================================*/
/* eAA.cpp -- Api Actions                                                     */
/*============================================================================*/

#include "BASE.h"
#include "eAA.h"

Stz* aAA[AA_ENUMS]
{
  /*  0 */ "NONE"_Stz,

  /*    */ "AA_API_ERROR"_Stz,
  /*    */ "AA_API_ENTRY"_Stz,
  /*    */ "AA_API_EXIT"_Stz,
  /*    */ "AA_API_SPAWN_JOB"_Stz,

  /*    */ "AA_SSN_CONSTRUCT"_Stz,
  /*    */ "AA_SSN_DESTRUCT"_Stz,
  
//  /*    */ "ENUMS"_Stz,
};

Stz* aAA_short[AA_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "Api error"_Stz,
  /*    */ "Api entry"_Stz,
  /*    */ "Api exit"_Stz,
  /*    */ "Api spawn job"_Stz,

  /*    */ "Session construct"_Stz,
  /*    */ "Session destruct"_Stz,
  
//  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
