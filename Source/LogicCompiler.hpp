/*============================================================================*/
/* LogicCompiler.hpp -- Logic Compiler                                        */
/*============================================================================*/
#pragma once

namespace Expressions
{
  static const i8 C_LOGIC_SOURCE  = C_SIZE_LARGE_OBJECT - C_1KB;

  //----------------------------------------------------------------------------

  class LogicCompiler : public Object
  {
  private:
  
    char  m_caSource[C_LOGIC_SOURCE];
    char* m_azSrcCrs;
    void* m_aTccState;
  
    LogicUnit* m_pLogicUnit;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    LogicCompiler(void);
   ~LogicCompiler(void) noexcept(false);
  
    static void vExtract(void);
    static bool bExtractAll(File* pFile, char** aazCrs);
    static void vExtractFragment(File* pFile, char** aazCrs);

    static eEO  eFindEO(Stz* szEOname);

    i8 iCall(void);
    i8 iCompile(LogicUnit* pLogicUnit, void* aDynArg);
  
  private:
  
    void vAcquire(void);
    void vCall(void* aDynArg);
    bool bCompile(void);
    void vRelease(void);
    void vRelocate(void);
    void vSymbols(void);

    void vAddFragment(i8 i_CodeFrom, i8 i_CodeTill);
    bool bEachSbr(i8& ri_Label, i8& ri_CodeFrom, i8& ri_CodeTill);
  
    void vRtnSource(void);
    void vSbrSource(void);
  
    void v_DynPreliminary(void);
    void v_DynConcluding(void);

    void v_RtnPreliminary(void);
    void v_RtnConcluding(void);

    void v_SbrPreliminary(Stz* szSbrNm);
    void v_SbrConcluding(void);
  
    void v_CallSbr(i8 i_Label);
  
    Footer* aFooter(void);
  };
  
  LogicCompiler* _pLogicCompiler(void* aAddr);
}

  void vTinyError(void *aOpaque, const char *azMsg);
  i8 iMul(i8 iTopA, i8 iTopB);

  Qty xMul(i8 iTopA, i8 iTopB);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
