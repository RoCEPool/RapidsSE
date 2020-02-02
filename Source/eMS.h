/*============================================================================*/
/* eMS.h -- Meta Structure                                                    */
/*============================================================================*/
#pragma once

enum eMS // meta structure
{
  /*  0 */ MS_NONE=0,
  
  /*    */ MS_CLASS,  // linear sequence of members
  /*    */ MS_STRUCT, // linear sequence of members
  /*    */ MS_UNION,  // union sequence of members
  
  /*    */ MS_ENUMS
};

static Stz* aMS[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "MS_CLASS"_Stz,
  /*    */ "MS_STRUCT"_Stz,
  /*    */ "MS_UNION"_Stz,
  
  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
