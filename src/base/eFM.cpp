/*============================================================================*/
/* eFM.cpp -- Function macros                                                 */
/*============================================================================*/

#include "BASE.h"
#include "eFM.h"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// when not being included by the tcc compiler
Stz* aFM[FM_ENUMS]
{
  /*  0 */ "NONE"_Stz,

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // arithmetic operators

  // add
  /*  */ "_ADD_ANYUNK"_Stz,
  /*  */ "_ADD_BCDBCD"_Stz,
  /*  */ "_ADD_DTEIVD"_Stz,
  /*  */ "_ADD_DTEIVY"_Stz,
  /*  */ "_ADD_FLTFLT"_Stz,
  /*  */ "_ADD_INTINT"_Stz,
  /*  */ "_ADD_IVDDTE"_Stz,
  /*  */ "_ADD_IVDTME"_Stz,
  /*  */ "_ADD_IVDTSP"_Stz,
  /*  */ "_ADD_IVYDTE"_Stz,
  /*  */ "_ADD_IVYTSP"_Stz,
  /*  */ "_ADD_IVDIVD"_Stz,
  /*  */ "_ADD_IVYIVY"_Stz,
  /*  */ "_ADD_TMEIVD"_Stz,
  /*  */ "_ADD_TSPIVD"_Stz,
  /*  */ "_ADD_TSPIVY"_Stz,
  /*  */ "_ADD_UNKANY"_Stz,

  // divide
  /*  */ "_DIV_ANYUNK"_Stz,
  /*  */ "_DIV_BCDBCD"_Stz,
  /*  */ "_DIV_FLTFLT"_Stz,
  /*  */ "_DIV_INTINT"_Stz,
  /*  */ "_DIV_IVDINT"_Stz,
  /*  */ "_DIV_IVYINT"_Stz,
  /*  */ "_DIV_UNKANY"_Stz,

  // modulo
  /*  */ "_MOD_ANYUNK"_Stz,
  /*  */ "_MOD_INTINT"_Stz,
  /*  */ "_MOD_UNKANY"_Stz,

  // multiply
  /*  */ "_MUL_ANYUNK"_Stz,
  /*  */ "_MUL_BCDBCD"_Stz,
  /*  */ "_MUL_FLTFLT"_Stz,
  /*  */ "_MUL_INTINT"_Stz,
  /*  */ "_MUL_INTIVD"_Stz,
  /*  */ "_MUL_INTIVY"_Stz,
  /*  */ "_MUL_IVDINT"_Stz,
  /*  */ "_MUL_IVYINT"_Stz,
  /*  */ "_MUL_UNKANY"_Stz,

  // operation invalid
  /*  */ "_OPR_INVINV"_Stz,
  /*  */ "_OPR_INV"_Stz,

  // negate
  /*  */ "_NEG_BCD"_Stz,
  /*  */ "_NEG_FLT"_Stz,
  /*  */ "_NEG_INT"_Stz,
  /*  */ "_NEG_IVD"_Stz,
  /*  */ "_NEG_IVY"_Stz,

  // not supported
  /*  */ "_OPR_NOTNOT"_Stz,
  /*  */ "_OPR_NOT"_Stz,

  // subtract
  /*  */ "_SUB_ANYUNK"_Stz,
  /*  */ "_SUB_BCDBCD"_Stz,
  /*  */ "_SUB_DTEDTE"_Stz,
  /*  */ "_SUB_DTEIVD"_Stz,
  /*  */ "_SUB_DTEIVY"_Stz,
  /*  */ "_SUB_FLTFLT"_Stz,
  /*  */ "_SUB_INTINT"_Stz,
  /*  */ "_SUB_IVDIVD"_Stz,
  /*  */ "_SUB_IVYIVY"_Stz,
  /*  */ "_SUB_TMEIVD"_Stz,
  /*  */ "_SUB_TMETME"_Stz,
  /*  */ "_SUB_TSPIVD"_Stz,
  /*  */ "_SUB_TSPIVY"_Stz,
  /*  */ "_SUB_TSPTSP"_Stz,
  /*  */ "_SUB_UNKANY"_Stz,

  // sum
  /*  */ "_SUM_ANYUNK"_Stz,
  /*  */ "_SUM_BCDBCD"_Stz,
  /*  */ "_SUM_FLTFLT"_Stz,
  /*  */ "_SUM_INTINT"_Stz,
  /*  */ "_SUM_UNKANY"_Stz,

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // between operators

  /*  */ "_BET_ANYUNK"_Stz,
  /*  */ "_BET_BCDBCD"_Stz,
  /*  */ "_BET_DTEDTE"_Stz,
  /*  */ "_BET_FLTFLT"_Stz,
  /*  */ "_BET_INTINT"_Stz,
  /*  */ "_BET_IVDIVD"_Stz,
  /*  */ "_BET_IVYIVY"_Stz,
  /*  */ "_BET_TMETME"_Stz,
  /*  */ "_BET_TSPTSP"_Stz,
  /*  */ "_BET_UNKANY"_Stz,
  /*  */ "_BET_VCHVCH"_Stz,


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast operators

  /*  */ "_ANY_TOUNK"_Stz,
  /*  */ "_BCD_TOFLT"_Stz,
  /*  */ "_BCD_TOINT"_Stz,
  /*  */ "_BCD_TOUNK"_Stz,
  /*  */ "_BCD_TOVCH"_Stz,

  /*  */ "_BOL_TOUNK"_Stz,
  /*  */ "_BOL_TOVCH"_Stz,

  /*  */ "_DTE_TOTSP"_Stz,
  /*  */ "_DTE_TOUNK"_Stz,
  /*  */ "_DTE_TOVCH"_Stz,

  /*  */ "_FLT_TOBCD"_Stz,
  /*  */ "_FLT_TOINT"_Stz,
  /*  */ "_FLT_TOUNK"_Stz,
  /*  */ "_FLT_TOVCH"_Stz,

  /*  */ "_INT_TOBCD"_Stz,
  /*  */ "_INT_TOFLT"_Stz,
  /*  */ "_INT_TOUNK"_Stz,
  /*  */ "_INT_TOVCH"_Stz,

  /*  */ "_IVD_TOUNK"_Stz,
  /*  */ "_IVD_TOVCH"_Stz,

  /*  */ "_IVY_TOUNK"_Stz,
  /*  */ "_IVY_TOVCH"_Stz,

  /*  */ "_TME_TOTSP"_Stz,
  /*  */ "_TME_TOUNK"_Stz,
  /*  */ "_TME_TOVCH"_Stz,

  /*  */ "_TSP_TOUNK"_Stz,
  /*  */ "_TSP_TODTE"_Stz,
  /*  */ "_TSP_TOTME"_Stz,
  /*  */ "_TSP_TOVCH"_Stz,

  /*  */ "_UNK_TOANY"_Stz,

  /*  */ "_VCH_TOBCD"_Stz,
  /*  */ "_VCH_TODTE"_Stz,
  /*  */ "_VCH_TOFLT"_Stz,
  /*  */ "_VCH_TOINT"_Stz,
  /*  */ "_VCH_TOIVD"_Stz,
  /*  */ "_VCH_TOIVY"_Stz,
  /*  */ "_VCH_TOTME"_Stz,
  /*  */ "_VCH_TOTSP"_Stz,
  /*  */ "_VCH_TOUNK"_Stz,

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // duplicates
  /*  */ "_DUP_BOL"_Stz,
  /*  */ "_DUP_BCD"_Stz,
  /*  */ "_DUP_DTE"_Stz,
  /*  */ "_DUP_FLT"_Stz,
  /*  */ "_DUP_INT"_Stz,
  /*  */ "_DUP_IVD"_Stz,
  /*  */ "_DUP_IVY"_Stz,
  /*  */ "_DUP_TME"_Stz,
  /*  */ "_DUP_TSP"_Stz,
  /*  */ "_DUP_UNK"_Stz,
  /*  */ "_DUP_VCH"_Stz,

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract operators

  /*  */ "_DAY_DTE"_Stz,
  /*  */ "_DAY_TSP"_Stz,
  /*  */ "_HOUR_TME"_Stz,
  /*  */ "_HOUR_TSP"_Stz,
  /*  */ "_MINUTE_TME"_Stz,
  /*  */ "_MINUTE_TSP"_Stz,
  /*  */ "_MONTH_DTE"_Stz,
  /*  */ "_MONTH_TSP"_Stz,
  /*  */ "_SECOND_TME"_Stz,
  /*  */ "_SECOND_TSP"_Stz,
  /*  */ "_YEAR_DTE"_Stz,
  /*  */ "_YEAR_TSP"_Stz,

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // in expression operators

  /*  */ "_INX_ANYUNK"_Stz,
  /*  */ "_INX_BCDBCD"_Stz,
  /*  */ "_INX_BOLBOL"_Stz,
  /*  */ "_INX_DTEDTE"_Stz,
  /*  */ "_INX_FLTFLT"_Stz,
  /*  */ "_INX_INTINT"_Stz,
  /*  */ "_INX_IVDIVD"_Stz,
  /*  */ "_INX_IVYIVY"_Stz,
  /*  */ "_INX_TMETME"_Stz,
  /*  */ "_INX_TSPTSP"_Stz,
  /*  */ "_INX_VCHVCH"_Stz,
  /*  */ "_INX_UNKANY"_Stz,

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // logical operators

  /*  */ "_AND_BOLBOL"_Stz,
  /*  */ "_NOT_BOLBOL"_Stz,
  /*  */ "_OR_BOLBOL"_Stz,
  /*  */ "_XOR_BOLBOL"_Stz,

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null test operators
  /*  */ "_ISNULL"_Stz,
  /*  */ "_ISNOTNULL"_Stz,

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push
  /*  */ "_PSH_BOL"_Stz,
  /*  */ "_PSH_BCD"_Stz,
  /*  */ "_PSH_DTE"_Stz,
  /*  */ "_PSH_FLT"_Stz,
  /*  */ "_PSH_INT"_Stz,
  /*  */ "_PSH_IVD"_Stz,
  /*  */ "_PSH_IVY"_Stz,
  /*  */ "_PSH_TME"_Stz,
  /*  */ "_PSH_TSP"_Stz,
  /*  */ "_PSH_UNK"_Stz,
  /*  */ "_PSH_VCH"_Stz,

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // relational operators

  // equal
  /*  */ "_EQ_ANYUNK"_Stz,
  /*  */ "_EQ_BCDBCD"_Stz,
  /*  */ "_EQ_BOLBOL"_Stz,
  /*  */ "_EQ_DTEDTE"_Stz,
  /*  */ "_EQ_FLTFLT"_Stz,
  /*  */ "_EQ_INTINT"_Stz,
  /*  */ "_EQ_IVDIVD"_Stz,
  /*  */ "_EQ_IVYIVY"_Stz,
  /*  */ "_EQ_TMETME"_Stz,
  /*  */ "_EQ_TSPTSP"_Stz,
  /*  */ "_EQ_UNKANY"_Stz,
  /*  */ "_EQ_VCHVCH"_Stz,

  // greater than equal
  /*  */ "_GE_ANYUNK"_Stz,
  /*  */ "_GE_BCDBCD"_Stz,
  /*  */ "_GE_BOLBOL"_Stz,
  /*  */ "_GE_DTEDTE"_Stz,
  /*  */ "_GE_FLTFLT"_Stz,
  /*  */ "_GE_INTINT"_Stz,
  /*  */ "_GE_IVDIVD"_Stz,
  /*  */ "_GE_IVYIVY"_Stz,
  /*  */ "_GE_TMETME"_Stz,
  /*  */ "_GE_TSPTSP"_Stz,
  /*  */ "_GE_UNKANY"_Stz,
  /*  */ "_GE_VCHVCH"_Stz,

  // greater than
  /*  */ "_GT_ANYUNK"_Stz,
  /*  */ "_GT_BCDBCD"_Stz,
  /*  */ "_GT_BOLBOL"_Stz,
  /*  */ "_GT_DTEDTE"_Stz,
  /*  */ "_GT_FLTFLT"_Stz,
  /*  */ "_GT_INTINT"_Stz,
  /*  */ "_GT_IVDIVD"_Stz,
  /*  */ "_GT_IVYIVY"_Stz,
  /*  */ "_GT_TMETME"_Stz,
  /*  */ "_GT_TSPTSP"_Stz,
  /*  */ "_GT_UNKANY"_Stz,
  /*  */ "_GT_VCHVCH"_Stz,

  // greater than aggregate
  /*  */ "_GTA_ANYUNK"_Stz,
  /*  */ "_GTA_BCDBCD"_Stz,
  /*  */ "_GTA_BOLBOL"_Stz,
  /*  */ "_GTA_DTEDTE"_Stz,
  /*  */ "_GTA_FLTFLT"_Stz,
  /*  */ "_GTA_INTINT"_Stz,
  /*  */ "_GTA_IVDIVD"_Stz,
  /*  */ "_GTA_IVYIVY"_Stz,
  /*  */ "_GTA_TMETME"_Stz,
  /*  */ "_GTA_TSPTSP"_Stz,
  /*  */ "_GTA_UNKANY"_Stz,
  /*  */ "_GTA_VCHVCH"_Stz,

  // less than equal
  /*  */ "_LE_ANYUNK"_Stz,
  /*  */ "_LE_BCDBCD"_Stz,
  /*  */ "_LE_BOLBOL"_Stz,
  /*  */ "_LE_DTEDTE"_Stz,
  /*  */ "_LE_FLTFLT"_Stz,
  /*  */ "_LE_INTINT"_Stz,
  /*  */ "_LE_IVDIVD"_Stz,
  /*  */ "_LE_IVYIVY"_Stz,
  /*  */ "_LE_TMETME"_Stz,
  /*  */ "_LE_TSPTSP"_Stz,
  /*  */ "_LE_UNKANY"_Stz,
  /*  */ "_LE_VCHVCH"_Stz,

  // less than
  /*  */ "_LT_ANYUNK"_Stz,
  /*  */ "_LT_BCDBCD"_Stz,
  /*  */ "_LT_BOLBOL"_Stz,
  /*  */ "_LT_DTEDTE"_Stz,
  /*  */ "_LT_FLTFLT"_Stz,
  /*  */ "_LT_INTINT"_Stz,
  /*  */ "_LT_IVDIVD"_Stz,
  /*  */ "_LT_IVYIVY"_Stz,
  /*  */ "_LT_TMETME"_Stz,
  /*  */ "_LT_TSPTSP"_Stz,
  /*  */ "_LT_UNKANY"_Stz,
  /*  */ "_LT_VCHVCH"_Stz,

  // less than aggregates
  /*  */ "_LTA_ANYUNK"_Stz,
  /*  */ "_LTA_BCDBCD"_Stz,
  /*  */ "_LTA_BOLBOL"_Stz,
  /*  */ "_LTA_DTEDTE"_Stz,
  /*  */ "_LTA_FLTFLT"_Stz,
  /*  */ "_LTA_INTINT"_Stz,
  /*  */ "_LTA_IVDIVD"_Stz,
  /*  */ "_LTA_IVYIVY"_Stz,
  /*  */ "_LTA_TMETME"_Stz,
  /*  */ "_LTA_TSPTSP"_Stz,
  /*  */ "_LTA_UNKANY"_Stz,
  /*  */ "_LTA_VCHVCH"_Stz,

  // not equal
  /*  */ "_NE_ANYUNK"_Stz,
  /*  */ "_NE_BCDBCD"_Stz,
  /*  */ "_NE_BOLBOL"_Stz,
  /*  */ "_NE_DTEDTE"_Stz,
  /*  */ "_NE_FLTFLT"_Stz,
  /*  */ "_NE_INTINT"_Stz,
  /*  */ "_NE_IVDIVD"_Stz,
  /*  */ "_NE_IVYIVY"_Stz,
  /*  */ "_NE_TMETME"_Stz,
  /*  */ "_NE_TSPTSP"_Stz,
  /*  */ "_NE_UNKANY"_Stz,
  /*  */ "_NE_VCHVCH"_Stz,

  // string operators
  /*  */ "_STR_COMPARE"_Stz,
  /*  */ "_STR_LIKE"_Stz,
  /*  */ "_STR_SUBSTRING2"_Stz,
  /*  */ "_STR_SUBSTRING3"_Stz,

  // date operators
  /*  */ "_DTE_NOW"_Stz,
  /*  */ "_DTE_YEAR"_Stz,

//  /*  */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
