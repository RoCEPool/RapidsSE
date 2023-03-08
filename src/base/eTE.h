/*============================================================================*/
/* eTE.h -- Term Evaluation                                                   */
/*============================================================================*/
#pragma once

enum eTE // atom evaluation
{
  /* */ TE_NONE=0,

  /* */ TE_AND,
  /* */ TE_BOOLFN,
  /* */ TE_EXECUTE,
  /* */ TE_EXISTS,
  /* */ TE_IN,
  /* */ TE_NOT,
  /* */ TE_NOTEXISTS,
  /* */ TE_NOTIN,
  /* */ TE_OR,
  /* */ TE_OTHER,
  /* */ TE_RELNOP,

  /* */ TE_ENUMS
};

static Stz* aTE[]
{
  /*  */ "TE_NONE"_Stz,

  /*  */ "TE_AND"_Stz,
  /*  */ "TE_BOOLFN"_Stz,
  /*  */ "TE_EXECUTE"_Stz,
  /*  */ "TE_EXISTS"_Stz,
  /*  */ "TE_IN"_Stz,
  /*  */ "TE_NOT"_Stz,
  /*  */ "TE_NOTEXISTS"_Stz,
  /*  */ "TE_NOTIN"_Stz,
  /*  */ "TE_OR"_Stz,
  /*  */ "TE_OTHER"_Stz,
  /*  */ "TE_RELNOP"_Stz,

  /*  */ "TE_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
