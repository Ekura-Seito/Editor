# Microsoft Developer Studio Project File - Name="Editor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Editor - Win32 Release
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Editor.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Editor.mak" CFG="Editor - Win32 Release"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Editor - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "Editor - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Editor - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /Zi /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "AFX_TARG_ENU" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /profile /debug /machine:I386 /out:".\Release\Xerxes.exe"

!ELSEIF  "$(CFG)" == "Editor - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "AFX_TARG_ENU" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /profile /debug /machine:I386 /out:".\Debug\Xerxes.exe"

!ENDIF 

# Begin Target

# Name "Editor - Win32 Release"
# Name "Editor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoResizer.cpp
# End Source File
# Begin Source File

SOURCE=.\Base64.cpp
# End Source File
# Begin Source File

SOURCE=.\BasicAuthorizationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapClient.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckExecPasswdDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ClipBoardViewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeConvDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeConvManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeConvStatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DateMaskEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\DdeClient.cpp
# End Source File
# Begin Source File

SOURCE=.\DdeServer.cpp
# End Source File
# Begin Source File

SOURCE=.\DecriptDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DecriptStatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DirCompare.cpp
# End Source File
# Begin Source File

SOURCE=.\DirCompareDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DirCompareResultDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DirCompareStatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DirDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Editor.cpp
# End Source File
# Begin Source File

SOURCE=.\Editor_eng\Editor_eng.rc
# End Source File
# Begin Source File

SOURCE=.\EditorDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorView.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorView2.cpp
# End Source File
# Begin Source File

SOURCE=.\EncriptDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EncriptStatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExecPasswdDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileCompare.cpp
# End Source File
# Begin Source File

SOURCE=.\FileCompareResultDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileCompareStatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileCopyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDialogEx.cpp
# End Source File
# Begin Source File

SOURCE=.\FileMoveDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FilePropertyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileRenameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileSearchStatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileTouchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FindDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalData.cpp
# End Source File
# Begin Source File

SOURCE=.\GrepDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GrepStatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HtmlOpenDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HtmlOpenResultDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HtmlOpenStatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\http.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\KaigyoConvDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KaigyoConvStatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyBind.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyBindListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyBindManager.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyOpeDispDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\Line.cpp
# End Source File
# Begin Source File

SOURCE=.\LineJumpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LineManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\MailListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MailListStatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MailSendDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MailSendStatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkLineListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyComboBoxEx.cpp
# End Source File
# Begin Source File

SOURCE=.\MyComboBoxEx2.cpp
# End Source File
# Begin Source File

SOURCE=.\MyDes.cpp
# End Source File
# Begin Source File

SOURCE=.\Myfeal.cpp
# End Source File
# Begin Source File

SOURCE=.\Mymd5.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage1.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage10.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage11.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage12.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage2.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage3.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage4.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage5.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage6.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage7.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage8.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage9.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPropertySheet.cpp
# End Source File
# Begin Source File

SOURCE=.\MySha1.cpp
# End Source File
# Begin Source File

SOURCE=.\Operation.cpp
# End Source File
# Begin Source File

SOURCE=.\OperationManager.cpp
# End Source File
# Begin Source File

SOURCE=.\pop.cpp
# End Source File
# Begin Source File

SOURCE=.\QuotedPrintable.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\ReplaceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetLicenseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\smtp.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticData.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticFilespec.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TimeMaskEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarEx.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItem.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemAddCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemAl2Lower.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemAl2Upper.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemAlnumHan2Zen.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemAlnumZen2Han.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemBackSpaceChar.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemCutSelectArea.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemDelAsmComment.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemDelChar.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemDelCppComment.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemDelQuote.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemDelTopSpaces.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemDelVbComment.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemEditPaste.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemInsAsmComment.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemInsCppComment.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemInsDay.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemInsQuote.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemInsTime.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemInsVbComment.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemKatakanaHan2Zen.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemKatakanaZen2Han.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemOleDrop.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemReplaceSelectWord.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemReturn.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemShiftTabWhenSelected.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemSpace2Tab.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemSpaceHan2Zen.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemSpaceZen2Han.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemTab2Space.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoItemTabWhenSelected.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Uudecoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Uuencoder.cpp
# End Source File
# Begin Source File

