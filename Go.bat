@echo off
xtimeq bash -c '"./Go" %*' 2>&1 | tee "_go" | tee -a "_go_a"
