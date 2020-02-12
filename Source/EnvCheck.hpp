/*============================================================================*/
/* EnvCheck.hpp -- Environment Check                                          */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class EnvCheck : public Object
  {
  public:
  
    static void vCheckManifest(void);
  
    static void vCheckDir(Stz* szDirPath, i8 iDisposition, i8 iPermissions);
    static void vCheckFile(Stz* szFilePath, i8 iDisposition, i8 iPermissions);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
