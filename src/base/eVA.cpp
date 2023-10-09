/*============================================================================*/
/* eVA.h -- Service Actions                                                   */
/*============================================================================*/
#include "BASE.h"
#include "eVA.h"

Stz* aVA[VA_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "VA_SVC_RECEIVED_JOB"_Stz,
  /*    */ "VA_SVC_RESPONSE_JOB"_Stz,
  /*    */ "VA_SVC_SPAWN_JOB"_Stz,

//  /*    */ "ENUMS"_Stz,
};

Stz* aVA_short[VA_ENUMS]
{
  /*  0 */ "NONE"_Stz,
    
  /*  0 */ "Received job"_Stz,
  /*  0 */ "Received job+response"_Stz,
  /*  0 */ "Spawn job"_Stz,

//  /*    */ "ENUMS"_Stz,
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
