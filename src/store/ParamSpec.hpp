/*============================================================================*/
/* ParamSpec.hpp -- Parameter Specification                                   */
/*============================================================================*/
#pragma once

namespace STORE
{
  class ParamSpec : public Object
                  , public ListItem_t<ParamSpec>
  {
  private:
    s32  m_sParamNm;  // parameter name


    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ParamSpec(Stz* szParamNm);
   ~ParamSpec(void) noexcept(false);

    Stz* szParamNm(void);
    i8   iParamNmBufSize(void);

    Footer* aFooter(void);
  };

  ParamSpec* _pParamSpec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
