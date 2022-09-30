/*============================================================================*/
/* eWC.h -- Word Category                                                     */
/*============================================================================*/
#pragma once

enum eWC // word category
{
  /*  0 */ WC_NONE=0,     // none
  /*  1 */ WC_CMDSYNTAX,
  /*  2 */ WC_CMDTAGS,
  /*  3 */ WC_DEMON,
  /*  3 */ WC_FOLDER,
  /*  4 */ WC_HANDLER,
  /*  4 */ WC_INCLUDE,
  /*  5 */ WC_OPERATOR,
  /*  6 */ WC_PROCEDURE,//
  /*  7 */ WC_PROMPT,
  /*  8 */ WC_ROOTCONSTANT,//
  /*  9 */ WC_TABLE,
  /*  10 */ WC_WORD//
};

static Stz* aWC[]
{
    /*  0 */ "NONE"_Stz,
    /*  1 */ "CMDSYNTAX"_Stz,
    /*  2 */ "CMDTAGS"_Stz,
    /*  2 */ "DEMON"_Stz,
    /*  3 */ "FOLDER"_Stz,
    /*  4 */ "HANDLER"_Stz,
    /*  4 */ "INCLUDE"_Stz,
    /*  5 */ "OPERATOR"_Stz,
    /*  6 */ "PROCEDURE"_Stz,
    /*  7 */ "PROMPT"_Stz,
    /*  8 */ "ROOTCONSTANT"_Stz,
    /*  9 */ "TABLE"_Stz,
    /*  10 */ "WORD"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
