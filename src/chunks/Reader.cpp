/*============================================================================*/
/* Reader.cpp -- Assistant File Reader                                        */
/*============================================================================*/

#include "Chunks.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Chunks,Reader,CN_READER)
__aNewFunction(Reader,CN_READER)
__vDeleteFunction(Reader,CN_READER)
__aFooterFunction(Reader)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Reader::Reader(void)
: Object(CN_READER)
, m_oFile(nullptr)
{
  _precondition_(_MAGIC_,CN_READER,_FOOTER_);
    
  m_i_Seek = 0;
  
  _checkInitialised_(CN_PERUSE);
  
  _postcondition_(_MAGIC_,CN_READER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Reader::~Reader(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_READER,_FOOTER_);
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

  m_oFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool Reader::bOpenRead(Stz* szPath)
{
  _precondition_(_MAGIC_,CN_READER,_FOOTER_);

  // open the file
  bool bOpened = m_oFile.bOpenRead(szPath);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* read lines                                                                 */
/*----------------------------------------------------------------------------*/
i8 Reader::nReadLines(Stz* szInto, i8 nMaxBytes)
{
  _precondition_(_MAGIC_,CN_READER,_FOOTER_);

  m_oFile.vSeek(m_i_Seek);
  
  i8 nBytesRead = m_oFile.nReadBytes(szInto->azChar(),nMaxBytes);
  
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
