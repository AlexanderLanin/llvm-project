#!/usr/bin/env python

import argparse
import os
import shutil
import sys


def main(argv):
    # difficult to parse in argparse, not needed anyway
    if "-" in argv:
        argv.remove("-")

    parser = argparse.ArgumentParser(description="Mocks clang-tidy")
    parser.add_argument("-list-checks", dest="list_checks", action="store_true")
    parser.add_argument("-p", dest="build_path")
    parser.add_argument("-export-fixes", dest="export_fixes")
    parser.add_argument("files", nargs="*")
    args = parser.parse_args(argv)

    if args.list_checks:
        print "mocked-check-1"
        return

    if args.files and args.export_fixes:
        assert len(args.files) == 1
        head_tail = os.path.split(args.export_fixes)
        # ensure deterministic (alphabetical) order
        fixed_export_fixes = os.path.join(
            head_tail[0], os.path.split(args.files[0])[1] + "_" + head_tail[1]
        )
        print (args.files[0] + " --> " + fixed_export_fixes)
        shutil.copy(args.files[0], fixed_export_fixes)
        return


if __name__ == "__main__":
    main(sys.argv[1:])
