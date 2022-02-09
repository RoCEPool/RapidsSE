/*============================================================================*/
/* eFM.h -- Funtion macros                                                    */
/*============================================================================*/
#pragma once


#ifdef __TINYC__

// when being included by the tcc compiler
enum eFM // function macros

#else

// when not being included by the tcc compiler
enum eFM : u2 // function macros

#endif
{
  /*  0 */ FM_NONE=0,

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // arithmetic operators

  // add
  /*  */ FM_ADD_BCDBCD,           // bcd + bcd
  /*  */ FM_ADD_BCDFLT,           // bcd + int
  /*  */ FM_ADD_BCDINT,           // bcd + int
  /*  */ FM_ADD_DTEIVL,           // dte + ivl
  /*  */ FM_ADD_FLTBCD,           // flt + bcd
  /*  */ FM_ADD_FLTINT,           // flt + int
  /*  */ FM_ADD_FLTFLT,           // flt + flt
  /*  */ FM_ADD_INTBCD,           // int + bcd
  /*  */ FM_ADD_INTFLT,           // int + flt
  /*  */ FM_ADD_INTINT,           // int + int
  /*  */ FM_ADD_IVLDTE,           // ivl + dte
  /*  */ FM_ADD_IVLIVL,           // ivl + ivl
  /*  */ FM_ADD_IVLTME,           // ivl + tme
  /*  */ FM_ADD_IVLTSP,           // ivl + tsp
  /*  */ FM_ADD_TMEIVL,           // tme + ivl
  /*  */ FM_ADD_TSPIVL,           // tsp + ivl
  /*  */ FM_ADD_UNKXYZ,           // unk + xyz
  /*  */ FM_ADD_XYZUNK,           // xyz + unk

  // divide
  /*  */ FM_DIV_BCDBCD,           // bcd / bcd
  /*  */ FM_DIV_BCDFLT,           // bcd / int
  /*  */ FM_DIV_BCDINT,           // bcd / int
  /*  */ FM_DIV_DTEIVL,           // dte / ivl
  /*  */ FM_DIV_FLTBCD,           // flt / bcd
  /*  */ FM_DIV_FLTINT,           // flt / int
  /*  */ FM_DIV_FLTFLT,           // flt / flt
  /*  */ FM_DIV_INTBCD,           // int / bcd
  /*  */ FM_DIV_INTFLT,           // int / flt
  /*  */ FM_DIV_INTINT,           // int / int
  /*  */ FM_DIV_IVLDTE,           // ivl / dte
  /*  */ FM_DIV_IVLIVL,           // ivl / ivl
  /*  */ FM_DIV_IVLTME,           // ivl / tme
  /*  */ FM_DIV_IVLTSP,           // ivl / tsp
  /*  */ FM_DIV_TMEIVL,           // tme / ivl
  /*  */ FM_DIV_TSPIVL,           // tsp / ivl
  /*  */ FM_DIV_UNKXYZ,           // unk / xyz
  /*  */ FM_DIV_XYZUNK,           // xyz / unk
  
  // modulo
  /*  */ FM_MOD_INTINT,          // int % int
  /*  */ FM_MOD_UNKXYZ,          // unk % xyz
  /*  */ FM_MOD_XYZUNK,          // xyz % unk

  // multiply
  /*  */ FM_MUL_BCDBCD,           // bcd * bcd
  /*  */ FM_MUL_BCDFLT,           // bcd * int
  /*  */ FM_MUL_BCDINT,           // bcd * int
  /*  */ FM_MUL_DTEIVL,           // dte * ivl
  /*  */ FM_MUL_FLTBCD,           // flt * bcd
  /*  */ FM_MUL_FLTINT,           // flt * int
  /*  */ FM_MUL_FLTFLT,           // flt * flt
  /*  */ FM_MUL_INTBCD,           // int * bcd
  /*  */ FM_MUL_INTFLT,           // int * flt
  /*  */ FM_MUL_INTINT,           // int * int
  /*  */ FM_MUL_IVLDTE,           // ivl * dte
  /*  */ FM_MUL_IVLIVL,           // ivl * ivl
  /*  */ FM_MUL_IVLTME,           // ivl * tme
  /*  */ FM_MUL_IVLTSP,           // ivl * tsp
  /*  */ FM_MUL_TMEIVL,           // tme * ivl
  /*  */ FM_MUL_TSPIVL,           // tsp * ivl
  /*  */ FM_MUL_UNKXYZ,           // unk * xyz
  /*  */ FM_MUL_XYZUNK,           // xyz * unk

  // negate
  /*  */ FM_NEG_BCD,             // -bcd
  /*  */ FM_NEG_FLT,             // -flt
  /*  */ FM_NEG_INT,             // -int
  /*  */ FM_NEG_IVL,             // -ivl

  // subtract
  /*  */ FM_SUB_BCDBCD,           // bcd * bcd
  /*  */ FM_SUB_BCDFLT,           // bcd * int
  /*  */ FM_SUB_BCDINT,           // bcd * int
  /*  */ FM_SUB_DTEIVL,           // dte * ivl
  /*  */ FM_SUB_FLTBCD,           // flt * bcd
  /*  */ FM_SUB_FLTINT,           // flt * int
  /*  */ FM_SUB_FLTFLT,           // flt * flt
  /*  */ FM_SUB_INTBCD,           // int * bcd
  /*  */ FM_SUB_INTFLT,           // int * flt
  /*  */ FM_SUB_INTINT,           // int * int
  /*  */ FM_SUB_IVLDTE,           // ivl * dte
  /*  */ FM_SUB_IVLIVL,           // ivl * ivl
  /*  */ FM_SUB_IVLTME,           // ivl * tme
  /*  */ FM_SUB_IVLTSP,           // ivl * tsp
  /*  */ FM_SUB_TMEIVL,           // tme * ivl
  /*  */ FM_SUB_TSPIVL,           // tsp * ivl
  /*  */ FM_SUB_UNKXYZ,           // unk * xyz
  /*  */ FM_SUB_XYZUNK,           // xyz * unk

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast operators

  /*  */ FM_BOL_TOVCH,          // bol to vch
  /*  */ FM_BOL_TOUNK,          // bol to unk

  /*  */ FM_DTE_TOTSP,          // dte to tsp
  /*  */ FM_DTE_TOVCH,          // dte to vch
  /*  */ FM_DTE_TOUNK,          // dte to unk

  /*  */ FM_BCD_TOFLT,          // bcd to flt
  /*  */ FM_BCD_TOINT,          // bcd to int
  /*  */ FM_BCD_TOVCH,          // bcd to vch
  /*  */ FM_BCD_TOUNK,          // bcd to unk

  /*  */ FM_FLT_TOBCD,          // flt to bcd
  /*  */ FM_FLT_TOINT,          // flt to int
  /*  */ FM_FLT_TOVCH,          // flt to vch
  /*  */ FM_FLT_TOUNK,          // flt to unk

  /*  */ FM_INT_TOBCD,          // int to bcd
  /*  */ FM_INT_TOFLT,          // int to flt
  /*  */ FM_INT_TOVCH,          // int to vch
  /*  */ FM_INT_TOUNK,          // int to unk

  /*  */ FM_IVL_TOVCH,          // ivl to vch
  /*  */ FM_IVL_TOUNK,          // ivl to unk

  /*  */ FM_IVY_TOVCH,          // ivy to vch
  /*  */ FM_IVY_TOUNK,          // ivy to unk

  /*  */ FM_TME_TOTSP,          // tme to tsp
  /*  */ FM_TME_TOVCH,          // tme to vch
  /*  */ FM_TME_TOUNK,          // tme to unk

  /*  */ FM_TSP_TODTE,          // tsp to dte
  /*  */ FM_TSP_TOTME,          // tsp to tme
  /*  */ FM_TSP_TOVCH,          // tsp to vch
  /*  */ FM_TSP_TOUNK,          // tsp to unk

  /*  */ FM_UNK_TOBCD,          // unk to bcd
  /*  */ FM_UNK_TOBOL,          // unk to bol
  /*  */ FM_UNK_TODTE,          // unk to dte
  /*  */ FM_UNK_TOFLT,          // unk to flt
  /*  */ FM_UNK_TOINT,          // unk to int
  /*  */ FM_UNK_TOIVL,          // unk to ivl
  /*  */ FM_UNK_TOIVY,          // unk to ivy
  /*  */ FM_UNK_TOTME,          // unk to tme
  /*  */ FM_UNK_TOTSP,          // unk to tsp
  /*  */ FM_UNK_TOVCH,          // unk to vch

  /*  */ FM_VCH_TOBCD,          // vch to bcd
  /*  */ FM_VCH_TODTE,          // vch to dte
  /*  */ FM_VCH_TOFLT,          // vch to flt
  /*  */ FM_VCH_TOINT,          // vch to int
  /*  */ FM_VCH_TOIVL,          // vch to ivl
  /*  */ FM_VCH_TOIVY,          // vch to ivy
  /*  */ FM_VCH_TOTME,          // vch to tme
  /*  */ FM_VCH_TOTSP,          // vch to tsp
  /*  */ FM_VCH_TOUNK,          // vch to unk

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // logical operators

  // equal
  /*  */ FM_AND_BOLBOL,        // bol && bol
  /*  */ FM_NOT_BOLBOL,        // ! bol
  /*  */ FM_OR_BOLBOL,         // bol || bol
  /*  */ FM_XOR_BOLBOL,        // bol xor bol
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null test operators

  /*  */ FM_ISNULL,           // test if value is null
  /*  */ FM_ISNOTNULL,        // test if value is not null

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // relational operators

  // equal
  /*  */ FM_EQ_BCDBCD,        // bcd == bcd
  /*  */ FM_EQ_BCDFLT,        // bcd == flt
  /*  */ FM_EQ_BCDINT,        // bcd == int
  /*  */ FM_EQ_BOLBOL,        // bol == bol
  /*  */ FM_EQ_DTEDTE,        // dte == bcd
  /*  */ FM_EQ_DTETSP,        // dte == tsp
  /*  */ FM_EQ_FLTBCD,        // flt == bcd
  /*  */ FM_EQ_FLTFLT,        // flt == flt
  /*  */ FM_EQ_FLTINT,        // flt == int
  /*  */ FM_EQ_INTBCD,        // int == bcd
  /*  */ FM_EQ_INTFLT,        // int == flt
  /*  */ FM_EQ_INTINT,        // int == int
  /*  */ FM_EQ_IVLIVL,        // ivl == ivl
  /*  */ FM_EQ_TMETME,        // tme == tme
  /*  */ FM_EQ_TSPDTE,        // tsp == dte
  /*  */ FM_EQ_TSPTSP,        // tsp == tsp
  /*  */ FM_EQ_VCHVCH,        // vch == vch
  /*  */ FM_EQ_UNKXYZ,        // unk == xyz
  /*  */ FM_EQ_XYZUNK,        // xyz == unk

  // greater than equal
  /*  */ FM_GE_BCDBCD,        // bcd >= bcd
  /*  */ FM_GE_BCDFLT,        // bcd >= flt
  /*  */ FM_GE_BCDINT,        // bcd >= int
  /*  */ FM_GE_BOLBOL,        // bol >= bol
  /*  */ FM_GE_DTEDTE,        // dte >= bcd
  /*  */ FM_GE_DTETSP,        // dte >= tsp
  /*  */ FM_GE_FLTBCD,        // flt >= bcd
  /*  */ FM_GE_FLTFLT,        // flt >= flt
  /*  */ FM_GE_FLTINT,        // flt >= int
  /*  */ FM_GE_INTBCD,        // int >= bcd
  /*  */ FM_GE_INTFLT,        // int >= flt
  /*  */ FM_GE_INTINT,        // int >= int
  /*  */ FM_GE_IVLIVL,        // ivl >= ivl
  /*  */ FM_GE_TMETME,        // tme >= tme
  /*  */ FM_GE_TSPDTE,        // tsp >= dte
  /*  */ FM_GE_TSPTSP,        // tsp >= tsp
  /*  */ FM_GE_VCHVCH,        // vch >= vch
  /*  */ FM_GE_UNKXYZ,        // unk >= xyz
  /*  */ FM_GE_XYZUNK,        // xyz >= unk

  // greater than
  /*  */ FM_GT_BCDBCD,        // bcd > bcd
  /*  */ FM_GT_BCDFLT,        // bcd > flt
  /*  */ FM_GT_BCDINT,        // bcd > int
  /*  */ FM_GT_BOLBOL,        // bol > bol
  /*  */ FM_GT_DTEDTE,        // dte > bcd
  /*  */ FM_GT_DTETSP,        // dte > tsp
  /*  */ FM_GT_FLTBCD,        // flt > bcd
  /*  */ FM_GT_FLTFLT,        // flt > flt
  /*  */ FM_GT_FLTINT,        // flt > int
  /*  */ FM_GT_INTBCD,        // int > bcd
  /*  */ FM_GT_INTFLT,        // int > flt
  /*  */ FM_GT_INTINT,        // int > int
  /*  */ FM_GT_IVLIVL,        // ivl > ivl
  /*  */ FM_GT_TMETME,        // tme > tme
  /*  */ FM_GT_TSPDTE,        // tsp > dte
  /*  */ FM_GT_TSPTSP,        // tsp > tsp
  /*  */ FM_GT_VCHVCH,        // vch > vch
  /*  */ FM_GT_UNKXYZ,        // unk > xyz
  /*  */ FM_GT_XYZUNK,        // xyz > unk

  // less than equal
  /*  */ FM_LE_BCDBCD,        // bcd <= bcd
  /*  */ FM_LE_BCDFLT,        // bcd <= flt
  /*  */ FM_LE_BCDINT,        // bcd <= int
  /*  */ FM_LE_BOLBOL,        // bol <= bol
  /*  */ FM_LE_DTEDTE,        // dte <= bcd
  /*  */ FM_LE_DTETSP,        // dte <= tsp
  /*  */ FM_LE_FLTBCD,        // flt <= bcd
  /*  */ FM_LE_FLTFLT,        // flt <= flt
  /*  */ FM_LE_FLTINT,        // flt <= int
  /*  */ FM_LE_INTBCD,        // int <= bcd
  /*  */ FM_LE_INTFLT,        // int <= flt
  /*  */ FM_LE_INTINT,        // int <= int
  /*  */ FM_LE_IVLIVL,        // ivl <= ivl
  /*  */ FM_LE_TMETME,        // tme <= tme
  /*  */ FM_LE_TSPDTE,        // tsp <= dte
  /*  */ FM_LE_TSPTSP,        // tsp <= tsp
  /*  */ FM_LE_VCHVCH,        // vch <= vch
  /*  */ FM_LE_UNKXYZ,        // unk <= xyz
  /*  */ FM_LE_XYZUNK,        // xyz <= unk

  // less than equal
  /*  */ FM_LT_BCDBCD,        // bcd < bcd
  /*  */ FM_LT_BCDFLT,        // bcd < flt
  /*  */ FM_LT_BCDINT,        // bcd < int
  /*  */ FM_LT_BOLBOL,        // bol < bol
  /*  */ FM_LT_DTEDTE,        // dte < bcd
  /*  */ FM_LT_DTETSP,        // dte < tsp
  /*  */ FM_LT_FLTBCD,        // flt < bcd
  /*  */ FM_LT_FLTFLT,        // flt < flt
  /*  */ FM_LT_FLTINT,        // flt < int
  /*  */ FM_LT_INTBCD,        // int < bcd
  /*  */ FM_LT_INTFLT,        // int < flt
  /*  */ FM_LT_INTINT,        // int < int
  /*  */ FM_LT_IVLIVL,        // ivl < ivl
  /*  */ FM_LT_TMETME,        // tme < tme
  /*  */ FM_LT_TSPDTE,        // tsp < dte
  /*  */ FM_LT_TSPTSP,        // tsp < tsp
  /*  */ FM_LT_VCHVCH,        // vch < vch
  /*  */ FM_LT_UNKXYZ,        // unk < xyz
  /*  */ FM_LT_XYZUNK,        // xyz < unk

  // not equal
  /*  */ FM_NE_BCDBCD,        // bcd != bcd
  /*  */ FM_NE_BCDFLT,        // bcd != flt
  /*  */ FM_NE_BCDINT,        // bcd != int
  /*  */ FM_NE_BOLBOL,        // bol != bol
  /*  */ FM_NE_DTEDTE,        // dte != bcd
  /*  */ FM_NE_DTETSP,        // dte != tsp
  /*  */ FM_NE_FLTBCD,        // flt != bcd
  /*  */ FM_NE_FLTFLT,        // flt != flt
  /*  */ FM_NE_FLTINT,        // flt != int
  /*  */ FM_NE_INTBCD,        // int != bcd
  /*  */ FM_NE_INTFLT,        // int != flt
  /*  */ FM_NE_INTINT,        // int != int
  /*  */ FM_NE_IVLIVL,        // ivl != ivl
  /*  */ FM_NE_TMETME,        // tme != tme
  /*  */ FM_NE_TSPDTE,        // tsp != dte
  /*  */ FM_NE_TSPTSP,        // tsp != tsp
  /*  */ FM_NE_VCHVCH,        // vch != vch
  /*  */ FM_NE_UNKXYZ,        // unk != xyz
  /*  */ FM_NE_XYZUNK,        // xyz != unk

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // string operators
  /*  */ FM_STR_COMPARE,      // string compare
  /*  */ FM_STR_LIKE,         // string like
  /*  */ FM_STR_SUBSTRING,    // string substring

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // date operators
  /*  M  */ FM_DTE_NOW,          // get current date
  /*  M  */ FM_DTE_YEAR,         // get year of date

  /*  */ FM_ENUMS
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

#ifndef __TINYC__

// when not being included by the tcc compiler
static Stz* aFM[]
{
  /*  0 */ "NONE"_Stz,

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // arithmetic operators

  // add
  /*  */ "_ADD_BCDBCD"_Stz,
  /*  */ "_ADD_BCDFLT"_Stz,
  /*  */ "_ADD_BCDINT"_Stz,
  /*  */ "_ADD_DTEIVL"_Stz,
  /*  */ "_ADD_FLTBCD"_Stz,
  /*  */ "_ADD_FLTINT"_Stz,
  /*  */ "_ADD_FLTFLT"_Stz,
  /*  */ "_ADD_INTBCD"_Stz,
  /*  */ "_ADD_INTFLT"_Stz,
  /*  */ "_ADD_INTINT"_Stz,
  /*  */ "_ADD_IVLDTE"_Stz,
  /*  */ "_ADD_IVLIVL"_Stz,
  /*  */ "_ADD_IVLTME"_Stz,
  /*  */ "_ADD_IVLTSP"_Stz,
  /*  */ "_ADD_TMEIVL"_Stz,
  /*  */ "_ADD_TSPIVL"_Stz,
  /*  */ "_ADD_UNKXYZ"_Stz,
  /*  */ "_ADD_XYZUNK"_Stz,

  // divide
  /*  */ "_DIV_BCDBCD"_Stz,
  /*  */ "_DIV_BCDFLT"_Stz,
  /*  */ "_DIV_BCDINT"_Stz,
  /*  */ "_DIV_DTEIVL"_Stz,
  /*  */ "_DIV_FLTBCD"_Stz,
  /*  */ "_DIV_FLTINT"_Stz,
  /*  */ "_DIV_FLTFLT"_Stz,
  /*  */ "_DIV_INTBCD"_Stz,
  /*  */ "_DIV_INTFLT"_Stz,
  /*  */ "_DIV_INTINT"_Stz,
  /*  */ "_DIV_IVLDTE"_Stz,
  /*  */ "_DIV_IVLIVL"_Stz,
  /*  */ "_DIV_IVLTME"_Stz,
  /*  */ "_DIV_IVLTSP"_Stz,
  /*  */ "_DIV_TMEIVL"_Stz,
  /*  */ "_DIV_TSPIVL"_Stz,
  /*  */ "_DIV_UNKXYZ"_Stz,
  /*  */ "_DIV_XYZUNK"_Stz,

  // modulo
  /*  */ "_MOD_INTINT"_Stz,
  /*  */ "_MOD_UNKXYZ"_Stz,
  /*  */ "_MOD_XYZUNK"_Stz,

  // multiply
  /*  */ "_MUL_BCDBCD"_Stz,
  /*  */ "_MUL_BCDFLT"_Stz,
  /*  */ "_MUL_BCDINT"_Stz,
  /*  */ "_MUL_DTEIVL"_Stz,
  /*  */ "_MUL_FLTBCD"_Stz,
  /*  */ "_MUL_FLTINT"_Stz,
  /*  */ "_MUL_FLTFLT"_Stz,
  /*  */ "_MUL_INTBCD"_Stz,
  /*  */ "_MUL_INTFLT"_Stz,
  /*  */ "_MUL_INTINT"_Stz,
  /*  */ "_MUL_IVLDTE"_Stz,
  /*  */ "_MUL_IVLIVL"_Stz,
  /*  */ "_MUL_IVLTME"_Stz,
  /*  */ "_MUL_IVLTSP"_Stz,
  /*  */ "_MUL_TMEIVL"_Stz,
  /*  */ "_MUL_TSPIVL"_Stz,
  /*  */ "_MUL_UNKXYZ"_Stz,
  /*  */ "_MUL_XYZUNK"_Stz,

  // negate
  /*  */ "_NEG_BCD"_Stz,
  /*  */ "_NEG_FLT"_Stz,
  /*  */ "_NEG_INT"_Stz,
  /*  */ "_NEG_IVL"_Stz,

  // subtract
  /*  */ "_SUB_BCDBCD"_Stz,
  /*  */ "_SUB_BCDFLT"_Stz,
  /*  */ "_SUB_BCDINT"_Stz,
  /*  */ "_SUB_DTEIVL"_Stz,
  /*  */ "_SUB_FLTBCD"_Stz,
  /*  */ "_SUB_FLTINT"_Stz,
  /*  */ "_SUB_FLTFLT"_Stz,
  /*  */ "_SUB_INTBCD"_Stz,
  /*  */ "_SUB_INTFLT"_Stz,
  /*  */ "_SUB_INTINT"_Stz,
  /*  */ "_SUB_IVLDTE"_Stz,
  /*  */ "_SUB_IVLIVL"_Stz,
  /*  */ "_SUB_IVLTME"_Stz,
  /*  */ "_SUB_IVLTSP"_Stz,
  /*  */ "_SUB_TMEIVL"_Stz,
  /*  */ "_SUB_TSPIVL"_Stz,
  /*  */ "_SUB_UNKXYZ"_Stz,
  /*  */ "_SUB_XYZUNK"_Stz,

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast operators

  /*  */ "_BOL_TOVCH"_Stz,
  /*  */ "_BOL_TOUNK"_Stz,

  /*  */ "_DTE_TOTSP"_Stz,
  /*  */ "_DTE_TOVCH"_Stz,
  /*  */ "_DTE_TOUNK"_Stz,

  /*  */ "_BCD_TOFLT"_Stz,
  /*  */ "_BCD_TOINT"_Stz,
  /*  */ "_BCD_TOVCH"_Stz,
  /*  */ "_BCD_TOUNK"_Stz,

  /*  */ "_FLT_TOBCD"_Stz,
  /*  */ "_FLT_TOINT"_Stz,
  /*  */ "_FLT_TOVCH"_Stz,
  /*  */ "_FLT_TOUNK"_Stz,

  /*  */ "_INT_TOBCD"_Stz,
  /*  */ "_INT_TOFLT"_Stz,
  /*  */ "_INT_TOVCH"_Stz,
  /*  */ "_INT_TOUNK"_Stz,

  /*  */ "_IVL_TOVCH"_Stz,
  /*  */ "_IVL_TOUNK"_Stz,

  /*  */ "_IVY_TOVCH"_Stz,
  /*  */ "_IVY_TOUNK"_Stz,

  /*  */ "_TME_TOTSP"_Stz,
  /*  */ "_TME_TOVCH"_Stz,
  /*  */ "_TME_TOUNK"_Stz,

  /*  */ "_TSP_TODTE"_Stz,
  /*  */ "_TSP_TOTME"_Stz,
  /*  */ "_TSP_TOVCH"_Stz,
  /*  */ "_TSP_TOUNK"_Stz,

  /*  */ "_UNK_TOBCD"_Stz,
  /*  */ "_UNK_TOBOL"_Stz,
  /*  */ "_UNK_TODTE"_Stz,
  /*  */ "_UNK_TOFLT"_Stz,
  /*  */ "_UNK_TOINT"_Stz,
  /*  */ "_UNK_TOIVL"_Stz,
  /*  */ "_UNK_TOIVY"_Stz,
  /*  */ "_UNK_TOTME"_Stz,
  /*  */ "_UNK_TOTSP"_Stz,
  /*  */ "_UNK_TOVCH"_Stz,

  /*  */ "_VCH_TOBCD"_Stz,
  /*  */ "_VCH_TODTE"_Stz,
  /*  */ "_VCH_TOFLT"_Stz,
  /*  */ "_VCH_TOINT"_Stz,
  /*  */ "_VCH_TOIVL"_Stz,
  /*  */ "_VCH_TOIVY"_Stz,
  /*  */ "_VCH_TOTME"_Stz,
  /*  */ "_VCH_TOTSP"_Stz,
  /*  */ "_VCH_TOUNK"_Stz,

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
  // relational operators

  // equal
  /*  */ "_EQ_BCDBCD"_Stz,
  /*  */ "_EQ_BCDFLT"_Stz,
  /*  */ "_EQ_BCDINT"_Stz,
  /*  */ "_EQ_BOLBOL"_Stz,
  /*  */ "_EQ_DTEDTE"_Stz,
  /*  */ "_EQ_DTETSP"_Stz,
  /*  */ "_EQ_FLTBCD"_Stz,
  /*  */ "_EQ_FLTFLT"_Stz,
  /*  */ "_EQ_FLTINT"_Stz,
  /*  */ "_EQ_INTBCD"_Stz,
  /*  */ "_EQ_INTFLT"_Stz,
  /*  */ "_EQ_INTINT"_Stz,
  /*  */ "_EQ_IVLIVL"_Stz,
  /*  */ "_EQ_TMETME"_Stz,
  /*  */ "_EQ_TSPDTE"_Stz,
  /*  */ "_EQ_TSPTSP"_Stz,
  /*  */ "_EQ_VCHVCH"_Stz,
  /*  */ "_EQ_UNKXYZ"_Stz,
  /*  */ "_EQ_XYZUNK"_Stz,

  // greater than equal
  /*  */ "_GE_BCDBCD"_Stz,
  /*  */ "_GE_BCDFLT"_Stz,
  /*  */ "_GE_BCDINT"_Stz,
  /*  */ "_GE_BOLBOL"_Stz,
  /*  */ "_GE_DTEDTE"_Stz,
  /*  */ "_GE_DTETSP"_Stz,
  /*  */ "_GE_FLTBCD"_Stz,
  /*  */ "_GE_FLTFLT"_Stz,
  /*  */ "_GE_FLTINT"_Stz,
  /*  */ "_GE_INTBCD"_Stz,
  /*  */ "_GE_INTFLT"_Stz,
  /*  */ "_GE_INTINT"_Stz,
  /*  */ "_GE_IVLIVL"_Stz,
  /*  */ "_GE_TMETME"_Stz,
  /*  */ "_GE_TSPDTE"_Stz,
  /*  */ "_GE_TSPTSP"_Stz,
  /*  */ "_GE_VCHVCH"_Stz,
  /*  */ "_GE_UNKXYZ"_Stz,
  /*  */ "_GE_XYZUNK"_Stz,

  // greater than
  /*  */ "_GT_BCDBCD"_Stz,
  /*  */ "_GT_BCDFLT"_Stz,
  /*  */ "_GT_BCDINT"_Stz,
  /*  */ "_GT_BOLBOL"_Stz,
  /*  */ "_GT_DTEDTE"_Stz,
  /*  */ "_GT_DTETSP"_Stz,
  /*  */ "_GT_FLTBCD"_Stz,
  /*  */ "_GT_FLTFLT"_Stz,
  /*  */ "_GT_FLTINT"_Stz,
  /*  */ "_GT_INTBCD"_Stz,
  /*  */ "_GT_INTFLT"_Stz,
  /*  */ "_GT_INTINT"_Stz,
  /*  */ "_GT_IVLIVL"_Stz,
  /*  */ "_GT_TMETME"_Stz,
  /*  */ "_GT_TSPDTE"_Stz,
  /*  */ "_GT_TSPTSP"_Stz,
  /*  */ "_GT_VCHVCH"_Stz,
  /*  */ "_GT_UNKXYZ"_Stz,
  /*  */ "_GT_XYZUNK"_Stz,

    // less than equal
  /*  */ "_LE_BCDBCD"_Stz,
  /*  */ "_LE_BCDFLT"_Stz,
  /*  */ "_LE_BCDINT"_Stz,
  /*  */ "_LE_BOLBOL"_Stz,
  /*  */ "_LE_DTEDTE"_Stz,
  /*  */ "_LE_DTETSP"_Stz,
  /*  */ "_LE_FLTBCD"_Stz,
  /*  */ "_LE_FLTFLT"_Stz,
  /*  */ "_LE_FLTINT"_Stz,
  /*  */ "_LE_INTBCD"_Stz,
  /*  */ "_LE_INTFLT"_Stz,
  /*  */ "_LE_INTINT"_Stz,
  /*  */ "_LE_IVLIVL"_Stz,
  /*  */ "_LE_TMETME"_Stz,
  /*  */ "_LE_TSPDTE"_Stz,
  /*  */ "_LE_TSPTSP"_Stz,
  /*  */ "_LE_VCHVCH"_Stz,
  /*  */ "_LE_UNKXYZ"_Stz,
  /*  */ "_LE_XYZUNK"_Stz,

    // less than
  /*  */ "_LT_BCDBCD"_Stz,
  /*  */ "_LT_BCDFLT"_Stz,
  /*  */ "_LT_BCDINT"_Stz,
  /*  */ "_LT_BOLBOL"_Stz,
  /*  */ "_LT_DTEDTE"_Stz,
  /*  */ "_LT_DTETSP"_Stz,
  /*  */ "_LT_FLTBCD"_Stz,
  /*  */ "_LT_FLTFLT"_Stz,
  /*  */ "_LT_FLTINT"_Stz,
  /*  */ "_LT_INTBCD"_Stz,
  /*  */ "_LT_INTFLT"_Stz,
  /*  */ "_LT_INTINT"_Stz,
  /*  */ "_LT_IVLIVL"_Stz,
  /*  */ "_LT_TMETME"_Stz,
  /*  */ "_LT_TSPDTE"_Stz,
  /*  */ "_LT_TSPTSP"_Stz,
  /*  */ "_LT_VCHVCH"_Stz,
  /*  */ "_LT_UNKXYZ"_Stz,
  /*  */ "_LT_XYZUNK"_Stz,

  // not equal
  /*  */ "_NE_BCDBCD"_Stz,
  /*  */ "_NE_BCDFLT"_Stz,
  /*  */ "_NE_BCDINT"_Stz,
  /*  */ "_NE_BOLBOL"_Stz,
  /*  */ "_NE_DTEDTE"_Stz,
  /*  */ "_NE_DTETSP"_Stz,
  /*  */ "_NE_FLTBCD"_Stz,
  /*  */ "_NE_FLTFLT"_Stz,
  /*  */ "_NE_FLTINT"_Stz,
  /*  */ "_NE_INTBCD"_Stz,
  /*  */ "_NE_INTFLT"_Stz,
  /*  */ "_NE_INTINT"_Stz,
  /*  */ "_NE_IVLIVL"_Stz,
  /*  */ "_NE_TMETME"_Stz,
  /*  */ "_NE_TSPDTE"_Stz,
  /*  */ "_NE_TSPTSP"_Stz,
  /*  */ "_NE_VCHVCH"_Stz,
  /*  */ "_NE_UNKXYZ"_Stz,
  /*  */ "_NE_XYZUNK"_Stz,
  
  // string operators
  /*  */ "_STR_COMPARE"_Stz,
  /*  */ "_STR_LIKE"_Stz,
  /*  */ "_STR_SUBSTRING"_Stz,

  // date operators
  /*  */ "_DTE_NOW"_Stz,
  /*  */ "_DTE_YEAR"_Stz,

  /*  */ "ENUMS"_Stz
};

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
