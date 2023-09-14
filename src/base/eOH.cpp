/*============================================================================*/
/* eOH.cpp -- Optimisation Phase                                              */
/*============================================================================*/
#include "BASE.h"
#include "eOH.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Stz* aOH[OH_ENUMS]
{
  /* */ "NONE"_Stz,

  /* */ "OH_PROLOG"_Stz,        // OH_PROLOG
  /* */ "OH_FLATTEN"_Stz,       // OH_FLATTEN
  /* */ "OH_JOIN_ORDERING"_Stz, // OH_JOIN_ORDERING
  /* */ "OH_OPTIMISE"_Stz,      // OH_OPTIMISE
  /* */ "OH_EPILOG"_Stz,        // OH_EPILOG
  /* */ "OH_SYNTHESISE"_Stz,    // OH_SYNTHESISE

//  /* */ "ENUMS"_Stz
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Stz* aOH_short[OH_ENUMS]
{
  /* */ "UK"_Stz,

  /* */ "PL"_Stz,  // OH_PROLOG
  /* */ "FL"_Stz,  // OH_FLATTEN
  /* */ "JO"_Stz,  // OH_JOIN_ORDERING
  /* */ "OZ"_Stz,  // OH_OPTIMISE
  /* */ "EL"_Stz,  // OH_EPILOG
  /* */ "SZ"_Stz,  // OH_SYNTHESISE

//  /* */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
