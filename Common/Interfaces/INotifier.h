#ifndef INotifier_h
#define INotifier_h

#include <Common/Serializable.h>

class INotifier
{
public:
  virtual void publishData(const char *topic, Serializable &data) = 0;
  virtual void publishData(const char *topic, const char *message) = 0;
};

#endif