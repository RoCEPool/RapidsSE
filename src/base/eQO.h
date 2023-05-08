/*============================================================================*/
/* eQO.h -- SubQuery called by what Operator                                  */
/*============================================================================*/
#pragma once

enum eQO // subquery calling operator
{
  /*   */ QO_NONE=0,
  
  /*   */ QO_EXISTS_OP,
  /*   */ QO_IN_OP,
  /*   */ QO_NOT_EXISTS_OP,
  /*   */ QO_NOT_IN_OP,
  /*   */ QO_RELN_OP,

  /*   */ QO_ENUMS
};

static Stz* aQO[]
{
  /*   */ "QO_NONE"_Stz,
  
  /*   */ "QO_EXISTS_OP"_Stz,
  /*   */ "QO_IN_OP"_Stz,
  /*   */ "QO_NOT_EXISTS_OP"_Stz,
  /*   */ "QO_NOT_IN_OP"_Stz,
  /*   */ "QO_RELN_OP"_Stz,

  /*   */ "QO_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
