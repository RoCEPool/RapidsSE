/*============================================================================*/
/* eFU.h -- Fact usage                                                        */
/*============================================================================*/
#pragma once

enum eFU // fact usage
{
  /*  0 */ FU_NONE=0,
  
  /*    */ FU_FUNCTION,
  /*    */ FU_INT,
  /*    */ FU_OBJECT,
  /*    */ FU_OPAQUE,
  /*    */ FU_TEXT,
  /*    */ FU_USAGE,
  /*    */ FU_WORD,
  /*    */ FU_XREF,

  /*    */ FU_ENUMS
};

static Stz* aFU[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "FU_FUNCTION"_Stz,
  /*    */ "FU_INT"_Stz,
  /*    */ "FU_OBJECT"_Stz,
  /*    */ "FU_OPAQUE"_Stz,
  /*    */ "FU_TEXT"_Stz,
  /*    */ "FU_USAGE"_Stz,
  /*    */ "FU_WORD"_Stz,
  /*    */ "FU_XREF"_Stz,

  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
