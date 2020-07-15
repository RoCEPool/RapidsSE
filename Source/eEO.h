/*============================================================================*/
/* eEO.h -- Expression Operators                                              */
/*============================================================================*/
#pragma once

enum eEO : i1 // expression operators
{
  /*  0 */ EO_NONE=0,
  
  // arithmetic operators
  /*    */ EO_ADD,              // add top two int's push result
  /*    */ EO_DIVIDE,           // divide top int by second push result
  /*    */ EO_MODULO,           // modulo top int be second push result
  /*    */ EO_MULTIPLY,         // multiply top two int's push result
  /*    */ EO_NEGATE,           // negate the top value
  /*    */ EO_SUBTRACT,         // subtract second int from top push result

  // branch operators
  /*    */ EO_JMP,              // jump to location
  /*    */ EO_JMPFALSE,         // pop top and jump to location if false
  /*    */ EO_JMPTRUE,          // pop top and jump to location if true
  
  // entry
  /*    */ EO_ENT_CREATEPAY,    // create a payload in entwrk
  /*    */ EO_ENT_INSERT,       // insert an entry into the indexTree
  /*    */ EO_ENT_INSERTPAY,    // insert a payload into an entry
  /*    */ EO_ENT_NEXT,         // move to next m_pEnt
  /*    */ EO_ENT_POSPRIOR,     // position m_pEnt prior using m_oEntWrk.key
  /*    */ EO_ENT_PUSH,         // push entry
  /*    */ EO_ENT_PUSHREF,      // push ref of current entry

  // entry relational operators
  /*    */ EO_ENT_LESSTHAN,     // test if m_pEnt.key <  m_oEntWrk.key
  /*    */ EO_ENT_LESSEQUAL,    // test if m_pEnt.key <= m_oEntWrk.key
  /*    */ EO_ENT_EQUAL,        // test if m_pEnt.key == m_oEntWrk.key
  /*    */ EO_ENT_NOTEQUAL,     // test if m_pEnt.key != m_oEntWrk.key
  /*    */ EO_ENT_GREATEREQUAL, // test if m_pEnt.key >= m_oEntWrk.key
  /*    */ EO_ENT_GREATERTHAN,  // test if m_pEnt.key >  m_oEntWrk.key
  
  // entWrk
  /*    */ EO_ENTWRK_APPENDELM, // append next element value
  /*    */ EO_ENTWRK_APPENDELMI,// append next element value (int)
  /*    */ EO_ENTWRK_POPOFS,    // pop m_oEntWrk.ref.buffer/offset
  /*    */ EO_ENTWRK_POPREF,    // pop m_oEntWrk.ref
  /*    */ EO_ENTWRK_PRINT,     // print m_oEntWrk
  /*    */ EO_ENTWRK_RESET,     // reset m_oEntWrk
  
  // finalise
  /*    */ EO_FIN_TRANSFORM,    // standard finalise

  // initialise
  /*    */ EO_INI_PIX_DEFOUT,   // init m_pPot pix from stream def out
  /*    */ EO_INI_PIX_SLABIN,   // init m_pPot pix from input slab
  /*    */ EO_INI_SRC_SLAB,     // init m_pSrcRowSlab etc

  // index tree
  /*    */ EO_IXT_INS_ENT,      // insert ent
  /*    */ EO_IXT_TRV_BEGIN,    // begin linear traverse
  /*    */ EO_IXT_TRV_NEXT,     // move to next entry
 
  // logical operators
  /*    */ EO_AND,              // logical and top two int's, push result
  /*    */ EO_OR,               // logical or top two int's, push result
  /*    */ EO_NOT,              // logical not top int, push result
  
  // null test operators
  /*    */ EO_ISNULL,           // test if top is null, push result
  /*    */ EO_ISNOTNULL,        // test if top is not null, push result
  
  // pay load
  /*    */ EO_PAY_FETCH_COL,    // fetch a column from the entry payload
  /*    */ EO_PAY_STORE_COL,    // store a column of the entry payload
  
  // pushes
  /*    */ EO_DATE,             // push date
  /*    */ EO_INTEGER,          // push integer
  /*    */ EO_PUSHNULL,         // push null
  /*    */ EO_PUSH0,            // push integer 0
  /*    */ EO_PUSH1,            // push integer 1

  // ref
  /*    */ EO_REF_GETCRW,       // get ref'd crw
  /*    */ EO_REF_SRCRSB,       // get ref'd src rsb
  /*    */ EO_REF_ALTRSB,       // get ref'd alt rsb

  // rsb / rowslab
  /*    */ EO_RSB_DST_COPYSRC,  // copy src rsb to dst rsb
  /*    */ EO_RSB_DST_PRINT,    // print dst rsb
  /*    */ EO_RSB_DST_NEW,      // create new destination rsb
  /*    */ EO_RSB_DST_TEST,     // test if destination rsb
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

  // relational operators
  /*    */ EO_LESSTHAN,         // test top < 2nd, push result
  /*    */ EO_LESSEQUAL,        // test top <= 2nd, push result
  /*    */ EO_EQUAL,            // test top == 2nd, push result
  /*    */ EO_NOTEQUAL,         // test top != 2nd, push result
  /*    */ EO_GREATEREQUAL,     // test top >= 2nd, push result
  /*    */ EO_GREATERTHAN,      // test top > 2nd, push result

