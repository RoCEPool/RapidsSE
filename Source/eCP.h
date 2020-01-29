/*============================================================================*/
/* eCP.h -- C++ Part                                                          */
/*============================================================================*/
#pragma once

enum eCP // c++ part
{
  /*  0 */ CP_NONE=0,
  
  /*    */ CP_FILE_ENUM,           // enum        (in file)
  /*    */ CP_FILE_FUNCTION,       // function    (in file)
  /*    */ CP_FILE_NAMESPACE,      // namespace   (in file)
  /*    */ CP_FILE_STATIC,         // static      (in file)
  /*    */ CP_FILE_USING,          // using name  (in file)
  
  /*    */ CP_HASH_DEFINE,         // # define
  /*    */ CP_HASH_INCLUDE,        // # include
  /*    */ CP_HASH_PRAGMA,         // # pragma
  
  /*    */ CP_MEMBER_FUNCTION,     // class member function
  /*    */ CP_MEMBER_STATIC,       // class member static function
  /*    */ CP_MEMBER_TEMPLATE_FUN, // class member templated function
  /*    */ CP_MEMBER_TEMPLATE_VAR, // class member templated variable
  /*    */ CP_MEMBER_VARIABLE,     // class member variable
  
  /*    */ CP_NAMESPACE_CLASS,     // class            (in namespace)
  /*    */ CP_NAMESPACE_FORWARD,   // forward class    (in namespace)
  /*    */ CP_NAMESPACE_FUNCTION,  // function         (in namespace)
  /*    */ CP_NAMESPACE_STATIC,    // static           (in namespace)
  /*    */ CP_NAMESPACE_STRUCT,    // struct           (in namespace)
  /*    */ CP_NAMESPACE_TEMPLATE,  // forward template (in namespace)
  /*    */ CP_NAMESPACE_TYPEDEF,   // typedef          (in namespace)

  /*    */ CP_ENUMS
};

static Stz* aCP[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "CP_FILE_ENUM"_Stz,
  /*    */ "CP_FILE_FUNCTION"_Stz,
  /*    */ "CP_FILE_NAMESPACE"_Stz,
  /*    */ "CP_FILE_STATIC"_Stz,
  /*    */ "CP_FILE_USING"_Stz,
  
  /*    */ "CP_HASH_DEFINE"_Stz,
  /*    */ "CP_HASH_INCLUDE"_Stz,
  /*    */ "CP_HASH_PRAGMA"_Stz,

  /*    */ "CP_MEMBER_FUNCTION"_Stz,
  /*    */ "CP_MEMBER_STATIC"_Stz,
  /*    */ "CP_MEMBER_TEMPLATE_FUN"_Stz,
  /*    */ "CP_MEMBER_TEMPLATE_VAR"_Stz,
  /*    */ "CP_MEMBER_VARIABLE"_Stz,

  /*    */ "CP_NAMESPACE_CLASS"_Stz,
  /*    */ "CP_NAMESPACE_FORWARD"_Stz,
  /*    */ "CP_NAMESPACE_FUNCTION"_Stz,
  /*    */ "CP_NAMESPACE_STATIC"_Stz,
  /*    */ "CP_NAMESPACE_TEMPLATE"_Stz,
  /*    */ "CP_NAMESPACE_TYPEDEF"_Stz,

  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
