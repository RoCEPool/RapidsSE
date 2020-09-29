/*============================================================================*/
/* eQN.h -- Questions                                                         */
/*============================================================================*/
#pragma once

enum eQN // queue kind
{
  /*  */ QN_NONE=0,
  /*  */ QN_ELEMENTS_BY_ATR_NAME,
  /*  */ QN_ELEMENTS_BY_ATR_NAME_VALUE,
  /*  */ QN_ELEMENTS_BY_ELM_NAME,
  /*  */ QN_ENUMS,
};

static Stz* aQN[]
{
  /*  */ "QN_NONE"_Stz,
  /*  */ "QN_ELEMENTS_BY_ATR_NAME"_Stz,
  /*  */ "QN_ELEMENTS_BY_ATR_NAME_VALUE"_Stz,
  /*  */ "QN_ELEMENTS_BY_ELM_NAME"_Stz,
  /*  */ "QN_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
