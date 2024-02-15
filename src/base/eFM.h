/*============================================================================*/
/* eFM.h -- Funtion macros                                                    */
/*============================================================================*/
#pragma once

enum eFM : u2 // function macros
{
  /*  0 */ FM_NONE=0,

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // arithmetic operators

  // add

  /*  */ FM_ADD_ANYUNK,           // any + unk
  /*  */ FM_ADD_BCDBCD,           // bcd + bcd
  /*  */ FM_ADD_DTEIVD,           // dte + ivd
  /*  */ FM_ADD_DTEIVY,           // dte + ivy
  /*  */ FM_ADD_FLTFLT,           // flt + flt
  /*  */ FM_ADD_INTINT,           // int + int
  /*  */ FM_ADD_IVDDTE,           // ivd + dte
  /*  */ FM_ADD_IVDTME,           // ivd + tme
  /*  */ FM_ADD_IVDTSP,           // ivd + tsp
  /*  */ FM_ADD_IVYDTE,           // ivy + dte
  /*  */ FM_ADD_IVYTSP,           // ivy + tsp
  /*  */ FM_ADD_IVDIVD,           // ivd + ivd
  /*  */ FM_ADD_IVYIVY,           // ivy + ivy
  /*  */ FM_ADD_TMEIVD,           // tme + ivd
  /*  */ FM_ADD_TSPIVD,           // tsp + ivd
  /*  */ FM_ADD_TSPIVY,           // tsp + ivy
  /*  */ FM_ADD_UNKANY,           // unk + any

  // divide
  /*  */ FM_DIV_ANYUNK,           // any / unk
  /*  */ FM_DIV_BCDBCD,           // bcd / bcd
  /*  */ FM_DIV_FLTFLT,           // flt / flt
  /*  */ FM_DIV_INTINT,           // int / int
  /*  */ FM_DIV_IVDINT,           // ivd / int
  /*  */ FM_DIV_IVYINT,           // ivy / int
  /*  */ FM_DIV_UNKANY,           // unk / any

  // modulo
  /*  */ FM_MOD_ANYUNK,          // any % unk
  /*  */ FM_MOD_INTINT,          // int % int
  /*  */ FM_MOD_UNKANY,          // unk % any

  // multiply
  /*  */ FM_MUL_ANYUNK,           // any + unk
  /*  */ FM_MUL_BCDBCD,           // bcd * bcd
  /*  */ FM_MUL_FLTFLT,           // flt * flt
  /*  */ FM_MUL_INTINT,           // int * int
  /*  */ FM_MUL_INTIVD,           // int * ivd
  /*  */ FM_MUL_INTIVY,           // int * ivy
  /*  */ FM_MUL_IVDINT,           // ivd * int
  /*  */ FM_MUL_IVYINT,           // ivy * int
  /*  */ FM_MUL_UNKANY,           // unk + any

  // operation invalid
  /*  */ FM_OPR_INVINV,           // invalid operation of types
  /*  */ FM_OPR_INV,              // invalid operation on type

  // negate
  /*  */ FM_NEG_BCD,             // -bcd
  /*  */ FM_NEG_FLT,             // -flt
  /*  */ FM_NEG_INT,             // -int
  /*  */ FM_NEG_IVD,             // -ivd
  /*  */ FM_NEG_IVY,             // -ivy

  // not supported
  /*  */ FM_OPR_NOTNOT,          // not supported
  /*  */ FM_OPR_NOT,             // not supported

  // subtract
  /*  */ FM_SUB_ANYUNK,           // any + unk
  /*  */ FM_SUB_BCDBCD,           // bcd - bcd
  /*  */ FM_SUB_DTEDTE,           // dte - dte
  /*  */ FM_SUB_DTEIVD,           // dte - ivd
  /*  */ FM_SUB_DTEIVY,           // dte - ivy
  /*  */ FM_SUB_FLTFLT,           // flt - flt
  /*  */ FM_SUB_INTINT,           // int - int
  /*  */ FM_SUB_IVDIVD,           // ivd - ivd
  /*  */ FM_SUB_IVYIVY,           // ivy - ivy
  /*  */ FM_SUB_TMEIVD,           // tme - ivd
  /*  */ FM_SUB_TMETME,           // tme - tme
  /*  */ FM_SUB_TSPIVD,           // tsp - ivd
  /*  */ FM_SUB_TSPIVY,           // tsp - ivy
  /*  */ FM_SUB_TSPTSP,           // tsp - tsp
  /*  */ FM_SUB_UNKANY,           // unk + any

  // sum
  /*  */ FM_SUM_ANYUNK,           // sum(any,unk)
  /*  */ FM_SUM_BCDBCD,           // sum(bcd,bcd)
  /*  */ FM_SUM_FLTFLT,           // sum(flt,flt)
  /*  */ FM_SUM_INTINT,           // sum(int,int)
  /*  */ FM_SUM_UNKANY,           // sum(unk,any)

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // between operators

