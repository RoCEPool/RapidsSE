/*============================================================================*/
/* ePA.h -- PROMPT Actions                                                    */
/*============================================================================*/
#pragma once

enum ePA // prompt actions
{
  /*  0 */ PA_PRM_NONE=0,

  /*    */ PA_PRM_COMMAND,
  /*    */ PA_PRM_DELETE_JOB,
  /*    */ PA_PRM_FREE_SPACE,
  /*    */ PA_PRM_LOG_NOTE,
  /*    */ PA_PRM_NEW_JOB,
  /*    */ PA_PRM_RESUMES,
  /*    */ PA_PRM_WAITS,

  /*    */ PA_PRM_ENUMS,
};

static Stz* aPA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "PA_PRM_COMMAND"_Stz,
  /*    */ "PA_PRM_DELETE_JOB"_Stz,
  /*    */ "PA_PRM_FREE_SPACE"_Stz,
  /*    */ "PA_PRM_LOG_NOTE"_Stz,
  /*    */ "PA_PRM_NEW_JOB"_Stz,
  /*    */ "PA_PRM_RESUMES"_Stz,
  /*    */ "PA_PRM_WAITS"_Stz,

  /*    */ "ENUMS"_Stz,
};

static Stz* aPA_short[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "PROMPT command"_Stz,
  /*    */ "PROMPT delete job"_Stz,
  /*    */ "PROMPT free space"_Stz,
  /*    */ "PROMPT log note"_Stz,
  /*    */ "PROMPT new job"_Stz,
  /*    */ "PROMPT resumes"_Stz,
  /*    */ "PROMPT waits"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
