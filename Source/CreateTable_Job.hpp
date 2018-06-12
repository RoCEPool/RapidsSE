/*============================================================================*/
/* CreateTable_Job.hpp -- Create Table Job                                    */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  static const i8 C_MAX_COLUMNS = 128;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class CreateTable_Job : public Job
  {
    s32    m_sTableName;   // table name
    eTT    m_eTableType;   // table type
  
    struct {
      
      i8    m_iColumnNo;     // column no )within this table)
      s32   m_sColumnName;   // name of column
      eCT   m_eColumnType;   // column type
      i8    m_iColumnLength; // length of column
      i8    m_iColumnScale;  // column scale
      eDM   m_eDatumType;    // SE datum type
      bool  m_bIsNullable;   // is allowed to be null
      bool  m_bIsPartitionKey; // is a partition key?
      
    } m_xColumns[C_MAX_COLUMNS];

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    CreateTable_Job(void);
    ~CreateTable_Job(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  CreateTable_Job* _pCreateTable_Job(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
