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
  // null test operators
  /*  S  */ FM_ISNULL,           // test if top is null, push result
  /*  S  */ FM_ISNULL_STR,       // test if top string is null, push result
  /*  S  */ FM_ISNOTNULL,        // test if top is not null, push result

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // relational operators

  // equal
  /*  S  */ FM_EQ_BCDBCD,        // bcd == bcd
  /*  S  */ FM_EQ_BCDFLT,        // bcd == flt
  /*  S  */ FM_EQ_BCDINT,        // bcd == int
  /*  S  */ FM_EQ_BOLBOL,        // bol == bol
  /*  S  */ FM_EQ_DTEDTE,        // dte == bcd
  /*  S  */ FM_EQ_DTETSP,        // dte == tsp
  /*  S  */ FM_EQ_FLTBCD,        // flt == bcd
  /*  S  */ FM_EQ_FLTFLT,        // flt == flt
  /*  S  */ FM_EQ_FLTINT,        // flt == int
  /*  S  */ FM_EQ_INTBCD,        // int == bcd
  /*  S  */ FM_EQ_INTFLT,        // int == flt
  /*  S  */ FM_EQ_INTINT,        // int == int
  /*  S  */ FM_EQ_IVLIVL,        // ivl == ivl
  /*  S  */ FM_EQ_TMETME,        // tme == tme
  /*  S  */ FM_EQ_TSPDTE,        // tsp == dte
  /*  S  */ FM_EQ_TSPTSP,        // tsp == tsp
  /*  S  */ FM_EQ_VCHVCH,        // vch == vch
  /*  S  */ FM_EQ_UNKXYZ,        // unk == xyz
  /*  S  */ FM_EQ_XYZUNK,        // xyz == unk

  // greater than equal
  /*  S  */ FM_GE_BCDBCD,        // bcd >= bcd
  /*  S  */ FM_GE_BCDFLT,        // bcd >= flt
  /*  S  */ FM_GE_BCDINT,        // bcd >= int
  /*  S  */ FM_GE_BOLBOL,        // bol >= bol
  /*  S  */ FM_GE_DTEDTE,        // dte >= bcd
  /*  S  */ FM_GE_DTETSP,        // dte >= tsp
  /*  S  */ FM_GE_FLTBCD,        // flt >= bcd
  /*  S  */ FM_GE_FLTFLT,        // flt >= flt
  /*  S  */ FM_GE_FLTINT,        // flt >= int
  /*  S  */ FM_GE_INTBCD,        // int >= bcd
  /*  S  */ FM_GE_INTFLT,        // int >= flt
  /*  S  */ FM_GE_INTINT,        // int >= int
  /*  S  */ FM_GE_IVLIVL,        // ivl >= ivl
  /*  S  */ FM_GE_TMETME,        // tme >= tme
  /*  S  */ FM_GE_TSPDTE,        // tsp >= dte
  /*  S  */ FM_GE_TSPTSP,        // tsp >= tsp
  /*  S  */ FM_GE_VCHVCH,        // vch >= vch
  /*  S  */ FM_GE_UNKXYZ,        // unk >= xyz
  /*  S  */ FM_GE_XYZUNK,        // xyz >= unk

  // greater than
  /*  S  */ FM_GT_BCDBCD,        // bcd > bcd
  /*  S  */ FM_GT_BCDFLT,        // bcd > flt
  /*  S  */ FM_GT_BCDINT,        // bcd > int
  /*  S  */ FM_GT_BOLBOL,        // bol > bol
  /*  S  */ FM_GT_DTEDTE,        // dte > bcd
  /*  S  */ FM_GT_DTETSP,        // dte > tsp
  /*  S  */ FM_GT_FLTBCD,        // flt > bcd
  /*  S  */ FM_GT_FLTFLT,        // flt > flt
  /*  S  */ FM_GT_FLTINT,        // flt > int
  /*  S  */ FM_GT_INTBCD,        // int > bcd
  /*  S  */ FM_GT_INTFLT,        // int > flt
  /*  S  */ FM_GT_INTINT,        // int > int
  /*  S  */ FM_GT_IVLIVL,        // ivl > ivl
  /*  S  */ FM_GT_TMETME,        // tme > tme
  /*  S  */ FM_GT_TSPDTE,        // tsp > dte
  /*  S  */ FM_GT_TSPTSP,        // tsp > tsp
  /*  S  */ FM_GT_VCHVCH,        // vch > vch
  /*  S  */ FM_GT_UNKXYZ,        // unk > xyz
  /*  S  */ FM_GT_XYZUNK,        // xyz > unk

  // less than equal
  /*  S  */ FM_LE_BCDBCD,        // bcd <= bcd
  /*  S  */ FM_LE_BCDFLT,        // bcd <= flt
  /*  S  */ FM_LE_BCDINT,        // bcd <= int
  /*  S  */ FM_LE_BOLBOL,        // bol <= bol
  /*  S  */ FM_LE_DTEDTE,        // dte <= bcd
  /*  S  */ FM_LE_DTETSP,        // dte <= tsp
  /*  S  */ FM_LE_FLTBCD,        // flt <= bcd
  /*  S  */ FM_LE_FLTFLT,        // flt <= flt
  /*  S  */ FM_LE_FLTINT,        // flt <= int
  /*  S  */ FM_LE_INTBCD,        // int <= bcd
  /*  S  */ FM_LE_INTFLT,        // int <= flt
  /*  S  */ FM_LE_INTINT,        // int <= int
  /*  S  */ FM_LE_IVLIVL,        // ivl <= ivl
  /*  S  */ FM_LE_TMETME,        // tme <= tme
  /*  S  */ FM_LE_TSPDTE,        // tsp <= dte
  /*  S  */ FM_LE_TSPTSP,        // tsp <= tsp
  /*  S  */ FM_LE_VCHVCH,        // vch <= vch
  /*  S  */ FM_LE_UNKXYZ,        // unk <= xyz
  /*  S  */ FM_LE_XYZUNK,        // xyz <= unk

  // less than equal
  /*  S  */ FM_LT_BCDBCD,        // bcd < bcd
  /*  S  */ FM_LT_BCDFLT,        // bcd < flt
  /*  S  */ FM_LT_BCDINT,        // bcd < int
  /*  S  */ FM_LT_BOLBOL,        // bol < bol
  /*  S  */ FM_LT_DTEDTE,        // dte < bcd
  /*  S  */ FM_LT_DTETSP,        // dte < tsp
  /*  S  */ FM_LT_FLTBCD,        // flt < bcd
  /*  S  */ FM_LT_FLTFLT,        // flt < flt
  /*  S  */ FM_LT_FLTINT,        // flt < int
  /*  S  */ FM_LT_INTBCD,        // int < bcd
  /*  S  */ FM_LT_INTFLT,        // int < flt
  /*  S  */ FM_LT_INTINT,        // int < int
  /*  S  */ FM_LT_IVLIVL,        // ivl < ivl
  /*  S  */ FM_LT_TMETME,        // tme < tme
  /*  S  */ FM_LT_TSPDTE,        // tsp < dte
  /*  S  */ FM_LT_TSPTSP,        // tsp < tsp
  /*  S  */ FM_LT_VCHVCH,        // vch < vch
  /*  S  */ FM_LT_UNKXYZ,        // unk < xyz
  /*  S  */ FM_LT_XYZUNK,        // xyz < unk

  // not equal
  /*  S  */ FM_NE_BCDBCD,        // bcd != bcd
  /*  S  */ FM_NE_BCDFLT,        // bcd != flt
  /*  S  */ FM_NE_BCDINT,        // bcd != int
  /*  S  */ FM_NE_BOLBOL,        // bol != bol
  /*  S  */ FM_NE_DTEDTE,        // dte != bcd
  /*  S  */ FM_NE_DTETSP,        // dte != tsp
  /*  S  */ FM_NE_FLTBCD,        // flt != bcd
  /*  S  */ FM_NE_FLTFLT,        // flt != flt
  /*  S  */ FM_NE_FLTINT,        // flt != int
  /*  S  */ FM_NE_INTBCD,        // int != bcd
  /*  S  */ FM_NE_INTFLT,        // int != flt
  /*  S  */ FM_NE_INTINT,        // int != int
  /*  S  */ FM_NE_IVLIVL,        // ivl != ivl
  /*  S  */ FM_NE_TMETME,        // tme != tme
  /*  S  */ FM_NE_TSPDTE,        // tsp != dte
  /*  S  */ FM_NE_TSPTSP,        // tsp != tsp
  /*  S  */ FM_NE_VCHVCH,        // vch != vch
  /*  S  */ FM_NE_UNKXYZ,        // unk != xyz
  /*  S  */ FM_NE_XYZUNK,        // xyz != unk

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // string operators
  /*  S  */ FM_STR_COMPARE,      // string compare
  /*  S  */ FM_STR_LIKE,         // string like
  /*  S  */ FM_STR_SUBSTRING,    // string substring

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // date operators
  /*  M  */ FM_DTE_NOW,          // get current date
  /*  M  */ FM_DTE_YEAR,         // get year of date

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // conversion/cast operators
  /*  M  */ FM_BCD_TOFLT,        // bcd->flt
  /*  M  */ FM_BCD_TOINT,        // bcd->int

  /*  M  */ FM_FLT_TOBCD,        // flt->bcd
  /*  M  */ FM_FLT_TOINT,        // flt->int

  /*  M  */ FM_INT_TOBCD,        // int->bcd
  /*  M  */ FM_INT_TOFLT,        // int->flt

  /*  M  */ FM_VCH_TOBCD,        // vch->bcd
  /*  M  */ FM_VCH_TOFLT,        // vch->flt
  /*  M  */ FM_VCH_TOINT,        // vch->int

  /*    */ FM_ENUMS
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
  /*    */ "FM_ADD_BCDBCD"_Stz,
  /*    */ "FM_ADD_BCDFLT"_Stz,
  /*    */ "FM_ADD_BCDINT"_Stz,
  /*    */ "FM_ADD_DTEIVL"_Stz,
  /*    */ "FM_ADD_FLTBCD"_Stz,
  /*    */ "FM_ADD_FLTINT"_Stz,
  /*    */ "FM_ADD_FLTFLT"_Stz,
  /*    */ "FM_ADD_INTBCD"_Stz,
  /*    */ "FM_ADD_INTFLT"_Stz,
  /*    */ "FM_ADD_INTINT"_Stz,
  /*    */ "FM_ADD_IVLDTE"_Stz,
  /*    */ "FM_ADD_IVLIVL"_Stz,
  /*    */ "FM_ADD_IVLTME"_Stz,
  /*    */ "FM_ADD_IVLTSP"_Stz,
  /*    */ "FM_ADD_TMEIVL"_Stz,
  /*    */ "FM_ADD_TSPIVL"_Stz,
  /*    */ "FM_ADD_UNKXYZ"_Stz,
  /*    */ "FM_ADD_XYZUNK"_Stz,

  // divide
  /*    */ "FM_DIV_BCDBCD"_Stz,
  /*    */ "FM_DIV_BCDFLT"_Stz,
  /*    */ "FM_DIV_BCDINT"_Stz,
  /*    */ "FM_DIV_DTEIVL"_Stz,
  /*    */ "FM_DIV_FLTBCD"_Stz,
  /*    */ "FM_DIV_FLTINT"_Stz,
  /*    */ "FM_DIV_FLTFLT"_Stz,
  /*    */ "FM_DIV_INTBCD"_Stz,
  /*    */ "FM_DIV_INTFLT"_Stz,
  /*    */ "FM_DIV_INTINT"_Stz,
  /*    */ "FM_DIV_IVLDTE"_Stz,
  /*    */ "FM_DIV_IVLIVL"_Stz,
  /*    */ "FM_DIV_IVLTME"_Stz,
  /*    */ "FM_DIV_IVLTSP"_Stz,
  /*    */ "FM_DIV_TMEIVL"_Stz,
  /*    */ "FM_DIV_TSPIVL"_Stz,
  /*    */ "FM_DIV_UNKXYZ"_Stz,
  /*    */ "FM_DIV_XYZUNK"_Stz,

  // modulo
  /*    */ "FM_MOD_INTINT"_Stz,
  /*    */ "FM_MOD_UNKXYZ"_Stz,
  /*    */ "FM_MOD_XYZUNK"_Stz,

  // multiply
  /*    */ "FM_MUL_BCDBCD"_Stz,
  /*    */ "FM_MUL_BCDFLT"_Stz,
  /*    */ "FM_MUL_BCDINT"_Stz,
  /*    */ "FM_MUL_DTEIVL"_Stz,
  /*    */ "FM_MUL_FLTBCD"_Stz,
  /*    */ "FM_MUL_FLTINT"_Stz,
  /*    */ "FM_MUL_FLTFLT"_Stz,
  /*    */ "FM_MUL_INTBCD"_Stz,
  /*    */ "FM_MUL_INTFLT"_Stz,
  /*    */ "FM_MUL_INTINT"_Stz,
  /*    */ "FM_MUL_IVLDTE"_Stz,
  /*    */ "FM_MUL_IVLIVL"_Stz,
  /*    */ "FM_MUL_IVLTME"_Stz,
  /*    */ "FM_MUL_IVLTSP"_Stz,
  /*    */ "FM_MUL_TMEIVL"_Stz,
  /*    */ "FM_MUL_TSPIVL"_Stz,
  /*    */ "FM_MUL_UNKXYZ"_Stz,
  /*    */ "FM_MUL_XYZUNK"_Stz,

  // negate
  /*    */ "FM_NEG_BCD"_Stz,
  /*    */ "FM_NEG_FLT"_Stz,
  /*    */ "FM_NEG_INT"_Stz,
  /*    */ "FM_NEG_IVL"_Stz,

  // subtract
  /*    */ "FM_SUB_BCDBCD"_Stz,
  /*    */ "FM_SUB_BCDFLT"_Stz,
  /*    */ "FM_SUB_BCDINT"_Stz,
  /*    */ "FM_SUB_DTEIVL"_Stz,
  /*    */ "FM_SUB_FLTBCD"_Stz,
  /*    */ "FM_SUB_FLTINT"_Stz,
  /*    */ "FM_SUB_FLTFLT"_Stz,
  /*    */ "FM_SUB_INTBCD"_Stz,
  /*    */ "FM_SUB_INTFLT"_Stz,
  /*    */ "FM_SUB_INTINT"_Stz,
  /*    */ "FM_SUB_IVLDTE"_Stz,
  /*    */ "FM_SUB_IVLIVL"_Stz,
  /*    */ "FM_SUB_IVLTME"_Stz,
  /*    */ "FM_SUB_IVLTSP"_Stz,
  /*    */ "FM_SUB_TMEIVL"_Stz,
  /*    */ "FM_SUB_TSPIVL"_Stz,
  /*    */ "FM_SUB_UNKXYZ"_Stz,
  /*    */ "FM_SUB_XYZUNK"_Stz,

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null test operators
  /*    */ "FM_ISNULL"_Stz,
  /*    */ "FM_ISNULL_STR"_Stz,
  /*    */ "FM_ISNOTNULL"_Stz,

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // relational operators

  // equal
  /*    */ "FM_EQ_BCDBCD"_Stz,
  /*    */ "FM_EQ_BCDFLT"_Stz,
  /*    */ "FM_EQ_BCDINT"_Stz,
  /*    */ "FM_EQ_BOLBOL"_Stz,
  /*    */ "FM_EQ_DTEDTE"_Stz,
  /*    */ "FM_EQ_DTETSP"_Stz,
  /*    */ "FM_EQ_FLTBCD"_Stz,
  /*    */ "FM_EQ_FLTFLT"_Stz,
  /*    */ "FM_EQ_FLTINT"_Stz,
  /*    */ "FM_EQ_INTBCD"_Stz,
  /*    */ "FM_EQ_INTFLT"_Stz,
  /*    */ "FM_EQ_INTINT"_Stz,
  /*    */ "FM_EQ_IVLIVL"_Stz,
  /*    */ "FM_EQ_TMETME"_Stz,
  /*    */ "FM_EQ_TSPDTE"_Stz,
  /*    */ "FM_EQ_TSPTSP"_Stz,
  /*    */ "FM_EQ_VCHVCH"_Stz,
  /*    */ "FM_EQ_UNKXYZ"_Stz,
  /*    */ "FM_EQ_XYZUNK"_Stz,

  // greater than equal
  /*    */ "FM_GE_BCDBCD"_Stz,
  /*    */ "FM_GE_BCDFLT"_Stz,
  /*    */ "FM_GE_BCDINT"_Stz,
  /*    */ "FM_GE_BOLBOL"_Stz,
  /*    */ "FM_GE_DTEDTE"_Stz,
  /*    */ "FM_GE_DTETSP"_Stz,
  /*    */ "FM_GE_FLTBCD"_Stz,
  /*    */ "FM_GE_FLTFLT"_Stz,
  /*    */ "FM_GE_FLTINT"_Stz,
  /*    */ "FM_GE_INTBCD"_Stz,
  /*    */ "FM_GE_INTFLT"_Stz,
  /*    */ "FM_GE_INTINT"_Stz,
  /*    */ "FM_GE_IVLIVL"_Stz,
  /*    */ "FM_GE_TMETME"_Stz,
  /*    */ "FM_GE_TSPDTE"_Stz,
  /*    */ "FM_GE_TSPTSP"_Stz,
  /*    */ "FM_GE_VCHVCH"_Stz,
  /*    */ "FM_GE_UNKXYZ"_Stz,
  /*    */ "FM_GE_XYZUNK"_Stz,

  // greater than
  /*    */ "FM_GT_BCDBCD"_Stz,
  /*    */ "FM_GT_BCDFLT"_Stz,
  /*    */ "FM_GT_BCDINT"_Stz,
  /*    */ "FM_GT_BOLBOL"_Stz,
  /*    */ "FM_GT_DTEDTE"_Stz,
  /*    */ "FM_GT_DTETSP"_Stz,
  /*    */ "FM_GT_FLTBCD"_Stz,
  /*    */ "FM_GT_FLTFLT"_Stz,
  /*    */ "FM_GT_FLTINT"_Stz,
  /*    */ "FM_GT_INTBCD"_Stz,
  /*    */ "FM_GT_INTFLT"_Stz,
  /*    */ "FM_GT_INTINT"_Stz,
  /*    */ "FM_GT_IVLIVL"_Stz,
  /*    */ "FM_GT_TMETME"_Stz,
  /*    */ "FM_GT_TSPDTE"_Stz,
  /*    */ "FM_GT_TSPTSP"_Stz,
  /*    */ "FM_GT_VCHVCH"_Stz,
  /*    */ "FM_GT_UNKXYZ"_Stz,
  /*    */ "FM_GT_XYZUNK"_Stz,

    // less than equal
  /*    */ "FM_LE_BCDBCD"_Stz,
  /*    */ "FM_LE_BCDFLT"_Stz,
  /*    */ "FM_LE_BCDINT"_Stz,
  /*    */ "FM_LE_BOLBOL"_Stz,
  /*    */ "FM_LE_DTEDTE"_Stz,
  /*    */ "FM_LE_DTETSP"_Stz,
  /*    */ "FM_LE_FLTBCD"_Stz,
  /*    */ "FM_LE_FLTFLT"_Stz,
  /*    */ "FM_LE_FLTINT"_Stz,
  /*    */ "FM_LE_INTBCD"_Stz,
  /*    */ "FM_LE_INTFLT"_Stz,
  /*    */ "FM_LE_INTINT"_Stz,
  /*    */ "FM_LE_IVLIVL"_Stz,
  /*    */ "FM_LE_TMETME"_Stz,
  /*    */ "FM_LE_TSPDTE"_Stz,
  /*    */ "FM_LE_TSPTSP"_Stz,
  /*    */ "FM_LE_VCHVCH"_Stz,
  /*    */ "FM_LE_UNKXYZ"_Stz,
  /*    */ "FM_LE_XYZUNK"_Stz,

    // less than
  /*    */ "FM_LT_BCDBCD"_Stz,
  /*    */ "FM_LT_BCDFLT"_Stz,
  /*    */ "FM_LT_BCDINT"_Stz,
  /*    */ "FM_LT_BOLBOL"_Stz,
  /*    */ "FM_LT_DTEDTE"_Stz,
  /*    */ "FM_LT_DTETSP"_Stz,
  /*    */ "FM_LT_FLTBCD"_Stz,
  /*    */ "FM_LT_FLTFLT"_Stz,
  /*    */ "FM_LT_FLTINT"_Stz,
  /*    */ "FM_LT_INTBCD"_Stz,
  /*    */ "FM_LT_INTFLT"_Stz,
  /*    */ "FM_LT_INTINT"_Stz,
  /*    */ "FM_LT_IVLIVL"_Stz,
  /*    */ "FM_LT_TMETME"_Stz,
  /*    */ "FM_LT_TSPDTE"_Stz,
  /*    */ "FM_LT_TSPTSP"_Stz,
  /*    */ "FM_LT_VCHVCH"_Stz,
  /*    */ "FM_LT_UNKXYZ"_Stz,
  /*    */ "FM_LT_XYZUNK"_Stz,

  // not equal
  /*    */ "FM_NE_BCDBCD"_Stz,
  /*    */ "FM_NE_BCDFLT"_Stz,
  /*    */ "FM_NE_BCDINT"_Stz,
  /*    */ "FM_NE_BOLBOL"_Stz,
  /*    */ "FM_NE_DTEDTE"_Stz,
  /*    */ "FM_NE_DTETSP"_Stz,
  /*    */ "FM_NE_FLTBCD"_Stz,
  /*    */ "FM_NE_FLTFLT"_Stz,
  /*    */ "FM_NE_FLTINT"_Stz,
  /*    */ "FM_NE_INTBCD"_Stz,
  /*    */ "FM_NE_INTFLT"_Stz,
  /*    */ "FM_NE_INTINT"_Stz,
  /*    */ "FM_NE_IVLIVL"_Stz,
  /*    */ "FM_NE_TMETME"_Stz,
  /*    */ "FM_NE_TSPDTE"_Stz,
  /*    */ "FM_NE_TSPTSP"_Stz,
  /*    */ "FM_NE_VCHVCH"_Stz,
  /*    */ "FM_NE_UNKXYZ"_Stz,
  /*    */ "FM_NE_XYZUNK"_Stz,
  
  // string operators
  /*    */ "FM_STR_COMPARE"_Stz,
  /*    */ "FM_STR_LIKE"_Stz,
  /*    */ "FM_STR_SUBSTRING"_Stz,

  //----------------------------------------------------------------------------
  // date operators
  /*    */ "FM_DTE_NOW"_Stz,
  /*    */ "FM_DTE_YEAR"_Stz,

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // conversion/cast operators
  /*  M  */ "FM_BCD_TOFLT"_Stz,
  /*  M  */ "FM_BCD_TOINT"_Stz,

  /*  M  */ "FM_FLT_TOBCD"_Stz,
  /*  M  */ "FM_FLT_TOINT"_Stz,

  /*  M  */ "FM_INT_TOBCD"_Stz,
  /*  M  */ "FM_INT_TOFLT"_Stz,

  /*  M  */ "FM_VCH_TOBCD"_Stz,
  /*  M  */ "FM_VCH_TOFLT"_Stz,
  /*  M  */ "FM_VCH_TOINT"_Stz,

  /*    */ "ENUMS"_Stz
};

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
