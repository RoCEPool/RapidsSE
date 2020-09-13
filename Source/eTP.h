/*============================================================================*/
/* eTP.h -- Transform/Question Parameter                                      */
/*============================================================================*/
#pragma once

enum eTP // transform/question parameter
{
  /*  0 */ TP_NONE=0,
  
  /*    */ TP_DATE,
  /*    */ TP_INT,
  /*    */ TP_LABEL,
  /*    */ TP_STRING,
  /*    */ TP_SUBSCR,

  /*    */ TP_ENUMS
};

static Stz* aTP[]
{
  /*  0 */ "TP_NONE"_Stz,
  
  /*  0 */ "TP_DATE"_Stz,
  /*  0 */ "TP_INT"_Stz,
  /*  0 */ "TP_LABEL"_Stz,
  /*  0 */ "TP_STRING"_Stz,
  /*  0 */ "TP_SUBSCR"_Stz,

  /*    */ "TP_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