  /*  */ FM_BET_ANYUNK,           // between any unk
  /*  */ FM_BET_BCDBCD,           // between bcd bcd
  /*  */ FM_BET_DTEDTE,           // between dte dte
  /*  */ FM_BET_FLTFLT,           // between flt flt
  /*  */ FM_BET_INTINT,           // between int int
  /*  */ FM_BET_IVDIVD,           // between ivd ivd
  /*  */ FM_BET_IVYIVY,           // between ivy ivy
  /*  */ FM_BET_TMETME,           // between bcd bcd
  /*  */ FM_BET_TSPTSP,           // between tme tme
  /*  */ FM_BET_UNKANY,           // between unk any
  /*  */ FM_BET_VCHVCH,           // between vch vch

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast operators

  /*  */ FM_ANY_TOUNK,          // any to unk
  /*  */ FM_BCD_TOBCD,          // bcd to bcd
  /*  */ FM_BCD_TOFLT,          // bcd to flt
  /*  */ FM_BCD_TOINT,          // bcd to int
  /*  */ FM_BCD_TOUNK,          // bcd to unk
  /*  */ FM_BCD_TOVCH,          // bcd to vch

  /*  */ FM_BOL_TOUNK,          // bol to unk
  /*  */ FM_BOL_TOVCH,          // bol to vch

  /*  */ FM_DTE_TOTSP,          // dte to tsp
  /*  */ FM_DTE_TOUNK,          // dte to unk
  /*  */ FM_DTE_TOVCH,          // dte to vch

  /*  */ FM_FLT_TOBCD,          // flt to bcd
  /*  */ FM_FLT_TOINT,          // flt to int
  /*  */ FM_FLT_TOUNK,          // flt to unk
  /*  */ FM_FLT_TOVCH,          // flt to vch

  /*  */ FM_INT_TOBCD,          // int to bcd
	/*  */ FM_INT_TODAY,          // int to day(ivd)
  /*  */ FM_INT_TOFLT,          // int to flt
	/*  */ FM_INT_TOHOUR,         // int to hour(ivd)
	/*  */ FM_INT_TOMINUTE,       // int to minutr(ivd)
	/*  */ FM_INT_TOMONTH,        // int to month(ivl)
	/*  */ FM_INT_TOSECOND,       // int to second(ivd)
  /*  */ FM_INT_TOUNK,          // int to unk
  /*  */ FM_INT_TOVCH,          // int to vch
	/*  */ FM_INT_TOYEAR,         // int to year(ivl)

  /*  */ FM_IVD_TOUNK,          // ivd to unk
  /*  */ FM_IVD_TOVCH,          // ivd to vch

  /*  */ FM_IVY_TOUNK,          // ivy to unk
  /*  */ FM_IVY_TOVCH,          // ivy to vch

  /*  */ FM_TME_TOTSP,          // tme to tsp
  /*  */ FM_TME_TOUNK,          // tme to unk
  /*  */ FM_TME_TOVCH,          // tme to vch

  /*  */ FM_TSP_TOUNK,          // tsp to unk
  /*  */ FM_TSP_TODTE,          // tsp to dte
  /*  */ FM_TSP_TOTME,          // tsp to tme
  /*  */ FM_TSP_TOVCH,          // tsp to vch

  /*  */ FM_UNK_TOANY,          // unk to any

  /*  */ FM_VCH_TOBCD,          // vch to bcd
  /*  */ FM_VCH_TODTE,          // vch to dte
  /*  */ FM_VCH_TOFLT,          // vch to flt
  /*  */ FM_VCH_TOINT,          // vch to int
  /*  */ FM_VCH_TOIVD,          // vch to ivd
  /*  */ FM_VCH_TOIVY,          // vch to ivy
  /*  */ FM_VCH_TOTME,          // vch to tme
  /*  */ FM_VCH_TOTSP,          // vch to tsp
  /*  */ FM_VCH_TOVCH,          // vch to vch
  /*  */ FM_VCH_TOUNK,          // vch to unk

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // duplicate operators

  /*  */ FM_DUP_BOL,             // dup bol
  /*  */ FM_DUP_BCD,             // dup bcd
  /*  */ FM_DUP_DTE,             // dup dte
  /*  */ FM_DUP_FLT,             // dup flt
  /*  */ FM_DUP_INT,             // dup int
  /*  */ FM_DUP_IVD,             // dup ivd
  /*  */ FM_DUP_IVY,             // dup ivy
  /*  */ FM_DUP_TME,             // dup tme
  /*  */ FM_DUP_TSP,             // dup tsp
  /*  */ FM_DUP_UNK,             // dup unk
  /*  */ FM_DUP_VCH,             // dup vch

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract operators

