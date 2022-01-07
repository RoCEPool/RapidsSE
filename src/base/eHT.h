/*============================================================================*/
/* eHT.h -- Heap Type                                                         */
/*============================================================================*/
#pragma once

enum eHT // Heap type
{
  /*  0 */ HT_INVALID=0,
  
  /*  1 */ HT_P_TINY,
  /*  2 */ HT_P_SMALL,
  /*  3 */ HT_P_MEDIUM,
  /*  4 */ HT_P_LARGE,
  
  /*  5 */ HT_T_TINY,
  /*  6 */ HT_T_SMALL,
  /*  7 */ HT_T_MEDIUM,
  /*  8 */ HT_T_LARGE,
  /*  9 */ HT_T_DATA,

  /* 10 */ HT_V_TINY,
  /* 11 */ HT_V_SMALL,
  /* 12 */ HT_V_MEDIUM,
  /* 13 */ HT_V_LARGE,
  
  /* 14 */ HT_ENUMS
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
  /*  9 */ "TRANSIENT_DATA"_Stz,

  /* 10 */ "VIRTUAL_TINY"_Stz,
  /* 11 */ "VIRTUAL_SMALL"_Stz,
  /* 12 */ "VIRTUAL_MEDIUM"_Stz,
  /* 13 */ "VIRTUAL_LARGE"_Stz,

  /* 14 */ "ENUMS"_Stz
};

static Stz* aHT_short[]
{
  /*  0 */ "I"_Stz,
  
  /*  1 */ "PT"_Stz,
  /*  2 */ "PS"_Stz,
  /*  3 */ "PM"_Stz,
  /*  4 */ "PL"_Stz,
  
  /*  5 */ "T"_Stz,
  /*  6 */ "S"_Stz,
  /*  7 */ "M"_Stz,
  /*  8 */ "L"_Stz,
  /*  9 */ "D"_Stz,
  
  /* 10 */ "VT"_Stz,
  /* 11 */ "VS"_Stz,
  /* 12 */ "VM"_Stz,
  /* 13 */ "VL"_Stz,

  /* 11 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
