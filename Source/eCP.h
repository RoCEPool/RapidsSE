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
  /*    */ CP_CONSTRUCTOR,         // constructor
  /*    */ CP_DESTRUCTOR,          // destructor
  /*    */ CP_EMPTY,               // ;
  /*    */ CP_ENUM,                // enum
  /*    */ CP_FORWARD,             // forward class
  /*    */ CP_FRIEND,              // friend
  /*    */ CP_FUNCTION,            // function
  /*    */ CP_HASH_DEFINE,         // # define
  /*    */ CP_HASH_IF,             // # if
  /*    */ CP_HASH_IFDEF,          // # ifdef
  /*    */ CP_HASH_INCLUDE,        // # include
  /*    */ CP_HASH_PRAGMA_ONCE,    // # pragma once
  /*    */ CP_HASH_PRAGMA_PACK,    // # pragma pack
  /*    */ CP_LABEL,               // class member label
  /*    */ CP_MEMBER,              // member
  /*    */ CP_NAMESPACE,           // namespace
  /*    */ CP_OTHER,               // other  (to be refined)
  /*    */ CP_STATIC,              // static
  /*    */ CP_STRUCT,              // struct
  /*    */ CP_TYPEDEF,             // typedef
  /*    */ CP_UNION,               // union
  /*    */ CP_USING,               // using namespace
  /*    */ CP_VARIABLE,            // variable

  /*    */ CP_ENUMS
};

static Stz* aCP[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "CP_BLOCK"_Stz,
  /*    */ "CP_CLASS"_Stz,
  /*    */ "CP_CONST"_Stz,
  /*    */ "CP_CONSTRUCTOR"_Stz,
  /*    */ "CP_DESTRUCTOR"_Stz,
  /*    */ "CP_EMPTY"_Stz,
  /*    */ "CP_ENUM"_Stz,
  /*    */ "CP_FORWARD"_Stz,
  /*    */ "CP_FRIEND"_Stz,
  /*    */ "CP_FUNCTION"_Stz,
  /*    */ "CP_HASH_DEFINE"_Stz,
  /*    */ "CP_HASH_IF"_Stz,
  /*    */ "CP_HASH_IFDEF"_Stz,
  /*    */ "CP_HASH_INCLUDE"_Stz,
  /*    */ "CP_HASH_PRAGMA_ONCE"_Stz,
  /*    */ "CP_HASH_PRAGMA_PACK"_Stz,
  /*    */ "CP_LABEL"_Stz,
  /*    */ "CP_MEMBER"_Stz,
  /*    */ "CP_NAMESPACE"_Stz,
  /*    */ "CP_OTHER"_Stz,
  /*    */ "CP_STATIC"_Stz,
  /*    */ "CP_STRUCT"_Stz,
  /*    */ "CP_TYPEDEF"_Stz,
  /*    */ "CP_UNION"_Stz,
  /*    */ "CP_USING"_Stz,
  /*    */ "CP_VARIABLE"_Stz,

  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
