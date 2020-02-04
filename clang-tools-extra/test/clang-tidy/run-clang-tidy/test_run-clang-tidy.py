#!/usr/bin/env python

import unittest
import io
import subprocess
import os


class TestRunClangTidy(unittest.TestCase):
    def run_with_mock(self, testdir):
        run_clang_tidy_path = os.path.join(
            "..", "..", "..", "clang-tidy", "tool", "run-clang-tidy.py"
        )
        invocation = [
            run_clang_tidy_path,
            "-clang-tidy-binary=./clang-tidy-mock.py",
            "-p=" + testdir,
            "-export-fixes=actual_output.tmp.yaml",
        ]
        with open(os.devnull, "w") as dev_null:
            subprocess.check_call(invocation, stdout=dev_null)
        self.maxDiff = None
        self.assertListEqual(
            list(io.open(os.path.join(testdir, "expected-output.yaml"))),
            list(io.open("actual_output.tmp.yaml")),
        )

    def tearDown(self):
        os.remove("actual_output.tmp.yaml")

    def test_1_trivial(self):
        self.run_with_mock("test_1_trivial")

    def test_2_merge(self):
        self.run_with_mock("test_2_merge")

    def test_3_only_fixable(self):
        self.run_with_mock("test_3_only_fixable")


if __name__ == "__main__":
    unittest.main()
