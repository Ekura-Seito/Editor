# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Editor - Win32 Debug
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの Editor - Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "Editor - Win32 Release" && "$(CFG)" != "Editor - Win32 Debug"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛ 'CFG' を定義することによって
!MESSAGE NMAKE 実行時にﾋﾞﾙﾄﾞ ﾓｰﾄﾞを指定できます。例えば:
!MESSAGE 
!MESSAGE NMAKE /f "Editor.mak" CFG="Editor - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Editor - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "Editor - Win32 Debug" ("Win32 (x86) Application" 用)
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
# PROP Target_Last_Scanned "Editor - Win32 Debug"
CPP=cl.exe
MTL=mktyplib.exe
RSC=rc.exe

!IF  "$(CFG)" == "Editor - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\Xerxes.exe" "$(OUTDIR)\Editor.bsc"

CLEAN : 
	-@erase "$(INTDIR)\AboutDlg.obj"
	-@erase "$(INTDIR)\AboutDlg.sbr"
	-@erase "$(INTDIR)\Base64.obj"
	-@erase "$(INTDIR)\Base64.sbr"
	-@erase "$(INTDIR)\BCMenu.obj"
	-@erase "$(INTDIR)\BCMenu.sbr"
	-@erase "$(INTDIR)\BitmapClient.obj"
	-@erase "$(INTDIR)\BitmapClient.sbr"
	-@erase "$(INTDIR)\CheckExecPasswdDlg.obj"
	-@erase "$(INTDIR)\CheckExecPasswdDlg.sbr"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\ChildFrm.sbr"
	-@erase "$(INTDIR)\ClipBoardViewDlg.obj"
	-@erase "$(INTDIR)\ClipBoardViewDlg.sbr"
	-@erase "$(INTDIR)\CodeConvDlg.obj"
	-@erase "$(INTDIR)\CodeConvDlg.sbr"
	-@erase "$(INTDIR)\CodeConvManager.obj"
	-@erase "$(INTDIR)\CodeConvManager.sbr"
	-@erase "$(INTDIR)\CodeConvStatusDlg.obj"
	-@erase "$(INTDIR)\CodeConvStatusDlg.sbr"
	-@erase "$(INTDIR)\ColorButton.obj"
	-@erase "$(INTDIR)\ColorButton.sbr"
	-@erase "$(INTDIR)\ColorManager.obj"
	-@erase "$(INTDIR)\ColorManager.sbr"
	-@erase "$(INTDIR)\ComboBoxEx.obj"
	-@erase "$(INTDIR)\ComboBoxEx.sbr"
	-@erase "$(INTDIR)\ComboBoxEx2.obj"
	-@erase "$(INTDIR)\ComboBoxEx2.sbr"
	-@erase "$(INTDIR)\DateMaskEdit.obj"
	-@erase "$(INTDIR)\DateMaskEdit.sbr"
	-@erase "$(INTDIR)\DdeServer.obj"
	-@erase "$(INTDIR)\DdeServer.sbr"
	-@erase "$(INTDIR)\DecriptDlg.obj"
	-@erase "$(INTDIR)\DecriptDlg.sbr"
	-@erase "$(INTDIR)\DecriptStatusDlg.obj"
	-@erase "$(INTDIR)\DecriptStatusDlg.sbr"
	-@erase "$(INTDIR)\DirCompare.obj"
	-@erase "$(INTDIR)\DirCompare.sbr"
	-@erase "$(INTDIR)\DirCompareDlg.obj"
	-@erase "$(INTDIR)\DirCompareDlg.sbr"
	-@erase "$(INTDIR)\DirCompareResultDlg.obj"
	-@erase "$(INTDIR)\DirCompareResultDlg.sbr"
	-@erase "$(INTDIR)\DirCompareStatusDlg.obj"
	-@erase "$(INTDIR)\DirCompareStatusDlg.sbr"
	-@erase "$(INTDIR)\DirDialog.obj"
	-@erase "$(INTDIR)\DirDialog.sbr"
	-@erase "$(INTDIR)\Editor.obj"
	-@erase "$(INTDIR)\Editor.pch"
	-@erase "$(INTDIR)\Editor.res"
	-@erase "$(INTDIR)\Editor.sbr"
	-@erase "$(INTDIR)\EditorDoc.obj"
	-@erase "$(INTDIR)\EditorDoc.sbr"
	-@erase "$(INTDIR)\EditorView.obj"
	-@erase "$(INTDIR)\EditorView.sbr"
	-@erase "$(INTDIR)\EditorView2.obj"
	-@erase "$(INTDIR)\EditorView2.sbr"
	-@erase "$(INTDIR)\EncriptDlg.obj"
	-@erase "$(INTDIR)\EncriptDlg.sbr"
	-@erase "$(INTDIR)\EncriptStatusDlg.obj"
	-@erase "$(INTDIR)\EncriptStatusDlg.sbr"
	-@erase "$(INTDIR)\ExecPasswdDlg.obj"
	-@erase "$(INTDIR)\ExecPasswdDlg.sbr"
	-@erase "$(INTDIR)\FileCompare.obj"
	-@erase "$(INTDIR)\FileCompare.sbr"
	-@erase "$(INTDIR)\FileCompareResultDlg.obj"
	-@erase "$(INTDIR)\FileCompareResultDlg.sbr"
	-@erase "$(INTDIR)\FileCompareStatusDlg.obj"
	-@erase "$(INTDIR)\FileCompareStatusDlg.sbr"
	-@erase "$(INTDIR)\FileCopyDlg.obj"
	-@erase "$(INTDIR)\FileCopyDlg.sbr"
	-@erase "$(INTDIR)\FileMoveDlg.obj"
	-@erase "$(INTDIR)\FileMoveDlg.sbr"
	-@erase "$(INTDIR)\FilePropertyDlg.obj"
	-@erase "$(INTDIR)\FilePropertyDlg.sbr"
	-@erase "$(INTDIR)\FileRenameDlg.obj"
	-@erase "$(INTDIR)\FileRenameDlg.sbr"
	-@erase "$(INTDIR)\FileSearchDlg.obj"
	-@erase "$(INTDIR)\FileSearchDlg.sbr"
	-@erase "$(INTDIR)\FileSearchStatusDlg.obj"
	-@erase "$(INTDIR)\FileSearchStatusDlg.sbr"
	-@erase "$(INTDIR)\FileTouchDlg.obj"
	-@erase "$(INTDIR)\FileTouchDlg.sbr"
	-@erase "$(INTDIR)\FindDlg.obj"
	-@erase "$(INTDIR)\FindDlg.sbr"
	-@erase "$(INTDIR)\GrepDlg.obj"
	-@erase "$(INTDIR)\GrepDlg.sbr"
	-@erase "$(INTDIR)\GrepStatusDlg.obj"
	-@erase "$(INTDIR)\GrepStatusDlg.sbr"
	-@erase "$(INTDIR)\HyperLink.obj"
	-@erase "$(INTDIR)\HyperLink.sbr"
	-@erase "$(INTDIR)\KeyOpeDispDlg.obj"
	-@erase "$(INTDIR)\KeyOpeDispDlg.sbr"
	-@erase "$(INTDIR)\Line.obj"
	-@erase "$(INTDIR)\Line.sbr"
	-@erase "$(INTDIR)\LineJumpDlg.obj"
	-@erase "$(INTDIR)\LineJumpDlg.sbr"
	-@erase "$(INTDIR)\LineManager.obj"
	-@erase "$(INTDIR)\LineManager.sbr"
	-@erase "$(INTDIR)\ListCtrlEx.obj"
	-@erase "$(INTDIR)\ListCtrlEx.sbr"
	-@erase "$(INTDIR)\MailListDlg.obj"
	-@erase "$(INTDIR)\MailListDlg.sbr"
	-@erase "$(INTDIR)\MailListStatusDlg.obj"
	-@erase "$(INTDIR)\MailListStatusDlg.sbr"
	-@erase "$(INTDIR)\MailSendDlg.obj"
	-@erase "$(INTDIR)\MailSendDlg.sbr"
	-@erase "$(INTDIR)\MailSendStatusDlg.obj"
	-@erase "$(INTDIR)\MailSendStatusDlg.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\MarkLineListDlg.obj"
	-@erase "$(INTDIR)\MarkLineListDlg.sbr"
	-@erase "$(INTDIR)\MyDes.obj"
	-@erase "$(INTDIR)\MyDes.sbr"
	-@erase "$(INTDIR)\Myfeal.obj"
	-@erase "$(INTDIR)\Myfeal.sbr"
	-@erase "$(INTDIR)\Mymd5.obj"
	-@erase "$(INTDIR)\Mymd5.sbr"
	-@erase "$(INTDIR)\MyPropertyPage1.obj"
	-@erase "$(INTDIR)\MyPropertyPage1.sbr"
	-@erase "$(INTDIR)\MyPropertyPage2.obj"
	-@erase "$(INTDIR)\MyPropertyPage2.sbr"
	-@erase "$(INTDIR)\MyPropertyPage3.obj"
	-@erase "$(INTDIR)\MyPropertyPage3.sbr"
	-@erase "$(INTDIR)\MyPropertyPage4.obj"
	-@erase "$(INTDIR)\MyPropertyPage4.sbr"
	-@erase "$(INTDIR)\MyPropertyPage5.obj"
	-@erase "$(INTDIR)\MyPropertyPage5.sbr"
	-@erase "$(INTDIR)\MyPropertyPage6.obj"
	-@erase "$(INTDIR)\MyPropertyPage6.sbr"
	-@erase "$(INTDIR)\MyPropertyPage7.obj"
	-@erase "$(INTDIR)\MyPropertyPage7.sbr"
	-@erase "$(INTDIR)\MyPropertyPage8.obj"
	-@erase "$(INTDIR)\MyPropertyPage8.sbr"
	-@erase "$(INTDIR)\MyPropertyPage9.obj"
	-@erase "$(INTDIR)\MyPropertyPage9.sbr"
	-@erase "$(INTDIR)\MyPropertySheet.obj"
	-@erase "$(INTDIR)\MyPropertySheet.sbr"
	-@erase "$(INTDIR)\MySha1.obj"
	-@erase "$(INTDIR)\MySha1.sbr"
	-@erase "$(INTDIR)\Operation.obj"
	-@erase "$(INTDIR)\Operation.sbr"
	-@erase "$(INTDIR)\OperationManager.obj"
	-@erase "$(INTDIR)\OperationManager.sbr"
	-@erase "$(INTDIR)\pop.obj"
	-@erase "$(INTDIR)\pop.sbr"
	-@erase "$(INTDIR)\QuotedPrintable.obj"
	-@erase "$(INTDIR)\QuotedPrintable.sbr"
	-@erase "$(INTDIR)\ReplaceDlg.obj"
	-@erase "$(INTDIR)\ReplaceDlg.sbr"
	-@erase "$(INTDIR)\SetLicenseDlg.obj"
	-@erase "$(INTDIR)\SetLicenseDlg.sbr"
	-@erase "$(INTDIR)\smtp.obj"
	-@erase "$(INTDIR)\smtp.sbr"
	-@erase "$(INTDIR)\Splash.obj"
	-@erase "$(INTDIR)\Splash.sbr"
	-@erase "$(INTDIR)\StaticFilespec.obj"
	-@erase "$(INTDIR)\StaticFilespec.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TimeMaskEdit.obj"
	-@erase "$(INTDIR)\TimeMaskEdit.sbr"
	-@erase "$(INTDIR)\ToolBarEx.obj"
	-@erase "$(INTDIR)\ToolBarEx.sbr"
	-@erase "$(INTDIR)\UndoItem.obj"
	-@erase "$(INTDIR)\UndoItem.sbr"
	-@erase "$(INTDIR)\UndoItemAddCharacter.obj"
	-@erase "$(INTDIR)\UndoItemAddCharacter.sbr"
	-@erase "$(INTDIR)\UndoItemAl2Lower.obj"
	-@erase "$(INTDIR)\UndoItemAl2Lower.sbr"
	-@erase "$(INTDIR)\UndoItemAl2Upper.obj"
	-@erase "$(INTDIR)\UndoItemAl2Upper.sbr"
	-@erase "$(INTDIR)\UndoItemAlnumHan2Zen.obj"
	-@erase "$(INTDIR)\UndoItemAlnumHan2Zen.sbr"
	-@erase "$(INTDIR)\UndoItemAlnumZen2Han.obj"
	-@erase "$(INTDIR)\UndoItemAlnumZen2Han.sbr"
	-@erase "$(INTDIR)\UndoItemBackSpaceChar.obj"
	-@erase "$(INTDIR)\UndoItemBackSpaceChar.sbr"
	-@erase "$(INTDIR)\UndoItemCutSelectArea.obj"
	-@erase "$(INTDIR)\UndoItemCutSelectArea.sbr"
	-@erase "$(INTDIR)\UndoItemDelAsmComment.obj"
	-@erase "$(INTDIR)\UndoItemDelAsmComment.sbr"
	-@erase "$(INTDIR)\UndoItemDelChar.obj"
	-@erase "$(INTDIR)\UndoItemDelChar.sbr"
	-@erase "$(INTDIR)\UndoItemDelCppComment.obj"
	-@erase "$(INTDIR)\UndoItemDelCppComment.sbr"
	-@erase "$(INTDIR)\UndoItemDelQuote.obj"
	-@erase "$(INTDIR)\UndoItemDelQuote.sbr"
	-@erase "$(INTDIR)\UndoItemDelTopSpaces.obj"
	-@erase "$(INTDIR)\UndoItemDelTopSpaces.sbr"
	-@erase "$(INTDIR)\UndoItemDelVbComment.obj"
	-@erase "$(INTDIR)\UndoItemDelVbComment.sbr"
	-@erase "$(INTDIR)\UndoItemEditPaste.obj"
	-@erase "$(INTDIR)\UndoItemEditPaste.sbr"
	-@erase "$(INTDIR)\UndoItemInsAsmComment.obj"
	-@erase "$(INTDIR)\UndoItemInsAsmComment.sbr"
	-@erase "$(INTDIR)\UndoItemInsCppComment.obj"
	-@erase "$(INTDIR)\UndoItemInsCppComment.sbr"
	-@erase "$(INTDIR)\UndoItemInsDay.obj"
	-@erase "$(INTDIR)\UndoItemInsDay.sbr"
	-@erase "$(INTDIR)\UndoItemInsQuote.obj"
	-@erase "$(INTDIR)\UndoItemInsQuote.sbr"
	-@erase "$(INTDIR)\UndoItemInsTime.obj"
	-@erase "$(INTDIR)\UndoItemInsTime.sbr"
	-@erase "$(INTDIR)\UndoItemInsVbComment.obj"
	-@erase "$(INTDIR)\UndoItemInsVbComment.sbr"
	-@erase "$(INTDIR)\UndoItemKatakanaHan2Zen.obj"
	-@erase "$(INTDIR)\UndoItemKatakanaHan2Zen.sbr"
	-@erase "$(INTDIR)\UndoItemKatakanaZen2Han.obj"
	-@erase "$(INTDIR)\UndoItemKatakanaZen2Han.sbr"
	-@erase "$(INTDIR)\UndoItemOleDrop.obj"
	-@erase "$(INTDIR)\UndoItemOleDrop.sbr"
	-@erase "$(INTDIR)\UndoItemReplaceSelectWord.obj"
	-@erase "$(INTDIR)\UndoItemReplaceSelectWord.sbr"
	-@erase "$(INTDIR)\UndoItemReturn.obj"
	-@erase "$(INTDIR)\UndoItemReturn.sbr"
	-@erase "$(INTDIR)\UndoItemShiftTabWhenSelected.obj"
	-@erase "$(INTDIR)\UndoItemShiftTabWhenSelected.sbr"
	-@erase "$(INTDIR)\UndoItemSpace2Tab.obj"
	-@erase "$(INTDIR)\UndoItemSpace2Tab.sbr"
	-@erase "$(INTDIR)\UndoItemSpaceHan2Zen.obj"
	-@erase "$(INTDIR)\UndoItemSpaceHan2Zen.sbr"
	-@erase "$(INTDIR)\UndoItemSpaceZen2Han.obj"
	-@erase "$(INTDIR)\UndoItemSpaceZen2Han.sbr"
	-@erase "$(INTDIR)\UndoItemTab2Space.obj"
	-@erase "$(INTDIR)\UndoItemTab2Space.sbr"
	-@erase "$(INTDIR)\UndoItemTabWhenSelected.obj"
	-@erase "$(INTDIR)\UndoItemTabWhenSelected.sbr"
	-@erase "$(INTDIR)\UndoManager.obj"
	-@erase "$(INTDIR)\UndoManager.sbr"
	-@erase "$(INTDIR)\Uudecoder.obj"
	-@erase "$(INTDIR)\Uudecoder.sbr"
	-@erase "$(INTDIR)\Uuencoder.obj"
	-@erase "$(INTDIR)\Uuencoder.sbr"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\WCntStatusDlg.obj"
	-@erase "$(INTDIR)\WCntStatusDlg.sbr"
	-@erase "$(INTDIR)\WordCountDlg.obj"
	-@erase "$(INTDIR)\WordCountDlg.sbr"
	-@erase "$(OUTDIR)\Editor.bsc"
	-@erase "$(OUTDIR)\Xerxes.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /Zi /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /Zi /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Editor.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\Release/
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/Editor.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Editor.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AboutDlg.sbr" \
	"$(INTDIR)\Base64.sbr" \
	"$(INTDIR)\BCMenu.sbr" \
	"$(INTDIR)\BitmapClient.sbr" \
	"$(INTDIR)\CheckExecPasswdDlg.sbr" \
	"$(INTDIR)\ChildFrm.sbr" \
	"$(INTDIR)\ClipBoardViewDlg.sbr" \
	"$(INTDIR)\CodeConvDlg.sbr" \
	"$(INTDIR)\CodeConvManager.sbr" \
	"$(INTDIR)\CodeConvStatusDlg.sbr" \
	"$(INTDIR)\ColorButton.sbr" \
	"$(INTDIR)\ColorManager.sbr" \
	"$(INTDIR)\ComboBoxEx.sbr" \
	"$(INTDIR)\ComboBoxEx2.sbr" \
	"$(INTDIR)\DateMaskEdit.sbr" \
	"$(INTDIR)\DdeServer.sbr" \
	"$(INTDIR)\DecriptDlg.sbr" \
	"$(INTDIR)\DecriptStatusDlg.sbr" \
	"$(INTDIR)\DirCompare.sbr" \
	"$(INTDIR)\DirCompareDlg.sbr" \
	"$(INTDIR)\DirCompareResultDlg.sbr" \
	"$(INTDIR)\DirCompareStatusDlg.sbr" \
	"$(INTDIR)\DirDialog.sbr" \
	"$(INTDIR)\Editor.sbr" \
	"$(INTDIR)\EditorDoc.sbr" \
	"$(INTDIR)\EditorView.sbr" \
	"$(INTDIR)\EditorView2.sbr" \
	"$(INTDIR)\EncriptDlg.sbr" \
	"$(INTDIR)\EncriptStatusDlg.sbr" \
	"$(INTDIR)\ExecPasswdDlg.sbr" \
	"$(INTDIR)\FileCompare.sbr" \
	"$(INTDIR)\FileCompareResultDlg.sbr" \
	"$(INTDIR)\FileCompareStatusDlg.sbr" \
	"$(INTDIR)\FileCopyDlg.sbr" \
	"$(INTDIR)\FileMoveDlg.sbr" \
	"$(INTDIR)\FilePropertyDlg.sbr" \
	"$(INTDIR)\FileRenameDlg.sbr" \
	"$(INTDIR)\FileSearchDlg.sbr" \
	"$(INTDIR)\FileSearchStatusDlg.sbr" \
	"$(INTDIR)\FileTouchDlg.sbr" \
	"$(INTDIR)\FindDlg.sbr" \
	"$(INTDIR)\GrepDlg.sbr" \
	"$(INTDIR)\GrepStatusDlg.sbr" \
	"$(INTDIR)\HyperLink.sbr" \
	"$(INTDIR)\KeyOpeDispDlg.sbr" \
	"$(INTDIR)\Line.sbr" \
	"$(INTDIR)\LineJumpDlg.sbr" \
	"$(INTDIR)\LineManager.sbr" \
	"$(INTDIR)\ListCtrlEx.sbr" \
	"$(INTDIR)\MailListDlg.sbr" \
	"$(INTDIR)\MailListStatusDlg.sbr" \
	"$(INTDIR)\MailSendDlg.sbr" \
	"$(INTDIR)\MailSendStatusDlg.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\MarkLineListDlg.sbr" \
	"$(INTDIR)\MyDes.sbr" \
	"$(INTDIR)\Myfeal.sbr" \
	"$(INTDIR)\Mymd5.sbr" \
	"$(INTDIR)\MyPropertyPage1.sbr" \
	"$(INTDIR)\MyPropertyPage2.sbr" \
	"$(INTDIR)\MyPropertyPage3.sbr" \
	"$(INTDIR)\MyPropertyPage4.sbr" \
	"$(INTDIR)\MyPropertyPage5.sbr" \
	"$(INTDIR)\MyPropertyPage6.sbr" \
	"$(INTDIR)\MyPropertyPage7.sbr" \
	"$(INTDIR)\MyPropertyPage8.sbr" \
	"$(INTDIR)\MyPropertyPage9.sbr" \
	"$(INTDIR)\MyPropertySheet.sbr" \
	"$(INTDIR)\MySha1.sbr" \
	"$(INTDIR)\Operation.sbr" \
	"$(INTDIR)\OperationManager.sbr" \
	"$(INTDIR)\pop.sbr" \
	"$(INTDIR)\QuotedPrintable.sbr" \
	"$(INTDIR)\ReplaceDlg.sbr" \
	"$(INTDIR)\SetLicenseDlg.sbr" \
	"$(INTDIR)\smtp.sbr" \
	"$(INTDIR)\Splash.sbr" \
	"$(INTDIR)\StaticFilespec.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TimeMaskEdit.sbr" \
	"$(INTDIR)\ToolBarEx.sbr" \
	"$(INTDIR)\UndoItem.sbr" \
	"$(INTDIR)\UndoItemAddCharacter.sbr" \
	"$(INTDIR)\UndoItemAl2Lower.sbr" \
	"$(INTDIR)\UndoItemAl2Upper.sbr" \
	"$(INTDIR)\UndoItemAlnumHan2Zen.sbr" \
	"$(INTDIR)\UndoItemAlnumZen2Han.sbr" \
	"$(INTDIR)\UndoItemBackSpaceChar.sbr" \
	"$(INTDIR)\UndoItemCutSelectArea.sbr" \
	"$(INTDIR)\UndoItemDelAsmComment.sbr" \
	"$(INTDIR)\UndoItemDelChar.sbr" \
	"$(INTDIR)\UndoItemDelCppComment.sbr" \
	"$(INTDIR)\UndoItemDelQuote.sbr" \
	"$(INTDIR)\UndoItemDelTopSpaces.sbr" \
	"$(INTDIR)\UndoItemDelVbComment.sbr" \
	"$(INTDIR)\UndoItemEditPaste.sbr" \
	"$(INTDIR)\UndoItemInsAsmComment.sbr" \
	"$(INTDIR)\UndoItemInsCppComment.sbr" \
	"$(INTDIR)\UndoItemInsDay.sbr" \
	"$(INTDIR)\UndoItemInsQuote.sbr" \
	"$(INTDIR)\UndoItemInsTime.sbr" \
	"$(INTDIR)\UndoItemInsVbComment.sbr" \
	"$(INTDIR)\UndoItemKatakanaHan2Zen.sbr" \
	"$(INTDIR)\UndoItemKatakanaZen2Han.sbr" \
	"$(INTDIR)\UndoItemOleDrop.sbr" \
	"$(INTDIR)\UndoItemReplaceSelectWord.sbr" \
	"$(INTDIR)\UndoItemReturn.sbr" \
	"$(INTDIR)\UndoItemShiftTabWhenSelected.sbr" \
	"$(INTDIR)\UndoItemSpace2Tab.sbr" \
	"$(INTDIR)\UndoItemSpaceHan2Zen.sbr" \
	"$(INTDIR)\UndoItemSpaceZen2Han.sbr" \
	"$(INTDIR)\UndoItemTab2Space.sbr" \
	"$(INTDIR)\UndoItemTabWhenSelected.sbr" \
	"$(INTDIR)\UndoManager.sbr" \
	"$(INTDIR)\Uudecoder.sbr" \
	"$(INTDIR)\Uuencoder.sbr" \
	"$(INTDIR)\WCntStatusDlg.sbr" \
	"$(INTDIR)\WordCountDlg.sbr"

