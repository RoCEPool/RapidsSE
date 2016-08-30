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
    i8     m_iLength;  // length of record
    
    ScanBuffer* m_pScanBuffer; // scanning buffer
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* pLoaderAddress);
    void  operator delete(void* pLoaderAddress);
 
    Loader(void);
    ~Loader(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    Table* pTable(void);
    void   vTable(Table* pTable);
    
    void   vPath(s& sPath);
    
    bool   bOpenFile(void);
    void   vBegin(void);
    bool   bFetchRecord(void);
    void   vGetRecord(StrVal* pStrVal);
    bool   bInsertRow(i8 iPartn);
    void   vEnd(void);
    void   vCloseFile(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
