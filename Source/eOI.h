/*============================================================================*/
/* eOI.h -- Operator Infoset                                                  */
/*============================================================================*/
#pragma once

enum eOI // operator Infoset
{
  /*  0 */ OI_NONE=0,
  
  /*    */ OI_GRIDSANITY,
  /*    */ OI_LISTCOLUMNS,
  /*    */ OI_NODEINFO,
  /*    */ OI_PROGRESSUNITS,
  /*    */ OI_SEEKOFFSETS,
  /*    */ OI_SPACE,
  /*    */ OI_TOTALUNITS,
 
  /*    */ OI_ENUMS,
};

static Stz* aOI[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "GRIDSANITY"_Stz,
  /*    */ "LISTCOLUMNS"_Stz,
  /*    */ "NODEINFO"_Stz,
  /*    */ "PROGRESSUNITS"_Stz,
  /*    */ "SEEKOFFSETS"_Stz,
  /*    */ "SPACE"_Stz,
  /*    */ "TOTALUNITS"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
