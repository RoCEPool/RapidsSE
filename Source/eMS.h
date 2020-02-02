/*============================================================================*/
/* eMS.h -- Meta Structure                                                    */
/*============================================================================*/
#pragma once

enum eMS // meta structure
{
  /*  0 */ MS_NONE=0,
  
  /*    */ MS_LINEAR,    // linear sequence of members
  /*    */ MS_TEMPLATE,  // template of linear sequence
  /*    */ MS_UNION,     // union of members
  
  /*    */ MS_ENUMS
};

static Stz* aMS[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "MS_LINEAR"_Stz,
  /*    */ "MS_TEMPLATE"_Stz,
  /*    */ "MS_UNION"_Stz,
  
  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
