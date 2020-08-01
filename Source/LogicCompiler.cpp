/*============================================================================*/
/* LogicCompiler.cpp -- Logic Compiler                                        */
/*============================================================================*/

#include "Expressions.h"

#ifdef __TINYC
#include "libtcc.h"
#endif

char gcaEoFragments[C_128K];
Stz* gszaEoFragments[EO_ENUMS];

extern EoArg EoArgs[];

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Expressions,LogicCompiler,CN_LOGICCOMPILER)
__aNewFunction(LogicCompiler,CN_LOGICCOMPILER)
__vDeleteFunction(LogicCompiler,CN_LOGICCOMPILER)
__aFooterFunction(LogicCompiler)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LogicCompiler::LogicCompiler(void)
: Object(CN_LOGICCOMPILER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
#ifdef WITH_DEBUG_VERSION
  MemLib::vSet(m_caSource,0xFF,C_LOGIC_SOURCE,CN_LOGICCOMPILER);
#endif
  
  m_caSource[0] = '\0';
  
  m_azSrcCrs = m_caSource;
  
  m_aTccState = nullptr;
  
  _checkInitialised_(CN_LOGICCOMPILER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LogicCompiler::~LogicCompiler(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract EO_ code fragments                                                 */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vExtract(void)
{
  _static_
    
  vTrace("LogicCompiler::vExtract -- Extract EO_ Fragments\n"_Stz);
  
  File* pFile = new File(nullptr);
  
  bool b = pFile->bOpenRead("../Source/LogicUnit.cpp"_Stz);
  RASSERT(b);
  
  char* azCrs = gcaEoFragments;
  
  s4K sTok;
  while (pFile->bNextLiteTok(sTok)) {
    
    if (sTok.bSame("//+ExtractBegin"_Stz)) {
      vTrace("Found +ExtractBegin\n"_Stz);
      while (bExtractAll(pFile,&azCrs))
        {;}
    }
  }
  
  vTrace("Fragments:: %s\n"_Stz,gcaEoFragments);
  
  for (i8 i_EO = 1; i_EO < EO_ENUMS; i_EO++) {
    vTrace("Fragment:: %s\n%s\n"_Stz,aEO[i_EO],gszaEoFragments[i_EO]);
  }
  
  pFile->vClose();
  
  delete pFile;
}

/*----------------------------------------------------------------------------*/
/* extract all EO_fragments                                                   */
/*----------------------------------------------------------------------------*/
bool LogicCompiler::bExtractAll(File* pFile, char** aazCrs)
{
  _static_
  
  s4K sTok;
  while (pFile->bNextLiteTok(sTok)) {
    if (sTok.bSame("case"_Stz)) {
      bool b = pFile->bNextLiteTok(sTok);
      vExtractFragment(pFile,aazCrs);
      return true;
    }
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* extract an EO_fragment                                                     */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vExtractFragment(File* pFile, char** aazCrs)
{
  _static_
  
  s4K sTok;
  bool b = pFile->bNextLiteTok(sTok);
  
  StrLib::vStrip(sTok.azChar(),':');
  
  i8 i_EO = eFindEO(sTok);
  vTrace("Fragment %s [%lld]\n"_Stz,sTok.szStz(),i_EO);

  gszaEoFragments[i_EO] = (Stz*)*aazCrs;
  
  for (;;) {
    b = pFile->bNextLiteTok(sTok);
    if (!b || sTok.bSame("break;//+endCase"_Stz))
      break;
    
    StrLib::vAppendString(aazCrs,sTok.azChar());
  }
  
  //StrLib::vAppendChar(aazCrs,'\n');
  StrLib::vAppendChar(aazCrs,'\n');
  StrLib::vAppendChar(aazCrs,'\0');
}

/*----------------------------------------------------------------------------*/
/* extract an EO_fragment                                                     */
/*----------------------------------------------------------------------------*/
eEO LogicCompiler::eFindEO(Stz* szEOname)
{
  _static_
  
  for (i8 i_EO = 1; i_EO < EO_ENUMS; i_EO++)
    if (StrLib::bSame(aEO[i_EO]->azChar(), szEOname->azChar()))
      return (eEO)i_EO;
  
  return EO_NONE;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* call                                                                       */
/*----------------------------------------------------------------------------*/
i8 LogicCompiler::iCall(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* compile                                                                    */
/*----------------------------------------------------------------------------*/
i8 LogicCompiler::iCompile(RLU* aRLU, i8 nCodes, eEO eaCode[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);

  i8 iResult = iToSource2(nCodes,eaCode);
  
  vAcquire();
  
  bCompile();
  
  vSymbols();
  
  vCall(aRLU);

  vRelease();
  
  return iResult;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vAcquire(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  RASSERT(! m_aTccState);
  
#ifdef __TINYC
  m_aTccState = tcc_new();
  RASSERT(m_aTccState);
  
  tcc_set_output_type((TCCState*)m_aTccState,TCC_OUTPUT_MEMORY);
  
  tcc_set_error_func((TCCState*)m_aTccState, nullptr, vTinyError);
#endif
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vCall(RLU* aRLU)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
#ifdef __TINYC
  i8 (*afDyn)(void* aRLU);

  afDyn = (i8 (*)(void*))tcc_get_symbol((TCCState*)m_aTccState,"iDynCompute");
  RASSERT(afDyn);
  
  afDyn(aRLU);
#endif
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool LogicCompiler::bCompile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
#ifdef __TINYC
  if (-1 == tcc_compile_string((TCCState*)m_aTccState,m_caSource))
    return false;
#endif
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vRelease(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
#ifdef __TINYC
  tcc_delete((TCCState*)m_aTccState);
#endif

  m_aTccState = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::vSymbols(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
#ifdef __TINYC
  // add external symbols
  tcc_add_symbol((TCCState*)m_aTccState, "iMul", (const void *)iMul);
  tcc_add_symbol((TCCState*)m_aTccState, "xMul", (const void *)xMul);

  // relocate the code
  if (tcc_relocate((TCCState*)m_aTccState, TCC_RELOCATE_AUTO) < 0) {
    RASSERT(0);
  }
#endif
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* tiny-c callback for error printing                                         */
/*----------------------------------------------------------------------------*/
void vTinyError(void *aOpaque, const char *azMsg)
{
  vTrace("TinyError: %s\n"_Stz,azMsg);
  
  vDebugPoint();
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* extern function test : iMul()                                              */
/*----------------------------------------------------------------------------*/
i8 iMul(i8 iTopA, i8 iTopB)
{
  return iTopA * iTopB;
}

/*----------------------------------------------------------------------------*/
/* extern function test : iMul()                                              */
/*----------------------------------------------------------------------------*/
Qty xMul(i8 iTopA, i8 iTopB)
{
  Qty xRes;
  
  xRes.m_iValue = iTopA * iTopB;
  return xRes;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_preliminary(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "\n  /*====================================================================*/"
  "\n  /* Dynamically Generated Code                                         */"
  "\n  /*====================================================================*/"
  "\n"
  "\n  #include <tcclib.h>" // include libc header for TCC
  "\n"
  "\n  typedef char      i1;"
  "\n  typedef long long i8;"
  "\n"
  "\n  #include \"../Source/Qty.h\""
  "\n"
  "\n  static const i8 C_LOGIC_STACK = 4096;"
  "\n"
  "\n  extern i8 iMul(i8 iTopA, i8 iTopB);"
  "\n  extern struct Qty xMul(i8 iTopA, i8 iTopB);"
  "\n"
  "\n  enum eDO { DO_A=0, DO_B, DO_C };"
  "\n"
  "\n  #define NoArg() "
  "\n  #define GetArg(x) "
  "\n  #define dLU0(x) "
  "\n  #define dLU1(x,a) "
  "\n  #define dLU2(x,a,b) "
  "\n  #define dLU3(x,a,b,c) "
  "\n"
  "\n  /*--------------------------------------------------------------------*/"
  "\n  /* rlu compute function                                               */"
  "\n  /*--------------------------------------------------------------------*/"
  "\n  i8 iDynCompute(void* aRLU)" // the dynamic function is: iDynCompute()
  "\n  { "
  "\n    i8 iaDataStack[C_LOGIC_STACK];"
  "\n"
  "\n    i8* aiDstk = &iaDataStack[0];"
  "\n"
  "\n    i8 iTopA;"
  "\n    i8 iTopB;"
  "\n    i8 iRes;"
  "\n    struct Qty xRes;"
  "\n"
  "\n"
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_concluding(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "\n"
  "\n    // hazard"
  "\n    return 0;"
  "\n  }"
  "\n"
  "\n  /*====================================================================*/"
  "\n  /* end                                                                */"
  "\n  /*====================================================================*/"
  "\n"
  "";
  
  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_EO_INTEGER(i8& ri_Code, eEO eaCode[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "\n"
  "\n    // EO_INTEGER"
  "\n    *(++aiDstk) = %lld;"
  "";

  //StrLib::vAppendString(&m_azSrcCrs,azFrag);
  
  eEO* aeCode = &eaCode[ri_Code + 1];
  i8 iArg = * (i8*)aeCode;
  StrLib::vSprintCat((Stz*)m_azSrcCrs,(Stz*)azFrag,iArg);
  
  m_azSrcCrs = StrLib::zNullTerminator(m_azSrcCrs);
  ri_Code += 8;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_EO_ADD(i8& ri_Code, eEO eaCode[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "\n"
  "\n    // EO_ADD"
  "\n    iTopB = *(aiDstk--);"
  "\n    iTopA = *(aiDstk--);"
  "\n    iRes  = iTopA + iTopB;"
  "\n    *(++aiDstk) = iRes;"
  "";

  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_EO_MULTIPLY(i8& ri_Code, eEO eaCode[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
//  "\n"
//  "\n    // EO_MULTIPLY"
//  "\n    iTopB = *(aiDstk--);"
//  "\n    iTopA = *(aiDstk--);"
//  "\n    iRes  = iMul(iTopA,iTopB);"
//  "\n    *(++aiDstk) = iRes;"
//  "";

  "\n"
  "\n    // EO_MULTIPLY"
  "\n    iTopB = *(aiDstk--);"
  "\n    iTopA = *(aiDstk--);"
  "\n    xRes  = xMul(iTopA,iTopB);"
  "\n    *(++aiDstk) = xRes.m_iValue;"
  "";

  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_EO_PRINT(i8& ri_Code, eEO eaCode[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "\n"
  "\n    // EO_PRINT"
  "\n    iTopA = *(aiDstk--);"
  "\n    printf(\"EO_PRINT: %lld\\n\",iTopA);"
  "";

  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void LogicCompiler::v_EO_RETURN(i8& ri_Code, eEO eaCode[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  char* azFrag = (char*)
  "\n"
  "\n    // EO_RETURN"
  "\n    return 1;"
  "";

  StrLib::vAppendString(&m_azSrcCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/* to source code                                                             */
/*----------------------------------------------------------------------------*/
i8 LogicCompiler::iToSource(i8 nCodes, eEO eaCode[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  v_preliminary();
  
  for (i8 i_Code = 0; i_Code < nCodes; i_Code++) {
    
    eEO eCode = eaCode[i_Code];

    switch (eCode) {
        
      case EO_INTEGER:  v_EO_INTEGER  (i_Code,eaCode); break;
      case EO_ADD:      v_EO_ADD      (i_Code,eaCode); break;
      case EO_MULTIPLY: v_EO_MULTIPLY (i_Code,eaCode); break;
      case EO_PRINT:    v_EO_PRINT    (i_Code,eaCode); break;
      case EO_RETURN:   v_EO_RETURN   (i_Code,eaCode); break;

      default:
        break;
    }
  }

  v_concluding();
  
  vTrace("Dynamic C code: %s\n"_Stz,m_caSource);

  return 1;
}

/*----------------------------------------------------------------------------*/
/* to source code                                                             */
/*----------------------------------------------------------------------------*/
i8 LogicCompiler::iToSource2(i8 nCodes, eEO eaCode[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICCOMPILER,_FOOTER_);
  
  v_preliminary();
  
  for (i8 i_Code = 0; i_Code < nCodes; i_Code++) {
    
    eEO eCode = eaCode[i_Code];

    char* azFrag = (char*)gszaEoFragments[eCode];

    StrLib::vAppendString(&m_azSrcCrs,"    // ");
    StrLib::vAppendString(&m_azSrcCrs,aEO[eCode]->azChar());
    StrLib::vAppendString(&m_azSrcCrs,":\n");
    StrLib::vAppendString(&m_azSrcCrs,"       ");

    StrLib::vAppendString(&m_azSrcCrs,azFrag);
    
    i_Code += EoArgs[eCode].m_nArgBytes;
  }

  v_concluding();
  
  vTrace("Dynamic C code: %s\n"_Stz,m_caSource);

  return 1;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
