/*============================================================================*/
/* Compiler.cpp -- Sen Word Compiler                                          */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(SenVM,Compiler,CN_COMPILER)
__aNewFunction(Compiler,CN_COMPILER)
__vDeleteFunction(Compiler,CN_COMPILER)
__aFooterFunction(Compiler)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Compiler::Compiler(void)
: Object(CN_COMPILER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_pCurrent   = nullptr;
  m_eMode      = CM_INTERPRETING;

  m_pTokenStream = new TokenStream;
  
//  m_pTokeniser = m_pTokenStream->pTokeniser();
  m_pTokeniser = new Tokeniser;

  m_nLocals    = 0;
  for (i8 i_Nested = 0; i_Nested < C_MAX_NESTED_WORDS; i_Nested++)
    m_paWord[i_Nested] = nullptr;

  _checkInitialised_(CN_COMPILER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Compiler::~Compiler(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  delete m_pTokeniser;
  delete m_pTokenStream;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is the compiler in compiling mode?                                         */
/*----------------------------------------------------------------------------*/
bool Compiler::bCompiling(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  bool bIsCompiling = CM_COMPILING == m_eMode;
  
  return bIsCompiling;
}

/*----------------------------------------------------------------------------*/
/* expect cpp tokens                                                          */
/*----------------------------------------------------------------------------*/
void Compiler::vCppTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_pTokeniser->vCppTokens();
}

/*----------------------------------------------------------------------------*/
/* get current word                                                           */
/*----------------------------------------------------------------------------*/
Word* Compiler::pCurrentWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  return m_pCurrent;
}

/*----------------------------------------------------------------------------*/
/* set current word                                                           */
/*----------------------------------------------------------------------------*/
void Compiler::vCurrentWord(Word* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_pCurrent = pWord;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Compiler::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_pTokeniser->vDump();
}

/*----------------------------------------------------------------------------*/
/* is the compiler in interpreting mode?                                      */
/*----------------------------------------------------------------------------*/
bool Compiler::bInterpreting(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  bool bIsInterpreting = CM_INTERPRETING == m_eMode;
  
  return bIsInterpreting;
}

/*----------------------------------------------------------------------------*/
/* set line mode                                                              */
/*----------------------------------------------------------------------------*/
void Compiler::vLineMode(bool bLineMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_pTokeniser->vLineMode(bLineMode);
}


/*----------------------------------------------------------------------------*/
/* get number of locals                                                       */
/*----------------------------------------------------------------------------*/
i8 Compiler::nLocals(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  return m_nLocals;
}

/*----------------------------------------------------------------------------*/
/* set number of locals                                                       */
/*----------------------------------------------------------------------------*/
void Compiler::vLocals(i8 nLocals)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_nLocals = nLocals;
}

/*----------------------------------------------------------------------------*/
/* get mode                                                                   */
/*----------------------------------------------------------------------------*/
eCM Compiler::eMode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  return m_eMode;
}

/*----------------------------------------------------------------------------*/
/* set mode                                                                   */
/*----------------------------------------------------------------------------*/
void Compiler::vMode(eCM eMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_eMode = eMode;
}

/*----------------------------------------------------------------------------*/
/* get nested word                                                            */
/*----------------------------------------------------------------------------*/
Word* Compiler::pNestedWord(i8 i_Nested)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  Word* pNested = m_paWord[i_Nested];
  
  return pNested;
}

/*----------------------------------------------------------------------------*/
/* set nested word                                                            */
/*----------------------------------------------------------------------------*/
void Compiler::vNestedWord(i8 i_Nested, Word* pNested)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_paWord[i_Nested] = pNested;
}

/*----------------------------------------------------------------------------*/
/* pop charser                                                                */
/*----------------------------------------------------------------------------*/
void Compiler::vPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  bool bPopped = m_pTokeniser->bPopCharser();
  ASSERT(bPopped);
}

/*----------------------------------------------------------------------------*/
/* is the compiler in postponing mode?                                        */
/*----------------------------------------------------------------------------*/
bool Compiler::bPostponing(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  bool bIsPostponing = CM_POSTPONING == m_eMode;
  
  return bIsPostponing;
}

/*----------------------------------------------------------------------------*/
/* push charser                                                               */
/*----------------------------------------------------------------------------*/
void Compiler::vPush(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_pTokeniser->vPushCharser();
}

/*----------------------------------------------------------------------------*/
/* expect sen tokens                                                          */
/*----------------------------------------------------------------------------*/
void Compiler::vSenTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_pTokeniser->vSenTokens();
}

/*----------------------------------------------------------------------------*/
/* advance to next token                                                      */
/*----------------------------------------------------------------------------*/
void Compiler::vReadToken_cpl(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);

  m_pTokeniser->vReadToken_nil();
}

/*----------------------------------------------------------------------------*/
/* get token type                                                             */
/*----------------------------------------------------------------------------*/
eTN Compiler::eToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  eTN eToken = m_pTokeniser->eToken();
  
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* get current token                                                          */
/*----------------------------------------------------------------------------*/
Stz* Compiler::szToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);

  Stz* szTerm = m_pTokeniser->szToken();
  //vTrace("Compiler:: %s\n"_Stz,szTerm);

  return m_pTokeniser->szToken();
}

/*----------------------------------------------------------------------------*/
/* get tokeniser                                                              */
/*----------------------------------------------------------------------------*/
Tokeniser* Compiler::pTokeniser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  return m_pTokeniser;
}

/*----------------------------------------------------------------------------*/
/* use begin                                                                  */
/*----------------------------------------------------------------------------*/
void Compiler::vUseBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_pTokeniser->vUseBegin();
}

/*----------------------------------------------------------------------------*/
/* use end                                                                    */
/*----------------------------------------------------------------------------*/
void Compiler::vUseEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_pTokeniser->vUseEnd();
}

/*----------------------------------------------------------------------------*/
/* use string                                                                 */
/*----------------------------------------------------------------------------*/
void Compiler::vUseFile(Stz* szPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  bool bOpened = m_pTokeniser->bUseFile(szPath);
  
  if ( ! bOpened )
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "No such file: %s"_Stz,szPath);
}

/*----------------------------------------------------------------------------*/
/* use end                                                                    */
/*----------------------------------------------------------------------------*/
void Compiler::vUseStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_pTokeniser->vUseStdinX();
}

/*----------------------------------------------------------------------------*/
/* use string                                                                 */
/*----------------------------------------------------------------------------*/
void Compiler::vUseString(Stz* szString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_pTokeniser->vUseString(szString);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
