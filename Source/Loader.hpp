/*============================================================================*/
/* Loader.hpp -- Table Loader                                                 */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Loader : public Object
  {
  private:
    Table* m_pTable;   // table being load
    File   m_xFile;    // from which file
    char*  m_pRecord;  // record from file
    ScanBuffer* m_pScanBuffer; // scanning buffer
    
  public:
    void* operator new(size_t);
    void  operator delete(void* pLoaderAddress);
 
    Loader(void);
    ~Loader(void);
    
    Table* pTable(void);
    void   vTable(Table* pTable);
    
    void   vPath(s& sPath);
    
    bool   bOpenFile(void);
    bool   bFetchRecord(void);
    void   vInsertRow(void);
    void   vCloseFile(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
