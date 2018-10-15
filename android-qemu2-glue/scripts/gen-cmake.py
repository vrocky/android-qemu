#!/usr/bin/env python
#
# This script is used to generate a set of cmake
# files that can be used to compile the Qemu components
# of the android emulator.
#
# This is done be extracting all the link information from
# LINK-xxx files that are generated by executing ./android/scripts/build-qemu-android.sh
# TODO(jansene): We will make this part of the build, as running this script locally (esp on mac)
# can produce inconsistent builds. (See b/117181765)
#
# These files contain the objects (and their order) used to link qemu. We extract this object
# information and roughly do the following:
#
# - Check if it is an autogenerated qemu file (occurs under ./qemu2-autogenerated or is not in git)
# - Check if this is a possible shared file (prefixed by .. in link files)
# - Check if this is a librabry dependency.
#
# Once all this information is collected we generate a file for each host that declares the sources
# needed to compile qemu.
#
# NOTE: There are serveral magical source files that should not be included:
#  'hw/i386/acpi-build'
#  'hw/i386/pc_piix'
#  'vl.c'
#
# These files need to be compiled with the android configuration flag, your image will not boot
# with default compilation flags for these.
#
# Invoking the script is usually done as follows:
#
# $ cd $QEMU2_TOP_DIR
# $ android-qemu2-glue/scripts/gen-cmake.py -i ../../prebuilts/android-emulator-build/qemu-upstream -r .
#
from __future__ import print_function
import argparse
import logging
import os
import random
import sys
import itertools
import shutil
import subprocess


class SetLikeList(list):
  """A set like list has set operations defined on a standard list.

  This means that every object only occurs once, and you can do the usual
  set operations on this list.

  Note that this thing is super expensive, and should only be used if
  insertion order is important.
  """

  def add(self, item):
    if item not in self:
      self.append(item)

  def __sub__(self, other):
    for idx in xrange(len(self) - 1, 0, -1):
      if self[idx] in other:
        del self[idx]
    return self

  def __and__(self, other):
    for idx in xrange(len(self) - 1, 0, -1):
      if self[idx] not in other:
        del self[idx]
    return self

  def __or__(self, other):
    for item in other:
      self.add(item)


class GitFiles(object):
  IGNORED_OBJECTS = [
      'audio/sdlaudio',
      'gdbstub-xml',
      'hax-stub',
      # These have to be compiled with specific flags, and cannot be included
      'hw/i386/acpi-build',
      'hw/i386/pc_piix',  # in the default build..

      # these aren't used in the emulator but add 1MB+ to binary size
      'hw/net/e1000',
      'hw/net/e1000e',
      'hw/net/e1000e_core',
      'hw/net/e1000x_common',
      'ui/sdl_zoom',
      'ui/sdl',
      'ui/sdl2',
      'ui/sdl2-2d',
      'ui/sdl2-input',
      'vl',
      'version',  # something from the Windows build
  ]

  def __init__(self, root):
    self.root = root
    self.files = set(
        subprocess.check_output(['git', 'ls-files'], cwd=root).split('\n'))

  def get_revision(self):
    """Returns the current sha of this git repo."""
    return subprocess.check_output(['git', 'rev-parse', 'HEAD'],
                                   cwd=self.root).strip()

  @staticmethod
  def base_name(fname):
    """Returns the filename without extension or prefix."""
    if fname.startswith('../'):
      fname = fname[3:]
    if '.' in fname:
      return os.path.splitext(fname)[0]
    else:
      return fname

  def is_c_file(self, fname):
    """Returns true if this is a .c file."""
    return self.base_name(fname) + '.c' in self.files

  def is_cc_file(self, fname):
    """Returns true if this is a .cc file."""
    return self.base_name(fname) + '.cc' in self.files

  def ignore(self, fname):
    for x in self.IGNORED_OBJECTS:
      if x in fname:
        return True
    return False

  def is_generated(self, fname):
    """Returns true if the fname is generated, ie.
       not found in the qemu root."""
    if fname.startswith('../'):
      fname = fname[3:]

    fname = os.path.splitext(fname)[0] + '.c'

    gen_file = os.path.join('qemu2-auto-generated', fname)
    return gen_file in self.files

  @staticmethod
  def is_archive(obj):
    """Returns true if this object refers to an archive."""
    return obj[-2:] == '.a'

  def object_to_file(self, obj):
    """Translates the .o file into a proper c/cc file."""
    if obj.startswith('../'):
      obj = obj[3:]

    obj = os.path.splitext(obj)[0]

    if (self.is_generated(obj)):
      return obj + '.c'

    if (self.is_c_file(obj)):
      return self.base_name(obj) + '.c'
    if (self.is_cc_file(obj)):
      return self.base_name(obj) + '.cc'
    logging.info('Unknown object: %s', obj)
    return None


