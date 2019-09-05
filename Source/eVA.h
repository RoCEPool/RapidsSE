/*============================================================================*/
/* eVA.h -- Service Actions                                                   */
/*============================================================================*/
#pragma once

enum eVA // service actions
{
  /*  0 */ VA_NONE=0,
  
  /*    */ VA_SVC_RECEIVED_JOB,
  /*    */ VA_SVC_RESPONSE_JOB,
  /*    */ VA_SVC_SPAWN_JOB,

  /*    */ VA_ENUMS,
};

static Stz* aVA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "VA_SVC_RECEIVED_JOB"_Stz,
  /*    */ "VA_SVC_RESPONSE_JOB"_Stz,
  /*    */ "VA_SVC_SPAWN_JOB"_Stz,

  /*    */ "ENUMS"_Stz,
};

static Stz* aVA_short[]
{
  /*  0 */ "NONE"_Stz,
    
  /*  0 */ "Received job"_Stz,
  /*  0 */ "Received job+response"_Stz,
  /*  0 */ "Spawn job"_Stz,

  /*    */ "ENUMS"_Stz,
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
