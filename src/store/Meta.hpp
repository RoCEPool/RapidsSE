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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* MetaPipeline.hpp -- Pipeline Metadata                                      */
/*============================================================================*/

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

/*============================================================================*/
/* MetaStage.hpp -- Stage Metadata                                            */
/*============================================================================*/

namespace STORE
{
  class MetaStage : public Meta
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    MetaStage(void);

    ~MetaStage(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  MetaStage* _pMetaStage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* MetaOp.hpp -- Op Metadata                                                  */
/*============================================================================*/

namespace STORE
{
  class MetaOp : public Meta
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    MetaOp(void);

    ~MetaOp(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  MetaOp* _pMetaOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* MetaArg.hpp -- Arg Metadata                                                */
/*============================================================================*/

namespace STORE
{
  class MetaArg : public Meta
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    MetaArg(void);

    ~MetaArg(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  MetaArg* _pMetaArg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* MetaColsReq.hpp -- Columns Required Metadata                               */
/*============================================================================*/

namespace STORE
{
  class MetaColsReq : public Meta
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    MetaColsReq(void);

    ~MetaColsReq(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  MetaColsReq* _pMetaColsReq(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* MetaExprStk.hpp -- Expression Stack Metadata                               */
/*============================================================================*/

namespace STORE
{
  class MetaExprStk : public Meta
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    MetaExprStk(void);

    ~MetaExprStk(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  MetaExprStk* _pMetaExprStk(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