"$(OUTDIR)\Editor.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /profile /debug /machine:I386 /out:"Release/Xerxes.exe"
LINK32_FLAGS=/nologo /subsystem:windows /profile /debug /machine:I386\
 /out:"$(OUTDIR)/Xerxes.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AboutDlg.obj" \
	"$(INTDIR)\Base64.obj" \
	"$(INTDIR)\BCMenu.obj" \
	"$(INTDIR)\BitmapClient.obj" \
	"$(INTDIR)\CheckExecPasswdDlg.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\ClipBoardViewDlg.obj" \
	"$(INTDIR)\CodeConvDlg.obj" \
	"$(INTDIR)\CodeConvManager.obj" \
	"$(INTDIR)\CodeConvStatusDlg.obj" \
	"$(INTDIR)\ColorButton.obj" \
	"$(INTDIR)\ColorManager.obj" \
	"$(INTDIR)\ComboBoxEx.obj" \
	"$(INTDIR)\ComboBoxEx2.obj" \
	"$(INTDIR)\DateMaskEdit.obj" \
	"$(INTDIR)\DdeServer.obj" \
	"$(INTDIR)\DecriptDlg.obj" \
	"$(INTDIR)\DecriptStatusDlg.obj" \
	"$(INTDIR)\DirCompare.obj" \
	"$(INTDIR)\DirCompareDlg.obj" \
	"$(INTDIR)\DirCompareResultDlg.obj" \
	"$(INTDIR)\DirCompareStatusDlg.obj" \
	"$(INTDIR)\DirDialog.obj" \
	"$(INTDIR)\Editor.obj" \
	"$(INTDIR)\Editor.res" \
	"$(INTDIR)\EditorDoc.obj" \
	"$(INTDIR)\EditorView.obj" \
	"$(INTDIR)\EditorView2.obj" \
	"$(INTDIR)\EncriptDlg.obj" \
	"$(INTDIR)\EncriptStatusDlg.obj" \
	"$(INTDIR)\ExecPasswdDlg.obj" \
	"$(INTDIR)\FileCompare.obj" \
	"$(INTDIR)\FileCompareResultDlg.obj" \
	"$(INTDIR)\FileCompareStatusDlg.obj" \
	"$(INTDIR)\FileCopyDlg.obj" \
	"$(INTDIR)\FileMoveDlg.obj" \
	"$(INTDIR)\FilePropertyDlg.obj" \
	"$(INTDIR)\FileRenameDlg.obj" \
	"$(INTDIR)\FileSearchDlg.obj" \
	"$(INTDIR)\FileSearchStatusDlg.obj" \
	"$(INTDIR)\FileTouchDlg.obj" \
	"$(INTDIR)\FindDlg.obj" \
	"$(INTDIR)\GrepDlg.obj" \
	"$(INTDIR)\GrepStatusDlg.obj" \
	"$(INTDIR)\HyperLink.obj" \
	"$(INTDIR)\KeyOpeDispDlg.obj" \
	"$(INTDIR)\Line.obj" \
	"$(INTDIR)\LineJumpDlg.obj" \
	"$(INTDIR)\LineManager.obj" \
	"$(INTDIR)\ListCtrlEx.obj" \
	"$(INTDIR)\MailListDlg.obj" \
	"$(INTDIR)\MailListStatusDlg.obj" \
	"$(INTDIR)\MailSendDlg.obj" \
	"$(INTDIR)\MailSendStatusDlg.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MarkLineListDlg.obj" \
	"$(INTDIR)\MyDes.obj" \
	"$(INTDIR)\Myfeal.obj" \
	"$(INTDIR)\Mymd5.obj" \
	"$(INTDIR)\MyPropertyPage1.obj" \
	"$(INTDIR)\MyPropertyPage2.obj" \
	"$(INTDIR)\MyPropertyPage3.obj" \
	"$(INTDIR)\MyPropertyPage4.obj" \
	"$(INTDIR)\MyPropertyPage5.obj" \
	"$(INTDIR)\MyPropertyPage6.obj" \
	"$(INTDIR)\MyPropertyPage7.obj" \
	"$(INTDIR)\MyPropertyPage8.obj" \
	"$(INTDIR)\MyPropertyPage9.obj" \
	"$(INTDIR)\MyPropertySheet.obj" \
	"$(INTDIR)\MySha1.obj" \
	"$(INTDIR)\Operation.obj" \
	"$(INTDIR)\OperationManager.obj" \
	"$(INTDIR)\pop.obj" \
	"$(INTDIR)\QuotedPrintable.obj" \
	"$(INTDIR)\ReplaceDlg.obj" \
	"$(INTDIR)\SetLicenseDlg.obj" \
	"$(INTDIR)\smtp.obj" \
	"$(INTDIR)\Splash.obj" \
	"$(INTDIR)\StaticFilespec.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TimeMaskEdit.obj" \
	"$(INTDIR)\ToolBarEx.obj" \
	"$(INTDIR)\UndoItem.obj" \
	"$(INTDIR)\UndoItemAddCharacter.obj" \
	"$(INTDIR)\UndoItemAl2Lower.obj" \
	"$(INTDIR)\UndoItemAl2Upper.obj" \
	"$(INTDIR)\UndoItemAlnumHan2Zen.obj" \
	"$(INTDIR)\UndoItemAlnumZen2Han.obj" \
	"$(INTDIR)\UndoItemBackSpaceChar.obj" \
	"$(INTDIR)\UndoItemCutSelectArea.obj" \
	"$(INTDIR)\UndoItemDelAsmComment.obj" \
	"$(INTDIR)\UndoItemDelChar.obj" \
	"$(INTDIR)\UndoItemDelCppComment.obj" \
	"$(INTDIR)\UndoItemDelQuote.obj" \
	"$(INTDIR)\UndoItemDelTopSpaces.obj" \
	"$(INTDIR)\UndoItemDelVbComment.obj" \
	"$(INTDIR)\UndoItemEditPaste.obj" \
	"$(INTDIR)\UndoItemInsAsmComment.obj" \
	"$(INTDIR)\UndoItemInsCppComment.obj" \
	"$(INTDIR)\UndoItemInsDay.obj" \
	"$(INTDIR)\UndoItemInsQuote.obj" \
	"$(INTDIR)\UndoItemInsTime.obj" \
	"$(INTDIR)\UndoItemInsVbComment.obj" \
	"$(INTDIR)\UndoItemKatakanaHan2Zen.obj" \
	"$(INTDIR)\UndoItemKatakanaZen2Han.obj" \
	"$(INTDIR)\UndoItemOleDrop.obj" \
	"$(INTDIR)\UndoItemReplaceSelectWord.obj" \
	"$(INTDIR)\UndoItemReturn.obj" \
	"$(INTDIR)\UndoItemShiftTabWhenSelected.obj" \
	"$(INTDIR)\UndoItemSpace2Tab.obj" \
	"$(INTDIR)\UndoItemSpaceHan2Zen.obj" \
	"$(INTDIR)\UndoItemSpaceZen2Han.obj" \
	"$(INTDIR)\UndoItemTab2Space.obj" \
	"$(INTDIR)\UndoItemTabWhenSelected.obj" \
	"$(INTDIR)\UndoManager.obj" \
	"$(INTDIR)\Uudecoder.obj" \
	"$(INTDIR)\Uuencoder.obj" \
	"$(INTDIR)\WCntStatusDlg.obj" \
	"$(INTDIR)\WordCountDlg.obj"

