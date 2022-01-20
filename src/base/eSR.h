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
  /*    */ SR_PT,   // project T
  /*    */ SR_TJ,   // join T with J
  /*    */ SR_H,    // project of join

  /*    */ SR_ENUMS
};

static Stz* aSR[]
{
  /*  0 */ "NONE"_Stz,

  /*    */ "SR_AT"_Stz,
  /*    */ "SR_IJ"_Stz,
  /*    */ "SR_IT"_Stz,
  /*    */ "SR_PT"_Stz,
  /*    */ "SR_TJ"_Stz,
  /*    */ "SR_H"_Stz,

  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
