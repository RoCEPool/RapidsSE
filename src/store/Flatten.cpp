
/*============================================================================*/
/* Flatten.cpp -- Flatten a query                                             */
/*============================================================================*/

#include "BASE.h"

#include "Flatten.hpp"
#include "TinyC.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Flatten,CN_FLATTEN)
__aNewFunction(Flatten,CN_FLATTEN)
__vDeleteFunction(Flatten,CN_FLATTEN)
__aFooterFunction(Flatten)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Flatten::Flatten(void)
: Object(CN_FLATTEN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FLATTEN,_FOOTER_);

  m_pCodeSynth = new CodeSynth;
  m_pTinyC     = new TinyC;
  m_afFlatten  = nullptr;

  _checkInitialised_(CN_FLATTEN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_FLATTEN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Flatten::~Flatten(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FLATTEN,_FOOTER_);

  delete m_pCodeSynth;
  delete m_pTinyC;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test flatten                                                               */
/*----------------------------------------------------------------------------*/
void Flatten::vTestFlatten(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FLATTEN,_FOOTER_);

  CodeSynth* pCodeFragment = new CodeSynth;

  // append preliminary code snippet
  m_pCodeSynth->vAppendPreliminary();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the code fragment
  pCodeFragment->vAppendFragment( "    printf(\"Flattended query\\n\");"_Stz);

  // append code fragment
  m_pCodeSynth->vAppendFragment(pCodeFragment);
  delete pCodeFragment;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append concluding code snippet
  m_pCodeSynth->vAppendConcluding();

  // print the flattened code
  vTrace("Flattened query code: %s\n"_Stz,m_pCodeSynth->szCode());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compile the flattened code
  bool bCompile = bCompileCode(m_pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // execute the flattened code
  bool bResult = bCallFlatten();
}

/*----------------------------------------------------------------------------*/
/* call the dynamic function                                                  */
/*----------------------------------------------------------------------------*/
bool Flatten::bCallFlatten(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FLATTEN,_FOOTER_);

  return m_afFlatten();
}

/*----------------------------------------------------------------------------*/
/* return the codeSynth                                                       */
/*----------------------------------------------------------------------------*/
CodeSynth* Flatten::pCodeSynth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FLATTEN,_FOOTER_);

  return m_pCodeSynth;
}
/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compile the synthesised code                                               */
/*----------------------------------------------------------------------------*/
bool Flatten::bCompileCode(CodeSynth* pCodeSynth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FLATTEN,_FOOTER_);

  AM::pTccLock()->vGrabWrite();
  m_pTinyC->vAcquire(this, vTccError);

  bool bCompile = m_pTinyC->bCompile((char*)pCodeSynth->szCode());

  //m_pTinyC->vAddSymbol((char*)bFlatten,
  //                     (void*)bFlatten);

  // relocate the code
  if ( ! m_pTinyC->bRelocate() )
    RASSERT(0);

  m_afFlatten =
      (bool (*)(void))m_pTinyC->pFindSymbol("bFlatten");

  RASSERT(m_afFlatten);

  AM::pTccLock()->vDropWrite();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* tiny-c callback for error printing                                         */
/*----------------------------------------------------------------------------*/
void vTccError(void *aOpaque, const char *azMsg)
{
  Flatten* pFlatten = _pFlatten(aOpaque);

  vTrace("Dynamic Source Code: \n\n%s\n"_Stz,pFlatten->pCodeSynth()->szCode());

  vTrace("TccError: %s\n"_Stz,azMsg);

  vDebugPoint();

  NEVER_REACH;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
