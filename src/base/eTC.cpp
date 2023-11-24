/*============================================================================*/
/* eTC.cpp -- Term Evaluation                                                 */
/*============================================================================*/
#include "BASE.h"
#include "eTC.h"

Stz* aTC[TC_ENUMS]
{
  /*  */ "TC_NONE"_Stz,

  /*  */ "TC_VERBOSE"_Stz,
  /*  */ "TC_WLP"_Stz,

//  /*  */ "TC_ENUMS"_Stz
};

Stz* aTCDesc[TC_ENUMS]
{
    /*  */ ""_Stz,

    /*  */ "VERBOSE"_Stz,
    /*  */ "WLP"_Stz,

//  /*  */ "TC_ENUMS"_Stz
};

/*----------------------------------------------------------------------------*/
/* Returns the eTC matching the textual description of the enum, or TC_NONE   */
/*----------------------------------------------------------------------------*/
eTC eFromEnumTC(Stz* szStr)
{
  for (i8 i = TC_NONE + 1; i < TC_ENUMS; i++) {
    Stz* szTcVal = aTCDesc[i];

    if (szTcVal->bSame(szStr)) {
      return (eTC) i;
    }
  }

  return TC_NONE;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
