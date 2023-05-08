/*============================================================================*/
/* StageDef.hpp -- Stage Metadata                                            */
/*============================================================================*/

#include "Meta.hpp"

namespace STORE
{
  class StageDef : public Meta
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    StageDef(void);

    ~StageDef(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  StageDef* _pStageDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
