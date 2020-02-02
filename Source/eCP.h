/*============================================================================*/
/* eCP.h -- C++ Part                                                          */
/*============================================================================*/
#pragma once

enum eCP // c++ part
{
  /*  0 */ CP_NONE=0,
  
  /*    */ CP_BLOCK,               // class
  /*    */ CP_CLASS,               // class
  /*    */ CP_CONST,               // const
  /*    */ CP_EMPTY,               // ;
  /*    */ CP_ENUM,                // enum
  /*    */ CP_FORWARD,             // forward class
  /*    */ CP_FRIEND,              // friend
  /*    */ CP_HASH_DEFINE,         // # define
  /*    */ CP_HASH_IF,             // # if
  /*    */ CP_HASH_IFDEF,          // # ifdef
  /*    */ CP_HASH_INCLUDE,        // # include
  /*    */ CP_HASH_PRAGMA_ONCE,    // # pragma once
  /*    */ CP_HASH_PRAGMA_PACK,    // # pragma pack
  /*    */ CP_LABEL,               // class member label
  /*    */ CP_NAMESPACE,           // namespace
  /*    */ CP_OTHER,               // other       (in file)
  /*    */ CP_STATIC,              // static
  /*    */ CP_STRUCT,              // struct
  /*    */ CP_TYPEDEF,             // typedef
  /*    */ CP_UNION,               // union
  /*    */ CP_USING,               // using namespace

  /*    */ CP_ENUMS
};

static Stz* aCP[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "CP_BLOCK"_Stz,
  
  /*    */ "CP_CONST"_Stz,
  /*    */ "CP_SEMICOLON"_Stz,

  /*    */ "CP_FILE_ENUM"_Stz,
  /*    */ "CP_FILE_NAMESPACE"_Stz,
  /*    */ "CP_FILE_OTHER"_Stz,
  /*    */ "CP_FILE_STATIC"_Stz,
  /*    */ "CP_FILE_USING"_Stz,
  
  /*    */ "CP_HASH_DEFINE"_Stz,
  /*    */ "CP_HASH_IF"_Stz,
  /*    */ "CP_HASH_IFDEF"_Stz,
  /*    */ "CP_HASH_INCLUDE"_Stz,
  /*    */ "CP_HASH_PRAGMA_ONCE"_Stz,
  /*    */ "CP_HASH_PRAGMA_PACK"_Stz,

  /*    */ "CP_MEMBER_FRIEND"_Stz,
  /*    */ "CP_MEMBER_LABEL"_Stz,
  /*    */ "CP_MEMBER_OTHER"_Stz,
  /*    */ "CP_MEMBER_STATIC"_Stz,
  /*    */ "CP_MEMBER_STRUCT"_Stz,

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
