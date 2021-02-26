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
  
  // column pushes
  /*  S  */ EO_PUSH_COLUMN_LENGTH, // push column length
  /*  S  */ EO_PUSH_COLUMN_SCALE,  // push column scale
  /*  S  */ EO_PUSH_COLUMN_TYPE,   // push column type

  // ixt device pushes
  /*  S  */ EO_PUSH_DEVIXT_ALT,     // push alt Ixt device
  /*  S  */ EO_PUSH_DEVIXT_SRC,     // push src Ixt device
  /*  S  */ EO_PUSH_DEVIXT_TMP,     // push tmp Ixt device

  // row device pushes
  /*  S  */ EO_PUSH_DEVROW_ALT,     // push alt row device
  /*  S  */ EO_PUSH_DEVROW_DST,     // push dst row device
  /*  S  */ EO_PUSH_DEVROW_SRC,     // push src row device

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
  
  // index
  /*  D  */ EO_INDEXNM_ALT_SET,    // set the alt index name
  /*  D  */ EO_INDEXNM_SRC_SET,    // set the src index name
  
  // indexTree
  /*  D  */ EO_INDEXTREE_ALT_SET,    // set the alt indexTree
  /*  D  */ EO_INDEXTREE_DEV_SET,    // set the dev to indexTree
  /*  D  */ EO_INDEXTREE_SRC_SET,    // set the src indexTree
  /*  D  */ EO_INDEXTREE_TMP_DELETE, // delete the tmp indexTree
  /*  D  */ EO_INDEXTREE_TMP_SET,    // set the tmp indexTree

  // indexTree
  /*  S  */ EO_INDEXTREE_SET,    // set the src indexTree
  /*  S  */ EO_INDEXTREE_DELETE, // delete the tmp indexTree

  // logical operators
  /*  S  */ EO_LGL_AND,          // logical and top two int's, push result
  /*  S  */ EO_LGL_OR,           // logical or top two int's, push result
  /*  S  */ EO_LGL_NOT,          // logical not top int, push result
  /*  S  */ EO_LGL_XOR,          // logical xor top two int's, push result

  // null test operators
  /*  S  */ EO_ISNULL,           // test if top is null, push result
  /*  S  */ EO_ISNOTNULL,        // test if top is not null, push result
  
  // pot
  /*  M  */ EO_POT_USE_STREAMDEFOUT, // use streamDefOut in the pot
  
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

  // relational operators
  /*  S  */ EO_LESSTHAN,         // test top < 2nd, push result
  /*  S  */ EO_LESSEQUAL,        // test top <= 2nd, push result
  /*  S  */ EO_EQUAL,            // test top == 2nd, push result
  /*  S  */ EO_NOTEQUAL,         // test top != 2nd, push result
  /*  S  */ EO_GREATEREQUAL,     // test top >= 2nd, push result
  /*  S  */ EO_GREATERTHAN,      // test top > 2nd, push result

  // register operators
  /*  M  */ EO_REG_EOSMARKER_NEW,          // new an eosMarker in Operand[]
  /*  M  */ EO_REG_INDEXDEF_ADDCOLUMN,     // add a column to indexDef[]
  /*  M  */ EO_REG_INDEXDEF_DELETE,        // delete the indexDef[]
  /*  M  */ EO_REG_INDEXDEF_NEW,           // new an indexDef in indexDef[]
  /*  M  */ EO_REG_INDEXTREE_NEW,          // new an indexTree in indexTree[]
  /*  M  */ EO_REG_INDEXTREE_NULL,         // null indexTree[]
  /*  M  */ EO_REG_PTNSTREAM_CLOSE,        // close a ptn stream
  /*  M  */ EO_REG_PTNSTREAM_DELETE,       // delete a ptn stream
  /*  M  */ EO_REG_PTNSTREAM_NEW,          // new a ptn stream
  /*  M  */ EO_REG_PTNSTREAM_OPEN,         // open a ptn stream
  /*  M  */ EO_REG_PTNSTREAM_READ,         // read a ptn stream
  /*  M  */ EO_REG_SRC_STREAMDEFOUT_NEW,   // new streamDefOut[] from DevSrcRow
  /*  M  */ EO_REG_STREAMDEFOUT_NEW,       // new an streamDef in streamDefOut[]
  /*  M  */ EO_REG_STREAMDEFOUT_ADDCOLUMN, // add a column to streamDefOut[]
  /*  M  */ EO_REG_STREAMDEF_STREAMDEFOUT, // copy streamDef[] to streamDefOut[]

  // register pushes
  /*  M  */ EO_PUSH_REG_INDEXTREE,         // push indexTree[]

  // column set
  /*  M  */ EO_SET_COLUMN,       // add column to set
  /*  M  */ EO_SET_COLUMNS,      // add columns to set
  /*  M  */ EO_SET_ITEMS,        // set number of column items
  /*  D  */ EO_SET_LOCALS_ALT,   // inflate alt columns direct into locals
  /*  D  */ EO_SET_LOCALS_ROW,   // row columns into locals
  /*  D  */ EO_SET_LOCALS_SRC,   // inflate src columns direct into locals
  /*  D  */ EO_SET_LOCALS_SRC2,  // inflate src columns direct into locals

  // table
  /*  D  */ EO_TABLENM_ALT_SET,    // set the alt table name
  /*  D  */ EO_TABLENM_SRC_SET,    // set the src table name

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

  // key
  /*  M  */ EO_KEY_LCL2C_TO_I,   // convert two local varchar(1) to int
  /*  D  */ EO_KEY_ROW2C_TO_I,   // convert two row varchar(1) to int

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
  
  // col operators
  /*  D  */ EO_COL_SRC_PUSHDST,  // push column subset from m_aSrcCrw into dst row
  /*  D  */ EO_COL_ALT_PUSHONE,  // push column value from m_aAltCrw
  /*  D  */ EO_COL_ALT_PUSHSET,  // push column subset from m_aAltCrw
  /*  D  */ EO_COL_SRC_PUSHONE,  // push column value from m_aSrcCrw
  /*  D  */ EO_COL_SRC_PUSHONE_a,// push value from m_aSrcCrw (str as ptr)
  /*  D  */ EO_COL_SRC_PUSHSET,  // push column subset from m_aSrcCrw

  // crw operators
  /*  D  */ EO_CRW_ALT_DEREF,    // deref m_aAltCrw using m_aAltEnt.ref
  /*  D  */ EO_CRW_SRC_BEGIN,    // begin src crw traverse
  /*  D  */ EO_CRW_SRC_DEREF,    // deref m_aSrcCrw using m_aSrcEnt.ref
  /*  D  */ EO_CRW_SRC_NEXT,     // move to next src row
  /*  D  */ EO_CRW_SRC_NULL,     // null crw src
  /*  D  */ EO_CRW_SRC_TEST,     // test if there is a crw buffer
  
  // entry
  /*  D  */ EO_ENT_FINDI,        // find entry with int key
  /*  D  */ EO_ENT_NEXT,         // move to next m_pEnt
  /*  D  */ EO_ENT_POSPRIOR,     // position m_pEnt prior using m_oEntWrk.key
  /*  D  */ EO_ENT_PUSH,         // push entry
  /*  D  */ EO_ENT_PUSHELMI0,    // push entry elm 0 int
  /*  D  */ EO_ENT_PUSHELMS0,    // push entry elm 0 str
  /*  D  */ EO_ENT_PUSHOFS,      // push entry offset
  /*  -  */ EO_ENT_PUSHREF,      // push ref of current entry

  /*  D  */ EO_ENT_SRC_PUSH,     // push src entry
  /*  D  */ EO_ENT_SRC_PUSHUPPER,// push src upper entry
  
  // entry relational operators
  /*  D  */ EO_ENT_LESS,         // test if m_pEnt.key <  m_oEntWrk.key
  /*  D  */ EO_ENT_LESSEQUAL,    // test if m_pEnt.key <= m_oEntWrk.key
  /*  D  */ EO_ENT_EQUAL,        // test if m_pEnt.key == m_oEntWrk.key
  /*  D  */ EO_ENT_NOTEQUAL,     // test if m_pEnt.key != m_oEntWrk.key
  /*  D  */ EO_ENT_GREATEREQUAL, // test if m_pEnt.key >= m_oEntWrk.key
  /*  D  */ EO_ENT_GREATER,      // test if m_pEnt.key >  m_oEntWrk.key
  /*  D  */ EO_ENT_PRINTKEYS,    // print both m_pEnt.key and m_oEntWrk.key

  // entWrk
  /*  D  */ EO_ENTWRK_APPENDELM, // append next element value
  /*  D  */ EO_ENTWRK_APPENDELMI,// append next element value (int)
  /*  D  */ EO_ENTWRK_APPENDELMS,// append next element value (str)
  /*  D  */ EO_ENTWRK_CREATEPAY, // create a payload in entwrk
  /*  D  */ EO_ENTWRK_POPOFS,    // pop m_oEntWrk.ref.buffer/offset
  /*  D  */ EO_ENTWRK_POPREF,    // pop m_oEntWrk.ref
  /*  D  */ EO_ENTWRK_PRINT,     // print m_oEntWrk
  /*  R  */ EO_ENTWRK_RESET,     // reset m_oEntWrk
  /*  R  */ EO_ENTWRK_SETINTELM, // set int only element value

  // index tree
  /*  D  */ EO_IXT_DEL_ENT,      // delete current m_aEnt from indexTree
  /*  D  */ EO_IXT_DUMP,         // dump index tree
  /*  D  */ EO_IXT_INS_ENT,      // insert an entry into the indexTree
  /*  D  */ EO_IXT_TRV_BEGIN,    // begin linear traverse
  /*  D  */ EO_IXT_TRV_NEXT,     // traverse to next entry
  /*  D  */ EO_IXT_TMP_ENTRIES,  // push number of entries in tmp index

  /*  D  */ EO_IXT_SRC_BEGIN,    // begin linear traverse
  /*  D  */ EO_IXT_SRC_LBOUND,   // position at lower bound
  /*  D  */ EO_IXT_SRC_NEXT,     // move to next entry
  /*  D  */ EO_IXT_SRC_PRINT,    // print src ent
  /*  D  */ EO_IXT_SRC_UBOUND,   // get upper bound
  /*  D  */ EO_IXT_SRC_UBOUND_LT, // get upper bound (less than)
  /*  D  */ EO_IXT_ALT_BEGIN,    // begin linear traverse
  /*  D  */ EO_IXT_ALT_NEXT,     // move to next entry
  /*  D  */ EO_IXT_ALT_PRINT,    // print src ent
  /*  D  */ EO_IXT_CMP_SRCALT,   // compare src.key to alt.key
  /*  D  */ EO_IXT_CMP_WRKALT,   // compare wrk.key to alt.key
  /*  D  */ EO_IXT_POS_ALTBYSRC, // position alt ixt using src.key
  /*  D  */ EO_IXT_POS_ALTBYWRK, // position alt ixt using entwrk.key

  // pay load
  /*  R  */ EO_PAY_ADDRESS,      // get payload address
  /*  M  */ EO_PAY_FETCH_COLI,   // fetch a column from entry payload
  /*  M  */ EO_PAY_FETCH_COLS,   // fetch a column from entry payload
  /*  M  */ EO_PAY_STORE_COLI,   // store a column into entry payload
  /*  M  */ EO_PAY_STORE_COLS,   // store a column into entry payload
  
  // pix operators
  /*  M  */ EO_PIX_STM_PRINT,    // print stream pix

  // pot operators
  /*  R  */ EO_POT_COMPLETED,    // do we have a full pot buffer ?
  
  // row / rowslab
  /*  D  */ EO_ROW_DST_PRINT,    // print dst row
  /*  D  */ EO_ROW_DST_RESET,    // reset dst rowslab
  /*  D  */ EO_ROW_DST_NEW,      // create new destination row
  /*  D  */ EO_ROW_DST_SET,      // set dst rowslab
  /*  D  */ EO_ROW_DST_TESTROW,  // test if destination row
  /*  D  */ EO_ROW_DST_ZERO,     // zero destination row
  /*  D  */ EO_ROW_SRC_BEGIN,    // begin src row traverse
  /*  D  */ EO_ROW_SRC_NEXT,     // move to next src row
  /*  D  */ EO_ROW_SRC_NULL,     // null rb src src
  /*  D  */ EO_ROW_SRC_PRINT,    // print src row
  /*  D  */ EO_ROW_SRC_PUSHOFS,  // push offset of source row
  /*  D  */ EO_ROW_SRC_SETBYOFS, // set src row by using offset on stack
  /*  D  */ EO_ROW_SRC_TEST,     // test if there is a row src buffer
  /*  -  */ EO_ROW_ALT_BEGIN,    // begin linear traverse
  /*  -  */ EO_ROW_ALT_NEXT,     // move to next row
  /*  -  */ EO_ROW_ALT_PRINT,    // print alt row
  /*  -  */ EO_ROW_ALT_PUSHOFS,  // push offset of alt row
  /*  -  */ EO_ROW_ALT_SETBYOFS, // set alt row by using offset on stack

  // row operators
  /*  D  */ EO_ROW_DST_POPCOLI,  // pop and deposit top into dst column
  /*  D  */ EO_ROW_DST_POPCOLS,  // pop and deposit top into dst column
  /*  D  */ EO_ROW_DST_PUSHCOLI, // push dst column
  /*  D  */ EO_ROW_SRC_PUSHCOLI, // push src column (integer)
  /*  D  */ EO_ROW_SRC_PUSHCOLS, // push src column (string)
  /*  D  */ EO_ROW_ALT_PUSHCOLI, // push alt column

  // super fragments
  /*  M  */ EO_SUPER_Q100,       // super fragment Q100
  /*  -  */ EO_SUPER_Z001,       // super fragment
  /*  -  */ EO_SUPER_Z002,       // super fragment
  /*  -  */ EO_SUPER_Z003,       // super fragment
  /*  -  */ EO_SUPER_Z004,       // super fragment
  /*  -  */ EO_SUPER_Z005,       // super fragment
  /*  -  */ EO_SUPER_Z006,       // super fragment
  /*  -  */ EO_SUPER_Z007,       // super fragment
  /*  -  */ EO_SUPER_Z008,       // super fragment
  /*  -  */ EO_SUPER_Z009,       // super fragment

  
  // revised
  /*  -  */ EO_REVISED_00,       // revised

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
  
  // column pushes
  /*    */ "EO_PUSH_COLUMN_LENGTH"_Stz,
  /*    */ "EO_PUSH_COLUMN_SCALE"_Stz,
  /*    */ "EO_PUSH_COLUMN_TYPE"_Stz,

  // ixt device pushes
  /*    */ "EO_PUSH_DEVIXT_ALT"_Stz,
  /*    */ "EO_PUSH_DEVIXT_SRC"_Stz,
  /*    */ "EO_PUSH_DEVIXT_TMP"_Stz,

  // row device pushes
  /*    */ "EO_PUSH_DEVROW_ALT"_Stz,
  /*    */ "EO_PUSH_DEVROW_DST"_Stz,
  /*    */ "EO_PUSH_DEVROW_SRC"_Stz,

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
  
  // index
  /*    */ "EO_INDEXNM_ALT_SET"_Stz,
  /*    */ "EO_INDEXNM_SRC_SET"_Stz,
  
  // indexTree
  /*    */ "EO_INDEXTREE_ALT_SET"_Stz,
  /*    */ "EO_INDEXTREE_DEV_SET"_Stz,
  /*    */ "EO_INDEXTREE_SRC_SET"_Stz,
  /*    */ "EO_INDEXTREE_TMP_DELETE"_Stz,
  /*    */ "EO_INDEXTREE_TMP_SET"_Stz,

  // indexTree
  /*    */ "EO_INDEXTREE_SET"_Stz,
  /*    */ "EO_INDEXTREE_DELETE"_Stz,

  // logical operators
  /*    */ "EO_LGL_AND"_Stz,
  /*    */ "EO_LGL_OR"_Stz,
  /*    */ "EO_LGL_NOT"_Stz,
  /*    */ "EO_LGL_XOR"_Stz,

  // null test operators
  /*    */ "EO_ISNULL"_Stz,
  /*    */ "EO_ISNOTNULL"_Stz,
  
  // pot
  /*    */ "EO_POT_USE_STREAMDEFOUT"_Stz,
  
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
  
  // relational operators
  /*    */ "EO_LESSTHAN"_Stz,
  /*    */ "EO_LESSEQUAL"_Stz,
  /*    */ "EO_EQUAL"_Stz,
  /*    */ "EO_NOTEQUAL"_Stz,
  /*    */ "EO_GREATEREQUAL"_Stz,
  /*    */ "EO_GREATERTHAN"_Stz,
  
  // register operators
  /*    */ "EO_REG_EOSMARKER_NEW"_Stz,
  /*    */ "EO_REG_INDEXDEF_ADDCOLUMN"_Stz,
  /*    */ "EO_REG_INDEXDEF_DELETE"_Stz,
  /*    */ "EO_REG_INDEXDEF_NEW"_Stz,
  /*    */ "EO_REG_INDEXTREE_NEW"_Stz,
  /*    */ "EO_REG_INDEXTREE_NULL"_Stz,
  /*    */ "EO_REG_PTNSTREAM_CLOSE"_Stz,
  /*    */ "EO_REG_PTNSTREAM_DELETE"_Stz,
  /*    */ "EO_REG_PTNSTREAM_NEW"_Stz,
  /*    */ "EO_REG_PTNSTREAM_OPEN"_Stz,
  /*    */ "EO_REG_PTNSTREAM_READ"_Stz,
  /*    */ "EO_REG_SRC_STREAMDEFOUT_NEW"_Stz,
  /*    */ "EO_REG_STREAMDEFOUT_NEW"_Stz,
  /*    */ "EO_REG_STREAMDEFOUT_ADDCOLUMN"_Stz,
  /*    */ "EO_REG_STREAMDEF_STREAMDEFOUT"_Stz,

  // register pushes
    /*    */ "EO_PUSH_REG_INDEXTREE"_Stz,

  // column set
  /*    */ "EO_SET_COLUMN"_Stz,
  /*    */ "EO_SET_COLUMNS"_Stz,
  /*    */ "EO_SET_ITEMS"_Stz,
  /*    */ "EO_SET_LOCALS_ALT"_Stz,
  /*    */ "EO_SET_LOCALS_ROW"_Stz,
  /*    */ "EO_SET_LOCALS_SRC"_Stz,
  /*    */ "EO_SET_LOCALS_SRC2"_Stz,

  // table
  /*    */ "EO_TABLENM_ALT_SET"_Stz,
  /*    */ "EO_TABLENM_SRC_SET"_Stz,

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
  
  // key
  /*    */ "EO_KEY_LCL2C_TO_I"_Stz,
  /*    */ "EO_KEY_ROW2C_TO_I"_Stz,

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

  // col operators
  /*    */ "EO_COL_SRC_PUSHDST"_Stz,
  /*    */ "EO_COL_ALT_PUSHONE"_Stz,
  /*    */ "EO_COL_ALT_PUSHSET"_Stz,
  /*    */ "EO_COL_SRC_PUSHONE"_Stz,
  /*    */ "EO_COL_SRC_PUSHONE_a"_Stz,
  /*    */ "EO_COL_SRC_PUSHSET"_Stz,

  // crw operators
  /*    */ "EO_CRW_ALT_DEREF"_Stz,
  /*    */ "EO_CRW_SRC_BEGIN"_Stz,
  /*    */ "EO_CRW_SRC_DEREF"_Stz,
  /*    */ "EO_CRW_SRC_NEXT"_Stz,
  /*    */ "EO_CRW_SRC_NULL"_Stz,
  /*    */ "EO_CRW_SRC_TEST"_Stz,
  
  // entry
  /*    */ "EO_ENT_FINDI"_Stz,
  /*    */ "EO_ENT_NEXT"_Stz,
  /*    */ "EO_ENT_POSPRIOR"_Stz,
  /*    */ "EO_ENT_PUSH"_Stz,
  /*    */ "EO_ENT_PUSHELMI0"_Stz,
  /*    */ "EO_ENT_PUSHELMS0"_Stz,
  /*    */ "EO_ENT_PUSHOFS"_Stz,
  /*    */ "EO_ENT_PUSHREF"_Stz,

  /*    */ "EO_ENT_SRC_PUSH"_Stz,
  /*    */ "EO_ENT_SRC_PUSHUPPER"_Stz,
  
  // entry relational operators
  /*    */ "EO_ENT_LESS"_Stz,
  /*    */ "EO_ENT_LESSEQUAL"_Stz,
  /*    */ "EO_ENT_EQUAL"_Stz,
  /*    */ "EO_ENT_NOTEQUAL"_Stz,
  /*    */ "EO_ENT_GREATEREQUAL"_Stz,
  /*    */ "EO_ENT_GREATER"_Stz,
  /*    */ "EO_ENT_PRINTKEYS"_Stz,

  // entWrk
  /*    */ "EO_ENTWRK_APPENDELM"_Stz,
  /*    */ "EO_ENTWRK_APPENDELMI"_Stz,
  /*    */ "EO_ENTWRK_APPENDELMS"_Stz,
  /*    */ "EO_ENTWRK_CREATEPAY"_Stz,
  /*    */ "EO_ENTWRK_POPOFS"_Stz,
  /*    */ "EO_ENTWRK_POPREF"_Stz,
  /*    */ "EO_ENTWRK_PRINT"_Stz,
  /*    */ "EO_ENTWRK_RESET"_Stz,
  /*    */ "EO_ENTWRK_SETINTELM"_Stz,

  // index tree
  /*    */ "EO_IXT_DEL_ENT"_Stz,
  /*    */ "EO_IXT_DUMP"_Stz,
  /*    */ "EO_IXT_INS_ENT"_Stz,
  /*    */ "EO_IXT_TRV_BEGIN"_Stz,
  /*    */ "EO_IXT_TRV_NEXT"_Stz,
  /*    */ "EO_IXT_TMP_ENTRIES"_Stz,
  
  /*    */ "EO_IXT_SRC_BEGIN"_Stz,
  /*    */ "EO_IXT_SRC_LBOUND"_Stz,
  /*    */ "EO_IXT_SRC_NEXT"_Stz,
  /*    */ "EO_IXT_SRC_PRINT"_Stz,
  /*    */ "EO_IXT_SRC_UBOUND"_Stz,
  /*    */ "EO_IXT_SRC_UBOUND_LT"_Stz,
  /*    */ "EO_IXT_ALT_BEGIN"_Stz,
  /*    */ "EO_IXT_ALT_NEXT"_Stz,
  /*    */ "EO_IXT_ALT_PRINT"_Stz,
  /*    */ "EO_IXT_CMP_SRCALT"_Stz,
  /*    */ "EO_IXT_CMP_WRKALT"_Stz,
  /*    */ "EO_IXT_POS_ALTBYSRC"_Stz,
  /*    */ "EO_IXT_POS_ALTBYWRK"_Stz,
  
  // pay load
  /*    */ "EO_PAY_ADDRESS"_Stz,
  /*    */ "EO_PAY_FETCH_COLI"_Stz,
  /*    */ "EO_PAY_FETCH_COLS"_Stz,
  /*    */ "EO_PAY_STORE_COLI"_Stz,
  /*    */ "EO_PAY_STORE_COLS"_Stz,
  
  // pix
  /*    */ "EO_PIX_STM_PRINT"_Stz,
  
  // pot
  /*    */ "EO_POT_COMPLETED"_Stz,
  
  // row / rowslab
  /*    */ "EO_ROW_DST_PRINT"_Stz,
  /*    */ "EO_ROW_DST_RESET"_Stz,
  /*    */ "EO_ROW_DST_NEW"_Stz,
  /*    */ "EO_ROW_DST_SET"_Stz,
  /*    */ "EO_ROW_DST_TESTROW"_Stz,
  /*    */ "EO_ROW_DST_ZERO"_Stz,
  /*    */ "EO_ROW_SRC_BEGIN"_Stz,
  /*    */ "EO_ROW_SRC_NEXT"_Stz,
  /*    */ "EO_ROW_SRC_NULL"_Stz,
  /*    */ "EO_ROW_SRC_PRINT"_Stz,
  /*    */ "EO_ROW_SRC_PUSHOFS"_Stz,
  /*    */ "EO_ROW_SRC_SETBYOFS"_Stz,
  /*    */ "EO_ROW_SRC_TEST"_Stz,
  /*    */ "EO_ROW_ALT_BEGIN"_Stz,
  /*    */ "EO_ROW_ALT_NEXT"_Stz,
  /*    */ "EO_ROW_ALT_PRINT"_Stz,
  /*    */ "EO_ROW_ALT_PUSHOFS"_Stz,
  /*    */ "EO_ROW_ALT_SETBYOFS"_Stz,

  // row operators
  /*    */ "EO_ROW_DST_POPCOLI"_Stz,
  /*    */ "EO_ROW_DST_POPCOLS"_Stz,
  /*    */ "EO_ROW_DST_PUSHCOLI"_Stz,
  /*    */ "EO_ROW_SRC_PUSHCOLI"_Stz,
  /*    */ "EO_ROW_SRC_PUSHCOLS"_Stz,
  /*    */ "EO_ROW_ALT_PUSHCOLI"_Stz,

  // super fragments
  /*    */ "EO_SUPER_Q100"_Stz,
  /*    */ "EO_SUPER_Z001"_Stz,
  /*    */ "EO_SUPER_Z002"_Stz,
  /*    */ "EO_SUPER_Z003"_Stz,
  /*    */ "EO_SUPER_Z004"_Stz,
  /*    */ "EO_SUPER_Z005"_Stz,
  /*    */ "EO_SUPER_Z006"_Stz,
  /*    */ "EO_SUPER_Z007"_Stz,
  /*    */ "EO_SUPER_Z008"_Stz,
  /*    */ "EO_SUPER_Z009"_Stz,
  
  // srevised
  /*    */ "EO_REVISED_00"_Stz,

  /*    */ "ENUMS"_Stz
};