"$(OUTDIR)\Xerxes.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Editor - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\Xerxes.exe" "$(OUTDIR)\Editor.bsc"

CLEAN : 
	-@erase "$(INTDIR)\AboutDlg.obj"
	-@erase "$(INTDIR)\AboutDlg.sbr"
	-@erase "$(INTDIR)\Base64.obj"
	-@erase "$(INTDIR)\Base64.sbr"
	-@erase "$(INTDIR)\BCMenu.obj"
	-@erase "$(INTDIR)\BCMenu.sbr"
	-@erase "$(INTDIR)\BitmapClient.obj"
	-@erase "$(INTDIR)\BitmapClient.sbr"
	-@erase "$(INTDIR)\CheckExecPasswdDlg.obj"
	-@erase "$(INTDIR)\CheckExecPasswdDlg.sbr"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\ChildFrm.sbr"
	-@erase "$(INTDIR)\ClipBoardViewDlg.obj"
	-@erase "$(INTDIR)\ClipBoardViewDlg.sbr"
	-@erase "$(INTDIR)\CodeConvDlg.obj"
	-@erase "$(INTDIR)\CodeConvDlg.sbr"
	-@erase "$(INTDIR)\CodeConvManager.obj"
	-@erase "$(INTDIR)\CodeConvManager.sbr"
	-@erase "$(INTDIR)\CodeConvStatusDlg.obj"
	-@erase "$(INTDIR)\CodeConvStatusDlg.sbr"
	-@erase "$(INTDIR)\ColorButton.obj"
	-@erase "$(INTDIR)\ColorButton.sbr"
	-@erase "$(INTDIR)\ColorManager.obj"
	-@erase "$(INTDIR)\ColorManager.sbr"
	-@erase "$(INTDIR)\ComboBoxEx.obj"
	-@erase "$(INTDIR)\ComboBoxEx.sbr"
	-@erase "$(INTDIR)\ComboBoxEx2.obj"
	-@erase "$(INTDIR)\ComboBoxEx2.sbr"
	-@erase "$(INTDIR)\DateMaskEdit.obj"
	-@erase "$(INTDIR)\DateMaskEdit.sbr"
	-@erase "$(INTDIR)\DdeServer.obj"
	-@erase "$(INTDIR)\DdeServer.sbr"
	-@erase "$(INTDIR)\DecriptDlg.obj"
	-@erase "$(INTDIR)\DecriptDlg.sbr"
	-@erase "$(INTDIR)\DecriptStatusDlg.obj"
	-@erase "$(INTDIR)\DecriptStatusDlg.sbr"
	-@erase "$(INTDIR)\DirCompare.obj"
	-@erase "$(INTDIR)\DirCompare.sbr"
	-@erase "$(INTDIR)\DirCompareDlg.obj"
	-@erase "$(INTDIR)\DirCompareDlg.sbr"
	-@erase "$(INTDIR)\DirCompareResultDlg.obj"
	-@erase "$(INTDIR)\DirCompareResultDlg.sbr"
	-@erase "$(INTDIR)\DirCompareStatusDlg.obj"
	-@erase "$(INTDIR)\DirCompareStatusDlg.sbr"
	-@erase "$(INTDIR)\DirDialog.obj"
	-@erase "$(INTDIR)\DirDialog.sbr"
	-@erase "$(INTDIR)\Editor.obj"
	-@erase "$(INTDIR)\Editor.pch"
	-@erase "$(INTDIR)\Editor.res"
	-@erase "$(INTDIR)\Editor.sbr"
	-@erase "$(INTDIR)\EditorDoc.obj"
	-@erase "$(INTDIR)\EditorDoc.sbr"
	-@erase "$(INTDIR)\EditorView.obj"
	-@erase "$(INTDIR)\EditorView.sbr"
	-@erase "$(INTDIR)\EditorView2.obj"
	-@erase "$(INTDIR)\EditorView2.sbr"
	-@erase "$(INTDIR)\EncriptDlg.obj"
	-@erase "$(INTDIR)\EncriptDlg.sbr"
	-@erase "$(INTDIR)\EncriptStatusDlg.obj"
	-@erase "$(INTDIR)\EncriptStatusDlg.sbr"
	-@erase "$(INTDIR)\ExecPasswdDlg.obj"
	-@erase "$(INTDIR)\ExecPasswdDlg.sbr"
	-@erase "$(INTDIR)\FileCompare.obj"
	-@erase "$(INTDIR)\FileCompare.sbr"
	-@erase "$(INTDIR)\FileCompareResultDlg.obj"
	-@erase "$(INTDIR)\FileCompareResultDlg.sbr"
	-@erase "$(INTDIR)\FileCompareStatusDlg.obj"
	-@erase "$(INTDIR)\FileCompareStatusDlg.sbr"
	-@erase "$(INTDIR)\FileCopyDlg.obj"
	-@erase "$(INTDIR)\FileCopyDlg.sbr"
	-@erase "$(INTDIR)\FileMoveDlg.obj"
	-@erase "$(INTDIR)\FileMoveDlg.sbr"
	-@erase "$(INTDIR)\FilePropertyDlg.obj"
	-@erase "$(INTDIR)\FilePropertyDlg.sbr"
	-@erase "$(INTDIR)\FileRenameDlg.obj"
	-@erase "$(INTDIR)\FileRenameDlg.sbr"
	-@erase "$(INTDIR)\FileSearchDlg.obj"
	-@erase "$(INTDIR)\FileSearchDlg.sbr"
	-@erase "$(INTDIR)\FileSearchStatusDlg.obj"
	-@erase "$(INTDIR)\FileSearchStatusDlg.sbr"
	-@erase "$(INTDIR)\FileTouchDlg.obj"
	-@erase "$(INTDIR)\FileTouchDlg.sbr"
	-@erase "$(INTDIR)\FindDlg.obj"
	-@erase "$(INTDIR)\FindDlg.sbr"
	-@erase "$(INTDIR)\GrepDlg.obj"
	-@erase "$(INTDIR)\GrepDlg.sbr"
	-@erase "$(INTDIR)\GrepStatusDlg.obj"
	-@erase "$(INTDIR)\GrepStatusDlg.sbr"
	-@erase "$(INTDIR)\HyperLink.obj"
	-@erase "$(INTDIR)\HyperLink.sbr"
	-@erase "$(INTDIR)\KeyOpeDispDlg.obj"
	-@erase "$(INTDIR)\KeyOpeDispDlg.sbr"
	-@erase "$(INTDIR)\Line.obj"
	-@erase "$(INTDIR)\Line.sbr"
	-@erase "$(INTDIR)\LineJumpDlg.obj"
	-@erase "$(INTDIR)\LineJumpDlg.sbr"
	-@erase "$(INTDIR)\LineManager.obj"
	-@erase "$(INTDIR)\LineManager.sbr"
	-@erase "$(INTDIR)\ListCtrlEx.obj"
	-@erase "$(INTDIR)\ListCtrlEx.sbr"
	-@erase "$(INTDIR)\MailListDlg.obj"
	-@erase "$(INTDIR)\MailListDlg.sbr"
	-@erase "$(INTDIR)\MailListStatusDlg.obj"
	-@erase "$(INTDIR)\MailListStatusDlg.sbr"
	-@erase "$(INTDIR)\MailSendDlg.obj"
	-@erase "$(INTDIR)\MailSendDlg.sbr"
	-@erase "$(INTDIR)\MailSendStatusDlg.obj"
	-@erase "$(INTDIR)\MailSendStatusDlg.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\MarkLineListDlg.obj"
	-@erase "$(INTDIR)\MarkLineListDlg.sbr"
	-@erase "$(INTDIR)\MyDes.obj"
	-@erase "$(INTDIR)\MyDes.sbr"
	-@erase "$(INTDIR)\Myfeal.obj"
	-@erase "$(INTDIR)\Myfeal.sbr"
	-@erase "$(INTDIR)\Mymd5.obj"
	-@erase "$(INTDIR)\Mymd5.sbr"
	-@erase "$(INTDIR)\MyPropertyPage1.obj"
	-@erase "$(INTDIR)\MyPropertyPage1.sbr"
	-@erase "$(INTDIR)\MyPropertyPage2.obj"
	-@erase "$(INTDIR)\MyPropertyPage2.sbr"
	-@erase "$(INTDIR)\MyPropertyPage3.obj"
	-@erase "$(INTDIR)\MyPropertyPage3.sbr"
	-@erase "$(INTDIR)\MyPropertyPage4.obj"
	-@erase "$(INTDIR)\MyPropertyPage4.sbr"
	-@erase "$(INTDIR)\MyPropertyPage5.obj"
	-@erase "$(INTDIR)\MyPropertyPage5.sbr"
	-@erase "$(INTDIR)\MyPropertyPage6.obj"
	-@erase "$(INTDIR)\MyPropertyPage6.sbr"
	-@erase "$(INTDIR)\MyPropertyPage7.obj"
	-@erase "$(INTDIR)\MyPropertyPage7.sbr"
	-@erase "$(INTDIR)\MyPropertyPage8.obj"
	-@erase "$(INTDIR)\MyPropertyPage8.sbr"
	-@erase "$(INTDIR)\MyPropertyPage9.obj"
	-@erase "$(INTDIR)\MyPropertyPage9.sbr"
	-@erase "$(INTDIR)\MyPropertySheet.obj"
	-@erase "$(INTDIR)\MyPropertySheet.sbr"
	-@erase "$(INTDIR)\MySha1.obj"
	-@erase "$(INTDIR)\MySha1.sbr"
	-@erase "$(INTDIR)\Operation.obj"
	-@erase "$(INTDIR)\Operation.sbr"
	-@erase "$(INTDIR)\OperationManager.obj"
	-@erase "$(INTDIR)\OperationManager.sbr"
	-@erase "$(INTDIR)\pop.obj"
	-@erase "$(INTDIR)\pop.sbr"
	-@erase "$(INTDIR)\QuotedPrintable.obj"
	-@erase "$(INTDIR)\QuotedPrintable.sbr"
	-@erase "$(INTDIR)\ReplaceDlg.obj"
	-@erase "$(INTDIR)\ReplaceDlg.sbr"
	-@erase "$(INTDIR)\SetLicenseDlg.obj"
	-@erase "$(INTDIR)\SetLicenseDlg.sbr"
	-@erase "$(INTDIR)\smtp.obj"
	-@erase "$(INTDIR)\smtp.sbr"
	-@erase "$(INTDIR)\Splash.obj"
	-@erase "$(INTDIR)\Splash.sbr"
	-@erase "$(INTDIR)\StaticFilespec.obj"
	-@erase "$(INTDIR)\StaticFilespec.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TimeMaskEdit.obj"
	-@erase "$(INTDIR)\TimeMaskEdit.sbr"
	-@erase "$(INTDIR)\ToolBarEx.obj"
	-@erase "$(INTDIR)\ToolBarEx.sbr"
	-@erase "$(INTDIR)\UndoItem.obj"
	-@erase "$(INTDIR)\UndoItem.sbr"
	-@erase "$(INTDIR)\UndoItemAddCharacter.obj"
	-@erase "$(INTDIR)\UndoItemAddCharacter.sbr"
	-@erase "$(INTDIR)\UndoItemAl2Lower.obj"
	-@erase "$(INTDIR)\UndoItemAl2Lower.sbr"
	-@erase "$(INTDIR)\UndoItemAl2Upper.obj"
	-@erase "$(INTDIR)\UndoItemAl2Upper.sbr"
	-@erase "$(INTDIR)\UndoItemAlnumHan2Zen.obj"
	-@erase "$(INTDIR)\UndoItemAlnumHan2Zen.sbr"
	-@erase "$(INTDIR)\UndoItemAlnumZen2Han.obj"
	-@erase "$(INTDIR)\UndoItemAlnumZen2Han.sbr"
	-@erase "$(INTDIR)\UndoItemBackSpaceChar.obj"
	-@erase "$(INTDIR)\UndoItemBackSpaceChar.sbr"
	-@erase "$(INTDIR)\UndoItemCutSelectArea.obj"
	-@erase "$(INTDIR)\UndoItemCutSelectArea.sbr"
	-@erase "$(INTDIR)\UndoItemDelAsmComment.obj"
	-@erase "$(INTDIR)\UndoItemDelAsmComment.sbr"
	-@erase "$(INTDIR)\UndoItemDelChar.obj"
	-@erase "$(INTDIR)\UndoItemDelChar.sbr"
	-@erase "$(INTDIR)\UndoItemDelCppComment.obj"
	-@erase "$(INTDIR)\UndoItemDelCppComment.sbr"
	-@erase "$(INTDIR)\UndoItemDelQuote.obj"
	-@erase "$(INTDIR)\UndoItemDelQuote.sbr"
	-@erase "$(INTDIR)\UndoItemDelTopSpaces.obj"
	-@erase "$(INTDIR)\UndoItemDelTopSpaces.sbr"
	-@erase "$(INTDIR)\UndoItemDelVbComment.obj"
	-@erase "$(INTDIR)\UndoItemDelVbComment.sbr"
	-@erase "$(INTDIR)\UndoItemEditPaste.obj"
	-@erase "$(INTDIR)\UndoItemEditPaste.sbr"
	-@erase "$(INTDIR)\UndoItemInsAsmComment.obj"
	-@erase "$(INTDIR)\UndoItemInsAsmComment.sbr"
	-@erase "$(INTDIR)\UndoItemInsCppComment.obj"
	-@erase "$(INTDIR)\UndoItemInsCppComment.sbr"
	-@erase "$(INTDIR)\UndoItemInsDay.obj"
	-@erase "$(INTDIR)\UndoItemInsDay.sbr"
	-@erase "$(INTDIR)\UndoItemInsQuote.obj"
	-@erase "$(INTDIR)\UndoItemInsQuote.sbr"
	-@erase "$(INTDIR)\UndoItemInsTime.obj"
	-@erase "$(INTDIR)\UndoItemInsTime.sbr"
	-@erase "$(INTDIR)\UndoItemInsVbComment.obj"
	-@erase "$(INTDIR)\UndoItemInsVbComment.sbr"
	-@erase "$(INTDIR)\UndoItemKatakanaHan2Zen.obj"
	-@erase "$(INTDIR)\UndoItemKatakanaHan2Zen.sbr"
	-@erase "$(INTDIR)\UndoItemKatakanaZen2Han.obj"
	-@erase "$(INTDIR)\UndoItemKatakanaZen2Han.sbr"
	-@erase "$(INTDIR)\UndoItemOleDrop.obj"
	-@erase "$(INTDIR)\UndoItemOleDrop.sbr"
	-@erase "$(INTDIR)\UndoItemReplaceSelectWord.obj"
	-@erase "$(INTDIR)\UndoItemReplaceSelectWord.sbr"
	-@erase "$(INTDIR)\UndoItemReturn.obj"
	-@erase "$(INTDIR)\UndoItemReturn.sbr"
	-@erase "$(INTDIR)\UndoItemShiftTabWhenSelected.obj"
	-@erase "$(INTDIR)\UndoItemShiftTabWhenSelected.sbr"
	-@erase "$(INTDIR)\UndoItemSpace2Tab.obj"
	-@erase "$(INTDIR)\UndoItemSpace2Tab.sbr"
	-@erase "$(INTDIR)\UndoItemSpaceHan2Zen.obj"
	-@erase "$(INTDIR)\UndoItemSpaceHan2Zen.sbr"
	-@erase "$(INTDIR)\UndoItemSpaceZen2Han.obj"
	-@erase "$(INTDIR)\UndoItemSpaceZen2Han.sbr"
	-@erase "$(INTDIR)\UndoItemTab2Space.obj"
	-@erase "$(INTDIR)\UndoItemTab2Space.sbr"
	-@erase "$(INTDIR)\UndoItemTabWhenSelected.obj"
	-@erase "$(INTDIR)\UndoItemTabWhenSelected.sbr"
	-@erase "$(INTDIR)\UndoManager.obj"
	-@erase "$(INTDIR)\UndoManager.sbr"
	-@erase "$(INTDIR)\Uudecoder.obj"
	-@erase "$(INTDIR)\Uudecoder.sbr"
	-@erase "$(INTDIR)\Uuencoder.obj"
	-@erase "$(INTDIR)\Uuencoder.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\WCntStatusDlg.obj"
	-@erase "$(INTDIR)\WCntStatusDlg.sbr"
	-@erase "$(INTDIR)\WordCountDlg.obj"
	-@erase "$(INTDIR)\WordCountDlg.sbr"
	-@erase "$(OUTDIR)\Editor.bsc"
	-@erase "$(OUTDIR)\Xerxes.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Editor.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/Editor.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Editor.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AboutDlg.sbr" \
	"$(INTDIR)\Base64.sbr" \
	"$(INTDIR)\BCMenu.sbr" \
	"$(INTDIR)\BitmapClient.sbr" \
	"$(INTDIR)\CheckExecPasswdDlg.sbr" \
	"$(INTDIR)\ChildFrm.sbr" \
	"$(INTDIR)\ClipBoardViewDlg.sbr" \
	"$(INTDIR)\CodeConvDlg.sbr" \
	"$(INTDIR)\CodeConvManager.sbr" \
	"$(INTDIR)\CodeConvStatusDlg.sbr" \
	"$(INTDIR)\ColorButton.sbr" \
	"$(INTDIR)\ColorManager.sbr" \
	"$(INTDIR)\ComboBoxEx.sbr" \
	"$(INTDIR)\ComboBoxEx2.sbr" \
	"$(INTDIR)\DateMaskEdit.sbr" \
	"$(INTDIR)\DdeServer.sbr" \
	"$(INTDIR)\DecriptDlg.sbr" \
	"$(INTDIR)\DecriptStatusDlg.sbr" \
	"$(INTDIR)\DirCompare.sbr" \
	"$(INTDIR)\DirCompareDlg.sbr" \
	"$(INTDIR)\DirCompareResultDlg.sbr" \
	"$(INTDIR)\DirCompareStatusDlg.sbr" \
	"$(INTDIR)\DirDialog.sbr" \
	"$(INTDIR)\Editor.sbr" \
	"$(INTDIR)\EditorDoc.sbr" \
	"$(INTDIR)\EditorView.sbr" \
	"$(INTDIR)\EditorView2.sbr" \
	"$(INTDIR)\EncriptDlg.sbr" \
	"$(INTDIR)\EncriptStatusDlg.sbr" \
	"$(INTDIR)\ExecPasswdDlg.sbr" \
	"$(INTDIR)\FileCompare.sbr" \
	"$(INTDIR)\FileCompareResultDlg.sbr" \
	"$(INTDIR)\FileCompareStatusDlg.sbr" \
	"$(INTDIR)\FileCopyDlg.sbr" \
	"$(INTDIR)\FileMoveDlg.sbr" \
	"$(INTDIR)\FilePropertyDlg.sbr" \
	"$(INTDIR)\FileRenameDlg.sbr" \
	"$(INTDIR)\FileSearchDlg.sbr" \
	"$(INTDIR)\FileSearchStatusDlg.sbr" \
	"$(INTDIR)\FileTouchDlg.sbr" \
	"$(INTDIR)\FindDlg.sbr" \
	"$(INTDIR)\GrepDlg.sbr" \
	"$(INTDIR)\GrepStatusDlg.sbr" \
	"$(INTDIR)\HyperLink.sbr" \
	"$(INTDIR)\KeyOpeDispDlg.sbr" \
	"$(INTDIR)\Line.sbr" \
	"$(INTDIR)\LineJumpDlg.sbr" \
	"$(INTDIR)\LineManager.sbr" \
	"$(INTDIR)\ListCtrlEx.sbr" \
	"$(INTDIR)\MailListDlg.sbr" \
	"$(INTDIR)\MailListStatusDlg.sbr" \
	"$(INTDIR)\MailSendDlg.sbr" \
	"$(INTDIR)\MailSendStatusDlg.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\MarkLineListDlg.sbr" \
	"$(INTDIR)\MyDes.sbr" \
	"$(INTDIR)\Myfeal.sbr" \
	"$(INTDIR)\Mymd5.sbr" \
	"$(INTDIR)\MyPropertyPage1.sbr" \
	"$(INTDIR)\MyPropertyPage2.sbr" \
	"$(INTDIR)\MyPropertyPage3.sbr" \
	"$(INTDIR)\MyPropertyPage4.sbr" \
	"$(INTDIR)\MyPropertyPage5.sbr" \
	"$(INTDIR)\MyPropertyPage6.sbr" \
	"$(INTDIR)\MyPropertyPage7.sbr" \
	"$(INTDIR)\MyPropertyPage8.sbr" \
	"$(INTDIR)\MyPropertyPage9.sbr" \
	"$(INTDIR)\MyPropertySheet.sbr" \
	"$(INTDIR)\MySha1.sbr" \
	"$(INTDIR)\Operation.sbr" \
	"$(INTDIR)\OperationManager.sbr" \
	"$(INTDIR)\pop.sbr" \
	"$(INTDIR)\QuotedPrintable.sbr" \
	"$(INTDIR)\ReplaceDlg.sbr" \
	"$(INTDIR)\SetLicenseDlg.sbr" \
	"$(INTDIR)\smtp.sbr" \
	"$(INTDIR)\Splash.sbr" \
	"$(INTDIR)\StaticFilespec.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TimeMaskEdit.sbr" \
	"$(INTDIR)\ToolBarEx.sbr" \
	"$(INTDIR)\UndoItem.sbr" \
	"$(INTDIR)\UndoItemAddCharacter.sbr" \
	"$(INTDIR)\UndoItemAl2Lower.sbr" \
	"$(INTDIR)\UndoItemAl2Upper.sbr" \
	"$(INTDIR)\UndoItemAlnumHan2Zen.sbr" \
	"$(INTDIR)\UndoItemAlnumZen2Han.sbr" \
	"$(INTDIR)\UndoItemBackSpaceChar.sbr" \
	"$(INTDIR)\UndoItemCutSelectArea.sbr" \
	"$(INTDIR)\UndoItemDelAsmComment.sbr" \
	"$(INTDIR)\UndoItemDelChar.sbr" \
	"$(INTDIR)\UndoItemDelCppComment.sbr" \
	"$(INTDIR)\UndoItemDelQuote.sbr" \
	"$(INTDIR)\UndoItemDelTopSpaces.sbr" \
	"$(INTDIR)\UndoItemDelVbComment.sbr" \
	"$(INTDIR)\UndoItemEditPaste.sbr" \
	"$(INTDIR)\UndoItemInsAsmComment.sbr" \
	"$(INTDIR)\UndoItemInsCppComment.sbr" \
	"$(INTDIR)\UndoItemInsDay.sbr" \
	"$(INTDIR)\UndoItemInsQuote.sbr" \
	"$(INTDIR)\UndoItemInsTime.sbr" \
	"$(INTDIR)\UndoItemInsVbComment.sbr" \
	"$(INTDIR)\UndoItemKatakanaHan2Zen.sbr" \
	"$(INTDIR)\UndoItemKatakanaZen2Han.sbr" \
	"$(INTDIR)\UndoItemOleDrop.sbr" \
	"$(INTDIR)\UndoItemReplaceSelectWord.sbr" \
	"$(INTDIR)\UndoItemReturn.sbr" \
	"$(INTDIR)\UndoItemShiftTabWhenSelected.sbr" \
	"$(INTDIR)\UndoItemSpace2Tab.sbr" \
	"$(INTDIR)\UndoItemSpaceHan2Zen.sbr" \
	"$(INTDIR)\UndoItemSpaceZen2Han.sbr" \
	"$(INTDIR)\UndoItemTab2Space.sbr" \
	"$(INTDIR)\UndoItemTabWhenSelected.sbr" \
	"$(INTDIR)\UndoManager.sbr" \
	"$(INTDIR)\Uudecoder.sbr" \
	"$(INTDIR)\Uuencoder.sbr" \
	"$(INTDIR)\WCntStatusDlg.sbr" \
	"$(INTDIR)\WordCountDlg.sbr"

