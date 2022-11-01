/*============================================================================*/
/* eTE.h -- Term Evaluation                                                   */
/*============================================================================*/
#pragma once

enum eTE // atom evaluation
{
  /* */ TE_NONE=0,

  /* */ TE_AND,
  /* */ TE_COMPARE,
  /* */ TE_EXECUTE,
  /* */ TE_EXISTS,
  /* */ TE_IN,
  /* */ TE_OR,
  /* */ TE_OTHER,

  /* */ TE_ENUMS
};

static Stz* aTE[]
{
  /*  */ "TE_NONE"_Stz,

  /*  */ "TE_AND"_Stz,
  /*  */ "TE_COMPARE"_Stz,
  /*  */ "TE_EXECUTE"_Stz,
  /*  */ "TE_EXISTS"_Stz,
  /*  */ "TE_IN"_Stz,
  /*  */ "TE_OR"_Stz,
  /*  */ "TE_OTHER"_Stz,

  /*  */ "TE_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
