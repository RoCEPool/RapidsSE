/*============================================================================*/
/* eSI.h - String Inflate                                                     */
/*============================================================================*/
#pragma once

enum eSI // string inflate
{
  /*  0 */ SI_NONE=0,
  /*  1 */ SI_AREA_ADR,   // inflate into given area, put address into cell
  /*  2 */ SI_AREA_OFS,   // inflate into given area, put offset into cell
  /*  3 */ SI_INSITU,     // inflate into given cell(s)
  /*  4 */ SI_INSITU_STK, // inflate into given cell(s), with size after
  /*  5 */ SI_INSITU_SZE, // inflate into given cell(s), with size before
  /*  6 */ SI_POINTER,    // put pointer into given cell (no inflate per se)
  /*  7 */ SI_ENUMS
};

static Stz* aSI[]
{
  /*  0 */ "SI_NONE"_Stz,
  /*  1 */ "SI_AREA_ADR"_Stz,
  /*  2 */ "SI_AREA_OFS"_Stz,
  /*  3 */ "SI_INSITU"_Stz,
  /*  4 */ "SI_INSITU_STK"_Stz,
  /*  5 */ "SI_INSITU_SZE"_Stz,
  /*  6 */ "SI_POINTER"_Stz,
  /*  7 */ "SI_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
