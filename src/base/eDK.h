/*============================================================================*/
/* eDK.h -- Dictionary Kind                                                   */
/*============================================================================*/
#pragma once

// Note: When adding anything to the below list
// Update the static functions in Dic.cpp/hpp
// to convert eWC to eDK and vice versa
enum eDK // dictionary kind
{
  /*  0 */ DK_NONE=0,     // none
  /*  1 */ DK_CATALOG,
  /*  2 */ DK_FOLDER,
  /*  3 */ DK_SCHEMA,
  /*  4 */ DK_TABLE,
  /*  5 */ DK_TENANT
};

static Stz* aDK[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "CATALOG"_Stz,
  /*  2 */ "FOLDER"_Stz,
  /*  3 */ "SCHEMA"_Stz,
  /*  4 */ "TABLE"_Stz,
  /*  5 */ "TENANT"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
