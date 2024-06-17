import argparse
import sys
import os
import typing
from stardock.generics import ServiceObject

class FileTreeScanner(ServiceObject[dict]):
  def exec(self) -> dict:
    self.mount_point: str
    res = self._analize_directory(".")
    if isinstance(res, dict):
      return res
    else:
      raise ValueError("path %s should be a directory" % self.mount_point)

  def _analize_directory(self, path: str) -> dict|str:
    fullpath = os.path.join(self.mount_point, path)
    if not os.path.exists(fullpath):
      raise ValueError("path '%s' doesn't exists" % fullpath)
    if os.path.isdir(fullpath):
      return {path: [self._analize_directory(os.path.join(path, entry)) for entry in os.listdir(fullpath)]}
    if os.path.isfile(fullpath):
      return path
    raise ValueError("path '%s' isn't identified" % fullpath)

class FileTreeVisitor(ServiceObject[None]):
  def exec(self) -> None:
    self.dir_fun: typing.Callable[[str], None]|None
    try:
      self.dir_fun
    except Exception:
      self.dir_fun = None
    if self.dir_fun is None:
      self.dir_fun = lambda _: None

    self.file_fun: typing.Callable[[str], None]|None
    try:
      self.file_fun
    except Exception:
      self.file_fun = None
    if self.file_fun is None:
      self.file_fun = lambda _: None

    self.file_tree: dict|str
    self._visit(self.file_tree)

  def _visit(self, entry: dict|str) -> None:
    if isinstance(entry, dict):
      for key in entry:
        self.dir_fun(key)
        for subentry in entry[key]:
          self._visit(subentry)
    elif isinstance(entry, str):
      self.file_fun(entry)
    else:
      ValueError("expected dict|str, got '%s'" % entry)

class FileTreeDiffer(ServiceObject[None]):
  def exec(self) -> None:
    self.src_mount_point: str
    self.src_tree = dict|str
    self.dst_mount_point: str
    self.dst_tree = dict|str

if __name__ == "__main__":
  cli = argparse.ArgumentParser()
  cli.add_argument("SRC", type=str, help="Source directory")
  cli.add_argument("DST", type=str, help="Destination directory")
  cli.add_argument("-v", "--verbose", action="store_true", default=False, help="Verbose log")
  cli.add_argument("-d", "--dry_run", action="store_true", default=False, help="Don't commit operations")
  config: argparse.Namespace = cli.parse_args(sys.argv[1:])

  if config.SRC is None or config.DST is None:
    cli.print_help()
    exit(1)

  if config.dry_run:
    exit(0)

  FileTreeVisitor.run(file_fun=lambda uri: print(config.SRC, uri),
                      file_tree=FileTreeScanner.run(mount_point=config.SRC))
  FileTreeVisitor.run(file_fun=lambda uri: print(config.DST, uri),
                      file_tree=FileTreeScanner.run(mount_point=config.DST))
