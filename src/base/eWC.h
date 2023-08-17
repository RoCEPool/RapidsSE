/*============================================================================*/
/* eWC.h -- Word Category                                                     */
/*============================================================================*/
#pragma once

// Note: When adding anything to the below list
// Update the static functions in Dic.cpp/hpp
// to convert eWC to eDK and vice versa

enum eWC // word category
{
  /*  0 */ WC_NONE=0,     // none
  /*  1 */ WC_CATALOG,
  /*  2 */ WC_CMDSYNTAX,
  /*  3 */ WC_CMDTAGS,
  /*  4 */ WC_DEMON,
  /*  5 */ WC_FOLDER,
  /*  6 */ WC_HANDLER,
  /*  7 */ WC_INCLUDE,
  /*  8 */ WC_INDEX,
  /*  9 */ WC_OPERATOR,
  /* 10 */ WC_PROCEDURE,//
  /* 11 */ WC_PROMPT,
  /* 12 */ WC_ROOTCONSTANT,//
  /* 13 */ WC_SCHEMA,
  /* 14 */ WC_TABLE,
  /* 15 */ WC_TENANT,
  /* 16 */ WC_WORD,
  /* 17 */ WC_ENUMS
};

extern Stz* aWC[WC_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
