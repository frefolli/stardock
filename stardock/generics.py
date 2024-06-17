import abc
import typing

Type = typing.TypeVar('Type')
class ServiceObject(abc.ABC, typing.Generic[Type]):
  """Trait for Ruby's inspired Service Object Pattern"""
  def __init__(self, **kwargs) -> None:
    for key, value in kwargs.items():
      self.__setattr__(key, value)
  
  @abc.abstractmethod
  def exec(self) -> Type:
    """Execute the service"""
    pass
  
  @classmethod
  def run(cls, **kwargs) -> Type:
    """Creates a new instance of the service and execute it"""
    return cls(**kwargs).exec()