  /*  */ FM_DAY_DTE,             // day from dte
  /*  */ FM_DAY_TSP,             // day from tsp
  /*  */ FM_HOUR_TME,            // hour from tme
  /*  */ FM_HOUR_TSP,            // hour from tsp
  /*  */ FM_MINUTE_TME,          // min from tme
  /*  */ FM_MINUTE_TSP,          // min from tsp
  /*  */ FM_MONTH_DTE,           // month from dte
  /*  */ FM_MONTH_TSP,           // month from tsp
  /*  */ FM_SECOND_TME,          // sec from tme
  /*  */ FM_SECOND_TSP,          // sec from tsp
  /*  */ FM_YEAR_DTE,            // year from year
  /*  */ FM_YEAR_TSP,            // year from tsp

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // in expression operators

  /*  */ FM_INX_ANYUNK,           // any in unk,...
  /*  */ FM_INX_BCDBCD,           // bcd in bcd,...
  /*  */ FM_INX_BOLBOL,           // bol in bol,...
  /*  */ FM_INX_DTEDTE,           // dte in dte,...
  /*  */ FM_INX_FLTFLT,           // flt in flt,...
  /*  */ FM_INX_INTINT,           // int in int,...
  /*  */ FM_INX_IVDIVD,           // ivd in ivd,...
  /*  */ FM_INX_IVYIVY,           // ivy in ivy,...
  /*  */ FM_INX_TMETME,           // tme in tme,...
  /*  */ FM_INX_TSPTSP,           // tsp in tsp,...
  /*  */ FM_INX_VCHVCH,           // vch in vch,...
  /*  */ FM_INX_UNKANY,           // unk in any,...

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

  // push
  /*  */ FM_PSH_BOL,             // push bol
  /*  */ FM_PSH_BCD,             // push bcd
  /*  */ FM_PSH_DTE,             // push dte
  /*  */ FM_PSH_FLT,             // push flt
  /*  */ FM_PSH_INT,             // push int
  /*  */ FM_PSH_IVD,             // push ivd
  /*  */ FM_PSH_IVY,             // push ivy
  /*  */ FM_PSH_TME,             // push tme
  /*  */ FM_PSH_TSP,             // push tsp
  /*  */ FM_PSH_UNK,             // push unk
  /*  */ FM_PSH_VCH,             // push vch

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // relational operators

  // equal
  /*  */ FM_EQ_ANYUNK,        // any == unk
  /*  */ FM_EQ_BCDBCD,        // bcd == bcd
  /*  */ FM_EQ_BOLBOL,        // bol == bol
  /*  */ FM_EQ_DTEDTE,        // dte == bcd
  /*  */ FM_EQ_FLTFLT,        // flt == flt
  /*  */ FM_EQ_INTINT,        // int == int
  /*  */ FM_EQ_IVDIVD,        // ivd == ivd
  /*  */ FM_EQ_IVYIVY,        // ivy == ivy
  /*  */ FM_EQ_TMETME,        // tme == tme
  /*  */ FM_EQ_TSPTSP,        // tsp == tsp
  /*  */ FM_EQ_UNKANY,        // unk == any
  /*  */ FM_EQ_VCHVCH,        // vch == vch

  // greater than equal
  /*  */ FM_GE_ANYUNK,        // any >= unk
  /*  */ FM_GE_BCDBCD,        // bcd >= bcd
  /*  */ FM_GE_BOLBOL,        // bol >= bol
  /*  */ FM_GE_DTEDTE,        // dte >= bcd
  /*  */ FM_GE_FLTFLT,        // flt >= flt
  /*  */ FM_GE_INTINT,        // int >= int
  /*  */ FM_GE_IVDIVD,        // ivd >= ivd
  /*  */ FM_GE_IVYIVY,        // ivy == ivy
  /*  */ FM_GE_TMETME,        // tme >= tme
  /*  */ FM_GE_TSPTSP,        // tsp >= tsp
  /*  */ FM_GE_UNKANY,        // unk >= any
  /*  */ FM_GE_VCHVCH,        // vch >= vch

  // greater than
  /*  */ FM_GT_ANYUNK,        // any > unk
  /*  */ FM_GT_BCDBCD,        // bcd > bcd
  /*  */ FM_GT_BOLBOL,        // bol > bol
  /*  */ FM_GT_DTEDTE,        // dte > bcd
  /*  */ FM_GT_FLTFLT,        // flt > flt
  /*  */ FM_GT_INTINT,        // int > int
  /*  */ FM_GT_IVDIVD,        // ivd > ivd
  /*  */ FM_GT_IVYIVY,        // ivy > ivy
  /*  */ FM_GT_TMETME,        // tme > tme
  /*  */ FM_GT_TSPTSP,        // tsp > tsp
  /*  */ FM_GT_UNKANY,        // unk > any
  /*  */ FM_GT_VCHVCH,        // vch > vch

