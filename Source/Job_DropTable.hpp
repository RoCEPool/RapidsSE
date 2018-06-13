/*============================================================================*/
/* Job_DropTable.hpp -- Drop Table Job                                        */
/*============================================================================*/
#pragma once

namespace JobsModel
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_DropTable : public Job
  {
  public:
    s32    m_sTableName;           // table name

    i8   m_iDicDepth;               // depth of dictionary path
    s256 m_sDicPath[C_MAX_FOLDERS]; // dictionary path
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_DropTable(void);
    ~Job_DropTable(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  Job_DropTable* _pJob_DropTable(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
