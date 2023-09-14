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

extern Stz* aFX[FX_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
