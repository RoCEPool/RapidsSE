/*============================================================================*/
/* RelationalOps.h -- The Standard Relational Operators                       */
/*============================================================================*/
#pragma once

namespace System
{
  enum eRO // Relational Op
  {
    /*  0 */ RO_NONE=0,
    
    /*  1 */ RO_LESS_THAN,
    /*  2 */ RO_LESS_EQUAL,
    /*  3 */ RO_EQUAL,
    /*  4 */ RO_NOT_EQUAL,
    /*  5 */ RO_GREATER_EQUAL,
    /*  6 */ RO_GREATER,
    /* 79 */ RO_ENUMS
  };
  
  static Str* aRO[]
  {
    /*  0 */ "NONE"_Str,
    
    /*  1 */ "<"_Str,
    /*  2 */ "<="_Str,
    /*  3 */ "=="_Str,
    /*  4 */ "!="_Str,
    /*  5 */ ">="_Str,
    /*  6 */ ">"_Str,
    
    /*  7 */ "ENUMS"_Str
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
