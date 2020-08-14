/*============================================================================*/
/* eEO.h -- Expression Operators                                              */
/*============================================================================*/
#pragma once

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
  /*    */ EO_ADD,              // add top two int's push result
  /*    */ EO_DIVIDE,           // divide top int by second push result
  /*    */ EO_MODULO,           // modulo top int be second push result
  /*    */ EO_MULTIPLY,         // multiply top two int's push result
  /*    */ EO_NEGATE,           // negate the top value
  /*    */ EO_SUBTRACT,         // subtract second int from top push result
  
  // counter
  /*    */ EO_CTR_0_DEC,        // dec counter 0
  /*    */ EO_CTR_0_INC,        // inc counter 0
  /*    */ EO_CTR_0_POP,        // pop counter 0
  /*    */ EO_CTR_0_PUSH,       // push counter 0

  // jump operators
  /*    */ EO_JMP,              // jump to location
  /*    */ EO_JMP_FALSE,        // pop top and jump to location if false
  /*    */ EO_JMP_TRUE,         // pop top and jump to location if true
  
  /*    */ EO_JMP_LESS,         // pop top and jump to location if <
  /*    */ EO_JMP_LESSEQUAL,    // pop top and jump to location if <=
  /*    */ EO_JMP_EQUAL,        // pop top and jump to location if ==
  /*    */ EO_JMP_NOTEQUAL,     // pop top and jump to location if !=
  /*    */ EO_JMP_GREATEREQUAL, // pop top and jump to location if >=
  /*    */ EO_JMP_GREATER,      // pop top and jump to location if >
                                    
  // logical operators
  /*    */ EO_LGL_AND,          // logical and top two int's, push result
  /*    */ EO_LGL_OR,           // logical or top two int's, push result
  /*    */ EO_LGL_NOT,          // logical not top int, push result
  
  // null test operators
  /*    */ EO_ISNULL,           // test if top is null, push result
  /*    */ EO_ISNOTNULL,        // test if top is not null, push result
  
  // pushes
  /*    */ EO_DATE,             // push date
  /*    */ EO_INTEGER,          // push integer
  /*    */ EO_PUSHNULL,         // push null
  /*    */ EO_PUSH_MINUS1,      // push integer -1
  /*    */ EO_PUSH_0,           // push integer 0
  /*    */ EO_PUSH_1,           // push integer 1
  /*    */ EO_RANDOM,           // push random number 0..arg-1

  // relational operators
  /*    */ EO_LESSTHAN,         // test top < 2nd, push result
  /*    */ EO_LESSEQUAL,        // test top <= 2nd, push result
  /*    */ EO_EQUAL,            // test top == 2nd, push result
  /*    */ EO_NOTEQUAL,         // test top != 2nd, push result
  /*    */ EO_GREATEREQUAL,     // test top >= 2nd, push result
  /*    */ EO_GREATERTHAN,      // test top > 2nd, push result

  // stack operators
  /*    */ EO_INC,              // increment top, push result
  /*    */ EO_DEC,              // decrement top, push result
  /*    */ EO_DUP,              // duplicate top
  /*    */ EO_POP,              // pop / discard top
  /*    */ EO_POP_LOCAL,        // pop into a local register
  /*    */ EO_PRINT,            // print top
  /*    */ EO_PUSH_LOCAL,       // push a local register

  // string operators
  /*    */ EO_STR_COMPARE,      // pop top stradr and compare str on stack
  /*    */ EO_STR_PRINTADR,     // pop top and print iArg as addr of string
  /*    */ EO_STR_PRINTSTK,     // pop string from top and print
  /*    */ EO_STR_LITERAL,      // push address of string literal

  // subroutine operators
  /*    */ EO_CALL,             // call subroutine at location
  /*    */ EO_RETURN,           // return from subroutine and

  // misc
  /*    */ EO_ASSERT,           // pop top and assert
  /*    */ EO_BREAKPOINT,       // break point
  /*    */ EO_CALLAGAIN,        // set call again
  /*    */ EO_CALLEDAGAIN,      // push called again
  /*    */ EO_DUMP,             // stack dump
  /*    */ EO_INIT,             // initialise the rlu common
  /*    */ EO_INTEGRITY,        // integrity check
  /*    */ EO_SANITY,           // sanity check
  /*    */ EO_SLEEP,            // sleep for top seconds
  /*    */ EO_SLOT,             // push i_Slot
  /*    */ EO_TRACEOFF,         // set trace off
  /*    */ EO_TRACEON,          // set trace on
  /*    */ EO_NOOP,             // no op
  /*    */ EO_ERROR,            // error marker

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // date operators
  /*    */ EO_DTE_NOW,          // push current date
  /*    */ EO_DTE_PRINT,        // pop top and print as date
  /*    */ EO_DTE_YEAR,         // pop top, push year of date
  
  // hashing
  /*    */ EO_HSH_ADD_HASHI,    // add hash of top int to running hash
  /*    */ EO_HSH_RESET,        // reset hash

  // partitioning
  /*    */ EO_PTN_CALC,         // calculate i_Ptn using running iHash
  /*    */ EO_PTN_PUSH,         // push i_Ptn
  /*    */ EO_PTN_RESET,        // reset running iHash
  /*    */ EO_PTN_RUNNING,      // mix top into running iHash

  // timer
  /*    */ EO_TMR_SINCE,        // get nsecs since ::constructor
  /*    */ EO_TMR_START,        // timer start
  /*    */ EO_TMR_NSECS,        // push timer nanoseconds
  /*    */ EO_TMR_UPDATE,       // timer update
  /*    */ EO_TMR_AGG_ADD,      // add timer to agg timer
  /*    */ EO_TMR_AGG_NSECS,    // push agg nanoseconds
  /*    */ EO_TMR_AGG_RESET,    // reset agg timer

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // complex eo
  
  // col operators
  /*    */ EO_COL_NOT_PROJECT,  // reset column in streamPix of dst
  /*    */ EO_COL_SRC_PUSH,     // push column value from m_aSrcCrw
  /*    */ EO_COL_SRC_PUSHI,    // push column int value from m_aSrcCrw
  /*    */ EO_COL_SRC_PUSHSET,  // push column subset from m_aSrcCrw
  /*    */ EO_COL_SRC_PUSHSTR,  // push column str value from m_aSrcCrw
  /*    */ EO_COL_ALT_PUSH,     // push column value from m_aAltCrw
  /*    */ EO_COL_ALT_PUSHI,    // push column int value from m_aAltCrw
  /*    */ EO_COL_ALT_PUSHSET,  // push column subset from m_aAltCrw

  // crw operators
  /*    */ EO_CRW_SRC_BEGIN,    // begin src crw traverse
  /*    */ EO_CRW_SRC_FETCH,    // fetch m_aSrcCrw using m_aSrcEnt.ref
  /*    */ EO_CRW_ALT_FETCH,    // fetch m_aAltCrw using m_aAltEnt.ref
  /*    */ EO_CRW_SRC_NEXT,     // move to next src rsb
  /*    */ EO_CRW_SRC_TORSB,    // convert crw src to rsb
  
  // entry
  /*    */ EO_ENT_NEXT,         // move to next m_pEnt
  /*    */ EO_ENT_POSPRIOR,     // position m_pEnt prior using m_oEntWrk.key
  /*    */ EO_ENT_PUSH,         // push entry
  /*    */ EO_ENT_PUSHELMI0,    // push entry elm 0 int
  /*    */ EO_ENT_PUSHELMS0,    // push entry elm 0 str
  /*    */ EO_ENT_PUSHOFS,      // push entry offset
  /*    */ EO_ENT_PUSHREF,      // push ref of current entry

  /*    */ EO_ENT_SRC_PUSH,     // push src entry
  /*    */ EO_ENT_SRC_PUSHUPPER,// push src upper entry
  
  // entry relational operators
  /*    */ EO_ENT_LESS,         // test if m_pEnt.key <  m_oEntWrk.key
  /*    */ EO_ENT_LESSEQUAL,    // test if m_pEnt.key <= m_oEntWrk.key
  /*    */ EO_ENT_EQUAL,        // test if m_pEnt.key == m_oEntWrk.key
  /*    */ EO_ENT_NOTEQUAL,     // test if m_pEnt.key != m_oEntWrk.key
  /*    */ EO_ENT_GREATEREQUAL, // test if m_pEnt.key >= m_oEntWrk.key
  /*    */ EO_ENT_GREATER,      // test if m_pEnt.key >  m_oEntWrk.key
  /*    */ EO_ENT_PRINTKEYS,    // print both m_pEnt.key and m_oEntWrk.key

  // entWrk
  /*    */ EO_ENTWRK_APPENDELM, // append next element value
  /*    */ EO_ENTWRK_APPENDELMI,// append next element value (int)
  /*    */ EO_ENTWRK_APPENDELMS,// append next element value (str)
  /*    */ EO_ENTWRK_CREATEPAY, // create a payload in entwrk
  /*    */ EO_ENTWRK_POPOFS,    // pop m_oEntWrk.ref.buffer/offset
  /*    */ EO_ENTWRK_POPREF,    // pop m_oEntWrk.ref
  /*    */ EO_ENTWRK_PRINT,     // print m_oEntWrk
  /*    */ EO_ENTWRK_RESET,     // reset m_oEntWrk
  
  // index tree
  /*    */ EO_IXT_DEL_ENT,      // delete current m_aEnt from indexTree
  /*    */ EO_IXT_DUMP,         // dump index tree
  /*    */ EO_IXT_INS_ENT,      // insert an entry into the indexTree
  /*    */ EO_IXT_TRV_BEGIN,    // begin linear traverse
  /*    */ EO_IXT_TRV_NEXT,     // traverse to next entry
  /*    */ EO_IXT_TMP_ENTRIES,  // push number of entries in tmp index

  
  /*    */ EO_IXT_SRC_BEGIN,    // begin linear traverse
  /*    */ EO_IXT_SRC_LBOUND,   // position at lower bound
  /*    */ EO_IXT_SRC_NEXT,     // move to next entry
  /*    */ EO_IXT_SRC_PRINT,    // print src ent
  /*    */ EO_IXT_SRC_UBOUND,   // get upper bound
  /*    */ EO_IXT_SRC_UBOUND_LT, // get upper bound (less than)
  /*    */ EO_IXT_ALT_BEGIN,    // begin linear traverse
  /*    */ EO_IXT_ALT_NEXT,     // move to next entry
  /*    */ EO_IXT_ALT_PRINT,    // print src ent
  /*    */ EO_IXT_CMP_SRCALT,   // compare src.key to alt.key
  /*    */ EO_IXT_CMP_WRKALT,   // compare wrk.key to alt.key
  /*    */ EO_IXT_POS_ALTBYSRC, // position alt ixt using src.key
  /*    */ EO_IXT_POS_ALTBYWRK, // position alt ixt using entwrk.key

  // pay load
  /*    */ EO_PAY_FETCH_COLI,   // fetch a column from entry payload
  /*    */ EO_PAY_FETCH_COLS,   // fetch a column from entry payload
  /*    */ EO_PAY_STORE_COLI,   // store a column into entry payload
  /*    */ EO_PAY_STORE_COLS,   // store a column into entry payload
  
  // pix operators
  /*    */ EO_PIX_STM_PRINT,    // print stream pix

  // rsb / rowslab
  /*    */ EO_RSB_DST_PRINT,    // print dst rsb
  /*    */ EO_RSB_DST_RESET,    // reset dst rowslab
  /*    */ EO_RSB_DST_NEW,      // create new destination rsb
  /*    */ EO_RSB_DST_TEST,     // test if destination rsb
  /*    */ EO_RSB_DST_ZERO,     // zero destination rsb
  /*    */ EO_RSB_SRC_BEGIN,    // begin src rsb traverse
  /*    */ EO_RSB_SRC_NEXT,     // move to next src rsb
  /*    */ EO_RSB_SRC_PRINT,    // print src rsb
  /*    */ EO_RSB_SRC_PUSHOFS,  // push offset of source rsb
  /*    */ EO_RSB_SRC_SETBYOFS, // set src rsb by using offset on stack
  /*    */ EO_RSB_ALT_BEGIN,    // begin linear traverse
  /*    */ EO_RSB_ALT_NEXT,     // move to next rsb
  /*    */ EO_RSB_ALT_PRINT,    // print alt rsb
  /*    */ EO_RSB_ALT_PUSHOFS,  // push offset of alt rsb
  /*    */ EO_RSB_ALT_SETBYOFS, // set alt rsb by using offset on stack

  // rsb operators
  /*    */ EO_RSB_DST_POPCOLI,  // pop and deposit top into dst column
  /*    */ EO_RSB_DST_POPCOLS,  // pop and deposit top into dst column
  /*    */ EO_RSB_DST_PUSHCOLI, // push dst column
  /*    */ EO_RSB_SRC_PUSHCOLI, // push src column (integer)
  /*    */ EO_RSB_SRC_PUSHCOLS, // push src column (string)
  /*    */ EO_RSB_ALT_PUSHCOLI, // push alt column
  
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
  
  // null test operators
  /*    */ "EO_ISNULL"_Stz,
  /*    */ "EO_ISNOTNULL"_Stz,
  
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
  
  // stack operators
  /*    */ "EO_INC"_Stz,
  /*    */ "EO_DEC"_Stz,
  /*    */ "EO_DUP"_Stz,
  /*    */ "EO_POP"_Stz,
  /*    */ "EO_POP_LOCAL"_Stz,
  /*    */ "EO_PRINT"_Stz,
  /*    */ "EO_PUSH_LOCAL"_Stz,

  // string operators
  /*    */ "EO_STR_COMPARE"_Stz,
  /*    */ "EO_STR_PRINTADR"_Stz,
  /*    */ "EO_STR_PRINTSTK"_Stz,
  /*    */ "EO_STR_LITERAL"_Stz,
  
  // subroutine operators
  /*    */ "EO_CALL"_Stz,
  /*    */ "EO_RETURN"_Stz,

  // misc
  /*    */ "EO_ASSERT"_Stz,
  /*    */ "EO_BREAKPOINT"_Stz,
  /*    */ "EO_CALLAGAIN"_Stz,
  /*    */ "EO_CALLEDAGAIN"_Stz,
  /*    */ "EO_DUMP"_Stz,
  /*    */ "EO_INIT"_Stz,
  /*    */ "EO_INTEGRITY"_Stz,
  /*    */ "EO_SANITY"_Stz,
  /*    */ "EO_SLEEP"_Stz,
  /*    */ "EO_SLOT"_Stz,
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

  // col operators
  /*    */ "EO_COL_NOT_PROJECT"_Stz,
  /*    */ "EO_COL_SRC_PUSH"_Stz,
  /*    */ "EO_COL_SRC_PUSHI"_Stz,
  /*    */ "EO_COL_SRC_PUSHSET"_Stz,
  /*    */ "EO_COL_SRC_PUSHSTR"_Stz,
  /*    */ "EO_COL_ALT_PUSH"_Stz,
  /*    */ "EO_COL_ALT_PUSHI"_Stz,
  /*    */ "EO_COL_ALT_PUSHSET"_Stz,

  // crw operators
  /*    */ "EO_CRW_SRC_BEGIN"_Stz,
  /*    */ "EO_CRW_SRC_FETCH"_Stz,
  /*    */ "EO_CRW_ALT_FETCH"_Stz,
  /*    */ "EO_CRW_SRC_NEXT"_Stz,
  /*    */ "EO_CRW_SRC_TORSB"_Stz,
  
  // entry
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
  /*    */ "EO_PAY_FETCH_COLI"_Stz,
  /*    */ "EO_PAY_FETCH_COLS"_Stz,
  /*    */ "EO_PAY_STORE_COLI"_Stz,
  /*    */ "EO_PAY_STORE_COLS"_Stz,
  
  // pix
  /*    */ "EO_PIX_STM_PRINT"_Stz,
  
  // rsb / rowslab
  /*    */ "EO_RSB_DST_PRINT"_Stz,
  /*    */ "EO_RSB_DST_RESET"_Stz,
  /*    */ "EO_RSB_DST_NEW"_Stz,
  /*    */ "EO_RSB_DST_TEST"_Stz,
  /*    */ "EO_RSB_DST_ZERO"_Stz,
  /*    */ "EO_RSB_SRC_BEGIN"_Stz,
  /*    */ "EO_RSB_SRC_NEXT"_Stz,
  /*    */ "EO_RSB_SRC_PRINT"_Stz,
  /*    */ "EO_RSB_SRC_PUSHOFS"_Stz,
  /*    */ "EO_RSB_SRC_SETBYOFS"_Stz,
  /*    */ "EO_RSB_ALT_BEGIN"_Stz,
  /*    */ "EO_RSB_ALT_NEXT"_Stz,
  /*    */ "EO_RSB_ALT_PRINT"_Stz,
  /*    */ "EO_RSB_ALT_PUSHOFS"_Stz,
  /*    */ "EO_RSB_ALT_SETBYOFS"_Stz,

  // rsb operators
  /*    */ "EO_RSB_DST_POPCOLI"_Stz,
  /*    */ "EO_RSB_DST_POPCOLS"_Stz,
  /*    */ "EO_RSB_DST_PUSHCOLI"_Stz,
  /*    */ "EO_RSB_SRC_PUSHCOLI"_Stz,
  /*    */ "EO_RSB_SRC_PUSHCOLS"_Stz,
  /*    */ "EO_RSB_ALT_PUSHCOLI"_Stz,
  
  /*    */ "ENUMS"_Stz
};