"$(OUTDIR)\Editor.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /profile /debug /machine:I386 /out:"Debug/Xerxes.exe"
LINK32_FLAGS=/nologo /subsystem:windows /profile /debug /machine:I386\
 /out:"$(OUTDIR)/Xerxes.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AboutDlg.obj" \
	"$(INTDIR)\Base64.obj" \
	"$(INTDIR)\BCMenu.obj" \
	"$(INTDIR)\BitmapClient.obj" \
	"$(INTDIR)\CheckExecPasswdDlg.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\ClipBoardViewDlg.obj" \
	"$(INTDIR)\CodeConvDlg.obj" \
	"$(INTDIR)\CodeConvManager.obj" \
	"$(INTDIR)\CodeConvStatusDlg.obj" \
	"$(INTDIR)\ColorButton.obj" \
	"$(INTDIR)\ColorManager.obj" \
	"$(INTDIR)\ComboBoxEx.obj" \
	"$(INTDIR)\ComboBoxEx2.obj" \
	"$(INTDIR)\DateMaskEdit.obj" \
	"$(INTDIR)\DdeServer.obj" \
	"$(INTDIR)\DecriptDlg.obj" \
	"$(INTDIR)\DecriptStatusDlg.obj" \
	"$(INTDIR)\DirCompare.obj" \
	"$(INTDIR)\DirCompareDlg.obj" \
	"$(INTDIR)\DirCompareResultDlg.obj" \
	"$(INTDIR)\DirCompareStatusDlg.obj" \
	"$(INTDIR)\DirDialog.obj" \
	"$(INTDIR)\Editor.obj" \
	"$(INTDIR)\Editor.res" \
	"$(INTDIR)\EditorDoc.obj" \
	"$(INTDIR)\EditorView.obj" \
	"$(INTDIR)\EditorView2.obj" \
	"$(INTDIR)\EncriptDlg.obj" \
	"$(INTDIR)\EncriptStatusDlg.obj" \
	"$(INTDIR)\ExecPasswdDlg.obj" \
	"$(INTDIR)\FileCompare.obj" \
	"$(INTDIR)\FileCompareResultDlg.obj" \
	"$(INTDIR)\FileCompareStatusDlg.obj" \
	"$(INTDIR)\FileCopyDlg.obj" \
	"$(INTDIR)\FileMoveDlg.obj" \
	"$(INTDIR)\FilePropertyDlg.obj" \
	"$(INTDIR)\FileRenameDlg.obj" \
	"$(INTDIR)\FileSearchDlg.obj" \
	"$(INTDIR)\FileSearchStatusDlg.obj" \
	"$(INTDIR)\FileTouchDlg.obj" \
	"$(INTDIR)\FindDlg.obj" \
	"$(INTDIR)\GrepDlg.obj" \
	"$(INTDIR)\GrepStatusDlg.obj" \
	"$(INTDIR)\HyperLink.obj" \
	"$(INTDIR)\KeyOpeDispDlg.obj" \
	"$(INTDIR)\Line.obj" \
	"$(INTDIR)\LineJumpDlg.obj" \
	"$(INTDIR)\LineManager.obj" \
	"$(INTDIR)\ListCtrlEx.obj" \
	"$(INTDIR)\MailListDlg.obj" \
	"$(INTDIR)\MailListStatusDlg.obj" \
	"$(INTDIR)\MailSendDlg.obj" \
	"$(INTDIR)\MailSendStatusDlg.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MarkLineListDlg.obj" \
	"$(INTDIR)\MyDes.obj" \
	"$(INTDIR)\Myfeal.obj" \
	"$(INTDIR)\Mymd5.obj" \
	"$(INTDIR)\MyPropertyPage1.obj" \
	"$(INTDIR)\MyPropertyPage2.obj" \
	"$(INTDIR)\MyPropertyPage3.obj" \
	"$(INTDIR)\MyPropertyPage4.obj" \
	"$(INTDIR)\MyPropertyPage5.obj" \
	"$(INTDIR)\MyPropertyPage6.obj" \
	"$(INTDIR)\MyPropertyPage7.obj" \
	"$(INTDIR)\MyPropertyPage8.obj" \
	"$(INTDIR)\MyPropertyPage9.obj" \
	"$(INTDIR)\MyPropertySheet.obj" \
	"$(INTDIR)\MySha1.obj" \
	"$(INTDIR)\Operation.obj" \
	"$(INTDIR)\OperationManager.obj" \
	"$(INTDIR)\pop.obj" \
	"$(INTDIR)\QuotedPrintable.obj" \
	"$(INTDIR)\ReplaceDlg.obj" \
	"$(INTDIR)\SetLicenseDlg.obj" \
	"$(INTDIR)\smtp.obj" \
	"$(INTDIR)\Splash.obj" \
	"$(INTDIR)\StaticFilespec.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TimeMaskEdit.obj" \
	"$(INTDIR)\ToolBarEx.obj" \
	"$(INTDIR)\UndoItem.obj" \
	"$(INTDIR)\UndoItemAddCharacter.obj" \
	"$(INTDIR)\UndoItemAl2Lower.obj" \
	"$(INTDIR)\UndoItemAl2Upper.obj" \
	"$(INTDIR)\UndoItemAlnumHan2Zen.obj" \
	"$(INTDIR)\UndoItemAlnumZen2Han.obj" \
	"$(INTDIR)\UndoItemBackSpaceChar.obj" \
	"$(INTDIR)\UndoItemCutSelectArea.obj" \
	"$(INTDIR)\UndoItemDelAsmComment.obj" \
	"$(INTDIR)\UndoItemDelChar.obj" \
	"$(INTDIR)\UndoItemDelCppComment.obj" \
	"$(INTDIR)\UndoItemDelQuote.obj" \
	"$(INTDIR)\UndoItemDelTopSpaces.obj" \
	"$(INTDIR)\UndoItemDelVbComment.obj" \
	"$(INTDIR)\UndoItemEditPaste.obj" \
	"$(INTDIR)\UndoItemInsAsmComment.obj" \
	"$(INTDIR)\UndoItemInsCppComment.obj" \
	"$(INTDIR)\UndoItemInsDay.obj" \
	"$(INTDIR)\UndoItemInsQuote.obj" \
	"$(INTDIR)\UndoItemInsTime.obj" \
	"$(INTDIR)\UndoItemInsVbComment.obj" \
	"$(INTDIR)\UndoItemKatakanaHan2Zen.obj" \
	"$(INTDIR)\UndoItemKatakanaZen2Han.obj" \
	"$(INTDIR)\UndoItemOleDrop.obj" \
	"$(INTDIR)\UndoItemReplaceSelectWord.obj" \
	"$(INTDIR)\UndoItemReturn.obj" \
	"$(INTDIR)\UndoItemShiftTabWhenSelected.obj" \
	"$(INTDIR)\UndoItemSpace2Tab.obj" \
	"$(INTDIR)\UndoItemSpaceHan2Zen.obj" \
	"$(INTDIR)\UndoItemSpaceZen2Han.obj" \
	"$(INTDIR)\UndoItemTab2Space.obj" \
	"$(INTDIR)\UndoItemTabWhenSelected.obj" \
	"$(INTDIR)\UndoManager.obj" \
	"$(INTDIR)\Uudecoder.obj" \
	"$(INTDIR)\Uuencoder.obj" \
	"$(INTDIR)\WCntStatusDlg.obj" \
	"$(INTDIR)\WordCountDlg.obj"

