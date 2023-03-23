/*============================================================================*/
/* LibInfo.hpp - Stateful information about compiling and using a dynamic lib */
/*============================================================================*/
#pragma once

#include "eEC.h"

namespace BASE { class Text; };
namespace STORE
{
  class LibInfo : public Object
  {
    friend class CompileLib;

  private:
    s256  m_sBaseName;
    s256  m_sSrcFilePath;
    s256  m_sObjFilePath;
    s256  m_sLogFilePath;
    s256  m_sDynLibPath;

    void* m_pvDynLib;
    eEC   m_eErrCode;
    Text* m_pErrDetails;

    i8    m_iCompileTime;
    i8    m_iDynLinkTime;
    i8    m_iDynUnlinkTime;
    i8    m_iDeleteTime;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    LibInfo(void);
    ~LibInfo(void);

    eEC  eErrCode();
    Stz* szErrDetails();

    Footer* aFooter(void);

  private:
    void vClearErrors();
    Stz* szBaseName();
  };

  LibInfo* _pLibInfo(void* aAddr);
} // STORE

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
