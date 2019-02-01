/*============================================================================*/
/* RelationalOps.h -- The Standard Relational Operators                       */
/*============================================================================*/
#pragma once

namespace DBsuper
{
  enum eRO // Relational Op
  {
    /*  0 */ RO_NONE=0,
    
    /*  1 */ RO_LESS_THAN,
    /*  2 */ RO_LESS_EQUAL,
    /*  3 */ RO_EQUAL_TO,
    /*  4 */ RO_NOT_EQUAL,
    /*  5 */ RO_GREATER_EQUAL,
    /*  6 */ RO_GREATER_THAN,
  
    /*  7 */ RO_ENUMS
  };
  
  static Stz* aRO[]
  {
    /*  0 */ "NONE"_Stz,
    
    /*  1 */ "<"_Stz,
    /*  2 */ "<="_Stz,
    /*  3 */ "=="_Stz,
    /*  4 */ "!="_Stz,
    /*  5 */ ">="_Stz,
    /*  6 */ ">"_Stz,
    
    /*  7 */ "ENUMS"_Stz
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
