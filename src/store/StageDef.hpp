/*============================================================================*/
/* StageDef.hpp -- Stage Metadata                                            */
/*============================================================================*/
#pragma once

#include "OpDef.hpp"
#include "ListExo2_t.hpp"
#include "Card_Stage.hpp"
#include "ExtendedText.hpp"

namespace STORE
{
  class StageDef : public Object
                 , public ListExo2_t<OpDef>
  {
  private:
    i8   m_i_Stage;
    s32  m_sStageNm;
    ExtendedText* m_pCode;

    Card_Stage* m_pCard_Stage;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    StageDef(void);
    StageDef(i8 i_Stage, Stz* szStageNm, Card_Stage* p_stage);
    ~StageDef(void) noexcept(false);

    Card_Stage* pCard_Stage(void);
    void vCard_Stage(Card_Stage* pCard_Stage);

    Stz* szCode(void);
    void vCode(Stz* szCode);

    ExtendedText* pCode(void);
    void vCode(ExtendedText* pCode);

    void vCollapseOps(void);

    void vDump(i8 i_Tab=1);

    i8 i_Stage(void);
    void vStage(i8 i_Stage);

    Stz* szStageNm(void);
    void vStageNm(Stz* szStageNm);

    void vValidate(void);

  protected:
    Footer* aFooter(void);
  };

  StageDef* _pStageDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
