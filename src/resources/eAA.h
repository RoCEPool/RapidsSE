/*============================================================================*/
/* eAA.h -- Api Actions                                                       */
/*============================================================================*/
#pragma once

enum eAA // api actions
{
  /*  0 */ AA_NONE=0,
  
  /*    */ AA_API_ERROR,
  /*    */ AA_API_ENTRY,
  /*    */ AA_API_EXIT,
  /*    */ AA_API_SPAWN_JOB,

  /*    */ AA_SSN_CONSTRUCT,
  /*    */ AA_SSN_DESTRUCT,

  /*    */ AA_ENUMS,
};

static Stz* aAA[]
{
  /*  0 */ "NONE"_Stz,

  /*    */ "AA_API_ERROR"_Stz,
  /*    */ "AA_API_ENTRY"_Stz,
  /*    */ "AA_API_EXIT"_Stz,
  /*    */ "AA_API_SPAWN_JOB"_Stz,

  /*    */ "AA_SSN_CONSTRUCT"_Stz,
  /*    */ "AA_SSN_DESTRUCT"_Stz,
  
  /*    */ "ENUMS"_Stz,
};

static Stz* aAA_short[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "Api error"_Stz,
  /*    */ "Api entry"_Stz,
  /*    */ "Api exit"_Stz,
  /*    */ "Api spawn job"_Stz,

  /*    */ "Session construct"_Stz,
  /*    */ "Session destruct"_Stz,
  
  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
