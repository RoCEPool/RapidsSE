/*============================================================================*/
/* eMB.h -- Meta Block                                                        */
/*============================================================================*/
#pragma once

enum eMB // meta block
{
  /*  0 */ MB_NONE=0,
  
  /*    */ MB_CLASS,  // linear sequence of members
  /*    */ MB_STRUCT, // linear sequence of members
  /*    */ MB_UNION,  // union sequence of members
  
  /*    */ MB_ENUMS
};

static Stz* aMB[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "MB_CLASS"_Stz,
  /*    */ "MB_STRUCT"_Stz,
  /*    */ "MB_UNION"_Stz,
  
  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
