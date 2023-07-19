@echo off
bash -c 'touch "_go_IncompleteBuild"; IMAGINARYPLAYER_STRING_VIEW_BOOST=1 IMAGINARYPLAYER_OPTIONAL_BOOST=1 "./Go" %*; if ((! "$?")); then echo "Build has completed."; rm "_go_IncompleteBuild"; fi' 2>&1 | tee "_go" | tee -a "_go_a"
