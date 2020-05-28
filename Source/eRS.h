/*============================================================================*/
/* eRS.h -- Response Kind                                                     */
/*============================================================================*/
#pragma once

enum eRS // response kind
{
  /* 0 */ RS_NONE=0,
  
  /* 1 */ RS_LOCAL_RESPONSE,
  /* 2 */ RS_DIS2PC_RESPONSE,
  /* 3 */ RS_OLD_JOB_RESPONSE,
    
  /* 4 */ RS_ENUMT_
};

static Stz* aRS[]
{
  /* 0 */ "NONE"_Stz,
  
  /* 1 */ "RS_DIS2PC_RESPONSE"_Stz,
  /* 2 */ "RS_LOCAL_RESPONSE"_Stz,
  /* 3 */ "RS_OLD_JOB_RESPONSE"_Stz,
  
  /* 5 */ "ENUMT_"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
