/*============================================================================*/
/* eCP.h -- C++ Part                                                          */
/*============================================================================*/
#pragma once

enum eCP // c++ part
{
  /*  0 */ CP_NONE=0,
  
  /*    */ CP_CLASS,    // class
  /*    */ CP_ENUM,     // enum
  /*    */ CP_INCLUDE,  // #include
  /*    */ CP_MBRFUN,   // member function
  /*    */ CP_MBRVAR,   // member variable
  /*    */ CP_ONCE,     // #pragma once
  /*    */ CP_TEMPLATE, // template
  
  /*    */ CP_ENUMS
};

static Stz* aCP[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "CP_CLASS"_Stz,
  /*    */ "CP_ENUM"_Stz,
  /*    */ "CP_INCLUDE"_Stz,
  /*    */ "CP_MBRFUN"_Stz,
  /*    */ "CP_MBRVAR"_Stz,
  /*    */ "CP_ONCE"_Stz,
  /*    */ "CP_TEMPLATE"_Stz,
  
  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
