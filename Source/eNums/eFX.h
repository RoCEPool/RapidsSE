/*============================================================================*/
/* eFX.h -- Feature                                                           */
/*============================================================================*/
#pragma once

enum eFX // feature
{
  /*  0 */ FX_NONE=0,
  
  /*    */ FX_IS_TABLE,
  /*    */ FX_IS_COLUMN,
  /*    */ FX_IS_PTNCOL,
  /*    */ FX_IS_INDEX,
  /*    */ FX_IS_IDXCOL,

  /*    */ FX_NO,
  /*    */ FX_NAME,
  /*    */ FX_SQLTYPE,
  /*    */ FX_WIDTH,
  /*    */ FX_SCALE,
  /*    */ FX_NULLABLE,
  
  /*    */ FX_OF_TABLE,
  /*    */ FX_OF_INDEX,

  /*    */ FX_TABLE_PTR,
  /*    */ FX_INDEX_PTR,

  /*    */ FX_ENUMS
};

static Stz* aFX[]
{
  /*  0 */ "FX_NONE"_Stz,
  
  /*    */ "FX_IS_TABLE"_Stz,
  /*    */ "FX_IS_COLUMN"_Stz,
  /*    */ "FX_IS_PTNCOL"_Stz,
  /*    */ "FX_IS_INDEX"_Stz,
  /*    */ "FX_IS_IDXCOL"_Stz,

  /*    */ "FX_NO"_Stz,
  /*    */ "FX_NAME"_Stz,
  /*    */ "FX_SQLTYPE"_Stz,
  /*    */ "FX_WIDTH"_Stz,
  /*    */ "FX_SCALE"_Stz,
  /*    */ "FX_NULLABLE"_Stz,

  /*    */ "FX_OF_TABLE"_Stz,
  /*    */ "FX_OF_INDEX"_Stz,

  /*    */ "FX_TABLE_PTR"_Stz,
  /*    */ "FX_INDEX_PTR"_Stz,

  /*    */ "FX_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
