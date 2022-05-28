/*============================================================================*/
/* eMG.h -- Marker Category                                                   */
/*============================================================================*/
#pragma once

enum eMG // meta member
{
  /*  0 */ MG_ORDINARY=0,  // ordinary
  /*    */ MG_CHAIN,       // from job maker chain
  /*    */ MG_MANAGER,     // from maker mgr in attach map
  
  /*    */ MG_ENUMS
};

static Stz* aMG[]
{
  /*  0 */ "MG_ORDINARY"_Stz,
  /*    */ "MG_CHAIN"_Stz,
  /*    */ "MG_MANAGER"_Stz,
  
  /*    */ "MG_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