//----------------------------------------------------------------------------

struct EoArg
{
  eEA m_eArgument;
  i8  m_nArgBytes;
};

/*----------------------------------------------------------------------------*/
/* Description of EO Args                                                     */
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

  // null test operators
  { EA_NONE,    0 },  // EO_ISNULL
  { EA_NONE,    0 },  // EO_ISNOTNULL

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

  // stack operators
  { EA_NONE,    0 },  // EO_INC
  { EA_NONE,    0 },  // EO_DEC
  { EA_NONE,    0 },  // EO_DUP
  { EA_NONE,    0 },  // EO_POP
  { EA_INT,     8 },  // EO_POP_LOCAL
  { EA_NONE,    0 },  // EO_PRINT
  { EA_INT,     8 },  // EO_PUSH_LOCAL
  
  // string operators
  { EA_NONE,    0 },  // EO_STR_COMPARE
  { EA_NONE,    0 },  // EO_STR_PRINTADR
  { EA_NONE,    0 },  // EO_STR_PRINTSTK
  { EA_STRING, -1 },  // EO_STR_LITERAL
  
  // subroutine operators
  { EA_LABEL,   8 },  // EO_CALL
  { EA_NONE,    0 },  // EO_RETURN

  // misc
  { EA_NONE,    0 },  // EO_ASSERT
  { EA_NONE,    0 },  // EO_BREAKPOINT
  { EA_NONE,    0 },  // EO_CALLAGAIN
  { EA_NONE,    0 },  // EO_CALLEDAGAIN
  { EA_NONE,    0 },  // EO_DUMP
  { EA_NONE,    0 },  // EO_INIT
  { EA_NONE,    0 },  // EO_INTEGRITY
  { EA_NONE,    0 },  // EO_SANITY
  { EA_NONE,    0 },  // EO_SLEEP
  { EA_NONE,    0 },  // EO_SLOT
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
  
  // col operators
  { EA_SUBSCR,  8 },  // EO_COL_NOT_PROJECT
  { EA_SUBSCR,  8 },  // EO_COL_SRC_PUSH
  { EA_SUBSCR,  8 },  // EO_COL_SRC_PUSHI
  { EA_NONE,    0 },  // EO_COL_SRC_PUSHSET
  { EA_SUBSCR,  8 },  // EO_COL_SRC_PUSHSTR
  { EA_SUBSCR,  8 },  // EO_COL_ALT_PUSH
  { EA_SUBSCR,  8 },  // EO_COL_ALT_PUSHI
  { EA_NONE,    0 },  // EO_COL_ALT_PUSHSET

  // crw operators
  { EA_NONE,    0 },  // EO_CRW_SRC_BEGIN
  { EA_NONE,    0 },  // EO_CRW_SRC_FETCH
  { EA_NONE,    0 },  // EO_CRW_ALT_FETCH
  { EA_NONE,    0 },  // EO_CRW_SRC_NEXT
  { EA_NONE,    0 },  // EO_CRW_SRC_TORSB
  
  // entry
  { EA_NONE,    0 },  // EO_ENT_NEXT
  { EA_NONE,    0 },  // EO_ENT_POSPRIOR
  { EA_NONE,    0 },  // EO_ENT_PUSH
  { EA_NONE,    0 },  // EO_ENT_PUSHELMI0
  { EA_NONE,    0 },  // EO_ENT_PUSHELMS0
  { EA_NONE,    0 },  // EO_ENT_PUSHOFS
  { EA_NONE,    0 },  // EO_ENT_PUSHREF

  // entry relational operators
  { EA_NONE,    0 },  // EO_ENT_LESS
  { EA_NONE,    0 },  // EO_ENT_LESSEQUAL
  { EA_NONE,    0 },  // EO_ENT_EQUAL
  { EA_NONE,    0 },  // EO_ENT_NOTEQUAL
  { EA_NONE,    0 },  // EO_ENT_GREATEREQUAL
  { EA_NONE,    0 },  // EO_ENT_GREATER
  { EA_NONE,    0 },  // EO_ENT_PRINTKEYS

  { EA_NONE,    0 },  // EO_ENT_SRC_PUSH
  { EA_NONE,    0 },  // EO_ENT_SRC_PUSHUPPER
  
  // entWrk
  { EA_SUBSCR,  8 },  // EO_ENTWRK_APPENDELM
  { EA_NONE,    0 },  // EO_ENTWRK_APPENDELMI
  { EA_NONE,    0 },  // EO_ENTWRK_APPENDELMS
  { EA_INT,     8 },  // EO_ENTWRK_CREATEPAY
  { EA_NONE,    0 },  // EO_ENTWRK_POPOFS
  { EA_NONE,    0 },  // EO_ENTWRK_POPREF
  { EA_NONE,    0 },  // EO_ENTWRK_PRINT
  { EA_NONE,    0 },  // EO_ENTWRK_RESET

  // index tree
  { EA_NONE,    0 },  // EO_IXT_DEL_ENT
  { EA_INT,     8 },  // EO_IXT_DUMP
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
  { EA_SUBSCR,  8 },  // EO_PAY_FETCH_COLI
  { EA_SUBSCR,  8 },  // EO_PAY_FETCH_COLS
  { EA_SUBSCR,  8 },  // EO_PAY_STORE_COLI
  { EA_SUBSCR,  8 },  // EO_PAY_STORE_COLS

  // pix
  { EA_NONE,    0 },  // EO_PIX_STM_PRINT
  
  // rsb / rowslab
  { EA_NONE,    0 },  // EO_RSB_DST_PRINT
  { EA_NONE,    0 },  // EO_RSB_DST_RESET
  { EA_NONE,    0 },  // EO_RSB_DST_NEW
  { EA_NONE,    0 },  // EO_RSB_DST_TEST
  { EA_NONE,    0 },  // EO_RSB_DST_ZERO
  { EA_NONE,    0 },  // EO_RSB_SRC_BEGIN
  { EA_NONE,    0 },  // EO_RSB_SRC_NEXT
  { EA_NONE,    0 },  // EO_RSB_SRC_PRINT
  { EA_NONE,    0 },  // EO_RSB_SRC_PUSHOFS
  { EA_NONE,    0 },  // EO_RSB_SRC_SETBYOFS
  { EA_NONE,    0 },  // EO_RSB_ALT_BEGIN
  { EA_NONE,    0 },  // EO_RSB_ALT_NEXT
  { EA_NONE,    0 },  // EO_RSB_ALT_PRINT
  { EA_NONE,    0 },  // EO_RSB_ALT_PUSHOFS
  { EA_NONE,    0 },  // EO_RSB_ALT_SETBYOFS

  // rsb operators
  { EA_SUBSCR,  8 },  // EO_RSB_DST_POPCOLI
  { EA_SUBSCR,  8 },  // EO_RSB_DST_POPCOLS
  { EA_SUBSCR,  8 },  // EO_RSB_DST_PUSHCOLI
  { EA_SUBSCR,  8 },  // EO_RSB_SRC_PUSHCOLI
  { EA_SUBSCR,  8 },  // EO_RSB_SRC_PUSHCOLS
  { EA_SUBSCR,  8 },  // EO_RSB_ALT_PUSHCOLI

  // -- end --
  { EA_NONE,    0 },  // EO_ENUMS
};
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
