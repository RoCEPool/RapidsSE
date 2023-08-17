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

extern Stz* aMT[MT_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
