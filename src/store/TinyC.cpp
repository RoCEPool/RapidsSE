
/*============================================================================*/
/* TinyC.cpp -- TinyC interface                                               */
/*============================================================================*/

#include "BASE.h"

#include "TinyC.hpp"

#include "libtcc.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(STORE,TinyC,CN_TINYC)
__aNewFunction(TinyC,CN_TINYC)
__vDeleteFunction(TinyC,CN_TINYC)
__aFooterFunction(TinyC)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TinyC::TinyC(void)
    : Object(CN_TINYC)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TINYC, _FOOTER_);

  m_aTccState = nullptr;
  
  m_sFunNm    = "(none)"_Stz;
  m_szSource  = nullptr;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TinyC::~TinyC(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TINYC,_FOOTER_);

  //tcc_delete(m_aTccState);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair and delete the stencil                                              */
/*----------------------------------------------------------------------------*/
void TinyC::vRepair(Object* pObject)
{
  _static_

  TinyC* pTinyC = _pTinyC(pObject);

  // repair the tinyc object
  // ...
  
  // delete the tinyc object
  delete pTinyC;
}

/*----------------------------------------------------------------------------*/
/* tiny-c callback for error printing                                         */
/*----------------------------------------------------------------------------*/
void TinyC::vTinyError(void *aOpaque, const char *azMsg)
{
  TinyC* pTinyC = _pTinyC(aOpaque);

  //vTrace("TinyError Source: \n\n%s\n"_Stz,pTinyC->m_szSource);

  vTrace("TinyError: %s\n"_Stz,azMsg);
  
  vDebugPoint();
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* acquire                                                                    */
/*----------------------------------------------------------------------------*/
void TinyC::vAcquire(void* aOpaque, void (fTinyError)(void*, const char*))
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TINYC, _FOOTER_);

  m_aTccState   = tcc_new();
  tcc_set_output_type(m_aTccState,TCC_OUTPUT_MEMORY);
  tcc_set_error_func(m_aTccState, aOpaque, fTinyError);
}

/*----------------------------------------------------------------------------*/
/*  add a symbol                                                              */
/*----------------------------------------------------------------------------*/
void TinyC::vAddSymbol(char* azSymbol, const void* fSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TINYC,_FOOTER_);

  tcc_add_symbol(m_aTccState,azSymbol,fSymbol);
}

/*----------------------------------------------------------------------------*/
/* begin use of tiny c                                                        */
/*----------------------------------------------------------------------------*/
void TinyC::vBegin()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TINYC,_FOOTER_);

  AM::pTccLock()->vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* compile the code                                                           */
/*----------------------------------------------------------------------------*/
bool TinyC::bCompile(char* azSrcCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TINYC,_FOOTER_);

  m_szSource = (Stz*)azSrcCode;
  // Compilation search path so that dynamically generated code can find include files for libtcc1.a
  tcc_add_include_path(m_aTccState, "../thirdparty/tcc/lib/tcc/include");

  // Library search path so that dynamically generated code can find the libtcc1.a library
  tcc_add_library_path(m_aTccState,"../thirdparty/tcc/lib/tcc");

  // Library search path so that dynamically generated code can find the libtcc1.a library
  // It's not entirely clear if this is required in addition to
  // the call to tcc_add_library_path above.
  // Some reference here: https://lists.gnu.org/archive/html/tinycc-devel/2010-11/msg00023.html
  tcc_set_lib_path(m_aTccState,"../thirdparty/tcc/lib/tcc");

  if (-1 == tcc_compile_string(m_aTccState,azSrcCode))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* end use of tiny c                                                          */
/*----------------------------------------------------------------------------*/
void TinyC::vEnd()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TINYC,_FOOTER_);
  
  AM::pTccLock()->vDropWrite();
}

/*----------------------------------------------------------------------------*/
/*  find the symbol                                                           */
/*----------------------------------------------------------------------------*/
void* TinyC::pFindSymbol(char* azSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TINYC,_FOOTER_);

  void* fSymbol;
  fSymbol = (void *)tcc_get_symbol(m_aTccState,azSymbol);

  return fSymbol;
}

/*----------------------------------------------------------------------------*/
/* get function name                                                          */
/*----------------------------------------------------------------------------*/
Stz* TinyC::szFunNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TINYC,_FOOTER_);

  return m_sFunNm;
}

/*----------------------------------------------------------------------------*/
/* set function name                                                          */
/*----------------------------------------------------------------------------*/
void TinyC::vFunNm(Stz* szFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TINYC,_FOOTER_);

  m_sFunNm = szFunNm;
}

/*----------------------------------------------------------------------------*/
/*  relocate object to machine code                                           */
/*----------------------------------------------------------------------------*/
bool TinyC::bRelocate(void*& raExeAddr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TINYC,_FOOTER_);

//  if (tcc_relocate(m_aTccState, TCC_RELOCATE_AUTO) < 0) {
//    return false;
//  }

  i8 nBytes = tcc_relocate(m_aTccState,nullptr);
  raExeAddr = malloc(nBytes);
  
  if (tcc_relocate(m_aTccState, raExeAddr) < 0) {
    void* aExeAddr = raExeAddr;
    vTrace("Failed to relocate %p\n"_Stz,aExeAddr);
    return false;
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
