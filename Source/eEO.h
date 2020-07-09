/*============================================================================*/
/* eEO.h -- Expression Operators                                              */
/*============================================================================*/
#pragma once

enum eEO : i1 // expression operators
{
  /*  0 */ EO_NONE=0,
  
  /*    */ EO_RESETENTWRK,      // reset m_oEntWrk
  /*    */ EO_APPENDELMI,       // append next element value (int)
  /*    */ EO_POSENTPRIOR,      // position m_pEnt prior using m_oEntWrk.key
  /*    */ EO_NEXTENT,          // move to next m_pEnt
  
  /*    */ EO_ENT_LESSTHAN,     // test if m_pEnt.key <  m_oEntWrk.key
  /*    */ EO_ENT_LESSEQUAL,    // test if m_pEnt.key <= m_oEntWrk.key
  /*    */ EO_ENT_EQUAL,        // test if m_pEnt.key == m_oEntWrk.key
  /*    */ EO_ENT_NOTEQUAL,     // test if m_pEnt.key != m_oEntWrk.key
  /*    */ EO_ENT_GREATEREQUAL, // test if m_pEnt.key >= m_oEntWrk.key
  /*    */ EO_ENT_GREATERTHAN,  // test if m_pEnt.key >  m_oEntWrk.key

  /*    */ EO_COPYRSB,          // copy src rsb to dst rsb       IS THIS NEEDED?
  /*    */ EO_DEPOSITCOLI,      // pop and deposit top into dst column
  /*    */ EO_PUSHDSTCOLI,      // push dst column
  /*    */ EO_PUSHSRCCOLI,      // push src column
  /*    */ EO_PUSHALTCOLI,      // push alt column

  /*    */ EO_INTEGER,          // push integer
  /*    */ EO_PUSHNULL,         // push null
  /*    */ EO_PUSH0,            // push integer 0
  /*    */ EO_PUSH1,            // push integer 1

  /*    */ EO_ADD,              // add top two int's push result
  /*    */ EO_DIVIDE,           // divide top int by second push result
  /*    */ EO_MODULO,           // modulo top int be second push result
  /*    */ EO_MULTIPLY,         // multiply top two int's push result
  /*    */ EO_NEGATE,           // negate the top value
  /*    */ EO_SUBTRACT,         // subtract second int from top push result

  /*    */ EO_LESSTHAN,         // test top < 2nd, push result
  /*    */ EO_LESSEQUAL,        // test top <= 2nd, push result
  /*    */ EO_EQUAL,            // test top == 2nd, push result
  /*    */ EO_NOTEQUAL,         // test top != 2nd, push result
  /*    */ EO_GREATEREQUAL,     // test top >= 2nd, push result
  /*    */ EO_GREATERTHAN,      // test top > 2nd, push result

  /*    */ EO_AND,              // logical and top two int's, push result
  /*    */ EO_OR,               // logical or top two int's, push result
  /*    */ EO_NOT,              // logical not top int, push result
  
  /*    */ EO_ISNULL,           // test if top is null, push result
  /*    */ EO_ISNOTNULL,        // test if top is not null, push result
  
  /*    */ EO_INC,              // increment top, push result
  /*    */ EO_DEC,              // decrement top, push result
  /*    */ EO_DUP,              // duplicate top
  /*    */ EO_POP,              // pop / discard top
  /*    */ EO_PRINT,            // print top
  
  /*    */ EO_JMP,              // jump to location
  /*    */ EO_JMPFALSE,         // pop top and jump to location if false
  /*    */ EO_JMPTRUE,          // pop top and jump to location if true

  /*    */ EO_CALL,             // call subroutine at location
  /*    */ EO_RETURN,           // return from subroutine and

  /*    */ EO_ENUMS
};

static Stz* aEO[]
{
  /*  0 */ "NONE"_Stz,
    
  /*    */ "EO_RESETENTWRK"_Stz,
  /*    */ "EO_APPENDELMI"_Stz,
  /*    */ "EO_POSENTPRIOR"_Stz,
  /*    */ "EO_NEXTENT"_Stz,
  
  /*    */ "EO_ENT_LESSTHAN"_Stz,
  /*    */ "EO_ENT_LESSEQUAL"_Stz,
  /*    */ "EO_ENT_EQUAL"_Stz,
  /*    */ "EO_ENT_NOTEQUAL"_Stz,
  /*    */ "EO_ENT_GREATEREQUAL"_Stz,
  /*    */ "EO_ENT_GREATERTHAN"_Stz,

  /*    */ "EO_COPYRSB"_Stz,
  /*    */ "EO_DEPOSITCOLI"_Stz,
  /*    */ "EO_PUSHDSTCOLI"_Stz,
  /*    */ "EO_PUSHSRCCOLI"_Stz,
  /*    */ "EO_PUSHALTCOLI"_Stz,

  /*    */ "EO_INTEGER"_Stz,
  /*    */ "EO_PUSHNULL"_Stz,
  /*    */ "EO_PUSH0"_Stz,
  /*    */ "EO_PUSH1"_Stz,

  /*    */ "EO_ADD"_Stz,
  /*    */ "EO_DIVIDE"_Stz,
  /*    */ "EO_MODULO"_Stz,
  /*    */ "EO_MULTIPLY"_Stz,
  /*    */ "EO_NEGATE"_Stz,
  /*    */ "EO_SUBTRACT"_Stz,

  /*    */ "EO_LESSTHAN"_Stz,
  /*    */ "EO_LESSEQUAL"_Stz,
  /*    */ "EO_EQUAL"_Stz,
  /*    */ "EO_NOTEQUAL"_Stz,
  /*    */ "EO_GREATEREQUAL"_Stz,
  /*    */ "EO_GREATERTHAN"_Stz,

  /*    */ "EO_AND"_Stz,
  /*    */ "EO_OR"_Stz,
  /*    */ "EO_NOT"_Stz,

  /*    */ "EO_ISNULL"_Stz,
  /*    */ "EO_ISNOTNULL"_Stz,
  
  /*    */ "EO_INC"_Stz,
  /*    */ "EO_DEC"_Stz,
  /*    */ "EO_DUP"_Stz,
  /*    */ "EO_POP"_Stz,
  /*    */ "EO_PRINT"_Stz,

  /*    */ "EO_JMP"_Stz,
  /*    */ "EO_JMPFALSE"_Stz,
  /*    */ "EO_JMPTRUE"_Stz,

  /*    */ "EO_CALL"_Stz,
  /*    */ "EO_RETURN"_Stz,

  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
