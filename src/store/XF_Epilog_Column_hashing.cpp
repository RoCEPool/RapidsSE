/*============================================================================*/
/* XF_Epilog_Column_hashing.cpp -- Column Mapping                                    */
/*============================================================================*/

#include "XF_Epilog_Column_hashing.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Column_hashing, CN_XF_COLUMN_HASHING)
__aNewFunction(XF_Epilog_Column_hashing, CN_XF_COLUMN_HASHING)
__vDeleteFunction(XF_Epilog_Column_hashing, CN_XF_COLUMN_HASHING)
__aFooterFunction(XF_Epilog_Column_hashing)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Column_hashing::XF_Epilog_Column_hashing(CardIter* pCI)
: XF_EPILOG(CN_XF_COLUMN_HASHING, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_HASHING, _FOOTER_);

  _checkInitialised_(CN_XF_COLUMN_HASHING);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_HASHING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Column_hashing::~XF_Epilog_Column_hashing(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_HASHING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Column_hashing::vEpilog(CardIter* pCI)
{
  _static_

  XF_Epilog_Column_hashing* pXF_Column_Hashing = new XF_Epilog_Column_hashing(pCI);

  pXF_Column_Hashing->vXF_Column_Hashing();

  delete pXF_Column_Hashing;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* column mapping                                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Column_hashing::vXF_Column_Hashing(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_COLUMN_HASHING, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_COLUMN_HASHING, "column hashing"_Stz);

  while (m_pCI->G_bEachMain_NONE()) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (CK_STAGE == m_pCI->G.e_MainCard) {
      m_pCI->vNullMembers();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (CK_PROJECT == m_pCI->G.e_MainCard) {
      m_pCI->G.p_top = nullptr;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (CK_TOP == m_pCI->G.e_MainCard) {
      m_pCI->G.p_project = nullptr;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (CK_HASH == m_pCI->G.e_MainCard) {
      m_pCI->G_vAppendNewChild(m_pCI->G.p_hash,CK_HASHING);

      if (m_pCI->G.p_project) {
        m_pCI->G.p_hshCol = m_pCI->G.p_hash->pSub_hshCol();
        m_pCI->G.p_term = m_pCI->G.p_hshCol->pSub_term();
        m_pCI->G.p_term = _pCard_Term(m_pCI->G.p_term->pDupCardRecursive());
        m_pCI->G.p_term->pSymbol()->Y_vSet(P_HashOf,"PROJECT"_Stz);
        m_pCI->G.p_hashing->vAppend(m_pCI->G.p_term);
      }
      else
      if (m_pCI->G.p_top) {
        m_pCI->G.p_hshCol = m_pCI->G.p_hash->pSub_hshCol();
        m_pCI->G.p_term = m_pCI->G.p_hshCol->pSub_term();
        m_pCI->G.p_term = _pCard_Term(m_pCI->G.p_term->pDupCardRecursive());
        m_pCI->G.p_term->pSymbol()->Y_vSet(P_HashOf,"TOP"_Stz);
        m_pCI->G.p_hashing->vAppend(m_pCI->G.p_term);
      }
      else {
        m_pCI->G.p_hshCol = m_pCI->G.p_hash->pSub_hshCol();
        m_pCI->G.p_term = m_pCI->G.p_hshCol->pSub_term();
        m_pCI->G.p_term = _pCard_Term(m_pCI->G.p_term->pDupCardRecursive());
        m_pCI->G.p_term->pSymbol()->Y_vSet(P_HashOf,"other"_Stz);
        m_pCI->G.p_hashing->vAppend(m_pCI->G.p_term);
      }
      continue;
    }

  }

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
