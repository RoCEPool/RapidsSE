/*============================================================================*/
/* eMB.h -- Meta Block                                                        */
/*============================================================================*/
#pragma once

enum eMB // meta block
{
  /*  0 */ MB_NONE=0,
  
  /*    */ MB_CLASS,        // linear sequence of var's
  /*    */ MB_STRUCT,       // struct of var's
  /*    */ MB_STRUCT_BITS,  // struct of bit fields
  /*    */ MB_UNION,        // union of var's
  
  /*    */ MB_ENUMS
};

static Stz* aMB[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "MB_CLASS"_Stz,
  /*    */ "MB_STRUCT"_Stz,
  /*    */ "MB_STRUCT_BITS"_Stz,
  /*    */ "MB_UNION"_Stz,
  
  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
