/*============================================================================*/
/* Dir.hpp -- directory services                                              */
/*============================================================================*/
#pragma once

namespace System
{
  class Dir : public Object
  {
  private:
    s256    m_sDirPath;   // the path of the directory
   
    DIR*    m_aDir;       // dir pointer/descriptor
    s256    m_sEntryName; // name of entry just read
    i8      m_iEntryType; // type of entry just read
 
    SysCall m_oSysCall;   // system call handler
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pDirAddress);
    
    Dir(void);
   ~Dir(void) noexcept(false);

    static bool bChangeCurrent(Stz* aDirPath);
    static bool bGetCurrent(Stz* aDirPath);
    static bool bExists(Stz* aDirPath);
    static bool bMake(Stz* aDirPath);
    static bool bRemove(Stz* aDirPath);
    
    void vClose(void);
    StrVal& sEntryName(void);
    i8   iEntryType(void);
    bool bIsClosed(void);
    bool bIsOpen(void);
    bool bOpen(Stz* aDirPath);
    StrVal& sPath(void);
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
