/*============================================================================*/
/* eEO.h -- Expression Operators                                              */
/*============================================================================*/
#pragma once

#include "eEA.h"

#ifdef __TINYC__

// when being included by the tcc compiler
enum eEO // expression operators

#else

// when not being included by the tcc compiler
enum eEO : u2 // expression operators

#endif
{
  /*  0 */ EO_NONE=0,
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // simple eo

  // arithmetic operators

  // add top two numbers and push result
  /*  S  */ EO_ADD_BCDBCD,           // bcd + bcd
  /*  S  */ EO_ADD_BCDFLT,           // bcd + int
  /*  S  */ EO_ADD_BCDINT,           // bcd + int
  /*  S  */ EO_ADD_DTEIVL,           // dte + ivl
  /*  S  */ EO_ADD_FLTBCD,           // flt + bcd
  /*  S  */ EO_ADD_FLTINT,           // flt + int
  /*  S  */ EO_ADD_FLTFLT,           // flt + flt
  /*  S  */ EO_ADD_INTBCD,           // int + bcd
  /*  S  */ EO_ADD_INTFLT,           // int + flt
  /*  S  */ EO_ADD_INTINT,           // int + int
  /*  S  */ EO_ADD_IVLDTE,           // ivl + dte
  /*  S  */ EO_ADD_IVLIVL,           // ivl + ivl
  /*  S  */ EO_ADD_IVLTME,           // ivl + tme
  /*  S  */ EO_ADD_IVLTSP,           // ivl + tsp
  /*  S  */ EO_ADD_TMEIVL,           // tme + ivl
  /*  S  */ EO_ADD_TSPIVL,           // tsp + ivl
  /*  S  */ EO_ADD_UNKXYZ,           // unk + xyz
  /*  S  */ EO_ADD_XYZUNK,           // xyz + unk

  /*  S  */ EO_DIV_BCDBCD,           // bcd / bcd
  /*  S  */ EO_DIV_BCDFLT,           // bcd / int
  /*  S  */ EO_DIV_BCDINT,           // bcd / int
  /*  S  */ EO_DIV_DTEIVL,           // dte / ivl
  /*  S  */ EO_DIV_FLTBCD,           // flt / bcd
  /*  S  */ EO_DIV_FLTINT,           // flt / int
  /*  S  */ EO_DIV_FLTFLT,           // flt / flt
  /*  S  */ EO_DIV_INTBCD,           // int / bcd
  /*  S  */ EO_DIV_INTFLT,           // int / flt
  /*  S  */ EO_DIV_INTINT,           // int / int
  /*  S  */ EO_DIV_IVLDTE,           // ivl / dte
  /*  S  */ EO_DIV_IVLIVL,           // ivl / ivl
  /*  S  */ EO_DIV_IVLTME,           // ivl / tme
  /*  S  */ EO_DIV_IVLTSP,           // ivl / tsp
  /*  S  */ EO_DIV_TMEIVL,           // tme / ivl
  /*  S  */ EO_DIV_TSPIVL,           // tsp / ivl
  /*  S  */ EO_DIV_UNKXYZ,           // unk / xyz
  /*  S  */ EO_DIV_XYZUNK,           // xyz / unk
  
  // modulo
  /*  S  */ EO_MOD_INTINT,          // int % int
  /*  S  */ EO_MOD_UNKXYZ,          // unk % xyz
  /*  S  */ EO_MOD_XYZUNK,          // xyz % unk

  // multiply
  /*  S  */ EO_MUL_BCDBCD,           // bcd * bcd
  /*  S  */ EO_MUL_BCDFLT,           // bcd * int
  /*  S  */ EO_MUL_BCDINT,           // bcd * int
  /*  S  */ EO_MUL_DTEIVL,           // dte * ivl
  /*  S  */ EO_MUL_FLTBCD,           // flt * bcd
  /*  S  */ EO_MUL_FLTINT,           // flt * int
  /*  S  */ EO_MUL_FLTFLT,           // flt * flt
  /*  S  */ EO_MUL_INTBCD,           // int * bcd
  /*  S  */ EO_MUL_INTFLT,           // int * flt
  /*  S  */ EO_MUL_INTINT,           // int * int
  /*  S  */ EO_MUL_IVLDTE,           // ivl * dte
  /*  S  */ EO_MUL_IVLIVL,           // ivl * ivl
  /*  S  */ EO_MUL_IVLTME,           // ivl * tme
  /*  S  */ EO_MUL_IVLTSP,           // ivl * tsp
  /*  S  */ EO_MUL_TMEIVL,           // tme * ivl
  /*  S  */ EO_MUL_TSPIVL,           // tsp * ivl
  /*  S  */ EO_MUL_UNKXYZ,           // unk * xyz
  /*  S  */ EO_MUL_XYZUNK,           // xyz * unk

  // negate
  /*  S  */ EO_NEG_BCD,             // -bcd
  /*  S  */ EO_NEG_FLT,             // -flt
  /*  S  */ EO_NEG_INT,             // -int
  /*  S  */ EO_NEG_IVL,             // -ivl

  // subtract
  /*  S  */ EO_SUB_BCDBCD,           // bcd * bcd
  /*  S  */ EO_SUB_BCDFLT,           // bcd * int
  /*  S  */ EO_SUB_BCDINT,           // bcd * int
  /*  S  */ EO_SUB_DTEIVL,           // dte * ivl
  /*  S  */ EO_SUB_FLTBCD,           // flt * bcd
  /*  S  */ EO_SUB_FLTINT,           // flt * int
  /*  S  */ EO_SUB_FLTFLT,           // flt * flt
  /*  S  */ EO_SUB_INTBCD,           // int * bcd
  /*  S  */ EO_SUB_INTFLT,           // int * flt
  /*  S  */ EO_SUB_INTINT,           // int * int
  /*  S  */ EO_SUB_IVLDTE,           // ivl * dte
  /*  S  */ EO_SUB_IVLIVL,           // ivl * ivl
  /*  S  */ EO_SUB_IVLTME,           // ivl * tme
  /*  S  */ EO_SUB_IVLTSP,           // ivl * tsp
  /*  S  */ EO_SUB_TMEIVL,           // tme * ivl
  /*  S  */ EO_SUB_TSPIVL,           // tsp * ivl
  /*  S  */ EO_SUB_UNKXYZ,           // unk * xyz
  /*  S  */ EO_SUB_XYZUNK,           // xyz * unk

  // counter
  /*  R  */ EO_CTR_0_DEC,        // dec counter 0
  /*  R  */ EO_CTR_0_INC,        // inc counter 0
  /*  R  */ EO_CTR_0_POP,        // pop counter 0
  /*  R  */ EO_CTR_0_PUSH,       // push counter 0

  // jump operators
  /*  S  */ EO_JMP,              // jump to location
  /*  S  */ EO_JMP_FALSE,        // pop top and jump to location if false
  /*  S  */ EO_JMP_TRUE,         // pop top and jump to location if true
  
  /*  S  */ EO_JMP_LESS,         // pop top and jump to location if <
  /*  S  */ EO_JMP_LESSEQUAL,    // pop top and jump to location if <=
  /*  S  */ EO_JMP_EQUAL,        // pop top and jump to location if ==
  /*  S  */ EO_JMP_NOTEQUAL,     // pop top and jump to location if !=
  /*  S  */ EO_JMP_GREATEREQUAL, // pop top and jump to location if >=
  /*  S  */ EO_JMP_GREATER,      // pop top and jump to location if >

  // logical operators
  /*  S  */ EO_LGL_AND,          // logical and top two int's, push result
  /*  S  */ EO_LGL_OR,           // logical or top two int's, push result
  /*  S  */ EO_LGL_NOT,          // logical not top int, push result
  /*  S  */ EO_LGL_XOR,          // logical xor top two int's, push result

  // null test operators
  /*  S  */ EO_ISNULL,           // test if top is null, push result
  /*  S  */ EO_ISNULL_STR,       // test if top string is null, push result
  /*  S  */ EO_ISNOTNULL,        // test if top is not null, push result

  // prints
  /*  S  */ EO_PRINT,            // print top
  /*  S  */ EO_PRINTF,           // formatted print

  // pushes
  /*  S  */ EO_DATE,             // push date
  /*  S  */ EO_INTEGER,          // push integer
  /*  S  */ EO_PUSHNULL,         // push null
  /*  S  */ EO_PUSH_MINUS1,      // push integer -1
  /*  S  */ EO_PUSH_0,           // push integer 0
  /*  S  */ EO_PUSH_1,           // push integer 1
  /*  S  */ EO_RANDOM,           // push random number 0..arg-1
  /*  S  */ EO_STRING,           // push string
  /*  S  */ EO_TIME,             // push time
  /*  S  */ EO_TIMESTAMP,        // push timestamp

  // relational operators
  /*  S  */ EO_EQ_BCDBCD,        // bcd == bcd
  /*  S  */ EO_EQ_BCDFLT,        // bcd == flt
  /*  S  */ EO_EQ_BCDINT,        // bcd == int
  /*  S  */ EO_EQ_BOLBOL,        // bol == bol
  /*  S  */ EO_EQ_DTEDTE,        // dte == bcd
  /*  S  */ EO_EQ_DTETSP,        // dte == tsp
  /*  S  */ EO_EQ_FLTBCD,        // flt == bcd
  /*  S  */ EO_EQ_FLTFLT,        // flt == flt
  /*  S  */ EO_EQ_FLTINT,        // flt == int
  /*  S  */ EO_EQ_INTBCD,        // int == bcd
  /*  S  */ EO_EQ_INTFLT,        // int == flt
  /*  S  */ EO_EQ_INTINT,        // int == int
  /*  S  */ EO_EQ_IVLIVL,        // ivl == ivl
  /*  S  */ EO_EQ_TMETME,        // tme == tme
  /*  S  */ EO_EQ_TSPDTE,        // tsp == dte
  /*  S  */ EO_EQ_TSPTSP,        // tsp == tsp
  /*  S  */ EO_EQ_VCHVCH,        // vch == vch
  /*  S  */ EO_EQ_UNKXYZ,        // unk == xyz
  /*  S  */ EO_EQ_XYZUNK,        // xyz == unk

  /*  S  */ EO_GE_BCDBCD,        // bcd >= bcd
  /*  S  */ EO_GE_BCDFLT,        // bcd >= flt
  /*  S  */ EO_GE_BCDINT,        // bcd >= int
  /*  S  */ EO_GE_BOLBOL,        // bol >= bol
  /*  S  */ EO_GE_DTEDTE,        // dte >= bcd
  /*  S  */ EO_GE_DTETSP,        // dte >= tsp
  /*  S  */ EO_GE_FLTBCD,        // flt >= bcd
  /*  S  */ EO_GE_FLTFLT,        // flt >= flt
  /*  S  */ EO_GE_FLTINT,        // flt >= int
  /*  S  */ EO_GE_INTBCD,        // int >= bcd
  /*  S  */ EO_GE_INTFLT,        // int >= flt
  /*  S  */ EO_GE_INTINT,        // int >= int
  /*  S  */ EO_GE_IVLIVL,        // ivl >= ivl
  /*  S  */ EO_GE_TMETME,        // tme >= tme
  /*  S  */ EO_GE_TSPDTE,        // tsp >= dte
  /*  S  */ EO_GE_TSPTSP,        // tsp >= tsp
  /*  S  */ EO_GE_VCHVCH,        // vch >= vch
  /*  S  */ EO_GE_UNKXYZ,        // unk >= xyz
  /*  S  */ EO_GE_XYZUNK,        // xyz >= unk

  /*  S  */ EO_GT_BCDBCD,        // bcd > bcd
  /*  S  */ EO_GT_BCDFLT,        // bcd > flt
  /*  S  */ EO_GT_BCDINT,        // bcd > int
  /*  S  */ EO_GT_BOLBOL,        // bol > bol
  /*  S  */ EO_GT_DTEDTE,        // dte > bcd
  /*  S  */ EO_GT_DTETSP,        // dte > tsp
  /*  S  */ EO_GT_FLTBCD,        // flt > bcd
  /*  S  */ EO_GT_FLTFLT,        // flt > flt
  /*  S  */ EO_GT_FLTINT,        // flt > int
  /*  S  */ EO_GT_INTBCD,        // int > bcd
  /*  S  */ EO_GT_INTFLT,        // int > flt
  /*  S  */ EO_GT_INTINT,        // int > int
  /*  S  */ EO_GT_IVLIVL,        // ivl > ivl
  /*  S  */ EO_GT_TMETME,        // tme > tme
  /*  S  */ EO_GT_TSPDTE,        // tsp > dte
  /*  S  */ EO_GT_TSPTSP,        // tsp > tsp
  /*  S  */ EO_GT_VCHVCH,        // vch > vch
  /*  S  */ EO_GT_UNKXYZ,        // unk > xyz
  /*  S  */ EO_GT_XYZUNK,        // xyz > unk

  /*  S  */ EO_LE_BCDBCD,        // bcd <= bcd
  /*  S  */ EO_LE_BCDFLT,        // bcd <= flt
  /*  S  */ EO_LE_BCDINT,        // bcd <= int
  /*  S  */ EO_LE_BOLBOL,        // bol <= bol
  /*  S  */ EO_LE_DTEDTE,        // dte <= bcd
  /*  S  */ EO_LE_DTETSP,        // dte <= tsp
  /*  S  */ EO_LE_FLTBCD,        // flt <= bcd
  /*  S  */ EO_LE_FLTFLT,        // flt <= flt
  /*  S  */ EO_LE_FLTINT,        // flt <= int
  /*  S  */ EO_LE_INTBCD,        // int <= bcd
  /*  S  */ EO_LE_INTFLT,        // int <= flt
  /*  S  */ EO_LE_INTINT,        // int <= int
  /*  S  */ EO_LE_IVLIVL,        // ivl <= ivl
  /*  S  */ EO_LE_TMETME,        // tme <= tme
  /*  S  */ EO_LE_TSPDTE,        // tsp <= dte
  /*  S  */ EO_LE_TSPTSP,        // tsp <= tsp
  /*  S  */ EO_LE_VCHVCH,        // vch <= vch
  /*  S  */ EO_LE_UNKXYZ,        // unk <= xyz
  /*  S  */ EO_LE_XYZUNK,        // xyz <= unk

  /*  S  */ EO_LT_BCDBCD,        // bcd < bcd
  /*  S  */ EO_LT_BCDFLT,        // bcd < flt
  /*  S  */ EO_LT_BCDINT,        // bcd < int
  /*  S  */ EO_LT_BOLBOL,        // bol < bol
  /*  S  */ EO_LT_DTEDTE,        // dte < bcd
  /*  S  */ EO_LT_DTETSP,        // dte < tsp
  /*  S  */ EO_LT_FLTBCD,        // flt < bcd
  /*  S  */ EO_LT_FLTFLT,        // flt < flt
  /*  S  */ EO_LT_FLTINT,        // flt < int
  /*  S  */ EO_LT_INTBCD,        // int < bcd
  /*  S  */ EO_LT_INTFLT,        // int < flt
  /*  S  */ EO_LT_INTINT,        // int < int
  /*  S  */ EO_LT_IVLIVL,        // ivl < ivl
  /*  S  */ EO_LT_TMETME,        // tme < tme
  /*  S  */ EO_LT_TSPDTE,        // tsp < dte
  /*  S  */ EO_LT_TSPTSP,        // tsp < tsp
  /*  S  */ EO_LT_VCHVCH,        // vch < vch
  /*  S  */ EO_LT_UNKXYZ,        // unk < xyz
  /*  S  */ EO_LT_XYZUNK,        // xyz < unk

  /*  S  */ EO_NE_BCDBCD,        // bcd != bcd
  /*  S  */ EO_NE_BCDFLT,        // bcd != flt
  /*  S  */ EO_NE_BCDINT,        // bcd != int
  /*  S  */ EO_NE_BOLBOL,        // bol != bol
  /*  S  */ EO_NE_DTEDTE,        // dte != bcd
  /*  S  */ EO_NE_DTETSP,        // dte != tsp
  /*  S  */ EO_NE_FLTBCD,        // flt != bcd
  /*  S  */ EO_NE_FLTFLT,        // flt != flt
  /*  S  */ EO_NE_FLTINT,        // flt != int
  /*  S  */ EO_NE_INTBCD,        // int != bcd
  /*  S  */ EO_NE_INTFLT,        // int != flt
  /*  S  */ EO_NE_INTINT,        // int != int
  /*  S  */ EO_NE_IVLIVL,        // ivl != ivl
  /*  S  */ EO_NE_TMETME,        // tme != tme
  /*  S  */ EO_NE_TSPDTE,        // tsp != dte
  /*  S  */ EO_NE_TSPTSP,        // tsp != tsp
  /*  S  */ EO_NE_VCHVCH,        // vch != vch
  /*  S  */ EO_NE_UNKXYZ,        // unk != xyz
  /*  S  */ EO_NE_XYZUNK,        // xyz != unk
  
  // column pushes
  /*  S  */ EO_PUSH_COLUMN_LENGTH, // push column length
  /*  S  */ EO_PUSH_COLUMN_SCALE,  // push column scale
  /*  S  */ EO_PUSH_COLUMN_TYPE,   // push column type

  // ixt device pushes
  /*  S  */ EO_PUSH_DEVIXT_ALT,     // push alt Ixt device
  /*  S  */ EO_PUSH_DEVIXT_ENT,     // push entry of ixt device
  /*  S  */ EO_PUSH_DEVIXT_LFT,     // push lft Ixt device
  /*  S  */ EO_PUSH_DEVIXT_RHT,     // push rht Ixt device
  /*  S  */ EO_PUSH_DEVIXT_TMP,     // push tmp Ixt device

  // row device pushes
  /*  S  */ EO_PUSH_DEVROW_ALT,     // push alt row device
  /*  S  */ EO_PUSH_DEVROW_DST,     // push dst row device
  /*  S  */ EO_PUSH_DEVROW_LFT,     // push lft row device
  /*  S  */ EO_PUSH_DEVROW_RHT,     // push rht row device

  // rlu pushes
  /*  S  */ EO_PUSH_RLU_COLSET,     // push colset from the rlu
  /*  S  */ EO_PUSH_RLU_ENT,        // push ent from the rlu
  /*  S  */ EO_PUSH_RLU_ENTWRK,     // push entwrk from the rlu
  /*  S  */ EO_PUSH_RLU_INDEXDEF,   // push indexdef from the rlu
  /*  S  */ EO_PUSH_RLU_INDEXTREE,  // push indextree from the rlu
  /*  S  */ EO_PUSH_RLU_JOINEDROW,  // push joinedRow from rlu
  /*  S  */ EO_PUSH_RLU_JOINMAP_BYTE, // push joinmap byte from rlu
  /*  S  */ EO_PUSH_RLU_LOCALS,     // push locals pointer from the rlu
  /*  S  */ EO_PUSH_RLU_POT,        // push pot from the rlu
  /*  S  */ EO_PUSH_RLU_PTN,        // push partition from the rlu
  /*  S  */ EO_PUSH_RLU_ROWPIX,     // push rowpix from the rlu

  // table ptn
  /*  M  */ EO_TABLEPTN_DROPREAD,  // drop a tablePtn read lock
  /*  M  */ EO_TABLEPTN_GRABREAD,  // grab a tablePtn read lock

  // stack operators
  /*  S  */ EO_INC,              // increment top, push result
  /*  S  */ EO_DEC,              // decrement top, push result
  /*  S  */ EO_DUP,              // duplicate top
  /*  S  */ EO_DUP_STR,          // duplicate top
  /*  S  */ EO_POP,              // pop / discard top
  /*  S  */ EO_POP_LOCALI,       // pop integer into a local register
  /*  S  */ EO_POP_LOCALS,       // pop string into a local register
  /*  S  */ EO_POP_STR,          // pop str / discard top str
  /*  S  */ EO_PUSH_LOCALI,      // push an integer from a local register
  /*  S  */ EO_PUSH_LOCALS,      // push an string from a local register
  
  // string operators
  /*  S  */ EO_STR_COMPARE,      // pop top stradr and compare str on stack
  /*  S  */ EO_STR_COMPARE_a,    // pop top two azStr and compare
  /*  S  */ EO_STR_PRINTADR,     // pop top and print iArg as addr of string
  /*  S  */ EO_STR_PRINTSTK,     // pop string from top and print
  /*  S  */ EO_STR_LITERAL,      // push address of string literal

  // subroutine operators
  /*  S  */ EO_CALL,             // call subroutine at location
  /*  S  */ EO_RETURN,           // return from subroutine
  /*  S  */ EO_RETURN_FALSE,     // return from subroutine on false
  /*  S  */ EO_RETURN_TRUE,      // return from subroutine on true
  /*  S  */ EO_RETURN_VALUE,     // return a value from subroutine

  // misc
  /*  S  */ EO_ASSERT,           // pop top and assert
  /*  P  */ EO_BREAKPOINT,       // break point
  /*  R  */ EO_CALLAGAIN,        // set call again
  /*  R  */ EO_CALLEDAGAIN,      // push called again
  /*  R  */ EO_DUMP,             // stack dump
  /*  R  */ EO_ENTER_LU,         // diarise routine enter
  /*  R  */ EO_EXIT_LU,          // diarise routine exit
  /*  M  */ EO_HANDLER,          // set handler
  /*  R  */ EO_INIT,             // initialise the rlu common
  /*  R  */ EO_INTEGRITY,        // integrity check
  /*  M  */ EO_NEVERREACH,       // assert (never reach code)
  /*  S  */ EO_SANITY,           // sanity check
  /*  M  */ EO_SLEEP,            // sleep for top seconds
  /*  R  */ EO_SQL,              // set sql no
  /*  R  */ EO_TABLEPTN,         // push i_TablePtn
  /*  R  */ EO_TRACEOFF,         // set trace off
  /*  R  */ EO_TRACEON,          // set trace on
  /*  S  */ EO_NOOP,             // no op
  /*  M  */ EO_ERROR,            // error marker

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // date operators
  /*  M  */ EO_DTE_NOW,          // push current date
  /*  R  */ EO_DTE_PRINT,        // pop top and print as date
  /*  M  */ EO_DTE_YEAR,         // pop top, push year of date

  // hashing
  /*  R  */ EO_HSH_ADD_HASHI,    // add hash of top int to running hash
  /*  R  */ EO_HSH_RESET,        // reset hash

  // partitioning
  /*  R  */ EO_PTN_CALC,         // calculate i_Ptn using running iHash
  /*  R  */ EO_PTN_PUSH,         // push i_Ptn
  /*  R  */ EO_PTN_RESET,        // reset running iHash
  /*  R  */ EO_PTN_RUNNING,      // mix top into running iHash

  // timer
  /*  M  */ EO_TMR_SINCE,        // get nsecs since ::constructor
  /*  M  */ EO_TMR_START,        // timer start
  /*  M  */ EO_TMR_NSECS,        // push timer nanoseconds
  /*  M  */ EO_TMR_UPDATE,       // timer update
  /*  M  */ EO_TMR_AGG_ADD,      // add timer to agg timer
  /*  M  */ EO_TMR_AGG_NSECS,    // push agg nanoseconds
  /*  M  */ EO_TMR_AGG_RESET,    // reset agg timer

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // rlu column set
  /*  M  */ EO_RLU_SET_COLUMN,       // add column to set
  /*  M  */ EO_RLU_SET_COLUMNS,      // add columns to set
  /*  M  */ EO_RLU_SET_ITEMS,        // set number of column items

  // rlu index join
  /*  M  */ EO_RLU_RESET_JOINEDROW,     // reset JoinedRow in RLU
  /*  M  */ EO_RLU_SET_JOINEDROW,       // set JoinedRow in RLU

  // rlu joinmap set
  /*  M  */ EO_RLU_RESET_JOINMAP,      // reset the JOINMAP
  /*  M  */ EO_RLU_SET_JOINMAP_BYTE,   // set the JOINMAP byte for given index

  // rlu operators
  /*  M  */ EO_RLU_INDEXDEF_ADDCOLUMN,     // add a column to indexDefRlu[]
  /*  M  */ EO_RLU_INDEXDEF_DELETE,        // delete the indexDefRlu[]
  /*  M  */ EO_RLU_INDEXDEF_NEW,           // new an indexDef in indexDefRlu[]
  /*  M  */ EO_RLU_INDEXTREE_NEW,          // new an indexTree in indexTreeRlu[]
  /*  M  */ EO_RLU_ROWPIX_PRINT,           // print row pix
  /*  M  */ EO_RLU_POT_COMPLETED,          // do we have a full pot buffer ?

  // ixt device col operators
  /*  S  */ EO_DEVROW_COLS_PUSHSET_PST,  // push column subset from dev row
  /*  S  */ EO_DEVIXT_COLS_TO_LOCALS,    // inflate dev columns direct into locals
  /*  S  */ EO_DEVIXT_COLS_TO_ROW,       // push all columns from ixt dev to row dev
  /*  S  */ EO_DEVIXT_COLS_TO_ROW_OFS,   // push columns from ixt dev to row dev at offset column
  /*  S  */ EO_DEVROW_COLS_TO_LOCALS,    // row columns into locals
  /*  S  */ EO_DEVROW_COLS_TO_ROW,       // row columns into row
  /*  S  */ EO_DEVROW_COLS_TO_ROW_OFS,   // row columns into row at offset column

  /*  S  */ EO_DEVROW_NULS_TO_ROW_OFS,   // row columns into row at offset column

  // ixt device crwbuffer operators
  /*  S  */ EO_DEVIXT_CRWBUF_BEGIN, // begin crw buffer traverse
  /*  S  */ EO_DEVIXT_CRWBUF_FINDCRW, // find crw row given rowno
  /*  S  */ EO_DEVIXT_CRWBUF_NEXT,  // move to next row in crw buffer
  /*  S  */ EO_DEVIXT_CRWBUF_NULL,  // null crw buffer
  /*  S  */ EO_DEVIXT_CRWBUF_ROWCNT,// push row count of crwbuffer
  /*  S  */ EO_DEVIXT_CRWBUF_ROWNO, // push rowno of crwbuffer
  /*  S  */ EO_DEVIXT_CRWBUF_TEST,  // test if there is a crw buffer

  // ixt device entry operators
  /*  S  */ EO_DEVIXT_ENT_DEREF,     // deref crw using Ent.ref
  /*  D  */ EO_DEVIXT_ENT_FINDI,     // find entry with int key
  /*  D  */ EO_DEVIXT_ENT_NEXT,      // move to next m_pEnt
  /*  D  */ EO_DEVIXT_ENT_POSPRIOR,  // position m_pEnt prior using m_oEntWrk.key
  /*  D  */ EO_DEVIXT_ENT_PUSH,      // push entry
  /*  D  */ EO_DEVIXT_ENT_PUSHELMI0, // push entry elm 0 int
  /*  D  */ EO_DEVIXT_ENT_PUSHELMS0, // push entry elm 0 str
  /*  D  */ EO_DEVIXT_ENT_PUSHUPPER, // push upper entry
  
  // ixt device entry relational operators
  /*    */ EO_DEVIXT_ENTKEY_LESS,         // test if dev key <  other working key
  /*    */ EO_DEVIXT_ENTKEY_LESSEQUAL,    // test if dev key <= other working key
  /*    */ EO_DEVIXT_ENTKEY_EQUAL,        // test if dev key == other working key
  /*    */ EO_DEVIXT_ENTKEY_NOTEQUAL,     // test if dev key != other working key
  /*    */ EO_DEVIXT_ENTKEY_GREATEREQUAL, // test if dev key >= other working key
  /*    */ EO_DEVIXT_ENTKEY_GREATER,      // test if dev key >  other working key
  /*    */ EO_DEVIXT_ENTKEY_PRINTKEYS,    // print device key and other working key

  // ixt device entwrk operators
  /*    */ EO_ENTWRK_PRINT,     // print m_oEntWrk
  /*    */ EO_ENTWRK_PUSHELMI,  // push element
  /*    */ EO_ENTWRK_PUSHELMS,  // push string element
  /*    */ EO_ENTWRK_RESET,     // reset m_oEntWrk
  /*    */ EO_ENTWRK_SETENTKND, // set entry kind
  /*    */ EO_ENTWRK_SETINTELM, // set int only element value
  /*    */ EO_ENTWRK_SETSTZELM, // set stz only element value

  // ixt device index operators
  /*  S  */ EO_DEVIXT_IDX_DEL,      // delete current m_aEnt from indexTree
  /*  S  */ EO_DEVIXT_IDX_DUMP,     // dump index tree
  /*  S  */ EO_DEVIXT_IDX_INS,      // insert an entry into the indexTree
  /*  S  */ EO_DEVIXT_IDX_BEGIN,    // begin linear traverse of indexTree
  /*  S  */ EO_DEVIXT_IDX_NEXT,     // traverse to next entry
  /*  S  */ EO_DEVIXT_IDX_ENTRIES,  // push number of entries in index

  /*  D  */ EO_DEVIXT_LBOUND_GE,    // position at lower bound (greater than equal)
  /*  D  */ EO_DEVIXT_LBOUND_GT,    // position at lower bound (greater than)
  /*  D  */ EO_DEVIXT_UBOUND_LE,    // get upper bound (less than equal)
  /*  D  */ EO_DEVIXT_UBOUND_LT,    // get upper bound (less than)
  /*  D  */ EO_DEVIXT_CMP_KEYS,     // compare keys of two devices
  /*  D  */ EO_DEVIXT_POS_BYKEY,    // position dev ixt by key
  /*  D  */ EO_DEVIXT_ENT_PRINT,    // print ent

  // ixt device index set operators
  /*  S  */ EO_DEVIXT_INDEXNM_SET,      // set the index name in the dev
  /*  S  */ EO_DEVIXT_INDEXTREE_SET,    // set the indexTree in the ixt dev

  // row device entWrk
  /*    */ EO_DEVROW_ENTWRK_APPENDELMI,// append next element value (int)
  /*    */ EO_DEVROW_ENTWRK_APPENDELMS,// append next element value (str)
  /*    */ EO_DEVROW_ENTWRK_CREATEPAY, // create a payload in entwrk
  /*    */ EO_DEVROW_TABLENM_SET,      // set the table name in the row dev

  // row device row operators
  /*     */ EO_DEVROW_ROW_NEW,       // create new row in row dev
  /*     */ EO_DEVROW_ROW_PRINT,     // print row from row dev
  /*     */ EO_DEVROW_ROW_SET,       // set rowslab in row dev
  /*     */ EO_DEVROW_ROW_TEST,      // test if row in row dev
  /*     */ EO_DEVROW_ROW_ZERO,      // zero destination row in row dev

  // row device row buffer operators
  /*     */ EO_DEVROW_ROWBUF_BEGIN,  // begin row traverse in row dev
  /*  S  */ EO_DEVROW_ROWBUF_FINDROW, // find row row given rowno
  /*     */ EO_DEVROW_ROWBUF_NEXT,   // move to next src row in row dev
  /*     */ EO_DEVROW_ROWBUF_NULL,   // null row buffer in the row dev
  /*     */ EO_DEVROW_ROWBUF_RESET,  // reset rowslab in row dev
  /*  S  */ EO_DEVROW_ROWBUF_ROWCNT,// push row count of rowbuffer
  /*  S  */ EO_DEVROW_ROWBUF_ROWNO, // push rowno of rowbuffer
  /*     */ EO_DEVROW_ROWBUF_TEST,   // test if buffer in row dev

  // row device column pop operators
  /*    */ EO_DEVROW_POPCOLI,  // pop and deposit top integer into column
  /*    */ EO_DEVROW_POPCOLS,  // pop and deposit top string into column

  // row device column push operators
  /*    */ EO_DEVROW_PUSHCOLI, // push column
  /*    */ EO_DEVROW_PUSHCOLS, // push column string

  // pay load
  /*  R  */ EO_PAY_ADDRESS,      // get payload address
  /*  M  */ EO_PAY_FETCH_COLI,   // fetch a column from entry payload
  /*  M  */ EO_PAY_FETCH_COLS,   // fetch a column from entry payload
  /*  M  */ EO_PAY_STORE_COLI,   // store a column into entry payload
  /*  M  */ EO_PAY_STORE_COLS,   // store a column into entry payload

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // -- TO GO --
  /*  S  */ EO_ENTWRK_SETINTELM_OLD,   // set int elm (assuming key kind is int)

  // dev col operators
  /*  S  */ EO_DEVIXT_COL_PUSHSET_ADR,   // push column value from dev ixt (str as ptr)
  /*  S  */ EO_DEVIXT_COL_PUSHSET_PST,   // push column subset from dev ixt

  // key
  /*  M  */ EO_KEY_LCL2C_TO_I,   // convert two local varchar(1) to int
  /*  D  */ EO_KEY_ROW2C_TO_I,   // convert two row varchar(1) to int

  // pot
  /*  M  */ EO_POT_USE_STREAMDEFOUT, // use streamDefOut in the pot

  // register operators
  /*  M  */ EO_REG_EOSMARKER_NEW,          // new an eosMarker in Operand[]           // OLD QUERIES (will go)
  /*  M  */ EO_REG_PTNSTREAM_CLOSE,        // close a ptn stream
  /*  M  */ EO_REG_PTNSTREAM_DELETE,       // delete a ptn stream
  /*  M  */ EO_REG_PTNSTREAM_NEW,          // new a ptn stream
  /*  M  */ EO_REG_PTNSTREAM_OPEN,         // open a ptn stream
  /*  M  */ EO_REG_PTNSTREAM_READ,         // read a ptn stream
  /*  M  */ EO_REG_STREAMDEFOUT_NEW,       // new an streamDef in streamDefOut[]        // OLD QUERIES (will go)
  /*  M  */ EO_REG_STREAMDEFOUT_ADDCOLUMN, // add a column to streamDefOut[]            // OLD QUERIES (will go)
  /*  M  */ EO_REG_STREAMDEF_STREAMDEFOUT, // copy StreamDef[] to streamDefOut[]        // OLD QUERIES (will go)

  // super fragments
  /*  M  */ EO_SUPER_Q100,       // super fragment Q100

  /*  D  */ EO_DEVIXT_LBOUND_GE_OLD,    // position at lower bound (greater than equal)
  /*  D  */ EO_DEVIXT_UBOUND_LT_OLD,    // get upper bound (less than)

  /*    */ EO_ENUMS
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

#ifndef __TINYC__

// when not being included by the tcc compiler
static Stz* aEO[]
{
  /*  0 */ "NONE"_Stz,
  
  // arithmetic operators
  // add
  /*    */ "EO_ADD_BCDBCD"_Stz,
  /*    */ "EO_ADD_BCDFLT"_Stz,
  /*    */ "EO_ADD_BCDINT"_Stz,
  /*    */ "EO_ADD_DTEIVL"_Stz,
  /*    */ "EO_ADD_FLTBCD"_Stz,
  /*    */ "EO_ADD_FLTINT"_Stz,
  /*    */ "EO_ADD_FLTFLT"_Stz,
  /*    */ "EO_ADD_INTBCD"_Stz,
  /*    */ "EO_ADD_INTFLT"_Stz,
  /*    */ "EO_ADD_INTINT"_Stz,
  /*    */ "EO_ADD_IVLDTE"_Stz,
  /*    */ "EO_ADD_IVLIVL"_Stz,
  /*    */ "EO_ADD_IVLTME"_Stz,
  /*    */ "EO_ADD_IVLTSP"_Stz,
  /*    */ "EO_ADD_TMEIVL"_Stz,
  /*    */ "EO_ADD_TSPIVL"_Stz,
  /*    */ "EO_ADD_UNKXYZ"_Stz,
  /*    */ "EO_ADD_XYZUNK"_Stz,

  // divide
  /*    */ "EO_DIV_BCDBCD"_Stz,
  /*    */ "EO_DIV_BCDFLT"_Stz,
  /*    */ "EO_DIV_BCDINT"_Stz,
  /*    */ "EO_DIV_DTEIVL"_Stz,
  /*    */ "EO_DIV_FLTBCD"_Stz,
  /*    */ "EO_DIV_FLTINT"_Stz,
  /*    */ "EO_DIV_FLTFLT"_Stz,
  /*    */ "EO_DIV_INTBCD"_Stz,
  /*    */ "EO_DIV_INTFLT"_Stz,
  /*    */ "EO_DIV_INTINT"_Stz,
  /*    */ "EO_DIV_IVLDTE"_Stz,
  /*    */ "EO_DIV_IVLIVL"_Stz,
  /*    */ "EO_DIV_IVLTME"_Stz,
  /*    */ "EO_DIV_IVLTSP"_Stz,
  /*    */ "EO_DIV_TMEIVL"_Stz,
  /*    */ "EO_DIV_TSPIVL"_Stz,
  /*    */ "EO_DIV_UNKXYZ"_Stz,
  /*    */ "EO_DIV_XYZUNK"_Stz,

  // modulo
  /*    */ "EO_MOD_INTINT"_Stz,
  /*    */ "EO_MOD_UNKXYZ"_Stz,
  /*    */ "EO_MOD_XYZUNK"_Stz,

  // multiply
  /*    */ "EO_MUL_BCDBCD"_Stz,
  /*    */ "EO_MUL_BCDFLT"_Stz,
  /*    */ "EO_MUL_BCDINT"_Stz,
  /*    */ "EO_MUL_DTEIVL"_Stz,
  /*    */ "EO_MUL_FLTBCD"_Stz,
  /*    */ "EO_MUL_FLTINT"_Stz,
  /*    */ "EO_MUL_FLTFLT"_Stz,
  /*    */ "EO_MUL_INTBCD"_Stz,
  /*    */ "EO_MUL_INTFLT"_Stz,
  /*    */ "EO_MUL_INTINT"_Stz,
  /*    */ "EO_MUL_IVLDTE"_Stz,
  /*    */ "EO_MUL_IVLIVL"_Stz,
  /*    */ "EO_MUL_IVLTME"_Stz,
  /*    */ "EO_MUL_IVLTSP"_Stz,
  /*    */ "EO_MUL_TMEIVL"_Stz,
  /*    */ "EO_MUL_TSPIVL"_Stz,
  /*    */ "EO_MUL_UNKXYZ"_Stz,
  /*    */ "EO_MUL_XYZUNK"_Stz,

  /*    */ "EO_NEG_BCD"_Stz,
  /*    */ "EO_NEG_FLT"_Stz,
  /*    */ "EO_NEG_INT"_Stz,
  /*    */ "EO_NEG_IVL"_Stz,

  // subtract
  /*    */ "EO_SUB_BCDBCD"_Stz,
  /*    */ "EO_SUB_BCDFLT"_Stz,
  /*    */ "EO_SUB_BCDINT"_Stz,
  /*    */ "EO_SUB_DTEIVL"_Stz,
  /*    */ "EO_SUB_FLTBCD"_Stz,
  /*    */ "EO_SUB_FLTINT"_Stz,
  /*    */ "EO_SUB_FLTFLT"_Stz,
  /*    */ "EO_SUB_INTBCD"_Stz,
  /*    */ "EO_SUB_INTFLT"_Stz,
  /*    */ "EO_SUB_INTINT"_Stz,
  /*    */ "EO_SUB_IVLDTE"_Stz,
  /*    */ "EO_SUB_IVLIVL"_Stz,
  /*    */ "EO_SUB_IVLTME"_Stz,
  /*    */ "EO_SUB_IVLTSP"_Stz,
  /*    */ "EO_SUB_TMEIVL"_Stz,
  /*    */ "EO_SUB_TSPIVL"_Stz,
  /*    */ "EO_SUB_UNKXYZ"_Stz,
  /*    */ "EO_SUB_XYZUNK"_Stz,

  // counter
  /*    */ "EO_CTR_0_DEC"_Stz,
  /*    */ "EO_CTR_0_INC"_Stz,
  /*    */ "EO_CTR_0_POP"_Stz,
  /*    */ "EO_CTR_0_PUSH"_Stz,

  // jump operators
  /*    */ "EO_JMP"_Stz,
  /*    */ "EO_JMP_FALSE"_Stz,
  /*    */ "EO_JMP_TRUE"_Stz,

  /*    */ "EO_JMP_LESS"_Stz,
  /*    */ "EO_JMP_LESSEQUAL"_Stz,
  /*    */ "EO_JMP_EQUAL"_Stz,
  /*    */ "EO_JMP_NOTEQUAL"_Stz,
  /*    */ "EO_JMP_GREATEREQUAL"_Stz,
  /*    */ "EO_JMP_GREATER"_Stz,

  // logical operators
  /*    */ "EO_LGL_AND"_Stz,
  /*    */ "EO_LGL_OR"_Stz,
  /*    */ "EO_LGL_NOT"_Stz,
  /*    */ "EO_LGL_XOR"_Stz,

  // null test operators
  /*    */ "EO_ISNULL"_Stz,
  /*    */ "EO_ISNULL_STR"_Stz,
  /*    */ "EO_ISNOTNULL"_Stz,

  // prints
  /*    */ "EO_PRINT"_Stz,
  /*    */ "EO_PRINTF"_Stz,

  // pushes
  /*    */ "EO_DATE"_Stz,
  /*    */ "EO_INTEGER"_Stz,
  /*    */ "EO_PUSHNULL"_Stz,
  /*    */ "EO_PUSH_MINUS1"_Stz,
  /*    */ "EO_PUSH_0"_Stz,
  /*    */ "EO_PUSH_1"_Stz,
  /*    */ "EO_RANDOM"_Stz,
  /*    */ "EO_STRING"_Stz,
  /*    */ "EO_TIME"_Stz,
  /*    */ "EO_TIMESTAMP"_Stz,
  
  // relational operators
  /*    */ "EO_EQ_BCDBCD"_Stz,
  /*    */ "EO_EQ_BCDFLT"_Stz,
  /*    */ "EO_EQ_BCDINT"_Stz,
  /*    */ "EO_EQ_BOLBOL"_Stz,
  /*    */ "EO_EQ_DTEDTE"_Stz,
  /*    */ "EO_EQ_DTETSP"_Stz,
  /*    */ "EO_EQ_FLTBCD"_Stz,
  /*    */ "EO_EQ_FLTFLT"_Stz,
  /*    */ "EO_EQ_FLTINT"_Stz,
  /*    */ "EO_EQ_INTBCD"_Stz,
  /*    */ "EO_EQ_INTFLT"_Stz,
  /*    */ "EO_EQ_INTINT"_Stz,
  /*    */ "EO_EQ_IVLIVL"_Stz,
  /*    */ "EO_EQ_TMETME"_Stz,
  /*    */ "EO_EQ_TSPDTE"_Stz,
  /*    */ "EO_EQ_TSPTSP"_Stz,
  /*    */ "EO_EQ_VCHVCH"_Stz,
  /*    */ "EO_EQ_UNKXYZ"_Stz,
  /*    */ "EO_EQ_XYZUNK"_Stz,

  /*    */ "EO_GE_BCDBCD"_Stz,
  /*    */ "EO_GE_BCDFLT"_Stz,
  /*    */ "EO_GE_BCDINT"_Stz,
  /*    */ "EO_GE_BOLBOL"_Stz,
  /*    */ "EO_GE_DTEDTE"_Stz,
  /*    */ "EO_GE_DTETSP"_Stz,
  /*    */ "EO_GE_FLTBCD"_Stz,
  /*    */ "EO_GE_FLTFLT"_Stz,
  /*    */ "EO_GE_FLTINT"_Stz,
  /*    */ "EO_GE_INTBCD"_Stz,
  /*    */ "EO_GE_INTFLT"_Stz,
  /*    */ "EO_GE_INTINT"_Stz,
  /*    */ "EO_GE_IVLIVL"_Stz,
  /*    */ "EO_GE_TMETME"_Stz,
  /*    */ "EO_GE_TSPDTE"_Stz,
  /*    */ "EO_GE_TSPTSP"_Stz,
  /*    */ "EO_GE_VCHVCH"_Stz,
  /*    */ "EO_GE_UNKXYZ"_Stz,
  /*    */ "EO_GE_XYZUNK"_Stz,

  /*    */ "EO_GT_BCDBCD"_Stz,
  /*    */ "EO_GT_BCDFLT"_Stz,
  /*    */ "EO_GT_BCDINT"_Stz,
  /*    */ "EO_GT_BOLBOL"_Stz,
  /*    */ "EO_GT_DTEDTE"_Stz,
  /*    */ "EO_GT_DTETSP"_Stz,
  /*    */ "EO_GT_FLTBCD"_Stz,
  /*    */ "EO_GT_FLTFLT"_Stz,
  /*    */ "EO_GT_FLTINT"_Stz,
  /*    */ "EO_GT_INTBCD"_Stz,
  /*    */ "EO_GT_INTFLT"_Stz,
  /*    */ "EO_GT_INTINT"_Stz,
  /*    */ "EO_GT_IVLIVL"_Stz,
  /*    */ "EO_GT_TMETME"_Stz,
  /*    */ "EO_GT_TSPDTE"_Stz,
  /*    */ "EO_GT_TSPTSP"_Stz,
  /*    */ "EO_GT_VCHVCH"_Stz,
  /*    */ "EO_GT_UNKXYZ"_Stz,
  /*    */ "EO_GT_XYZUNK"_Stz,

  /*    */ "EO_LE_BCDBCD"_Stz,
  /*    */ "EO_LE_BCDFLT"_Stz,
  /*    */ "EO_LE_BCDINT"_Stz,
  /*    */ "EO_LE_BOLBOL"_Stz,
  /*    */ "EO_LE_DTEDTE"_Stz,
  /*    */ "EO_LE_DTETSP"_Stz,
  /*    */ "EO_LE_FLTBCD"_Stz,
  /*    */ "EO_LE_FLTFLT"_Stz,
  /*    */ "EO_LE_FLTINT"_Stz,
  /*    */ "EO_LE_INTBCD"_Stz,
  /*    */ "EO_LE_INTFLT"_Stz,
  /*    */ "EO_LE_INTINT"_Stz,
  /*    */ "EO_LE_IVLIVL"_Stz,
  /*    */ "EO_LE_TMETME"_Stz,
  /*    */ "EO_LE_TSPDTE"_Stz,
  /*    */ "EO_LE_TSPTSP"_Stz,
  /*    */ "EO_LE_VCHVCH"_Stz,
  /*    */ "EO_LE_UNKXYZ"_Stz,
  /*    */ "EO_LE_XYZUNK"_Stz,

  /*    */ "EO_LT_BCDBCD"_Stz,
  /*    */ "EO_LT_BCDFLT"_Stz,
  /*    */ "EO_LT_BCDINT"_Stz,
  /*    */ "EO_LT_BOLBOL"_Stz,
  /*    */ "EO_LT_DTEDTE"_Stz,
  /*    */ "EO_LT_DTETSP"_Stz,
  /*    */ "EO_LT_FLTBCD"_Stz,
  /*    */ "EO_LT_FLTFLT"_Stz,
  /*    */ "EO_LT_FLTINT"_Stz,
  /*    */ "EO_LT_INTBCD"_Stz,
  /*    */ "EO_LT_INTFLT"_Stz,
  /*    */ "EO_LT_INTINT"_Stz,
  /*    */ "EO_LT_IVLIVL"_Stz,
  /*    */ "EO_LT_TMETME"_Stz,
  /*    */ "EO_LT_TSPDTE"_Stz,
  /*    */ "EO_LT_TSPTSP"_Stz,
  /*    */ "EO_LT_VCHVCH"_Stz,
  /*    */ "EO_LT_UNKXYZ"_Stz,
  /*    */ "EO_LT_XYZUNK"_Stz,

  /*    */ "EO_NE_BCDBCD"_Stz,
  /*    */ "EO_NE_BCDFLT"_Stz,
  /*    */ "EO_NE_BCDINT"_Stz,
  /*    */ "EO_NE_BOLBOL"_Stz,
  /*    */ "EO_NE_DTEDTE"_Stz,
  /*    */ "EO_NE_DTETSP"_Stz,
  /*    */ "EO_NE_FLTBCD"_Stz,
  /*    */ "EO_NE_FLTFLT"_Stz,
  /*    */ "EO_NE_FLTINT"_Stz,
  /*    */ "EO_NE_INTBCD"_Stz,
  /*    */ "EO_NE_INTFLT"_Stz,
  /*    */ "EO_NE_INTINT"_Stz,
  /*    */ "EO_NE_IVLIVL"_Stz,
  /*    */ "EO_NE_TMETME"_Stz,
  /*    */ "EO_NE_TSPDTE"_Stz,
  /*    */ "EO_NE_TSPTSP"_Stz,
  /*    */ "EO_NE_VCHVCH"_Stz,
  /*    */ "EO_NE_UNKXYZ"_Stz,
  /*    */ "EO_NE_XYZUNK"_Stz,
  
  // column pushes
  /*    */ "EO_PUSH_COLUMN_LENGTH"_Stz,
  /*    */ "EO_PUSH_COLUMN_SCALE"_Stz,
  /*    */ "EO_PUSH_COLUMN_TYPE"_Stz,

  // ixt device pushes
  /*    */ "EO_PUSH_DEVIXT_ALT"_Stz,
  /*    */ "EO_PUSH_DEVIXT_ENT"_Stz,
  /*    */ "EO_PUSH_DEVIXT_LFT"_Stz,
  /*    */ "EO_PUSH_DEVIXT_RHT"_Stz,
  /*    */ "EO_PUSH_DEVIXT_TMP"_Stz,

  // row device pushes
  /*    */ "EO_PUSH_DEVROW_ALT"_Stz,
  /*    */ "EO_PUSH_DEVROW_DST"_Stz,
  /*    */ "EO_PUSH_DEVROW_LFT"_Stz,
  /*    */ "EO_PUSH_DEVROW_RHT"_Stz,

  // rlu pushes
  /*    */ "EO_PUSH_RLU_COLSET"_Stz,
  /*    */ "EO_PUSH_RLU_ENT"_Stz,
  /*    */ "EO_PUSH_RLU_ENTWRK"_Stz,
  /*    */ "EO_PUSH_RLU_INDEXDEF"_Stz,
  /*    */ "EO_PUSH_RLU_INDEXTREE"_Stz,
  /*    */ "EO_PUSH_RLU_JOINEDROW"_Stz,
  /*    */ "EO_PUSH_RLU_JOINMAP_BYTE"_Stz,
  /*    */ "EO_PUSH_RLU_LOCALS"_Stz,
  /*    */ "EO_PUSH_RLU_POT"_Stz,
  /*    */ "EO_PUSH_RLU_PTN"_Stz,
  /*    */ "EO_PUSH_RLU_ROWPIX"_Stz,

  // table ptn
  /*    */ "EO_TABLEPTN_DROPREAD"_Stz,
  /*    */ "EO_TABLEPTN_GRABREAD"_Stz,

  // stack operators
  /*    */ "EO_INC"_Stz,
  /*    */ "EO_DEC"_Stz,
  /*    */ "EO_DUP"_Stz,
  /*    */ "EO_DUP_STR"_Stz,
  /*    */ "EO_POP"_Stz,
  /*    */ "EO_POP_LOCALI"_Stz,
  /*    */ "EO_POP_LOCALS"_Stz,
  /*    */ "EO_POP_STR"_Stz,
  /*    */ "EO_PUSH_LOCALI"_Stz,
  /*    */ "EO_PUSH_LOCALS"_Stz,
  
  // string operators
  /*    */ "EO_STR_COMPARE"_Stz,
  /*    */ "EO_STR_COMPARE_a"_Stz,
  /*    */ "EO_STR_PRINTADR"_Stz,
  /*    */ "EO_STR_PRINTSTK"_Stz,
  /*    */ "EO_STR_LITERAL"_Stz,
  
  // subroutine operators
  /*    */ "EO_CALL"_Stz,
  /*    */ "EO_RETURN"_Stz,
  /*    */ "EO_RETURN_FALSE"_Stz,
  /*    */ "EO_RETURN_TRUE"_Stz,
  /*    */ "EO_RETURN_VALUE"_Stz,

  // misc
  /*    */ "EO_ASSERT"_Stz,
  /*    */ "EO_BREAKPOINT"_Stz,
  /*    */ "EO_CALLAGAIN"_Stz,
  /*    */ "EO_CALLEDAGAIN"_Stz,
  /*    */ "EO_DUMP"_Stz,
  /*    */ "EO_ENTER_LU"_Stz,
  /*    */ "EO_EXIT_LU"_Stz,
  /*    */ "EO_HANDLER"_Stz,
  /*    */ "EO_INIT"_Stz,
  /*    */ "EO_INTEGRITY"_Stz,
  /*    */ "EO_NEVERREACH"_Stz,
  /*    */ "EO_SANITY"_Stz,
  /*    */ "EO_SLEEP"_Stz,
  /*    */ "EO_SQL"_Stz,
  /*    */ "EO_TABLEPTN"_Stz,
  /*    */ "EO_TRACEOFF"_Stz,
  /*    */ "EO_TRACEON"_Stz,
  /*    */ "EO_NOOP"_Stz,
  /*    */ "EO_ERROR"_Stz,

  //----------------------------------------------------------------------------
  
  // date operators
  /*    */ "EO_DTE_NOW"_Stz,
  /*    */ "EO_DTE_PRINT"_Stz,
  /*    */ "EO_DTE_YEAR"_Stz,
  
  // hashing
  /*    */ "EO_HSH_ADD_HASHI"_Stz,
  /*    */ "EO_HSH_RESET"_Stz,

  // partitioning
  /*    */ "EO_PTN_CALC"_Stz,
  /*    */ "EO_PTN_PUSH"_Stz,
  /*    */ "EO_PTN_RESET"_Stz,
  /*    */ "EO_PTN_RUNNING"_Stz,

  // timer
  /*    */ "EO_TMR_SINCE"_Stz,
  /*    */ "EO_TMR_START"_Stz,
  /*    */ "EO_TMR_NSECS"_Stz,
  /*    */ "EO_TMR_UPDATE"_Stz,
  /*    */ "EO_TMR_AGG_ADD"_Stz,
  /*    */ "EO_TMR_AGG_NSECS"_Stz,
  /*    */ "EO_TMR_AGG_RESET"_Stz,

  //----------------------------------------------------------------------------
  // rlu column set
  /*    */ "EO_RLU_SET_COLUMN"_Stz,
  /*    */ "EO_RLU_SET_COLUMNS"_Stz,
  /*    */ "EO_RLU_SET_ITEMS"_Stz,

  /*   */ "EO_RLU_RESET_JOINEDROW"_Stz,
  /*   */ "EO_RLU_SET_JOINEDROW"_Stz,

  // rlu joinmap set
  /*    */ "EO_RLU_RESET_JOINMAP"_Stz,
  /*    */ "EO_RLU_SET_JOINMAP_BYTE"_Stz,

  // rlu operators
  /*    */ "EO_RLU_INDEXDEF_ADDCOLUMN"_Stz,
  /*    */ "EO_RLU_INDEXDEF_DELETE"_Stz,
  /*    */ "EO_RLU_INDEXDEF_NEW"_Stz,
  /*    */ "EO_RLU_INDEXTREE_NEW"_Stz,
  /*    */ "EO_RLU_ROWPIX_PRINT"_Stz,
  /*    */ "EO_RLU_POT_COMPLETED"_Stz,

  // ixt device col operators
  /*    */ "EO_DEVROW_COLS_PUSHSET_PST"_Stz,
  /*    */ "EO_DEVIXT_COLS_TO_LOCALS"_Stz,
  /*    */ "EO_DEVIXT_COLS_TO_ROW"_Stz,
  /*    */ "EO_DEVIXT_COLS_TO_ROW_OFS"_Stz,
  /*    */ "EO_DEVROW_COLS_TO_LOCALS"_Stz,
  /*    */ "EO_DEVROW_COLS_TO_ROW"_Stz,
  /*    */ "EO_DEVROW_COLS_TO_ROW_OFS"_Stz,
  /*    */ "EO_DEVROW_NULS_TO_ROW_OFS"_Stz,

  // ixt device crwbuffer operators
  /*    */ "EO_DEVIXT_CRWBUF_BEGIN"_Stz,
  /*    */ "EO_DEVIXT_CRWBUF_FINDCRW"_Stz,
  /*    */ "EO_DEVIXT_CRWBUF_NEXT"_Stz,
  /*    */ "EO_DEVIXT_CRWBUF_NULL"_Stz,
  /*    */ "EO_DEVIXT_CRWBUF_ROWCNT"_Stz,
  /*    */ "EO_DEVIXT_CRWBUF_ROWNO"_Stz,
  /*    */ "EO_DEVIXT_CRWBUF_TEST"_Stz,

  // ixt device entry operators
  /*    */ "EO_DEVIXT_ENT_DEREF"_Stz,
  /*    */ "EO_DEVIXT_ENT_FINDI"_Stz,
  /*    */ "EO_DEVIXT_ENT_NEXT"_Stz,
  /*    */ "EO_DEVIXT_ENT_POSPRIOR"_Stz,
  /*    */ "EO_DEVIXT_ENT_PUSH"_Stz,
  /*    */ "EO_DEVIXT_ENT_PUSHELMI0"_Stz,
  /*    */ "EO_DEVIXT_ENT_PUSHELMS0"_Stz,
  /*    */ "EO_DEVIXT_ENT_PUSHUPPER"_Stz,
  
  // ixt device entry relational operators
  /*    */ "EO_DEVIXT_ENTKEY_LESS"_Stz,
  /*    */ "EO_DEVIXT_ENTKEY_LESSEQUAL"_Stz,
  /*    */ "EO_DEVIXT_ENTKEY_EQUAL"_Stz,
  /*    */ "EO_DEVIXT_ENTKEY_NOTEQUAL"_Stz,
  /*    */ "EO_DEVIXT_ENTKEY_GREATEREQUAL"_Stz,
  /*    */ "EO_DEVIXT_ENTKEY_GREATER"_Stz,
  /*    */ "EO_DEVIXT_ENTKEY_PRINTKEYS"_Stz,

  // ixt device entwrk operators
  /*    */ "EO_ENTWRK_PRINT"_Stz,
  /*    */ "EO_ENTWRK_PUSHELMI"_Stz,
  /*    */ "EO_ENTWRK_PUSHELMS"_Stz,
  /*    */ "EO_ENTWRK_RESET"_Stz,
  /*    */ "EO_ENTWRK_SETENTKND"_Stz,
  /*    */ "EO_ENTWRK_SETINTELM"_Stz,
  /*    */ "EO_ENTWRK_SETSTZELM"_Stz,

  // ixt device index operators
  /*    */ "EO_DEVIXT_IDX_DEL"_Stz,
  /*    */ "EO_DEVIXT_IDX_DUMP"_Stz,
  /*    */ "EO_DEVIXT_IDX_INS"_Stz,
  /*    */ "EO_DEVIXT_IDX_BEGIN"_Stz,
  /*    */ "EO_DEVIXT_IDX_NEXT"_Stz,
  /*    */ "EO_DEVIXT_IDX_ENTRIES"_Stz,

  /*    */ "EO_DEVIXT_LBOUND_GE"_Stz,
  /*    */ "EO_DEVIXT_LBOUND_GT"_Stz,
  /*    */ "EO_DEVIXT_UBOUND_LE"_Stz,
  /*    */ "EO_DEVIXT_UBOUND_LT"_Stz,
  /*    */ "EO_DEVIXT_CMP_KEYS"_Stz,
  /*    */ "EO_DEVIXT_POS_BYKEY"_Stz,
  /*    */ "EO_DEVIXT_ENT_PRINT"_Stz,

  // ixt device index set operators
  /*    */ "EO_DEVIXT_INDEXNM_SET"_Stz,
  /*    */ "EO_DEVIXT_INDEXTREE_SET"_Stz,

  // row device entWrk operators
  /*    */ "EO_DEVROW_ENTWRK_APPENDELMI"_Stz,
  /*    */ "EO_DEVROW_ENTWRK_APPENDELMS"_Stz,
  /*    */ "EO_DEVROW_ENTWRK_CREATEPAY"_Stz,
  /*    */ "EO_DEVROW_TABLENM_SET"_Stz,

  // row device row operators
  /*     */ "EO_DEVROW_ROW_NEW"_Stz,
  /*     */ "EO_DEVROW_ROW_PRINT"_Stz,
  /*     */ "EO_DEVROW_ROW_SET"_Stz,
  /*     */ "EO_DEVROW_ROW_TEST"_Stz,
  /*     */ "EO_DEVROW_ROW_ZERO"_Stz,

  // row device row buffer operators
  /*     */ "EO_DEVROW_ROWBUF_BEGIN"_Stz,
  /*     */ "EO_DEVROW_ROWBUF_FINDROW"_Stz,
  /*     */ "EO_DEVROW_ROWBUF_NEXT"_Stz,
  /*     */ "EO_DEVROW_ROWBUF_NULL"_Stz,
  /*     */ "EO_DEVROW_ROWBUF_RESET"_Stz,
  /*     */ "EO_DEVROW_ROWBUF_ROWCNT"_Stz,
  /*     */ "EO_DEVROW_ROWBUF_ROWNO"_Stz,
  /*     */ "EO_DEVROW_ROWBUF_TEST"_Stz,

  //  row device column operators
  /*    */ "EO_DEVROW_POPCOLI"_Stz,
  /*    */ "EO_DEVROW_POPCOLS"_Stz,
  /*    */ "EO_DEVROW_PUSHCOLI"_Stz,
  /*    */ "EO_DEVROW_PUSHCOLS"_Stz,

  // pay load operators
  /*    */ "EO_PAY_ADDRESS"_Stz,
  /*    */ "EO_PAY_FETCH_COLI"_Stz,
  /*    */ "EO_PAY_FETCH_COLS"_Stz,
  /*    */ "EO_PAY_STORE_COLI"_Stz,
  /*    */ "EO_PAY_STORE_COLS"_Stz,

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // -- TO GO --

  /*    */ "EO_ENTWRK_SETINTELM_OLD"_Stz,

  // dev col
  /*    */ "EO_DEVIXT_COL_PUSHSET_ADR"_Stz,
  /*    */ "EO_DEVIXT_COL_PUSHSET_PST"_Stz,

  // key
  /*    */ "EO_KEY_LCL2C_TO_I"_Stz,
  /*    */ "EO_KEY_ROW2C_TO_I"_Stz,

  // pot
  /*    */ "EO_POT_USE_STREAMDEFOUT"_Stz,

    // register operators
  /*    */ "EO_REG_EOSMARKER_NEW"_Stz,
  /*    */ "EO_REG_PTNSTREAM_CLOSE"_Stz,
  /*    */ "EO_REG_PTNSTREAM_DELETE"_Stz,
  /*    */ "EO_REG_PTNSTREAM_NEW"_Stz,
  /*    */ "EO_REG_PTNSTREAM_OPEN"_Stz,
  /*    */ "EO_REG_PTNSTREAM_READ"_Stz,
  /*    */ "EO_REG_STREAMDEFOUT_NEW"_Stz,
  /*    */ "EO_REG_STREAMDEFOUT_ADDCOLUMN"_Stz,
  /*    */ "EO_REG_STREAMDEF_STREAMDEFOUT"_Stz,

  // super fragments
  /*    */ "EO_SUPER_Q100"_Stz,

  /*  D  */ "EO_DEVIXT_LBOUND_GE_OLD"_Stz,
  /*  D  */ "EO_DEVIXT_UBOUND_LT_OLD"_Stz,

    /*    */ "ENUMS"_Stz
};

//----------------------------------------------------------------------------

struct EoArg
{
  eEA m_eArgument;
  i8  m_nArgBytes;
};

/*----------------------------------------------------------------------------*/
/* Description of EO CmdLineArgs                                              */
/*----------------------------------------------------------------------------*/
static EoArg EoArgs[]
{
  { EA_NONE,    0 },  // EO_NONE

  // arithmetic operators
  // add
  { EA_NONE,    0 },  // EO_ADD_BCDBCD
  { EA_NONE,    0 },  // EO_ADD_BCDFLT
  { EA_NONE,    0 },  // EO_ADD_BCDINT
  { EA_NONE,    0 },  // EO_ADD_DTEIVL
  { EA_NONE,    0 },  // EO_ADD_FLTBCD
  { EA_NONE,    0 },  // EO_ADD_FLTINT
  { EA_NONE,    0 },  // EO_ADD_FLTFLT
  { EA_NONE,    0 },  // EO_ADD_INTBCD
  { EA_NONE,    0 },  // EO_ADD_INTFLT
  { EA_NONE,    0 },  // EO_ADD_INTINT
  { EA_NONE,    0 },  // EO_ADD_IVLDTE
  { EA_NONE,    0 },  // EO_ADD_IVLIVL
  { EA_NONE,    0 },  // EO_ADD_IVLTME
  { EA_NONE,    0 },  // EO_ADD_IVLTSP
  { EA_NONE,    0 },  // EO_ADD_TMEIVL
  { EA_NONE,    0 },  // EO_ADD_TSPIVL
  { EA_NONE,    0 },  // EO_ADD_UNKXYZ
  { EA_NONE,    0 },  // EO_ADD_XYZUNK

  // divide
  { EA_NONE,    0 },  // EO_DIV_BCDBCD
  { EA_NONE,    0 },  // EO_DIV_BCDFLT
  { EA_NONE,    0 },  // EO_DIV_BCDINT
  { EA_NONE,    0 },  // EO_DIV_DTEIVL
  { EA_NONE,    0 },  // EO_DIV_FLTBCD
  { EA_NONE,    0 },  // EO_DIV_FLTINT
  { EA_NONE,    0 },  // EO_DIV_FLTFLT
  { EA_NONE,    0 },  // EO_DIV_INTBCD
  { EA_NONE,    0 },  // EO_DIV_INTFLT
  { EA_NONE,    0 },  // EO_DIV_INTINT
  { EA_NONE,    0 },  // EO_DIV_IVLDTE
  { EA_NONE,    0 },  // EO_DIV_IVLIVL
  { EA_NONE,    0 },  // EO_DIV_IVLTME
  { EA_NONE,    0 },  // EO_DIV_IVLTSP
  { EA_NONE,    0 },  // EO_DIV_TMEIVL
  { EA_NONE,    0 },  // EO_DIV_TSPIVL
  { EA_NONE,    0 },  // EO_DIV_UNKXYZ
  { EA_NONE,    0 },  // EO_DIV_XYZUNK

  // modulo
  { EA_NONE,    0 },  // EO_MOD_INTINT
  { EA_NONE,    0 },  // EO_MOD_UNKXYZ
  { EA_NONE,    0 },  // EO_MOD_XYZUNK

  // multiply
  { EA_NONE,    0 },  // EO_MUL_BCDBCD
  { EA_NONE,    0 },  // EO_MUL_BCDFLT
  { EA_NONE,    0 },  // EO_MUL_BCDINT
  { EA_NONE,    0 },  // EO_MUL_DTEIVL
  { EA_NONE,    0 },  // EO_MUL_FLTBCD
  { EA_NONE,    0 },  // EO_MUL_FLTINT
  { EA_NONE,    0 },  // EO_MUL_FLTFLT
  { EA_NONE,    0 },  // EO_MUL_INTBCD
  { EA_NONE,    0 },  // EO_MUL_INTFLT
  { EA_NONE,    0 },  // EO_MUL_INTINT
  { EA_NONE,    0 },  // EO_MUL_IVLDTE
  { EA_NONE,    0 },  // EO_MUL_IVLIVL
  { EA_NONE,    0 },  // EO_MUL_IVLTME
  { EA_NONE,    0 },  // EO_MUL_IVLTSP
  { EA_NONE,    0 },  // EO_MUL_TMEIVL
  { EA_NONE,    0 },  // EO_MUL_TSPIVL
  { EA_NONE,    0 },  // EO_MUL_UNKXYZ
  { EA_NONE,    0 },  // EO_MUL_XYZUNK

  { EA_NONE,    0 },  // EO_NEG_BCD
  { EA_NONE,    0 },  // EO_NEG_FLT
  { EA_NONE,    0 },  // EO_NEG_INT
  { EA_NONE,    0 },  // EO_NEG_IVL

  // subtract
  { EA_NONE,    0 },  // EO_SUB_BCDBCD
  { EA_NONE,    0 },  // EO_SUB_BCDFLT
  { EA_NONE,    0 },  // EO_SUB_BCDINT
  { EA_NONE,    0 },  // EO_SUB_DTEIVL
  { EA_NONE,    0 },  // EO_SUB_FLTBCD
  { EA_NONE,    0 },  // EO_SUB_FLTINT
  { EA_NONE,    0 },  // EO_SUB_FLTFLT
  { EA_NONE,    0 },  // EO_SUB_INTBCD
  { EA_NONE,    0 },  // EO_SUB_INTFLT
  { EA_NONE,    0 },  // EO_SUB_INTINT
  { EA_NONE,    0 },  // EO_SUB_IVLDTE
  { EA_NONE,    0 },  // EO_SUB_IVLIVL
  { EA_NONE,    0 },  // EO_SUB_IVLTME
  { EA_NONE,    0 },  // EO_SUB_IVLTSP
  { EA_NONE,    0 },  // EO_SUB_TMEIVL
  { EA_NONE,    0 },  // EO_SUB_TSPIVL
  { EA_NONE,    0 },  // EO_SUB_UNKXYZ
  { EA_NONE,    0 },  // EO_SUB_XYZUNK

   // counter
  { EA_NONE,    0 },  // EO_CTR_0_DEC
  { EA_NONE,    0 },  // EO_CTR_0_INC
  { EA_NONE,    0 },  // EO_CTR_0_POP
  { EA_NONE,    0 },  // EO_CTR_0_PUSH

  // jump operators
  { EA_LABEL,   8 },  // EO_JMP
  { EA_LABEL,   8 },  // EO_JMP_FALSE
  { EA_LABEL,   8 },  // EO_JMP_TRUE

  { EA_LABEL,   8 },  // EO_JMP_LESSTHAN
  { EA_LABEL,   8 },  // EO_JMP_LESSEQUAL
  { EA_LABEL,   8 },  // EO_JMP_EQUAL
  { EA_LABEL,   8 },  // EO_JMP_NOTEQUAL
  { EA_LABEL,   8 },  // EO_JMP_GREATEREQUAL
  { EA_LABEL,   8 },  // EO_JMP_GREATEREQUAL

  // logical operators
  { EA_NONE,    0 },  // EO_LGL_AND
  { EA_NONE,    0 },  // EO_LGL_OR
  { EA_NONE,    0 },  // EO_LGL_NOT
  { EA_NONE,    0 },  // EO_LGL_XOR

  // null test operators
  { EA_NONE,    0 },  // EO_ISNULL
  { EA_NONE,    0 },  // EO_ISNULL_STR
  { EA_NONE,    0 },  // EO_ISNOTNULL

  // prints
  { EA_NONE,    0 },  // EO_PRINT
  { EA_STRING, -1 },  // EO_PRINTF

  // pushes
  // pushes
  { EA_DATE,    8 },  // EO_DATE
  { EA_INT,     8 },  // EO_INTEGER
  { EA_NONE,    0 },  // EO_PUSHNULL
  { EA_NONE,    0 },  // EO_PUSH_MINUS1
  { EA_NONE,    0 },  // EO_PUSH_0
  { EA_NONE,    0 },  // EO_PUSH_1
  { EA_INT,     8 },  // EO_RANDOM
  { EA_STRING,  -1},  // EO_STRING
  { EA_TIME,    8 },  // EO_TIME
  { EA_TIMESTAMP,8 }, // EO_TIMESTAMP

  // relational operators
  { EA_NONE,    0 },  // EO_EQ_BCDBCD
  { EA_NONE,    0 },  // EO_EQ_BCDBOL,
  { EA_NONE,    0 },  // EO_EQ_BCDFLT
  { EA_NONE,    0 },  // EO_EQ_BCDINT
  { EA_NONE,    0 },  // EO_EQ_DTEDTE
  { EA_NONE,    0 },  // EO_EQ_DTETSP
  { EA_NONE,    0 },  // EO_EQ_FLTBCD
  { EA_NONE,    0 },  // EO_EQ_FLTFLT
  { EA_NONE,    0 },  // EO_EQ_FLTINT
  { EA_NONE,    0 },  // EO_EQ_INTBCD
  { EA_NONE,    0 },  // EO_EQ_INTFLT
  { EA_NONE,    0 },  // EO_EQ_INTINT
  { EA_NONE,    0 },  // EO_EQ_IVLIVL
  { EA_NONE,    0 },  // EO_EQ_TMETME
  { EA_NONE,    0 },  // EO_EQ_TSPDTE
  { EA_NONE,    0 },  // EO_EQ_TSPTSP
  { EA_NONE,    0 },  // EO_EQ_VCHVCH
  { EA_NONE,    0 },  // EO_EQ_UNKXYZ
  { EA_NONE,    0 },  // EO_EQ_XYZUNK

  { EA_NONE,    0 },  // EO_GE_BCDBCD
  { EA_NONE,    0 },  // EO_GE_BCDBOL,
  { EA_NONE,    0 },  // EO_GE_BCDFLT
  { EA_NONE,    0 },  // EO_GE_BCDINT
  { EA_NONE,    0 },  // EO_GE_DTEDTE
  { EA_NONE,    0 },  // EO_GE_DTETSP
  { EA_NONE,    0 },  // EO_GE_FLTBCD
  { EA_NONE,    0 },  // EO_GE_FLTFLT
  { EA_NONE,    0 },  // EO_GE_FLTINT
  { EA_NONE,    0 },  // EO_GE_INTBCD
  { EA_NONE,    0 },  // EO_GE_INTFLT
  { EA_NONE,    0 },  // EO_GE_INTINT
  { EA_NONE,    0 },  // EO_GE_IVLIVL
  { EA_NONE,    0 },  // EO_GE_TMETME
  { EA_NONE,    0 },  // EO_GE_TSPDTE
  { EA_NONE,    0 },  // EO_GE_TSPTSP
  { EA_NONE,    0 },  // EO_GE_VCHVCH
  { EA_NONE,    0 },  // EO_GE_UNKXYZ
  { EA_NONE,    0 },  // EO_GE_XYZUNK

  { EA_NONE,    0 },  // EO_GT_BCDBCD
  { EA_NONE,    0 },  // EO_GT_BCDFLT
  { EA_NONE,    0 },  // EO_GT_BCDINT
  { EA_NONE,    0 },  // EO_GT_BOLBOL
  { EA_NONE,    0 },  // EO_GT_DTEDTE
  { EA_NONE,    0 },  // EO_GT_DTETSP
  { EA_NONE,    0 },  // EO_GT_FLTBCD
  { EA_NONE,    0 },  // EO_GT_FLTFLT
  { EA_NONE,    0 },  // EO_GT_FLTINT
  { EA_NONE,    0 },  // EO_GT_INTBCD
  { EA_NONE,    0 },  // EO_GT_INTFLT
  { EA_NONE,    0 },  // EO_GT_INTINT
  { EA_NONE,    0 },  // EO_GT_IVLIVL
  { EA_NONE,    0 },  // EO_GT_TMETME
  { EA_NONE,    0 },  // EO_GT_TSPDTE
  { EA_NONE,    0 },  // EO_GT_TSPTSP
  { EA_NONE,    0 },  // EO_GT_VCHVCH
  { EA_NONE,    0 },  // EO_GT_UNKXYZ
  { EA_NONE,    0 },  // EO_GT_XYZUNK

  { EA_NONE,    0 },  // EO_LE_BCDBCD
  { EA_NONE,    0 },  // EO_LE_BCDBOL,
  { EA_NONE,    0 },  // EO_LE_BCDFLT
  { EA_NONE,    0 },  // EO_LE_BCDINT
  { EA_NONE,    0 },  // EO_LE_DTEDTE
  { EA_NONE,    0 },  // EO_LE_DTETSP
  { EA_NONE,    0 },  // EO_LE_FLTBCD
  { EA_NONE,    0 },  // EO_LE_FLTFLT
  { EA_NONE,    0 },  // EO_LE_FLTINT
  { EA_NONE,    0 },  // EO_LE_INTBCD
  { EA_NONE,    0 },  // EO_LE_INTFLT
  { EA_NONE,    0 },  // EO_LE_INTINT
  { EA_NONE,    0 },  // EO_LE_IVLIVL
  { EA_NONE,    0 },  // EO_LE_TMETME
  { EA_NONE,    0 },  // EO_LE_TSPDTE
  { EA_NONE,    0 },  // EO_LE_TSPTSP
  { EA_NONE,    0 },  // EO_LE_VCHVCH
  { EA_NONE,    0 },  // EO_LE_UNKXYZ
  { EA_NONE,    0 },  // EO_LE_XYZUNK

  { EA_NONE,    0 },  // EO_LT_BCDBCD
  { EA_NONE,    0 },  // EO_LT_BCDBOL,
  { EA_NONE,    0 },  // EO_LT_BCDFLT
  { EA_NONE,    0 },  // EO_LT_BCDINT
  { EA_NONE,    0 },  // EO_LT_DTEDTE
  { EA_NONE,    0 },  // EO_LT_DTETSP
  { EA_NONE,    0 },  // EO_LT_FLTBCD
  { EA_NONE,    0 },  // EO_LT_FLTFLT
  { EA_NONE,    0 },  // EO_LT_FLTINT
  { EA_NONE,    0 },  // EO_LT_INTBCD
  { EA_NONE,    0 },  // EO_LT_INTFLT
  { EA_NONE,    0 },  // EO_LT_INTINT
  { EA_NONE,    0 },  // EO_LT_IVLIVL
  { EA_NONE,    0 },  // EO_LT_TMETME
  { EA_NONE,    0 },  // EO_LT_TSPDTE
  { EA_NONE,    0 },  // EO_LT_TSPTSP
  { EA_NONE,    0 },  // EO_LT_VCHVCH
  { EA_NONE,    0 },  // EO_LT_UNKXYZ
  { EA_NONE,    0 },  // EO_LT_XYZUNK

  { EA_NONE,    0 },  // EO_NE_BCDBCD
  { EA_NONE,    0 },  // EO_NE_BCDBOL,
  { EA_NONE,    0 },  // EO_NE_BCDFLT
  { EA_NONE,    0 },  // EO_NE_BCDINT
  { EA_NONE,    0 },  // EO_NE_DTEDTE
  { EA_NONE,    0 },  // EO_NE_DTETSP
  { EA_NONE,    0 },  // EO_NE_FLTBCD
  { EA_NONE,    0 },  // EO_NE_FLTFLT
  { EA_NONE,    0 },  // EO_NE_FLTINT
  { EA_NONE,    0 },  // EO_NE_INTBCD
  { EA_NONE,    0 },  // EO_NE_INTFLT
  { EA_NONE,    0 },  // EO_NE_INTINT
  { EA_NONE,    0 },  // EO_NE_IVLIVL
  { EA_NONE,    0 },  // EO_NE_TMETME
  { EA_NONE,    0 },  // EO_NE_TSPDTE
  { EA_NONE,    0 },  // EO_NE_TSPTSP
  { EA_NONE,    0 },  // EO_NE_VCHVCH
  { EA_NONE,    0 },  // EO_NE_UNKXYZ
  { EA_NONE,    0 },  // EO_NE_XYZUNK

    // column pushes
  { EA_INT,     8 },  // EO_PUSH_COLUMN_LENGTH
  { EA_INT,     8 },  // EO_PUSH_COLUMN_SCALE
  { EA_INT,     8 },  // EO_PUSH_COLUMN_TYPE

  // ixt device pushes
  { EA_NONE,    0 },  // EO_PUSH_DEVIXT_ALT
  { EA_NONE,    0 },  // EO_PUSH_DEVIXT_ENT
  { EA_NONE,    0 },  // EO_PUSH_DEVIXT_LFT
  { EA_NONE,    0 },  // EO_PUSH_DEVIXT_RHT
  { EA_NONE,    0 },  // EO_PUSH_DEVIXT_TMP

  // row device pushes
  { EA_NONE,    0 },  // EO_PUSH_DEVROW_ALT
  { EA_NONE,    0 },  // EO_PUSH_DEVROW_DST
  { EA_NONE,    0 },  // EO_PUSH_DEVROW_LFT
  { EA_NONE,    0 },  // EO_PUSH_DEVROW_RHT

  // rlu pushes
  { EA_NONE,    0 },  // EO_PUSH_RLU_COLSET
  { EA_NONE,    0 },  // EO_PUSH_RLU_ENT
  { EA_NONE,    0 },  // EO_PUSH_RLU_ENTWRK
  { EA_NONE,    0 },  // EO_PUSH_RLU_INDEXDEF
  { EA_NONE,    0 },  // EO_PUSH_RLU_INDEXTREE
  { EA_NONE,    0 },  // EO_PUSH_RLU_JOINEDROW
  { EA_NONE,    0 },  // EO_PUSH_RLU_JOINMAP_BYTE
  { EA_NONE,    0 },  // EO_PUSH_RLU_LOCALS
  { EA_NONE,    0 },  // EO_PUSH_RLU_POT
  { EA_NONE,    0 },  // EO_PUSH_RLU_PTN
  { EA_NONE,    0 },  // EO_PUSH_RLU_ROWPIX

  // table
  { EA_NONE, 0 },  // EO_TABLEPTN_DROPREAD
  { EA_NONE, 0 },  // EO_TABLEPTN_GRABREAD

  // stack operators
  { EA_NONE,    0 },  // EO_INC
  { EA_NONE,    0 },  // EO_DEC
  { EA_NONE,    0 },  // EO_DUP
  { EA_NONE,    0 },  // EO_DUP_STR
  { EA_NONE,    0 },  // EO_POP
  { EA_INT,     8 },  // EO_POP_LOCALI
  { EA_INT,     8 },  // EO_POP_LOCALS
  { EA_NONE,    0 },  // EO_POP_STR
  { EA_INT,     8 },  // EO_PUSH_LOCALI
  { EA_INT,     8 },  // EO_PUSH_LOCALS

  // string operators
  { EA_NONE,    0 },  // EO_STR_COMPARE
  { EA_NONE,    0 },  // EO_STR_COMPARE_a
  { EA_NONE,    0 },  // EO_STR_PRINTADR
  { EA_NONE,    0 },  // EO_STR_PRINTSTK
  { EA_STRING, -1 },  // EO_STR_LITERAL

  // subroutine operators
  { EA_LABEL,   8 },  // EO_CALL
  { EA_NONE,    0 },  // EO_RETURN
  { EA_NONE,    0 },  // EO_RETURN_FALSE
  { EA_NONE,    0 },  // EO_RETURN_TRUE
  { EA_NONE,    0 },  // EO_RETURN_VALUE

  // misc
  { EA_NONE,    0 },  // EO_ASSERT
  { EA_NONE,    0 },  // EO_BREAKPOINT
  { EA_NONE,    0 },  // EO_CALLAGAIN
  { EA_NONE,    0 },  // EO_CALLEDAGAIN
  { EA_NONE,    0 },  // EO_DUMP
  { EA_NONE,    0 },  // EO_ENTER_LU
  { EA_NONE,    0 },  // EO_EXIT_LU
  { EA_STRING,  -1 }, // EO_HANDLER
  { EA_NONE,    0 },  // EO_INIT
  { EA_NONE,    0 },  // EO_INTEGRITY
  { EA_NONE,    0 },  // EO_NEVERREACH
  { EA_NONE,    0 },  // EO_SANITY
  { EA_NONE,    0 },  // EO_SLEEP
  { EA_INT,     8 },  // EO_SQL
  { EA_NONE,    0 },  // EO_TABLEPTN
  { EA_NONE,    0 },  // EO_TRACEOFF
  { EA_NONE,    0 },  // EO_TRACEON
  { EA_NONE,    0 },  // EO_NOOP
  { EA_NONE,    0 },  // EO_ERROR

  //----------------------------------------------------------------------------

  // date operators
  { EA_NONE,    0 },  // EO_DTE_NOW
  { EA_NONE,    0 },  // EO_DTE_PRINT
  { EA_NONE,    0 },  // EO_DTE_YEAR

  // hashing
  { EA_NONE,    0 },  // EO_HSH_ADD_HASHI
  { EA_NONE,    0 },  // EO_HSH_RESET

  // partitioning
  { EA_NONE,    0 },  // EO_PTN_CALC
  { EA_NONE,    0 },  // EO_PTN_PUSH
  { EA_NONE,    0 },  // EO_PTN_RESET
  { EA_NONE,    0 },  // EO_PTN_RUNNING

    // timer
  { EA_NONE,    0 },  // EO_TMR_SINCE
  { EA_NONE,    0 },  // EO_TMR_START
  { EA_NONE,    0 },  // EO_TMR_NSECS
  { EA_NONE,    0 },  // EO_TMR_UPDATE
  { EA_NONE,    0 },  // EO_TMR_AGG_ADD
  { EA_NONE,    0 },  // EO_TMR_AGG_NSECS
  { EA_NONE,    0 },  // EO_TMR_AGG_RESET

  //----------------------------------------------------------------------------

  // rlu column set
  { EA_INT,      8 },  // EO_RLU_SET_COLUMN
  { EA_INT,      8 },  // EO_RLU_SET_COLUMNS
  { EA_INT,      8 },  // EO_RLU_SET_ITEMS

    // rlu joinedRow
  { EA_NONE,     0 },  // EO_RLU_RESET_JOINEDROW
  { EA_NONE,     0 },  // EO_RLU_SET_JOINEDROW

  // rlu joinmap set
  { EA_NONE,     0 },  // EO_RLU_RESET_JOINMAP
  { EA_NONE,     0 },  // EO_RLU_SET_JOINMAP_BYTE

  // rlu operators
  { EA_NONE,    0 },  // EO_RLU_INDEXDEF_ADDCOLUMN
  { EA_NONE,    0 },  // EO_RLU_INDEXDEF_DELETE
  { EA_NONE,    0 },  // EO_RLU_INDEXDEF_NEW
  { EA_NONE,    0 },  // EO_RLU_INDEXTREE_NEW
  { EA_NONE,    0 },  // EO_RLU_ROWPIX_PRINT
  { EA_NONE,    0 },  // EO_RLU_POT_COMPLETED

  // ixt device col operators
  { EA_NONE,    0 },  // EO_DEVROW_COLS_PUSHSET_PST
  { EA_NONE,    0 },  // EO_DEVIXT_COLS_TO_LOCALS
  { EA_NONE,    0 },  // EO_DEVIXT_COLS_TO_ROW
  { EA_NONE,    0 },  // EO_DEVIXT_COLS_TO_ROW_OFS
  { EA_NONE,    0 },  // EO_DEVROW_COLS_TO_LOCALS
  { EA_NONE,    0 },  // EO_DEVROW_COLS_TO_ROW
  { EA_NONE,    0 },  // EO_DEVROW_COLS_TO_ROW_OFS
  { EA_NONE,    0 },  // EO_DEVROW_NULS_TO_ROW_OFS

  // ixt device crwbuffer operators
  { EA_NONE,    0 },  // EO_DEVIXT_CRWBUF_BEGIN
  { EA_NONE,    0 },  // EO_DEVIXT_CRWBUF_FINDROW
  { EA_NONE,    0 },  // EO_DEVIXT_CRWBUF_NEXT
  { EA_NONE,    0 },  // EO_DEVIXT_CRWBUF_NULL
  { EA_NONE,    0 },  // EO_DEVIXT_CRWBUF_ROWCNT
  { EA_NONE,    0 },  // EO_DEVIXT_CRWBUF_ROWNO
  { EA_NONE,    0 },  // EO_DEVIXT_CRWBUF_TEST

  // ixt device entry relational operators
  { EA_NONE,    0 },  // EO_DEVIXT_RNT_DEREF
  { EA_NONE,    0 },  // EO_DEVIXT_ENT_FINDI
  { EA_NONE,    0 },  // EO_DEVIXT_ENT_NEXT
  { EA_NONE,    0 },  // EO_DEVIXT_ENT_POSPRIOR
  { EA_NONE,    0 },  // EO_DEVIXT_ENT_PUSH
  { EA_NONE,    0 },  // EO_DEVIXT_ENT_PUSHELMI0
  { EA_NONE,    0 },  // EO_DEVIXT_ENT_PUSHELMS0
  { EA_NONE,    0 },  // EO_DEVIXT_ENT_PUSHUPPER

  // ixt device  relational operators
  { EA_NONE,    0 },  // EO_DEVIXT_ENTKEY_LESS
  { EA_NONE,    0 },  // EO_DEVIXT_ENTKEY_LESSEQUAL
  { EA_NONE,    0 },  // EO_DEVIXT_ENTKEY_EQUAL
  { EA_NONE,    0 },  // EO_DEVIXT_ENTKEY_NOTEQUAL
  { EA_NONE,    0 },  // EO_DEVIXT_ENTKEY_GREATEREQUAL
  { EA_NONE,    0 },  // EO_DEVIXT_ENTKEY_GREATER
  { EA_NONE,    0 },  // EO_DEVIXT_ENTKEY_PRINTKEYS

  // ixt device entwrk operators
  { EA_NONE,    0 },  // EO_ENTWRK_PRINT
  { EA_INT,     8 },  // EO_ENTWRK_PUSHELMI
  { EA_INT,     8 },  // EO_ENTWRK_PUSHELMS
  { EA_NONE,    0 },  // EO_ENTWRK_RESET
  { EA_NONE,    0 },  // EO_ENTWRK_SETENTKND
  { EA_NONE,    0 },  // EO_ENTWRK_SETINTELM
  { EA_NONE,    0 },  // EO_ENTWRK_SETSTZELM

  // ixt device index operators
  { EA_NONE,    0 },  // EO_DEVIXT_IDX_DEL
  { EA_NONE,    0 },  // EO_DEVIXT_IDX_DUMP
  { EA_NONE,    0 },  // EO_DEVIXT_IDX_INS
  { EA_NONE,    0 },  // EO_DEVIXT_IDX_BEGIN
  { EA_NONE,    0 },  // EO_DEVIXT_IDX_NEXT
  { EA_NONE,    0 },  // EO_DEVIXT_IDX_ENTRIES

  { EA_NONE,    0 },  // EO_DEVIXT_LBOUND_GE
  { EA_NONE,    0 },  // EO_DEVIXT_LBOUND_GT
  { EA_NONE,    0 },  // EO_DEVIXT_UBOUND_LE
  { EA_NONE,    0 },  // EO_DEVIXT_UBOUND_LT
  { EA_NONE,    0 },  // EO_DEVIXT_CMP_KEYS
  { EA_NONE,    0 },  // EO_DEVIXT_POS_BYKEY
  { EA_NONE,    0 },  // EO_DEVIXT_ENT_PRINT

  // ixt device index set operators
  { EA_STRING, -1 },  // EO_DEVIXT_INDEXNM_SET
  { EA_NONE,    0 },  // EO_DEVIXT_INDEXTREE_SET

  // row device entWrk
  { EA_NONE,    0 },  // EO_DEVROW_ENTWRK_APPENDELMI
  { EA_NONE,    0 },  // EO_DEVROW_ENTWRK_APPENDELMS
  { EA_INT,     8 },  // EO_DEVROW_ENTWRK_CREATEPAY
  { EA_STRING, -1 },  // EO_DEVROW_TABLENM_SET

  // row device row operators
  { EA_NONE,    0 },  // EO_DEVROW_ROW_NEW
  { EA_NONE,    0 },  // EO_DEVROW_ROW_PRINT
  { EA_NONE,    0 },  // EO_DEVROW_ROW_SET
  { EA_NONE,    0 },  // EO_DEVROW_ROW_TEST
  { EA_NONE,    0 },  // EO_DEVROW_ROW_ZERO

  // row device row buffer operators
  { EA_NONE,    0 },  // EO_DEVROW_ROWBUF_BEGIN
  { EA_NONE,    0 },  // EO_DEVROW_ROWBUF_FINDROW
  { EA_NONE,    0 },  // EO_DEVROW_ROWBUF_NEXT
  { EA_NONE,    0 },  // EO_DEVROW_ROWBUF_NULL
  { EA_NONE,    0 },  // EO_DEVROW_ROWBUF_RESET
  { EA_NONE,    0 },  // EO_DEVROW_ROWBUF_ROWCNT
  { EA_NONE,    0 },  // EO_DEVROW_ROWBUF_ROWNO
  { EA_NONE,    0 },  // EO_DEVROW_ROWBUF_TEST

  //  row device column operators
  { EA_SUBSCR,  8 },  // EO_DEVROW_POPCOLI
  { EA_SUBSCR,  8 },  // EO_DEVROW_POPCOLS

  { EA_SUBSCR,  8 },  // EO_DEVROW_PUSHCOLI
  { EA_SUBSCR,  8 },  // EO_DEVROW_PUSHCOLS

  // pay load
  { EA_NONE,    0 },  // EO_PAY_ADDRESS
  { EA_SUBSCR,  8 },  // EO_PAY_FETCH_COLI
  { EA_SUBSCR,  8 },  // EO_PAY_FETCH_COLS
  { EA_SUBSCR,  8 },  // EO_PAY_STORE_COLI
  { EA_SUBSCR,  8 },  // EO_PAY_STORE_COLS

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  // ---TO GO ---

  // setintelm old
  { EA_NONE,    0 },  // EO_ENTWRK_SETINTELM_OLD

  // dev column
  { EA_NONE,    0 },  // EO_DEVIXT_COL_PUSHSET_ADR
  { EA_NONE,    0 },  // EO_DEVIXT_COL_PUSHSET_PST

  // key
  { EA_INT,     8 },  // EO_KEY_LCL2C_TO_I
  { EA_INT,     8 },  // EO_KEY_ROW2C_TO_I

    // pot
  { EA_INT,     8 },  // EO_POT_USE_STREAMDEFOUT

    // register operators
  { EA_NONE,    0 },  // EO_REG_EOSMARKER_NEW
  { EA_NONE,    0},   // EO_REG_PTNSTREAM_CLOSE
  { EA_NONE,    0},   // EO_REG_PTNSTREAM_DELETE
  { EA_NONE,    0},   // EO_REG_PTNSTREAM_NEW
  { EA_NONE,    0},   // EO_REG_PTNSTREAM_OPEN
  { EA_NONE,    0},   // EO_REG_PTNSTREAM_READ
  { EA_NONE,    0 },  // EO_REG_STREAMDEFOUT_NEW
  { EA_STRING, -1 },  // EO_REG_STREAMDEFOUT_ADDCOLUMN
  { EA_NONE,    0 },  // EO_REG_STREAMDEF_STREAMDEFOUT_ADDCOLUMN

  // super
  { EA_NONE,    0 },  // super fragment Q100

  { EA_NONE,    0 },  // EO_DEVIXT_LBOUND_GE
  { EA_NONE,    0 },  // EO_DEVIXT_UBOUND_LT

  // -- end --
  { EA_NONE,    0 },  // EO_ENUMS
};
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