"$(OUTDIR)\Xerxes.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "Editor - Win32 Release"
# Name "Editor - Win32 Debug"

!IF  "$(CFG)" == "Editor - Win32 Release"

!ELSEIF  "$(CFG)" == "Editor - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "Editor - Win32 Release"

!ELSEIF  "$(CFG)" == "Editor - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Editor.cpp
DEP_CPP_EDITO=\
	".\AboutDlg.h"\
	".\BCMenu.h"\
	".\BitmapClient.h"\
	".\CheckExecPasswdDlg.h"\
	".\ChildFrm.h"\
	".\ColorManager.h"\
	".\DdeServer.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\HyperLink.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\MainFrm.h"\
	".\Myfeal.h"\
	".\Mymd5.h"\
	".\Operation.h"\
	".\OperationManager.h"\
	".\SetLicenseDlg.h"\
	".\Splash.h"\
	".\StatusBarDisp.h"\
	".\StdAfx.h"\
	".\ToolBarEx.h"\
	".\UndoItem.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\Editor.obj" : $(SOURCE) $(DEP_CPP_EDITO) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\Editor.sbr" : $(SOURCE) $(DEP_CPP_EDITO) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "Editor - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /Zi /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Editor.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\StdAfx.sbr" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Editor.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Editor - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Editor.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\StdAfx.sbr" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Editor.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "Editor - Win32 Release"

DEP_CPP_MAINF=\
	".\Base64.h"\
	".\BCMenu.h"\
	".\BitmapClient.h"\
	".\ChildFrm.h"\
	".\ClipBoardViewDlg.h"\
	".\CodeConvDlg.h"\
	".\CodeConvManager.h"\
	".\CodeConvStatusDlg.h"\
	".\ColorButton.h"\
	".\ColorManager.h"\
	".\ComboBoxEx.h"\
	".\ComboBoxEx2.h"\
	".\DateMaskEdit.h"\
	".\DecriptDlg.h"\
	".\DecriptStatusDlg.h"\
	".\DirCompare.h"\
	".\DirCompareDlg.h"\
	".\DirCompareResultDlg.h"\
	".\DirCompareStatusDlg.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\EncriptDlg.h"\
	".\EncriptStatusDlg.h"\
	".\FileCompare.h"\
	".\FileCompareResultDlg.h"\
	".\FileCompareStatusDlg.h"\
	".\FilePropertyDlg.h"\
	".\FileSearchDlg.h"\
	".\FileSearchStatusDlg.h"\
	".\FileTouchDlg.h"\
	".\FindDlg.h"\
	".\GrepDlg.h"\
	".\GrepStatusDlg.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\ListCtrlEx.h"\
	".\MailListDlg.h"\
	".\MailListStatusDlg.h"\
	".\MainFrm.h"\
	".\MyDes.h"\
	".\MyPropertyPage1.h"\
	".\mypropertypage2.h"\
	".\MyPropertyPage3.h"\
	".\MyPropertyPage4.h"\
	".\MyPropertyPage5.h"\
	".\MyPropertyPage6.h"\
	".\MyPropertyPage7.h"\
	".\MyPropertyPage8.h"\
	".\MyPropertyPage9.h"\
	".\MyPropertySheet.h"\
	".\Operation.h"\
	".\OperationManager.h"\
	".\Pop.h"\
	".\QuotedPrintable.h"\
	".\ReplaceDlg.h"\
	".\Splash.h"\
	".\StaticFilespec.h"\
	".\StatusBarDisp.h"\
	".\StdAfx.h"\
	".\TimeMaskEdit.h"\
	".\ToolBarEx.h"\
	".\UndoItem.h"\
	".\UndoManager.h"\
	".\Uuencoder.h"\
	".\WCntStatusDlg.h"\
	".\WordCountDlg.h"\
	".\zmouse.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\MainFrm.sbr" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


!ELSEIF  "$(CFG)" == "Editor - Win32 Debug"

DEP_CPP_MAINF=\
	".\Base64.h"\
	".\BCMenu.h"\
	".\BitmapClient.h"\
	".\ChildFrm.h"\
	".\ClipBoardViewDlg.h"\
	".\CodeConvDlg.h"\
	".\CodeConvManager.h"\
	".\CodeConvStatusDlg.h"\
	".\ColorButton.h"\
	".\ColorManager.h"\
	".\ComboBoxEx.h"\
	".\ComboBoxEx2.h"\
	".\DateMaskEdit.h"\
	".\DecriptDlg.h"\
	".\DecriptStatusDlg.h"\
	".\DirCompare.h"\
	".\DirCompareDlg.h"\
	".\DirCompareResultDlg.h"\
	".\DirCompareStatusDlg.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\EncriptDlg.h"\
	".\EncriptStatusDlg.h"\
	".\FileCompare.h"\
	".\FileCompareResultDlg.h"\
	".\FileCompareStatusDlg.h"\
	".\FilePropertyDlg.h"\
	".\FileSearchDlg.h"\
	".\FileSearchStatusDlg.h"\
	".\FileTouchDlg.h"\
	".\FindDlg.h"\
	".\GrepDlg.h"\
	".\GrepStatusDlg.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\ListCtrlEx.h"\
	".\MailListDlg.h"\
	".\MailListStatusDlg.h"\
	".\MainFrm.h"\
	".\MyDes.h"\
	".\MyPropertyPage1.h"\
	".\mypropertypage2.h"\
	".\MyPropertyPage3.h"\
	".\MyPropertyPage4.h"\
	".\MyPropertyPage5.h"\
	".\MyPropertyPage6.h"\
	".\MyPropertyPage7.h"\
	".\MyPropertyPage8.h"\
	".\MyPropertyPage9.h"\
	".\MyPropertySheet.h"\
	".\Operation.h"\
	".\OperationManager.h"\
	".\Pop.h"\
	".\QuotedPrintable.h"\
	".\ReplaceDlg.h"\
	".\Splash.h"\
	".\StaticFilespec.h"\
	".\StatusBarDisp.h"\
	".\StdAfx.h"\
	".\TimeMaskEdit.h"\
	".\ToolBarEx.h"\
	".\UndoItem.h"\
	".\UndoManager.h"\
	".\Uuencoder.h"\
	".\WCntStatusDlg.h"\
	".\WordCountDlg.h"\
	".\zmouse.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\MainFrm.sbr" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ChildFrm.cpp
