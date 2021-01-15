/*============================================================================*/
/* SeApi.hpp -- Se Reentrant API                                              */
/*============================================================================*/
#pragma once

namespace STORE
{
  class SeApi : public Object
  {
  private:
  
    s48K m_sSequence;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* pSeApi);
    
    SeApi(void);
   ~SeApi(void) noexcept(false);
  
    static Stz* szCurrentFolderNm(String* szFolderNm);
    static Stz* szCurrentFolderPath(String* szFolderPath);
    static bool bIsKnownTableNm(Stz* szTableNm);
    static bool bIsKnownTableColumnNm(Stz* szTableNm,Stz* szColumnNm);
    static void vPrintAllColumnsOfTable(Stz* szTableNm);
    static void vPrintAllTablesInCurrentFolder(void);

    SeApi& operator |(SeApi* pArg);

    SeApi* pAdd(Stz* szOpNm, Stz* szOpArg);
    SeApi* pEncode(Stz* szAction);
    SeApi* pEncode(Stz* szAction, Stz* szArgument);

    Stz*   szGet(void);

  private:
    
    Footer* aFooter(void);
  };
  
  SeApi* _pSeApi(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
