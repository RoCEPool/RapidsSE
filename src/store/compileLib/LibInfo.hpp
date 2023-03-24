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
    s256  m_sLogFilePath;
    s256  m_sDynLibPath;

    s256  m_sCompileCmd;

    void* m_pvDynLib;
    eEC   m_eErrCode;
    s2K   m_sErrDetails;

    bool  m_bCollectStats;
    u8    m_uCompileNanos;
    u8    m_uDynLinkNanos;
    u8    m_uSymAddrNanos;
    u8    m_uDynUnlinkNanos;
    u8    m_uDeleteNanos;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    LibInfo(bool bCollectStats=false);
    ~LibInfo(void);

    eEC  eErrCode();
    Stz* szErrDetails();
    u8   uCompileNanos();
    u8   uDynLinkNanos();
    u8   uSymAddrNanos();
    u8   uDynUnlinkNanos();
    u8   uDeleteNanos();

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