DEP_CPP_CHILD=\
	".\ChildFrm.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\ChildFrm.sbr" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EditorDoc.cpp
DEP_CPP_EDITOR=\
	".\Base64.h"\
	".\CodeConvManager.h"\
	".\ColorManager.h"\
	".\ComboBoxEx.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\FileCopyDlg.h"\
	".\FileMoveDlg.h"\
	".\FileRenameDlg.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\MailSendDlg.h"\
	".\MailSendStatusDlg.h"\
	".\QuotedPrintable.h"\
	".\smtp.h"\
	".\StaticFilespec.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemAddCharacter.h"\
	".\UndoItemAl2Lower.h"\
	".\UndoItemAl2Upper.h"\
	".\UndoItemAlnumHan2Zen.h"\
	".\UndoItemAlnumZen2Han.h"\
	".\UndoItemBackSpaceChar.h"\
	".\UndoItemCutSelectArea.h"\
	".\UndoItemDelAsmComment.h"\
	".\UndoItemDelChar.h"\
	".\UndoItemDelCppComment.h"\
	".\UndoItemDelQuote.h"\
	".\UndoItemDelTopSpaces.h"\
	".\UndoItemDelVbComment.h"\
	".\UndoItemEditPaste.h"\
	".\UndoItemInsAsmComment.h"\
	".\UndoItemInsCppComment.h"\
	".\UndoItemInsDay.h"\
	".\UndoItemInsQuote.h"\
	".\UndoItemInsTime.h"\
	".\UndoItemInsVbComment.h"\
	".\UndoItemKatakanaHan2Zen.h"\
	".\UndoItemKatakanaZen2Han.h"\
	".\UndoItemOleDrop.h"\
	".\UndoItemReplaceSelectWord.h"\
	".\UndoItemReturn.h"\
	".\UndoItemShiftTabWhenSelected.h"\
	".\UndoItemSpace2Tab.h"\
	".\UndoItemSpaceHan2Zen.h"\
	".\UndoItemSpaceZen2Han.h"\
	".\UndoItemTab2Space.h"\
	".\UndoItemTabWhenSelected.h"\
	".\UndoManager.h"\
	".\Uudecoder.h"\
	

"$(INTDIR)\EditorDoc.obj" : $(SOURCE) $(DEP_CPP_EDITOR) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\EditorDoc.sbr" : $(SOURCE) $(DEP_CPP_EDITOR) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EditorView.cpp
DEP_CPP_EDITORV=\
	".\BCMenu.h"\
	".\BitmapClient.h"\
	".\CodeConvManager.h"\
	".\ColorManager.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineJumpDlg.h"\
	".\LineManager.h"\
	".\MainFrm.h"\
	".\MarkLineListDlg.h"\
	".\Operation.h"\
	".\OperationManager.h"\
	".\StatusBarDisp.h"\
	".\StdAfx.h"\
	".\ToolBarEx.h"\
	".\UndoItem.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\EditorView.obj" : $(SOURCE) $(DEP_CPP_EDITORV) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\EditorView.sbr" : $(SOURCE) $(DEP_CPP_EDITORV) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Editor.rc
DEP_RSC_EDITOR_=\
	".\res\bmp00001.bmp"\
	".\res\bmp00002.bmp"\
	".\res\bmp00003.bmp"\
	".\res\bmp00004.bmp"\
	".\res\bmp00005.bmp"\
	".\res\bmp00006.bmp"\
	".\res\bmp00007.bmp"\
	".\res\bmp00008.bmp"\
	".\res\bmp00009.bmp"\
	".\res\bmp00010.bmp"\
	".\res\bmp00011.bmp"\
	".\res\bmp00012.bmp"\
	".\res\bmp00013.bmp"\
	".\res\bmp00014.bmp"\
	".\res\bmp00015.bmp"\
	".\res\bmp00016.bmp"\
	".\res\bmp00017.bmp"\
	".\res\bmp00018.bmp"\
	".\res\bmp00019.bmp"\
	".\res\bmp00020.bmp"\
	".\res\cursor1.cur"\
	".\res\edit_cli.bmp"\
	".\res\edit_del.bmp"\
	".\res\edit_fil.bmp"\
	".\res\edit_ins.bmp"\
	".\res\Editor.rc2"\
	".\res\icon1.ico"\
	".\res\idr_main.ico"\
	".\res\instime.bmp"\
	".\res\jump_nex.bmp"\
	".\res\jump_pre.bmp"\
	".\res\jump_set.bmp"\
	".\res\logo.bmp"\
	".\res\mailattach.bmp"\
	".\res\security.bmp"\
	".\res\setreado.bmp"\
	".\res\splash.BMP"\
	".\res\Toolbar.bmp"\
	".\res\toolbar1.bmp"\
	".\res\toolbar_.bmp"\
	".\res\window_c.bmp"\
	".\res\window_t.bmp"\
	".\res\wnd_next.bmp"\
	".\res\wnd_prev.bmp"\
	

"$(INTDIR)\Editor.res" : $(SOURCE) $(DEP_RSC_EDITOR_) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Line.cpp
DEP_CPP_LINE_=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\Line.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Line.obj" : $(SOURCE) $(DEP_CPP_LINE_) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\Line.sbr" : $(SOURCE) $(DEP_CPP_LINE_) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LineManager.cpp
DEP_CPP_LINEM=\
	".\BCMenu.h"\
	".\BitmapClient.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\MainFrm.h"\
	".\Operation.h"\
	".\OperationManager.h"\
	".\StatusBarDisp.h"\
	".\StdAfx.h"\
	".\ToolBarEx.h"\
	

"$(INTDIR)\LineManager.obj" : $(SOURCE) $(DEP_CPP_LINEM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\LineManager.sbr" : $(SOURCE) $(DEP_CPP_LINEM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoManager.cpp
DEP_CPP_UNDOM=\
	".\Editor_Def.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoManager.obj" : $(SOURCE) $(DEP_CPP_UNDOM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoManager.sbr" : $(SOURCE) $(DEP_CPP_UNDOM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemAddCharacter.cpp
DEP_CPP_UNDOI=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemAddCharacter.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemAddCharacter.obj" : $(SOURCE) $(DEP_CPP_UNDOI) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemAddCharacter.sbr" : $(SOURCE) $(DEP_CPP_UNDOI) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItem.cpp
DEP_CPP_UNDOIT=\
	".\Editor_Def.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	

"$(INTDIR)\UndoItem.obj" : $(SOURCE) $(DEP_CPP_UNDOIT) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItem.sbr" : $(SOURCE) $(DEP_CPP_UNDOIT) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MyPropertySheet.cpp
DEP_CPP_MYPRO=\
	".\BCMenu.h"\
	".\BitmapClient.h"\
	".\ColorButton.h"\
	".\ColorManager.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\MainFrm.h"\
	".\MyPropertyPage1.h"\
	".\mypropertypage2.h"\
	".\MyPropertyPage3.h"\
	".\MyPropertyPage4.h"\
	".\MyPropertyPage5.h"\
	".\MyPropertyPage6.h"\
	".\MyPropertyPage7.h"\
	".\MyPropertyPage8.h"\
	".\MyPropertyPage9.h"\
	".\MyPropertySheet.h"\
	".\Operation.h"\
	".\OperationManager.h"\
	".\StatusBarDisp.h"\
	".\StdAfx.h"\
	".\ToolBarEx.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\MyPropertySheet.obj" : $(SOURCE) $(DEP_CPP_MYPRO) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\MyPropertySheet.sbr" : $(SOURCE) $(DEP_CPP_MYPRO) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MyPropertyPage1.cpp
DEP_CPP_MYPROP=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\ExecPasswdDlg.h"\
	".\Mymd5.h"\
	".\MyPropertyPage1.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MyPropertyPage1.obj" : $(SOURCE) $(DEP_CPP_MYPROP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\MyPropertyPage1.sbr" : $(SOURCE) $(DEP_CPP_MYPROP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemDelChar.cpp
DEP_CPP_UNDOITE=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemDelChar.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemDelChar.obj" : $(SOURCE) $(DEP_CPP_UNDOITE) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemDelChar.sbr" : $(SOURCE) $(DEP_CPP_UNDOITE) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemReturn.cpp
DEP_CPP_UNDOITEM=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemReturn.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemReturn.obj" : $(SOURCE) $(DEP_CPP_UNDOITEM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemReturn.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemBackSpaceChar.cpp
DEP_CPP_UNDOITEMB=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemBackSpaceChar.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemBackSpaceChar.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMB)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemBackSpaceChar.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMB)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemCutSelectArea.cpp
DEP_CPP_UNDOITEMC=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemCutSelectArea.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemCutSelectArea.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMC)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemCutSelectArea.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMC)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ColorButton.cpp
DEP_CPP_COLOR=\
	".\ColorButton.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ColorButton.obj" : $(SOURCE) $(DEP_CPP_COLOR) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\ColorButton.sbr" : $(SOURCE) $(DEP_CPP_COLOR) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MyPropertyPage3.cpp
DEP_CPP_MYPROPE=\
	".\ColorButton.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\MyPropertyPage3.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MyPropertyPage3.obj" : $(SOURCE) $(DEP_CPP_MYPROPE) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\MyPropertyPage3.sbr" : $(SOURCE) $(DEP_CPP_MYPROPE) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemEditPaste.cpp
DEP_CPP_UNDOITEME=\
	".\ColorManager.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemEditPaste.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemEditPaste.obj" : $(SOURCE) $(DEP_CPP_UNDOITEME) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemEditPaste.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEME) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ColorManager.cpp
DEP_CPP_COLORM=\
	".\ColorManager.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\Line.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ColorManager.obj" : $(SOURCE) $(DEP_CPP_COLORM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\ColorManager.sbr" : $(SOURCE) $(DEP_CPP_COLORM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LineJumpDlg.cpp
DEP_CPP_LINEJ=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\LineJumpDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\LineJumpDlg.obj" : $(SOURCE) $(DEP_CPP_LINEJ) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\LineJumpDlg.sbr" : $(SOURCE) $(DEP_CPP_LINEJ) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemReplaceSelectWord.cpp
DEP_CPP_UNDOITEMR=\
	".\ColorManager.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemReplaceSelectWord.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemReplaceSelectWord.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMR)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemReplaceSelectWord.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMR)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemTabWhenSelected.cpp
DEP_CPP_UNDOITEMT=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemTabWhenSelected.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemTabWhenSelected.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMT)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemTabWhenSelected.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMT)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ToolBarEx.cpp
DEP_CPP_TOOLB=\
	".\StdAfx.h"\
	".\ToolBarEx.h"\
	

"$(INTDIR)\ToolBarEx.obj" : $(SOURCE) $(DEP_CPP_TOOLB) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\ToolBarEx.sbr" : $(SOURCE) $(DEP_CPP_TOOLB) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\BCMenu.cpp
DEP_CPP_BCMEN=\
	".\BCMenu.h"\
	".\BitmapClient.h"\
	".\Editor_Def.h"\
	".\MainFrm.h"\
	".\Operation.h"\
	".\OperationManager.h"\
	".\StatusBarDisp.h"\
	".\StdAfx.h"\
	".\ToolBarEx.h"\
	

"$(INTDIR)\BCMenu.obj" : $(SOURCE) $(DEP_CPP_BCMEN) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\BCMenu.sbr" : $(SOURCE) $(DEP_CPP_BCMEN) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DirDialog.cpp
DEP_CPP_DIRDI=\
	".\DirDialog.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DirDialog.obj" : $(SOURCE) $(DEP_CPP_DIRDI) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\DirDialog.sbr" : $(SOURCE) $(DEP_CPP_DIRDI) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\GrepDlg.cpp
DEP_CPP_GREPD=\
	".\ComboBoxEx.h"\
	".\DirDialog.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\GrepDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\GrepDlg.obj" : $(SOURCE) $(DEP_CPP_GREPD) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\GrepDlg.sbr" : $(SOURCE) $(DEP_CPP_GREPD) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\GrepStatusDlg.cpp
DEP_CPP_GREPS=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\GrepStatusDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\GrepStatusDlg.obj" : $(SOURCE) $(DEP_CPP_GREPS) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\GrepStatusDlg.sbr" : $(SOURCE) $(DEP_CPP_GREPS) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ComboBoxEx.cpp
DEP_CPP_COMBO=\
	".\ComboBoxEx.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ComboBoxEx.obj" : $(SOURCE) $(DEP_CPP_COMBO) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\ComboBoxEx.sbr" : $(SOURCE) $(DEP_CPP_COMBO) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\HyperLink.cpp
DEP_CPP_HYPER=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\HyperLink.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\HyperLink.obj" : $(SOURCE) $(DEP_CPP_HYPER) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\HyperLink.sbr" : $(SOURCE) $(DEP_CPP_HYPER) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\AboutDlg.cpp
DEP_CPP_ABOUT=\
	".\AboutDlg.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\HyperLink.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\AboutDlg.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\AboutDlg.sbr" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemTab2Space.cpp
