/*============================================================================*/
/* eHT.h -- Heap Type                                                         */
/*============================================================================*/
#pragma once

enum eHT // Heap type
{
  /*  0 */ HT_INVALID=0,
  /*  1 */ HT_PERSISTENT_SMALL,
  /*  2 */ HT_PERSISTENT_MEDIUM,
  /*  3 */ HT_PERSISTENT_LARGE,
  /*  4 */ HT_PERSISTENT_JUMBO,
  /*  5 */ HT_TRANSIENT_SMALL,
  /*  6 */ HT_TRANSIENT_MEDIUM,
  /*  7 */ HT_TRANSIENT_LARGE,
  /*  8 */ HT_TRANSIENT_JUMBO,
  /*  9 */ HT_ENUMS
};

static Stz* aHT[]
{
  /*  0 */ "INVALID"_Stz,
  /*  1 */ "PERSISTENT_SMALL"_Stz,
  /*  2 */ "PERSISTENT_MEDIUM"_Stz,
  /*  3 */ "PERSISTENT_LARGE"_Stz,
  /*  4 */ "PERSISTENT_JUMBO"_Stz,
  /*  5 */ "TRANSIENT_SMALL"_Stz,
  /*  6 */ "TRANSIENT_MEDIUM"_Stz,
  /*  7 */ "TRANSIENT_LARGE"_Stz,
  /*  8 */ "TRANSIENT_JUMBO"_Stz,
  /*  9 */ "ENUMS"_Stz
};

static Stz* aHT_short[]
{
  /*  0 */ "I"_Stz,
  /*  1 */ "PS"_Stz,
  /*  2 */ "PSM"_Stz,
  /*  3 */ "PL"_Stz,
  /*  4 */ "PJ"_Stz,
  /*  5 */ "S"_Stz,
  /*  6 */ "M"_Stz,
  /*  7 */ "L"_Stz,
  /*  8 */ "J"_Stz,
  /*  9 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
