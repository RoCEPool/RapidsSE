/*============================================================================*/
/* StreamDef.cpp -- Stream Definition                                         */
/*============================================================================*/

#include "MetaData.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StreamDef* MetaData::_pStreamDef(void* aAddr)
{
  StreamDef* pStreamDef = reinterpret_cast<StreamDef*>(aAddr);
  
  RASSERT_TYPE(pStreamDef,CN_STREAMDEF);
  
  return pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StreamDef::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_STREAMDEF);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void StreamDef::operator delete(void* aAddr)
{
  StreamDef* pStreamDef = reinterpret_cast<StreamDef*>(aAddr);
  
  HeapMem::vFree(pStreamDef);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StreamDef::StreamDef(void)
: Definition(CN_STREAMDEF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  //m_sName;
  //m_sPath;
  
  m_iStreamId = AM::iNextStreamId();
  m_nDefSets = 0;
  
  //m_saDefName[C_MAX_STREAMDEFS];
  
  for (i8 i_StreamDefs = 0; i_StreamDefs < C_MAX_STREAMDEFS; i_StreamDefs++) {
    m_i_DefStart[i_StreamDefs] = C_UNKNOWN;
    m_nDefColumns[i_StreamDefs] = C_UNKNOWN;
  }
  //m_xaColumn[C_MAX_DEFCOLUMNS];
  
  m_bTemp = false;
  //vTrace("StreamDef permanent %p\n"_Stz, this);
  
  m_nFixedSized = 0;
  
  _checkInitialised_(CN_STREAMDEF);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StreamDef::StreamDef(StreamDef* pStreamDef)
: Definition(CN_STREAMDEF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  m_sName = pStreamDef->m_sName;
  m_sPath = pStreamDef->m_sPath;
  
  m_iStreamId = AM::iNextStreamId();
  m_nDefSets = pStreamDef->m_nDefSets;
  
  for (i8 i_StreamDef = 0; i_StreamDef < m_nDefSets; i_StreamDef++) {
    m_nDefColumns[i_StreamDef] = pStreamDef->m_nDefColumns[i_StreamDef];
    m_i_DefStart[i_StreamDef] = pStreamDef->m_i_DefStart[i_StreamDef];
    
    for (i8 i_DefCol = 0; i_DefCol < m_nDefColumns[i_StreamDef]; i_DefCol++) {
      m_xaColumn[i_DefCol].vCopy(&pStreamDef->m_xaColumn[i_DefCol]);
    }
    
  }
  
  m_bTemp = true;
  
  //_checkInitialised_(CN_STREAMDEF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StreamDef::~StreamDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* add column vec from stream def                                             */
/*----------------------------------------------------------------------------*/
void StreamDef::vAdd(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  m_nDefSets++;
  
  for (i8 i_StreamDef = 0; i_StreamDef < pStreamDef->m_nDefSets; i_StreamDef++) {
    m_nDefColumns[i_StreamDef] = m_nDefColumns[i_StreamDef];
    m_i_DefStart[i_StreamDef] = pStreamDef->m_i_DefStart[i_StreamDef];
    
    for (i8 i_DefCol = 0; i_DefCol < m_nDefColumns[i_StreamDef]; i_DefCol++) {
      m_xaColumn[i_DefCol].vCopy(&pStreamDef->m_xaColumn[i_DefCol]);
    }
    
  }
  
  _checkInitialised_(CN_STREAMDEF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* add a column to the stream def to the current def set                      */
/*----------------------------------------------------------------------------*/
void StreamDef::vAddColumn(Stz* sColumnNm, eCT eColumnType)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  m_i_DefStart[m_nDefSets] = 0;
  
  //! ALKA come back to this
  m_nDefColumns[m_nDefSets]++;
  i8 i_DefCol = m_nDefColumns[m_nDefSets];
 
  m_xaColumn[i_DefCol].vColumnNo(m_nDefColumns[i_DefCol]);
  m_xaColumn[i_DefCol].vColumnNm(sColumnNm);
  m_xaColumn[i_DefCol].vColumnType(eColumnType);
  
  m_nDefColumns[m_nDefSets]++;
}

/*----------------------------------------------------------------------------*/
/* convert streamDef to a string                                              */
/*----------------------------------------------------------------------------*/
void StreamDef::vAsString(String* pString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  pString->vSprint("streamDef %s: ("_Stz,m_sName.szStz());
  
  for (i8 i_DefSet = 0; i_DefSet < m_nDefSets; i_DefSet++) {
    
    if (i_DefSet != 0)
      pString->vConcat(","_Stz);
    
    for (i8 i_DefColumn = 0; i_DefColumn < m_nDefColumns[i_DefSet]; i_DefColumn++) {
        
      if (i_DefColumn != 0)
        pString->vConcat(","_Stz);
      
      pString->vCatSprint("%s %s"_Stz,
                          m_xaColumn[i_DefColumn].szColNm(),
                          aCT[m_xaColumn[i_DefColumn].eColumnType()]);
    }
  }
  pString->vConcat(")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get no of columns in subset of streamDef                                   */
/*----------------------------------------------------------------------------*/
i8 StreamDef::nColumns(i8 iSubSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  i8 nColumns = 0;
  
  if (C_ALL == iSubSet) {
    for (i8 i_StreamDef = 0; i_StreamDef < m_nDefSets; i_StreamDef++) {
      nColumns +=  m_nDefColumns[i_StreamDef];
    }
  }
  else {
    nColumns +=  m_nDefColumns[iSubSet];
  }
  return nColumns;
  
}


/*----------------------------------------------------------------------------*/
/* get column vector at index                                                 */
/*----------------------------------------------------------------------------*/
i8 StreamDef::iFindColumn(char* sColumnNm, i8& i_ColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);

// ALKA_TO_REVIEW
//  for (i_ColVec = 0; i_ColVec < m_oColVecAry.nItems(); i_ColVec++) {
//    ColumnVec* pColumnVec = m_oColVecAry.pGet(i_ColVec);
//    i8 nColumns = pColumnVec->nItems();
//    for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
//      Column* pCol = pColumnVec->pGet(i_Col);
//      if (StrLib::bEqualTo(sColumnNm,pCol->sColumnName().azChar(),C_NO_CASE)) {
//        return i_Col;
//      }
//    }
//  }
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* get column at index                                                        */
/*----------------------------------------------------------------------------*/
Column* StreamDef::pGetColumn(i8 iColumn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  return &m_xaColumn[iColumn];
}

/*----------------------------------------------------------------------------*/
/* get stream name                                                            */
/*----------------------------------------------------------------------------*/
Stz* StreamDef::szName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* get column at index                                                        */
/*----------------------------------------------------------------------------*/
void StreamDef::vName(Stz* sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  m_sName = sName;
}

/*----------------------------------------------------------------------------*/
/* convert streamDef to colPix format                                         */
/*----------------------------------------------------------------------------*/
void StreamDef::vToColPix(ColPix* axaColPix)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  for (i8 i_DefSet = 0; i_DefSet < m_nDefSets; i_DefSet++) {
        
    for (i8 i_DefColumn = 0; i_DefColumn < m_nDefColumns[i_DefSet]; i_DefColumn++) {
      
      Column* pColumn  = &m_xaColumn[i_DefColumn];
      ColPix* axColPix = &axaColPix[i_DefColumn];
      
      axColPix->m_uType     = pColumn->eColumnType();
      axColPix->m_uLength   = pColumn->iColumnLength();
      axColPix->m_uScale    = pColumn->iColumnScale();
      axColPix->m_uNullable = pColumn->bIsNullable();

    }
  }
}

/*----------------------------------------------------------------------------*/
/* convert streamDef to its metadat format                                    */
/*----------------------------------------------------------------------------*/
void StreamDef::vToMetadata(Stz* sMetadata)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMDEF,_FOOTER_);
  
  sMetadata->vCopy("metadata: ("_Stz);
  
  for (i8 i_DefSet = 0; i_DefSet < m_nDefSets; i_DefSet++) {
    
    if (i_DefSet != 0)
      sMetadata->vConcat(","_Stz);
    
    for (i8 i_DefColumn = 0; i_DefColumn < m_nDefColumns[i_DefSet]; i_DefColumn++) {
        
      if (i_DefColumn != 0)
        sMetadata->vConcat(","_Stz);
      
      sMetadata->vCatSprint("%s %s"_Stz, m_xaColumn[i_DefColumn].szColNm(),
                            aCT[m_xaColumn[i_DefColumn].eColumnType()]);
    }
  }
  sMetadata->vConcat(")"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(StreamDef);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
