/*============================================================================*/
/* eEO.h -- Expression Operators                                              */
/*============================================================================*/
#pragma once

#include "eEA.h"

#ifdef __TINYC__
enum eEO // expression operators
#else
enum eEO : u1 // expression operators
#endif
{
  /*  0 */ EO_NONE=0,
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // simple eo

  // arithmetic operators
  /*  S  */ EO_ADD,              // add top two int's push result
  /*  S  */ EO_DIVIDE,           // divide top int by second push result
  /*  S  */ EO_MODULO,           // modulo top int be second push result
  /*  S  */ EO_MULTIPLY,         // multiply top two int's push result
  /*  S  */ EO_NEGATE,           // negate the top value
  /*  S  */ EO_SUBTRACT,         // subtract second int from top push result

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

  // relational operators
  /*  S  */ EO_LESSTHAN,         // test top < 2nd, push result
  /*  S  */ EO_LESSEQUAL,        // test top <= 2nd, push result
  /*  S  */ EO_EQUAL,            // test top == 2nd, push result
  /*  S  */ EO_NOTEQUAL,         // test top != 2nd, push result
  /*  S  */ EO_GREATEREQUAL,     // test top >= 2nd, push result
  /*  S  */ EO_GREATERTHAN,      // test top > 2nd, push result

  // column pushes
  /*  S  */ EO_PUSH_COLUMN_LENGTH, // push column length
  /*  S  */ EO_PUSH_COLUMN_SCALE,  // push column scale
  /*  S  */ EO_PUSH_COLUMN_TYPE,   // push column type

  // ixt device pushes
  /*  S  */ EO_PUSH_DEVIXT_ALT,     // push alt Ixt device
  /*  S  */ EO_PUSH_DEVIXT_ENT,     // push entry of ixt device
  /*  S  */ EO_PUSH_DEVIXT_SRC,     // push src Ixt device
  /*  S  */ EO_PUSH_DEVIXT_TMP,     // push tmp Ixt device

  // row device pushes
  /*  S  */ EO_PUSH_DEVROW_ALT,     // push alt row device
  /*  S  */ EO_PUSH_DEVROW_DST,     // push dst row device
  /*  S  */ EO_PUSH_DEVROW_SRC,     // push src row device

  // rlu pushes
  /*  S  */ EO_PUSH_RLU_COLSET,     // push colset from the rlu
  /*  S  */ EO_PUSH_RLU_ENT,        // push ent from the rlu
  /*  S  */ EO_PUSH_RLU_ENTWRK,     // push entwrk from the rlu
  /*  S  */ EO_PUSH_RLU_INDEXDEF,   // push indexdef from the rlu
  /*  S  */ EO_PUSH_RLU_INDEXTREE,  // push indextree from the rlu
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
  /*  S  */ EO_POP,              // pop / discard top
  /*  S  */ EO_POP_LOCALI,       // pop integer into a local register
  /*  S  */ EO_POP_LOCALS,       // pop string into a local register
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
  /*  S  */ EO_RETURN,           // return from subroutine and
  /*  S  */ EO_RETURN_FALSE,     // return from subroutine on false

  // misc
  /*  S  */ EO_ASSERT,           // pop top and assert
  /*  P  */ EO_BREAKPOINT,       // break point
  /*  R  */ EO_CALLAGAIN,        // set call again
  /*  R  */ EO_CALLEDAGAIN,      // push called again
  /*  R  */ EO_DUMP,             // stack dump
  /*  R  */ EO_ENTER,            // diarise routine enter
  /*  R  */ EO_EXIT,             // diarise routine exit
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

  // rlu operators
  /*  M  */ EO_RLU_INDEXDEF_ADDCOLUMN,     // add a column to indexDefRlu[]
  /*  M  */ EO_RLU_INDEXDEF_DELETE,        // delete the indexDefRlu[]
  /*  M  */ EO_RLU_INDEXDEF_NEW,           // new an indexDef in indexDefRlu[]
  /*  M  */ EO_RLU_INDEXTREE_NEW,          // new an indexTree in indexTreeRlu[]
  /*  M  */ EO_RLU_ROWPIX_PRINT,           // print row pix
  /*  M  */ EO_RLU_POT_COMPLETED,          // do we have a full pot buffer ?

  // ixt device col operators
  /*  S  */ EO_DEVIXT_COLS_TO_LOCALS,    // inflate dev columns direct into locals
  /*  S  */ EO_DEVIXT_COLS_TO_ROW,       // push all columns from ixt dev to row dev
  /*  S  */ EO_DEVROW_COLS_TO_LOCALS,     // row columns into locals

  // ixt device crwbuffer operators
  /*  S  */ EO_DEVIXT_CRWBUF_BEGIN, // begin crw buffer traverse
  /*  S  */ EO_DEVIXT_CRWBUF_NEXT,  // move to next row in crw buffer
  /*  S  */ EO_DEVIXT_CRWBUF_NULL,  // null crw buffer
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
  /*    */ EO_ENTWRK_RESET,     // reset m_oEntWrk
  /*    */ EO_ENTWRK_SETINTELM, // set int only element value

  // ixt device index operators
  /*  S  */ EO_DEVIXT_IDX_DEL,      // delete current m_aEnt from indexTree
  /*  S  */ EO_DEVIXT_IDX_DUMP,     // dump index tree
  /*  S  */ EO_DEVIXT_IDX_INS,      // insert an entry into the indexTree
  /*  S  */ EO_DEVIXT_IDX_BEGIN,    // begin linear traverse of indexTree
  /*  S  */ EO_DEVIXT_IDX_NEXT,     // traverse to next entry
  /*  S  */ EO_DEVIXT_IDX_ENTRIES,  // push number of entries in index

  /*  D  */ EO_DEVIXT_LBOUND,    // position at lower bound
  /*  D  */ EO_DEVIXT_UBOUND,    // get upper bound
  /*  D  */ EO_DEVIXT_UBOUND_LT, // get upper bound (less than)
  /*  D  */ EO_DEVIXT_CMP_KEYS,  // compare keys of two devices
  /*  D  */ EO_DEVIXT_POS_BYKEY, // position dev ixt by key
  /*  D  */ EO_DEVIXT_ENT_PRINT, // print ent

  // ixt device index set operators
  /*  S  */ EO_DEVIXT_INDEXNM_SET,      // set the index name in the dev
  /*  S  */ EO_DEVIXT_INDEXTREE_SET,    // set the indexTree in the ixt dev
  /*  S  */ EO_DEVIXT_INDEXTREE_DELETE, // delete the indexTree in the ixt dev

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
  /*     */ EO_DEVROW_ROWBUF_NEXT,   // move to next src row in row dev
  /*     */ EO_DEVROW_ROWBUF_NULL,   // null row buffer in the row dev
  /*     */ EO_DEVROW_ROWBUF_RESET,  // reset rowslab in row dev
  /*     */ EO_DEVROW_ROWBUF_TEST,   // test if buffer in row dev

  // row device column operators
  /*    */ EO_DEVROW_POPCOLI,  // pop and deposit top into column
  /*    */ EO_DEVROW_POPCOLS,  // pop and deposit top string into column
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
  /*  M  */ EO_REG_INDEXDEF_ADDCOLUMN,     // add a column to indexDef[]
  /*  M  */ EO_REG_PTNSTREAM_CLOSE,        // close a ptn stream
  /*  M  */ EO_REG_PTNSTREAM_DELETE,       // delete a ptn stream
  /*  M  */ EO_REG_PTNSTREAM_NEW,          // new a ptn stream
  /*  M  */ EO_REG_PTNSTREAM_OPEN,         // open a ptn stream
  /*  M  */ EO_REG_PTNSTREAM_READ,         // read a ptn stream
  /*  M  */ EO_REG_STREAMDEFOUT_NEW,       // new an streamDef in streamDefOut[]        // OLD QUERIES (will go)
  /*  M  */ EO_REG_STREAMDEFOUT_ADDCOLUMN, // add a column to streamDefOut[]            // OLD QUERIES (will go)
  /*  M  */ EO_REG_STREAMDEF_STREAMDEFOUT, // copy streamDef[] to streamDefOut[]        // OLD QUERIES (will go)

  // super fragments
  /*  M  */ EO_SUPER_Q100,       // super fragment Q100

  /*    */ EO_ENUMS
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

#ifndef __TINYC__

static Stz* aEO[]
{
  /*  0 */ "NONE"_Stz,
  
  // arithmetic operators
  /*    */ "EO_ADD"_Stz,
  /*    */ "EO_DIVIDE"_Stz,
  /*    */ "EO_MODULO"_Stz,
  /*    */ "EO_MULTIPLY"_Stz,
  /*    */ "EO_NEGATE"_Stz,
  /*    */ "EO_SUBTRACT"_Stz,

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
  
  // relational operators
  /*    */ "EO_LESSTHAN"_Stz,
  /*    */ "EO_LESSEQUAL"_Stz,
  /*    */ "EO_EQUAL"_Stz,
  /*    */ "EO_NOTEQUAL"_Stz,
  /*    */ "EO_GREATEREQUAL"_Stz,
  /*    */ "EO_GREATERTHAN"_Stz,

  // column pushes
  /*    */ "EO_PUSH_COLUMN_LENGTH"_Stz,
  /*    */ "EO_PUSH_COLUMN_SCALE"_Stz,
  /*    */ "EO_PUSH_COLUMN_TYPE"_Stz,

  // ixt device pushes
  /*    */ "EO_PUSH_DEVIXT_ALT"_Stz,
  /*    */ "EO_PUSH_DEVIXT_ENT"_Stz,
  /*    */ "EO_PUSH_DEVIXT_SRC"_Stz,
  /*    */ "EO_PUSH_DEVIXT_TMP"_Stz,

  // row device pushes
  /*    */ "EO_PUSH_DEVROW_ALT"_Stz,
  /*    */ "EO_PUSH_DEVROW_DST"_Stz,
  /*    */ "EO_PUSH_DEVROW_SRC"_Stz,

  // rlu pushes
  /*    */ "EO_PUSH_RLU_COLSET"_Stz,
  /*    */ "EO_PUSH_RLU_ENT"_Stz,
  /*    */ "EO_PUSH_RLU_ENTWRK"_Stz,
  /*    */ "EO_PUSH_RLU_INDEXDEF"_Stz,
  /*    */ "EO_PUSH_RLU_INDEXTREE"_Stz,
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
  /*    */ "EO_POP"_Stz,
  /*    */ "EO_POP_LOCALI"_Stz,
  /*    */ "EO_POP_LOCALS"_Stz,
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

  // misc
  /*    */ "EO_ASSERT"_Stz,
  /*    */ "EO_BREAKPOINT"_Stz,
  /*    */ "EO_CALLAGAIN"_Stz,
  /*    */ "EO_CALLEDAGAIN"_Stz,
  /*    */ "EO_DUMP"_Stz,
  /*    */ "EO_ENTER"_Stz,
  /*    */ "EO_EXIT"_Stz,
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

  // rlu operators
  /*    */ "EO_RLU_INDEXDEF_ADDCOLUMN"_Stz,
  /*    */ "EO_RLU_INDEXDEF_DELETE"_Stz,
  /*    */ "EO_RLU_INDEXDEF_NEW"_Stz,
  /*    */ "EO_RLU_INDEXTREE_NEW"_Stz,
  /*    */ "EO_RLU_ROWPIX_PRINT"_Stz,
  /*    */ "EO_RLU_POT_COMPLETED"_Stz,

  // ixt device col operators
  /*    */ "EO_DEVIXT_COLS_TO_LOCALS"_Stz,
  /*    */ "EO_DEVIXT_COLS_TO_ROW"_Stz,
  /*    */ "EO_DEVROW_COLS_TO_LOCALS"_Stz,

  // ixt device crwbuffer operators
  /*    */ "EO_DEVIXT_CRWBUF_BEGIN"_Stz,
  /*    */ "EO_DEVIXT_CRWBUF_NEXT"_Stz,
  /*    */ "EO_DEVIXT_CRWBUF_NULL"_Stz,
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
  /*    */ "EO_ENTWRK_RESET"_Stz,
  /*    */ "EO_ENTWRK_SETINTELM"_Stz,

  // ixt device index operators
  /*    */ "EO_DEVIXT_IDX_DEL"_Stz,
  /*    */ "EO_DEVIXT_IDX_DUMP"_Stz,
  /*    */ "EO_DEVIXT_IDX_INS"_Stz,
  /*    */ "EO_DEVIXT_IDX_BEGIN"_Stz,
  /*    */ "EO_DEVIXT_IDX_NEXT"_Stz,
  /*    */ "EO_DEVIXT_IDX_ENTRIES"_Stz,

  /*    */ "EO_DEVIXT_LBOUND"_Stz,
  /*    */ "EO_DEVIXT_UBOUND"_Stz,
  /*    */ "EO_DEVIXT_UBOUND_LT"_Stz,
  /*    */ "EO_DEVIXT_CMP_KEYS"_Stz,
  /*    */ "EO_DEVIXT_POS_BYKEY"_Stz,
  /*    */ "EO_DEVIXT_ENT_PRINT"_Stz,

  // ixt device index set operators
  /*    */ "EO_DEVIXT_INDEXNM_SET"_Stz,
  /*    */ "EO_DEVIXT_INDEXTREE_SET"_Stz,
  /*    */ "EO_DEVIXT_INDEXTREE_DELETE"_Stz,

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
  /*     */ "EO_DEVROW_ROWBUF_NEXT"_Stz,
  /*     */ "EO_DEVROW_ROWBUF_NULL"_Stz,
  /*     */ "EO_DEVROW_ROWBUF_RESET"_Stz,
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
  /*    */ "EO_REG_INDEXDEF_ADDCOLUMN"_Stz,
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
  { EA_NONE,    0 },  // EO_ADD
  { EA_NONE,    0 },  // EO_DIVIDE
  { EA_NONE,    0 },  // EO_MODULO
  { EA_NONE,    0 },  // EO_MULTIPLY
  { EA_NONE,    0 },  // EO_NEGATE
  { EA_NONE,    0 },  // EO_SUBTRACT

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
  { EA_NONE,    0 },  // EO_ISNOTNULL

  // prints
  { EA_NONE,    0 },  // EO_PRINT
  { EA_STRING, -1 },  // EO_PRINTF

  // pushes
  { EA_DATE,    8 },  // EO_DATE
  { EA_INT,     8 },  // EO_INTEGER
  { EA_NONE,    0 },  // EO_PUSHNULL
  { EA_NONE,    0 },  // EO_PUSH_MINUS1
  { EA_NONE,    0 },  // EO_PUSH_0
  { EA_NONE,    0 },  // EO_PUSH_1
  { EA_INT,     8 },  // EO_RANDOM
  { EA_STRING,  -1},  // EO_STRING

  // relational operators
  { EA_NONE,    0 },  // EO_LESSTHAN
  { EA_NONE,    0 },  // EO_LESSEQUAL
  { EA_NONE,    0 },  // EO_EQUAL
  { EA_NONE,    0 },  // EO_NOTEQUAL
  { EA_NONE,    0 },  // EO_GREATEREQUAL
  { EA_NONE,    0 },  // EO_GREATERTHAN

  // column pushes
  { EA_INT,     8 },  // EO_PUSH_COLUMN_LENGTH
  { EA_INT,     8 },  // EO_PUSH_COLUMN_SCALE
  { EA_INT,     8 },  // EO_PUSH_COLUMN_TYPE

  // ixt device pushes
  { EA_NONE,    0 },  // EO_PUSH_DEVIXT_ALT
  { EA_NONE,    0 },  // EO_PUSH_DEVIXT_ENT
  { EA_NONE,    0 },  // EO_PUSH_DEVIXT_SRC
  { EA_NONE,    0 },  // EO_PUSH_DEVIXT_TMP

  // row device pushes
  { EA_NONE,    0 },  // EO_PUSH_DEVROW_ALT
  { EA_NONE,    0 },  // EO_PUSH_DEVROW_DST
  { EA_NONE,    0 },  // EO_PUSH_DEVROW_SRC

  // rlu pushes
  { EA_NONE,    0 },  // EO_PUSH_RLU_COLSET
  { EA_NONE,    0 },  // EO_PUSH_RLU_ENT
  { EA_NONE,    0 },  // EO_PUSH_RLU_ENTWRK
  { EA_NONE,    0 },  // EO_PUSH_RLU_INDEXDEF
  { EA_NONE,    0 },  // EO_PUSH_RLU_INDEXTREE
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
  { EA_NONE,    0 },  // EO_POP
  { EA_INT,     8 },  // EO_POP_LOCALI
  { EA_INT,     8 },  // EO_POP_LOCALS
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

  // misc
  { EA_NONE,    0 },  // EO_ASSERT
  { EA_NONE,    0 },  // EO_BREAKPOINT
  { EA_NONE,    0 },  // EO_CALLAGAIN
  { EA_NONE,    0 },  // EO_CALLEDAGAIN
  { EA_NONE,    0 },  // EO_DUMP
  { EA_NONE,    0 },  // EO_ENTER
  { EA_NONE,    0 },  // EO_EXIT
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
  { EA_INT,     8 },  // EO_RLU_SET_COLUMN
  { EA_INT,     8 },  // EO_RLU_SET_COLUMNS
  { EA_INT,     8 },  // EO_RLU_SET_ITEMS

    // rlu operators
  { EA_NONE,    0 },  // EO_RLU_INDEXDEF_ADDCOLUMN
  { EA_NONE,    0 },  // EO_RLU_INDEXDEF_DELETE
  { EA_NONE,    0 },  // EO_RLU_INDEXDEF_NEW
  { EA_NONE,    0 },  // EO_RLU_INDEXTREE_NEW
  { EA_NONE,    0 },  // EO_RLU_ROWPIX_PRINT
  { EA_NONE,    0 },  // EO_RLU_POT_COMPLETED

  // ixt device col operators
  { EA_NONE,    0 },  // EO_DEVIXT_COLS_TO_LOCALS
  { EA_NONE,    0 },  // EO_DEVIXT_COLS_TOROW
  { EA_NONE,    0 },  // EO_DEVROW_COLS_TO_LOCALS

  // ixt device crwbuffer operators
  { EA_NONE,    0 },  // EO_DEVIXT_CRWBUF_BEGIN
  { EA_NONE,    0 },  // EO_DEVIXT_CRWBUF_NEXT
  { EA_NONE,    0 },  // EO_DEVIXT_CRWBUF_NULL
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
  { EA_NONE,    0 },  // EO_ENTWRK_RESET
  { EA_NONE,    0 },  // EO_ENTWRK_SETINTELM

  // ixt device index operators
  { EA_NONE,    0 },  // EO_DEVIXT_IDX_DEL
  { EA_NONE,    0 },  // EO_DEVIXT_IDX_DUMP
  { EA_NONE,    0 },  // EO_DEVIXT_IDX_INS
  { EA_NONE,    0 },  // EO_DEVIXT_IDX_BEGIN
  { EA_NONE,    0 },  // EO_DEVIXT_IDX_NEXT
  { EA_NONE,    0 },  // EO_DEVIXT_IDX_ENTRIES

  { EA_NONE,    0 },  // EO_DEVIXT_LBOUND
  { EA_NONE,    0 },  // EO_DEVIXT_UBOUND
  { EA_NONE,    0 },  // EO_DEVIXT_UBOUND_LT
  { EA_NONE,    0 },  // EO_DEVIXT_CMP_KEYS
  { EA_NONE,    0 },  // EO_DEVIXT_POS_BYKEY
  { EA_NONE,    0 },  // EO_DEVIXT_ENT_PRINT

  // ixt device index set operators
  { EA_STRING, -1 },  // EO_DEVIXT_INDEXNM_SET
  { EA_NONE,    0 },  // EO_DEVIXT_INDEXTREE_SET
  { EA_NONE,    0 },  // EO_DEVIXT_INDEXTREE_DELETE

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
  { EA_NONE,    0 },  // EO_DEVROW_ROWBUF_NEXT
  { EA_NONE,    0 },  // EO_DEVROW_ROWBUF_NULL
  { EA_NONE,    0 },  // EO_DEVROW_ROWBUF_RESET
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
  { EA_STRING, -1 },  // EO_REG_INDEXDEF_ADDCOLUMN
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

    // -- end --
  { EA_NONE,    0 },  // EO_ENUMS
};
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
