/*============================================================================*/
/* eYF.h -- Parcel Forward                                                    */
/*============================================================================*/
#pragma once

enum eYF // parcel forward
{
  /*  0 */ YF_NONE=0,
  
  /*    */ YF_ALL_NODES,
  /*    */ YF_ALL_PARTITIONS,
  /*    */ YF_ALL_PARTITIONS_ON_ORIGIN,
  /*    */ YF_NEXT_NODE,
  /*    */ YF_ORIGIN,

  /*    */ YF_ENUMS,
};

static Stz* aYF[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "YF_ALL_NODES"_Stz,
  /*    */ "YF_ALL_PARTITIONS"_Stz,
  /*    */ "YF_ALL_PARTITIONS_ON_ORIGIN"_Stz,
  /*    */ "YF_NEXT_NODE"_Stz,
  /*    */ "YF_ORIGIN"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
