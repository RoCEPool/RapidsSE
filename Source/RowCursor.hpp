/*============================================================================*/
/* RowCursor.hpp - row cursor                                                 */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class RowCursor : public Object
  {
  private:
    Table*     m_pTable;
    Partition* m_pPartition;
    RowStrand* m_pRowStrand;
    RowBuffer* m_pRowBuffer;
    Object*    m_pRow;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* pRowCursorAddress);
    void  operator delete(void*);
    
    RowCursor(void);
    ~RowCursor(void);
    
    void    vBegin(void);
    void    vEnd(void);
    void    vFirst(void);
    void    vNext(void);
    Object* pRow(void);
    char*   pRowAsStr(void);
    Table*  pTable(void);
    void    vTable(Table* pTable);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/