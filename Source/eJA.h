/*============================================================================*/
/* eJA.h -- Job Actions                                                       */
/*============================================================================*/
#pragma once

enum eJA // job actions
{
  /*  0 */ JA_NONE=0,
  
  /*    */ JA_JOB_ADD_ORIGINATING,
  /*    */ JA_JOB_ADD_PARTICIPATING,
  /*    */ JA_JOB_CONSTRUCTOR,
  /*    */ JA_JOB_DESTRUCTOR,
  /*    */ JA_JOB_DUMP_ORIGINATING,
  /*    */ JA_JOB_DUMP_PARTICIPATING,
  /*    */ JA_JOB_FIND_ORIGINATING,
  /*    */ JA_JOB_FIND_PARTICIPATING,
  /*    */ JA_JOB_INF,
  /*    */ JA_JOB_REMOVE_ORIGINATING,
  /*    */ JA_JOB_REMOVE_PARTICIPATING,

  /*    */ JA_ENUMS,
};

static Stz* aJA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  0 */ "JA_JOB_ADD_ORIGINATING"_Stz,
  /*  0 */ "JA_JOB_ADD_PARTICIPATING"_Stz,
  /*  0 */ "JA_JOB_CONSTRUCTOR"_Stz,
  /*  0 */ "JA_JOB_DESTRUCTOR"_Stz,
  /*  0 */ "JA_JOB_DUMP_ORIGINATING"_Stz,
  /*  0 */ "JA_JOB_DUMP_PARTICIPATING"_Stz,
  /*  0 */ "JA_JOB_FIND_ORIGINATING"_Stz,
  /*  0 */ "JA_JOB_FIND_PARTICIPATING"_Stz,
  /*  0 */ "JA_JOB_INF"_Stz,
  /*  0 */ "JA_JOB_REMOVE_ORIGINATING"_Stz,
  /*  0 */ "JA_JOB_REMOVE_PARTICIPATING"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
