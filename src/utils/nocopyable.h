#ifndef NOCOPYABLE_H
#define NOCOPYABLE_H

BEGIN_NAMESPACE(utils)

class nocopyable{
private:
  nocopyable(const nocopyable& x) = delete;
  nocopyable& operator=(const nocopyable& x) = delete;
public:
  nocopyable() = default;
  ~nocopyable() = default;
};

END_NAMESPACE
#endif
