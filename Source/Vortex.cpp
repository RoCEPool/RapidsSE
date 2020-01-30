/*============================================================================*/
/* Vortex.cpp - Vortex                                                        */
/*============================================================================*/

#include "Utilities.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Utilities,Vortex,CN_VORTEX)
__aNewFunction(Vortex,CN_VORTEX)
__vDeleteFunction(Vortex,CN_VORTEX)
__aFooterFunction(Vortex)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Vortex::Vortex(void)
: Object(CN_VORTEX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_pOutputFile  = nullptr;
  m_sSourcePath  = ""_Stz;
  m_pInput       = nullptr;
  
  m_pMetaGraph   = new MetaGraph;
  m_p_ConstSet   = m_pMetaGraph->pConstSet();
  m_p_MetaBlock  = nullptr;
  m_p_MetaMember = nullptr;
  
  //m_oFunVarTerms
  m_bTraceTokens = false;
  
  m_bInClass     = false;
  m_sClassNm     = C_NO_NAME;
  m_bInNamespace = false;
  m_sNamespaceNm = C_NO_NAME;
  
  m_sPart        = ""_Stz;

  _checkInitialised_(CN_VORTEX);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Vortex::~Vortex(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  delete m_pOutputFile;
  delete m_pInput;
  
  delete m_pMetaGraph;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

#define CHECKED(fun) if (! fun) vThrowProblem(__LINE__);

/*----------------------------------------------------------------------------*/
/* close the vortex                                                           */
/*----------------------------------------------------------------------------*/
void Vortex::vClose(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  m_pMetaGraph->vRecursivePrint();
  
  if (m_pOutputFile && m_pOutputFile->bIsOpen())
    m_pOutputFile->vClose();
  
  delete m_pOutputFile;
  m_pOutputFile = nullptr;
}

/*----------------------------------------------------------------------------*/
/* open the vortex                                                            */
/*----------------------------------------------------------------------------*/
bool Vortex::bOpen(Stz* szVortexPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  RASSERT(! m_pOutputFile);
  
  m_pOutputFile = new File(nullptr);
  
  bool b = m_pOutputFile->bOpenWrite(szVortexPath);
  RASSERT(b);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* process a source file (.h or .hpp)                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vProcessSource(Stz* szSourcePath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  vTrace("\n\n\n"_Stz);
  vTrace("-------------------------------------------------------------\n"_Stz);
  vTrace("Parsing file %s\n"_Stz,szSourcePath->azChar());
  
  m_sSourcePath = szSourcePath;
  m_pInput = new Input;
  m_pInput->vPushFile(szSourcePath);
  
  //vProcAll();
  bool b = bParseFile();
  
  if (! b) {
    vTrace("\n\n"_Stz);
    vTrace("** Unable to fully parse %s\n"_Stz,szSourcePath->azChar());
    vTrace("** Last Part: %s\n"_Stz,m_sPart.azChar());
    vTrace("\n\n"_Stz);
    vDebugPoint();
  }
  
  delete m_pInput;
  m_pInput = nullptr;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* process all parts in the current source file (.h or .hpp)                  */
/*----------------------------------------------------------------------------*/
bool Vortex::bParseFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  RASSERT(! m_bInNamespace);
  RASSERT(! m_bInClass);

  m_bTraceTokens = true;
  
  // -- parse till EOF --
  Token xTkn;
  while (bGetNextToken(xTkn)) {
  
    try {
      Stz* szKeyWord = xTkn.szTerm();
        
      if (szKeyWord->bSame("class"_Stz)) {
        // in SE we don't expect a class outside of a namespace
        vThrowProblem(__LINE__);
      }
      else
      if (szKeyWord->bSame("enum"_Stz)) {
        // in SE we permit be outside a namespace
        vParseFileEnum(szKeyWord);
      }
      else
      if (szKeyWord->bSame("friend"_Stz)) {
        vThrowProblem(__LINE__); // syntax?
      }
      else
      if (szKeyWord->bSame("#"_Stz)) {
        // in SE we #include etc to be outside a namespace
        vParseFileHash(szKeyWord);
      }
      else
      if (szKeyWord->bSame("namespace"_Stz)) {
        // recognise a namespace
        vParseFileNamespace(szKeyWord);
      }
      else
      if (szKeyWord->bSame("static"_Stz)) {
         vParseFileStatic(szKeyWord);
     }
      else
      if (szKeyWord->bSame("struct"_Stz)) {
        // in SE we don't expect a struct outside of a namespace
        vThrowProblem(__LINE__);
      }
      else
      if (szKeyWord->bSame("template"_Stz)) {
         // in SE we have templated functions outside of a namespace
         vParseFileTemplate(szKeyWord);
      }
      else
      if (szKeyWord->bSame("union"_Stz)) {
        vThrowProblem(__LINE__); // syntax?
      }
      else
      if (szKeyWord->bSame("using"_Stz)) {
        // using namespace is outside a namespace
        vParseFileUsing(szKeyWord);
      }
      else {
        // in SE we allow global functions
        vParseFileFunction(szKeyWord);
      }

    }
    catch (i8 i_Problem) {
      return false;
    }
    
  } // while more tokens
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* within file, parse TEMPLATE ... CLASS _^_ <name> ...                       */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFileClass(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  RASSERT(! m_p_MetaBlock);
  RASSERT(szSawTerm->bSame("class"_Stz));
  
  Token xTkn;
  Token xName;

  
  vDebugPoint();
  
  // get <name>
  CHECKED(bGetNextToken(xTkn));
  xName = xTkn;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  CHECKED(bGetNextToken(xTkn));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vDescendIntoClass(xName.szTerm());

  vNewTopBlock(m_sClassNm.szStz(),MB_CLASS);
  
  // detect class base clause ?
  if (xTkn.bSame(":"_Stz)) {
    vParseClassBase(xTkn.szTerm());
    m_p_MetaBlock = nullptr;
    return;
  }
  
  // detect class block ?
  if (xTkn.bSame("{"_Stz)) {
    vParseClassBlock(xTkn.szTerm());
    m_p_MetaBlock = nullptr;
    return;
  }

  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* within file, parse ENUM _^_ ... ;                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFileEnum(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("enum"_Stz));
  
  vDebugPoint();
  
  vSkipTill(szSawTerm,";"_Stz);
  
  vRecognised(CP_FILE_ENUM);                                             // ✅
}

/*----------------------------------------------------------------------------*/
/* within file, parse <term> _^_ <term> ... ;                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFileFunction(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  //RASSERT(szSawTerm->bSame("???"_Stz));
  
  eFN eFun;
  Token xFunNm;
  CHECKED(bCollectFunctionTerms(szSawTerm,xFunNm,eFun));

//  // get (
//  Token xTkn;
//  CHECKED(bGetNextToken(xTkn));
//
//  if (xTkn.bSame("noexcept"_Stz)) {
//    CHECKED(bGetNextToken(xTkn));
//    CHECKED(xTkn.bSame("("_Stz));
//    CHECKED(bGetNextToken(xTkn));
//    CHECKED(xTkn.bSame("false"_Stz));
//    CHECKED(bGetNextToken(xTkn));
//    CHECKED(xTkn.bSame(")"_Stz));
//    CHECKED(bGetNextToken(xTkn));
//  }
//
//  if (xTkn.bSame(";"_Stz)) {
//  }
//  else
//  if (xTkn.bSame(":"_Stz)) {
//    vSkipTill(xTkn.szTerm(),"{"_Stz);
//    vSkipBalancedClosingBrace("{"_Stz);
//  }
//  else {
//    CHECKED(xTkn.bSame("{"_Stz));
//    vSkipBalancedClosingBrace("{"_Stz);
//    vDebugPoint();
//  }
  
  vRecognised(CP_FILE_FUNCTION);                                         // ✅
}

/*----------------------------------------------------------------------------*/
/* within file, parse # _^_ DEFINE ... | INCLUDE ... | PRAGMA ...             */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFileHash(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("#"_Stz));
  
  vDebugPoint();
  
  Token xTkn;
  CHECKED(bGetNextToken(xTkn));
  
  if (xTkn.bSame("define"_Stz)) {
    vParseHashDefine(xTkn.szTerm());
    return;
  }

  if (xTkn.bSame("if"_Stz)) {
    vParseHashIf(xTkn.szTerm());
    return;
  }

  if (xTkn.bSame("include"_Stz)) {
    vParseHashInclude(xTkn.szTerm());
    return;
  }

  if (xTkn.bSame("pragma"_Stz)) {
    vParseHashPragma(xTkn.szTerm());
    return;
  }

  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* within file, parse NAMESPACE _^_ <name> { ... }                            */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFileNamespace(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  RASSERT(szSawTerm->bSame("namespace"_Stz));
  
  vDebugPoint();
  
  // get <name> of namespace
  Token xTkn;
  CHECKED(bGetNextToken(xTkn));
  
  vDescendIntoNamepace(xTkn.szTerm());
  
  // get {
  CHECKED(bGetNextToken(xTkn));
  CHECKED(xTkn.bSame("{"_Stz));

  // parse ... }
  while (bGetNextToken(xTkn)) {
  
    // test for end-of-namespace
    if (xTkn.bSame("}"_Stz))  {
      vRecognised(CP_FILE_NAMESPACE);                          // namespace ✅
      vAscendFromNamepace();
      return;
    }
    
    // -- -- -- -- -- --
    
    Stz* szKeyWord = xTkn.szTerm();
    
    // the possible key words in a namespacee
    if (szKeyWord->bSame("class"_Stz)) {
      vParseNamespaceClass(szKeyWord);
    }
    else
    if (szKeyWord->bSame("enum "_Stz)) {
      vThrowProblem(__LINE__);
    }
    else
    if (szKeyWord->bSame("friend"_Stz)) {
       vThrowProblem(__LINE__);
    }
    else
    if (szKeyWord->bSame("#"_Stz)) {
      vThrowProblem(__LINE__);
    }
    else
    if (szKeyWord->bSame("static"_Stz)) {
      vParseNamespaceStatic(szKeyWord);
    }
    else
    if (szKeyWord->bSame("struct"_Stz)) {
      vParseNamespaceStruct(szKeyWord);
    }
    else
    if (szKeyWord->bSame("template"_Stz)) {
      vParseNamespaceTemplate(szKeyWord);
    }
    else
    if (szKeyWord->bSame("typedef"_Stz)) {
      vParseNamespaceTypedef(szKeyWord);
    }
    else
    if (szKeyWord->bSame("union"_Stz)) {
      vParseNamespaceStruct(szKeyWord);
    }
    else {
      vParseNamespaceFunction(szKeyWord);
    }
    
  } // while more tokens
  
  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* within file, parse STATIC ... ;                                            */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFileStatic(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  RASSERT(szSawTerm->bSame("static"_Stz));
  
  vDebugPoint();
  
  Token xTkn;
  
  vSkipTill(szSawTerm,";"_Stz);
  
  vRecognised(CP_FILE_STATIC);                                            // ✅
}

/*----------------------------------------------------------------------------*/
/* within file, parse TEMPLATE < ... > <fileClass>  | <fileFunction>          */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFileTemplate(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  RASSERT(szSawTerm->bSame("template"_Stz));
  
  vDebugPoint();
  
  Token xTkn;
  CHECKED(bGetNextToken(xTkn));
  CHECKED(xTkn.bSame("<"_Stz));
  CHECKED(bCollectTemplateArgs(xTkn.szTerm()));
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // class or function
  CHECKED(bGetNextToken(xTkn));
  
  if (xTkn.bSame("class"_Stz)) {
    vThrowProblem(__LINE__);
    vParseFileClass(xTkn.szTerm());
  }
  else {
    vParseFileFunction(xTkn.szTerm());
  }
}

/*----------------------------------------------------------------------------*/
/* within file, parse USING _^_ NAMESPACE <name> ;                            */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFileUsing(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  RASSERT(szSawTerm->bSame("using"_Stz));
  
  vDebugPoint();
  
  Token xTkn;
  
  // get NAMESPACE
  CHECKED(bGetNextToken(xTkn));
  CHECKED(xTkn.bSame("namespace"_Stz));

  // get <name>
  CHECKED(bGetNextToken(xTkn));

  // get ;
  CHECKED(bGetNextToken(xTkn));
  CHECKED(xTkn.bSame(";"_Stz));
  
  vRecognised(CP_FILE_USING);                                            // ✅
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* within #, parse DEFINE _^_ ...                                             */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashDefine(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("#"_Stz));
  
  vDebugPoint();
  
  Token xTkn;
  CHECKED(bGetNextToken(xTkn));
  
  if (xTkn.bSame("define"_Stz)) {
    CHECKED(bGetNextToken(xTkn));
    vThrowProblem(__LINE__);
    vRecognised(CP_HASH_DEFINE);
    return;
  }

  if (xTkn.bSame("include"_Stz)) {
    CHECKED(bGetNextToken(xTkn));
    vRecognised(CP_HASH_INCLUDE);
    return;
  }

  if (xTkn.bSame("pragma"_Stz)) {
    CHECKED(bGetNextToken(xTkn));
    CHECKED(xTkn.bSame("once"_Stz));
    vRecognised(CP_HASH_PRAGMA);
    return;
  }

  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* within #, parse INCLUDE _^_ <filePath>                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashIf(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("if"_Stz));
      
  vRecognised(CP_HASH_INCLUDE);
}

/*----------------------------------------------------------------------------*/
/* within #, parse INCLUDE _^_ <filePath>                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashInclude(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("include"_Stz));
  
  vDebugPoint();
  
  Token xTkn;
  CHECKED(bGetNextToken(xTkn));
    
  vRecognised(CP_HASH_INCLUDE);
}

/*----------------------------------------------------------------------------*/
/* within #, parse # PRAGMA _^_ ONCE                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vParseHashPragma(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("pragma"_Stz));
  
  vDebugPoint();
  
  Token xTkn;
  CHECKED(bGetNextToken(xTkn));
  CHECKED(xTkn.bSame("once"_Stz));

  vRecognised(CP_HASH_PRAGMA);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* within NAMESPACE, parse CLASS _^_ <name> ...                               */
/*----------------------------------------------------------------------------*/
void Vortex::vParseNamespaceClass(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  RASSERT(! m_p_MetaBlock);
  RASSERT(szSawTerm->bSame("class"_Stz));
  
  vDebugPoint();
  
  // get <name>
  Token xTkn;
  Token xName;
  CHECKED(bGetNextToken(xTkn));
  xName = xTkn;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  CHECKED(bGetNextToken(xTkn));

  // forward declaration of a class?
  if (xTkn.bSame(";"_Stz)) {
    vRecognised(CP_NAMESPACE_FORWARD);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vDescendIntoClass(xName.szTerm());

  vNewTopBlock(m_sClassNm.szStz(),MB_CLASS);
  
  // detect class base clause ?
  if (xTkn.bSame(":"_Stz)) {
    vParseClassBase(xTkn.szTerm());
    m_p_MetaBlock = nullptr;
    return;
  }
  
  // detect class block ?
  if (xTkn.bSame("{"_Stz)) {
    vParseClassBlock(xTkn.szTerm());
    m_p_MetaBlock = nullptr;
    return;
  }

  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* within NAMESPACE, parse <term> _^_ <term> ... ;                            */
/*----------------------------------------------------------------------------*/
void Vortex::vParseNamespaceFunction(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  //RASSERT(szSawTerm->bSame("???"_Stz));
  
  eFN eFun;
  Token xFunNm;
  CHECKED(bCollectFunctionTerms(szSawTerm,xFunNm,eFun));

  
  vRecognised(CP_NAMESPACE_FUNCTION);
}

/*----------------------------------------------------------------------------*/
/* within NAMESPACE, parse STATIC _^_ ...                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vParseNamespaceStatic(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("static"_Stz));
  
  vDebugPoint();
  
  if (! m_bInClass) {
    vSkipTill(szSawTerm,";"_Stz);
    vRecognised(CP_NAMESPACE_STATIC);
    return;
  }
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* parse STRUCT {                                                             */
/*----------------------------------------------------------------------------*/
void Vortex::vParseNamespaceStruct(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("struct"_Stz));

  Token xTkn;
  
  vNewTopBlock("struct"_Stz,MB_STRUCT);

  vSkipTill(szSawTerm,";"_Stz);
  
  vRecognised(CP_NAMESPACE_STRUCT);               // struct within namespace ✅
  
  vPopNestedBlock();
}

/*----------------------------------------------------------------------------*/
/* parse template _^_ ...                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vParseNamespaceTemplate(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("template"_Stz));
  
  vDebugPoint();
  
  Token xTkn;
  CHECKED(bGetNextToken(xTkn));
  CHECKED(xTkn.bSame("<"_Stz));
  
  CHECKED(bCollectTemplateArgs(xTkn.szTerm()));
    
  CHECKED(bGetNextToken(xTkn));
  
  if (xTkn.bSame("class"_Stz)) {
    vParseNamespaceClass(xTkn.szTerm());
  }
  else {
    vParseNamespaceFunction(xTkn.szTerm());
  }
}

/*----------------------------------------------------------------------------*/
/* parse typedef _^_ ...                                                      */
/*----------------------------------------------------------------------------*/
void Vortex::vParseNamespaceTypedef(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("typedef"_Stz));
  
  vDebugPoint();
  
  vSkipTill(szSawTerm,";"_Stz);
  
  vRecognised(CP_NAMESPACE_TYPEDEF);    // forward template within namespace ✅
}

/*----------------------------------------------------------------------------*/
/* within namespace, parse UNION ...                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vParseNamespaceUnion(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("union"_Stz));
  
  Token xTkn;
    
  vNewTopBlock("union"_Stz,MB_UNION);
  
  //vProcUnionMember();
  
  vThrowProblem(__LINE__);
  
  m_p_MetaBlock = nullptr;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse within:                                                              */
/*----------------------------------------------------------------------------*/
void Vortex::vParseClassBase(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_bInClass);
  RASSERT(szSawTerm->bSame(":"_Stz));
  
  Token xTkn;

  vDebugPoint();
  
  while (bGetNextToken(xTkn)) {

    Stz* szTerm = xTkn.szTerm();
    
    // class base is terminated by {
    if (szTerm->bSame("{"_Stz)) {
      vParseClassBlock(szTerm);
      return;
    }

    // skip token
    
  }

  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* parse: namespace <namespaceNm> { ... class _^_ ...                         */
/*----------------------------------------------------------------------------*/
void Vortex::vParseClassBlock(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_bInClass);
  RASSERT(szSawTerm->bSame("{"_Stz));
  
  vDebugPoint();
  
  Token xTkn;

  while (bGetNextToken(xTkn)) {

    Stz* szTerm = xTkn.szTerm();

    // class base is terminated by } ;
    if (szTerm->bSame("}"_Stz)) {
     CHECKED(bGetNextToken(xTkn));
     CHECKED(szTerm->bSame(";"_Stz));
     vRecognised(CP_NAMESPACE_CLASS);
     vPopNestedBlock();
     vAscendFromClass();
     return;
    }

    vParseBlockMember(szTerm);
  }

  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* parse friend _^_ ...                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vParseBlockFriend(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  CHECKED(szSawTerm->bSame("friend"_Stz));
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* within class block, skip a label i.e. private|protected|public :           */
/*----------------------------------------------------------------------------*/
bool Vortex::bParseBlockLabel(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  
  if (szSawTerm->bSame("private"_Stz)) {
    CHECKED(bGetNextToken(xTkn));
    CHECKED(xTkn.bSame(":"_Stz));
    vRecognised(CP_MEMBER_LABEL);
    return true;
  }
  
  if (szSawTerm->bSame("protected"_Stz)) {
    CHECKED(bGetNextToken(xTkn));
    CHECKED(xTkn.bSame(":"_Stz));
    vRecognised(CP_MEMBER_LABEL);
    return true;
  }
  
  if (szSawTerm->bSame("public"_Stz)) {
    CHECKED(bGetNextToken(xTkn));
    CHECKED(xTkn.bSame(":"_Stz));
    vRecognised(CP_MEMBER_LABEL);
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* parse single class member (no label)                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vParseBlockMember(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
 
  RASSERT(m_bInClass);
  //RASSERT(szSawTerm->bSame("???"_Stz));
  
  Stz* szClassNm = m_p_MetaBlock->szBlockNm();
  
  vTrace("%lld.%lld) %s : "_Stz,
         m_pMetaGraph->nItems(),
         m_p_MetaBlock->nItems(),
         szClassNm->azChar());
    
  vResetFunVarTerms();
  vAddTerm(szSawTerm);

  Token xTkn;
  while (bGetNextToken(xTkn)) {
    
    if (xTkn.bSame(";"_Stz)) {
      break;
    }
    
    if (xTkn.bSame("{"_Stz)) {
      bCollectFunVarTerms(xTkn.szTerm(),"}"_Stz);
      CHECKED(bGetNextToken(xTkn))
      break ;
    }

    vAddTerm(xTkn.szTerm());
  }
    
  if (! MetaMember::bIsMember(szClassNm,&m_oFunVarTerms)) {
    vTrace("  not a member variable\n"_Stz);
    vRecognised(CP_MEMBER_FUNCTION);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8   nPointers;
  bool bFunPtr;
  i8   nElements;
  i8   nBits;

  
  Stz* szVarNm;
  Stz* szTypeNm;
  szVarNm = MetaMember::szMemberNm(&m_oFunVarTerms,szTypeNm,
                                   nPointers,bFunPtr,nElements,nBits);
  
  m_p_MetaMember = m_p_MetaBlock->pAddMember(szVarNm);
  
  m_p_MetaMember->vTypeNm(szTypeNm);
  m_p_MetaMember->vPointers(nPointers);
  m_p_MetaMember->vFunPtr(bFunPtr);
  m_p_MetaMember->vElements(nElements);
  m_p_MetaMember->vBits(nBits);

  vRecognised(CP_MEMBER_VARIABLE);

  vDebugPoint();
}


/*----------------------------------------------------------------------------*/
/* can we recognise: struct {                                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vParseMemberStruct(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("static"_Stz));

  Token xTkn;
  
  vPushNestedBlock("struct"_Stz,MB_STRUCT);
  //vProcStructMember();
  
  vThrowProblem(__LINE__);

  m_p_MetaBlock = nullptr;
}

/*----------------------------------------------------------------------------*/
/* parse template _^_ ...                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vParseMemberTemplate(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("template"_Stz));
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* can we recognise: union  {                                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vParseMemberUnion(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("union"_Stz));
  
  Token xTkn;
    
  vPushNestedBlock("union"_Stz,MB_UNION);
  //vProcUnionMember();
  vThrowProblem(__LINE__);
  m_p_MetaBlock = nullptr;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse <term> _^_                                                           */
/*----------------------------------------------------------------------------*/
void Vortex::vParseMemberFunVar(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  //RASSERT(szSawTerm->bSame("???"_Stz));
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* parse static _^_ ...                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vParseMemberStatic(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("static"_Stz));
  
  vDebugPoint();
  
  if (! m_bInClass) {
    vSkipTill(szSawTerm,";"_Stz);
    vRecognised(CP_MEMBER_STATIC);
    return;
  }
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect all terms till given end (optionally adding the end term)          */
/*----------------------------------------------------------------------------*/
bool Vortex::bCollectFunVarTerms(Stz* szSawTerm, Stz* szEnd, bool bAddEnd)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vAddPart("@"_Stz);
  
  if (szSawTerm->bSame(szEnd)) {
    if (bAddEnd)
      m_oFunVarTerms.vAdd(szSawTerm);
    return true;
  }
  
  m_oFunVarTerms.vAdd(szSawTerm);
  
  Token xTkn;
  for (;;) {
    
    if (! bGetNextToken(xTkn))
      return false;
    
    if (xTkn.bSame(szEnd)) {
      
      if (bAddEnd)
        m_oFunVarTerms.vAdd(xTkn.szTerm());
      
      break;
    }
    
    m_oFunVarTerms.vAdd(xTkn.szTerm());
  }

  s48K sTermsString;
  m_oFunVarTerms.vNamesString(sTermsString);
  
  vTrace("  %s Terms: %s\n"_Stz,__func__,sTermsString.azChar());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect all terms for a function (with optional { statements } )           */
/*----------------------------------------------------------------------------*/
bool Vortex::bCollectFunctionArgs(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("("_Stz));

  vAddPart("@"_Stz);

  m_oFunVarTerms.vReset();
  m_oFunVarTerms.vAdd(szSawTerm);

  Token xTkn;
  
  // collect ... ( ... )
  for (;;) {
    CHECKED(bGetNextToken(xTkn));
    m_oFunVarTerms.vAdd(xTkn.szTerm());
    if (xTkn.bSame(")"_Stz))
      break;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s48K sTermsString;
  m_oFunVarTerms.vNamesString(sTermsString);
  
  vTrace("  %s FunVar: %s\n"_Stz,__func__,sTermsString.azChar());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect terms for function intro:                                          */
/*----------------------------------------------------------------------------*/
bool Vortex::bCollectFunctionTerms(Stz* szSawTerm, Token& roFunNm, eFN& reFun)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  
   1)
      void vCheckAssumptions ( void ) ;
   
   2)
      Forth * _pForth  ( void * aAddr ) ;
   
   3)    template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
      OVER * Nub_t <CN,HEAD,OVER,TAIL,DESC> :: aAllotOver (i8 nOverSize)
   
   
   4)    template <eCN CN, class HEAD, class OVER, class TAIL, class DESC, i8 BYTES>
      TrayNL_t < CN,HEAD,OVER,TAIL,DESC,BYTES> * _pNubNL (void* aAddr);

   5)
         template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
      Nub_t < CN,HEAD,OVER,TAIL,DESC> :: Nub_t (void)
      : Object(CN)
      { .. }

   6)
         template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
      Nub_t < CN,HEAD,OVER,TAIL,DESC> :: ~ Nub_t (void) noexcept(false)
      { ... }
   
   7)    template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
      i8 Nub_t  <CN,HEAD,OVER,TAIL,DESC> :: nBytes (i8 nUnits) { .. }


  maybe
    ?)    template <eCN CN, class HEAD, class OVER, class TAIL, class DESC>
      Some_t <CN>* Nub_t <CN,HEAD,OVER,TAIL,DESC> :: aFun (i8 nOverSize)

   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  
  vAddPart("@"_Stz);

  Token xTkn;

  m_oFunctionTerms.vReset();
  
  s32 sPrior = szSawTerm;
  
  xTkn.vCopy(sPrior);
  xTkn.vToken(TN_NAME);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // skip over optional inline

  if (xTkn.bSame("inline"_Stz)) {
    m_oFunctionTerms.vAdd(szSawTerm);
    CHECKED(bGetNextToken(xTkn));
  }
  m_oFunctionTerms.vAdd(xTkn.szTerm());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  while (! xTkn.bSame("("_Stz)) {
    m_oFunctionTerms.vAdd(xTkn.szTerm());
    sPrior = xTkn.szTerm();
    CHECKED(bGetNextToken(xTkn));
  }
  m_oFunctionTerms.vAdd(xTkn.szTerm());

  roFunNm.vCopy(sPrior);
  roFunNm.vToken(TN_NAME);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  while (! xTkn.bSame(")"_Stz)) {
    m_oFunctionTerms.vAdd(xTkn.szTerm());
    CHECKED(bGetNextToken(xTkn));
  }
  m_oFunctionTerms.vAdd(xTkn.szTerm());

  CHECKED(bGetNextToken(xTkn));

  if (xTkn.bSame(":"_Stz)) {
    while (! xTkn.bSame("{"_Stz)) {
      CHECKED(bGetNextToken(xTkn));
    }
  }
  
  if (xTkn.bSame("noexcept"_Stz)) {
    CHECKED(bGetNextToken(xTkn));
    CHECKED(xTkn.bSame("("_Stz));
    CHECKED(bGetNextToken(xTkn));
    CHECKED(xTkn.bSame("false"_Stz));
    CHECKED(bGetNextToken(xTkn));
    CHECKED(xTkn.bSame(")"_Stz));
    CHECKED(bGetNextToken(xTkn));
  }

  if (xTkn.bSame("{"_Stz)) {
    vSkipBalancedClosingBrace("{"_Stz);
  }
  else {
    CHECKED(xTkn.bSame(";"_Stz));
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s48K sTermsString;
  m_oFunctionTerms.vNamesString(sTermsString);
  
  vTrace("  %s Fun: %s ReturnType: %s\n"_Stz,
         __func__,
         roFunNm.szTerm(),sTermsString.azChar());
  
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect all teplate arg terms : TEMPLATE _^_ < ... >                       */
/*----------------------------------------------------------------------------*/
bool Vortex::bCollectTemplateArgs(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("<"_Stz));

  vAddPart("@"_Stz);
  
  m_oTplArgTerms.vReset();
  m_oTplArgTerms.vAdd(szSawTerm);

  Token xTkn;
  
  // collect ... ( ... )
  for (;;) {
    CHECKED(bGetNextToken(xTkn));
    m_oTplArgTerms.vAdd(xTkn.szTerm());
    if (xTkn.bSame(">"_Stz))
      break;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s48K sTermsString;
  m_oTplArgTerms.vNamesString(sTermsString);
  
  vTrace("  %s TplArgs: %s\n"_Stz,__func__,sTermsString.azChar());
  
  return true;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next token                                                             */
/*----------------------------------------------------------------------------*/
void Vortex::vAddPart(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (m_sPart.nLength())
    m_sPart.vAppend(' ');
  
  m_sPart.vConcat(szTerm);
  
  if (m_bTraceTokens)
    vTrace("Part: %s\n"_Stz,m_sPart.azChar());

}

/*----------------------------------------------------------------------------*/
/* collect term                                                               */
/*----------------------------------------------------------------------------*/
void Vortex::vAddTerm(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_oFunVarTerms.vAdd(szTerm);
}

/*----------------------------------------------------------------------------*/
/* ascend from parsing a class                                                */
/*----------------------------------------------------------------------------*/
void Vortex::vAscendFromClass(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_bInNamespace);
  RASSERT(m_bInClass);

  m_bInClass = false;
  m_sClassNm = ""_Stz;

  m_p_MetaBlock  = nullptr;
}

/*----------------------------------------------------------------------------*/
/* ascend from parsing a namespace                                            */
/*----------------------------------------------------------------------------*/
void Vortex::vAscendFromNamepace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_bInNamespace);
  RASSERT(! m_bInClass);

  m_bInNamespace = false;
  m_sNamespaceNm = ""_Stz;
}

/*----------------------------------------------------------------------------*/
/* descend into parsing a class                                               */
/*----------------------------------------------------------------------------*/
void Vortex::vDescendIntoClass(Stz* szClassNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(m_bInNamespace);
  RASSERT(! m_bInClass);
  
  RASSERT(! m_p_MetaBlock);

  m_sClassNm = szClassNm;
  m_bInClass = true;
}

/*----------------------------------------------------------------------------*/
/* descend into parsing a namespace                                           */
/*----------------------------------------------------------------------------*/
void Vortex::vDescendIntoNamepace(Stz* szNamespaceNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(! m_bInNamespace);
  RASSERT(! m_bInClass);

  m_sNamespaceNm = szNamespaceNm;
  m_bInNamespace = true;
}

/*----------------------------------------------------------------------------*/
/* get next token                                                             */
/*----------------------------------------------------------------------------*/
bool Vortex::bGetNextToken(Token& roToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  bool b = m_pInput->bGetToken(&roToken);
  
  if (b)
    vAddPart(roToken.szTerm());
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* create a new top block                                                      */
/*----------------------------------------------------------------------------*/
void Vortex::vNewTopBlock(Stz* szBlockNme, eMB eBlock)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_pMetaGraph);
  RASSERT(! m_p_MetaBlock);

  m_p_MetaBlock = new MetaBlock(szBlockNme,eBlock,m_pMetaGraph,nullptr);
}

/*----------------------------------------------------------------------------*/
/* pop at block end                                                           */
/*----------------------------------------------------------------------------*/
void Vortex::vPopNestedBlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  CHECKED(m_p_MetaBlock);
  
  m_p_MetaBlock = m_p_MetaBlock->p_ParentBlock();
}

/*----------------------------------------------------------------------------*/
/* push new block                                                             */
/*----------------------------------------------------------------------------*/
void Vortex::vPushNestedBlock(Stz* szBlockNm, eMB eBlock)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_pMetaGraph);
  
  m_p_MetaBlock = new MetaBlock(szBlockNm,eBlock,m_pMetaGraph,m_p_MetaBlock);
}

