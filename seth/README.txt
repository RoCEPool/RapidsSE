README.txt
==========

This file tells you how to run unit tests with seth locally.

Seth needs to be told what tests to run. It can either be given the path to
specific tests on the command line, or it can be given a "-f" parameter and
a path to the testlist file which itself contains a list of tests to run.

./seth.sh      - Run all of the single node unit tests.
                 i.e. those defined in the seth/bamboo/testlists/1N.testlist
                 This is what you want to run if you simply want to test your
                 local changes.

./sethTest.sh  - Run whatever tests or testlists your heart desires.
                 Run this if there are specific tests that you want to run against
                 your local changes. You must supply parameters. See example below.

./seth1N1P.sh  - A convenient shortcut to run the single node single partition tests.

./sethMN.sh    - A convenient shortcut to run the multi-node tests.


Examples:
---------

To run Seth with a specific test file against your local changes:
  ./sethTest.sh path/to/your/test/file.test

To run Seth with multiple test files against your local changes:
  ./sethTest.sh path/to/file1.test path/to/file2.test path/to/file3.test

To run Seth with a testlist file that references multiple tests:
  ./sethTest.sh -f path/to/testlist

