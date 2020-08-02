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
    i8 iCompile(LogicUnit* pLogicUnit, RLU* aRLU,
                i8 nCodes, eEO eaCode[],
                i8 nLabels, struct JumpLabel xaJumpLabel[]);
  
  private:
  
    void vAcquire(void);
    void vBegin(void);
    void vCall(RLU* aRLU);
    bool bCompile(void);
    void vEnd(void);
    void vRelease(void);
    void vSymbols(void);

    i8 iToSourceFun(LogicUnit* pLogicUnit,
                 i8 nCodes, eEO eaCode[],
                 i8 nLabels, struct JumpLabel xaJumpLabel[]);
    i8 iToSourceSbr(LogicUnit* pLogicUnit,
                    i8 nCodes, eEO eaCode[],
                    i8 nLabels, struct JumpLabel xaJumpLabel[]);
  
    bool bFindSbr(LogicUnit* pLogicUnit,
                  i8 nCodes, eEO eaCode[],
                  i8 nLabels, struct JumpLabel xaJumpLabel[],
                  i8& ri_From, i8& ri_Start, i8& ri_End);
  
    void v_DynPreliminary(void);
    void v_DynConcluding(void);

    void v_FunPreliminary(void);
    void v_FunConcluding(void);

    void v_SbrPreliminary(i8 i_Code);
    void v_SbrConcluding(void);
  
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
