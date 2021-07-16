#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();
  
  void Update(long idle, long active, long total);

  
  // TODO: Declare any necessary private members
 private:
  long idle_; //m_idle
  long active_;// m_active
  long total_;//m_total

};

#endif