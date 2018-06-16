/*============================================================================*/
/* Job_CreateTable.hpp -- Create Table Job                                    */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_CreateTable : public Job
  {
    s32    m_sTableName;   // table name
    eTT    m_eTableType;   // table type
    i8     m_iColumns;     // no of columns
    
    struct {
      
      i8    m_iColumnNo;     // column no )within this table)
      s32   m_sColumnName;   // name of column
      eCT   m_eColumnType;   // column type
      i8    m_iColumnLength; // length of column
      i8    m_iColumnScale;  // column scale
      bool  m_bIsNullable;   // is allowed to be null
      bool  m_bIsPartitionKey; // is a partition key?
      
      eDM   m_eDatumType;    // SE datum type
      
    } m_xColumns[C_MAX_COLUMNS];

    i8   m_iDicDepth;               // depth of dictionary path
    s256 m_sDicPath[C_MAX_FOLDERS]; // dictionary path
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_CreateTable(void);
    ~Job_CreateTable(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_CreateTable* _pJob_CreateTable(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
