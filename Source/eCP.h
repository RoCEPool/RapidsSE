/*============================================================================*/
/* eCP.h -- C++ Part                                                          */
/*============================================================================*/
#pragma once

enum eCP // c++ part
{
  /*  0 */ CP_NONE=0,
  
  /*    */ CP_BLOCK,               // { members }   (in file, namespace, class)
  
  /*    */ CP_CONST,               // const       (in file, namespace, class)
  
  /*    */ CP_FILE_ENUM,           // enum        (in file)
  /*    */ CP_FILE_NAMESPACE,      // namespace   (in file)
  /*    */ CP_FILE_OTHER,          // other       (in file)
  /*    */ CP_FILE_STATIC,         // static      (in file)
  /*    */ CP_FILE_USING,          // using name  (in file)
  
  /*    */ CP_HASH_DEFINE,         // # define
  /*    */ CP_HASH_IF,             // # if
  /*    */ CP_HASH_IFDEF,          // # ifdef
  /*    */ CP_HASH_INCLUDE,        // # include
  /*    */ CP_HASH_PRAGMA,         // # pragma

  /*    */ CP_MEMBER_FRIEND,       // class member friend
  /*    */ CP_MEMBER_LABEL,        // class member label
  /*    */ CP_MEMBER_OTHER,        // class member other
  /*    */ CP_MEMBER_STATIC,       // class member static function
  
  /*    */ CP_NAMESPACE_CLASS,     // class            (in namespace)
  /*    */ CP_NAMESPACE_FORWARD,   // forward class    (in namespace)
  /*    */ CP_NAMESPACE_OTHER,     // other            (in namespace)
  /*    */ CP_NAMESPACE_STATIC,    // static           (in namespace)
  /*    */ CP_NAMESPACE_STRUCT,    // struct           (in namespace)
  /*    */ CP_NAMESPACE_TYPEDEF,   // typedef          (in namespace)
  /*    */ CP_NAMESPACE_UNION,     // union            (in namespace)

  /*    */ CP_ENUMS
};

static Stz* aCP[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "CP_BLOCK"_Stz,
  
  /*    */ "CP_CONST"_Stz,
  
  /*    */ "CP_FILE_ENUM"_Stz,
  /*    */ "CP_FILE_NAMESPACE"_Stz,
  /*    */ "CP_FILE_OTHER"_Stz,
  /*    */ "CP_FILE_STATIC"_Stz,
  /*    */ "CP_FILE_USING"_Stz,
  
  /*    */ "CP_HASH_DEFINE"_Stz,
  /*    */ "CP_HASH_IF"_Stz,
  /*    */ "CP_HASH_IFDEF"_Stz,
  /*    */ "CP_HASH_INCLUDE"_Stz,
  /*    */ "CP_HASH_PRAGMA"_Stz,

  /*    */ "CP_MEMBER_FRIEND"_Stz,
  /*    */ "CP_MEMBER_LABEL"_Stz,
  /*    */ "CP_MEMBER_OTHER"_Stz,
  /*    */ "CP_MEMBER_STATIC"_Stz,

  /*    */ "CP_NAMESPACE_CLASS"_Stz,
  /*    */ "CP_NAMESPACE_FORWARD"_Stz,
  /*    */ "CP_NAMESPACE_OTHER"_Stz,
  /*    */ "CP_NAMESPACE_STATIC"_Stz,
  /*    */ "CP_NAMESPACE_STRUCT"_Stz,
  /*    */ "CP_NAMESPACE_TYPEDEF"_Stz,
  /*    */ "CP_NAMESPACE_UNION"_Stz,

  /*    */ "CP_OTHER"_Stz,
  
  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
