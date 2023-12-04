/*============================================================================*/
/* XF_Epilog_Hash_Columns.cpp -- Hash Columns                                 */
/*============================================================================*/

#include "XF_Epilog_Hash_Columns.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Epilog_Hash_Columns, CN_XF_EPILOG_HASH_COLUMNS)
__aNewFunction(XF_Epilog_Hash_Columns, CN_XF_EPILOG_HASH_COLUMNS)
__vDeleteFunction(XF_Epilog_Hash_Columns, CN_XF_EPILOG_HASH_COLUMNS)
__aFooterFunction(XF_Epilog_Hash_Columns)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Epilog_Hash_Columns::XF_Epilog_Hash_Columns(CardIter* pCI)
: XF_EPILOG(CN_XF_EPILOG_HASH_COLUMNS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_HASH_COLUMNS, _FOOTER_);

  _checkInitialised_(CN_XF_EPILOG_HASH_COLUMNS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_HASH_COLUMNS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Epilog_Hash_Columns::~XF_Epilog_Hash_Columns(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_HASH_COLUMNS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Hash_Columns::vHash(CardIter* pCI)
{
  _static_

  XF_Epilog_Hash_Columns* pXF_Column_Hashing = new XF_Epilog_Hash_Columns(pCI);

  pXF_Column_Hashing->vHash_Columns();

  delete pXF_Column_Hashing;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* column mapping                                                             */
/*----------------------------------------------------------------------------*/
void XF_Epilog_Hash_Columns::vHash_Columns(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_HASH_COLUMNS, _FOOTER_);

  m_pCI->vTransform(__func__, XF_EPILOG_HASH_COLUMNS, "column hashing"_Stz);

  m_pCI->vBeginEach();
  while (m_pCI->G_bEachMain_NONE()) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (CK_c_STAGE == m_pCI->G.e_MainCard) {
      m_pCI->vNullMembers();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (CK_m_PROJECT == m_pCI->G.e_MainCard) {
      m_pCI->G.p_top = nullptr;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (CK_m_TOP == m_pCI->G.e_MainCard) {
      m_pCI->G.p_project = nullptr;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (CK_m_HASH == m_pCI->G.e_MainCard) {
      m_pCI->G_vAppendNewChild(m_pCI->G.p_hash, CK_s_HASHING);

      Kid xHshColList(m_pCI->G.p_hash->pSub_hshColList());
      while (xHshColList.bEach()) {
        m_pCI->G.p_hshCol = xHshColList.p_hshCol();
        if (m_pCI->G.p_project) {
          m_pCI->G.p_term = m_pCI->G.p_hshCol->pSub_term();
          m_pCI->G.p_term = _pCard_Term(m_pCI->G.p_term->pDupCardRecursive());
          m_pCI->G.p_term->pSymbol()->Y_vSet(P_HashOf,"PROJECT"_Stz);
          m_pCI->G.p_hashing->vAppendChild(m_pCI->G.p_term);
        }
        else
        if (m_pCI->G.p_top) {
          m_pCI->G.p_term = m_pCI->G.p_hshCol->pSub_term();
          m_pCI->G.p_term = _pCard_Term(m_pCI->G.p_term->pDupCardRecursive());
          m_pCI->G.p_term->pSymbol()->Y_vSet(P_HashOf,"TOP"_Stz);
          m_pCI->G.p_hashing->vAppendChild(m_pCI->G.p_term);
        }
        else {
          m_pCI->G.p_term = m_pCI->G.p_hshCol->pSub_term();
          m_pCI->G.p_term = _pCard_Term(m_pCI->G.p_term->pDupCardRecursive());
          m_pCI->G.p_term->pSymbol()->Y_vSet(P_HashOf,"other"_Stz);
          m_pCI->G.p_hashing->vAppendChild(m_pCI->G.p_term);
        }

      }

//      if (m_pCI->G.p_project) {
//        m_pCI->G.p_hshCol = m_pCI->G.p_hash->pSub_hshCol();
//        m_pCI->G.p_term = m_pCI->G.p_hshCol->pSub_term();
//        m_pCI->G.p_term = _pCard_Term(m_pCI->G.p_term->pDupCardRecursive());
//        m_pCI->G.p_term->pSymbol()->Y_vSet(P_HashOf,"PROJECT"_Stz);
//        m_pCI->G.p_hashing->vAppendChild(m_pCI->G.p_term);
//      }
//      else
//      if (m_pCI->G.p_top) {
//        m_pCI->G.p_hshCol = m_pCI->G.p_hash->pSub_hshCol();
//        m_pCI->G.p_term = m_pCI->G.p_hshCol->pSub_term();
//        m_pCI->G.p_term = _pCard_Term(m_pCI->G.p_term->pDupCardRecursive());
//        m_pCI->G.p_term->pSymbol()->Y_vSet(P_HashOf,"TOP"_Stz);
//        m_pCI->G.p_hashing->vAppendChild(m_pCI->G.p_term);
//      }
//      else {
//        m_pCI->G.p_hshCol = m_pCI->G.p_hash->pSub_hshCol();
//        m_pCI->G.p_term = m_pCI->G.p_hshCol->pSub_term();
//        m_pCI->G.p_term = _pCard_Term(m_pCI->G.p_term->pDupCardRecursive());
//        m_pCI->G.p_term->pSymbol()->Y_vSet(P_HashOf,"other"_Stz);
//        m_pCI->G.p_hashing->vAppendChild(m_pCI->G.p_term);
//      }
      continue;
    }

  }

  m_pCI->vEndTransform(__LINE__);
}
/*----------------------------------------------------------------------------*/
//void XF_Epilog_Hash_Columns::vHash_Columns(void)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_EPILOG_HASH_COLUMNS, _FOOTER_);
//
//  m_pCI->vTransform(__func__, XF_EPILOG_HASH_COLUMNS, "column hashing"_Stz);
//
//  m_pCI->vBeginEach();
//  while (m_pCI->G_bEachMain_NONE()) {
//
//    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    if (CK_c_STAGE == m_pCI->G.e_MainCard) {
//      m_pCI->vNullMembers();
//      continue;
//    }
//
//    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    if (CK_m_PROJECT == m_pCI->G.e_MainCard) {
//      m_pCI->G.p_top = nullptr;
//      continue;
//    }
//
//    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    if (CK_m_TOP == m_pCI->G.e_MainCard) {
//      m_pCI->G.p_project = nullptr;
//      continue;
//    }
//
//    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    if (CK_m_HASH == m_pCI->G.e_MainCard) {
//      m_pCI->G_vAppendNewChild(m_pCI->G.p_hash,CK_s_HASHING);
//
//      if (m_pCI->G.p_project) {
//        m_pCI->G.p_hshCol = m_pCI->G.p_hash->pSub_hshCol();
//        m_pCI->G.p_term = m_pCI->G.p_hshCol->pSub_term();
//        m_pCI->G.p_term = _pCard_Term(m_pCI->G.p_term->pDupCardRecursive());
//        m_pCI->G.p_term->pSymbol()->Y_vSet(P_HashOf,"PROJECT"_Stz);
//        m_pCI->G.p_hashing->vAppendChild(m_pCI->G.p_term);
//      }
//      else
//      if (m_pCI->G.p_top) {
//        m_pCI->G.p_hshCol = m_pCI->G.p_hash->pSub_hshCol();
//        m_pCI->G.p_term = m_pCI->G.p_hshCol->pSub_term();
//        m_pCI->G.p_term = _pCard_Term(m_pCI->G.p_term->pDupCardRecursive());
//        m_pCI->G.p_term->pSymbol()->Y_vSet(P_HashOf,"TOP"_Stz);
//        m_pCI->G.p_hashing->vAppendChild(m_pCI->G.p_term);
//      }
//      else {
//        m_pCI->G.p_hshCol = m_pCI->G.p_hash->pSub_hshCol();
//        m_pCI->G.p_term = m_pCI->G.p_hshCol->pSub_term();
//        m_pCI->G.p_term = _pCard_Term(m_pCI->G.p_term->pDupCardRecursive());
//        m_pCI->G.p_term->pSymbol()->Y_vSet(P_HashOf,"other"_Stz);
//        m_pCI->G.p_hashing->vAppendChild(m_pCI->G.p_term);
//      }
//      continue;
//    }
//
//  }
//
//  m_pCI->vEndTransform(__LINE__);
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