  // greater than for aggreagtes
  /*  */ FM_GTA_ANYUNK,        // any > unk
  /*  */ FM_GTA_BCDBCD,        // bcd > bcd
  /*  */ FM_GTA_BOLBOL,        // bol > bol
  /*  */ FM_GTA_DTEDTE,        // dte > bcd
  /*  */ FM_GTA_FLTFLT,        // flt > flt
  /*  */ FM_GTA_INTINT,        // int > int
  /*  */ FM_GTA_IVDIVD,        // ivd > ivd
  /*  */ FM_GTA_IVYIVY,        // ivy > ivy
  /*  */ FM_GTA_TMETME,        // tme > tme
  /*  */ FM_GTA_TSPTSP,        // tsp > tsp
  /*  */ FM_GTA_UNKANY,        // unk > any
  /*  */ FM_GTA_VCHVCH,        // vch > vch

  // less than equal
  /*  */ FM_LE_ANYUNK,        // any <= unk
  /*  */ FM_LE_BCDBCD,        // bcd <= bcd
  /*  */ FM_LE_BOLBOL,        // bol <= bol
  /*  */ FM_LE_DTEDTE,        // dte <= bcd
  /*  */ FM_LE_FLTFLT,        // flt <= flt
  /*  */ FM_LE_INTINT,        // int <= int
  /*  */ FM_LE_IVDIVD,        // ivd <= ivd
  /*  */ FM_LE_IVYIVY,        // ivy <= ivy
  /*  */ FM_LE_TMETME,        // tme <= tme
  /*  */ FM_LE_TSPTSP,        // tsp <= tsp
  /*  */ FM_LE_UNKANY,        // unk <= any
  /*  */ FM_LE_VCHVCH,        // vch <= vch

  // less than
  /*  */ FM_LT_ANYUNK,        // any < unk
  /*  */ FM_LT_BCDBCD,        // bcd < bcd
  /*  */ FM_LT_BOLBOL,        // bol < bol
  /*  */ FM_LT_DTEDTE,        // dte < bcd
  /*  */ FM_LT_FLTFLT,        // flt < flt
  /*  */ FM_LT_INTINT,        // int < int
  /*  */ FM_LT_IVDIVD,        // ivd < ivd
  /*  */ FM_LT_IVYIVY,        // ivy < ivy
  /*  */ FM_LT_TMETME,        // tme < tme
  /*  */ FM_LT_TSPTSP,        // tsp < tsp
  /*  */ FM_LT_UNKANY,        // unk < any
  /*  */ FM_LT_VCHVCH,        // vch < vch

  // less than for aggregates
  /*  */ FM_LTA_ANYUNK,        // any < unk
  /*  */ FM_LTA_BCDBCD,        // bcd < bcd
  /*  */ FM_LTA_BOLBOL,        // bol < bol
  /*  */ FM_LTA_DTEDTE,        // dte < bcd
  /*  */ FM_LTA_FLTFLT,        // flt < flt
  /*  */ FM_LTA_INTINT,        // int < int
  /*  */ FM_LTA_IVDIVD,        // ivd < ivd
  /*  */ FM_LTA_IVYIVY,        // ivy < ivy
  /*  */ FM_LTA_TMETME,        // tme < tme
  /*  */ FM_LTA_TSPTSP,        // tsp < tsp
  /*  */ FM_LTA_UNKANY,        // unk < any
  /*  */ FM_LTA_VCHVCH,        // vch < vch

  // not equal
  /*  */ FM_NE_ANYUNK,        // any != unk
  /*  */ FM_NE_BCDBCD,        // bcd != bcd
  /*  */ FM_NE_BOLBOL,        // bol != bol
  /*  */ FM_NE_DTEDTE,        // dte != bcd
  /*  */ FM_NE_FLTFLT,        // flt != flt
  /*  */ FM_NE_INTINT,        // int != int
  /*  */ FM_NE_IVDIVD,        // ivd != ivd
  /*  */ FM_NE_IVYIVY,        // ivy != ivy
  /*  */ FM_NE_TMETME,        // tme != tme
  /*  */ FM_NE_TSPTSP,        // tsp != tsp
  /*  */ FM_NE_UNKANY,        // unk != any
  /*  */ FM_NE_VCHVCH,        // vch != vch

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // string operators
  /*  */ FM_STR_COMPARE,      // string compare
  /*  */ FM_STR_LIKE,         // string like
  /*  */ FM_STR_SUBSTRING2,   // string substring
  /*  */ FM_STR_SUBSTRING3,   // string substring

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // date operators
  /*  M  */ FM_DTE_NOW,          // get current date
  /*  M  */ FM_DTE_YEAR,         // get year of date

  /*  */ FM_ENUMS
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

extern Stz* aFM[FM_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
