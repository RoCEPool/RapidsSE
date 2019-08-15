/*============================================================================*/
/* eHA.h -- Thread Actions                                                    */
/*============================================================================*/
#pragma once

enum eHA // thread actions
{
  /*  0 */ HA_NONE=0,
  
  /*    */ HA_ADD_DEDICATED_THREAD,
  /*    */ HA_ADD_GENERAL_THREAD,
  /*    */ HA_REMOVE_DEDICATED_THREAD,
  /*    */ HA_REMOVE_GENERAL_THREAD,

  /*    */ HA_ENUMS,
};

static Stz* aHA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "HA_ADD_DEDICATED_THREAD"_Stz,
  /*    */ "HA_ADD_GENERAL_THREAD"_Stz,
  /*    */ "HA_REMOVE_DEDICATED_THREAD"_Stz,
  /*    */ "HA_REMOVE_GENERAL_THREAD"_Stz,
  
  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
