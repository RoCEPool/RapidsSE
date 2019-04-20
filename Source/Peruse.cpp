/*============================================================================*/
/* Peruse.cpp - number peruse                                                 */
/*============================================================================*/

#include "Measurement.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Peruse::Peruse(Stz* szName)
: Object(CN_PERUSE)
{
  _precondition_(_MAGIC_,CN_PERUSE,_FOOTER_);
  
  m_sName  = szName;
  
  m_iCount = 0;
  m_iMax   = C_I8_MIN;
  m_iMin   = C_I8_MAX;
  m_iSum   = 0;

  m_iMean  = 0;
  m_iMeanSqr = 0;
  
  m_iGrandCount = 0;
  m_iGrandMax   = C_I8_MIN;
  m_iGrandMin   = C_I8_MAX;
  m_iGrandSum   = 0;
  
  m_iGrandMean  = 0;
  m_iGrandMeanSqr = 0;

  _checkInitialised_(CN_PERUSE);
  
  _postcondition_(_MAGIC_,CN_PERUSE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Peruse::~Peruse(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_PERUSE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* generate next name                                                         */
/*----------------------------------------------------------------------------*/
void Peruse::vAdd(i8 iArg, Stz* szFormat,...)
{
  _precondition_(_MAGIC_,CN_PERUSE,_FOOTER_);

  m_iCount++;
  
  m_iSum += iArg;
  if (iArg < m_iMin)
    m_iMin = iArg;
  if (iArg > m_iMax)
    m_iMax = iArg;

  i8 iDelta = iArg - m_iMean;
  m_iMean += iDelta / m_iCount;
  i8 iDelta2 = iArg - m_iMean;
  m_iMeanSqr += iDelta * iDelta2;
  
  
  m_iGrandCount++;
  
  m_iGrandSum += iArg;
  if (iArg < m_iGrandMin)
    m_iGrandMin = iArg;
  if (iArg > m_iGrandMax)
    m_iGrandMax = iArg;
  
  i8 iGrandDelta = iArg - m_iGrandMean;
  m_iGrandMean += iGrandDelta / m_iGrandCount;
  i8 iGrandDelta2 = iArg - m_iGrandMean;
  m_iGrandMeanSqr += iGrandDelta * iGrandDelta2;
  
  if (IntLib::bIsDecaCycle(m_iCount)) {
    
    i8 iGrandAvg = m_iGrandSum / m_iGrandCount;
    i8 iGrandVar = m_iGrandMeanSqr / m_iGrandCount;

    i8 iGrandStdDev = (i8)sqrt((d8)(iGrandVar));
    
    vTrace("%s cnt %lld sum %lld min %lld max %lld avg %lld var %lld stddev %lld"_Stz,
           m_sName.aStz(),
           m_iGrandCount,
           m_iGrandSum,
           m_iGrandMin,
           m_iGrandMax,
           iGrandAvg,
           iGrandVar,
           iGrandStdDev
          );
    
    va_list xArgs;
    va_start(xArgs,szFormat);
    ::vPrint(szFormat,xArgs);
    va_end(xArgs);
  }
}
/*
# for a new value newValue, compute the new count, new mean, the new M2.
# mean accumulates the mean of the entire dataset
# M2 aggregates the squared distance from the mean
# count aggregates the number of samples seen so far
def update(existingAggregate, newValue):
(count, mean, M2) = existingAggregate
count += 1
delta = newValue - mean
mean += delta / count
delta2 = newValue - mean
M2 += delta * delta2

return (count, mean, M2)

# retrieve the mean, variance and sample variance from an aggregate
def finalize(existingAggregate):
(count, mean, M2) = existingAggregate
(mean, variance, sampleVariance) = (mean, M2/count, M2/(count - 1))
if count < 2:
return float('nan')
else:
return (mean, variance, sampleVariance)
*/


/*----------------------------------------------------------------------------*/
/* generate next name                                                         */
/*----------------------------------------------------------------------------*/
void Peruse::vReset(void)
{
  _precondition_(_MAGIC_,CN_PERUSE,_FOOTER_);
  
  m_iCount = 0;
  m_iMax   = C_I8_MIN;
  m_iMin   = C_I8_MAX;
  m_iSum   = 0;
  
  m_iMean = 0;
  m_iMeanSqr = 0;
}

/*----------------------------------------------------------------------------*/
/* generate next name                                                         */
/*----------------------------------------------------------------------------*/
void Peruse::vPrint(Stz* szFormat,...)
{
  _precondition_(_MAGIC_,CN_PERUSE,_FOOTER_);
  
  i8 iAvg = m_iSum / m_iCount;
  i8 iVar = m_iMeanSqr / m_iCount;
  
  i8 iStdDev = (i8)sqrt((d8)(iVar));
  
  vTrace("%s cnt %lld sum %lld min %lld max %lld avg %lld var %lld stddev %lld"_Stz,
         m_sName.aStz(),
         m_iCount,
         m_iSum,
         m_iMin,
         m_iMax,
         iAvg,
         iVar,
         iStdDev
         );
  
  va_list xArgs;
  va_start(xArgs,szFormat);
  vPrint(szFormat,xArgs);
  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Peruse::aFooter(void)
{
  _precondition_(_MAGIC_,CN_PERUSE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
