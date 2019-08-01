/*============================================================================*/
/* eRK.h -- response kind                                                     */
/*============================================================================*/
#pragma once

enum eRK // response kind
{
  /* 0 */ RK_NONE=0,
  
  /* 1 */ RK_DATA_ROWS,
  /* 2 */ RK_LOAD_ROWS,
  /* 3 */ RK_LOCAL_ERRORS,
  /* 4 */ RK_LOCAL_ROWS,
  /* 5 */ RK_LOCAL_ZERO,
  /* 6 */ RK_NODE_ERRORS,
  /* 7 */ RK_NODE_ROWS,
  /* 8 */ RK_NODE_ZERO,
  
  /* 9 */ RK_ENUMS_
};

static Stz* aRK[]
{
  /* 0 */ "NONE"_Stz,
  
  /* 1 */ "DATA_ROWS"_Stz,
  /* 2 */ "LOAD_ROWS"_Stz,
  /* 3 */ "LOCAL_ERRORS"_Stz,
  /* 4 */ "LOCAL_ROWS"_Stz,
  /* 5 */ "LOCAL_ZERO"_Stz,
  /* 6 */ "NODE_ERRORS"_Stz,
  /* 7 */ "NODE_ROWS"_Stz,
  /* 8 */ "NODE_ZERO"_Stz,
  
  /* 9 */ "ENUMS_"_Stz
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
