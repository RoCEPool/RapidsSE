/*============================================================================*/
/* eHT.cpp -- Heap Type                                                       */
/*============================================================================*/
#include "BASE.h"
#include "eHT.h"

Stz* aHT[HT_ENUMS]
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

  /* 9 */ "VIRTUAL_TINY"_Stz,
  /* 10 */ "VIRTUAL_SMALL"_Stz,
  /* 11 */ "VIRTUAL_MEDIUM"_Stz,
  /* 12 */ "VIRTUAL_LARGE"_Stz,

//  /* 13 */ "ENUMS"_Stz
};

Stz* aHT_short[HT_ENUMS]
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

  /* 9 */ "VT"_Stz,
  /* 10 */ "VS"_Stz,
  /* 11 */ "VM"_Stz,
  /* 12 */ "VL"_Stz,

//  /* 13 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
