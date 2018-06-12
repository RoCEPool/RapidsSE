/*============================================================================*/
/* Job.hpp -- a job in the system                                             */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  class Job : public Object
  {
  private:
    i8     m_iJobId; // job id
  
  public:
    Job(eCN eClassNo);
    ~Job(void);
    
    i8   iJobId(void);
    void vJobId(i8 iJobId);
    i8   iJobNode(void);
      
  };
  
  Job* _pJob(void* aAddr);
}

bool bJobCompare(void* pObjectInsert, void* pObjectScan, bool& bMatch);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
