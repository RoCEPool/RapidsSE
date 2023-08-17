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

extern Stz* aPA[PA_PRM_ENUMS];
extern Stz* aPA_short[PA_PRM_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
