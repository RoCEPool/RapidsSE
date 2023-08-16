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

extern Stz* aDK[];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
