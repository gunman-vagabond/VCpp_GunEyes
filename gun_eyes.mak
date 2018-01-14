# Microsoft Developer Studio Generated NMAKE File, Format Version 40001
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=gun_eyes - Win32 Debug
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの gun_eyes - Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "gun_eyes - Win32 Release" && "$(CFG)" !=\
 "gun_eyes - Win32 Debug"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛ 'CFG' を定義することによって
!MESSAGE NMAKE 実行時にﾋﾞﾙﾄﾞ ﾓｰﾄﾞを指定できます。例えば:
!MESSAGE 
!MESSAGE NMAKE /f "gun_eyes.mak" CFG="gun_eyes - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "gun_eyes - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "gun_eyes - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 
!ERROR 無効な構成が指定されています。
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "gun_eyes - Win32 Debug"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "gun_eyes - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\gun_eyes.exe"

CLEAN : 
	-@erase ".\Release\gun_eyes.exe"
	-@erase ".\Release\setupwin.obj"
	-@erase ".\Release\gun_eyes.obj"
	-@erase ".\Release\gun_eyes.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/gun_eyes.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/gun_eyes.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/gun_eyes.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ddraw.lib d3drm.lib winmm.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib ddraw.lib d3drm.lib winmm.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/gun_eyes.pdb" /machine:I386\
 /out:"$(OUTDIR)/gun_eyes.exe" 
LINK32_OBJS= \
	"$(INTDIR)/setupwin.obj" \
	"$(INTDIR)/gun_eyes.obj" \
	"$(INTDIR)/gun_eyes.res"

"$(OUTDIR)\gun_eyes.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "gun_eyes - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\gun_eyes.exe"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\gun_eyes.exe"
	-@erase ".\Debug\gun_eyes.obj"
	-@erase ".\Debug\setupwin.obj"
	-@erase ".\Debug\gun_eyes.res"
	-@erase ".\Debug\gun_eyes.ilk"
	-@erase ".\Debug\gun_eyes.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/gun_eyes.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/gun_eyes.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/gun_eyes.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ddraw.lib d3drm.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib ddraw.lib d3drm.lib winmm.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/gun_eyes.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/gun_eyes.exe" 
LINK32_OBJS= \
	"$(INTDIR)/gun_eyes.obj" \
	"$(INTDIR)/setupwin.obj" \
	"$(INTDIR)/gun_eyes.res"

"$(OUTDIR)\gun_eyes.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "gun_eyes - Win32 Release"
# Name "gun_eyes - Win32 Debug"

!IF  "$(CFG)" == "gun_eyes - Win32 Release"

!ELSEIF  "$(CFG)" == "gun_eyes - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\gun_eyes.cpp
DEP_CPP_GUN_E=\
	{$(INCLUDE)}"\d3drmwin.h"\
	".\setupwin.h"\
	{$(INCLUDE)}"\d3drm.h"\
	{$(INCLUDE)}"\ddraw.h"\
	{$(INCLUDE)}"\d3d.h"\
	{$(INCLUDE)}"\d3drmobj.h"\
	{$(INCLUDE)}"\d3drmdef.h"\
	{$(INCLUDE)}"\d3dtypes.h"\
	{$(INCLUDE)}"\d3dcaps.h"\
	
NODEP_CPP_GUN_E=\
	"d:\directx3\sdk\inc\subwtype.h"\
	"d:\directx3\sdk\inc\d3dcom.h"\
	

"$(INTDIR)\gun_eyes.obj" : $(SOURCE) $(DEP_CPP_GUN_E) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\setupwin.cpp
DEP_CPP_SETUP=\
	{$(INCLUDE)}"\d3drmwin.h"\
	".\setupwin.h"\
	{$(INCLUDE)}"\d3drm.h"\
	{$(INCLUDE)}"\ddraw.h"\
	{$(INCLUDE)}"\d3d.h"\
	{$(INCLUDE)}"\d3drmobj.h"\
	{$(INCLUDE)}"\d3drmdef.h"\
	{$(INCLUDE)}"\d3dtypes.h"\
	{$(INCLUDE)}"\d3dcaps.h"\
	
NODEP_CPP_SETUP=\
	"d:\directx3\sdk\inc\subwtype.h"\
	"d:\directx3\sdk\inc\d3dcom.h"\
	

"$(INTDIR)\setupwin.obj" : $(SOURCE) $(DEP_CPP_SETUP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\setupwin.h

!IF  "$(CFG)" == "gun_eyes - Win32 Release"

!ELSEIF  "$(CFG)" == "gun_eyes - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\gun_eyes.rc
DEP_RSC_GUN_EY=\
	".\icon1.ico"\
	

"$(INTDIR)\gun_eyes.res" : $(SOURCE) $(DEP_RSC_GUN_EY) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