/*----------------------------------------------------------------------------*/
/* recognised c++ part                                                        */
/*----------------------------------------------------------------------------*/
void Vortex::vRecognised(eCP ePart)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vTrace("Recognised %s\n"_Stz,aCP[ePart]);
  vTrace("\n"_Stz);

  vResetPart();
  vResetFunVarTerms();
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* reset terms                                                                */
/*----------------------------------------------------------------------------*/
void Vortex::vResetFunVarTerms(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_oFunVarTerms.vReset();
}

/*----------------------------------------------------------------------------*/
/* start new c++ part                                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vResetPart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_sPart = ""_Stz;
}

/*----------------------------------------------------------------------------*/
/* skip till balanced closing brace                                           */
/*----------------------------------------------------------------------------*/
void Vortex::vSkipBalancedClosingBrace(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("{"_Stz));
       
  Token xTkn;
  for (;;) {
    
    CHECKED(bGetNextToken(xTkn));
    
    // test for closing brace
    if (xTkn.bSame("}"_Stz))
      return;
    
    // test for nested opening brace
    if (xTkn.bSame("{"_Stz))
      vSkipBalancedClosingBrace(xTkn.szTerm());
      
    // ignore any other token
  }
  
}

/*----------------------------------------------------------------------------*/
/* skip till ;                                                                */
/*----------------------------------------------------------------------------*/
//void Vortex::vSkipTillSemicolon(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
//
//  vTrace("Skipping: "_Stz);
//
//  Token xTkn;
//  while (bGetNextToken(xTkn)) {
//    vTrace("%s "_Stz,xTkn.azTerm());
//    if (xTkn.bSame(";"_Stz)) {
//      vTrace("\n"_Stz,xTkn.azTerm());
//      return;
//    }
//  }
//}

