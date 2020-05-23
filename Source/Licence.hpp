/*============================================================================*/
/* Licence.hpp - software licence                                             */
/*============================================================================*/
#pragma once

namespace Arguments
{
  class Licence : public Object
  {
  private:
    
    s256 m_sDescription;   // description
  
    Dte  m_wCreationDate;  // date licence was created
    Dte  m_wExpiryDate;    // date licence will expire

    i8   m_nMaxMemory;     // maximum amount of memory
    i8   m_nMaxNodes;      // maximum number of nodes
    i8   m_nMaxPartitions; // maximum number of partitions per node
    i8   m_nMaxSessions;   // maximum number of sessions
    i8   m_nMaxThreads;    // maximum number of threads

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Licence(void);
   ~Licence(void) noexcept(false);
    
    Footer* aFooter(void);
  };

  Licence* _pLicence(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
