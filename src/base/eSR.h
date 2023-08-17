/*============================================================================*/
/* eSR.h -- Stencil Row                                                       */
/*============================================================================*/
#pragma once

enum eSR // stencil row
{
  /*  0 */ SR_NONE=0,

  /*    */ SR_AT,   // aggregate T
  /*    */ SR_IJ,   // inflate J
  /*    */ SR_IT,   // inflate T
  /*    */ SR_PL,   // project PayLoad
  /*    */ SR_PO,   // pot output
  /*    */ SR_PT,   // project T
  /*    */ SR_TO,   // Tub Output
  /*    */ SR_TJ,   // join T with J

  /*    */ SR_ENUMS
};

extern Stz* aSR[SR_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