SOURCE=.\WCntStatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WordCountDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\BasicAuthorizationDlg.h
# End Source File
# Begin Source File

SOURCE=.\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\BitmapClient.h
# End Source File
# Begin Source File

SOURCE=.\CheckExecPasswdDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ClipBoardViewDlg.h
# End Source File
# Begin Source File

SOURCE=.\CodeConvDlg.h
# End Source File
# Begin Source File

SOURCE=.\CodeConvManager.h
# End Source File
# Begin Source File

SOURCE=.\CodeConvStatusDlg.h
# End Source File
# Begin Source File

SOURCE=.\ColorButton.h
# End Source File
# Begin Source File

SOURCE=.\ColorManager.h
# End Source File
# Begin Source File

SOURCE=.\DecriptDlg.h
# End Source File
# Begin Source File

SOURCE=.\DecriptStatusDlg.h
# End Source File
# Begin Source File

SOURCE=.\DirCompareResultDlg.h
# End Source File
# Begin Source File

SOURCE=.\DirDialog.h
# End Source File
# Begin Source File

SOURCE=.\Editor.h
# End Source File
# Begin Source File

SOURCE=.\Editor_Def.h
# End Source File
# Begin Source File

SOURCE=.\EditorDoc.h
# End Source File
# Begin Source File

SOURCE=.\EditorView.h
# End Source File
# Begin Source File

SOURCE=.\EncriptStatusDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExecPasswdDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileCompareStatusDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileDialogEx.h
# End Source File
# Begin Source File

SOURCE=.\FilePropertyDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileSearchStatusDlg.h
# End Source File
# Begin Source File

SOURCE=.\FindDlg.h
# End Source File
# Begin Source File

SOURCE=.\GrepDlg.h
# End Source File
# Begin Source File

SOURCE=.\GrepStatusDlg.h
# End Source File
# Begin Source File

SOURCE=.\HtmlOpenDlg.h
# End Source File
# Begin Source File

SOURCE=.\HtmlOpenResultDlg.h
# End Source File
# Begin Source File

SOURCE=.\HtmlOpenStatusDlg.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\KaigyoConvDlg.h
# End Source File
# Begin Source File

SOURCE=.\KaigyoConvStatusDlg.h
# End Source File
# Begin Source File

SOURCE=.\KeyBind.h
# End Source File
# Begin Source File

SOURCE=.\KeyBindListDlg.h
# End Source File
# Begin Source File

SOURCE=.\KeyBindManager.h
# End Source File
# Begin Source File

SOURCE=.\KeyOpeDispDlg.h
# End Source File
# Begin Source File

SOURCE=.\Line.h
# End Source File
# Begin Source File

SOURCE=.\LineJumpDlg.h
# End Source File
# Begin Source File

SOURCE=.\LineManager.h
# End Source File
# Begin Source File

SOURCE=.\MailListDlg.h
# End Source File
# Begin Source File

SOURCE=.\MailListStatusDlg.h
# End Source File
# Begin Source File

SOURCE=.\MailSendDlg.h
# End Source File
# Begin Source File

SOURCE=.\MailSendStatusDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MarkLineListDlg.h
# End Source File
# Begin Source File

SOURCE=.\Myfeal.h
# End Source File
# Begin Source File

SOURCE=.\Mymd5.h
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage1.h
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage10.h
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage11.h
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage12.h
# End Source File
# Begin Source File

SOURCE=.\mypropertypage2.h
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage3.h
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage4.h
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage5.h
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage6.h
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage7.h
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage8.h
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage9.h
# End Source File
# Begin Source File

SOURCE=.\MyPropertySheet.h
# End Source File
# Begin Source File

SOURCE=.\Operation.h
# End Source File
# Begin Source File

