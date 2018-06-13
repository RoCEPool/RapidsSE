/*============================================================================*/
/* Job_DropSchema.hpp -- Drop Schema Job                                      */
/*============================================================================*/
#pragma once

namespace JobsModel
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_DropSchema : public Job
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
  
    Job_DropSchema(void);
    ~Job_DropSchema(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  Job_DropSchema* _pJob_DropSchema(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
