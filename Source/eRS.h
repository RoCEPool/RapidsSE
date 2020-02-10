/*============================================================================*/
/* eRS.h -- Response Kind                                                     */
/*============================================================================*/
#pragma once

enum eRS // response kind
{
  /* 0  */ RS_NONE=0,
  
  /*  1 */ RS_DATA_ROWS,
  /*  2 */ RS_LOAD_ROWS,
  /*  3 */ RS_LOCAL_ROWS,
  /*  4 */ RS_LOCAL_ZERO,
  /*  5 */ RS_NODE_ROWS,
  /*  6 */ RS_QUEUE_ROWS,
  
  /*  7 */ RS_ENUMT_
};

static Stz* aRS[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  1 */ "DATA_ROWS"_Stz,
  /*  2 */ "LOAD_ROWS"_Stz,
  /*  3 */ "LOCAL_ROWS"_Stz,
  /*  4 */ "LOCAL_ZERO"_Stz,
  /*  5 */ "NODE_ROWS"_Stz,
  /*  6 */ "QUEUE_ROWS"_Stz,
  
  /*  7 */ "ENUMT_"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
