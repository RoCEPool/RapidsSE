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

static Stz* aDA[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  0 */ "DA_DDL_CREATE_CATALOG"_Stz,
  /*  0 */ "DA_DDL_CREATE_INDEX"_Stz,
  /*  0 */ "DA_DDL_CREATE_SCHEMA"_Stz,
  /*  0 */ "DA_DDL_CREATE_TABLE"_Stz,
  /*  0 */ "DA_DDL_CREATE_TENANT"_Stz,
  /*  0 */ "DA_DDL_DROP_CATALOG"_Stz,
  /*  0 */ "DA_DDL_DROP_INDEX"_Stz,
  /*  0 */ "DA_DDL_DROP_SCHEMA"_Stz,
  /*  0 */ "DA_DDL_DROP_TABLE"_Stz,
  /*  0 */ "DA_DDL_DROP_TENANT"_Stz,

  /*    */ "ENUMS"_Stz,
};

static Stz* aDA_short[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  0 */ "create catalog"_Stz,
  /*  0 */ "create index"_Stz,
  /*  0 */ "create schema"_Stz,
  /*  0 */ "create table"_Stz,
  /*  0 */ "create tenant"_Stz,
  /*  0 */ "drop catalog"_Stz,
  /*  0 */ "drop index"_Stz,
  /*  0 */ "drop schema"_Stz,
  /*  0 */ "drop table"_Stz,
  /*  0 */ "drop tenant"_Stz,

  /*    */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
