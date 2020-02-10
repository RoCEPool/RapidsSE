/*============================================================================*/
/* eVP.h -- Vortex Syntax Element                                             */
/*============================================================================*/
#pragma once

enum eVP // c++ part
{
  /*  0 */ VP_NONE=0,
  
  /*    */ VP_ARRAY_SPEC,          // array spec
  /*    */ VP_BASE,                // base list
  /*    */ VP_BASE_END,            // base list end
  /*    */ VP_CLASS,               // class
  /*    */ VP_CLASS_END,           // class end
  /*    */ VP_CONST,               // const
  /*    */ VP_CONSTRUCTOR,         // constructor
  /*    */ VP_DESTRUCTOR,          // destructor
  /*    */ VP_EMPTY,               // ;
  /*    */ VP_ENUM,                // enum
  /*    */ VP_EXTERN,              // extern
  /*    */ VP_FIELD_BITS,          // field
  /*    */ VP_FORWARD_CLASS,       // forward class
  /*    */ VP_FRIEND_CLASS,        // friend
  /*    */ VP_FUNCTION,            // function
  /*    */ VP_FUNCTION_BODY,       // function
  /*    */ VP_FUNCTION_POINTER,    // function
  /*    */ VP_FUNVAR_NAME,         // function/variable name
  /*    */ VP_HASH,                // #
  /*    */ VP_HASH_DEFINE,         // # define
  /*    */ VP_HASH_IF,             // # if
  /*    */ VP_HASH_IFDEF,          // # ifdef
  /*    */ VP_HASH_INCLUDE,        // # include
  /*    */ VP_HASH_PRAGMA_ONCE,    // # pragma once
  /*    */ VP_HASH_PRAGMA_PACK,    // # pragma pack
  /*    */ VP_INHERITED_CLASS,     // inherited class
  /*    */ VP_INLINE,              // inhline
  /*    */ VP_LABEL,               // class member label
  /*    */ VP_MEMBER_FUNCTION,     // member function
  /*    */ VP_MEMBER_VARIABLE,     // member variable
  /*    */ VP_MEMBERS,             // members
  /*    */ VP_MEMBERS_END,         // members end
  /*    */ VP_NAMESPACE,           // namespace
  /*    */ VP_NAMESPACE_END,       // end of namespace
  /*    */ VP_NORETURN,            // [[ noreturn ]]
  /*    */ VP_OPERATOR,            // function
  /*    */ VP_PUBLIC,              // public
  /*    */ VP_STATIC,              // static
  /*    */ VP_STRUCT,              // struct
  /*    */ VP_STRUCT_END,          // struct end
  /*    */ VP_TEMPLATE,            // template
  /*    */ VP_TYPEDEF,             // typedef
  /*    */ VP_TYPE_ARGS,           // typetemplate args
  /*    */ VP_TYPE_SPECIFICATION,  // type specification
  /*    */ VP_UNSIGNED,            // unsigned
  /*    */ VP_UNION,               // union
  /*    */ VP_USING,               // using namespace
  /*    */ VP_VARIABLE,            // variable
  
  /*    */ VP_ENUMS
};

static Stz* aVP[]
{
  /*  0 */ "NONE"_Stz,
  
  /*    */ "VP_ARRAY_SPEC"_Stz,
  /*    */ "VP_BASE"_Stz,
  /*    */ "VP_BASE_END"_Stz,
  /*    */ "VP_CLASS"_Stz,
  /*    */ "VP_CLASS_END"_Stz,
  /*    */ "VP_CONST"_Stz,
  /*    */ "VP_CONSTRUCTOR"_Stz,
  /*    */ "VP_DESTRUCTOR"_Stz,
  /*    */ "VP_EMPTY"_Stz,
  /*    */ "VP_ENUM"_Stz,
  /*    */ "VP_EXTERN"_Stz,
  /*    */ "VP_FIELD_BITS"_Stz,
  /*    */ "VP_FORWARD_CLASS"_Stz,
  /*    */ "VP_FRIEND_CLASS"_Stz,
  /*    */ "VP_FUNCTION"_Stz,
  /*    */ "VP_FUNCTION_BODY"_Stz,
  /*    */ "VP_FUNCTION_POINTER"_Stz,
  /*    */ "VP_FUNVAR_NAME"_Stz,
  /*    */ "VP_HASH"_Stz,
  /*    */ "VP_HASH_DEFINE"_Stz,
  /*    */ "VP_HASH_IF"_Stz,
  /*    */ "VP_HASH_IFDEF"_Stz,
  /*    */ "VP_HASH_INCLUDE"_Stz,
  /*    */ "VP_HASH_PRAGMA_ONCE"_Stz,
  /*    */ "VP_HASH_PRAGMA_PACK"_Stz,
  /*    */ "VP_INHERITED_CLASS"_Stz,
  /*    */ "VP_INLINE"_Stz,
  /*    */ "VP_LABEL"_Stz,
  /*    */ "VP_MEMBER_FUNCTION"_Stz,
  /*    */ "VP_MEMBER_VARIABLE"_Stz,
  /*    */ "VP_MEMBERS"_Stz,
  /*    */ "VP_MEMBERS_END"_Stz,
  /*    */ "VP_NAMESPACE"_Stz,
  /*    */ "VP_NAMESPACE_END"_Stz,
  /*    */ "VP_NORETURN"_Stz,
  /*    */ "VP_OPERATOR"_Stz,
  /*    */ "VP_PUBLIC"_Stz,
  /*    */ "VP_STATIC"_Stz,
  /*    */ "VP_STRUCT"_Stz,
  /*    */ "VP_STRUCT_END"_Stz,
  /*    */ "VP_TEMPLATE"_Stz,
  /*    */ "VP_TYPEDEF"_Stz,
  /*    */ "VP_TYPE_ARGS"_Stz,
  /*    */ "VP_TYPE_SPECIFICATION"_Stz,
  /*    */ "VP_UNION"_Stz,
  /*    */ "VP_UNION_END"_Stz,
  /*    */ "VP_UNSIGNED"_Stz,
  /*    */ "VP_USING"_Stz,
  /*    */ "VP_VARIABLE"_Stz,
  
  /*    */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
