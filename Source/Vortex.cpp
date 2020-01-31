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
  
  // input
  
  m_sSourcePath  = ""_Stz;
  m_pInput       = nullptr;
  m_bTrace       = false;

  // output
  
  m_pOutputFile  = nullptr;
  m_pMetaGraph   = new MetaGraph;
  
  // working
  
  m_sFragment    = ""_Stz;
  
  m_bInClass     = false;
  m_sClassNm     = C_NO_NAME;
  m_bInNamespace = false;
  m_sNamespaceNm = C_NO_NAME;
  
  m_p_MetaBlock  = nullptr;
  m_p_MetaMember = nullptr;
  m_p_ConstSet   = m_pMetaGraph->pConstSet();

  //m_oOtherTerms
  
  _checkInitialised_(CN_VORTEX);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Vortex::~Vortex(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  delete m_pInput;
  delete m_pOutputFile;
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
    vTrace("** Last Fragment: %s\n"_Stz,m_sFragment.azChar());
    vTrace("\n\n"_Stz);
    vDebugPoint();
  }
  
  delete m_pInput;
  m_pInput = nullptr;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                                 machinary                                  */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* get next token                                                             */
/*----------------------------------------------------------------------------*/
bool Vortex::bDiscardNextToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  
  bool b = m_pInput->bGetToken(&xTkn);
  
  if (b && m_bTrace)
    vTrace("Discarding: %s\n"_Stz,xTkn.azTerm());

  return b;
}

/*----------------------------------------------------------------------------*/
/* get next token as per normal but also and put it into m_oOtherTerms        */
/*----------------------------------------------------------------------------*/
bool Vortex::bGetNextTerm(Token& roTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  bool b = bGetNextToken(roTerm);
  
  if (b)
    m_oOtherTerms.vAdd(roTerm.szTerm());
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get next token                                                             */
/*----------------------------------------------------------------------------*/
bool Vortex::bGetNextToken(Token& roToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  bool b = m_pInput->bGetToken(&roToken);
  
  if (b)
    vFragmentAdd(roToken.szTerm());
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* add sub-string to fragment string                                          */
/*----------------------------------------------------------------------------*/
void Vortex::vFragmentAdd(Stz* szString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  if (m_sFragment.nLength())
    m_sFragment.vAppend(' ');
  
  m_sFragment.vConcat(szString);
  
  if (m_bTrace)
    vTrace("Fragment: %s\n"_Stz,m_sFragment.azChar());
}

/*----------------------------------------------------------------------------*/
/* reset fragment string                                                      */
/*----------------------------------------------------------------------------*/
void Vortex::vFragmentReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_sFragment = ""_Stz;
}

/*----------------------------------------------------------------------------*/
/* recognised c++ part                                                        */
/*----------------------------------------------------------------------------*/
void Vortex::vPartRecognised(eCP ePart)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vTrace("Recognised %s\n"_Stz,aCP[ePart]);
  vTrace("\n"_Stz);

  vFragmentReset();
  vTermsReset();
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* throw problem                                                              */
/*----------------------------------------------------------------------------*/
void Vortex::vThrowProblem(i8 i_Problem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  throw i_Problem;
}




/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                             block management                               */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* create a new top block                                                     */
/*----------------------------------------------------------------------------*/
void Vortex::vBlockNewTop(Stz* szBlockNme, eMB eBlock)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_pMetaGraph);
  RASSERT(! m_p_MetaBlock);

  m_p_MetaBlock = new MetaBlock(szBlockNme,eBlock,m_pMetaGraph,nullptr);
}

/*----------------------------------------------------------------------------*/
/* pop at block end                                                           */
/*----------------------------------------------------------------------------*/
void Vortex::vBlockPopNested(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  CHECKED(m_p_MetaBlock);
  
  m_p_MetaBlock = m_p_MetaBlock->p_ParentBlock();
}

/*----------------------------------------------------------------------------*/
/* push new block                                                             */
/*----------------------------------------------------------------------------*/
void Vortex::vBlockPushNested(Stz* szBlockNm, eMB eBlock)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(m_pMetaGraph);
  
  m_p_MetaBlock = new MetaBlock(szBlockNm,eBlock,m_pMetaGraph,m_p_MetaBlock);
}