DEP_CPP_UNDOITEMTA=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemTab2Space.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemTab2Space.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMTA) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemTab2Space.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMTA) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemSpace2Tab.cpp
DEP_CPP_UNDOITEMS=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemSpace2Tab.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemSpace2Tab.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMS) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemSpace2Tab.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMS) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemSpaceZen2Han.cpp
DEP_CPP_UNDOITEMSP=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemSpaceZen2Han.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemSpaceZen2Han.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMSP)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemSpaceZen2Han.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMSP)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemSpaceHan2Zen.cpp
DEP_CPP_UNDOITEMSPA=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemSpaceHan2Zen.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemSpaceHan2Zen.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMSPA)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemSpaceHan2Zen.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMSPA)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CodeConvDlg.cpp
DEP_CPP_CODEC=\
	".\CodeConvDlg.h"\
	".\ComboBoxEx2.h"\
	".\DirDialog.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CodeConvDlg.obj" : $(SOURCE) $(DEP_CPP_CODEC) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\CodeConvDlg.sbr" : $(SOURCE) $(DEP_CPP_CODEC) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ComboBoxEx2.cpp
DEP_CPP_COMBOB=\
	".\CodeConvDlg.h"\
	".\ComboBoxEx2.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ComboBoxEx2.obj" : $(SOURCE) $(DEP_CPP_COMBOB) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\ComboBoxEx2.sbr" : $(SOURCE) $(DEP_CPP_COMBOB) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CodeConvStatusDlg.cpp
DEP_CPP_CODECO=\
	".\CodeConvStatusDlg.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CodeConvStatusDlg.obj" : $(SOURCE) $(DEP_CPP_CODECO) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\CodeConvStatusDlg.sbr" : $(SOURCE) $(DEP_CPP_CODECO) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CodeConvManager.cpp
DEP_CPP_CODECON=\
	".\CodeConvManager.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CodeConvManager.obj" : $(SOURCE) $(DEP_CPP_CODECON) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\CodeConvManager.sbr" : $(SOURCE) $(DEP_CPP_CODECON) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MyPropertyPage4.cpp
DEP_CPP_MYPROPER=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\MyPropertyPage4.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MyPropertyPage4.obj" : $(SOURCE) $(DEP_CPP_MYPROPER) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\MyPropertyPage4.sbr" : $(SOURCE) $(DEP_CPP_MYPROPER) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MyPropertyPage5.cpp
DEP_CPP_MYPROPERT=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\MyPropertyPage5.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MyPropertyPage5.obj" : $(SOURCE) $(DEP_CPP_MYPROPERT) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\MyPropertyPage5.sbr" : $(SOURCE) $(DEP_CPP_MYPROPERT) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FilePropertyDlg.cpp
DEP_CPP_FILEP=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\FilePropertyDlg.h"\
	".\StaticFilespec.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FilePropertyDlg.obj" : $(SOURCE) $(DEP_CPP_FILEP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\FilePropertyDlg.sbr" : $(SOURCE) $(DEP_CPP_FILEP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemKatakanaHan2Zen.cpp
DEP_CPP_UNDOITEMK=\
	".\CodeConvManager.h"\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemKatakanaHan2Zen.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemKatakanaHan2Zen.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMK)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemKatakanaHan2Zen.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMK)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemKatakanaZen2Han.cpp
DEP_CPP_UNDOITEMKA=\
	".\CodeConvManager.h"\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemKatakanaZen2Han.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemKatakanaZen2Han.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMKA)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemKatakanaZen2Han.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMKA)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\WordCountDlg.cpp
DEP_CPP_WORDC=\
	".\DirDialog.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\StdAfx.h"\
	".\WordCountDlg.h"\
	

"$(INTDIR)\WordCountDlg.obj" : $(SOURCE) $(DEP_CPP_WORDC) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\WordCountDlg.sbr" : $(SOURCE) $(DEP_CPP_WORDC) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StaticFilespec.cpp
DEP_CPP_STATI=\
	".\StaticFilespec.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\StaticFilespec.obj" : $(SOURCE) $(DEP_CPP_STATI) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\StaticFilespec.sbr" : $(SOURCE) $(DEP_CPP_STATI) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MyPropertyPage2.cpp
DEP_CPP_MYPROPERTY=\
	".\Editor_Def.h"\
	".\mypropertypage2.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MyPropertyPage2.obj" : $(SOURCE) $(DEP_CPP_MYPROPERTY) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\MyPropertyPage2.sbr" : $(SOURCE) $(DEP_CPP_MYPROPERTY) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EditorView2.cpp
DEP_CPP_EDITORVI=\
	".\BCMenu.h"\
	".\BitmapClient.h"\
	".\ColorManager.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineJumpDlg.h"\
	".\LineManager.h"\
	".\MainFrm.h"\
	".\Operation.h"\
	".\OperationManager.h"\
	".\StatusBarDisp.h"\
	".\StdAfx.h"\
	".\ToolBarEx.h"\
	".\UndoItem.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\EditorView2.obj" : $(SOURCE) $(DEP_CPP_EDITORVI) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\EditorView2.sbr" : $(SOURCE) $(DEP_CPP_EDITORVI) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FileSearchDlg.cpp
DEP_CPP_FILES=\
	".\DirDialog.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\FileSearchDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FileSearchDlg.obj" : $(SOURCE) $(DEP_CPP_FILES) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\FileSearchDlg.sbr" : $(SOURCE) $(DEP_CPP_FILES) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FileSearchStatusDlg.cpp
DEP_CPP_FILESE=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\FileSearchStatusDlg.h"\
	".\StaticFilespec.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FileSearchStatusDlg.obj" : $(SOURCE) $(DEP_CPP_FILESE) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\FileSearchStatusDlg.sbr" : $(SOURCE) $(DEP_CPP_FILESE) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemAlnumZen2Han.cpp
DEP_CPP_UNDOITEMA=\
	".\CodeConvManager.h"\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemAlnumZen2Han.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemAlnumZen2Han.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMA)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemAlnumZen2Han.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMA)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemAlnumHan2Zen.cpp
DEP_CPP_UNDOITEMAL=\
	".\CodeConvManager.h"\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemAlnumHan2Zen.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemAlnumHan2Zen.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMAL)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemAlnumHan2Zen.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMAL)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MyPropertyPage6.cpp
DEP_CPP_MYPROPERTYP=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\MyPropertyPage6.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MyPropertyPage6.obj" : $(SOURCE) $(DEP_CPP_MYPROPERTYP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\MyPropertyPage6.sbr" : $(SOURCE) $(DEP_CPP_MYPROPERTYP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mymd5.cpp
DEP_CPP_MYMD5=\
	".\Mymd5.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Mymd5.obj" : $(SOURCE) $(DEP_CPP_MYMD5) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\Mymd5.sbr" : $(SOURCE) $(DEP_CPP_MYMD5) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SetLicenseDlg.cpp
DEP_CPP_SETLI=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\Myfeal.h"\
	".\Mymd5.h"\
	".\SetLicenseDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\SetLicenseDlg.obj" : $(SOURCE) $(DEP_CPP_SETLI) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\SetLicenseDlg.sbr" : $(SOURCE) $(DEP_CPP_SETLI) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemDelTopSpaces.cpp
DEP_CPP_UNDOITEMD=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemDelTopSpaces.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemDelTopSpaces.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMD)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemDelTopSpaces.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMD)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemShiftTabWhenSelected.cpp
DEP_CPP_UNDOITEMSH=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemShiftTabWhenSelected.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemShiftTabWhenSelected.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMSH)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemShiftTabWhenSelected.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMSH)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Myfeal.cpp
DEP_CPP_MYFEA=\
	".\Editor_Def.h"\
	".\Myfeal.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Myfeal.obj" : $(SOURCE) $(DEP_CPP_MYFEA) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\Myfeal.sbr" : $(SOURCE) $(DEP_CPP_MYFEA) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemOleDrop.cpp
DEP_CPP_UNDOITEMO=\
	".\ColorManager.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemOleDrop.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemOleDrop.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMO) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemOleDrop.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMO) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\BitmapClient.cpp
DEP_CPP_BITMA=\
	".\BitmapClient.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\BitmapClient.obj" : $(SOURCE) $(DEP_CPP_BITMA) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\BitmapClient.sbr" : $(SOURCE) $(DEP_CPP_BITMA) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Splash.cpp
DEP_CPP_SPLAS=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\Splash.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Splash.obj" : $(SOURCE) $(DEP_CPP_SPLAS) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\Splash.sbr" : $(SOURCE) $(DEP_CPP_SPLAS) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MyPropertyPage7.cpp
DEP_CPP_MYPROPERTYPA=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\MyPropertyPage7.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MyPropertyPage7.obj" : $(SOURCE) $(DEP_CPP_MYPROPERTYPA) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\MyPropertyPage7.sbr" : $(SOURCE) $(DEP_CPP_MYPROPERTYPA) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\WCntStatusDlg.cpp
DEP_CPP_WCNTS=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\StdAfx.h"\
	".\WCntStatusDlg.h"\
	

"$(INTDIR)\WCntStatusDlg.obj" : $(SOURCE) $(DEP_CPP_WCNTS) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\WCntStatusDlg.sbr" : $(SOURCE) $(DEP_CPP_WCNTS) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ClipBoardViewDlg.cpp
DEP_CPP_CLIPB=\
	".\ClipBoardViewDlg.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ClipBoardViewDlg.obj" : $(SOURCE) $(DEP_CPP_CLIPB) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\ClipBoardViewDlg.sbr" : $(SOURCE) $(DEP_CPP_CLIPB) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Operation.cpp
DEP_CPP_OPERA=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\Operation.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Operation.obj" : $(SOURCE) $(DEP_CPP_OPERA) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\Operation.sbr" : $(SOURCE) $(DEP_CPP_OPERA) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\OperationManager.cpp
DEP_CPP_OPERAT=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\KeyOpeDispDlg.h"\
	".\ListCtrlEx.h"\
	".\Operation.h"\
	".\OperationManager.h"\
	".\StaticFilespec.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\OperationManager.obj" : $(SOURCE) $(DEP_CPP_OPERAT) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\OperationManager.sbr" : $(SOURCE) $(DEP_CPP_OPERAT) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\KeyOpeDispDlg.cpp
DEP_CPP_KEYOP=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\KeyOpeDispDlg.h"\
	".\ListCtrlEx.h"\
	".\Operation.h"\
	".\OperationManager.h"\
	".\StaticFilespec.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\KeyOpeDispDlg.obj" : $(SOURCE) $(DEP_CPP_KEYOP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\KeyOpeDispDlg.sbr" : $(SOURCE) $(DEP_CPP_KEYOP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MyPropertyPage8.cpp
DEP_CPP_MYPROPERTYPAG=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\MyPropertyPage8.h"\
	".\StatusBarDisp.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MyPropertyPage8.obj" : $(SOURCE) $(DEP_CPP_MYPROPERTYPAG)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\MyPropertyPage8.sbr" : $(SOURCE) $(DEP_CPP_MYPROPERTYPAG)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemInsCppComment.cpp
DEP_CPP_UNDOITEMI=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemInsCppComment.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemInsCppComment.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMI)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemInsCppComment.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMI)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemInsVbComment.cpp
DEP_CPP_UNDOITEMIN=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemInsVbComment.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemInsVbComment.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMIN)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemInsVbComment.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMIN)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemInsAsmComment.cpp
DEP_CPP_UNDOITEMINS=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemInsAsmComment.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemInsAsmComment.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMINS)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemInsAsmComment.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMINS)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MarkLineListDlg.cpp
DEP_CPP_MARKL=\
	".\ColorManager.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\MarkLineListDlg.h"\
	".\StdAfx.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\MarkLineListDlg.obj" : $(SOURCE) $(DEP_CPP_MARKL) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\MarkLineListDlg.sbr" : $(SOURCE) $(DEP_CPP_MARKL) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemInsTime.cpp
DEP_CPP_UNDOITEMINST=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemInsTime.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemInsTime.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMINST) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemInsTime.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMINST) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemInsDay.cpp
DEP_CPP_UNDOITEMINSD=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemInsDay.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemInsDay.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMINSD) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemInsDay.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMINSD) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemAl2Upper.cpp
DEP_CPP_UNDOITEMAL2=\
	".\CodeConvManager.h"\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemAl2Upper.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemAl2Upper.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMAL2) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemAl2Upper.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMAL2) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemAl2Lower.cpp
DEP_CPP_UNDOITEMAL2L=\
	".\CodeConvManager.h"\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemAl2Lower.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemAl2Lower.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMAL2L)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemAl2Lower.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMAL2L)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Uudecoder.cpp
DEP_CPP_UUDEC=\
	".\StdAfx.h"\
	".\Uudecoder.h"\
	

"$(INTDIR)\Uudecoder.obj" : $(SOURCE) $(DEP_CPP_UUDEC) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\Uudecoder.sbr" : $(SOURCE) $(DEP_CPP_UUDEC) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\QuotedPrintable.cpp
DEP_CPP_QUOTE=\
	".\QuotedPrintable.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\QuotedPrintable.obj" : $(SOURCE) $(DEP_CPP_QUOTE) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\QuotedPrintable.sbr" : $(SOURCE) $(DEP_CPP_QUOTE) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Base64.cpp
DEP_CPP_BASE6=\
	".\Base64.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Base64.obj" : $(SOURCE) $(DEP_CPP_BASE6) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\Base64.sbr" : $(SOURCE) $(DEP_CPP_BASE6) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EncriptStatusDlg.cpp
DEP_CPP_ENCRI=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EncriptStatusDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\EncriptStatusDlg.obj" : $(SOURCE) $(DEP_CPP_ENCRI) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\EncriptStatusDlg.sbr" : $(SOURCE) $(DEP_CPP_ENCRI) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DecriptStatusDlg.cpp
DEP_CPP_DECRI=\
	".\DecriptStatusDlg.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DecriptStatusDlg.obj" : $(SOURCE) $(DEP_CPP_DECRI) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\DecriptStatusDlg.sbr" : $(SOURCE) $(DEP_CPP_DECRI) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EncriptDlg.cpp
