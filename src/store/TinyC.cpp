
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

  m_aTccState   = nullptr;
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* acquire                                                                    */
/*----------------------------------------------------------------------------*/
void TinyC::vAcquire(void* pOpaque, void (fTinyError)(void*, const char*))
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TINYC, _FOOTER_);

  m_aTccState   = tcc_new();
  tcc_set_output_type(m_aTccState,TCC_OUTPUT_MEMORY);
  tcc_set_error_func(m_aTccState, pOpaque, fTinyError);
}

/*----------------------------------------------------------------------------*/
/*  add a symbol                                                              */
/*----------------------------------------------------------------------------*/
void TinyC::vAddSymbol(char* pSymbol, const void* fSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TINYC,_FOOTER_);

  tcc_add_symbol(m_aTccState,pSymbol,fSymbol);
}

/*----------------------------------------------------------------------------*/
/* compile the code                                                           */
/*----------------------------------------------------------------------------*/
bool TinyC::bCompile(char* pSrcCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TINYC,_FOOTER_);

  // Compilation search path so that dynamically generated code can find include files for libtcc1.a
  tcc_add_include_path(m_aTccState, "../thirdparty/tcc/lib/tcc/include");

  // Library search path so that dynamically generated code can find the libtcc1.a library
  tcc_add_library_path(m_aTccState,"../thirdparty/tcc/lib/tcc");

  // Library search path so that dynamically generated code can find the libtcc1.a library
  // It's not entirely clear if this is required in addition to
  // the call to tcc_add_library_path above.
  // Some reference here: https://lists.gnu.org/archive/html/tinycc-devel/2010-11/msg00023.html
  tcc_set_lib_path(m_aTccState,"../thirdparty/tcc/lib/tcc");

  if (-1 == tcc_compile_string(m_aTccState,pSrcCode))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*  find the symbol                                                           */
/*----------------------------------------------------------------------------*/
void* TinyC::pFindSymbol(char* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TINYC,_FOOTER_);

  void* fSymbol;
  fSymbol = (void *)tcc_get_symbol(m_aTccState,pSymbol);

  return fSymbol;
}

/*----------------------------------------------------------------------------*/
/*  relocate object to machine code                                           */
/*----------------------------------------------------------------------------*/
bool TinyC::bRelocate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TINYC,_FOOTER_);

  if (tcc_relocate(m_aTccState, TCC_RELOCATE_AUTO) < 0) {
    return false;
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
