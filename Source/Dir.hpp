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

    bool bChangeCurrent(s256& rPath);
    bool bExists(s256& rPath);
    bool bGetCurrent(s256& rPath);
    void vInit(void);
    bool bMake(s256& rPath);
    bool bRemove(s256& rPath);
    
    //--
    
    void vClose(void);
    StrVal&   sEntryName(void);
    i8   iEntryType(void);
    bool bIsClosed(void);
    bool bIsOpen(void);
    bool bOpen(Stz* aDirPath);
    StrVal&   sPath(void);
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
