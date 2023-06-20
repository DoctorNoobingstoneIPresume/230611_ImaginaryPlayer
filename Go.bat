@echo off
bash -c 'touch "_go_IncompleteBuild"; "./Go" %*; if ((! "$?")); then echo "Build has completed."; rm "_go_IncompleteBuild"; fi' 2>&1 | tee "_go" | tee -a "_go_a"
