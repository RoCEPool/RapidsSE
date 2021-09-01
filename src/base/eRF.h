/*============================================================================*/
/* eRF.h -- Repair from Failure                                               */
/*============================================================================*/
#pragma once

enum eRF // repair from failure
{
  /*  0 */ RF_NONE=0,
  
  /*    */ RF_REPAIR_DELETE,
  /*    */ RF_REPAIR_NONE,
  /*    */ RF_REPAIR_ONLY,
  /*    */ RF_DELETE_ONLY,
  /*    */ RF_NEVER,
  /*    */ RF_SET_NULL,

  /*    */ RF_ENUMS,
};

static Stz* aRF[]
{
  /*  0 */ "RF_NONE"_Stz,
  
  /*    */ "RF_REPAIR_DELETE"_Stz,
  /*    */ "RF_REPAIR_NONE"_Stz,
  /*    */ "RF_REPAIR_ONLY"_Stz,
  /*    */ "RF_DELETE_ONLY"_Stz,
  /*    */ "RF_NEVER"_Stz,
  /*    */ "RF_SET_NULL"_Stz,

  /*    */ "RF_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
