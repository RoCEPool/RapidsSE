/*============================================================================*/
/* Meta.hpp -- Abstract Metadata                                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eCK.h"

namespace STORE
{
  class Meta : public Object
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Meta(eCN eMeta);

   ~Meta(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  Meta* _pMeta(void* aAddr);
}

namespace STORE
{
  class MetaPipeline : public Meta
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    MetaPipeline(void);

    ~MetaPipeline(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  MetaPipeline* _pMetaPipeline(void* aAddr);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
