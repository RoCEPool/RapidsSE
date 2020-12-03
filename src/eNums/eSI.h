/*============================================================================*/
/* eSI.h - String Inflate                                                     */
/*============================================================================*/
#pragma once

// ALERT: If this enum changes, make the change in LogicCompiler (line 765)
enum eSI // string inflate
{
  /*  0 */ SI_NONE=0,
  /*  1 */ SI_AREA_ADR,   // inflate into given area, put address into cell
  /*  2 */ SI_AREA_OFS,   // inflate into given area, put offset into cell
  /*  3 */ SI_DEST_ADR,   // put address into given cell (no inflate per se)
  /*  4 */ SI_DEST_ANT,   // inflate into given cell(s), with size before
  /*  5 */ SI_DEST_PST,   // inflate into given cell(s), with size after
  /*  6 */ SI_KEY,        // inflate into given cell(s), with key semantics
  /*  7 */ SI_ENUMS
};

static Stz* aSI[]
{
  /*  0 */ "SI_NONE"_Stz,
  /*  1 */ "SI_AREA_ADR"_Stz,
  /*  2 */ "SI_AREA_OFS"_Stz,
  /*  3 */ "SI_DEST_ADR"_Stz,
  /*  4 */ "SI_DEST_ANT"_Stz,
  /*  5 */ "SI_DEST_PST"_Stz,
  /*  6 */ "SI_KEY"_Stz,
  /*  7 */ "SI_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
