/*============================================================================*/
/* eMS.h -- Member Set                                                        */
/*============================================================================*/
#pragma once

enum eMS // member set
{
  /*  0 */ MS_NONE=0,
  
  /*    */ MS_LINEAR,       // linear sequence of var's
  /*    */ MS_STRUCT,       // struct of var's
  /*    */ MS_STRUCT_BITS,  // struct of bit fields
  /*    */ MS_UNION,        // union of var's
  
  /*    */ MS_ENUMS
};

static Stz* aMS[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "MS_LINEAR"_Stz,
  /*    */ "MS_STRUCT"_Stz,
  /*    */ "MS_STRUCT_BITS"_Stz,
  /*    */ "MS_UNION"_Stz,
  
  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
