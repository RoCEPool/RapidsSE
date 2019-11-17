/*============================================================================*/
/* FreqDist.cpp - Frequency Distribution                                      */
/*============================================================================*/

#include "Gadgets.h"


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
/* get a bucket at index                                                      */
/*----------------------------------------------------------------------------*/
i8 FreqDist::iBucket(i8 i_Index)
{
  return m_nBucket[i_Index];
}

/*----------------------------------------------------------------------------*/
/* get a bucket distribution value of bucket                                  */
/*----------------------------------------------------------------------------*/
i8 FreqDist::iDist(i8 i_Bucket)
{
  return m_nDist[i_Bucket];
}

/*----------------------------------------------------------------------------*/
/* init the frequency distribution classes                                    */
/*----------------------------------------------------------------------------*/
void FreqDist::vInit(i8 i_Bucket, ...)
{
  va_list args;

  i8 i_Index = 0;
  va_start(args,i_Bucket);
  
  // add first bucket
  m_nBucket[i_Index++] = i_Bucket;
  
  do {
    i_Bucket = va_arg(args, int);
    m_nBucket[i_Index++] = i_Bucket;
    
  } while (i_Bucket != INT_MAX);
  
  m_nBuckets = i_Index;
          
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
__aFooterFunction(FreqDist)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
