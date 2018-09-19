#ifndef INotifier_h
#define INotifier_h

#include <Common/Serializable.h>

class INotifier
{
protected:
  virtual void publishData(const char *topic, Serializable &data) = 0;
  virtual void publishData(const char *topic, const char *message) = 0;
public:
  virtual void publishTelemetry(const char *message) = 0;
  virtual void publishTelemetry(Serializable &data) = 0;
  virtual void publishAttributes(const char *message) = 0;
  virtual void publishAttributes(Serializable &data) = 0;
};

#endif