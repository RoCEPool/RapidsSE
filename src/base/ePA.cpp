/*============================================================================*/
/* ePA.cpp -- PROMPT Actions                                                  */
/*============================================================================*/
#include "BASE.h"
#include "ePA.h"

Stz* aPA[PA_PRM_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "PA_PRM_COMMAND"_Stz,
  /*    */ "PA_PRM_DELETE_JOB"_Stz,
  /*    */ "PA_PRM_FREE_SPACE"_Stz,
  /*    */ "PA_PRM_LOG_NOTE"_Stz,
  /*    */ "PA_PRM_NEW_JOB"_Stz,
  /*    */ "PA_PRM_RESUMES"_Stz,
  /*    */ "PA_PRM_WAITS"_Stz,

//  /*    */ "ENUMS"_Stz,
};

Stz* aPA_short[PA_PRM_ENUMS]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "PROMPT command"_Stz,
  /*    */ "PROMPT delete job"_Stz,
  /*    */ "PROMPT free space"_Stz,
  /*    */ "PROMPT log note"_Stz,
  /*    */ "PROMPT new job"_Stz,
  /*    */ "PROMPT resumes"_Stz,
  /*    */ "PROMPT waits"_Stz,

//  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
