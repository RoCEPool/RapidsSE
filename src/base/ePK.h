/*============================================================================*/
/* ePK.h -- Predicate Kind                                                    */
/*============================================================================*/
#pragma once

enum ePK // predicate kind
{
  /* 0 */ PK_NONE=0,
  /* 1 */ PK_COLUMN_COLUMN_OP,
  /* 2 */ PK_COLUMN_LITERAL_OP,   // this should always be the case
  /* 3 */ PK_LITERAL_LITERAL_OP,
  /* 4 */ PK_LITERAL_COLUMN_OP,
  /* 5 */ PK_ENUMS
};

static Stz* aPK[]
{
  /* 0 */ "none"_Stz,
  /* 1 */ "column column op"_Stz,
  /* 2 */ "column literal op"_Stz,
  /* 3 */ "literal literal op"_Stz,
  /* 4 */ "literal column op"_Stz,
  /* 5 */ "enums"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
