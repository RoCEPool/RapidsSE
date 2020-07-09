/*============================================================================*/
/* eEO.h -- Expression Operators                                              */
/*============================================================================*/
#pragma once

enum eEO : i1 // expression operators
{
  /*  0 */ EO_NONE=0,
  
  // arithmetic operators
  /*    */ EO_ADD,                // add top two int's push result
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
  /*    */ EO_ENT_CREATEPAY,    // create a payload entry
  /*    */ EO_ENT_INSERTPAY,    // insert a payload entry into the indexTree
  /*    */ EO_ENT_NEXT,         // move to next m_pEnt
  /*    */ EO_ENT_POSPRIOR,     // position m_pEnt prior using m_oEntWrk.key
  
  // entry relational operators
  /*    */ EO_ENT_LESSTHAN,     // test if m_pEnt.key <  m_oEntWrk.key
  /*    */ EO_ENT_LESSEQUAL,    // test if m_pEnt.key <= m_oEntWrk.key
  /*    */ EO_ENT_EQUAL,        // test if m_pEnt.key == m_oEntWrk.key
  /*    */ EO_ENT_NOTEQUAL,     // test if m_pEnt.key != m_oEntWrk.key
  /*    */ EO_ENT_GREATEREQUAL, // test if m_pEnt.key >= m_oEntWrk.key
  /*    */ EO_ENT_GREATERTHAN,  // test if m_pEnt.key >  m_oEntWrk.key
  
  // entWrk
  /*    */ EO_ENTWRK_APPENDELMI,   // append next element value (int)
  /*    */ EO_ENTWRK_RESET,        // reset m_oEntWrk
  
  // logical operators
  /*    */ EO_AND,              // logical and top two int's, push result
  /*    */ EO_OR,               // logical or top two int's, push result
  /*    */ EO_NOT,              // logical not top int, push result
  
  // null test operators
  /*    */ EO_ISNULL,           // test if top is null, push result
  /*    */ EO_ISNOTNULL,        // test if top is not null, push result
  
  // pay load
  /*    */ EO_PAY_FETCH_COL,    // fetch a column from the entry payload
  /*    */ EO_PAY_STORE_COL,    // store in column of the entry payload
  
  // pushes
  /*    */ EO_INTEGER,          // push integer
  /*    */ EO_PUSHNULL,         // push null
  /*    */ EO_PUSH0,            // push integer 0
  /*    */ EO_PUSH1,            // push integer 1

  // relational operators
  /*    */ EO_LESSTHAN,         // test top < 2nd, push result
  /*    */ EO_LESSEQUAL,        // test top <= 2nd, push result
  /*    */ EO_EQUAL,            // test top == 2nd, push result
  /*    */ EO_NOTEQUAL,         // test top != 2nd, push result
  /*    */ EO_GREATEREQUAL,     // test top >= 2nd, push result
  /*    */ EO_GREATERTHAN,      // test top > 2nd, push result

  // rsb operators
  /*    */ EO_COPYRSB,          // copy src rsb to dst rsb       IS THIS NEEDED?
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
  /*    */ "EO_ENT_INSERTPAY"_Stz,
  /*    */ "EO_ENT_NEXT"_Stz,
  /*    */ "EO_ENT_POSPRIOR"_Stz,
  
  // entry relational operators
  /*    */ "EO_ENT_LESSTHAN"_Stz,
  /*    */ "EO_ENT_LESSEQUAL"_Stz,
  /*    */ "EO_ENT_EQUAL"_Stz,
  /*    */ "EO_ENT_NOTEQUAL"_Stz,
  /*    */ "EO_ENT_GREATEREQUAL"_Stz,
  /*    */ "EO_ENT_GREATERTHAN"_Stz,
  
  // entWrk
  /*    */ "EO_ENTWRK_APPENDELMI"_Stz,
  /*    */ "EO_ENTWRK_RESET"_Stz,

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
  /*    */ "EO_INTEGER"_Stz,
  /*    */ "EO_PUSHNULL"_Stz,
  /*    */ "EO_PUSH0"_Stz,
  /*    */ "EO_PUSH1"_Stz,

  // relational operators
  /*    */ "EO_LESSTHAN"_Stz,
  /*    */ "EO_LESSEQUAL"_Stz,
  /*    */ "EO_EQUAL"_Stz,
  /*    */ "EO_NOTEQUAL"_Stz,
  /*    */ "EO_GREATEREQUAL"_Stz,
  /*    */ "EO_GREATERTHAN"_Stz,
  
  // rsb operators
  /*    */ "EO_COPYRSB"_Stz,
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

  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
