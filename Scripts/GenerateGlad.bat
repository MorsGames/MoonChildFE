@echo off
setlocal

cd /d "%~dp0.."

python -m glad --generator=c --api "gl=2.1,gles2=2.0" --profile=compatibility --out-path Libraries/glad --reproducible --local-files --extensions=
