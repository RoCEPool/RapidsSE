/*============================================================================*/
/* Peruse.cpp - number peruser                                                */
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

  m_dM1    = 0.0;
  m_dM2    = 0.0;
  m_dM3    = 0.0;
  m_dM4    = 0.0;

  m_iGrandCount = 0;
  m_iGrandMax   = C_I8_MIN;
  m_iGrandMin   = C_I8_MAX;
  m_iGrandSum   = 0;
  
  m_dGrandM1    = 0.0;
  m_dGrandM2    = 0.0;
  m_dGrandM3    = 0.0;
  m_dGrandM4    = 0.0;

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
  
  d8 dDelta    = iArg - m_dM1;
  d8 dDelta_n  = dDelta / m_iCount;
  d8 dDelta_n2 = dDelta_n * dDelta_n;
  d8 term1     = dDelta * dDelta_n * (m_iCount-1);
  
  
  m_dM1 += dDelta_n;
  m_dM4 += term1 * dDelta_n2
             * (m_iGrandCount*m_iGrandCount - 3*m_iGrandCount + 3)
             + 6 * dDelta_n2 * m_dM2 - 4 * dDelta_n * m_dM3;
  m_dM3 += term1 * dDelta_n * (m_iGrandCount - 2) - 3 * dDelta_n * m_dM2;
  m_dM2 += term1;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_iGrandSum += iArg;
  if (iArg < m_iGrandMin)
    m_iGrandMin = iArg;
  if (iArg > m_iGrandMax)
    m_iGrandMax = iArg;
  
  m_iGrandCount++;
  
  dDelta    = iArg - m_dGrandM1;
  dDelta_n  = dDelta / m_iGrandCount;
  dDelta_n2 = dDelta_n * dDelta_n;
  term1     = dDelta * dDelta_n * (m_iGrandCount-1);
  
  m_dGrandM1 += dDelta_n;
  m_dGrandM4 += term1 * dDelta_n2
                      * (m_iGrandCount*m_iGrandCount - 3*m_iGrandCount + 3)
                  + 6 * dDelta_n2 * m_dGrandM2
                  - 4 * dDelta_n  * m_dGrandM3;
  m_dGrandM3 += term1 * dDelta_n * (m_iGrandCount - 2)
                  - 3 * dDelta_n * m_dGrandM2;
  m_dGrandM2 += term1;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (IntLib::bIsDecaCycle(m_iCount)) {
    
    i8 iGrandAvg = m_iGrandSum / m_iGrandCount;
    
    d8 dGrandVar = m_dGrandM2/(m_iGrandCount-1.0);
    
    d8 dGrandStdDev = sqrt( dGrandVar );
    
    d8 dGrandSkew = sqrt(double(m_iGrandCount)) * m_dGrandM3/ pow(m_dGrandM2, 1.5);
    
    d8 dGrandKurt = double(m_iGrandCount)*m_dGrandM4 / (m_dGrandM2*m_dGrandM2) - 3.0;
    
    vTrace("%s cnt %lld sum %lld min %lld max %lld avg %lld var %g dev %g skw %g krt %g"_Stz,
           m_sName.aStz(),
           m_iGrandCount,
           m_iGrandSum,
           m_iGrandMin,
           m_iGrandMax,
           iGrandAvg,
           dGrandVar,
           dGrandStdDev,
           dGrandSkew,
           dGrandKurt
          );
    
    va_list xArgs;
    va_start(xArgs,szFormat);
    ::vPrint(szFormat,xArgs);
    va_end(xArgs);
  }
}

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
  
  m_dM1    = 0.0;
  m_dM2    = 0.0;
  m_dM3    = 0.0;
  m_dM4    = 0.0;
  
}

