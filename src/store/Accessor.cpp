/*============================================================================*/
/* Accessor.cpp --                                                            */
/*============================================================================*/

#include "Accessor.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Accessor::Accessor(eCN eClassNo, Bench* pBench)
: TransformerTool(eClassNo, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Accessor::~Accessor(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* access table                                                               */
/*----------------------------------------------------------------------------*/
bool Accessor::A_bTable(Stz* szTableNm)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pTable = AM::pTableFind(szTableNm);

  if (m_pTable)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* access index of table                                                      */
/*----------------------------------------------------------------------------*/
bool Accessor::A_bIndex(Stz* szIndexNm)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pIndex = m_pTable->pFindIndex(szIndexNm);

  if (m_pIndex)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* access index of table                                                      */
/*----------------------------------------------------------------------------*/
bool Accessor::A_bIndex(Stz* szTableNm, Stz* szIndexNm)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (!A_bTable(szTableNm))
    return false;
  if (!A_bIndex(szIndexNm))
    return false;
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* access stream defn of table                                                */
/*----------------------------------------------------------------------------*/
void Accessor::A_vStreamOfTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (!m_pTable)
    return;

  m_pStreamDef = m_pTable->pStreamDef();

  M.p_stream = new Card_Stream;
  M.p_streamColList = new Card_StreamColList;

  i8 nCols = m_pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = m_pStreamDef->pGetColumn(i_Col);

    M.p_streamCol = new Card_StreamCol();
    M.p_streamCol->vValue(pColumn->szColNm());
    M.p_streamColList->vAppend(M.p_streamCol);

    M.p_stmColNm = new Card_StmColNm();
    M.p_streamCol->vAppend(M.p_stmColNm);

    Card_Type *pCard_Type = new Card_Type(pColumn);
    M.p_streamCol->vAppend(pCard_Type);
  }

}

/*----------------------------------------------------------------------------*/
/* access partition of table                                                  */
/*----------------------------------------------------------------------------*/
void Accessor::A_vPartitionOfTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

}

/*----------------------------------------------------------------------------*/
/* access partition of table                                                  */
/*----------------------------------------------------------------------------*/
void Accessor::A_vSizeOfTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
