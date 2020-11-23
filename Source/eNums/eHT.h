/*============================================================================*/
/* eHT.h -- Heap Type                                                         */
/*============================================================================*/
#pragma once

enum eHT // Heap type
{
  /*  0 */ HT_INVALID=0,
  
  /*  1 */ HT_PERSISTENT_TINY,
  /*  2 */ HT_PERSISTENT_SMALL,
  /*  3 */ HT_PERSISTENT_MEDIUM,
  /*  4 */ HT_PERSISTENT_LARGE,
  
  /*  5 */ HT_TRANSIENT_TINY,
  /*  6 */ HT_TRANSIENT_SMALL,
  /*  7 */ HT_TRANSIENT_MEDIUM,
  /*  8 */ HT_TRANSIENT_LARGE,
  
  /*  9 */ HT_ENUMS
};

static Stz* aHT[]
{
  /*  0 */ "INVALID"_Stz,
  
  /*  1 */ "PERSISTENT_TINY"_Stz,
  /*  2 */ "PERSISTENT_SMALL"_Stz,
  /*  3 */ "PERSISTENT_MEDIUM"_Stz,
  /*  4 */ "PERSISTENT_LARGE"_Stz,
  
  /*  5 */ "TRANSIENT_TINY"_Stz,
  /*  6 */ "TRANSIENT_SMALL"_Stz,
  /*  7 */ "TRANSIENT_MEDIUM"_Stz,
  /*  8 */ "TRANSIENT_LARGE"_Stz,
  
  /*  9 */ "ENUMS"_Stz
};

static Stz* aHT_short[]
{
  /*  0 */ "I"_Stz,
  
  /*  1 */ "PT"_Stz,
  /*  2 */ "PS"_Stz,
  /*  3 */ "PSM"_Stz,
  /*  4 */ "PL"_Stz,
  /*  6 */ "PJ"_Stz,
  
  /*  6 */ "T"_Stz,
  /*  7 */ "S"_Stz,
  /*  8 */ "M"_Stz,
  /*  9 */ "L"_Stz,
  /* 10 */ "J"_Stz,
  
  /* 11 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
