/*============================================================================*/
/* eCP.h -- C++ Part                                                          */
/*============================================================================*/
#pragma once

enum eCP // c++ part
{
  /*  0 */ CP_NONE=0,
  
  /*    */ CP_CLASS,          // class
  /*    */ CP_ENUM,           // enum
  /*    */ CP_FILE_INCLUDE,   // # include
  /*    */ CP_FORWARD_CLASS,  // class <classNm> ;
  /*    */ CP_HASH_DEFINE,    // # define
  /*    */ CP_MACRO,          // macro
  /*    */ CP_MEMBER_FUN,     // member function
  /*    */ CP_MEMBER_VAR,     // member variable
  /*    */ CP_NAMESPACE,      // namespace <namespaceNm> { ... }
  /*    */ CP_NAMESPACE_FUN,  // namespace function
  /*    */ CP_PRAGMA_ONCE,    // #pragma once
  /*    */ CP_TEMPLATE,       // template
  
  /*    */ CP_ENUMS
};

static Stz* aCP[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "CP_CLASS"_Stz,
  /*    */ "CP_ENUM"_Stz,
  /*    */ "CP_FILE_INCLUDE"_Stz,
  /*    */ "CP_FORWARD_CLASS"_Stz,
  /*    */ "CP_HASH_DEFINE"_Stz,
  /*    */ "CP_MACRO"_Stz,
  /*    */ "CP_MEMBER_FUN"_Stz,
  /*    */ "CP_MEMBER_FUN"_Stz,
  /*    */ "CP_NAMESPACE"_Stz,
  /*    */ "CP_NAMESPACE_FUN"_Stz,
  /*    */ "CP_PRAGMA_ONCE"_Stz,
  /*    */ "CP_TEMPLATE"_Stz,
  
  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
