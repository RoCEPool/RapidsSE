/*============================================================================*/
/* eTM.h -- Type Mapping shortcuts                                            */
/*============================================================================*/
#pragma once

namespace Parser
{
  enum eTM
  {
    /*  0 */ TM_NONE = 0,
    /*  1 */ TM_ALL_NUMERICS,      // meaning CT_INTEGER, CT_DECIMAL and CT_FLOAT
    /*  2 */ TM_ALL_STRINGS,       // meaning CT_CHAR and CT_VARCHAR
    /*  3 */ TM_ALL_YM_INTERVALS,  // meaning YEAR, MONTH and YEAR TO MONTH intervals
    /*  4 */ TM_ALL_DT_INTERVALS,  // meaning DAY, HOUR, MINUTE, SECOND, DAY TO HOUR...
    /*  5 */ TM_ALL_NON_UNKNOWN,   // meaning all data types except CT_UNKNOWN
    /*  6 */ TM_ALL_INC_UNKNOWN,   // meaning all data types including CT_UNKNOWN
    /*  7 */ TM_SAME,              // meaning same type as the other parameter given
    /*  8 */ TM_ENUMS
  };

  static const Stz* aTM[]
  {
      /*  0 */ "NONE"_Stz,
      /*  1 */ "TM_ALL_NUMERICS"_Stz,
      /*  2 */ "TM_ALL_STRINGS"_Stz,
      /*  3 */ "TM_ALL_YM_INTERVALS"_Stz,
      /*  4 */ "TM_ALL_DT_INTERVALS"_Stz,
      /*  5 */ "TM_ALL_NON_NULL"_Stz,
      /*  6 */ "TM_ALL"_Stz,
      /*  7 */ "TM_SAME"_Stz,
      /* 13 */ "ENUMS"_Stz
  };
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
