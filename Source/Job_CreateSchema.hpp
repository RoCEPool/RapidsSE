/*============================================================================*/
/* Job_CreateSchema.hpp -- Create Schema Job                                  */
/*============================================================================*/
#pragma once

namespace JobsModel
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_CreateSchema : public Job
  {
  public:
    s32    m_sSchemaName;           // schema name

    i8   m_iDicDepth;               // depth of dictionary path
    s256 m_sDicPath[C_MAX_FOLDERS]; // dictionary path
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_CreateSchema(void);
    ~Job_CreateSchema(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  Job_CreateSchema* _pJob_CreateSchema(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