/*----------------------------------------------------------------------------*/
/* ascend from parsing a class                                                */
/*----------------------------------------------------------------------------*/
void Vortex::vClassAscend(void)
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
void Vortex::vNamepaceAscend(void)
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
void Vortex::vClassDescend(Stz* szClassNm)
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
void Vortex::vNamepaceDescend(Stz* szNamespaceNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(! m_bInNamespace);
  RASSERT(! m_bInClass);

  m_sNamespaceNm = szNamespaceNm;
  m_bInNamespace = true;
}

/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                            top-down parsing                                */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* process all parts in the current source file (.h or .hpp)                  */
/*----------------------------------------------------------------------------*/
bool Vortex::bParseFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  RASSERT(! m_bInNamespace);
  RASSERT(! m_bInClass);

  m_bTrace = true;
  
  // -- parse till EOF --
  Token xTkn;
  while (bGetNextToken(xTkn)) {
  
    try {
      Stz* szKeyWord = xTkn.szTerm();
        
      if (szKeyWord->bSame("class"_Stz)) {
        vThrowProblem(__LINE__);   // don't expect class outside of a namespace
      }
      else
      if (szKeyWord->bSame("const"_Stz)) {
         vParseFileConst(szKeyWord);
      }
      else
      if (szKeyWord->bSame("enum"_Stz)) {
         vParseFileEnum(szKeyWord);
      }
      else
      if (szKeyWord->bSame("#"_Stz)) {
        vParseFileHash(szKeyWord);
      }
      else
      if (szKeyWord->bSame("namespace"_Stz)) {
        vParseFileNamespace(szKeyWord);
      }
      else
      if (szKeyWord->bSame("struct"_Stz)) {
        vThrowProblem(__LINE__);  // don't expect struct outside of a namespace
      }
      else
      if (szKeyWord->bSame("template"_Stz)) {
         vParseFileTemplate(szKeyWord);
      }
      else
      if (szKeyWord->bSame("typedef"_Stz)) {
        vThrowProblem(__LINE__); // don't expect typedef outside of a namespace
      }
      else
      if (szKeyWord->bSame("union"_Stz)) {
        vThrowProblem(__LINE__);   // don't expect union outside of a namespace
      }
      else
      if (szKeyWord->bSame("using"_Stz)) {
        vParseFileUsing(szKeyWord);
      }
      else {
        vParseFileOther(szKeyWord);
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
  
  vClassDescend(xName.szTerm());

  vBlockNewTop(m_sClassNm.szStz(),MB_CLASS);
  
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
/* within file, parse CONST _^_ ... ;                                         */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFileConst(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  RASSERT(szSawTerm->bSame("const"_Stz));
 
  vThrowProblem(__LINE__);

  vPartRecognised(CP_FILE_CONST);                                         // ✅
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
  
  vDiscardTill(szSawTerm,";"_Stz);
  
  vPartRecognised(CP_FILE_ENUM);                                          // ✅
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
  
  vNamepaceDescend(xTkn.szTerm());
  
  // get {
  CHECKED(bGetNextToken(xTkn));
  CHECKED(xTkn.bSame("{"_Stz));

  // parse ... }
  while (bGetNextToken(xTkn)) {
  
    // test for end-of-namespace
    if (xTkn.bSame("}"_Stz))  {
      vPartRecognised(CP_FILE_NAMESPACE);                          // namespace ✅
      vNamepaceAscend();
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
/* within file, parse <other> _^_ <term> ... ;                                */
/*----------------------------------------------------------------------------*/
void Vortex::vParseFileOther(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  //RASSERT(szSawTerm->bSame("???"_Stz));
  
  vCollectOtherTerms(szSawTerm);
  
  vPartRecognised(CP_FILE_OTHER);                                         // ✅
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
  
  vDiscardTill(szSawTerm,";"_Stz);
  
  vPartRecognised(CP_FILE_STATIC);                                            // ✅
}

/*----------------------------------------------------------------------------*/
/* within file, parse TEMPLATE < ... > CLASS ...  | <other> ...               */
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
    vParseFileOther(xTkn.szTerm());
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
  
  vPartRecognised(CP_FILE_USING);                                            // ✅
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

  RASSERT(szSawTerm->bSame("define"_Stz));
  
  vDebugPoint();
  
  Token xTkn;
  while (bGetNextToken(xTkn)) {
    
    if (xTkn.bSame("EndDefine"_Stz))
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
  
  Token xTkn;
  
  CHECKED(bGetNextToken(xTkn));

  while (! xTkn.bSame("endif"_Stz)) {
    CHECKED(bGetNextToken(xTkn));
  }
      
  vPartRecognised(CP_HASH_IF);
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
    
  vPartRecognised(CP_HASH_INCLUDE);
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

  vPartRecognised(CP_HASH_PRAGMA);
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
    vPartRecognised(CP_NAMESPACE_FORWARD);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vClassDescend(xName.szTerm());

  vBlockNewTop(m_sClassNm.szStz(),MB_CLASS);
  
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

  
  vPartRecognised(CP_NAMESPACE_FUNCTION);
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
    vDiscardTill(szSawTerm,";"_Stz);
    vPartRecognised(CP_NAMESPACE_STATIC);
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
  
  vBlockNewTop("struct"_Stz,MB_STRUCT);

  vDiscardTill(szSawTerm,";"_Stz);
  
  vPartRecognised(CP_NAMESPACE_STRUCT);           // struct within namespace ✅
  
  vBlockPopNested();
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
  
  vDiscardTill(szSawTerm,";"_Stz);
  
  vPartRecognised(CP_NAMESPACE_TYPEDEF); // forward template within namespace ✅
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
    
  vBlockNewTop("union"_Stz,MB_UNION);
  
  //vProcUnionMember();
  
  vThrowProblem(__LINE__);
  
  m_p_MetaBlock = nullptr;
}


/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                      class, block, member parsing                          */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

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
     vPartRecognised(CP_NAMESPACE_CLASS);
     vBlockPopNested();
     vClassAscend();
     return;
    }
    
    if (bParseBlockLabel(xTkn.szTerm()))
      continue;

    vParseBlockOther(szTerm);
  }

  vThrowProblem(__LINE__);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
    vPartRecognised(CP_MEMBER_LABEL);
    return true;
  }
  
  if (szSawTerm->bSame("protected"_Stz)) {
    CHECKED(bGetNextToken(xTkn));
    CHECKED(xTkn.bSame(":"_Stz));
    vPartRecognised(CP_MEMBER_LABEL);
    return true;
  }
  
  if (szSawTerm->bSame("public"_Stz)) {
    CHECKED(bGetNextToken(xTkn));
    CHECKED(xTkn.bSame(":"_Stz));
    vPartRecognised(CP_MEMBER_LABEL);
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* parse single class member (no label)                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vParseBlockOther(Stz* szSawTerm)
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
    
  vTermsReset();
  vTermAdd(szSawTerm);

  Token xTkn;
  while (bGetNextToken(xTkn)) {
    
    if (xTkn.bSame(";"_Stz)) {
      break;
    }
    
    if (xTkn.bSame("{"_Stz)) {
      bCollectMemberTerms(xTkn.szTerm());
      //CHECKED(bGetNextToken(xTkn))
      break ;
    }

    vTermAdd(xTkn.szTerm());
  }
    
  if (! MetaMember::bIsMemberVar(szClassNm,&m_oMemberTerms)) {
    vTrace("  not a member variable\n"_Stz);
    vPartRecognised(CP_MEMBER_FUNCTION);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8   nPointers;
  bool bFunPtr;
  i8   nElements;
  i8   nBits;

  
  Stz* szVarNm;
  Stz* szTypeNm;
  szVarNm = MetaMember::szMemberNm(&m_oMemberTerms,szTypeNm,
                                   nPointers,bFunPtr,nElements,nBits);
  
  m_p_MetaMember = m_p_MetaBlock->pAddMember(szVarNm);
  
  m_p_MetaMember->vTypeNm(szTypeNm);
  m_p_MetaMember->vPointers(nPointers);
  m_p_MetaMember->vFunPtr(bFunPtr);
  m_p_MetaMember->vElements(nElements);
  m_p_MetaMember->vBits(nBits);

  vPartRecognised(CP_MEMBER_VARIABLE);

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* can we recognise: struct {                                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vParseBlockStruct(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("static"_Stz));

  Token xTkn;
  
  vBlockPushNested("struct"_Stz,MB_STRUCT);
  //vProcStructMember();
  
  vThrowProblem(__LINE__);

  m_p_MetaBlock = nullptr;
}

/*----------------------------------------------------------------------------*/
/* can we recognise: union  {                                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vParseBlockUnion(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  RASSERT(szSawTerm->bSame("union"_Stz));
  
  Token xTkn;
    
  vBlockPushNested("union"_Stz,MB_UNION);
  //vProcUnionMember();
  vThrowProblem(__LINE__);
  m_p_MetaBlock = nullptr;
}


/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                            term collection                                 */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* collect delimited tokens (potentially nested)                              */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectBalanced(Stz* szOpen, Stz* szClose)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
         
  Token xTkn;
  for (;;) {
    
    CHECKED(bGetNextTerm(xTkn));
    
    // test for close
    if (xTkn.bSame(szClose))
      return;
    
    // test for nested open
    if (xTkn.bSame(szOpen))
      vCollectBalanced(szOpen,szClose);
      
    // ignore any other token
  }
  
}

/*----------------------------------------------------------------------------*/
/* collect balanced angle brackets pair: < ... >                              */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectBalancedAngles(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vCollectBalanced("<"_Stz,">"_Stz);
}

/*----------------------------------------------------------------------------*/
/* collect balanced curly braces pair: { ... }                                */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectBalancedBraces(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vCollectBalanced("{"_Stz,"}"_Stz);
}

/*----------------------------------------------------------------------------*/
/* collect balanced parenthesis pair: ( ... )                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectBalancedParenthesis(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vCollectBalanced("("_Stz,")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* collect other terms: <other> ... | ; | { ... }                             */
/*----------------------------------------------------------------------------*/
void Vortex::vCollectOtherTerms(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Token xTkn;
  for (;;) {
    
    CHECKED(bGetNextTerm(xTkn));
    
    // test for terminating ;
    if (xTkn.bSame(";"_Stz))
      return;
    
    // test for terminating brace pair
    if (xTkn.bSame("{"_Stz)) {
      vCollectBalancedBraces("{"_Stz);
      return;
    }
    
    // ignore any other token
  }

}





/*----------------------------------------------------------------------------*/
/* collect all terms till given end (optionally adding the end term)          */
/*----------------------------------------------------------------------------*/
bool Vortex::bCollectMemberTerms(Stz* szSawTerm)
{
  vTrace("> %s SAW: %s\n"_Stz,__func__,szSawTerm->azChar());
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vFragmentAdd("@"_Stz);
  
  if (szSawTerm->bSame("}"_Stz)) {
      m_oMemberTerms.vAdd(szSawTerm);
    return true;
  }
  
  m_oMemberTerms.vAdd(szSawTerm);
  
  Token xTkn;
  for (;;) {
    
    CHECKED(bGetNextToken(xTkn));
    
    if (xTkn.bSame("}"_Stz)) {
      m_oMemberTerms.vAdd(xTkn.szTerm());
      break;
    }
    
    m_oMemberTerms.vAdd(xTkn.szTerm());
    
    if (xTkn.bSame("{"_Stz)) {
      bCollectMemberTerms("{"_Stz);
    }
  }

  s48K sTermsString;
  m_oMemberTerms.vNamesString(sTermsString);
  
  vTrace("  %s MemberTerms: %s\n"_Stz,__func__,sTermsString.azChar());
  
  if (m_oMemberTerms.szName(0)->bSame("bIsValid"_Stz))
    vDebugPoint();
  
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
  
  vFragmentAdd("@"_Stz);

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
    vSkipBalancedBraces("{"_Stz);
  }
  else {
    CHECKED(xTkn.bSame(";"_Stz));
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s48K sTermsString;
  m_oFunctionTerms.vNamesString(sTermsString);
  
  vTrace("  %s FunctionTerms: %s ReturnType: %s\n"_Stz,
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

  vFragmentAdd("@"_Stz);
  
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
/* discard delimited tokens (potentially nested)                              */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardBalanced(Stz* szOpen, Stz* szClose)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
         
  Token xTkn;
  for (;;) {
    
    CHECKED(bGetNextTerm(xTkn));
    
    // test for close
    if (xTkn.bSame(szClose))
      return;
    
    // test for nested open
    if (xTkn.bSame(szOpen))
      vCollectBalanced(szOpen,szClose);
      
    // ignore any other token
  }
  
}

/*----------------------------------------------------------------------------*/
/* discard balanced angle brackets pair: < ... >                              */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardBalancedAngles(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vDiscardBalanced("<"_Stz,">"_Stz);
}

/*----------------------------------------------------------------------------*/
/* discard balanced curly braces pair: { ... }                                */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardBalancedBraces(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vDiscardBalanced("{"_Stz,"}"_Stz);
}

/*----------------------------------------------------------------------------*/
/* discard balanced parenthesis pair: ( ... )                                 */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardBalancedParenthesis(Stz* szSawTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  vDiscardBalanced("("_Stz,")"_Stz);
}



/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                              term skipping                                 */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* skip till balanced closing brace                                           */
/*----------------------------------------------------------------------------*/
void Vortex::vSkipBalancedBraces(Stz* szSawTerm)
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
      vSkipBalancedBraces(xTkn.szTerm());
      
    // ignore any other token
  }
  
}

/*----------------------------------------------------------------------------*/
/* skip till given term                                                       */
/*----------------------------------------------------------------------------*/
void Vortex::vDiscardTill(Stz* szTerm, Stz* szTill)
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


/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */
/*                                   terms                                    */
/*- + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + */

/*----------------------------------------------------------------------------*/
/* get term                                                                   */
/*----------------------------------------------------------------------------*/
Stz* Vortex::szTerm(i8 i_Term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  Stz* sz = m_oOtherTerms.szName(i_Term);
  
  return sz;
}

/*----------------------------------------------------------------------------*/
/* add term                                                                   */
/*----------------------------------------------------------------------------*/
void Vortex::vTermAdd(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_oMemberTerms.vAdd(szTerm);
}

/*----------------------------------------------------------------------------*/
/* are terms a function ?                                                     */
/*----------------------------------------------------------------------------*/
bool Vortex::bTermsAreFunction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);

  Stz* szTerm;
 
  i8 nTerms = m_oMemberTerms.nItems();
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // term 0
  
  i8 i_Term = 0;
  szTerm = m_oMemberTerms.szName(i_Term++);
  
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
  szTerm = m_oMemberTerms.szName(i_Term++);
  while (szTerm->bSame("*"_Stz))
    szTerm = m_oMemberTerms.szName(i_Term++);
  
  // -- -- -- -- -- --
  // test for member which is a member which is a function pointer
  if (m_bInClass && szTerm->bSame("("_Stz))
    return false;

  // -- -- -- -- -- --
  // attempt to detect a function
  szTerm = m_oMemberTerms.szName(i_Term++);
  while (i_Term < nTerms) {
    if (szTerm->bSame("("_Stz))
      return true;
    szTerm = m_oMemberTerms.szName(i_Term++);
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
  
   i8 nTerms = m_oMemberTerms.nItems();
     
   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   // term 0
   
   i8 i_Term = 0;
   szTerm = m_oMemberTerms.szName(i_Term++);
  
  // -- -- -- -- -- --
  // exclude constructor function
  if (m_bInClass && szTerm->bSame(m_sClassNm.szStz()) &&
      m_oMemberTerms.szName(i_Term)->bSame("("_Stz)) {
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
  szTerm = m_oMemberTerms.szName(i_Term++);
  while (szTerm->bSame("*"_Stz))
    szTerm = m_oMemberTerms.szName(i_Term++);
  
  // -- -- -- -- -- --
  // test for variable which is a function pointer
  if (szTerm->bSame("("_Stz))
    return true;

  // -- -- -- -- -- --
  // attempt to detect a function
  szTerm = m_oMemberTerms.szName(i_Term++);
  while (i_Term < nTerms) {
    if (szTerm->bSame("("_Stz))
      return false;
    szTerm = m_oMemberTerms.szName(i_Term++);
  }
  
  // -- -- -- -- -- --
  // must be a variable
  return true;
}

/*----------------------------------------------------------------------------*/
/* get number of terms                                                         */
/*----------------------------------------------------------------------------*/
i8 Vortex::nTerms(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  i8 nTerms = m_oOtherTerms.nItems();
  
  return nTerms;
}

/*----------------------------------------------------------------------------*/
/* reset terms                                                                */
/*----------------------------------------------------------------------------*/
void Vortex::vTermsReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_VORTEX,_FOOTER_);
  
  m_oMemberTerms.vReset();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