  // rsb operators
  /*    */ EO_DEPOSITCOLI,      // pop and deposit top into dst column
  /*    */ EO_PUSHDSTCOLI,      // push dst column
  /*    */ EO_PUSHSRCCOLI,      // push src column
  /*    */ EO_PUSHALTCOLI,      // push alt column
  
  // stack operators
  /*    */ EO_INC,              // increment top, push result
  /*    */ EO_DEC,              // decrement top, push result
  /*    */ EO_DUP,              // duplicate top
  /*    */ EO_POP,              // pop / discard top
  /*    */ EO_PRINT,            // print top

  // subroutine operators
  /*    */ EO_CALL,             // call subroutine at location
  /*    */ EO_RETURN,           // return from subroutine and

  // misc
  /*    */ EO_TRACE,            // set trace
  /*    */ EO_NOOP,             // no op
  
  /*    */ EO_ENUMS
};

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
  
  // branch operators
  /*    */ "EO_JMP"_Stz,
  /*    */ "EO_JMPFALSE"_Stz,
  /*    */ "EO_JMPTRUE"_Stz,

  // entry
  /*    */ "EO_ENT_CREATEPAY"_Stz,
  /*    */ "EO_ENT_INSERT"_Stz,
  /*    */ "EO_ENT_INSERTPAY"_Stz,
  /*    */ "EO_ENT_NEXT"_Stz,
  /*    */ "EO_ENT_POSPRIOR"_Stz,
  /*    */ "EO_ENT_PUSH"_Stz,
  /*    */ "EO_ENT_PUSHREF"_Stz,

  // entry relational operators
  /*    */ "EO_ENT_LESSTHAN"_Stz,
  /*    */ "EO_ENT_LESSEQUAL"_Stz,
  /*    */ "EO_ENT_EQUAL"_Stz,
  /*    */ "EO_ENT_NOTEQUAL"_Stz,
  /*    */ "EO_ENT_GREATEREQUAL"_Stz,
  /*    */ "EO_ENT_GREATERTHAN"_Stz,
  
  // entWrk
  /*    */ "EO_ENTWRK_APPENDELM"_Stz,
  /*    */ "EO_ENTWRK_APPENDELMI"_Stz,
  /*    */ "EO_ENTWRK_POPOFS"_Stz,
  /*    */ "EO_ENTWRK_POPREF"_Stz,
  /*    */ "EO_ENTWRK_PRINT"_Stz,
  /*    */ "EO_ENTWRK_RESET"_Stz,

  // finalise
  /*    */ "EO_FIN_TRANSFORM"_Stz,

  // initialise
  /*    */ "EO_INI_PIX_DEFOUT"_Stz,
  /*    */ "EO_INI_PIX_SLABIN"_Stz,
  /*    */ "EO_INI_SRC_SLAB"_Stz,

  // index tree
  /*    */ "EO_IXT_INS_ENT"_Stz,
  /*    */ "EO_IXT_TRV_BEGIN"_Stz,
  /*    */ "EO_IXT_TRV_NEXT"_Stz,

  // logical operators
  /*    */ "EO_AND"_Stz,
  /*    */ "EO_OR"_Stz,
  /*    */ "EO_NOT"_Stz,
  
  // null test operators
  /*    */ "EO_ISNULL"_Stz,
  /*    */ "EO_ISNOTNULL"_Stz,
  
  // pay load
  /*    */ "EO_PAY_FETCH_COL"_Stz,
  /*    */ "EO_PAY_STORE_COL"_Stz,
  
  // pushes
  /*    */ "EO_DATE"_Stz,
  /*    */ "EO_INTEGER"_Stz,
  /*    */ "EO_PUSHNULL"_Stz,
  /*    */ "EO_PUSH0"_Stz,
  /*    */ "EO_PUSH1"_Stz,

  // ref
  /*    */ "EO_REF_GETCRW"_Stz,
  /*    */ "EO_REF_SRCRSB"_Stz,
  /*    */ "EO_REF_ALTRSB"_Stz,
  
  // rsb / rowslab
  /*    */ "EO_RSB_DST_COPYSRC"_Stz,
  /*    */ "EO_RSB_DST_PRINT"_Stz,
  /*    */ "EO_RSB_DST_NEW"_Stz,
  /*    */ "EO_RSB_DST_TEST"_Stz,
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

  // relational operators
  /*    */ "EO_LESSTHAN"_Stz,
  /*    */ "EO_LESSEQUAL"_Stz,
  /*    */ "EO_EQUAL"_Stz,
  /*    */ "EO_NOTEQUAL"_Stz,
  /*    */ "EO_GREATEREQUAL"_Stz,
  /*    */ "EO_GREATERTHAN"_Stz,
  
  // rsb operators
  /*    */ "EO_DEPOSITCOLI"_Stz,
  /*    */ "EO_PUSHDSTCOLI"_Stz,
  /*    */ "EO_PUSHSRCCOLI"_Stz,
  /*    */ "EO_PUSHALTCOLI"_Stz,
  
  // stack operators
  /*    */ "EO_INC"_Stz,
  /*    */ "EO_DEC"_Stz,
  /*    */ "EO_DUP"_Stz,
  /*    */ "EO_POP"_Stz,
  /*    */ "EO_PRINT"_Stz,

  // subroutine operators
  /*    */ "EO_CALL"_Stz,
  /*    */ "EO_RETURN"_Stz,

  // misc
  /*    */ "EO_TRACE"_Stz,
  /*    */ "EO_NOOP"_Stz,

  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
