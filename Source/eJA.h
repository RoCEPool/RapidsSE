/*============================================================================*/
/* eJA.h -- Job Actions                                                       */
/*============================================================================*/
#pragma once

enum eJA // job actions
{
  /*  0 */ JA_NONE=0,
  
  /*    */ JA_JOB_ADD_ORIGIN,
  /*    */ JA_JOB_ADD_PARTG,
  /*    */ JA_JOB_CONSTRUCT,
  /*    */ JA_JOB_DESTRUCT,
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
  
  /*  0 */ "JA_JOB_ADD_ORIGIN"_Stz,
  /*  0 */ "JA_JOB_ADD_PARTG"_Stz,
  /*  0 */ "JA_JOB_CONSTRUCT"_Stz,
  /*  0 */ "JA_JOB_DESTRUCT"_Stz,
  /*  0 */ "JA_JOB_DUMP_ORIGINATING"_Stz,
  /*  0 */ "JA_JOB_DUMP_PARTICIPATING"_Stz,
  /*  0 */ "JA_JOB_FIND_ORIGINATING"_Stz,
  /*  0 */ "JA_JOB_FIND_PARTICIPATING"_Stz,
  /*  0 */ "JA_JOB_INF"_Stz,
  /*  0 */ "JA_JOB_REMOVE_ORIGINATING"_Stz,
  /*  0 */ "JA_JOB_REMOVE_PARTICIPATING"_Stz,

  /*    */ "ENUMS"_Stz,
};

static Stz* aJA_short[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "Job add originating"_Stz,
  /*    */ "Job add participating"_Stz,
  /*    */ "Job construct"_Stz,
  /*    */ "Job destruct"_Stz,
  /*    */ "Job dump originating"_Stz,
  /*    */ "Job dump participating"_Stz,
  /*    */ "Job find originating"_Stz,
  /*    */ "Job find participating"_Stz,
  /*    */ "Job information"_Stz,
  /*    */ "Job remove originating"_Stz,
  /*    */ "Job remove participating"_Stz,

  /*    */ "ENUMS"_Stz,
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
