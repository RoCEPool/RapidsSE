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

extern Stz* aPK[PK_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