/*----------------------------------------------------------------------------*/
/* skip till given term                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vSkipTill(Stz* szTerm, Stz* szTill)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  
  vTrace("Skipping: %s "_Stz,szTerm->azChar());
  if (szTerm->bSame(szTill)) {
    vTrace("\n"_Stz,szTerm->azChar());
    return;
  }
        
  while (bGetNextToken(xTkn)) {
    vTrace("%s "_Stz,xTkn.azTerm());
    if (xTkn.bSame(szTill)) {
      vTrace("\n"_Stz,xTkn.azTerm());
      return;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* skip till ;                                                                */
/*----------------------------------------------------------------------------*/
void Vortex::vSkipUnrecognised(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  
  vTrace("Unrecognised: %s "_Stz,szTerm->azChar());
  if (szTerm->bSame(";"_Stz)) {
    vTrace("\n"_Stz,szTerm->azChar());
    return;
  }
        
  while (bGetNextToken(xTkn)) {
    vTrace("%s "_Stz,xTkn.azTerm());
    if (xTkn.bSame(";"_Stz)) {
      vTrace("\n"_Stz,xTkn.azTerm());
      return;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* are terms a function ?                                                     */
/*----------------------------------------------------------------------------*/
bool Vortex::bTermsAreFunction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  Stz* szTerm;
 
  i8 nTerms = m_oFunVarTerms.nItems();
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // term 0
  
  i8 i_Term = 0;
  szTerm = m_oFunVarTerms.szName(i_Term++);
  
  // -- -- -- -- -- --
  // constructor function
  if (m_bInClass && szTerm->bSame(m_sClassNm.szStz())) {
    return true;
  }
  
  // -- -- -- -- -- --
  // destructor function
  if (m_bInClass && szTerm->bSame("~"_Stz))
    return true;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  
  // nb. szTerm is return type at this point

  
  // -- -- -- -- -- --
  // skip zero or more '*'
  szTerm = m_oFunVarTerms.szName(i_Term++);
  while (szTerm->bSame("*"_Stz))
    szTerm = m_oFunVarTerms.szName(i_Term++);
  
  // -- -- -- -- -- --
  // test for member which is a member which is a function pointer
  if (m_bInClass && szTerm->bSame("("_Stz))
    return false;

  // -- -- -- -- -- --
  // attempt to detect a function
  szTerm = m_oFunVarTerms.szName(i_Term++);
  while (i_Term < nTerms) {
    if (szTerm->bSame("("_Stz))
      return true;
    szTerm = m_oFunVarTerms.szName(i_Term++);
  }
  
  // -- -- -- -- -- --
  // must be a variable
  return false;
}

/*----------------------------------------------------------------------------*/
/* are terms a variable ?                                                     */
/*----------------------------------------------------------------------------*/
bool Vortex::bTermsAreVariable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
   Stz* szTerm;
  
   i8 nTerms = m_oFunVarTerms.nItems();
     
   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   // term 0
   
   i8 i_Term = 0;
   szTerm = m_oFunVarTerms.szName(i_Term++);
  
  // -- -- -- -- -- --
  // exclude constructor function
  if (m_bInClass && szTerm->bSame(m_sClassNm.szStz()) &&
      m_oFunVarTerms.szName(i_Term)->bSame("("_Stz)) {
    return false;
  }
  
  // -- -- -- -- -- --
  // exclude destructor function
  if (m_bInClass && szTerm->bSame("~"_Stz))
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  
  // nb. szTerm is return type at this point

  
  // -- -- -- -- -- --
  // skip zero or more '*'
  szTerm = m_oFunVarTerms.szName(i_Term++);
  while (szTerm->bSame("*"_Stz))
    szTerm = m_oFunVarTerms.szName(i_Term++);
  
  // -- -- -- -- -- --
  // test for variable which is a function pointer
  if (szTerm->bSame("("_Stz))
    return true;

  // -- -- -- -- -- --
  // attempt to detect a function
  szTerm = m_oFunVarTerms.szName(i_Term++);
  while (i_Term < nTerms) {
    if (szTerm->bSame("("_Stz))
      return false;
    szTerm = m_oFunVarTerms.szName(i_Term++);
  }
  
  // -- -- -- -- -- --
  // must be a variable
  return true;
}

/*----------------------------------------------------------------------------*/
/* throw problem                                                              */
/*----------------------------------------------------------------------------*/
void Vortex::vThrowProblem(i8 i_Problem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  throw i_Problem;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/




/*============================================================================*/
/*                                  REMOVE                                    */
/*============================================================================*/

#if 0

/*----------------------------------------------------------------------------*/
/* process all classes in the current source file (.h or .hpp)                */
/*----------------------------------------------------------------------------*/
void Vortex::vProcAll(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xFirstTkn;
  while (bGetNextToken(xFirstTkn)) {                                          // 🌀

    if (bRecogniseClass(xFirstTkn.szTerm()))
      continue;

    if (bRecogniseConst(xFirstTkn.szTerm()))
      continue;
    
    if (bRecogniseFriend(xFirstTkn.szTerm()))
      continue;

    if (bRecogniseHash(xFirstTkn.szTerm()))
      continue;
    
    if (bRecogniseNamespace(xFirstTkn.szTerm()))
      continue;
    
    if (bRecogniseUsing(xFirstTkn.szTerm()))
      continue;
    
    vSkipTillSemicolon(xFirstTkn.szTerm());
    
  } // while

}

/*----------------------------------------------------------------------------*/
/* parse base (inheritence) clause of a class                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vProcClassBase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  // just saw: class <classNm :
  
  Token xTkn;
  while (bGetNextToken(xTkn)) {
    
    if (xTkn.bSame("{"_Stz)) {
      vProcClassBlock();
      break;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* parse class block, i.e. the { ... } sequence                               */
/*----------------------------------------------------------------------------*/
void Vortex::vProcClassBlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  // just saw {
  
  vProcClassMembers();
}

/*----------------------------------------------------------------------------*/
/* parse class function                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vProcClassFunction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  // just saw {
  
  Token xTkn;
  while (bGetNextToken(xTkn)) {
    
    if (xTkn.bSame("}"_Stz))
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* parse all members of a class                                               */
/*----------------------------------------------------------------------------*/
void Vortex::vProcClassMembers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(0);
  

}

