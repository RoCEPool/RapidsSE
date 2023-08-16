/*============================================================================*/
/* eDA.h -- DDL Actions                                                       */
/*============================================================================*/
#pragma once

enum eDA // ddl actions
{
  /*  0 */ DA_NONE=0,
  
  /*  0 */ DA_DDL_CREATE_CATALOG,
  /*  0 */ DA_DDL_CREATE_INDEX,
  /*  0 */ DA_DDL_CREATE_SCHEMA,
  /*  0 */ DA_DDL_CREATE_TABLE,
  /*  0 */ DA_DDL_CREATE_TENANT,
  /*  0 */ DA_DDL_DROP_CATALOG,
  /*  0 */ DA_DDL_DROP_INDEX,
  /*  0 */ DA_DDL_DROP_SCHEMA,
  /*  0 */ DA_DDL_DROP_TABLE,
  /*  0 */ DA_DDL_DROP_TENANT,

  /*    */ DA_ENUMS,
};

extern Stz* aDA[DA_ENUMS];
extern Stz* aDA_short[DA_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
