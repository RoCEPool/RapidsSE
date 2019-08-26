/*============================================================================*/
/* eIK.h -- Id Kind                                                           */
/*============================================================================*/
#pragma once

enum eIK // id kind
{
  /*  0 */ IK_NONE=0,
  
  /*    */ IK_JOB,
  /*    */ IK_LOCK,
  /*    */ IK_OP,
  /*    */ IK_PROMPT,
  /*    */ IK_REQUEST,
  /*    */ IK_RESOURCE,
  /*    */ IK_SESSION,
  /*    */ IK_TASK,
  /*    */ IK_THREAD,
  /*    */ IK_TX,

  /*    */ IK_ENUMS
};

static Stz* aIK[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "IK_JOB"_Stz,
  /*    */ "IK_LOCK"_Stz,
  /*    */ "IK_OP"_Stz,
  /*    */ "IK_PROMPT"_Stz,
  /*    */ "IK_REQUEST"_Stz,
  /*    */ "IK_RESOURCE"_Stz,
  /*    */ "IK_SESSION"_Stz,
  /*    */ "IK_TASK"_Stz,
  /*    */ "IK_THREAD"_Stz,
  /*    */ "IK_TX"_Stz,

  /*    */ "ENUMS"_Stz
};

static Stz* aIK_3c[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "JOB"_Stz,
  /*    */ "LCK"_Stz,
  /*    */ "OPR"_Stz,
  /*    */ "PRM"_Stz,
  /*    */ "REQ"_Stz,
  /*    */ "RES"_Stz,
  /*    */ "SSN"_Stz,
  /*    */ "TSK"_Stz,
  /*    */ "THD"_Stz,
  /*    */ "TXN"_Stz,
  
  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
