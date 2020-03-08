/*============================================================================*/
/* InfoSet.cpp -- Information Set                                             */
/*============================================================================*/

#include "InfoData.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
InfoSet* InfoData::_pInfoSet(void* aAddr)
{
  InfoSet* pInfoSet = reinterpret_cast<InfoSet*>(aAddr);
  
  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* InfoSet::operator new(size_t)
{
  InfoSet* pInfoSet =
  static_cast<InfoSet*>(HeapMem::pAllocate(CN_INFOSET));
  
  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void InfoSet::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InfoSet::InfoSet(void)
: Operand(CN_INFOSET)
{
  _precondition_(_MAGIC_,CN_INFOSET);
  
  m_nInfoRows = 0;
  m_nInfoValues = 0;
  
  for (i8 i_Set = 0; i_Set < C_INFO_VALUES; i_Set++) {
   m_xInfo[i_Set].m_i_Value = C_UNKNOWN;
  }
  
  _checkInitialised_(CN_INFOSET);
  
  _postcondition_(_MAGIC_,CN_INFOSET,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
InfoSet::~InfoSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
}

/*----------------------------------------------------------------------------*/
/* add                                                                        */
/*----------------------------------------------------------------------------*/
void InfoSet::vAddValue(i8 iValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  m_xInfo[m_nInfoValues++].m_i_Value = iValue;
}

/*----------------------------------------------------------------------------*/
/* add                                                                        */
/*----------------------------------------------------------------------------*/
void InfoSet::vAddValue(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  m_xInfo[m_nInfoValues++].m_i_Name = m_yxStrings.iAppend(szName);
}

/*----------------------------------------------------------------------------*/
/* inc rows in the set                                                        */
/*----------------------------------------------------------------------------*/
bool InfoSet::bIncRows(i8 nValues)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  if (m_nInfoValues + nValues < C_INFO_VALUES) {
    m_nInfoRows++;
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get no of infoRows                                                         */
/*----------------------------------------------------------------------------*/
i8 InfoSet::nInfoRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  return m_nInfoRows;
}

/*----------------------------------------------------------------------------*/
/* print the values as per the stream def                                     */
/*----------------------------------------------------------------------------*/
void InfoSet::vPrint(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  // streamdef columns should match the rows/values
  ASSERT(m_nInfoRows / m_nInfoValues == pStreamDef->nColumns(0));
  
  // for each infoset
  for (i8 i_InfoRow = 0; i_InfoRow < m_nInfoRows; i_InfoRow++) {
    
    // for each column
    for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(0); i_Col++) {
      
      Column* pColumn = pStreamDef->pGetColumn(i_Col);
      i8 i_Value = i_InfoRow + i_Col;
      
      if (pColumn->eColumnType() == CT_INTEGER)
        vTrace("%s %slld\n"_Stz, pColumn->szColNm(),m_xInfo[i_Value].m_i_Name);
      else {
        vTrace("%s %s\n"_Stz, pColumn->szColNm(),m_yxStrings.szGet(m_xInfo[i_Value].m_i_Name));
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get no of values                                                           */
/*----------------------------------------------------------------------------*/
i8 InfoSet::nValues(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  return m_nInfoValues;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
