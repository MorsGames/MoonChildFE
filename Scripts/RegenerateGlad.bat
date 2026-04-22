@echo off
setlocal

cd /d "%~dp0.."

python -m glad --generator=c --api gles2=2.0 --out-path Libraries/glad --reproducible --local-files --extensions=
