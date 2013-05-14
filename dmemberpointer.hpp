#ifndef DESTRUCT_MEMPTR_H
#define DESTRUCT_MEMPTR_H

namespace Destruct
{

template <typename BaseType, typename BaseTargetType>
class MemPtrBase
{
public:
  virtual BaseTargetType & value(BaseType & obj) const = 0;
  virtual BaseTargetType const & value(BaseType const & obj) const = 0;
  virtual ~MemPtrBase() {};

protected:
  MemPtrBase() {}

private:
  MemPtrBase(MemPtrBase const &);
  MemPtrBase & operator=(MemPtrBase const &);
};

template <typename BaseType, typename BaseTargetType, typename TargetType>
class TypedMemPtr : public MemPtrBase<BaseType, BaseTargetType>
{
public:
  TypedMemPtr(TargetType BaseType::* ptr) : p(ptr)
  {
  }

  BaseTargetType & value(BaseType & obj) const
  {
    return (obj.*p);
  }

  BaseTargetType const & value(BaseType const & obj) const
  {
    return (obj.*p);
  }

private:
  TargetType BaseType::* p;
};

template <typename BaseType, typename BaseTargetType>
class MemberPointer
{
public:
  template <typename BaseType2, typename TargetType> 
  explicit MemberPointer(TargetType BaseType2::* ptr) : p(new TypedMemPtr<BaseType, BaseTargetType, TargetType>(static_cast<TargetType BaseType::*>(ptr)))
  {
  }
    
  ~MemberPointer()
  {
    delete p;
  }
    
  BaseTargetType & value(BaseType & obj) const
  {
    return (p->value(obj));
  }
    
  BaseTargetType const & value(BaseType const & obj) const
  {
    return (p->value(obj));
  }
    
private:
  MemPtrBase<BaseType, BaseTargetType> * p;
};

}
#endif