EXPECTED_HOSTS = set(
    ['linux-x86_64', 'windows-x86', 'windows-x86_64', 'windows_msvc-x86_64', 'darwin-x86_64'])


def read_link_file(link_file, git_fs):
  """Parses the link file.

  The link file contains all the file included in a lib/exe. We have to look
  at everything between the (rcs for archive) and (-o for executable) and the
  first - which start the link needs.

  Returning (Name, Type, [Files], [Dependencies, [maybe_shared])
  """
  files = []
  deps = set()
  shared = set()
  with open(link_file) as lfile:
    content = lfile.readlines()
    content = [
        x.strip() for x in itertools.dropwhile(
            lambda x: x.strip() != '-o' and x.strip() != 'rcs', content)
    ]
    if not content:
      logging.warn('Nothing found in %s', link_file)
      return None
    typ = content[0]
    if content[1].endswith('w.exe'):
      name = content[1][:-5]
    else:
      name = git_fs.base_name(content[1])
    for obj in content[2:]:
      # Check if we have seen all files.
      if obj[0] == '-':
        break
      if git_fs.ignore(obj):
        logging.info('Ignoring %s', obj)
        continue

      fname = git_fs.object_to_file(obj)
      # Possible shared objects start with .. in qemu
      # build
      if obj.startswith('..') and fname:
        shared.add(fname)

      if git_fs.is_archive(obj):
        deps.add(git_fs.base_name(obj))
      else:
        if fname:
          files.append(git_fs.object_to_file(obj))
        else:
          logging.info('Ignoring: %s', obj)
  return name, typ, files, deps, shared


def find_link_files(build_path, host, git_fs):
  discovered = []
  build_path = os.path.join(build_path, host)
  link_prefix = 'LINK-'
  for subdir, _, files in os.walk(build_path):
    for efile in files:
      if efile.startswith(link_prefix):
        link = read_link_file(os.path.join(subdir, efile), git_fs)
        if link:
          discovered.append(link)

  return discovered


def group_by_dir(list_of_files):
  """Groups all files by directory."""
  buckets = {}
  for dir_name, g in itertools.groupby(list_of_files, os.path.dirname):
    if dir_name in buckets:
      buckets[dir_name] += list(g)
    else:
      buckets[dir_name] = list(g)
  return buckets


def split_by_generated(file_list, git_fs):
  """Splits the files into generated and non-generated."""
  generated = [fname for fname in file_list if git_fs.is_generated(fname)]
  files = [fname for fname in file_list if not git_fs.is_generated(fname)]
  return generated, files


def source_properties(source_list, git_fs):
  """Sets the source properties for all files to include the directory of the
  source itself. This is needed so cmake has a proper include directory for the
  qemu trace files.
  """
  cmake = ''
  buckets = group_by_dir(source_list)
  for dir_name, file_list in buckets.iteritems():
    generated, files = split_by_generated(file_list, git_fs)
    sources = ['${ANDROID_AUTOGEN}/' + fname for fname in generated]
    sources += files

    sources = '${ANDROID_QEMU2_TOP_DIR}/' + ' ${ANDROID_QEMU2_TOP_DIR}/'.join(
        sources)
    cmake += ('set_source_files_properties(%s PROPERTIES COMPILE_FLAGS " '
              '-I ${ANDROID_QEMU2_TOP_DIR}/${ANDROID_AUTOGEN}/%s")\n') % (
                  sources, dir_name)
  return cmake


