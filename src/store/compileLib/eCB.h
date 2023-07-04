/*============================================================================*/
/* eCB.h -- Compiler Breed                                                    */
/*============================================================================*/
#pragma once

enum eCB
{
  /*  0 */ CB_NONE=0,
  /*  1 */ CB_GCC,
  /*  2 */ CB_CLANG,
  /*  3 */ CB_TCC,
  /*  4 */ CB_SCL_GCC,    // For CentOS 7 environments
  /*  5 */ CB_SCL_CLANG,  // For CentOS 7 environments
  /*  6 */ CB_ENUMS
};

static Stz* aCB[]
{
    /*  0 */ "NONE"_Stz,
    /*  1 */ "CB_GCC"_Stz,
    /*  2 */ "CB_CLANG"_Stz,
    /*  3 */ "CB_TCC"_Stz,
    /*  4 */ "CB_SCL_GCC"_Stz,
    /*  5 */ "CB_SCL_CLANG"_Stz,
    /*  6 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
