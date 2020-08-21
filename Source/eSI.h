/*============================================================================*/
/* eSI.h - String Inflate                                                     */
/*============================================================================*/
#pragma once

enum eSI // string inflate
{
  /*  0 */ SI_NONE=0,
  /*  1 */ SI_AREA_ADR,   // inflate into given area, put address into cell
  /*  2 */ SI_AREA_OFS,   // inflate into given area, put offset into cell
  /*  3 */ SI_DEST,       // inflate into given destination cell(s)
  /*  4 */ SI_DEST_ANT,   // inflate into given cell(s), with size before
  /*  5 */ SI_DEST_PST,   // inflate into given cell(s), with size after
  /*  6 */ SI_POINTER,    // put pointer into given cell (no inflate per se)
  /*  7 */ SI_ENUMS
};

static Stz* aSI[]
{
  /*  0 */ "SI_NONE"_Stz,
  /*  1 */ "SI_AREA_ADR"_Stz,
  /*  2 */ "SI_AREA_OFS"_Stz,
  /*  3 */ "SI_DEST"_Stz,
  /*  4 */ "SI_DEST_ANT"_Stz,
  /*  5 */ "SI_DEST_PST"_Stz,
  /*  6 */ "SI_POINTER"_Stz,
  /*  7 */ "SI_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