/*----------------------------------------------------------------------------*/
/* generate next name                                                         */
/*----------------------------------------------------------------------------*/
void Peruse::vPrint(Stz* szFormat,...)
{
  _precondition_(_MAGIC_,CN_PERUSE,_FOOTER_);
  
  i8 iAvg = m_iSum / m_iCount;
  
  d8 dVar = m_dM2/(m_iCount-1.0);
  
  d8 dStdDev = sqrt( dVar );
  
  d8 dSkew = sqrt(double(m_iCount)) * m_dM3/ pow(m_dM2, 1.5);
  
  d8 dKurt = double(m_iCount)*m_dM4 / (m_dM2*m_dM2) - 3.0;
  
  vTrace("%s cnt %lld sum %lld min %lld max %lld avg %lld var %g dev %g skw %g krt %g"_Stz,
         m_sName.aStz(),
         m_iCount,
         m_iSum,
         m_iMin,
         m_iMax,
         iAvg,
         dVar,
         dStdDev,
         dSkew,
         dKurt
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

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// refer: https://www.johndcook.com/blog/skewness_kurtosis/

/*
#ifndef RUNNINGSTATS_H
#define RUNNINGSTATS_H

class RunningStats
{
public:
  RunningStats();
  void Clear();
  void Push(double x);
  long long NumDataValues() const;
  double Mean() const;
  double Variance() const;
  double StandardDeviation() const;
  double Skewness() const;
  double Kurtosis() const;
  
  friend RunningStats operator+(const RunningStats a, const RunningStats b);
  RunningStats& operator+=(const RunningStats &rhs);
  
private:
  long long n;
  double M1, M2, M3, M4;
};

#endif


RunningStats::RunningStats()
{
  Clear();
}

void RunningStats::Clear()
{
  n = 0;
  M1 = M2 = M3 = M4 = 0.0;
}

void RunningStats::Push(double x)
{
  double delta, delta_n, delta_n2, term1;
  
  long long n1 = n;
  n++;
  delta = x - M1;
  delta_n = delta / n;
  delta_n2 = delta_n * delta_n;
  term1 = delta * delta_n * n1;
  M1 += delta_n;
  M4 += term1 * delta_n2 * (n*n - 3*n + 3) + 6 * delta_n2 * M2 - 4 * delta_n * M3;
  M3 += term1 * delta_n * (n - 2) - 3 * delta_n * M2;
  M2 += term1;
}

long long RunningStats::NumDataValues() const
{
  return n;
}

double RunningStats::Mean() const
{
  return M1;
}

double RunningStats::Variance() const
{
  return M2/(n-1.0);
}

double RunningStats::StandardDeviation() const
{
  return sqrt( Variance() );
}

double RunningStats::Skewness() const
{
  return sqrt(double(n)) * M3/ pow(M2, 1.5);
}

double RunningStats::Kurtosis() const
{
  return double(n)*M4 / (M2*M2) - 3.0;
}

RunningStats operator+(const RunningStats a, const RunningStats b)
{
  RunningStats combined;
  
  combined.n = a.n + b.n;
  
  double delta = b.M1 - a.M1;
  double delta2 = delta*delta;
  double delta3 = delta*delta2;
  double delta4 = delta2*delta2;
  
  combined.M1 = (a.n*a.M1 + b.n*b.M1) / combined.n;
  
  combined.M2 = a.M2 + b.M2 +
  delta2 * a.n * b.n / combined.n;
  
  combined.M3 = a.M3 + b.M3 +
  delta3 * a.n * b.n * (a.n - b.n)/(combined.n*combined.n);
  combined.M3 += 3.0*delta * (a.n*b.M2 - b.n*a.M2) / combined.n;
  
  combined.M4 = a.M4 + b.M4 + delta4*a.n*b.n * (a.n*a.n - a.n*b.n + b.n*b.n) /
  (combined.n*combined.n*combined.n);
  combined.M4 += 6.0*delta2 * (a.n*a.n*b.M2 + b.n*b.n*a.M2)/(combined.n*combined.n) +
  4.0*delta*(a.n*b.M3 - b.n*a.M3) / combined.n;
  
  return combined;
}

RunningStats& RunningStats::operator+=(const RunningStats& rhs)
{
  RunningStats combined = *this + rhs;
  *this = combined;
  return *this;
}

*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