def transform(target, git_fs, file_list, libdeps):
  generated, files = split_by_generated(file_list, git_fs)

  cmake = ' ## TARGET: %s ##\n\n' % target
  cmake += 'set(%s_sources ""\n   ' % target
  cmake += '\n   '.join(files)
  cmake += ')\n'
  cmake += 'set(%s_generated_sources "" \n' % target
  cmake += '${ANDROID_AUTOGEN}/' + '\n  ${ANDROID_AUTOGEN}/'.join(generated)
  cmake += ')\n\n'
  cmake += 'set(%s_dep %s)\n\n' % (target, ' '.join(libdeps))
  return cmake


def main(_):
  parser = argparse.ArgumentParser(description='Generate the sub-Makefiles '
                                   'describing the common and '
                                   'target-specific sources for the QEMU2 '
                                   'build performed with the emulator\'s '
                                   'build system. This is done by looking at '
                                   'the output of a regular QEMU2 build, and '
                                   'finding which files were built and '
                                   'where.')

  parser.add_argument(
      '-i',
      '--input',
      dest='inputs',
      type=str,
      required=True,
      help='The input directory containing all the '
      'build binaries.')
  parser.add_argument(
      '-r',
      '--root',
      dest='root',
      type=str,
      required=True,
      default='.',
      help='The qemu root directory')
  parser.add_argument(
      '-s',
      '--host_set',
      dest='hosts',
      action='append',
      help='Restrict the generated sources only to this host set. '
      'DO NOT USE, ONLY USED FOR MAKING MERGES EASIER"')
  parser.add_argument(
      '-v',
      '--verbose',
      action='store_const',
      dest='loglevel',
      const=logging.INFO,
      help='Be more verbose')

  args = parser.parse_args()

  if not args.hosts:
    args.hosts = EXPECTED_HOSTS

  logging.basicConfig(level=args.loglevel)

  build_dir = args.inputs
  git_fs = GitFiles(args.root)

  for host in args.hosts:
    logging.info('Parsing %s', host)
    all_sources = set()
    shared_dependencies = None
    deps = sorted(find_link_files(build_dir, host, git_fs), key=lambda t : t[0])
    targets = [t for (t, _, _, _, _) in deps]
    cmake =  '## AUTOGENERATED FILE, DO NOT EDIT \n'
    cmake += '# Instead execute the following: \n'
    cmake += '# ./android/scripts/build-qemu-android.sh \n'
    cmake += '# ./android-qemu2-glue/scripts/gen-cmake.py -i ../../prebuilts/android-emulator-build/qemu-upstream -r .\n'
    cmake += '# Defines targets: ' + '\n#   '.join(targets)
    cmake += '\n\n'
    for (target, _, files, libdeps, shared) in deps:
      logging.info('Considering target: %s - files: %d', target, len(files))
      all_sources |= set(files)
      if 'qemu-system' in target:
        if shared_dependencies is None:
          shared_dependencies = set(shared)
        else:
          shared_dependencies &= set(shared)

    if not shared_dependencies:
      shared_dependencies = set(shared)

    cmake += transform('qemu2-shared', git_fs, shared_dependencies, [])
    for (target, _, files, libdeps, _) in deps:
      # Remove the shared dependencies
      files = [x for x in files if x not in shared_dependencies]
      libdeps.add('qemu2-shared')
      cmake += transform(target, git_fs, files, libdeps)

    # Set all the source properties to make sure we set
    # the include directory right.
    cmake += source_properties(all_sources, git_fs)

    # And write the cmake file.
    with open('cmake-main.%s.inc' % host, 'w') as f:
      f.write(cmake)


if __name__ == '__main__':
  main(sys.argv)
