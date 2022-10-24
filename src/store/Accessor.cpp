/*============================================================================*/
/* Accessor.cpp --                                                            */
/*============================================================================*/

#include "Accessor.hpp"

#include "PtnDef.hpp"

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
bool Accessor::A_bIndexListOfTable(void)
{
  _precondition_(_ADDRESS_, _MAGIC_);

  M.p_indexList = nullptr;

  if (!m_pTable)
    return false;

  IdxVec* pIdxVec = m_pTable->pIdxVec();
  if (! pIdxVec)
    return false;

  M.p_indexList = new Card_IndexList;

  i8 nIndexes = pIdxVec->nItems();
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    Index* pIndex = pIdxVec->pGet(i_Index);

    M.p_index = new Card_Index;
    M.p_indexList->vAppend(M.p_index);

    M.p_indexNm = new Card_IndexNm;
    M.p_indexNm->vValue(pIndex->szIndexNm());
    M.p_index->vAppend(M.p_indexNm);

    M.p_idxColList = new Card_IdxColList;
    M.p_index->vAppend(M.p_idxColList);

    m_pIndexDef = pIndex->pIndexDef();

    i8 nColumns = m_pIndexDef->nColumns();
    for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

      m_pColumn = m_pIndexDef->pGetColumn(i_Col);
      M.p_idxCol = new Card_IdxCol;
      M.p_idxCol->vValue(m_pColumn->szColNm());

      M.p_idxColList->vAppend(M.p_idxCol);

      M.p_idxColNm = new Card_IdxColNm;
      M.p_idxColNm->vValue(m_pColumn->szColNm());
      M.p_idxCol->vAppend(M.p_idxColNm);

    }

  }

  return true;
}

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
  M.p_stmColList = new Card_StmColList;
  M.p_stream->vAppend(M.p_stmColList);

  i8 nCols = m_pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = m_pStreamDef->pGetColumn(i_Col);

    M.p_stmCol = new Card_StmCol();
    M.p_stmCol->vValue(pColumn->szColNm());
    M.p_stmColList->vAppend(M.p_stmCol);

    M.p_stmColNm = new Card_StmColNm();
    M.p_stmColNm->vValue(pColumn->szColNm());
    M.p_stmCol->vAppend(M.p_stmColNm);

    Stz* szType = aCT[pColumn->eColumnType()];
    Card_Type *pCard_Type = new Card_Type(szType);
    M.p_stmCol->vAppend(pCard_Type);
  }

}

/*----------------------------------------------------------------------------*/
/* access partition of table                                                  */
/*----------------------------------------------------------------------------*/
void Accessor::A_vPartitionOfTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  char cTableSyn = M.p_table->cValue();

  PtnDef* pPtnDef = m_pTable->pPtnDef();
  Column* pPtnColumn = pPtnDef->pGetColumn(0);

  StreamDef* pStreamDef = m_pTable->pStreamDef();
  Column* pStmColumn = pStreamDef->pFindColumn(pPtnColumn->szColNm());
  i8 iStmColNo = pStmColumn->iColumnNo();
  s32 sTablePtn;
  sTablePtn.vSprint("$$%c.%lld"_Stz, cTableSyn, iStmColNo + 1);

  M.p_tablePtn = new Card_TablePtn;
  M.p_tablePtn->vValue(sTablePtn);
}

/*----------------------------------------------------------------------------*/
/* access size of table                                                       */
/*----------------------------------------------------------------------------*/
void Accessor::A_vSizeOfTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (! m_pTable) {
    vTrace("A: m_pTable not set\n"_Stz);
    return;
  }
  if (! M.p_table) {
    vTrace("A: M.p_table not set\n"_Stz);
    return;
  }

  i8 jTableSize = m_pTable->pStreamDef()->jTableSize();

  M.p_size = new Card_Size;
  s32 sSize(jTableSize);
  M.p_size->vValue(sSize);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
