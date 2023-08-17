/*============================================================================*/
/* eSI.h - String Inflate                                                     */
/*============================================================================*/
#include "BASE.h"
#include "eSI.h"

// ALERT: If this enum changes, make the change in LogicCompiler (line 765)

Stz* aSI[SI_ENUMS]
{
  /*  0 */ "SI_NONE"_Stz,

  /*  3 */ "SI_CELLARRAY_ADR"_Stz,
  /*  4 */ "SI_CELLARRAY_ANT"_Stz,
  /*  5 */ "SI_CELLARRAY_PST"_Stz,
  /*  6 */ "SI_KEY"_Stz,
  /*  1 */ "SI_STRAREA_ADR"_Stz,
  /*  2 */ "SI_STRAREA_OFS"_Stz,
//  /*  7 */ "SI_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
