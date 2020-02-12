/*============================================================================*/
/* eRS.h -- Response Kind                                                     */
/*============================================================================*/
#pragma once

enum eRS // response kind
{
  /* 0 */ RS_NONE=0,
  
  /* 1 */ RS_JOB_RESPONSE,
  /* 2 */ RS_LOAD_RESPONSE,
  /* 3 */ RS_PARTICIPANT_ERROR,
  /* 4 */ RS_PARTICIPANT_RESPONSE,
    
  /* 5 */ RS_ENUMT_
};

static Stz* aRS[]
{
  /* 0 */ "NONE"_Stz,
  
  /* 1 */ "RS_JOB_RESPONSE"_Stz,
  /* 2 */ "RS_LOAD_RESPONSE"_Stz,
  /* 3 */ "RS_PARTICIPANT_ERROR"_Stz,
  /* 4 */ "RS_PARTICIPANT_RESPONSE"_Stz,
  
  /* 5 */ "ENUMT_"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
