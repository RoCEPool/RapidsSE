/*============================================================================*/
/* CarryForward.hpp -- Carry Forward                                          */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "Board.hpp"


namespace STORE
{
  class CarryForward : public Object
  {
  private:
    Card_Expr* m_p_sqCrlKey;
    bool       m_bCrlKeyActive;

    Card_Expr* m_p_sqResult;
    bool       m_bResultActive;

  public:
    CardIter*  m_pCF;

    struct {
      bool m_bReproject;
    } Table;

    struct {
      bool m_bReproject;
    } Where_preAgg;

    struct {
      bool m_bReproject;
    } Aggreg;

    struct {
      bool m_bReproject;
    } GbyAgg;

    struct {
      bool m_bReproject;
    } Project_postAgg;

    struct {
      bool m_bReproject;
    } Having;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    CarryForward(void);
   ~CarryForward(void) noexcept(false);

    bool bCrlKeyActive(void);
    void vCrlKeyActive(bool bCrlKeyActive);

    bool bIsCorrelated(void);

    bool bResultActive(void);
    void vResultActive(bool bResultActive);

    Card_Expr* p_sqCrlKey(void);
    void v_sqCrlKey(Card_Expr* p_sqCrlKey);

    Card_Expr* p_sqResult(void);
    void v_sqResult(Card_Expr* p_sqResult);

    bool bWorkToDo(void);

  protected:
    Footer* aFooter(void);
  };

  CarryForward* _pCarryForward(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
