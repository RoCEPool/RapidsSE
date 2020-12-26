/*============================================================================*/
/* Dir.hpp -- directory services                                              */
/*============================================================================*/
#pragma once

#include "Resource.hpp"

static const i8 C_ACCESS_READ      = R_OK;
static const i8 C_ACCESS_WRITE     = W_OK;
static const i8 C_ACCESS_READWRITE = R_OK | W_OK;
static const i8 C_ACCESS_NOCHECK   = -1;

static const i8 C_EXIST_NOCHECK   = -1;
static const i8 C_EXIST_EXISTS    = 1;
static const i8 C_EXIST_NOTEXISTS = 2;

namespace Resources
{
  class Dir : public Resource
  {
  private:
    s256    m_sDirPath;   // the path of the directory
   
    DIR*    m_aDir;       // dir pointer/descriptor
  
    s256    m_sEntryNm;   // name of entry just read
    i8      m_iEntryType; // type of entry just read
 
    SysCall m_oSysCall;   // system call handler
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pDirAddress);
    
    Dir(Focal* pIsFor);
   ~Dir(void) noexcept(false);

    static bool bAccess(Stz* szDirPath, i8 iPermission);
    static bool bChangeCurrent(Stz* szDirPath);
    static bool bGetCurrent(Stz* szDirPath);
    static bool bExists(Stz* szDirPath);
    static bool bMake(Stz* szDirPath);
    static bool bRemove(Stz* szDirPath);
    
    void vClose(void);
    Stz* szEntryNm(void);
    i8   iEntryType(void);
    void vFocalOutline(s4K& sOutline);
    bool bIsClosed(void);
    bool bIsOpen(void);
    bool bOpen(Stz* aDirPath);
    Stz* szPath(void);
    bool bRead(void);
    void vRewind(void);
    void vSeek(i8 iPosition);
    i8   iTell(void);
    
    Footer* aFooter(void);
  };

  Dir* _pDir(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
