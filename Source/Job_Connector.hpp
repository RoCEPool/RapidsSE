/*============================================================================*/
/* Job_Connector.hpp -- Socket/Node Cross Connector Job                       */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Connector : public Job
  {
  private:

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Connector(Interface::SEitf* pSEitf);
    ~Job_Connector(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:

    void vShow(void);
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Connector* _pJob_Connector(void* aAddr);
  
  Job* pJob_Connector_new(Job* pArg);
  void vJob_Connector_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_Connector_show(Job* pJob);
  
  pNEXTJOBFUN* pJob_Connector_next(void** aaArg);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
