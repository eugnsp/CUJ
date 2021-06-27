#ifndef OBSERVATION_H_
#define OBSERVATION_H_

#include <time.h>

class observation
{
public:
  typedef float range_type;

  observation(range_type rHigh, range_type rLow, short sID, 
              time_t DateTime) : m_rHigh(rHigh), m_rLow(rLow),
                                 m_sID(sID){}

  float low() const { return m_rLow; }
  float high() const { return m_rHigh; }
  short id() const { return m_sID; }
  time_t date() const { return m_tmDate; }

private:
  range_type  m_rHigh;
  range_type  m_rLow;
  short       m_sID;
  time_t      m_tmDate;
};

#endif // OBSERVATION_H_
