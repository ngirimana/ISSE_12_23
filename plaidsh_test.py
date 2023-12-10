#! /usr/bin/env python3
#
# An automated interactive test for plaidsh, based on top of pexpect
#
# Author: Howdy Pierce, howdy@sleepymoose.net
#
#  Usage: ./ps_interactive_test.py (executable-name)

import pexpect
import sys
import re
import os

# re to match the prompt
prompt_re = "#\?.* "

initial_cwd=os.getcwd()

# find the setup_playground script by searching these directories in order
script_path=['.', '/var/local/isse-12']
for s in script_path:
    setup_script=os.path.join(s, "setup_playground.sh")
    if (os.path.exists(setup_script)):
        break

print(f"Found {setup_script}...")

# (input-string) (expected-output-regexp) (points)
tests = [
    # forms of the empty string
    ("", "", 1),
    ("    ", "", 1),
    ("  \\  ", "", 1),

    # examples from p. 1 of the assignment writeup
    ("pwd", initial_cwd, 1),
    ("ls --color", os.listdir(".")[0], 1),
    (setup_script, "Plaid Shell playground created", 1),
    ("cd Plaid\\ Shell\\ Playground", "", 1),
    ("pwd", f"{initial_cwd}/Plaid Shell Playground", 1),
    ("ls",
     "README +'best sitcoms.txt'[ \t]+'seven dwarfs.txt'[ \t]+shells.txt", 1),
    ("ls *.txt",
     "'best sitcoms.txt'[ \t]+'seven dwarfs.txt'[ \t]+shells.txt", 1),
    ("echo $PATH", "\\$PATH", 1),
    ("author", "", 1),
    ("author | sed -e \"s/^/Written by /\"", "Written by ", 1),
    ("grep Happy *.txt",
     "best sitcoms.txt:Happy Days.*seven dwarfs.txt:Happy", 1),
    ("cat \"best sitcoms.txt\" | grep Seinfeld", "Seinfeld", 1),
    ("cat \"best sitcoms.txt\"|grep Seinfeld|wc -l", "1", 1),
    ("sed -ne \"s/The Simpsons/I Love Lucy/p\" < best\\ sitcoms.txt > output",
     "", 1),
    ("ls -l", "output", 1),
    ("cat output", "I Love Lucy", 1),
    ("this is not a command",
     "this: Command not found.*Child.*exited with status 2", 2),
    ("echo Hello > /usr/bin/cant_write",
     "/usr/bin/cant_write: Permission denied", 2),
    ("cd", "", 1),
    ("pwd", os.getenv("HOME"), 2),
    (f"cd {initial_cwd}", "", 1),
    ("pwd", initial_cwd, 1),
    ("cd ~", "", 1),
    ("pwd", os.getenv("HOME"), 2),

    # Test the up-arrow for history
    ("echo \"Operator could you help me place this call?\"",
     "Operator could you help me place this call\\?", 1),
    ("seq 10 | wc\"-l\"", "10", 1),
    ('\x1b[A\x1b[A', "Operator could you help me place this call\\?", 2),

    # additional tricky examples, some with errors
    ("env\t|grep PATH", os.getenv("PATH"), 2),
    ("echo Hello World | cat -n | cat -n | cat -n",
     "1[ \t]+1[ \t]+1[ \t]+Hello World", 2),
    ("echo \\c", "Illegal escape character '?c", 2),
    ("echo \"\\c\"", "Illegal escape character '?c", 1),
    ("echo \"hi", "Unterminated quote", 2),
    ("echo \"|1|2|3|\" | sed -e \"s/[0-9]//\"", "||||", 1),
    ("printf \"=%s=\\n\" one two three four five six seven eight nine ten "
     "eleven twelve thirteen fourteen fifteen sixteen seventeen eighteen "
     "nineteen twenty twenty-one twenty-two twenty-three twenty-four|"
     "wc -l", "24", 2),
    ("printf\"+%s\"one\"two\"three\n", "\\+one\\+two\\+three", 2),
    ("echo > file1 >file2", "Multiple redirection", 1),
    ("cat <", "Expect filename after redirection", 1),
    ("cat | cat | cat >", "Expect filename after redirection", 1),
    ("grep | ", "No command specified", 1),
    ("| grep", "No command specified", 1),
    ("echo || grep", "No command specified", 1),
    ("echo \\<\\|\\> | cat", "<\\|>", 1),
    ("echo hello\\|grep ell", "hello\\|grep ell", 1),

    # make certain that exit works
    ("exit", pexpect.EOF, 1)
]

# run the tests and report the resulting score
def run_tests(executable):
    score_pts=0    # points scored
    total_pts=0    # total possible points

    child = pexpect.spawn(executable, encoding='utf-8', timeout=1)

    logname = f"{executable}_test.log"
    flog = open(logname, mode='w')
    child.logfile_read = flog

    for test in tests:
        inp = test[0]
        exp_result = test[1]
        total_pts += test[2]

        child.expect(prompt_re)
        child.sendline(inp)
        try:
          child.expect(exp_result)
          score_pts += test[2]
        except pexpect.TIMEOUT:
          print(f"FAIL: Input '{inp}': Expected '{exp_result}'")
        except pexpect.EOF:
          print(f"FAIL (EOF): Input '{inp}': Expected '{exp_result}'")

    # check exit status
    child.close()

    total_pts += 2
    if (child.exitstatus == 0):
        score_pts += 2
    else:
        print(f"FAIL: plaidsh exited with status {child.exitstatus}")

    # check for memory leaks
    total_pts += 5
    if (child.before.find("memory leak") < 0):
        score_pts += 5
    else:
        print("FAIL: Memory leaks detected")

    print(f"Raw Score: {score_pts} out of {total_pts}")
    if (score_pts == total_pts):
        print("  ...plus 5 bonus points for passing all the tests!") #
        print(f"Total score: {score_pts+5} out of {total_pts+5}")
    else:
        print("TEST FAILURES EXIST")
        print(f"Total score: {score_pts} out of {total_pts+5}")


if (len(sys.argv) != 2):
    print(f"Usage: {sys.argv[0]} (executable-name)")
    sys.exit(1)

run_tests(sys.argv[1])
