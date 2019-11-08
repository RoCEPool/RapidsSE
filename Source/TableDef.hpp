/*============================================================================*/
/* TableDef.hpp -- Table Definition                                           */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class TableDef : public Object
  {
  friend class Table;
  
  private:
    s32 m_sTableNm;          // table name
    eTT m_eTableType;        // table type
  
    ColumnVec* m_pColVec;    // column vector
    StreamDef* m_pStreamDef; // definition
  
    s4K m_sCmd;              // the command which created this def

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TableDef(void);
   ~TableDef(void) noexcept(false);
  
    bool bParse(Stz* szCmd);
  
    Footer* aFooter(void);
  };
  
  TableDef* _pTableDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