DEP_CPP_ENCRIP=\
	".\DirDialog.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EncriptDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\EncriptDlg.obj" : $(SOURCE) $(DEP_CPP_ENCRIP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\EncriptDlg.sbr" : $(SOURCE) $(DEP_CPP_ENCRIP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DecriptDlg.cpp
DEP_CPP_DECRIP=\
	".\DecriptDlg.h"\
	".\DirDialog.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DecriptDlg.obj" : $(SOURCE) $(DEP_CPP_DECRIP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\DecriptDlg.sbr" : $(SOURCE) $(DEP_CPP_DECRIP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ExecPasswdDlg.cpp
DEP_CPP_EXECP=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\ExecPasswdDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ExecPasswdDlg.obj" : $(SOURCE) $(DEP_CPP_EXECP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\ExecPasswdDlg.sbr" : $(SOURCE) $(DEP_CPP_EXECP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CheckExecPasswdDlg.cpp
DEP_CPP_CHECK=\
	".\CheckExecPasswdDlg.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\Mymd5.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CheckExecPasswdDlg.obj" : $(SOURCE) $(DEP_CPP_CHECK) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\CheckExecPasswdDlg.sbr" : $(SOURCE) $(DEP_CPP_CHECK) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MySha1.cpp
DEP_CPP_MYSHA=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\MySha1.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MySha1.obj" : $(SOURCE) $(DEP_CPP_MYSHA) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\MySha1.sbr" : $(SOURCE) $(DEP_CPP_MYSHA) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MyDes.cpp
DEP_CPP_MYDES=\
	".\MyDes.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MyDes.obj" : $(SOURCE) $(DEP_CPP_MYDES) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\MyDes.sbr" : $(SOURCE) $(DEP_CPP_MYDES) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Uuencoder.cpp
DEP_CPP_UUENC=\
	".\StdAfx.h"\
	".\Uuencoder.h"\
	

"$(INTDIR)\Uuencoder.obj" : $(SOURCE) $(DEP_CPP_UUENC) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\Uuencoder.sbr" : $(SOURCE) $(DEP_CPP_UUENC) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FileCompare.cpp
DEP_CPP_FILEC=\
	".\ColorManager.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\FileCompare.h"\
	".\FileCompareStatusDlg.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StaticFilespec.h"\
	".\StdAfx.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\FileCompare.obj" : $(SOURCE) $(DEP_CPP_FILEC) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\FileCompare.sbr" : $(SOURCE) $(DEP_CPP_FILEC) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FileCompareStatusDlg.cpp
DEP_CPP_FILECO=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\FileCompareStatusDlg.h"\
	".\StaticFilespec.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FileCompareStatusDlg.obj" : $(SOURCE) $(DEP_CPP_FILECO) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\FileCompareStatusDlg.sbr" : $(SOURCE) $(DEP_CPP_FILECO) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FileCompareResultDlg.cpp
DEP_CPP_FILECOM=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\FileCompareResultDlg.h"\
	".\StaticFilespec.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FileCompareResultDlg.obj" : $(SOURCE) $(DEP_CPP_FILECOM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\FileCompareResultDlg.sbr" : $(SOURCE) $(DEP_CPP_FILECOM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DirCompareDlg.cpp
DEP_CPP_DIRCO=\
	".\DirCompareDlg.h"\
	".\DirDialog.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DirCompareDlg.obj" : $(SOURCE) $(DEP_CPP_DIRCO) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\DirCompareDlg.sbr" : $(SOURCE) $(DEP_CPP_DIRCO) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DirCompareStatusDlg.cpp
DEP_CPP_DIRCOM=\
	".\DirCompareStatusDlg.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\StaticFilespec.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DirCompareStatusDlg.obj" : $(SOURCE) $(DEP_CPP_DIRCOM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\DirCompareStatusDlg.sbr" : $(SOURCE) $(DEP_CPP_DIRCOM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DirCompare.cpp
DEP_CPP_DIRCOMP=\
	".\ColorManager.h"\
	".\DirCompare.h"\
	".\DirCompareStatusDlg.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StaticFilespec.h"\
	".\StdAfx.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\DirCompare.obj" : $(SOURCE) $(DEP_CPP_DIRCOMP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\DirCompare.sbr" : $(SOURCE) $(DEP_CPP_DIRCOMP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DirCompareResultDlg.cpp
DEP_CPP_DIRCOMPA=\
	".\DirCompareResultDlg.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\StaticFilespec.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DirCompareResultDlg.obj" : $(SOURCE) $(DEP_CPP_DIRCOMPA) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\DirCompareResultDlg.sbr" : $(SOURCE) $(DEP_CPP_DIRCOMPA) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DdeServer.cpp
DEP_CPP_DDESE=\
	".\BCMenu.h"\
	".\BitmapClient.h"\
	".\CodeConvManager.h"\
	".\ColorManager.h"\
	".\DdeServer.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\MainFrm.h"\
	".\Operation.h"\
	".\OperationManager.h"\
	".\StatusBarDisp.h"\
	".\StdAfx.h"\
	".\ToolBarEx.h"\
	

"$(INTDIR)\DdeServer.obj" : $(SOURCE) $(DEP_CPP_DDESE) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\DdeServer.sbr" : $(SOURCE) $(DEP_CPP_DDESE) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FileCopyDlg.cpp
DEP_CPP_FILECOP=\
	".\DirDialog.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\FileCopyDlg.h"\
	".\StaticFilespec.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FileCopyDlg.obj" : $(SOURCE) $(DEP_CPP_FILECOP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\FileCopyDlg.sbr" : $(SOURCE) $(DEP_CPP_FILECOP) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FileRenameDlg.cpp
DEP_CPP_FILER=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\FileRenameDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FileRenameDlg.obj" : $(SOURCE) $(DEP_CPP_FILER) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\FileRenameDlg.sbr" : $(SOURCE) $(DEP_CPP_FILER) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FileMoveDlg.cpp
DEP_CPP_FILEM=\
	".\DirDialog.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\FileMoveDlg.h"\
	".\StaticFilespec.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FileMoveDlg.obj" : $(SOURCE) $(DEP_CPP_FILEM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\FileMoveDlg.sbr" : $(SOURCE) $(DEP_CPP_FILEM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FileTouchDlg.cpp
DEP_CPP_FILET=\
	".\DateMaskEdit.h"\
	".\DirDialog.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\FileTouchDlg.h"\
	".\StdAfx.h"\
	".\TimeMaskEdit.h"\
	

"$(INTDIR)\FileTouchDlg.obj" : $(SOURCE) $(DEP_CPP_FILET) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\FileTouchDlg.sbr" : $(SOURCE) $(DEP_CPP_FILET) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DateMaskEdit.cpp
DEP_CPP_DATEM=\
	".\DateMaskEdit.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DateMaskEdit.obj" : $(SOURCE) $(DEP_CPP_DATEM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\DateMaskEdit.sbr" : $(SOURCE) $(DEP_CPP_DATEM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TimeMaskEdit.cpp
DEP_CPP_TIMEM=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\StdAfx.h"\
	".\TimeMaskEdit.h"\
	

"$(INTDIR)\TimeMaskEdit.obj" : $(SOURCE) $(DEP_CPP_TIMEM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\TimeMaskEdit.sbr" : $(SOURCE) $(DEP_CPP_TIMEM) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FindDlg.cpp
DEP_CPP_FINDD=\
	".\BCMenu.h"\
	".\BitmapClient.h"\
	".\ComboBoxEx.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EditorView.h"\
	".\FindDlg.h"\
	".\Line.h"\
	".\MainFrm.h"\
	".\OperationManager.h"\
	".\StatusBarDisp.h"\
	".\StdAfx.h"\
	".\ToolBarEx.h"\
	

"$(INTDIR)\FindDlg.obj" : $(SOURCE) $(DEP_CPP_FINDD) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\FindDlg.sbr" : $(SOURCE) $(DEP_CPP_FINDD) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ReplaceDlg.cpp
DEP_CPP_REPLA=\
	".\BCMenu.h"\
	".\BitmapClient.h"\
	".\ComboBoxEx.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\MainFrm.h"\
	".\OperationManager.h"\
	".\ReplaceDlg.h"\
	".\StatusBarDisp.h"\
	".\StdAfx.h"\
	".\ToolBarEx.h"\
	

"$(INTDIR)\ReplaceDlg.obj" : $(SOURCE) $(DEP_CPP_REPLA) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\ReplaceDlg.sbr" : $(SOURCE) $(DEP_CPP_REPLA) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MailSendStatusDlg.cpp
DEP_CPP_MAILS=\
	".\Editor.h"\
	".\MailSendStatusDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MailSendStatusDlg.obj" : $(SOURCE) $(DEP_CPP_MAILS) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\MailSendStatusDlg.sbr" : $(SOURCE) $(DEP_CPP_MAILS) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MailSendDlg.cpp
DEP_CPP_MAILSE=\
	".\ComboBoxEx.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\MailSendDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MailSendDlg.obj" : $(SOURCE) $(DEP_CPP_MAILSE) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\MailSendDlg.sbr" : $(SOURCE) $(DEP_CPP_MAILSE) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\smtp.cpp
DEP_CPP_SMTP_=\
	".\Base64.h"\
	".\CodeConvManager.h"\
	".\ColorManager.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\QuotedPrintable.h"\
	".\smtp.h"\
	".\StdAfx.h"\
	".\UndoManager.h"\
	".\Uuencoder.h"\
	

"$(INTDIR)\smtp.obj" : $(SOURCE) $(DEP_CPP_SMTP_) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\smtp.sbr" : $(SOURCE) $(DEP_CPP_SMTP_) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MyPropertyPage9.cpp
DEP_CPP_MYPROPERTYPAGE=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\MyPropertyPage9.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MyPropertyPage9.obj" : $(SOURCE) $(DEP_CPP_MYPROPERTYPAGE)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\MyPropertyPage9.sbr" : $(SOURCE) $(DEP_CPP_MYPROPERTYPAGE)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemInsQuote.cpp
DEP_CPP_UNDOITEMINSQ=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemInsQuote.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemInsQuote.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMINSQ)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemInsQuote.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMINSQ)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemDelCppComment.cpp
DEP_CPP_UNDOITEMDE=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemDelCppComment.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemDelCppComment.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMDE)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemDelCppComment.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMDE)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemDelVbComment.cpp
DEP_CPP_UNDOITEMDEL=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemDelVbComment.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemDelVbComment.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMDEL)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemDelVbComment.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMDEL)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemDelAsmComment.cpp
DEP_CPP_UNDOITEMDELA=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemDelAsmComment.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemDelAsmComment.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMDELA)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemDelAsmComment.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMDELA)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UndoItemDelQuote.cpp
DEP_CPP_UNDOITEMDELQ=\
	".\ColorManager.h"\
	".\Editor_Def.h"\
	".\EditorDoc.h"\
	".\EditorView.h"\
	".\Line.h"\
	".\LineManager.h"\
	".\StdAfx.h"\
	".\UndoItem.h"\
	".\UndoItemDelQuote.h"\
	".\UndoManager.h"\
	

"$(INTDIR)\UndoItemDelQuote.obj" : $(SOURCE) $(DEP_CPP_UNDOITEMDELQ)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"

"$(INTDIR)\UndoItemDelQuote.sbr" : $(SOURCE) $(DEP_CPP_UNDOITEMDELQ)\
 "$(INTDIR)" "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\pop.cpp
DEP_CPP_POP_C=\
	".\Base64.h"\
	".\CodeConvManager.h"\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\Pop.h"\
	".\QuotedPrintable.h"\
	".\StdAfx.h"\
	".\Uudecoder.h"\
	

"$(INTDIR)\pop.obj" : $(SOURCE) $(DEP_CPP_POP_C) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\pop.sbr" : $(SOURCE) $(DEP_CPP_POP_C) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MailListStatusDlg.cpp
DEP_CPP_MAILL=\
	".\Editor.h"\
	".\MailListStatusDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MailListStatusDlg.obj" : $(SOURCE) $(DEP_CPP_MAILL) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\MailListStatusDlg.sbr" : $(SOURCE) $(DEP_CPP_MAILL) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MailListDlg.cpp
DEP_CPP_MAILLI=\
	".\Editor.h"\
	".\Editor_Def.h"\
	".\ListCtrlEx.h"\
	".\MailListDlg.h"\
	".\Pop.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MailListDlg.obj" : $(SOURCE) $(DEP_CPP_MAILLI) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\MailListDlg.sbr" : $(SOURCE) $(DEP_CPP_MAILLI) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ListCtrlEx.cpp
DEP_CPP_LISTC=\
	".\ListCtrlEx.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ListCtrlEx.obj" : $(SOURCE) $(DEP_CPP_LISTC) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"

"$(INTDIR)\ListCtrlEx.sbr" : $(SOURCE) $(DEP_CPP_LISTC) "$(INTDIR)"\
 "$(INTDIR)\Editor.pch"


# End Source File
# End Target
# End Project
################################################################################
################################################################################
# Section Editor : {DF09B7FA-5DA8-11D1-8E0D-444553540000}
# 	1:17:CG_IDS_DISK_SPACE:110
# 	1:19:CG_IDS_PHYSICAL_MEM:109
# 	1:25:CG_IDS_DISK_SPACE_UNAVAIL:111
# 	2:10:SysInfoKey:1234
# End Section
################################################################################
################################################################################
# Section Editor : {DF09B7F7-5DA8-11D1-8E0D-444553540000}
# 	0:8:Splash.h:C:\Msdev\projects\Editor\Splash.h
# 	0:10:Splash.cpp:C:\Msdev\projects\Editor\Splash.cpp
# 	1:10:IDB_SPLASH:112
# 	2:10:ResHdrName:resource.h
# 	2:11:ProjHdrName:stdafx.h
# 	2:10:WrapperDef:_SPLASH_SCRN_
# 	2:12:SplClassName:CSplashWnd
# 	2:21:SplashScreenInsertKey:4.0
# 	2:10:HeaderName:Splash.h
# 	2:10:ImplemName:Splash.cpp
# 	2:7:BmpID16:IDB_SPLASH
# End Section
################################################################################
