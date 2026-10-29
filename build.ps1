
param(

    [ValidateSet("10","11","12","13","14")]
    [string]$PyMinor = "12",

    [switch]$All,         # Clean + Configure + Build + Install + Package + Tests
    [switch]$FromScratch, # Clean + Configure + Build
    [switch]$BuildOnly,   # Build uniquement
    [switch]$InstallOnly, # Install uniquement
    [switch]$Package,     # Packaging .7z
    [switch]$Tests        # Tests CTest
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
$PSNativeCommandUseErrorActionPreference = $true

# --- LOCAL CONFIGURATION ------------------------------------------------------

$CondaEnvName = "CloudComPy3$PyMinor"
$CondaBase = "$Home/miniconda3"
$CondaRoot = "$CondaBase/envs/$CondaEnvName"
#$Qt6root = "C:/Qt/6.10.2/msvc2022_64"
$Qt6root = "$CondaRoot/Library"
$WorkRoot = "$Home/cloudComPy"
$SourceDir = "$WorkRoot/CloudComPy"
$BuildRoot = "$WorkRoot/build2026"
$InstallRoot = "$WorkRoot/install/$CondaEnvName"
$Configuration = "Release"
$BuildDir = "$BuildRoot/x64-$Configuration"
$corkDir = "$WorkRoot/cork"
$libiglDir = "$WorkRoot/libigl/libigl"
$fbxSdk = "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.9"
$PythonVenv = "$WorkRoot/venv3${PyMinor}doc"
$CloudCompareVersion = "2.14.beta"

# --- LOGGING & TIMING ---------------------------------------------------------

$Global:StepTimes = [ordered]@{}
$Global:LogFile = Join-Path $WorkRoot "build.log"

# Reset log file
if (Test-Path $Global:LogFile) { Remove-Item $Global:LogFile -Force }
New-Item -ItemType File -Path $Global:LogFile | Out-Null

function Write-Log($text) {
    Add-Content -Path $Global:LogFile -Value $text
}

function Write-Step($text, $color="Cyan") {
    [Console]::Out.Flush()
    Write-Host ""
    Write-Host $text -ForegroundColor $color
    Write-Log $text
}

function Start-Step($name) {
    $Global:StepTimes[$name] = [ordered]@{
        Start = Get-Date
        End   = $null
        Elapsed = $null
    }
    Write-Step "▶️  $name" "Yellow"
}

function End-Step($name) {
    $Global:StepTimes[$name].End = Get-Date
    $Global:StepTimes[$name].Elapsed = `
        ($Global:StepTimes[$name].End - $Global:StepTimes[$name].Start)

    $elapsed = $Global:StepTimes[$name].Elapsed
    $formatted = "{0:hh\:mm\:ss\.ff}" -f $elapsed
    $msg = "⏱️  Duration : $formatted"
    Write-Host $msg -ForegroundColor DarkGray
    Write-Log $msg
}

function Show-Summary {
    Write-Host ""
    Write-Host "======================" -ForegroundColor Cyan
    Write-Host "   BUILD SUMMARY      " -ForegroundColor Cyan
    Write-Host "======================" -ForegroundColor Cyan

    Write-Log ""
    Write-Log "======================"
    Write-Log "   BUILD SUMMARY      "
    Write-Log "======================"

    foreach ($k in $Global:StepTimes.Keys) {
        $t = $Global:StepTimes[$k].Elapsed
        $formatted = "{0:hh\:mm\:ss\.ff}" -f $t
        $line = ("{0,-20} {1}" -f $k, $formatted)
        Write-Host $line -ForegroundColor Green
        Write-Log $line
    }
}

# --- BUILD FUNCTIONS ----------------------------------------------------------

function Invoke-Clean {
    Start-Step "Clean"
    if (Test-Path $BuildDir) { Remove-Item -Recurse -Force $BuildDir }
    if (Test-Path $InstallRoot) { Remove-Item -Recurse -Force $InstallRoot }
    End-Step "Clean"
}

function Invoke-PrepareEnvironment {
    Start-Step "PrepareEnvironment"

    Clear-Host
    Write-Step "🐍 Conda Activation for Python 3.$PyMinor (Qt neutralized)" "Green"

    # Cleaning PATH / Qt Conda
    $env:PATH = ($env:PATH -split ';' | Where-Object { $_ -notmatch 'Library\\bin\\Qt' -and $_ -notmatch 'Library\\bin\\qt' }) -join ';'

    # Supress CMAKE_PREFIX_PATH from Conda
    Remove-Item Env:CMAKE_PREFIX_PATH -ErrorAction Ignore

    Write-Step "⚙️ MSVC Loading..." "Cyan"
    $vcvarsPath = "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
    $output = cmd /c "`"$vcvarsPath`" && set"

    $output | Select-String '^([^=]*)=(.*)$' | ForEach-Object {
        if ($_.Matches.Groups[1].Value -match '^(PATH|INCLUDE|LIB|LIBPATH|TMP|TEMP|CMAKE|C|CXX)') {
            Set-Item -Path "env:$($_.Matches.Groups[1].Value)" -Value $_.Matches.Groups[2].Value
        }
    }

    Write-Host "cl.exe: $(where.exe cl.exe 2>$null)" -ForegroundColor Green
    Write-Log "cl.exe: $(where.exe cl.exe 2>$null)"

    End-Step "PrepareEnvironment"
}

function Invoke-Configure {
    Start-Step "Configure"

    if (Test-Path $BuildDir) { Remove-Item -Recurse -Force $BuildDir }
    New-Item -ItemType Directory -Force $BuildDir | Out-Null

    Write-Step "🔧 CMake configure for Python 3.$PyMinor..." "Green"

    conda list > "$SourceDir\building\conda-list_Windows11_conda3$PyMinor"

    $cmakeArgs = @(
        "-S$SourceDir",
        "-B$BuildDir",
        "-G", "Ninja",
        "-DCMAKE_BUILD_TYPE=$Configuration",
        "-DCMAKE_SHARED_LINKER_FLAGS='/machine:x64 /FORCE:MULTIPLE'",
        "-DPYTHON_PREFERED_VERSION=3.$PyMinor",
        "-DPYMINOR=$PyMinor",
        "-DUSE_CONDA_PACKAGES=ON",
        "-DCLOUDCOMPARE_VERSION=$CloudCompareVersion",
        "-DCONDA_BASE_DIRECTORY=$CondaBase",
        "-DCONDA_ENV_NAME=$CondaEnvName",
        "-DCONDA_ROOT_DIRECTORY=$CondaRoot",
        "-DQt6_DIR=$Qt6root/lib/cmake/Qt6",
        "-DCMAKE_PREFIX_PATH=$Qt6root",
        "-DCMAKE_INSTALL_PREFIX=$InstallRoot",
        "-DBUILD_PYPI=1",
        "-DBUILD_PY_TESTING=1",
        "-DBUILD_REFERENCE_DOC=1",
        "-DBUILD_TESTING=1",
        "-DCCCORELIB_SHARED=1",
        "-DCCCORELIB_USE_CGAL=1",
        "-DCCCORELIB_USE_QT_CONCURRENT=1",
        "-DCCCORELIB_USE_TBB=0",
        "-DCORK_INCLUDE_DIR:PATH=$corkDir/src",
        "-DCORK_RELEASE_LIBRARY_FILE=$corkDir/win/VS2022/x64/Release/wincork2022.lib",
        "-DDRACO_INCLUDE_DIRS=$condaRoot/Library/include",
        "-DDRACO_LIBRARIES=$condaRoot/Library/lib/draco.lib",
        "-DEIGEN_ROOT_DIR=$condaRoot/Library/include/eigen3",
        "-DFBX_SDK_INCLUDE_DIR=$fbxSdk/include",
        "-DFBX_SDK_LIBRARY_FILE=$fbxSdk/lib/x64/release/libfbxsdk-md.lib",
        "-DFBX_XML2_LIBRARY_FILE=$fbxSdk/lib/x64/release/libxml2.lib",
        "-DFBX_ZLIB_LIBRARY_FILE=$fbxSdk/lib/x64/release/zlib.lib",
        "-DINSTALL_PREREQUISITE_LIBRARIES=0",
        "-DLIBIGL_INCLUDE_DIR=$libiglDir/include",
        "-DLIBIGL_RELEASE_LIBRARY_FILE=$libiglDir/out/install/x64-Release/lib/igl.lib",
        "-DMPIR_INCLUDE_DIR=$condaRoot/Library/include",
        "-DMPIR_RELEASE_LIBRARY_FILE=$condaRoot/Library/lib/mpir.lib",
        "-DOPENCASCADE_78_OR_NEWER=1",
        "-DOPENCASCADE_INC_DIR=$condaRoot/Library/include/opencascade",
        "-DOPENCASCADE_LIB_DIR=$condaRoot/Library/lib",
        "-DOPENCASCADE_DLL_DIR=$condaRoot/Library/bin",
        "-DOPENCASCADE_TBB_DLL_DIR=$condaRoot/Library/bin",
        "-DOPTION_BUILD_CCVIEWER=OFF",
        "-DOPTION_SUPPORT_3DMOUSE_WITH_3DxWARE=OFF",
        "-DOPTION_SUPPORT_3DMOUSE_WITH_HIDAPI=ON",
        "-DOPTION_HID_DEBUG=OFF",
        "-DOPTION_USE_GDAL=1",
        "-DPLUGIN_EXAMPLE_GL=1",
        "-DPLUGIN_EXAMPLE_IO=1",
        "-DPLUGIN_EXAMPLE_STANDARD=1",
        "-DPLUGIN_GL_QEDL=1",
        "-DPLUGIN_GL_QSSAO=1",
        "-DPLUGIN_IO_QADDITIONAL=1",
        "-DPLUGIN_IO_QCORE=1",
        "-DPLUGIN_IO_QCSV_MATRIX=1",
        "-DPLUGIN_IO_QDRACO=1",
        "-DPLUGIN_IO_QE57=1",
        "-DPLUGIN_IO_QFBX=1",
        "-DPLUGIN_IO_QLAS=1",
        "-DPLUGIN_IO_QLAS_FWF=0",
        "-DPLUGIN_IO_QPDAL=0",
        "-DPLUGIN_IO_QPHOTOSCAN=1",
        "-DPLUGIN_IO_QRDB=1",
        "-DPLUGIN_IO_QRDB_FETCH_DEPENDENCY=1",
        "-DPLUGIN_IO_QRDB_INSTALL_DEPENDENCY=1",
        "-DPLUGIN_IO_QSTEP=1",
        "-DPLUGIN_PYTHON=OFF",
        "-DPLUGIN_STANDARD_3DMASC=1",
        "-DPLUGIN_STANDARD_MASONRY_QAUTO_SEG=1",
        "-DPLUGIN_STANDARD_MASONRY_QMANUAL_SEG=1",
        "-DPLUGIN_STANDARD_QANIMATION=1",
        "-DPLUGIN_STANDARD_QBROOM=1",
        "-DPLUGIN_STANDARD_QCANUPO=1",
        "-DPLUGIN_STANDARD_QCLOUDLAYERS=1",
        "-DPLUGIN_STANDARD_QCOLORIMETRIC_SEGMENTER=1",
        "-DPLUGIN_STANDARD_QCOMPASS=1",
        "-DPLUGIN_STANDARD_QCORK=1",
        "-DPLUGIN_STANDARD_QCSF=1",
        "-DPLUGIN_STANDARD_QFACETS=1",
        "-DPLUGIN_STANDARD_QHOUGH_NORMALS=1",
        "-DPLUGIN_STANDARD_QHPR=1",
        "-DPLUGIN_STANDARD_QJSONRPC=1",
        "-DPLUGIN_STANDARD_QM3C2=1",
        "-DPLUGIN_STANDARD_QMESH_BOOLEAN=1",
        "-DPLUGIN_STANDARD_QMPLANE=1",
        "-DPLUGIN_STANDARD_QPCL=1",
        "-DPLUGIN_STANDARD_QPCV=1",
        "-DPLUGIN_STANDARD_QPOISSON_RECON=1",
        "-DPLUGIN_STANDARD_QRANSAC_SD=1",
        "-DPLUGIN_STANDARD_QSRA=1",
        "-DPLUGIN_STANDARD_QTREEISO=1",
        "-DPYTHONAPI_TEST_DIRECTORY=CloudComPy/Data",
        "-DPYTHONAPI_EXTDATA_DIRECTORY=CloudComPy/ExternalData",
        "-DPYTHONAPI_TRACES=1",
        "-DPYTHONVENV=$PythonVenv",
        "-DQANIMATION_WITH_FFMPEG_SUPPORT=1",
        "-DREPO_DIR=$SourceDir",
        "-DUSE_EXTERNAL_QHULL_FOR_QHPR=0",
        "-DWORKROOT=$WorkRoot"
    )

    cmake @cmakeArgs 2>&1 | Tee-Object -FilePath $Global:LogFile -Append

    End-Step "Configure"
}

function Invoke-Build {
    Start-Step "Build"
    Write-Step "🚀 Build..." "Green"

    Set-Location $BuildDir
    cmake --build . 2>&1 | Tee-Object -FilePath $Global:LogFile -Append
    Set-Location $SourceDir

    End-Step "Build"
}

function Invoke-Install {
    Start-Step "Install"
    Write-Step "📦 Install..." "Green"

    if (Test-Path $InstallRoot) { Remove-Item -Recurse -Force $InstallRoot }
    Set-Location $BuildDir
    cmake --install . 2>&1 | Tee-Object -FilePath $Global:LogFile -Append
    Set-Location $SourceDir

    End-Step "Install"
}

function Invoke-Package {
    Start-Step "Package"

    # Write-Step "📦 Packaging CloudComPy..." "Cyan"

    # $timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
    # $packageName = "CloudComPy_${CondaEnvName}_$timestamp.7z"
    # $packagePath = Join-Path "$WorkRoot/install" $packageName

    # $sevenZip = "C:\Program Files\7-Zip\7z.exe"

    # & $sevenZip a -t7z -m0=lzma2 -mx=9 -mmt=on -ms=on $packagePath $InstallRoot `
    #     2>&1 | Tee-Object -FilePath $Global:LogFile -Append

    # Write-Host "🎉 Package generated : $packagePath" -ForegroundColor Green
    # Write-Log "Package generated : $packagePath"

    Write-Host "📦 Building wheel..." -ForegroundColor Cyan

    # Nettoyage
    if (Test-Path dist) { Remove-Item dist -Recurse -Force }

    # Construction du wheel
    $venvActivate = Join-Path $PythonVenv "Scripts/Activate.ps1"
    & $venvActivate
    Copy-Item Windows/pyproject.toml pyproject.toml -Force
    Copy-Item Windows/setup.py setup.py -Force

    $Old = "CloudComPy312"
    $New = "CloudComPy3$PyMinor"

    $PyProject = Join-Path $SourceDir "pyproject.toml"

    (Get-Content $PyProject -Raw) `
        -replace $Old, $New |
        Set-Content $PyProject -Encoding UTF8

    python -m build --wheel

    # Vérification
    twine check dist\*.whl

    Write-Host "🎉 Wheel généré et vérifié !" -ForegroundColor Green

    Copy-Item dist\*.whl "$WorkRoot/install" -Force

    End-Step "Package"
}

function Invoke-Tests {
    Start-Step "Tests"
    Write-Step "🧪 Launching PythonAPI tests..." "Cyan"

    $venvActivate = Join-Path $PythonVenv "Scripts/Activate.ps1"
    & $venvActivate

    $envDump = cmd /c "call `"$InstallRoot\cloudComPy\envCloudComPy.bat`" && set" `
        2>&1 | Tee-Object -FilePath $Global:LogFile -Append

    foreach ($line in $envDump) {
        if ($line -match '^(.*?)=(.*)$') {
            Set-Item -Path Env:$($matches[1]) -Value $matches[2]
        }
    }

    Write-Host "=== PATH ==="
    $env:PATH -split ';'
    Write-Host "`n=== PYTHONPATH ==="
    $env:PYTHONPATH -split ';'

    Push-Location (Join-Path $InstallRoot "cloudComPy/doc/PythonAPI_test")
    ctest --output-on-failure 2>&1 | Tee-Object -FilePath $Global:LogFile -Append

    Pop-Location
    Pop-Location

    Write-Host "🧪 Tests completed." -ForegroundColor Green
    Write-Log "Tests completed."

    End-Step "Tests"
}

# --- MAIN LOGIC ---------------------------------------------------------------

Invoke-PrepareEnvironment

if ($All) {
    Invoke-Clean
    Invoke-Configure
    Invoke-Build
    Invoke-Install
    Invoke-Package
    Invoke-Tests
    Show-Summary
    exit
}

if ($FromScratch) {
    Invoke-Clean
    Invoke-Configure
    Invoke-Build
    Show-Summary
    exit
}

if ($BuildOnly) {
    Invoke-Build
    Show-Summary
    exit
}

if ($InstallOnly) {
    Invoke-Install
    Show-Summary
    exit
}

if ($Package) {
    Invoke-Package
    Show-Summary
    exit
}

if ($Tests) {
    Invoke-Tests
    Show-Summary
    exit
}

Write-Host "ℹ️ No switch provided. Use -All, -FromScratch, -BuildOnly, -InstallOnly, -Package or -Tests. Add also-PyMinor=10|11|12|13|14 to specify the Python version." -ForegroundColor Yellow
