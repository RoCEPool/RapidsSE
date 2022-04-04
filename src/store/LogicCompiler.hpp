/*============================================================================*/
/* LogicCompiler.hpp -- Logic Compiler                                        */
/*============================================================================*/
#pragma once

#include "eEO.h"

#include "TinyC.hpp"

namespace STORE
{
  static const i8 C_LOGIC_SOURCE  = C_SIZE_LARGE_OBJECT - C_1KB;

  struct DynamicSymbol
  {
    Stz* m_szSymbolNm;
    const void *m_afSymbol;
  };

  //----------------------------------------------------------------------------

  class LogicCompiler : public Object
  {
  private:
  
    char   m_caSource[C_LOGIC_SOURCE];
    char*  m_azSrcCrs;
    TinyC* m_pTinyC;
  
    LogicUnit* m_pLogicUnit;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    LogicCompiler(LogicUnit* pLogicUnit);
   ~LogicCompiler(void) noexcept(false);

    static void vRepair(Object* pObject);

    Stz* szSource(void);

  private:
  
    void vAcquire(void);
    void vRelease(void);
    void vRelocate(void);

    Footer* aFooter(void);
  };
  
  LogicCompiler* _pLogicCompiler(void* aAddr);
}

  void vTinyError(void *aOpaque, const char *azMsg);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