//----------------------------------------------------------------------------

struct EoArg
{
  eEA m_eArgument;
  i8  m_nArgBytes;
};

/*----------------------------------------------------------------------------*/
/* Description of EO CmdLineArgs                                                     */
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

  // column pushes
  { EA_INT,     8 },  // EO_PUSH_COLUMN_LENGTH
  { EA_INT,     8 },  // EO_PUSH_COLUMN_SCALE
  { EA_INT,     8 },  // EO_PUSH_COLUMN_TYPE

  // ixt device pushes
  { EA_NONE,    0 },  // EO_PUSH_DEVIXT_ALT
  { EA_NONE,    0 },  // EO_PUSH_DEVIXT_SRC
  { EA_NONE,    0 },  // EO_PUSH_DEVIXT_TMP

  // Ixt device pushes
  { EA_NONE,    0 },  // EO_PUSH_DEVROW_ALT
  { EA_NONE,    0 },  // EO_PUSH_DEVROW_DST
  { EA_NONE,    0 },  // EO_PUSH_DEVROW_SRC

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

  // index
  { EA_STRING, -1 },  // EO_INDEXNM_ALT_SET
  { EA_STRING, -1 },  // EO_INDEXNM_SRC_SET
  
  // indexTree
  { EA_NONE,    0 },  // EO_INDEXTREE_ALT_SET
  { EA_INT,     8 },  // EO_INDEXTREE_DEV_SET
  { EA_NONE,    0 },  // EO_INDEXTREE_SRC_SET
  { EA_NONE,    0 },  // EO_INDEXTREE_TMP_DELETE
  { EA_NONE,    0 },  // EO_INDEXTREE_TMP_SET

    // indexTree
  { EA_NONE,    0 },  // EO_INDEXTREE_SET
  { EA_NONE,    0 },  // EO_INDEXTREE_DELETE

  // logical operators
  { EA_NONE,    0 },  // EO_LGL_AND
  { EA_NONE,    0 },  // EO_LGL_OR
  { EA_NONE,    0 },  // EO_LGL_NOT
  { EA_NONE,    0 },  // EO_LGL_XOR

  // null test operators
  { EA_NONE,    0 },  // EO_ISNULL
  { EA_NONE,    0 },  // EO_ISNOTNULL

  // pot
  { EA_INT,     8 },  // EO_POT_USE_STREAMDEFOUT
  
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

  // relational operators
  { EA_NONE,    0 },  // EO_LESSTHAN
  { EA_NONE,    0 },  // EO_LESSEQUAL
  { EA_NONE,    0 },  // EO_EQUAL
  { EA_NONE,    0 },  // EO_NOTEQUAL
  { EA_NONE,    0 },  // EO_GREATEREQUAL
  { EA_NONE,    0 },  // EO_GREATERTHAN

  // register operators
  { EA_NONE,    0 },  // EO_REG_EOSMARKER_NEW
  { EA_STRING, -1 },  // EO_REG_INDEXDEF_ADDCOLUMN
  { EA_NONE,    0 },  // EO_REG_INDEXDEF_DELETE
  { EA_STRING, -1 },  // EO_REG_INDEXDEF_NEW
  { EA_NONE,    0 },  // EO_REG_INDEXTREE_NEW
  { EA_NONE,    0 },  // EO_REG_INDEXTREE_NULL
  { EA_NONE,    0},   // EO_REG_PTNSTREAM_CLOSE
  { EA_NONE,    0},   // EO_REG_PTNSTREAM_DELETE
  { EA_NONE,    0},   // EO_REG_PTNSTREAM_NEW
  { EA_NONE,    0},   // EO_REG_PTNSTREAM_OPEN
  { EA_NONE,    0},   // EO_REG_PTNSTREAM_READ
  { EA_NONE,    0 },  // EO_REG_SRC_STREAMDEFOUT_NEW
  { EA_NONE,    0 },  // EO_REG_STREAMDEFOUT_NEW
  { EA_STRING, -1 },  // EO_REG_STREAMDEFOUT_ADDCOLUMN
  { EA_NONE,    0 },  // EO_REG_STREAMDEF_STREAMDEFOUT_ADDCOLUMN

  // register pushes
  { EA_NONE,    0 },  // EO_PUSH_REG_INDEXTREE

  // column set
  { EA_INT,     8 },  // EO_SET_COLUMN
  { EA_INT,     8 },  // EO_SET_COLUMNS
  { EA_INT,     8 },  // EO_SET_ITEMS
  { EA_INT,     8 },  // EO_SET_LOCALS_ALT
  { EA_NONE,    0 },  // EO_SET_LOCALS_ROW
  { EA_INT,     8 },  // EO_SET_LOCALS_SRC
  { EA_NONE,    0 },  // EO_SET_LOCALS_SRC2

  // table
  { EA_STRING, -1 },  // EO_TABLENM_ALT_SET
  { EA_STRING, -1 },  // EO_TABLENM_SRC_SET

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

  // key
  { EA_INT,     8 },  // EO_KEY_LCL2C_TO_I
  { EA_INT,     8 },  // EO_KEY_ROW2C_TO_I

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
  
  // col operators
  { EA_NONE,    0 },  // EO_COL_SRC_PUSHDST
  { EA_SUBSCR,  8 },  // EO_COL_ALT_PUSHONE
  { EA_NONE,    0 },  // EO_COL_ALT_PUSHSET
  { EA_SUBSCR,  8 },  // EO_COL_SRC_PUSHONE
  { EA_SUBSCR,  8 },  // EO_COL_SRC_PUSHONE_a
  { EA_NONE,    0 },  // EO_COL_SRC_PUSHSET

  // crw operators
  { EA_NONE,    0 },  // EO_CRW_ALT_DEREF
  { EA_NONE,    0 },  // EO_CRW_SRC_BEGIN
  { EA_NONE,    0 },  // EO_CRW_SRC_DEREF
  { EA_NONE,    0 },  // EO_CRW_SRC_NEXT
  { EA_NONE,    0 },  // EO_CRW_SRC_NULL
  { EA_NONE,    0 },  // EO_CRW_SRC_TEST
  
  // entry
  { EA_NONE,    0 },  // EO_ENT_FINDI
  { EA_NONE,    0 },  // EO_ENT_NEXT
  { EA_NONE,    0 },  // EO_ENT_POSPRIOR
  { EA_NONE,    0 },  // EO_ENT_PUSH
  { EA_NONE,    0 },  // EO_ENT_PUSHELMI0
  { EA_NONE,    0 },  // EO_ENT_PUSHELMS0
  { EA_NONE,    0 },  // EO_ENT_PUSHOFS
  { EA_NONE,    0 },  // EO_ENT_PUSHREF

  { EA_NONE,    0 },  // EO_ENT_SRC_PUSH
  { EA_NONE,    0 },  // EO_ENT_SRC_PUSHUPPER

  // entry relational operators
  { EA_NONE,    0 },  // EO_ENT_LESS
  { EA_NONE,    0 },  // EO_ENT_LESSEQUAL
  { EA_NONE,    0 },  // EO_ENT_EQUAL
  { EA_NONE,    0 },  // EO_ENT_NOTEQUAL
  { EA_NONE,    0 },  // EO_ENT_GREATEREQUAL
  { EA_NONE,    0 },  // EO_ENT_GREATER
  { EA_NONE,    0 },  // EO_ENT_PRINTKEYS


  
  // entWrk
  { EA_SUBSCR,  8 },  // EO_ENTWRK_APPENDELM
  { EA_NONE,    0 },  // EO_ENTWRK_APPENDELMI
  { EA_NONE,    0 },  // EO_ENTWRK_APPENDELMS
  { EA_INT,     8 },  // EO_ENTWRK_CREATEPAY
  { EA_NONE,    0 },  // EO_ENTWRK_POPOFS
  { EA_NONE,    0 },  // EO_ENTWRK_POPREF
  { EA_NONE,    0 },  // EO_ENTWRK_PRINT
  { EA_NONE,    0 },  // EO_ENTWRK_RESET
  { EA_NONE,    0 },  // EO_ENTWRK_SETINTELM

  // index tree
  { EA_NONE,    0 },  // EO_IXT_DEL_ENT
  { EA_NONE,    0 },  // EO_IXT_DUMP
  { EA_NONE,    0 },  // EO_IXT_INS_ENT
  { EA_NONE,    0 },  // EO_IXT_TRV_BEGIN
  { EA_NONE,    0 },  // EO_IXT_TRV_NEXT
  { EA_NONE,    0 },  // EO_IXT_TMP_ENTRIES

  { EA_NONE,    0 },  // EO_IXT_SRC_BEGIN
  { EA_NONE,    0 },  // EO_IXT_SRC_LBOUND
  { EA_NONE,    0 },  // EO_IXT_SRC_NEXT
  { EA_NONE,    0 },  // EO_IXT_SRC_PRINT
  { EA_NONE,    0 },  // EO_IXT_SRC_UBOUND
  { EA_NONE,    0 },  // EO_IXT_SRC_UBOUND_LT
  { EA_NONE,    0 },  // EO_IXT_ALT_BEGIN
  { EA_NONE,    0 },  // EO_IXT_ALT_NEXT
  { EA_NONE,    0 },  // EO_IXT_ALT_PRINT
  { EA_NONE,    0 },  // EO_IXT_CMP_SRCALT
  { EA_NONE,    0 },  // EO_IXT_CMP_WRKALT
  { EA_NONE,    0 },  // EO_IXT_POS_ALTBYSRC
  { EA_NONE,    0 },  // EO_IXT_POS_ALTBYWRK

  // pay load
  { EA_NONE,    0 },  // EO_PAY_ADDRESS
  { EA_SUBSCR,  8 },  // EO_PAY_FETCH_COLI
  { EA_SUBSCR,  8 },  // EO_PAY_FETCH_COLS
  { EA_SUBSCR,  8 },  // EO_PAY_STORE_COLI
  { EA_SUBSCR,  8 },  // EO_PAY_STORE_COLS

  // pix
  { EA_NONE,    0 },  // EO_PIX_STM_PRINT
  
  // pot
  { EA_NONE,    0 },  // EO_POT_COMPLETED
  
  // row / rowslab
  { EA_NONE,    0 },  // EO_ROW_DST_PRINT
  { EA_NONE,    0 },  // EO_ROW_DST_RESET
  { EA_NONE,    0 },  // EO_ROW_DST_NEW
  { EA_NONE,    0 },  // EO_ROW_DST_SET
  { EA_NONE,    0 },  // EO_ROW_DST_TESTROW
  { EA_NONE,    0 },  // EO_ROW_DST_ZERO
  { EA_NONE,    0 },  // EO_ROW_SRC_BEGIN
  { EA_NONE,    0 },  // EO_ROW_SRC_NEXT
  { EA_NONE,    0 },  // EO_ROW_SRC_NULL
  { EA_NONE,    0 },  // EO_ROW_SRC_PRINT
  { EA_NONE,    0 },  // EO_ROW_SRC_PUSHOFS
  { EA_NONE,    0 },  // EO_ROW_SRC_SETBYOFS
  { EA_NONE,    0 },  // EO_ROW_SRC_TEST
  { EA_NONE,    0 },  // EO_ROW_ALT_BEGIN
  { EA_NONE,    0 },  // EO_ROW_ALT_NEXT
  { EA_NONE,    0 },  // EO_ROW_ALT_PRINT
  { EA_NONE,    0 },  // EO_ROW_ALT_PUSHOFS
  { EA_NONE,    0 },  // EO_ROW_ALT_SETBYOFS

  // row operators
  { EA_SUBSCR,  8 },  // EO_ROW_DST_POPCOLI
  { EA_SUBSCR,  8 },  // EO_ROW_DST_POPCOLS
  { EA_SUBSCR,  8 },  // EO_ROW_DST_PUSHCOLI
  { EA_SUBSCR,  8 },  // EO_ROW_SRC_PUSHCOLI
  { EA_SUBSCR,  8 },  // EO_ROW_SRC_PUSHCOLS
  { EA_SUBSCR,  8 },  // EO_ROW_ALT_PUSHCOLI

  { EA_NONE,    0 },  // super fragment Q100
  { EA_NONE,    0 },  // super fragment
  { EA_NONE,    0 },  // super fragment
  { EA_NONE,    0 },  // super fragment
  { EA_NONE,    0 },  // super fragment
  { EA_NONE,    0 },  // super fragment
  { EA_NONE,    0 },  // super fragment
  { EA_NONE,    0 },  // super fragment
  { EA_NONE,    0 },  // super fragment
  { EA_NONE,    0 },  // super fragment

  // -- end --
  { EA_NONE,    0 },  // EO_ENUMS
};
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
