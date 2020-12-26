/*============================================================================*/
/* eCP.h -- C++ Part                                                          */
/*============================================================================*/
#pragma once

enum eCP // c++ part
{
  /*  0 */ CP_NONE=0,
  
  /*    */ CP_ARRAY_SPEC,          // array spec
  /*    */ CP_BASE_BEGIN,          // base list
  /*    */ CP_BASE_END,            // base list end
  /*    */ CP_CLASS_BEGIN,         // class
  /*    */ CP_CLASS_END,           // class end
  /*    */ CP_CONST,               // const
  /*    */ CP_CONSTRUCTOR,         // constructor
  /*    */ CP_DESTRUCTOR,          // destructor
  /*    */ CP_EMPTY,               // ;
  /*    */ CP_ENUM,                // enum
  /*    */ CP_EXTERN,              // extern
  /*    */ CP_FORWARD,             // forward class
  /*    */ CP_FRIEND,              // friend
  /*    */ CP_FUNCTION,            // function
  /*    */ CP_FUNCTION_BODY,       // function
  /*    */ CP_FUNCTION_POINTER,    // function
  /*    */ CP_FUNVAR_NAME,         // function/variable name
  /*    */ CP_HASH_DEFINE,         // # define
  /*    */ CP_HASH_IF,             // # if
  /*    */ CP_HASH_IFDEF,          // # ifdef
  /*    */ CP_HASH_INCLUDE,        // # include
  /*    */ CP_HASH_PRAGMA_ONCE,    // # pragma once
  /*    */ CP_HASH_PRAGMA_PACK,    // # pragma pack
  /*    */ CP_INHERITED_CLASS,     // inherited class
  /*    */ CP_LABEL,               // class member label
  /*    */ CP_MEMBER_FUNCTION,     // member function
  /*    */ CP_MEMBER_VARIABLE,     // member variable
  /*    */ CP_MEMBERS_BEGIN,       // members
  /*    */ CP_MEMBERS_END,         // members end
  /*    */ CP_NAMESPACE_BEGIN,     // namespace
  /*    */ CP_NAMESPACE_END,       // end of namespace
  /*    */ CP_OPERATOR,            // function
  /*    */ CP_STATIC,              // static
  /*    */ CP_STRUCT,              // struct
  /*    */ CP_TEMPLATE_CLASS,      // template class
  /*    */ CP_TYPEDEF,             // typedef
  /*    */ CP_TYPE_ARGS,           // typetemplate args
  /*    */ CP_TYPE_SPECIFICATION,  // type specification
  /*    */ CP_UNION,               // union
  /*    */ CP_USING,               // using namespace
  /*    */ CP_VARIABLE,            // variable
  
  /*    */ CP_ENUMS
};

static Stz* aCP[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "CP_ARRAY_SPEC"_Stz,
  /*    */ "CP_BASE_BEGIN"_Stz,
  /*    */ "CP_BASE_END"_Stz,
  /*    */ "CP_CLASS_BEGIN"_Stz,
  /*    */ "CP_CLASS_END"_Stz,
  /*    */ "CP_CONST"_Stz,
  /*    */ "CP_CONSTRUCTOR"_Stz,
  /*    */ "CP_DESTRUCTOR"_Stz,
  /*    */ "CP_EMPTY"_Stz,
  /*    */ "CP_ENUM"_Stz,
  /*    */ "CP_EXTERN"_Stz,
  /*    */ "CP_FORWARD"_Stz,
  /*    */ "CP_FRIEND"_Stz,
  /*    */ "CP_FUNCTION"_Stz,
  /*    */ "CP_FUNCTION_BODY"_Stz,
  /*    */ "CP_FUNCTION_POINTER"_Stz,
  /*    */ "CP_FUNVAR_NAME"_Stz,
  /*    */ "CP_HASH_DEFINE"_Stz,
  /*    */ "CP_HASH_IF"_Stz,
  /*    */ "CP_HASH_IFDEF"_Stz,
  /*    */ "CP_HASH_INCLUDE"_Stz,
  /*    */ "CP_HASH_PRAGMA_ONCE"_Stz,
  /*    */ "CP_HASH_PRAGMA_PACK"_Stz,
  /*    */ "CP_INHERITED_CLASS"_Stz,
  /*    */ "CP_LABEL"_Stz,
  /*    */ "CP_MEMBER_FUNCTION"_Stz,
  /*    */ "CP_MEMBER_VARIABLE"_Stz,
  /*    */ "CP_MEMBERS_BEGIN"_Stz,
  /*    */ "CP_MEMBERS_END"_Stz,
  /*    */ "CP_NAMESPACE_BEGIN"_Stz,
  /*    */ "CP_NAMESPACE_END"_Stz,
  /*    */ "CP_OPERATOR"_Stz,
  /*    */ "CP_STATIC"_Stz,
  /*    */ "CP_STRUCT"_Stz,
  /*    */ "CP_TEMPLATE_CLASS"_Stz,
  /*    */ "CP_TYPEDEF"_Stz,
  /*    */ "CP_TYPE_ARGS"_Stz,
  /*    */ "CP_TYPE_SPECIFICATION"_Stz,
  /*    */ "CP_UNION"_Stz,
  /*    */ "CP_USING"_Stz,
  /*    */ "CP_VARIABLE"_Stz,
  
  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