SOURCE=.\OperationManager.h
# End Source File
# Begin Source File

SOURCE=.\ReplaceDlg.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SetLicenseDlg.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StaticFilespec.h
# End Source File
# Begin Source File

SOURCE=.\StatusBarDisp.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarEx.h
# End Source File
# Begin Source File

SOURCE=.\UndoItem.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemAddCharacter.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemAl2Lower.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemAl2Upper.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemAlnumHan2Zen.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemAlnumZen2Han.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemBackSpaceChar.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemCutSelectArea.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemDelChar.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemDelTopSpaces.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemEditPaste.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemInsAsmComment.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemInsCppComment.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemInsDay.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemInsTime.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemInsVbComment.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemKatakanaHan2Zen.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemKatakanaZen2Han.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemOleDrop.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemReplaceSelectWord.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemReturn.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemShiftTabWhenSelected.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemSpace2Tab.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemSpaceHan2Zen.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemSpaceZen2Han.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemTab2Space.h
# End Source File
# Begin Source File

SOURCE=.\UndoItemTabWhenSelected.h
# End Source File
# Begin Source File

SOURCE=.\UndoManager.h
# End Source File
# Begin Source File

SOURCE=.\Uudecoder.h
# End Source File
# Begin Source File

SOURCE=.\WCntStatusDlg.h
# End Source File
# Begin Source File

SOURCE=.\WordCountDlg.h
# End Source File
# Begin Source File

SOURCE=.\zmouse.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\auth.ico
# End Source File
# Begin Source File

SOURCE=.\res\bar_edit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bar_find.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bar_jump.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bar_tool.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bar_wnd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00012.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00013.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00014.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00015.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00016.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00017.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00018.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00019.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00020.bmp
# End Source File
# Begin Source File

SOURCE=.\res\brws_url.bmp
# End Source File
# Begin Source File

SOURCE=.\res\clipview.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\decrypt.bmp
# End Source File
# Begin Source File

SOURCE=.\res\edit_cli.bmp
# End Source File
# Begin Source File

SOURCE=.\res\edit_del.bmp
# End Source File
# Begin Source File

SOURCE=.\res\edit_fil.bmp
# End Source File
# Begin Source File

SOURCE=.\res\edit_ins.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Editor.rc2
# End Source File
# Begin Source File

SOURCE=.\res\encrypt.bmp
# End Source File
# Begin Source File

SOURCE=.\res\file_cmp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\file_toc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\ins_asm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ins_cpp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ins_day.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ins_tab.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ins_vb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\instime.bmp
# End Source File
# Begin Source File

SOURCE=.\res\jump_nex.bmp
# End Source File
# Begin Source File

SOURCE=.\res\jump_pre.bmp
# End Source File
# Begin Source File

SOURCE=.\res\jump_set.bmp
# End Source File
# Begin Source File

SOURCE=.\res\laugh.ico
# End Source File
# Begin Source File

SOURCE=.\res\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mailattach.bmp
# End Source File
# Begin Source File

SOURCE=.\res\security.bmp
# End Source File
# Begin Source File

SOURCE=.\res\setreado.bmp
# End Source File
# Begin Source File

SOURCE=.\res\smallico.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash.BMP
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\unreadon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\window_c.bmp
# End Source File
# Begin Source File

SOURCE=.\res\window_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\window_t.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wnd_acls.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wnd_arng.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wnd_casc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wnd_clos.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wnd_hrz.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wnd_new.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wnd_next.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wnd_prev.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wnd_splt.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wnd_vrt.bmp
# End Source File
# End Group
# End Target
# End Project
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
# Section Editor : {DF09B7FA-5DA8-11D1-8E0D-444553540000}
# 	1:17:CG_IDS_DISK_SPACE:110
# 	1:19:CG_IDS_PHYSICAL_MEM:109
# 	1:25:CG_IDS_DISK_SPACE_UNAVAIL:111
# 	2:10:SysInfoKey:1234
# End Section
