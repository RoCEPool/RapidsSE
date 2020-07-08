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

  /*    */ EO_INTEGER,
  /*    */ EO_PUSHNULL,
  /*    */ EO_PUSH0,
  /*    */ EO_PUSH1,

  /*    */ EO_ADD,
  /*    */ EO_DIVIDE,
  /*    */ EO_MODULO,
  /*    */ EO_MULTIPLY,
  /*    */ EO_NEGATE,
  /*    */ EO_SUBTRACT,

  /*    */ EO_LESSTHAN,
  /*    */ EO_LESSEQUAL,
  /*    */ EO_EQUAL,
  /*    */ EO_NOTEQUAL,
  /*    */ EO_GREATEREQUAL,
  /*    */ EO_GREATERTHAN,

  /*    */ EO_AND,
  /*    */ EO_OR,
  /*    */ EO_NOT,
  
  /*    */ EO_ISNULL,
  /*    */ EO_ISNOTNULL,
  
  /*    */ EO_INC,
  /*    */ EO_DEC,
  /*    */ EO_DUP,
  /*    */ EO_POP,
  /*    */ EO_PRINT,
  
  /*    */ EO_JMP,
  /*    */ EO_JMPFALSE,
  /*    */ EO_JMPTRUE,

  /*    */ EO_CALL,
  /*    */ EO_RETURN,

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
