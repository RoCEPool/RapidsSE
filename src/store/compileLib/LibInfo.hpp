/*============================================================================*/
/* LibInfo.hpp - Stateful information about compiling and using a dynamic lib */
/*============================================================================*/
#pragma once

#include "eEC.h"
#include "eCB.h"

namespace BASE { class Text; };
namespace STORE
{
  class LibInfo : public Object
  {
    friend class CompileLib;

  private:
    s256  m_sBaseName;        // The unique base of all filenames
    s256  m_sSrcFilePath;     // Path of the single source file being compiled
    s256  m_sLogFilePath;     // Path of the log file capturing the compilation output
    s256  m_sDynLibPath;      // Path of the compiled shared object

    eCB   m_eCompilerType;    // The breed of compiler we're using
    s256  m_sCompilerPath;    // The path to the compiler specified
    s256  m_sCompileCmd;      // The command used to compile the source file to the shared object

    void* m_pvDynLib;         // Handle of the dynamically loaded shared object
    eEC   m_eErrCode;         // The last error code encountered or EC_NONE
    s2K   m_sErrDetails;      // Any extra details associated with the above error code

    bool  m_bCollectStats;    // If true then the timings below will be measured
    u8    m_uCompileNanos;    // Time in ns to write the src to disk and compile it
    u8    m_uDynLinkNanos;    // Time in ns to dynamically load the shared object
    u8    m_uSymAddrNanos;    // Time in ns to resolve a symbol name to an address
    u8    m_uDynUnlinkNanos;  // Time in ns to unload the shared object from memory
    u8    m_uDeleteNanos;     // Time in ns to delete all src/log/bin files previously created

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