/*----------------------------------------------------------------------------*/
/* parse: static const i8 <constNm> ^_ = ... ;                                */
/*----------------------------------------------------------------------------*/
void Vortex::vProcConst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  // seen: static const i8
  
  m_oFunVarTerms.vReset();
  
  Token xConstNm;
  if (! bGetNextToken(xConstNm))
     return;

  Token xTkn;
  do {
    if (! bGetNextToken(xTkn))
      return;
    
  } while (! xTkn.bSame(";"_Stz));

  vTrace("Const %s\n"_Stz,xConstNm.azTerm());
  
  if (bCollectFunVarTerms(xConstNm.szTerm(),";"_Stz)) {
    ConstDef* pConstDef = m_p_ConsSet->pAddConstDef(xConstNm.szTerm());
    pConstDef->vTerms(m_oFunVarTerms);
  }
}

/*----------------------------------------------------------------------------*/
/* parse:                                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vProcEnum(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);

}

/*----------------------------------------------------------------------------*/
/* parse:                                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vProcHashDefine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;

  RASSERT(bGetNextToken(xTkn));
}

/*----------------------------------------------------------------------------*/
/* parse:                                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vProcHashInclude(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;

  RASSERT(bGetNextToken(xTkn));
}

/*----------------------------------------------------------------------------*/
/* parse:                                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vProcHashPragma(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;

  RASSERT(bGetNextToken(xTkn));
}

/*----------------------------------------------------------------------------*/
/* parse single (optionally labelled) member                                  */
/*----------------------------------------------------------------------------*/
void Vortex::vProcLabelledMember(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  RASSERT(m_bInClass);
  
  Token xTkn;
  CHECKED(bGetNextToken(xTkn));

  if (bRecogniseStruct(xTkn.szTerm()))
    return;
    
  if (bRecogniseUnion(xTkn.szTerm()))
    return;

  if (! m_p_MetaBlock)
    m_p_MetaBlock = m_pMetaGraph->pAddBlock("linear"_Stz,m_p_MetaBlock);
  
  vProcMemberProper(xTkn.szTerm());
  
}

