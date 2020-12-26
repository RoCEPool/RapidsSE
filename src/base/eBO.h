/*============================================================================*/
/* eBO.h -- Boot Options                                                      */
/*============================================================================*/
#pragma once

enum eBO // boot options
{
  /*  0 */ BO_NONE=0,
  
  /*    */ BO_ADD_NODE,
  /*    */ BO_PROMPT,
  /*    */ BO_SETH_PROMPT,
  /*    */ BO_START,
  /*    */ BO_START_PROMPT,
  /*    */ BO_STATUS,
  /*    */ BO_STOP,
  /*    */ BO_UNLINK,
  
  /*    */ BO_ENUMS,
};

static Stz* aBO[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "ADD_NODE"_Stz,
  /*    */ "PROMPT"_Stz,
  /*    */ "SETH_PROMPT"_Stz,
  /*    */ "START"_Stz,
  /*    */ "START_PROMPT"_Stz,
  /*    */ "STATUS"_Stz,
  /*    */ "STOP"_Stz,
  /*    */ "UNLINK"_Stz,
  
  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
