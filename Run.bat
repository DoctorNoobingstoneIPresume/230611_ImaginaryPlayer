@echo off

printf "Running...\n{\n"
if not exist "_Build/Main.exe"      goto Missing
if      exist "_go_IncompleteBuild" goto IncompleteBuild

xtimeq "_Build/Main.exe" "Input_Board.txt" "Input_Moves.txt" %* 2>&1 | tee "_run" | tee -a "_run_a"
printf "}\n\n"
goto :EOF

:Missing
printf "Missing \"_Build/Main.exe\" !\n"
goto :EOF

:IncompleteBuild
printf "Present \"_go_IncompleteBuild\" -- we might have a build error or a failing unittest !\n"
goto :EOF
