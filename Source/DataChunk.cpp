/*============================================================================*/
/* DataChunk.cpp -- Data Chunk                                                */
/*============================================================================*/

#include "Chunks.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
DataChunk* Chunks::_pDataChunk(void* aAddr)
{
  DataChunk* pDataChunk = reinterpret_cast<DataChunk*>(aAddr);
    
  return pDataChunk;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* DataChunk::operator new(size_t)
{
  vFatal("fatal: JobObjectName class cannot be new'd\n"_Stz);

  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DataChunk::DataChunk(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);

  m_nDataBytes      = 0;
  m_aLastDataByte   = nullptr;
  m_nUnprocessedBytes = 0;
  
#ifdef __DEBUG_CHECKS
  m_xWsp.vFill(0xEF,eClassNo);
#endif
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DataChunk::~DataChunk(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy source data chunk into this data chunk                                */
/*----------------------------------------------------------------------------*/
void DataChunk::vCopy(DataChunk* pSource)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  MemLib::vCopy(m_xWsp.aArea(),
                pSource->aDataArea(),
                pSource->m_nDataBytes,
                eClass());
  
  vSetBytes(pSource->m_nDataBytes);
}

/*----------------------------------------------------------------------------*/
/* get data area of wsp                                                       */
/*----------------------------------------------------------------------------*/
char* DataChunk::aDataArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  char* aArea = m_xWsp.aArea();
  
  return aArea;
}

/*----------------------------------------------------------------------------*/
/* set members                                                                */
/*----------------------------------------------------------------------------*/
void DataChunk::vSetBytes(i8 nDataBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_nDataBytes = nDataBytes;
  m_nUnprocessedBytes = m_nDataBytes;

  m_aLastDataByte = &m_xWsp.aStz()[m_nDataBytes - 1];
}

/*----------------------------------------------------------------------------*/
/* compute the number of bytes which constitute complete records              */
/*----------------------------------------------------------------------------*/
i8 DataChunk::nQuantumBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 nBytes = 0;
  
  char* aLastByte = &(aDataArea()[m_nDataBytes - 1]);
  
  const char *aFinalEol = aLastByte;
  
  bool bFoundEol = StrLib::bBackward(&aFinalEol,aLastByte,'\n');
  if (!bFoundEol)
    bFoundEol = StrLib::bBackward(&aFinalEol,aLastByte,'\r');
  RASSERT(bFoundEol);
  
  nBytes = nSub((void*)aFinalEol,(void*)aDataArea()) + 1;
  
  return nBytes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
