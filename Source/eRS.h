/*============================================================================*/
/* eRS.h -- Response Kind                                                     */
/*============================================================================*/
#pragma once

enum eRS // response kind
{
  /* 0 */ RS_NONE=0,
  
  /* 1 */ RS_DATA_ROWS,
  /* 2 */ RS_LOAD_ROWS,
  /* 3 */ RS_LOCAL_ERRORS,
  /* 4 */ RS_LOCAL_ROWS,
  /* 5 */ RS_LOCAL_ZERO,
  /* 6 */ RS_NODE_ERRORS,
  /* 7 */ RS_NODE_ROWS,
  /* 8 */ RS_NODE_ZERO,
  
  /* 9 */ RS_ENUMS_
};

static Stz* aRS[]
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
