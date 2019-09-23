/*============================================================================*/
/* JobObjectName.hpp -- Object Name part for a Job                            */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobObjectName
  {
  private:
    s32  m_sObjectName; // object name (e.g. schema, table etc)
    s32  m_sIndexName;  // index name
    
  public:
    void* operator new(size_t);
  
    JobObjectName(void);
   ~JobObjectName(void) noexcept(false);
  
    Stz* szCatalogName(void);
    void vCatalogName(Stz* szCatalogName);
  
    Stz* szFolderName(void);
    void vFolderName(Stz* szFolderName);
  
    Stz* szIndexName(void);
    void vIndexName(Stz* szIndexName);
  
    Stz* szNodeName(void);
    void vNodeName(Stz* szNodeName);
  
    Stz* szObjectName(void);
    void vObjectName(Stz* szObjectName);
  
    Stz* szTenantName(void);
    void vTenantName(Stz* szTenantName);
  
    Stz* szSchemaName(void);
    void vSchemaName(Stz* szSchemaName);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
