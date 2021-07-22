set VSVER=16
set VSTOOLSVER=142

@if     "%ProgramFiles(x86)%" == "" set UniProgramFiles=%ProgramFiles%\
@if not "%ProgramFiles(x86)%" == "" set UniProgramFiles=%ProgramFiles(x86)%\

call :GetVSCommonToolsDir

call "%VSCOMNTOOLS%VC\Auxiliary\Build\vcvars32.bat"

devenv NCadSDK.sln /Build "Debug NCAD|Win32"
devenv NCadSDK.sln /Build "Release NCAD|Win32"

devenv NCadSDK.sln /Build "Debug NCAD|x64"
devenv NCadSDK.sln /Build "Release NCAD|x64"
:ENDNCAD


if "%OARXROOT2021%"=="" goto ENDACAD

devenv NCADSDK.sln /Build "Debug ACAD|x64"
devenv NCADSDK.sln /Build "Release ACAD|x64"
:ENDACAD


if "%WIX%"=="" goto ENDWIX
devenv Installer\SamplesAppInstaller.sln /Build "Debug"
devenv Installer\SamplesAppInstaller.sln /Build "Release"
:ENDWIX

goto :EOF

:GetVSCommonToolsDir
@set VSCOMNTOOLS=
@set vswhere="%UniProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"
@for /f "usebackq delims=*" %%i in (`%vswhere% -version %VSVER% -property installationPath`) do (
  @if exist "%%i\Common7\Tools\vsdevcmd.bat" (
	@SET VSCOMNTOOLS=%%i\
	@exit /b 0
  )
)
@exit /B 1