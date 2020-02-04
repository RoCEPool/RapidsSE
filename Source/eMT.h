/*============================================================================*/
/* eMT.h -- Meta Type                                                         */
/*============================================================================*/
#pragma once

enum eMT // meta type
{
  /*  0 */ MT_NONE=0,
  
  /*    */ MT_SIMPLEX,   // fundamental data type
  
  /*    */ MT_LINEAR,    // complex: linear sequence of members
  /*    */ MT_TEMPLATE,  // complex: template of linear sequence
  /*    */ MT_UNION,     // complex: union of members
  
  /*    */ MT_ENUMS
};

static Stz* aMT[]
{
  /*  0 */ "NONE"_Stz,

  /*    */ "MT_SIMPLEX"_Stz,

  /*    */ "MT_LINEAR"_Stz,
  /*    */ "MT_TEMPLATE"_Stz,
  /*    */ "MT_UNION"_Stz,
  
  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
