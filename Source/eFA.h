/*============================================================================*/
/* eFA.h -- Function Applied                                                  */
/*============================================================================*/
#pragma once

enum eFA // function applied
{
  /*  0 */ FA_NONE=0,
  
  /*    */ FA_AVG,
  /*    */ FA_COUNT,
  /*    */ FA_CUSTOM,
  /*    */ FA_MAX,
  /*    */ FA_MIN,
  /*    */ FA_STDDEV,
  /*    */ FA_SUM,
  /*    */ FA_VAR,

  /*    */ FA_ENUMS
};

static Stz* aFA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "FA_AVG"_Stz,
  /*    */ "FA_COUNT"_Stz,
  /*    */ "FA_CUSTOM"_Stz,
  /*    */ "FA_MAX"_Stz,
  /*    */ "FA_MIN"_Stz,
  /*    */ "FA_STDDEV"_Stz,
  /*    */ "FA_SUM"_Stz,
  /*    */ "FA_VAR"_Stz,

  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
