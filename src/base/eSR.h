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

static Stz* aSR[]
{
    /*  0 */ "NONE"_Stz,

    /*    */ "SR_AT"_Stz,
    /*    */ "SR_IJ"_Stz,
    /*    */ "SR_IT"_Stz,
    /*    */ "SR_PL"_Stz,
    /*    */ "SR_PO"_Stz,
    /*    */ "SR_PT"_Stz,
    /*    */ "SR_TO"_Stz,
    /*    */ "SR_TJ"_Stz,

    /*    */ "ENUMS"_Stz
};


static Stz* aSRArea[]
{
  /*  0 */ "NONE"_Stz,

      /*    */ "SR_AT"_Stz,
      /*    */ "SR_IJ"_Stz,
      /*    */ "SR_IT"_Stz,
      /*    */ "SR_PL"_Stz,
      /*    */ "SR_PO"_Stz,
      /*    */ "SR_PT"_Stz,
      /*    */ "aDynamicT->m_xRowInfoTO.m_acStrArea"_Stz,
      /*    */ "aDynamicT->m_xRowInfoTJ.m_acStrArea"_Stz,

      /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
