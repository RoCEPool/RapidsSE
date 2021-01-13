/*============================================================================*/
/* SenCompiler.cpp -- Sen Word Compiler                                       */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,SenCompiler,CN_COMPILER)
__aNewFunction(SenCompiler,CN_COMPILER)
__vDeleteFunction(SenCompiler,CN_COMPILER)
__aFooterFunction(SenCompiler)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SenCompiler::SenCompiler(void)
: Object(CN_COMPILER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_pCurrent   = nullptr;
  m_eMode      = CM_INTERPRETING;

  m_nLocals    = 0;
  for (i8 i_Nested = 0; i_Nested < C_MAX_NESTED_WORDS; i_Nested++)
    m_paWord[i_Nested] = nullptr;

  _checkInitialised_(CN_COMPILER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SenCompiler::~SenCompiler(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is the compiler in compiling mode?                                         */
/*----------------------------------------------------------------------------*/
bool SenCompiler::bCompiling(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  bool bIsCompiling = CM_COMPILING == m_eMode;
  
  return bIsCompiling;
}

/*----------------------------------------------------------------------------*/
/* get current word                                                           */
/*----------------------------------------------------------------------------*/
Word* SenCompiler::pCurrentWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  return m_pCurrent;
}

/*----------------------------------------------------------------------------*/
/* set current word                                                           */
/*----------------------------------------------------------------------------*/
void SenCompiler::vCurrentWord(Word* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_pCurrent = pWord;
}

/*----------------------------------------------------------------------------*/
/* is the compiler in interpreting mode?                                      */
/*----------------------------------------------------------------------------*/
bool SenCompiler::bInterpreting(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  bool bIsInterpreting = CM_INTERPRETING == m_eMode;
  
  return bIsInterpreting;
}

/*----------------------------------------------------------------------------*/
/* get number of locals                                                       */
/*----------------------------------------------------------------------------*/
i8 SenCompiler::nLocals(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  return m_nLocals;
}

/*----------------------------------------------------------------------------*/
/* set number of locals                                                       */
/*----------------------------------------------------------------------------*/
void SenCompiler::vLocals(i8 nLocals)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_nLocals = nLocals;
  
  static i8 nMaxLocals = 0;
  
  if (nLocals > nMaxLocals) {
    nMaxLocals = nLocals;
//    vTrace("                                                      MaxLocals %lld\n"_Stz,nMaxLocals);
  }
}

/*----------------------------------------------------------------------------*/
/* get mode                                                                   */
/*----------------------------------------------------------------------------*/
eCM SenCompiler::eMode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  return m_eMode;
}

/*----------------------------------------------------------------------------*/
/* set mode                                                                   */
/*----------------------------------------------------------------------------*/
void SenCompiler::vMode(eCM eMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_eMode = eMode;
}

/*----------------------------------------------------------------------------*/
/* get nested word                                                            */
/*----------------------------------------------------------------------------*/
Word* SenCompiler::pNestedWord(i8 i_Nested)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  Word* pNested = m_paWord[i_Nested];
  
  return pNested;
}

/*----------------------------------------------------------------------------*/
/* set nested word                                                            */
/*----------------------------------------------------------------------------*/
void SenCompiler::vNestedWord(i8 i_Nested, Word* pNested)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_paWord[i_Nested] = pNested;
}

/*----------------------------------------------------------------------------*/
/* is the compiler in postponing mode?                                        */
/*----------------------------------------------------------------------------*/
bool SenCompiler::bPostponing(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  bool bIsPostponing = CM_POSTPONING == m_eMode;
  
  return bIsPostponing;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
