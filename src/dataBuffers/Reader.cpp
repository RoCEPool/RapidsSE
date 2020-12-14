/*============================================================================*/
/* Reader.cpp -- Assistant File Reader                                        */
/*============================================================================*/

#include "DataBuffers.h"

#include "File.hpp" //?

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(DataBuffers,Reader,CN_READER)
__aNewFunction(Reader,CN_READER)
__vDeleteFunction(Reader,CN_READER)
__aFooterFunction(Reader)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Reader::Reader(void)
: Object(CN_READER)
{
  _precondition_(_MAGIC_,CN_READER,_FOOTER_);
    
  m_i_Seek = 0;
  
  m_pFile = new File(nullptr);
  
  _checkInitialised_(CN_PERUSE);
  
  _postcondition_(_MAGIC_,CN_READER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Reader::~Reader(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_READER,_FOOTER_);
  
  if (m_pFile->bIsOpen())
    m_pFile->vClose();
  
  delete m_pFile;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void Reader::vCloseFile(void)
{
  _precondition_(_MAGIC_,CN_READER,_FOOTER_);

  m_pFile->vClose();
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool Reader::bOpenRead(Stz* szPath)
{
  _precondition_(_MAGIC_,CN_READER,_FOOTER_);

  // open the file
  bool bOpened = m_pFile->bOpenRead(szPath);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* read lines                                                                 */
/*----------------------------------------------------------------------------*/
i8 Reader::nReadLines(Stz* szInto, i8 nMaxBytes)
{
  _precondition_(_MAGIC_,CN_READER,_FOOTER_);

  m_pFile->vSeek(m_i_Seek);
  
  i8 nBytesRead = m_pFile->nReadBytes(szInto->azChar(),nMaxBytes);
  
  ((char*)szInto)[nBytesRead] = '\0';

  i8 nTrimmedTo = nBytesRead;
  
  char* acNewLineFind = (char*)&szInto[nBytesRead - 1];
  
  while (nTrimmedTo && '\n' != * acNewLineFind) {
    --acNewLineFind;
    --nTrimmedTo;
  }
    
  ((char*)szInto)[nTrimmedTo] = '\0';
  
  m_i_Seek += nTrimmedTo;
  
  return nTrimmedTo;
}


/*----------------------------------------------------------------------------*/
/* read lines into csv buffer                                                 */
/*----------------------------------------------------------------------------*/
CsvBuffer* Reader::pReadCsvBuffer(void)
{
  _precondition_(_MAGIC_,CN_READER,_FOOTER_);
  
  CsvBuffer* pCsvBuffer = new CsvBuffer;

  char* azInto = (char*)pCsvBuffer->ayDataArea();
  i8 nBytesRead = nReadLines((Stz*)azInto,C_1M);
  
  if (nBytesRead) {
    pCsvBuffer->vTake(nBytesRead + 1);
    
    //  vTrace("CsvBuffer data:\n"_Stz);
    //  vTrace("%s"_Stz,azInto);
    //  vTrace("End CsvBuffer\n"_Stz);

  }
  else {
    delete pCsvBuffer;
    pCsvBuffer = nullptr;
  }

  return pCsvBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
