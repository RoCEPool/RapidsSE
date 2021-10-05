
/*============================================================================*/
/* TinyC.cpp -- TinyC interface                                               */
/*============================================================================*/

#include "BASE.h"

#include "TinyC.hpp"

#include "libtcc.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*  add a symbol                                                              */
/*----------------------------------------------------------------------------*/
void TinyC::vAddSymbol(void* pTccState, char* pSymbol, const void* fSymbol)
{
  _static_

  tcc_add_symbol((TCCState*)pTccState,pSymbol,fSymbol);
}

/*----------------------------------------------------------------------------*/
/* compile the code                                                           */
/*----------------------------------------------------------------------------*/
bool TinyC::bCompile(void* pTccState, char* pSrcCode)
{
  _static_

  // Compilation search path so that dynamically generated code can find include files for libtcc1.a
  tcc_add_include_path((TCCState*)pTccState, "../thirdparty/tcc/lib/tcc/include");

  // Library search path so that dynamically generated code can find the libtcc1.a library
  tcc_add_library_path((TCCState*)pTccState,"../thirdparty/tcc/lib/tcc");

  // Library search path so that dynamically generated code can find the libtcc1.a library
  // It's not entirely clear if this is required in addition to
  // the call to tcc_add_library_path above.
  // Some reference here: https://lists.gnu.org/archive/html/tinycc-devel/2010-11/msg00023.html
  tcc_set_lib_path((TCCState*)pTccState,"../thirdparty/tcc/lib/tcc");

  if (-1 == tcc_compile_string((TCCState*)pTccState,pSrcCode))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* finalize TinyC                                                             */
/*----------------------------------------------------------------------------*/
void TinyC::vFinalize(void* pTccState)
{
  _static_

  tcc_delete((TCCState*)pTccState);
}

/*----------------------------------------------------------------------------*/
/*  find the symbol                                                           */
/*----------------------------------------------------------------------------*/
void* TinyC::pFindSymbol(void* pTccState, char* pSymbol)
{
  _static_

  void* fSymbol;
  fSymbol = (void *)tcc_get_symbol((TCCState*)pTccState,pSymbol);

  return fSymbol;
}

/*----------------------------------------------------------------------------*/
/* initialize TinyC                                                           */
/*----------------------------------------------------------------------------*/
void* TinyC::pInitialize(void* pOpaque, void (fTinyError)(void*, const char*))
{
  _static_

  void* pTccState   = tcc_new();

  tcc_set_output_type((TCCState*)pTccState,TCC_OUTPUT_MEMORY);

  tcc_set_error_func((TCCState*)pTccState, pOpaque, fTinyError);

  return pTccState;
}

/*----------------------------------------------------------------------------*/
/*  relocate object to machine code                                           */
/*----------------------------------------------------------------------------*/
bool TinyC::bRelocate(void* pTccState)
{
  _static_

  if (tcc_relocate((TCCState*)pTccState, TCC_RELOCATE_AUTO) < 0) {
    return false;
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