/*----------------------------------------------------------------------------*/
/* parse member                                                               */
/*----------------------------------------------------------------------------*/
void Vortex::vProcMemberProper(Stz* szFirstTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Stz* szClassNm = m_p_MetaBlock->szBlockNm();
  
  vTrace("%lld.%lld) %s : "_Stz,
         m_pMetaGraph->nItems(),
         m_p_MetaBlock->nItems(),
         szClassNm->azChar());
  
  //vTrace("%s "_Stz,xTkn.azChar());
  
  m_oFunVarTerms.vReset();

  vAddTerm(szFirstTerm);
  
  Token xTkn;
  while (bGetNextToken(xTkn)) {
    
    if (xTkn.bSame(";"_Stz)) {
      break;
    }
    
    if (xTkn.bSame("{"_Stz)) {
      vProcClassFunction();
      break ;
    }

    vAddTerm(xTkn.szTerm());
  }
    
  if (! MetaMember::bIsMember(szClassNm,&m_oFunVarTerms)) {
    vTrace("  not a member variable\n"_Stz);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8   nPointers;
  bool bFunPtr;
  i8   nElements;
  i8   nBits;

  
  Stz* szVarNm;
  Stz* szTypeNm;
  szVarNm = MetaMember::szVarNm(&m_oFunVarTerms,szTypeNm,
                                nPointers,bFunPtr,nElements,nBits);
  
  m_p_MetaMember = m_p_MetaBlock->pAddMember(szVarNm);
  
  m_p_MetaMember->vTypeNm(szTypeNm);
  m_p_MetaMember->vPointers(nPointers);
  m_p_MetaMember->vFunPtr(bFunPtr);
  m_p_MetaMember->vElements(nElements);
  m_p_MetaMember->vBits(nBits);

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* process all classes in the current source file (.h or .hpp)                */
/*----------------------------------------------------------------------------*/
bool Vortex::bProcNamespace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;
  
  RASSERT(bGetNextToken(xTkn));
  RASSERT(xTkn.bSame("{"_Stz));
  
  while (bGetNextToken(xTkn)) {

    if (xTkn.bSame("}"_Stz))
      return true;
    
    if (bRecogniseClass(xTkn.szTerm()))
      continue;

    if (bRecogniseConst(xTkn.szTerm()))
      continue;
    
    if (bRecogniseFriend(xTkn.szTerm()))
      continue;

    if (bRecogniseHash(xTkn.szTerm()))
      continue;
    
    if (bRecogniseTemplate(xTkn.szTerm()))
      continue;
    
    // -- last --
    if (bRecogniseFunction(xTkn.szTerm()))
      continue;
    
    vSkipUnrecognised(xTkn.szTerm());
    
  } // while

  return false;
}

/*----------------------------------------------------------------------------*/
/* parse struct array                                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vProcStructArray(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;
  RASSERT(bGetNextToken(xTkn));
  vAddTerm(xTkn.szTerm());
  
  RASSERT(bGetNextToken(xTkn));
  RASSERT(xTkn.bSame("]"_Stz));
  vAddTerm(xTkn.szTerm());

  RASSERT(bGetNextToken(xTkn));
  RASSERT(xTkn.bSame(";"_Stz));
  vAddTerm(xTkn.szTerm());
};

/*----------------------------------------------------------------------------*/
/* parse struct member                                                        */
/*----------------------------------------------------------------------------*/
void Vortex::vProcStructMember(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  // just saw: struct
  
  m_bTraceTokens = true;
  
  // may see name
  Token xTkn;
  RASSERT(bGetNextToken(xTkn));
  if (! xTkn.bSame("{"_Stz)) {
    RASSERT(bGetNextToken(xTkn));
    if (! xTkn.bSame("{"_Stz)) {
      RASSERT(bGetNextToken(xTkn));
      RASSERT(xTkn.bSame(";"_Stz));
      
      m_bTraceTokens = false;
      return;
    }
  }

  while (bGetNextToken(xTkn)) {

    if (xTkn.bSame("}"_Stz)) {
      RASSERT(bGetNextToken(xTkn));
      if (! xTkn.bSame(";"_Stz)) {
        RASSERT(bGetNextToken(xTkn));
        if (! xTkn.bSame(";"_Stz)) {
          vAddTerm(xTkn.szTerm());
          vProcStructArray();
        }
      }
      break;
    }
    
    vProcMemberProper(xTkn.szTerm());
  }
  
  m_bTraceTokens = false;
}

/*----------------------------------------------------------------------------*/
/* parse union member                                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vProcUnionMember(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  // just saw: union
  
  //m_bTraceTokens = true;
  
  Token xTkn;
  RASSERT(bGetNextToken(xTkn));
  RASSERT(xTkn.bSame("{"_Stz));

  while (bGetNextToken(xTkn)) {

    if (xTkn.bSame("}"_Stz)) {
      RASSERT(bGetNextToken(xTkn));
      RASSERT(xTkn.bSame(";"_Stz));
      break;
    }
    
    vProcMemberProper(xTkn.szTerm());
  }
  
  //m_bTraceTokens = false;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: class ... ; ?                                            */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseClass(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(0);
  
  Token xTkn;
  Token xClassNm;


  if (! xTkn.bSame("class"_Stz))
    return false;
    
  if (! bGetNextToken(xClassNm))
    return false;

  if (! bGetNextToken(xTkn))
    return false;

  // -- -- -- -- -- --
  
  m_pOutputFile->vPrint("%s\n"_Stz,xClassNm.azTerm());
  
  m_p_MetaBlock = m_pMetaGraph->pAddBlock(xClassNm.szTerm(),m_p_MetaBlock);
  if (xTkn.bSame(":"_Stz)) {
    m_p_MetaBlock = nullptr;
    m_p_MetaMember = nullptr;
    vProcClassBase();
    return true;
  }
  
  if (xTkn.bSame("{"_Stz)) {
    m_p_MetaBlock = nullptr;
    m_p_MetaMember = nullptr;
    vProcClassBlock();
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: static const i8 ... ; ?                                  */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseConst(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(0);
  
  Token xTkn;
  
  if (! szTerm->bSame("static"_Stz))
    return false;
      
  if (! bGetNextToken(xTkn))
    return false;
  if (! xTkn.bSame("const"_Stz))
    return false;

  if (! bGetNextToken(xTkn))
    return false;
  if (! xTkn.bSame("i8"_Stz))
    return false;

  vProcConst();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: enum ... ;                                               */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseEnum(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: friend ... ;                                             */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseFriend(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;
  
  if (! szTerm->bSame("friend"_Stz))
    return false;
        
  while (bGetNextToken(xTkn)) {
    if (xTkn.bSame(";"_Stz))
      break;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: function                                                 */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseFunction(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
    
  RASSERT(0);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: # ...                                                    */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseHash(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;
  
  if (! szTerm->bSame("#"_Stz))
    return false;
  
  RASSERT(bGetNextToken(xTkn));
  
  if (xTkn.bSame("define"_Stz)) {
    RASSERT(bGetNextToken(xTkn));
    return true;
  }

  if (xTkn.bSame("include"_Stz)) {
    RASSERT(bGetNextToken(xTkn));
    return true;
  }

  if (xTkn.bSame("pragma"_Stz)) {
    RASSERT(bGetNextToken(xTkn));
    RASSERT(xTkn.bSame("once"_Stz));
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: private|protected|public :                               */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseLabel(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;
  
  if (szTerm->bSame("private"_Stz)) {
    RASSERT(bGetNextToken(xTkn));
    RASSERT(xTkn.bSame(":"_Stz));
    return true;
  }
  
  if (szTerm->bSame("protected"_Stz)) {
    RASSERT(bGetNextToken(xTkn));
    RASSERT(xTkn.bSame(":"_Stz));
    return true;
  }
  
  if (szTerm->bSame("public"_Stz)) {
    RASSERT(bGetNextToken(xTkn));
    RASSERT(xTkn.bSame(":"_Stz));
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: namespace ...                                            */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseNamespace(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  if (! szTerm->bSame("namespace"_Stz))
    return false;

  Token xTkn;
  
  RASSERT(bGetNextToken(xTkn));
  
  m_sNamespaceNm = xTkn.szTerm();
  
  bool b = bProcNamespace();

  return b;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: struct {                                                 */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseStruct(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;
  
  if (! szTerm->bSame("struct"_Stz))
    return false;
  
  m_p_MetaBlock = m_pMetaGraph->pAddBlock("struct"_Stz,m_p_MetaBlock);
  vProcStructMember();
  m_p_MetaBlock = nullptr;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: template ...                                             */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseTemplate(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  if (! szTerm->bSame("template"_Stz))
    return false;

  
  
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: union  {                                                 */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseUnion(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;
  
  if (! szTerm->bSame("union"_Stz))
    return false;
  
  m_p_MetaBlock = m_pMetaGraph->pAddBlock("union"_Stz,m_p_MetaBlock);
  vProcUnionMember();
  m_p_MetaBlock = nullptr;

  return true;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: using namespace ...                                      */
/*----------------------------------------------------------------------------*/
bool Vortex::bRecogniseUsing(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(0);
  
  Token xTkn;
  
  if (! szTerm->bSame("using"_Stz))
    return false;
  
  RASSERT(bGetNextToken(xTkn));
  RASSERT(xTkn.bSame("namespace"_Stz));

  RASSERT(bGetNextToken(xTkn));
  
  RASSERT(bGetNextToken(xTkn));
  RASSERT(xTkn.bSame(";"_Stz));

  return true;
}


#endif
