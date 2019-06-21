/*============================================================================*/
/* FreqDist.cpp - Resource Tracker                                     */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
FreqDist::FreqDist(void)
: Object(CN_FREQDIST)
{
  _precondition_(_MAGIC_,CN_FREQDIST,_FOOTER_);
 
  m_nBuckets = 0;
  for (i8 i_Index = 0; i_Index < 128; i_Index++) {
    m_nBucket[i_Index] = C_UNKNOWN;
    m_nDist[i_Index] = 0;
  }

  _checkInitialised_(CN_FREQDIST);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
FreqDist::~FreqDist(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_FREQDIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* Add to the frequency distribution                                          */
/*----------------------------------------------------------------------------*/
void FreqDist::vAdd(i8 nBytes)
{
  for (i8 i_Index = 0; i_Index < m_nBuckets; i_Index++) {
    if (nBytes <= m_nBucket[i_Index]) {
      m_nDist[i_Index] += nBytes;
      return;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get no of buckets                                                          */
/*----------------------------------------------------------------------------*/
i8 FreqDist::nBuckets(void)
{
  return m_nBuckets;
}

/*----------------------------------------------------------------------------*/
/* get a bucket distribution value                                            */
/*----------------------------------------------------------------------------*/
i8 FreqDist::iDist(i8 i_Bucket)
{
  for (i8 i_Index = 0; i_Index < m_nBuckets; i_Index++)
    if (m_nBucket[i_Index]  == i_Bucket)
      return m_nDist[i_Index];
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* init the frequency distribution classes                                    */
/*----------------------------------------------------------------------------*/
void FreqDist::vInit(i8 nBuckets, ...)
{
  va_list args;

  va_start(args,nBuckets);
  
  m_nBuckets = nBuckets;
  for (i8 i_Index = 0; i_Index < nBuckets; i_Index++)
    m_nBucket[i_Index] = va_arg(args, int);
          
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* get the sum of the samples                                                 */
/*----------------------------------------------------------------------------*/
i8 FreqDist::nSum(void)
{
  i8 iSum = 0;
  
  for (i8 i_Index = 0; i_Index < m_nBuckets; i_Index++)
    iSum += m_nDist[i_Index];
  
  return iSum;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* FreqDist::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
