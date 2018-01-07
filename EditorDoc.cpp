// EditorDoc.cpp : CEditorDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include <Afxtempl.h>			//�l�e�b�̂b�l�������g�p����
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include <direct.h>
#include <io.h>					//for _access
#include "Editor_Def.h"
#include "ColorManager.h"
#include "Editor.h"
#include "Line.h"
#include "LineManager.h"
#include "UndoItem.h"
#include "UndoItemAddCharacter.h"
#include "UndoItemDelChar.h"
#include "UndoItemReturn.h"
#include "UndoItemBackSpaceChar.h"
#include "UndoItemCutSelectArea.h"
#include "UndoItemEditPaste.h"
#include "UndoItemReplaceSelectWord.h"
#include "UndoItemTabWhenSelected.h"
#include "UndoItemShiftTabWhenSelected.h"
#include "UndoItemDelTopSpaces.h"
#include "UndoItemTab2Space.h"
#include "UndoItemSpace2Tab.h"
#include "UndoItemSpaceZen2Han.h"
#include "UndoItemSpaceHan2Zen.h"
#include "UndoItemKatakanaHan2Zen.h"
#include "UndoItemKatakanaZen2Han.h"
#include "UndoItemAlnumHan2Zen.h"
#include "UndoItemAlnumZen2Han.h"
#include "UndoItemOleDrop.h"
#include "UndoItemInsCppComment.h"
#include "UndoItemDelCppComment.h"
#include "UndoItemInsVbComment.h"
#include "UndoItemDelVbComment.h"
#include "UndoItemInsAsmComment.h"
#include "UndoItemDelAsmComment.h"
#include "UndoItemInsQuote.h"
#include "UndoItemDelQuote.h"
#include "UndoItemInsDay.h"
#include "UndoItemInsTime.h"
#include "UndoItemAl2Upper.h"
#include "UndoItemAl2Lower.h"
#include "UndoManager.h"
#include "UuDecoder.h"
#include "Base64.h"
#include "QuotedPrintable.h"
#include "EditorDoc.h"
#include "BCMenu.h"
#include "EditorView.h"
#include "CodeConvManager.h"
#include "StaticFilespec.h"
#include "AutoResizer.h"
#include "FileMoveDlg.h"
#include "FileCopyDlg.h"
#include "FileRenameDlg.h"
#include "smtp.h"
#include "MyComboBoxEx.h"
#include "MailSendDlg.h"
#include "MailSendStatusDlg.h"

#ifdef _DEBUG
#define	new	DEBUG_NEW
#undef THIS_FILE
static char	THIS_FILE[]	= __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc

IMPLEMENT_DYNCREATE(CEditorDoc,	CDocument)

BEGIN_MESSAGE_MAP(CEditorDoc, CDocument)
	//{{AFX_MSG_MAP(CEditorDoc)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_VIEW_TAB_4, OnViewTab4)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TAB_4,	OnUpdateViewTab4)
	ON_COMMAND(ID_VIEW_TAB_8, OnViewTab8)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TAB_8,	OnUpdateViewTab8)
	ON_COMMAND(ID_EDIT_CLEARUNDOBUF, OnEditClearundobuf)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEARUNDOBUF, OnUpdateEditClearundobuf)
	ON_COMMAND(ID_FILE_OPENSAMENAME, OnFileOpensamename)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPENSAMENAME, OnUpdateFileOpensamename)
	ON_COMMAND(ID_FILE_COPY, OnFileCopy)
	ON_UPDATE_COMMAND_UI(ID_FILE_COPY, OnUpdateFileCopy)
	ON_COMMAND(ID_FILE_RENAME, OnFileRename)
	ON_UPDATE_COMMAND_UI(ID_FILE_RENAME, OnUpdateFileRename)
	ON_COMMAND(ID_FILE_MOVE, OnFileMove)
	ON_UPDATE_COMMAND_UI(ID_FILE_MOVE, OnUpdateFileMove)
	ON_COMMAND(ID_FILE_SHELLOPEN, OnFileShellopen)
	ON_UPDATE_COMMAND_UI(ID_FILE_SHELLOPEN, OnUpdateFileShellopen)
	ON_COMMAND(ID_EDIT_CODE2EUC, OnEditCode2euc)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CODE2EUC, OnUpdateEditCode2euc)
	ON_COMMAND(ID_EDIT_CODE2JIS, OnEditCode2jis)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CODE2JIS, OnUpdateEditCode2jis)
	ON_COMMAND(ID_EDIT_CODE2SJIS, OnEditCode2sjis)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CODE2SJIS, OnUpdateEditCode2sjis)
	ON_COMMAND(ID_VIEW_TAB_2, OnViewTab2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TAB_2, OnUpdateViewTab2)
	ON_COMMAND(ID_FILE_SENDMAIL, OnFileSendmail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SENDMAIL, OnUpdateFileSendmail)
	ON_COMMAND(ID_EDIT_CRLF, OnEditCrlf)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CRLF, OnUpdateEditCrlf)
	ON_COMMAND(ID_EDIT_CR, OnEditCr)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CR, OnUpdateEditCr)
	ON_COMMAND(ID_EDIT_LF, OnEditLf)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LF, OnUpdateEditLf)
	ON_COMMAND(ID_EDIT_ALLUNDO, OnEditAllundo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ALLUNDO, OnUpdateEditAllundo)
	ON_COMMAND(ID_EDIT_ALLREDO, OnEditAllredo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ALLREDO, OnUpdateEditAllredo)
	ON_COMMAND(ID_EDIT_SOURCEARRANGE, OnEditSourcearrange)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SOURCEARRANGE, OnUpdateEditSourcearrange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CEditorDoc::CEditorDoc()
{
	m_pApp = (CEditorApp*)AfxGetApp();

	m_dwFileAttributes = 0;
	m_bConvMode	= 0;
	m_bGrepMode	= 0;
	m_bDumpMode	= 0;
	m_bWCountMode =	0;
	m_bFSearchMode = 0;
	m_bBinaryMode =	0;
	m_bEncriptMode = 0;
	m_bDecriptMode = 0;
	m_bEncodeMode = 0;
	m_pobjLastWrittenTime =	NULL;
	m_bDelEndSpacesOnSave =	0;
	m_bSpace2TabOnSave = 0;
	m_bCompareMode = 0;
	m_objFileCmpFile1 = _T("");
	m_objFileCmpFile2 = _T("");
	m_bDirCmpMode = 0;
	m_objDirCmpFolder1 = _T("");
	m_objDirCmpFolder2 = _T("");
	m_bIsPrintPreview = 0;
	m_pobjMailSendStatusDlg = NULL;

	m_bDelEndSpacesOnSave =	m_pApp->GetProfileInt("CEditorDoc","bDelEndSpacesOnSave",0);
	m_bSpace2TabOnSave = m_pApp->GetProfileInt("CEditorDoc","bSpace2TabOnSave",0);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CEditorDoc::~CEditorDoc()
{
	if(m_pobjLastWrittenTime!=NULL)
		delete m_pobjLastWrittenTime;

	m_pApp->WriteProfileInt("CEditorDoc","bDelEndSpacesOnSave",m_bDelEndSpacesOnSave);
	m_pApp->WriteProfileInt("CEditorDoc","bSpace2TabOnSave",m_bSpace2TabOnSave);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�h�L�������g����������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::OnNewDocument()
{
	//���N���X�̃n���h���R�[��
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI	�h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
	m_dwFileAttributes = 0;
	m_strFileName = "";
	m_strFilePath = "";
	m_strFileDir = "";
	m_strFileExt = "";
	m_objUndoManager.Initialize();//Undo,Redo�Ǘ��N���X������
	if(m_pobjLastWrittenTime!=NULL)
		delete m_pobjLastWrittenTime;
	m_pobjLastWrittenTime=NULL;

	//�t�@�C���g���q�ɑΉ������F�A�t�H���g�ɐݒ�
	SetFontOfCurrentFileExt(m_strFileExt);
	SetColorOfCurrentFileExt(m_strFileExt);

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�h�L�������g�I�[�v��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::OnOpenDocument(LPCTSTR	lpszPathName)
{
	//�o�C�i���t�@�C���`�F�b�N
	CCodeConvManager	objCodeConvManager;
	if(objCodeConvManager.DetectCode(lpszPathName) == INPUT_BINARY)
		SetBinaryMode(TRUE);
	else
		SetBinaryMode(FALSE);

	//���N���X�̃n���h���R�[��
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	//�t�@�C�����擾
	m_strFilePath = lpszPathName;
	long lPos=0;
	for(long lCnt=0; lCnt<m_strFilePath.GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,(BYTE)m_strFilePath.GetAt(lCnt)))//�Q�޲ĕ����̑���޲�
		{
			lCnt+=2;
		}
		else
		{
			if(m_strFilePath.GetAt(lCnt)=='\\')
			{
				//'\'�ʒu�ێ�
				lPos = lCnt;
			}
			lCnt++;
		}
	}
	m_strFileName = m_strFilePath.Mid(lPos+1,m_strFilePath.GetLength()-lPos);
	m_pApp->WriteProfileString("CEditorApp","OpenFileName",m_strFileName);

	//�t�@�C���f�B���N�g���擾
	CString strDir = lpszPathName;
	int nROffset = strDir.ReverseFind('\\');
	int nFOffset = strDir.Find('\\');
	if( nROffset != nFOffset )
		strDir.SetAt(nROffset,0);
	else
		strDir.SetAt(nROffset+1,0);
	m_strFileDir = strDir;

	//�t�@�C���g���q�擾
	lPos = -1;
	for(long lCnt=0; lCnt<m_strFileName.GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,(BYTE)m_strFileName.GetAt(lCnt)))
		{
			lCnt+=2;
		}
		else
		{
			if(m_strFileName.GetAt(lCnt)=='.')
			{
				//'.'�ʒu�ێ�
				lPos = lCnt;
			}
			lCnt++;
		}
	}
	if(lPos != -1)
		m_strFileExt = m_strFileName.Mid(lPos+1,m_strFileName.GetLength()-lPos);
	else
		m_strFileExt = "";

	//�t�@�C���g���q�ɑΉ������F�A�t�H���g�ɐݒ�
	SetFontOfCurrentFileExt(m_strFileExt);
	SetColorOfCurrentFileExt(m_strFileExt);

	//�f�B���N�g���擾
	if(m_pApp->GetProfileInt("CEditorApp","IsChdirToOpenFile",0) != 0)
	{
		//�I�[�v�������t�@�C���̃f�B���N�g���ֈړ�
		m_pApp->ChangeDirectory(m_strFileDir);
	}

	//�t�@�C���ŏI�X�V�����擾
	if(m_pobjLastWrittenTime!=NULL)
		delete m_pobjLastWrittenTime;
	m_pobjLastWrittenTime =	new	CTime;
	CFileStatus		objFileStatus;
	CFile::GetStatus(m_strFilePath,objFileStatus);
	(*m_pobjLastWrittenTime) = objFileStatus.m_mtime;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�h�L�������g�ۑ�������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	BOOL bRet=0;

	//���N���X�̃n���h���R�[��
	bRet = CDocument::OnSaveDocument(lpszPathName);

	//�t�@�C�����擾
	m_strFilePath = lpszPathName;
	long lPos=0;
	for(long lCnt=0; lCnt<m_strFilePath.GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,(BYTE)m_strFilePath.GetAt(lCnt)))//�Q�޲ĕ����̑���޲�
		{
			lCnt+=2;
		}
		else
		{
			if(m_strFilePath.GetAt(lCnt)=='\\')
			{
				//'\'�ʒu�ێ�
				lPos = lCnt;
			}
			lCnt++;
		}
	}
	m_strFileName = m_strFilePath.Mid(lPos+1,m_strFilePath.GetLength()-lPos);

	//�t�@�C���f�B���N�g���擾
	CString strDir = lpszPathName;
	int nROffset = strDir.ReverseFind('\\');
	int nFOffset = strDir.Find('\\');
	if( nROffset != nFOffset )
		strDir.SetAt(nROffset,0);
	else
		strDir.SetAt(nROffset+1,0);
	m_strFileDir = strDir;

	//�t�@�C���g���q�擾
	lPos = -1;
	for(long lCnt=0; lCnt<m_strFileName.GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,(BYTE)m_strFileName.GetAt(lCnt)))
		{
			lCnt+=2;
		}
		else
		{
			if(m_strFileName.GetAt(lCnt)=='.')
			{
				//'.'�ʒu�ێ�
				lPos = lCnt;
			}
			lCnt++;
		}
	}
	if(lPos != -1)
		m_strFileExt = m_strFileName.Mid(lPos+1,m_strFileName.GetLength()-lPos);
	else
		m_strFileExt = "";

	//�t�@�C���g���q�ɑΉ������F�A�t�H���g�ɐݒ�
	SetFontOfCurrentFileExt(m_strFileExt);
	SetColorOfCurrentFileExt(m_strFileExt);

	//�f�B���N�g���擾
	if(m_pApp->GetProfileInt("CEditorApp","IsChdirToOpenFile",0) != 0)
	{
		//�I�[�v�������t�@�C���̃f�B���N�g���ֈړ�
		m_pApp->ChangeDirectory(m_strFileDir);
	}

	//�t�@�C���ŏI�X�V�����擾
	if(m_pobjLastWrittenTime!=NULL)
		delete m_pobjLastWrittenTime;
	m_pobjLastWrittenTime =	new	CTime;
	CFileStatus		objFileStatus;
	CFile::GetStatus(lpszPathName,objFileStatus);
	(*m_pobjLastWrittenTime) = objFileStatus.m_mtime;

	return bRet;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�h�L�������g�N���[�Y������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnCloseDocument()
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	m_dwFileAttributes = 0;
	m_strFileName = "";
	m_strFilePath = "";
	if(m_pobjLastWrittenTime!=NULL)
		delete m_pobjLastWrittenTime;
	m_pobjLastWrittenTime=NULL;

	//���N���X�̃n���h���R�[��
	CDocument::OnCloseDocument();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�h�L�������g�^�C�g���ݒ菈��											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetTitle(LPCTSTR lpszTitle)
{
	CString title;

	//�ݒ�擾
	BOOL bIsFullTitle =	m_pApp->GetProfileInt("CEditorDoc","bIsFullTitle",0);

	if(	bIsFullTitle !=	0 )	//�^�C�g���t���p�X�\��
	{
		title = m_strFilePath;
		if(	strstr(lpszTitle," *") != 0 )
			title += " *";

		if(	m_strFilePath.GetLength() )
			CDocument::SetTitle(title);
		else
			CDocument::SetTitle(lpszTitle);
	}
	else
	{
		CDocument::SetTitle(lpszTitle);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc �V���A���C�[�[�V����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�V���A���C�Y															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::Serialize(CArchive& ar)
{
	char* pPtr=NULL;
	long	lCnt=0;
	CFile* pobjFile	= ar.GetFile();

	if(ar.IsStoring()) //�ۑ�
	{
		//�s���󔒍폜�t���O�`�F�b�N
		if(GetFlagDelEndSpacesOnSave())
		{
			for(lCnt=0;	lCnt<m_objLineManager.GetLineCount(); lCnt++)
			{
				CLine* pobjLine	= m_objLineManager.GetOneLine(lCnt);
				if(pobjLine	!= NULL)
				{
					pobjLine->DelEndSpaces();
					m_objLineManager.SetOneLine(lCnt,pobjLine);
				}
			}
			m_objUndoManager.Initialize();	//Undo,Redo	�Ǘ��N���X������
		}
		//�󔒁|���^�u�ϊ��t���O�`�F�b�N
		if(GetFlagSpace2TabOnSave())
		{
			long lfWidth=0;
			POSITION pos = GetFirstViewPosition();
			while(pos != 0)
			{
				CEditorView* pView = (CEditorView*)GetNextView(pos);
				lfWidth	= pView->m_lfWidth;
				break;
			}
			m_objLineManager.ConvSpace2TabOnSave(lfWidth,GetTabSize());
			m_objUndoManager.Initialize();	//Undo,Redo	�Ǘ��N���X������
		}

		m_objLineManager.Serialize(ar);

		if(	GetFlagDelEndSpacesOnSave()	|| GetFlagSpace2TabOnSave()	)
		{
			POSITION pos = GetFirstViewPosition();
			while(pos != 0)
			{
				CEditorView* pView = (CEditorView*)GetNextView(pos);
				//�ĕ`��
				pView->OnUpdate(NULL,NULL,NULL);
				pView->Invalidate(TRUE);
//				//�L�����b�g�ʒu�������ʒu��
//				pView->MySetCaretPos(CPoint(pView->m_LineNoPos.x,pView->m_caretPos.y));
				m_pApp->MessagePump();
			}
		}
	}
	else //�ǂݍ���
	{
		//�t�@�C����������
		CString	objFilePath	= pobjFile->GetFilePath();
		m_dwFileAttributes = GetFileAttributes(	(LPCTSTR)objFilePath );
		if(IsBinary()) //�o�C�i���\�����[�h
		{
			//�ǂݍ��ݐ�p�Ƃ���
			m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
		}

		//�ǂݍ���
		if(IsBinary())
		{
			//�o�C�i���\�����[�h�œǂݍ���
			m_objLineManager.SerializeBinary(ar);
		}
		else
		{
			//�e�L�X�g�\�����[�h�œǂݍ���
			m_objLineManager.Serialize(ar);
		}
		m_objUndoManager.Initialize();	//Undo,Redo	�Ǘ��N���X������
//1999.05.16 Replace Start
//		UpdateAllViews(NULL,NULL,NULL);
		POSITION pos = GetFirstViewPosition();
		while(pos != NULL)
		{
			CEditorView* pView = (CEditorView*)GetNextView(pos);
			pView->OnUpdate(NULL,NULL,NULL);
			pView->Invalidate(TRUE);
		}
//1999.05.16 Replace End
	}

	//�h�L�������g�N���[��
	SetModifiedFlag(FALSE);

	//�^�C�g���X�V
	CString	objTitle = GetTitle();
	if(	(objTitle.GetLength() >	2) &&
		(objTitle.Right(2) == "	*")
	)
	{
		objTitle = objTitle.Left(objTitle.GetLength() -	2);
		SetTitle(objTitle);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc �N���X�̐f�f

#ifdef _DEBUG
void CEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEditorDoc::Dump(CDumpContext&	dc)	const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc ���b�Z�[�W�n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���|�㏑���ۑ��v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	if(IsReadOnly())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���|���O��t���ĕۑ��v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnUpdateFileSaveAs(CCmdUI*	pCmdUI)
{
	if(IsReadOnly())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���|�������O�̃t�@�C�����J���v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnFileOpensamename() 
{
	DWORD	dwRet=0;
	CString objFilter,objFolder,objOneKind,objPath;
	int		nOpenCount=0;
	CString fname="";

	//�t�@�C�����i�g���q�Ȃ��j�擾
	long lPos=0;
	long lCnt=0;
	for(lCnt=0; lCnt<m_strFileName.GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,(BYTE)m_strFileName.GetAt(lCnt)))//�Q�޲ĕ����̑���޲�
		{
			lCnt+=2;
		}
		else
		{
			if(m_strFileName.GetAt(lCnt)=='.')
			{
				fname.Format("%s.*",m_strFileName.Left(lCnt));
				break;
			}
			lCnt++;
		}
	}
	objOneKind = fname;

	//�t�@�C�������L�[�쐬
	objFolder = m_strFileDir;
	if(m_pApp->IsDriveRoot(objFolder))
		objFilter.Format("%s%s",objFolder,objOneKind);
	else
		objFilter.Format("%s\\%s",objFolder,objOneKind);

	//�����̃t�@�C��������
	CFileFind	objFileFind;
	nOpenCount = 0;
	BOOL bRet = objFileFind.FindFile(objFilter);
	if(bRet != FALSE)
	{
		while(objFileFind.FindNextFile()!=FALSE)
		{
			if( objFileFind.IsDots() != FALSE ) // "." or ".."
				continue;
			if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
				continue;
			//�����t�@�C���I�[�v��
			objPath = objFileFind.GetFilePath();
			if(objPath.CompareNoCase(m_strFilePath) != 0)
			{
				m_pApp->OpenFile(objPath);
				nOpenCount++;
			}
		}
		//�Ō�̂P��
		if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
		{
			objPath = objFileFind.GetFilePath();
			if(objPath.CompareNoCase(m_strFilePath) != 0)
			{
				m_pApp->OpenFile(objPath);
				nOpenCount++;
			}
		}
	}
	objFileFind.Close();

	if(nOpenCount == 0)
	{
		CString	objStr, objMsg;
		objMsg.LoadString(IDS_NOFILE_INTHISDIR);
		objStr.Format((LPCTSTR)objMsg, m_strFileDir);
		m_pApp->MyMessageBox((LPCTSTR)objStr,(MB_OK|MB_ICONSTOP),0);
	}
}
void CEditorDoc::OnUpdateFileOpensamename(CCmdUI* pCmdUI) 
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�t�@�C�����`�F�b�N
	if(	m_strFilePath.GetLength()==0 || m_strFileName.GetLength()==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���|�֘A�Â���ꂽ�\�t�g�ŊJ���v�I��������					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnFileShellopen() 
{
	CString objMsg;
	CWaitCursor	objCursor;
	HINSTANCE result = ::ShellExecute(NULL, _T("open"), m_strFilePath, NULL,NULL, SW_SHOW);
	if((UINT)result <= HINSTANCE_ERROR)
	{
		objMsg.LoadString(IDS_SOFTKICK_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
	}
}
void CEditorDoc::OnUpdateFileShellopen(CCmdUI* pCmdUI) 
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�t�@�C�����`�F�b�N
	if(	m_strFilePath.GetLength()==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���|�ړ��v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnFileMove() 
{
	CFileMoveDlg	objFileMoveDlg;

	//�t�@�C���ړ��_�C�A���O�\��
	strcpy(objFileMoveDlg.m_szFromPath,m_strFilePath);
	objFileMoveDlg.m_objFolder = m_pApp->GetProfileString("CEditorDoc","FileMoveToDir","");
	if(objFileMoveDlg.DoModal() != IDOK)
		return;
	m_pApp->WriteProfileString("CEditorDoc","FileMoveToDir",objFileMoveDlg.m_objFolder);

	SHFILEOPSTRUCT	stFileOp;
	memset(&stFileOp,0,sizeof(SHFILEOPSTRUCT));
	char	szFrom[_MAX_PATH],szTo[_MAX_PATH];
	memset(szFrom,0,sizeof(szFrom));
	memset(szTo,0,sizeof(szTo));

	//�}�E�X�J�[�\���������v�ɂ���
	CWaitCursor	objWaitCursor;

	//�r���o�߂�\������E�C���h�E�̐e�̃E�C���h�E
	CWnd* pWnd = (CWnd*)AfxGetMainWnd();
	stFileOp.hwnd = pWnd->m_hWnd;
	//�I�y���[�V����
	stFileOp.wFunc = FO_MOVE;
	//��
	strcpy(szFrom, m_strFilePath);
	stFileOp.pFrom = szFrom;
	//��
	strcpy(szTo, objFileMoveDlg.m_szToPath);
	stFileOp.pTo = szTo;
	//����Ɋւ���t���O
	stFileOp.fFlags = FOF_ALLOWUNDO;

	//�ړ��������s
	if( ::SHFileOperation(&stFileOp) == 0 ) //OK
	{
		if( _access(objFileMoveDlg.m_szToPath,0) != -1 &&
			_access(m_strFilePath,0) == -1
		)
		{
			//�h�L�������g�N���X�ɐV�����p�X��o�^
			m_strFilePath = objFileMoveDlg.m_szToPath;
			SetPathName(m_strFilePath);

			//�f�B���N�g���ړ�
			if(m_pApp->GetProfileInt("CEditorApp","IsChdirToOpenFile",0) != 0)
			{
				//�I�[�v�������t�@�C���̃f�B���N�g���ֈړ�
				m_pApp->ChangeDirectory((LPCTSTR)objFileMoveDlg.m_objFolder);
			}

			//�t�@�C���ŏI�X�V�����擾
			if(m_pobjLastWrittenTime!=NULL)
				delete m_pobjLastWrittenTime;
			m_pobjLastWrittenTime =	new	CTime;
			CFileStatus		objFileStatus;
			CFile::GetStatus(m_strFilePath,objFileStatus);
			(*m_pobjLastWrittenTime) = objFileStatus.m_mtime;
		}
	}
}
void CEditorDoc::OnUpdateFileMove(CCmdUI* pCmdUI) 
{
	//�t�@�C�����`�F�b�N
	if( m_strFileName.GetLength()==0 || m_strFilePath.GetLength()==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C�������A
	//�Í����A�������`�F�b�N
	if( IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���|�R�s�[�v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnFileCopy() 
{
	CFileCopyDlg	objFileCopyDlg;

	//�R�s�[��w��_�C�A���O�\��
	objFileCopyDlg.m_bRenameOnCollision = m_pApp->GetProfileInt("CEditorDoc","RenameOnCollision",0);
	objFileCopyDlg.m_objFile            = m_strFilePath;
	objFileCopyDlg.m_objFolder          = m_pApp->GetProfileString("CEditorDoc","FileCopyToDir","");
	if(objFileCopyDlg.DoModal() != IDOK)
		return;
	m_pApp->WriteProfileString("CEditorDoc","FileCopyToDir",objFileCopyDlg.m_objFolder);
	m_pApp->WriteProfileInt("CEditorDoc","RenameOnCollision",objFileCopyDlg.m_bRenameOnCollision);

	SHFILEOPSTRUCT	stFileOp;
	memset(&stFileOp,0,sizeof(SHFILEOPSTRUCT));
	char	szFrom[_MAX_PATH],szTo[_MAX_PATH];
	memset(szFrom,0,sizeof(szFrom));
	memset(szTo,0,sizeof(szTo));

	//�}�E�X�J�[�\���������v�ɂ���
	CWaitCursor	objWaitCursor;

	//�r���o�߂�\������E�C���h�E�̐e�̃E�C���h�E
	CWnd* pWnd = (CWnd*)AfxGetMainWnd();
	stFileOp.hwnd = pWnd->m_hWnd;
	//�I�y���[�V����
	stFileOp.wFunc = FO_COPY;
	//�R�s�[��
	strcpy(szFrom, m_strFilePath);
	stFileOp.pFrom = szFrom;
	//�R�s�[��
	strcpy(szTo, (LPCTSTR)objFileCopyDlg.m_objFolder);
	stFileOp.pTo = szTo;
	//����Ɋւ���t���O
	if(objFileCopyDlg.m_bRenameOnCollision != FALSE)
		stFileOp.fFlags = FOF_ALLOWUNDO|FOF_RENAMEONCOLLISION;
	else
		stFileOp.fFlags = FOF_ALLOWUNDO;

	//�t�@�C���R�s�[�������s
	::SHFileOperation(&stFileOp);
}
void CEditorDoc::OnUpdateFileCopy(CCmdUI* pCmdUI) 
{
	//�t�@�C�����`�F�b�N
	if( m_strFileName.GetLength()==0 || m_strFilePath.GetLength()==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C�������A
	//�Í����A�������`�F�b�N
	if( IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���|���O�̕ύX�v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnFileRename() 
{
	long	lCnt=0;
	CFileRenameDlg	objFileRenameDlg;

	//�t�@�C�����ύX�_�C�A���O�\��
	strcpy(objFileRenameDlg.m_szFromPath,m_strFilePath);
	objFileRenameDlg.m_objToFile = m_pApp->GetProfileString("CEditorDoc","FileRenameTo","");
	if(objFileRenameDlg.DoModal() != IDOK)
		return;
	m_pApp->WriteProfileString("CEditorDoc","FileRenameTo",objFileRenameDlg.m_objToFile);

	SHFILEOPSTRUCT	stFileOp;
	memset(&stFileOp,0,sizeof(SHFILEOPSTRUCT));
	char	szFrom[_MAX_PATH],szTo[_MAX_PATH];
	memset(szFrom,0,sizeof(szFrom));
	memset(szTo,0,sizeof(szTo));

	//�}�E�X�J�[�\���������v�ɂ���
	CWaitCursor	objWaitCursor;

	//�r���o�߂�\������E�C���h�E�̐e�̃E�C���h�E
	CWnd* pWnd = (CWnd*)AfxGetMainWnd();
	stFileOp.hwnd = pWnd->m_hWnd;
	//�I�y���[�V����
	stFileOp.wFunc = FO_RENAME;
	//��
	strcpy(szFrom, m_strFilePath);
	stFileOp.pFrom = szFrom;
	//��
	strcpy(szTo, objFileRenameDlg.m_szToPath);
	stFileOp.pTo = szTo;
	//����Ɋւ���t���O
	stFileOp.fFlags = FOF_ALLOWUNDO;

	//���O�ύX�������s
	if( ::SHFileOperation(&stFileOp) == 0 ) //OK
	{
		if( _access(objFileRenameDlg.m_szToPath,0) != -1 &&
			_access(m_strFilePath,0) == -1
		)
		{
			//�^�C�g���ύX
			m_strFileName = (LPCTSTR)objFileRenameDlg.m_objToFile;
			m_strFilePath = objFileRenameDlg.m_szToPath;
			//�t�@�C���g���q�擾
			for(lCnt=0; lCnt<m_strFileName.GetLength(); )
			{
				if(IsDBCSLeadByteEx(0,(BYTE)m_strFileName.GetAt(lCnt)))
				{
					lCnt+=2;
				}
				else
				{
					if(m_strFileName.GetAt(lCnt)=='.')
					{
						m_strFileExt = m_strFileName.Mid(lCnt+1,m_strFileName.GetLength()-lCnt);
						break;
					}
					lCnt++;
				}
			}

			//�t�@�C���g���q�ɑΉ������F�A�t�H���g�ɐݒ�
			SetFontOfCurrentFileExt(m_strFileExt);
			SetColorOfCurrentFileExt(m_strFileExt);

			//�h�L�������g�N���X�ɐV�����p�X��o�^
			SetPathName(m_strFilePath);

			//�t�@�C���ŏI�X�V�����擾
			if(m_pobjLastWrittenTime!=NULL)
				delete m_pobjLastWrittenTime;
			m_pobjLastWrittenTime =	new	CTime;
			CFileStatus		objFileStatus;
			CFile::GetStatus(m_strFilePath,objFileStatus);
			(*m_pobjLastWrittenTime) = objFileStatus.m_mtime;
		}
	}
}
void CEditorDoc::OnUpdateFileRename(CCmdUI* pCmdUI) 
{
	//�t�@�C�����`�F�b�N
	if( m_strFileName.GetLength()==0 || m_strFilePath.GetLength()==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C�������A
	//�Í����A�������`�F�b�N
	if( IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|���ɖ߂��v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditUndo()
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		return;
	}

	if(m_objUndoManager.IsCanUndo())
	{
		m_objUndoManager.doUndo();
//		UpdateAllViews(NULL,NULL,NULL);
		POSITION pos = GetFirstViewPosition();
		while(pos != NULL)
		{
			CEditorView* pView = (CEditorView*)GetNextView(pos);
			pView->OnUpdate(NULL,NULL,NULL);
			pView->Invalidate(TRUE);
		}
	}
	else
	{
		MessageBeep((UINT)-1);
	}
}
void CEditorDoc::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_objUndoManager.IsCanUndo())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|�J��Ԃ��v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditRedo()
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		return;
	}

	if(m_objUndoManager.IsCanRedo())
	{
		m_objUndoManager.doRedo();
//		UpdateAllViews(NULL,NULL,NULL);
		POSITION pos = GetFirstViewPosition();
		while(pos != 0)
		{
			CEditorView* pView = (CEditorView*)GetNextView(pos);
			pView->OnUpdate(NULL,NULL,NULL);
			pView->Invalidate(TRUE);
		}
	}
	else
	{
		MessageBeep((UINT)-1);
	}
}
void CEditorDoc::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_objUndoManager.IsCanRedo())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|Undo/Redo�o�b�t�@�N���A�v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditClearundobuf()
{
	m_objUndoManager.Initialize();
}
void CEditorDoc::OnUpdateEditClearundobuf(CCmdUI* pCmdUI)
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_objUndoManager.IsCanUndo()	|| m_objUndoManager.IsCanRedo())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|���ׂČ��ɖ߂��v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditAllundo() 
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		return;
	}

	if(m_objUndoManager.IsCanUndo())
	{
		while(m_objUndoManager.IsCanUndo())
		{
			m_objUndoManager.doUndo();
			m_pApp->MessagePump();

//			UpdateAllViews(NULL,NULL,NULL);
			POSITION pos = GetFirstViewPosition();
			while(pos != NULL)
			{
				CEditorView* pView = (CEditorView*)GetNextView(pos);
				pView->OnUpdate(NULL,NULL,NULL);
				pView->Invalidate(TRUE);
			}
		}
	}
	else
	{
		MessageBeep((UINT)-1);
	}
}
void CEditorDoc::OnUpdateEditAllundo(CCmdUI* pCmdUI) 
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_objUndoManager.IsCanUndo())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|���ׂČJ��Ԃ��v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditAllredo() 
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		return;
	}

	if(m_objUndoManager.IsCanRedo())
	{
		while(m_objUndoManager.IsCanRedo())
		{
			m_objUndoManager.doRedo();
			m_pApp->MessagePump();

//			UpdateAllViews(NULL,NULL,NULL);
			POSITION pos = GetFirstViewPosition();
			while(pos != 0)
			{
				CEditorView* pView = (CEditorView*)GetNextView(pos);
				pView->OnUpdate(NULL,NULL,NULL);
				pView->Invalidate(TRUE);
			}
		}
	}
	else
	{
		MessageBeep((UINT)-1);
	}
}
void CEditorDoc::OnUpdateEditAllredo(CCmdUI* pCmdUI) 
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_objUndoManager.IsCanRedo())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|���s�R�[�h DOS/Windows�`���v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditCrlf() 
{
	SetKaigyoKind(KAIGYO_CRLF);
}
void CEditorDoc::OnUpdateEditCrlf(CCmdUI* pCmdUI) 
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C�������A
	//�Í����A�������`�F�b�N
	if( IsReadOnly()||
		IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//���s�R�[�h����
	if(GetKaigyoKind() == KAIGYO_CRLF)
	{
		pCmdUI->SetCheck(TRUE);
		return;
	}

	pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|���s�R�[�h UNIX�`���v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditLf() 
{
	SetKaigyoKind(KAIGYO_LF);
}
void CEditorDoc::OnUpdateEditLf(CCmdUI* pCmdUI) 
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C�������A
	//�Í����A�������`�F�b�N
	if( IsReadOnly()||
		IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//���s�R�[�h����
	if(GetKaigyoKind() == KAIGYO_LF)
	{
		pCmdUI->SetCheck(TRUE);
		return;
	}

	pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|���s�R�[�h Macintosh�`���v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditCr() 
{
	SetKaigyoKind(KAIGYO_CR);
}
void CEditorDoc::OnUpdateEditCr(CCmdUI* pCmdUI) 
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C�������A
	//�Í����A�������`�F�b�N
	if( IsReadOnly()||
		IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//���s�R�[�h����
	if(GetKaigyoKind() == KAIGYO_CR)
	{
		pCmdUI->SetCheck(TRUE);
		return;
	}

	pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|�d�t�b�ϊ��v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditCode2euc() 
{
	char	szTempFileName[_MAX_FNAME];
	UINT	nDetectCode=0;
	int		nRet=0;
	CCodeConvManager	objCodeConvManager;
	CString objMsg;

	//�o�͗p�ꎞ�t�@�C�����擾
	::GetTempFileName( m_strFileDir, "Cnv", 0, szTempFileName );

	//���̓t�@�C�������R�[�h�̔���
	nDetectCode = objCodeConvManager.DetectCode(m_strFilePath);
	if(nDetectCode != INPUT_SJIS && nDetectCode != INPUT_JIS)
		return;

	//�����R�[�h�ϊ����s
	if(nDetectCode == INPUT_SJIS)
		nRet = objCodeConvManager.ConvertSjisToEuc(m_strFilePath,szTempFileName);
	else if(nDetectCode == INPUT_JIS)
		nRet = objCodeConvManager.ConvertJisToEuc(m_strFilePath,szTempFileName);
	if(nRet == FALSE)
	{
		objMsg.LoadString(IDS_CONVERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//���̓t�@�C���폜
	::DeleteFile(m_strFilePath);
	//�o�̓t�@�C��������̓t�@�C�����֕ύX
	rename(szTempFileName,m_strFilePath);

	//�ēǂݍ���
	ReloadDocument(FALSE);
}
void CEditorDoc::OnUpdateEditCode2euc(CCmdUI* pCmdUI) 
{
	//�t�@�C�����`�F�b�N
	if( m_strFileName.GetLength()==0 || m_strFilePath.GetLength()==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C�������A
	//�Í����A�������`�F�b�N
	if( IsReadOnly()||
		IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�����R�[�h����
	CCodeConvManager	objCodeConvManager;
	UINT nDetectCode = objCodeConvManager.DetectCode(m_strFilePath);
	if( nDetectCode == INPUT_BINARY  ||		//���̓t�@�C�����o�C�i���t�@�C��
		nDetectCode == INPUT_ASCII   ||		//���̓t�@�C���Ɋ����R�[�h�Ȃ�
		nDetectCode == INPUT_UNKNOWN 		//�����R�[�h�s��
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else if( nDetectCode == INPUT_EUC )
	{
		pCmdUI->SetCheck(TRUE);
		return;
	}

	pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|�r�i�h�r�ϊ��v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditCode2sjis() 
{
	char	szTempFileName[_MAX_FNAME];
	UINT	nDetectCode=0;
	int		nRet=0;
	CCodeConvManager	objCodeConvManager;
	CString objMsg;

	//�o�͗p�ꎞ�t�@�C�����擾
	::GetTempFileName( m_strFileDir, "Cnv", 0, szTempFileName );

	//���̓t�@�C�������R�[�h�̔���
	nDetectCode = objCodeConvManager.DetectCode(m_strFilePath);
	if(nDetectCode != INPUT_EUC && nDetectCode != INPUT_JIS)
		return;

	//�����R�[�h�ϊ����s
	if(nDetectCode == INPUT_EUC)
		nRet = objCodeConvManager.ConvertEucToSjis(m_strFilePath,szTempFileName);
	else if(nDetectCode == INPUT_JIS)
		nRet = objCodeConvManager.ConvertJisToSjis(m_strFilePath,szTempFileName);
	if(nRet == FALSE)
	{
		objMsg.LoadString(IDS_CONVERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//���̓t�@�C���폜
	::DeleteFile(m_strFilePath);
	//�o�̓t�@�C��������̓t�@�C�����֕ύX
	rename(szTempFileName,m_strFilePath);

	//�ēǂݍ���
	ReloadDocument(FALSE);
}
void CEditorDoc::OnUpdateEditCode2sjis(CCmdUI* pCmdUI) 
{
	//�t�@�C�����`�F�b�N
	if( m_strFileName.GetLength()==0 || m_strFilePath.GetLength()==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C�������A
	//�Í����A�������`�F�b�N
	if(	IsReadOnly()||
		IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�����R�[�h����
	CCodeConvManager	objCodeConvManager;
	UINT nDetectCode = objCodeConvManager.DetectCode(m_strFilePath);
	if( nDetectCode == INPUT_BINARY  ||		//���̓t�@�C�����o�C�i���t�@�C��
		nDetectCode == INPUT_ASCII   ||		//���̓t�@�C���Ɋ����R�[�h�Ȃ�
		nDetectCode == INPUT_UNKNOWN 		//�����R�[�h�s��
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else if( nDetectCode == INPUT_SJIS )
	{
		pCmdUI->SetCheck(TRUE);
		return;
	}

	pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|�i�h�r�ϊ��v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditCode2jis() 
{
	char	szTempFileName[_MAX_FNAME];
	UINT	nDetectCode=0;
	int		nRet=0;
	CCodeConvManager	objCodeConvManager;
	CString objMsg;

	//�o�͗p�ꎞ�t�@�C�����擾
	::GetTempFileName( m_strFileDir, "Cnv", 0, szTempFileName );

	//���̓t�@�C�������R�[�h�̔���
	nDetectCode = objCodeConvManager.DetectCode(m_strFilePath);
	if(nDetectCode != INPUT_SJIS && nDetectCode != INPUT_EUC)
		return;

	//�����R�[�h�ϊ����s
	if(nDetectCode == INPUT_SJIS)
		nRet = objCodeConvManager.ConvertSjisToJis(m_strFilePath,szTempFileName);
	else if(nDetectCode == INPUT_EUC)
		nRet = objCodeConvManager.ConvertEucToJis(m_strFilePath,szTempFileName);
	if(nRet == FALSE)
	{
		objMsg.LoadString(IDS_CONVERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//���̓t�@�C���폜
	::DeleteFile(m_strFilePath);
	//�o�̓t�@�C��������̓t�@�C�����֕ύX
	rename(szTempFileName,m_strFilePath);

	//�ēǂݍ���
	ReloadDocument(FALSE);
}
void CEditorDoc::OnUpdateEditCode2jis(CCmdUI* pCmdUI) 
{
	//�t�@�C�����`�F�b�N
	if( m_strFileName.GetLength()==0 || m_strFilePath.GetLength()==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C�������A
	//�Í����A�������`�F�b�N
	if(	IsReadOnly()||
		IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�����R�[�h����
	CCodeConvManager	objCodeConvManager;
	UINT nDetectCode = objCodeConvManager.DetectCode(m_strFilePath);
	if( nDetectCode == INPUT_BINARY  ||		//���̓t�@�C�����o�C�i���t�@�C��
		nDetectCode == INPUT_ASCII   ||		//���̓t�@�C���Ɋ����R�[�h�Ȃ�
		nDetectCode == INPUT_UNKNOWN 		//�����R�[�h�s��
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else if( nDetectCode == INPUT_JIS )
	{
		pCmdUI->SetCheck(TRUE);
		return;
	}

	pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|�Q�^�u�v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnViewTab2() 
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		return;
	}

	if(	GetTabSize() ==	2 )	//���łɂQ�^�u
		return;

	//�Q�^�u�ݒ�
	SetTabSize(2);
}
void CEditorDoc::OnUpdateViewTab2(CCmdUI* pCmdUI) 
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(	GetTabSize() ==	2 )
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|�S�^�u�v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnViewTab4()
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		return;
	}

	if(	GetTabSize() ==	4 )	//���łɂS�^�u
		return;

	//�S�^�u�ݒ�
	SetTabSize(4);
}
void CEditorDoc::OnUpdateViewTab4(CCmdUI* pCmdUI)
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(	GetTabSize() ==	4 )
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|�W�^�u�v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnViewTab8()
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		return;
	}

	if(	GetTabSize() ==	8 )	//���łɂW�^�u
		return;

	//�W�^�u�ݒ�
	SetTabSize(8);
}
void CEditorDoc::OnUpdateViewTab8(CCmdUI* pCmdUI)
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(	GetTabSize() ==	8 )
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�c�[���|���[�����M�v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnFileSendmail() 
{
	long			lCnt=0, lRet=0;
	UINT			unRet=0;
	char			szBuf[256],szMsg[512],szRasMsg[512],szTempFileName[_MAX_FNAME];
	CLine*			pobjLine=NULL;
	CSmtp			objSmtp;
	CMailSendDlg	objMailSendDlg;
	CString			objStr;

	//���݂̃A�J�E���g�ɑΉ�����A���[���֘A�ݒ���擾
	char			szSection[64];
	int				nAccountNo = m_pApp->GetProfileInt("Mail","CurrentAccountNo",0);
	if(nAccountNo < 0 || nAccountNo > 8)
		nAccountNo = 0;
	sprintf(szSection,"Mail_Account%d",nAccountNo);
	CString objMailAddress = m_pApp->GetProfileString(szSection,"MailAddress","");
	CString objSmtpServer  = m_pApp->GetProfileString(szSection,"SmtpServer","");
	BOOL bIsDialup         = m_pApp->GetProfileInt(szSection,"bIsDialup",0);
	CString objDialupEntry = m_pApp->GetProfileString(szSection,"DialupEntry","");
	CString objRasUserName = m_pApp->GetProfileString(szSection,"RasUserName","");
	CString objRasPasswd   = m_pApp->GetProfileString(szSection,"RasPasswd","");
	BOOL bAutoDisconnect   = m_pApp->GetProfileInt(szSection,"AutoDisconnect",0);
	int nEncodeType        = m_pApp->GetProfileInt(szSection,"EncodeType",SMTP_ENCODE_BASE64);
	CString objSignature   = m_pApp->GetProfileString(szSection,"Signature","");

	DWORD dwRasState=0;
	CString objMsg;

	//���O�`�F�b�N
	if(objMailAddress.IsEmpty())
	{
		objMsg.LoadString(IDS_SMTP_NOFROM);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if(objSmtpServer.IsEmpty())
	{
		objMsg.LoadString(IDS_SMTP_NOSERVER);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//����ƌ����A�Y�t�t�@�C����ݒ肵�Ă��炤
	objMailSendDlg.m_objTo        = m_pApp->GetProfileString(szSection,"SendTo","");
	if(m_pApp->GetProfileInt(szSection,"bIsAutoSetSubject",1) == 1)
		objMailSendDlg.m_objSubject = GetTitle();
	else
		objMailSendDlg.m_objSubject = _T("");
	objMailSendDlg.m_objCc        = m_pApp->GetProfileString(szSection,"SendCc","");
	objMailSendDlg.m_objBcc       = m_pApp->GetProfileString(szSection,"SendBcc","");
	objMailSendDlg.m_objReplyTo   = m_pApp->GetProfileString(szSection,"SendReplyTo",""); 
	objMailSendDlg.m_objReference = m_pApp->GetProfileString(szSection,"Reference","");
	if(objMailSendDlg.DoModal() != IDOK)
	{
		objMailSendDlg.m_objAttachFiles.RemoveAll();
		return;
	}
	m_pApp->WriteProfileString(szSection,"SendTo",objMailSendDlg.m_objTo);
	m_pApp->WriteProfileString(szSection,"SendCc",objMailSendDlg.m_objCc);
	m_pApp->WriteProfileString(szSection,"SendBcc",objMailSendDlg.m_objBcc);
	m_pApp->WriteProfileString(szSection,"SendReplyTo",objMailSendDlg.m_objReplyTo); 
	m_pApp->WriteProfileString(szSection,"Reference",objMailSendDlg.m_objReference);

	//���[�����M�󋵃��[�h���X�_�C�A���O�\��
	m_pobjMailSendStatusDlg = new CMailSendStatusDlg();
	m_pobjMailSendStatusDlg->Create(IDD_MAILSENDSTATUS);
	WaitForMailSendDlgDisp();

	//���[�����M�p�����[�^�ݒ�
	objSmtp.SetEncodeType(nEncodeType);
	objSmtp.SetFrom(objMailAddress);
	for(lCnt=0; lCnt<objMailSendDlg.m_objToArray.GetSize(); lCnt++)
	{
		objStr = objMailSendDlg.m_objToArray.GetAt(lCnt);
		objSmtp.SetTo(objStr);
	}
	objSmtp.SetSubject(objMailSendDlg.m_objSubject);
	for(lCnt=0; lCnt<objMailSendDlg.m_objCcArray.GetSize(); lCnt++)
	{
		objStr = objMailSendDlg.m_objCcArray.GetAt(lCnt);
		objSmtp.SetCc(objStr);
	}
	for(lCnt=0; lCnt<objMailSendDlg.m_objBccArray.GetSize(); lCnt++)
	{
		objStr = objMailSendDlg.m_objBccArray.GetAt(lCnt);
		objSmtp.SetBcc(objStr);
	}
	objSmtp.SetReplyTo(objMailSendDlg.m_objReplyTo);
	objSmtp.SetReference(objMailSendDlg.m_objReference);
	for(lCnt=0; lCnt<objMailSendDlg.m_objAttachFiles.GetSize(); lCnt++)
	{
		objStr = objMailSendDlg.m_objAttachFiles.GetAt(lCnt);
		objSmtp.SetAttachFile(objStr);
	}
	objMailSendDlg.m_objToArray.RemoveAll();
	objMailSendDlg.m_objCcArray.RemoveAll();
	objMailSendDlg.m_objBccArray.RemoveAll();
	objMailSendDlg.m_objAttachFiles.RemoveAll();
	objSmtp.SetSignature(objSignature);
	objSmtp.m_bIsSendMessageID    = m_pApp->GetProfileInt(szSection,"bIsSendMessage",1);
	objSmtp.m_bIsSendDate         = m_pApp->GetProfileInt(szSection,"bIsSendDate",1);
	objSmtp.m_bIsReqMsgDispsition = objMailSendDlg.m_bIsReqMsgDispsition;

	if(bIsDialup) //�_�C�A���A�b�v�ڑ�
	{
		if(m_pApp->m_hRasDllInstance)
		{
			//�֐��A�h���X�擾
			m_pApp->m_lpfnRasFunc3 = (LPFNRASFUNC3)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasConnect");
			m_pApp->m_lpfnRasFunc4 = (LPFNRASFUNC4)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasIsConnected");
			m_pApp->m_lpfnRasFunc5 = (LPFNRASFUNC5)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasDisConnect");
			if( m_pApp->m_lpfnRasFunc3 != NULL && 
				m_pApp->m_lpfnRasFunc4 != NULL && 
				m_pApp->m_lpfnRasFunc5 != NULL )
			{
				//�q�`�r�ڑ�(rasConnect)
				objMsg.LoadString(IDS_RASCONNECT);
				m_pobjMailSendStatusDlg->SetStatus(objMsg);
				lRet = m_pApp->m_lpfnRasFunc3((char*)(LPCTSTR)objDialupEntry,
									(char*)(LPCTSTR)objRasUserName,
									(char*)(LPCTSTR)objRasPasswd,
									"",
									szBuf );
				if(lRet == -6)
				{
					//�q�`�r�ؒf(rasDisconnect)
					objMsg.LoadString(IDS_RASDISCONNECT);
					m_pobjMailSendStatusDlg->SetStatus(objMsg);
					m_pApp->m_lpfnRasFunc5();
					objMsg.LoadString(IDS_RASCONNECTERR);
					sprintf(szMsg,(LPCTSTR)objMsg,szBuf);
					m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);

					//���[�����M�󋵃��[�h���X�_�C�A���O����
					m_pobjMailSendStatusDlg->DestroyWindow();
					delete m_pobjMailSendStatusDlg;
					m_pobjMailSendStatusDlg = NULL;
					return;
				}
				else if(lRet == 1) //���łɃI�[�v���ς�
				{
					objMsg.LoadString(IDS_RASCONNECTEND);
					m_pobjMailSendStatusDlg->SetStatus(objMsg);
					//���̂܂܃��[�����M��
				}
				else if(lRet == 0) //�q�`�r�ڑ��n�j
				{
					//�ڑ������҂�
					for(;;)
					{
						m_pApp->MessagePump();
						Sleep(10);
						if(m_pobjMailSendStatusDlg->IsCanceled())
						{
							//�q�`�r�ؒf(rasDisconnect)
							objMsg.LoadString(IDS_RASDISCONNECT);
							m_pobjMailSendStatusDlg->SetStatus(objMsg);
							m_pApp->m_lpfnRasFunc5();

							//���[�����M�󋵃��[�h���X�_�C�A���O����
							m_pobjMailSendStatusDlg->DestroyWindow();
							delete m_pobjMailSendStatusDlg;
							m_pobjMailSendStatusDlg = NULL;
							return;
						}

						//��ԃ`�F�b�N(rasIsConnected)
						memset(szRasMsg,0,sizeof(szRasMsg));
						lRet = m_pApp->m_lpfnRasFunc4( &dwRasState, szRasMsg );
						objMsg.LoadString(IDS_RASISCONNECT);
						sprintf(szMsg,(LPCTSTR)objMsg,dwRasState,szRasMsg);
						m_pobjMailSendStatusDlg->SetStatus(szMsg);
						if(lRet) //�ڑ����ꂽ
						{
							objMsg.LoadString(IDS_RASCONNECTEND);
							m_pobjMailSendStatusDlg->SetStatus(objMsg);
							break;
						}
						else
						{
							if(dwRasState == 100 || dwRasState == 8193)
							{
								//�q�`�r�ؒf(rasDisconnect)
								m_pApp->m_lpfnRasFunc5();
								objMsg.LoadString(IDS_RAS_CONNECTERR);
								m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);

								//���[�����M�󋵃��[�h���X�_�C�A���O����
								m_pobjMailSendStatusDlg->DestroyWindow();
								delete m_pobjMailSendStatusDlg;
								m_pobjMailSendStatusDlg = NULL;
								return;
							}
						}
					}
				}
			}
		}
	}

	//�󋵕\��
	if(m_pobjMailSendStatusDlg->IsCanceled())
	{
		if( bIsDialup && bAutoDisconnect && m_pApp->m_lpfnRasFunc5) //�_�C�A���A�b�v�ڑ�
		{
			//�q�`�r�ؒf(rasDisconnect)
			objMsg.LoadString(IDS_RASDISCONNECT);
			m_pobjMailSendStatusDlg->SetStatus(objMsg);
			m_pApp->m_lpfnRasFunc5();
		}
		//���[�����M�󋵃��[�h���X�_�C�A���O����
		m_pobjMailSendStatusDlg->DestroyWindow();
		delete m_pobjMailSendStatusDlg;
		m_pobjMailSendStatusDlg = NULL;
		return;
	}

	//���[�����M
	objMsg.LoadString(IDS_MAILSERVER_CONNECT);
	m_pobjMailSendStatusDlg->SetStatus(objMsg);
	if(objSmtp.Connect(objSmtpServer))
	{
		//�󋵕\��
		objMsg.LoadString(IDS_MAIL_SEND);
		m_pobjMailSendStatusDlg->SetStatus(objMsg);
		if(m_pobjMailSendStatusDlg->IsCanceled())
		{
			if( bIsDialup && bAutoDisconnect && m_pApp->m_lpfnRasFunc5) //�_�C�A���A�b�v�ڑ�
			{
				//�q�`�r�ؒf(rasDisconnect)
				objMsg.LoadString(IDS_RASDISCONNECT);
				m_pobjMailSendStatusDlg->SetStatus(objMsg);
				m_pApp->m_lpfnRasFunc5();
			}

			//�ؒf
			objSmtp.Disconnect();

			//���[�����M�󋵃��[�h���X�_�C�A���O����
			m_pobjMailSendStatusDlg->DestroyWindow();
			delete m_pobjMailSendStatusDlg;
			m_pobjMailSendStatusDlg = NULL;
			return;
		}

		//���e���ꎞ�t�@�C���֕ۑ�
		memset(szTempFileName,0,sizeof(szTempFileName));
		::GetTempFileName( m_pApp->m_strExePath,"Smtp",0,szTempFileName );
		CFile objOutFile;
		if( objOutFile.Open( szTempFileName, (CFile::modeCreate|CFile::modeWrite), NULL ) ) //OK
		{
			for(lCnt=0; lCnt<GetLineCount(); lCnt++)
			{
				pobjLine = GetOneLine(lCnt);
				if(pobjLine == NULL)
					continue;
				objOutFile.Write(pobjLine->Data(),pobjLine->GetLength());
				if(lCnt < GetLineCount()-1)
					objOutFile.Write("\r\n",2);
			}
			objOutFile.Close();
		}

		if(objSmtp.SendMail(szTempFileName))
		{
			objMsg.LoadString(IDS_MAIL_SENDEND);
			m_pobjMailSendStatusDlg->SetStatus(objMsg);

			//�ؒf
			objSmtp.Disconnect();

			//���[�����M�I���ʒm
			objMsg.LoadString(IDS_SMTP_SENDEND);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
		::DeleteFile(szTempFileName);
	}

	if( bIsDialup && bAutoDisconnect && m_pApp->m_lpfnRasFunc5) //�_�C�A���A�b�v�ڑ�
	{
		//�q�`�r�ؒf(rasDisconnect)
		objMsg.LoadString(IDS_RASDISCONNECT);
		m_pobjMailSendStatusDlg->SetStatus(objMsg);
		m_pApp->m_lpfnRasFunc5();
	}

	//���[�����M�󋵃��[�h���X�_�C�A���O����
	m_pobjMailSendStatusDlg->DestroyWindow();
	delete m_pobjMailSendStatusDlg;
	m_pobjMailSendStatusDlg = NULL;
}
void CEditorDoc::OnUpdateFileSendmail(CCmdUI* pCmdUI) 
{
	//������`�F�b�N
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�h�L�������g�G���v�e�B�`�F�b�N
	if(GetLineCount() == 0)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�c�[���|�\�[�X���`�v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditSourcearrange() 
{
	long lCnt=0;
	long lLine=0;
	long lOffset=0;
	long lTabSize=GetTabSize();
	long lCurrentIndent=0;
	long lMatchLength=0;
	long lOpenParenCnt=0;
	unsigned char cBuf=0, cBuf2=0;
	unsigned char c1stChar=0;
	unsigned char c2ndChar=0;
	CLine* pobjLine=NULL;
	CLine* pobjNextLine=NULL;
	CLine objLineTemp;
	CString objFindStr=_T("");
	BOOL bFind=FALSE;
	BOOL bIsInsideOfSingleQuote=FALSE;
	BOOL bIsInsideOfDoubleQuote=FALSE;
	BOOL bIsInsideOfDoubleSlash=FALSE;
	BOOL bIsInsideOfComment=FALSE;
	BOOL bIsFillTab=TRUE;				//�C���f���g���^�u�ōs�����H�X�y�[�X�ōs�����H
	BOOL bIsPushOutOpenParen=TRUE;		//'{'�����̍s�֒ǂ��o�����H


	bIsInsideOfComment = FALSE;
	for(lLine=0; ; lLine++)
	{
		//�P�s���o��
		pobjLine = GetOneLine(lLine);
		if(pobjLine==NULL)
			break;
		if(pobjLine->GetLength() == 0)
			continue;

		//�C���f���g
		if(!bIsInsideOfComment)
		{
			pobjLine->DelTopSpaces();
			if(lCurrentIndent > 0)
			{
				if(bIsFillTab)	//�^�u�ŏ[�U����
					pobjLine->InsertChars(0,'\t',lCurrentIndent);
				else			//�X�y�[�X�ŏ[�U����
					pobjLine->InsertChars(0,' ',lCurrentIndent*lTabSize);
			}
		}

		//�������[�v
		bIsInsideOfSingleQuote = FALSE;
		bIsInsideOfDoubleSlash = FALSE;
		bIsInsideOfDoubleQuote = FALSE;
		for(lOffset=-1,lCnt=0; lCnt<pobjLine->GetLength(); )
		{
			cBuf = pobjLine->GetAt(lCnt);
			if(IsDBCSLeadByteEx(0,cBuf))
				lCnt+=2;
			else
			{
				// '\'' �� '"' �̒��ɂ���'{' �� '}'�͖�������
				// '//'�̂b�{�{�R�����g�ȍ~�ɂ���'{' �� '}'�͖�������
				// /* �` */ �̂b�R�����g�̒��ɂ���'{' �� '}'�͖�������
				if(cBuf == '\'')
					bIsInsideOfSingleQuote = !bIsInsideOfSingleQuote;
				else if(cBuf == '"')
					bIsInsideOfDoubleQuote = !bIsInsideOfDoubleQuote;
				else if(cBuf == '/')
				{
					cBuf2 = pobjLine->GetAt(lCnt+1);
					if(!IsDBCSLeadByteEx(0,cBuf2) && cBuf2 == '/')
						bIsInsideOfDoubleSlash = TRUE;
					else if(!IsDBCSLeadByteEx(0,cBuf2) && cBuf2 == '*' && !bIsInsideOfComment)
						bIsInsideOfComment = TRUE;
				}
				else if(cBuf == '*')
				{
					cBuf2 = pobjLine->GetAt(lCnt+1);
					if(!IsDBCSLeadByteEx(0,cBuf2) && cBuf2 == '/' && bIsInsideOfComment)
						bIsInsideOfComment = FALSE;
				}
				else if(cBuf == '{' && 
						!bIsInsideOfSingleQuote && !bIsInsideOfDoubleQuote &&
						!bIsInsideOfDoubleSlash && !bIsInsideOfComment
				)
				{
					objFindStr = '{';
					lOffset=lCnt;
					break;
				}
				else if(cBuf == '}' && 
						!bIsInsideOfSingleQuote && !bIsInsideOfDoubleQuote &&
						!bIsInsideOfDoubleSlash && !bIsInsideOfComment
				)
				{
					objFindStr = '}';
					lOffset=lCnt;
					break;
				}
				lCnt++;
			}
		}

		if(lOffset == -1)
			continue;

		if(!objFindStr.Compare("{")) // '{' �����������ꍇ
		{
			if(bIsPushOutOpenParen)//'{'�����̍s�֒ǂ��o���ꍇ
			{
				for(bFind=FALSE,lCnt=0; lCnt<lOffset; lCnt++)
				{
					cBuf = pobjLine->GetAt(lCnt);
					if(IsDBCSLeadByteEx(0,cBuf) || (!IsDBCSLeadByteEx(0,cBuf) && cBuf!=' ' && cBuf!='\t'))
					{
						bFind=TRUE;
						break;
					}
				}
				if(bFind) //'{'�̑O�ɋ󔒕����ȊO������
				{
					//'{'�����ʒu������ێ�
					pobjNextLine = new CLine();
					pobjNextLine->Set(pobjLine->Data(lOffset));
					//'{'�����ʒu�łP�s��؂�
					pobjLine->SetAt(lOffset,0);
					//�؂�o���ꂽ�s�����̍s�Ƃ���
					InsertOneLine(lLine+1,pobjNextLine);
					continue;
				}
			}

			if((lOffset+1) < pobjLine->GetLength()) //'{'���s���ɂ���
			{
				//'{'�����ʒu������ێ�
				pobjNextLine = new CLine();
				pobjNextLine->Set(pobjLine->Data(lOffset+1));
				//'{'�����ʒu�̎��łP�s��؂�
				pobjLine->SetAt(lOffset+1,0);
				//�؂�o���ꂽ�s�����̍s�Ƃ���
				InsertOneLine(lLine+1,pobjNextLine);
			}
			//�C���f���g���ɂP�v���X
			lCurrentIndent++;
			continue;
		}
		else if(!objFindStr.Compare("}")) // '}' �����������ꍇ
		{
			for(bFind=FALSE,lCnt=0; lCnt<lOffset; lCnt++)
			{
				cBuf = pobjLine->GetAt(lCnt);
				if(IsDBCSLeadByteEx(0,cBuf) || (!IsDBCSLeadByteEx(0,cBuf) && cBuf!=' ' && cBuf!='\t'))
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind) //'}'�̑O�ɋ󔒕����ȊO������
			{
				//'}'�����ʒu������ێ�
				pobjNextLine = new CLine();
				pobjNextLine->Set(pobjLine->Data(lOffset));
				//'}'�����ʒu�łP�s��؂�
				pobjLine->SetAt(lOffset,0);
				//�؂�o���ꂽ�s�����̍s�Ƃ���
				InsertOneLine(lLine+1,pobjNextLine);
				continue;
			}

			if((lOffset+1) < pobjLine->GetLength()) //'}'���s���ɂ���
			{
				//'}'�����ʒu������ێ�
				pobjNextLine = new CLine();
				pobjNextLine->Set(pobjLine->Data(lOffset+1));
				//'}'�����ʒu�̎��łP�s��؂�
				pobjLine->SetAt(lOffset+1,0);
				//�؂�o���ꂽ�s�����̍s�Ƃ���
				InsertOneLine(lLine+1,pobjNextLine);
			}

			//�^�u�폜
			cBuf = pobjLine->GetAt(0);
			if(!IsDBCSLeadByteEx(0,cBuf) && cBuf == '\t')
				pobjLine->DeleteChar(0,&c1stChar,&c2ndChar);
			//�C���f���g������P�}�C�i�X
			lCurrentIndent--;
		}
	}

	//�\���X�V
	UpdateAllViews(NULL,NULL,NULL);

	//�h�L�������g�_�[�e�B
	SetModifiedFlag(TRUE);

	//�^�C�g���X�V
	CString	objTitle = GetTitle();
	if(	(objTitle.GetLength() <	2) ||
		(objTitle.Right(2) != "	*")
	)
	{
		objTitle +=	" *";
		SetTitle(objTitle);
	}
}
void CEditorDoc::OnUpdateEditSourcearrange(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���݂̃t�@�C���g���q�ɑΉ�����t�H���g�ݒ�							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFontOfCurrentFileExt
	(	const char*	szFileExt
	)
{
	CString objKey;
	CString objFileExt = szFileExt;
	CString objMsg;

	if(objFileExt.IsEmpty())
		objKey = "FileExt_none";
	else if(!objFileExt.CompareNoCase("txt"))
		objKey = "FileExt_txt";
	else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
			!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
		objKey = "FileExt_htm html shtml shtm";
	else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
		objKey = "FileExt_c cpp";
	else if(!objFileExt.CompareNoCase("h"))
		objKey = "FileExt_h";
	else if(!objFileExt.CompareNoCase("asm"))
		objKey = "FileExt_asm";
	else if(!objFileExt.CompareNoCase("asp"))
		objKey = "FileExt_asp";
	else if(!objFileExt.CompareNoCase("frm"))
		objKey = "FileExt_frm";
	else if(!objFileExt.CompareNoCase("bas"))
		objKey = "FileExt_bas";
	else if(!objFileExt.CompareNoCase("pc"))
		objKey = "FileExt_pc";
	else if(!objFileExt.CompareNoCase("log"))
		objKey = "FileExt_log";
	else if(!objFileExt.CompareNoCase("dat"))
		objKey = "FileExt_dat";
	else if(!objFileExt.CompareNoCase("bat"))
		objKey = "FileExt_bat";
	else if(!objFileExt.CompareNoCase("ini"))
		objKey = "FileExt_ini";
	else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
		objKey = "FileExt_mak mk";
	else if(!objFileExt.CompareNoCase("java"))
		objKey = "FileExt_java";
	else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
		objKey = "FileExt_y l";
	else if(!objFileExt.CompareNoCase("wrl"))
		objKey = "FileExt_wrl";
	else
		objKey = "FileExt_other";

	objMsg.LoadString(IDS_FONTNAME_DISP);
	long lDispFontPoint     = m_pApp->GetProfileInt(objKey,"DispFontPoint",120);
	CString objDispFontName = m_pApp->GetProfileString(objKey,"DispFontName",objMsg);
	objMsg.LoadString(IDS_FONTNAME_PRINT);
	long lPrintFontPoint    = m_pApp->GetProfileInt(objKey,"PrintFontPoint",80);
	CString objPrintFontName= m_pApp->GetProfileString(objKey,"PrintFontName",objMsg);

	POSITION pos3 = GetFirstViewPosition();
	while(pos3 != 0)
	{
		CEditorView* pView = (CEditorView*)GetNextView(pos3);
		if(pView!=NULL)
		{
			//�\���t�H���g�ݒ�
			pView->SetDispFontPoint(lDispFontPoint);
			pView->SetDispFontName(objDispFontName);
			//�󎚃t�H���g�ݒ�
			pView->SetPrintFontPoint(lPrintFontPoint);
			pView->SetPrintFontName(objPrintFontName);
		}
	}

	//�r���[�̃f�t�H���g�l��ύX
	m_pApp->WriteProfileInt("CEditorView","DispFontPoint",lDispFontPoint);
	m_pApp->WriteProfileString("CEditorView","DispFontName",objDispFontName);
	m_pApp->WriteProfileInt("CEditorView","PrintFontPoint",lPrintFontPoint);
	m_pApp->WriteProfileString("CEditorView","PrintFontName",objPrintFontName);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���݂̃t�@�C���g���q�ɑΉ�����F�ݒ�									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetColorOfCurrentFileExt
	(	const char*	szFileExt
	)
{
	char	szKeyNo1Fname[_MAX_PATH],szKeyNo2Fname[_MAX_PATH];
	char	szKeyNo3Fname[_MAX_PATH],szKeyNo4Fname[_MAX_PATH];
	CString objKey;
	CString objFileExt = szFileExt;

	if(objFileExt.IsEmpty())
	{
		objKey = "FileExt_none";
		strcpy(szKeyNo1Fname,"keyword_no1.txt");
		strcpy(szKeyNo2Fname,"keyword_no2.txt");
		strcpy(szKeyNo3Fname,"keyword_no3.txt");
		strcpy(szKeyNo4Fname,"keyword_no4.txt");
	}
	else if(!objFileExt.CompareNoCase("txt"))
	{
		objKey = "FileExt_txt";
		strcpy(szKeyNo1Fname,"keyword_no1_txt.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_txt.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_txt.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_txt.txt");
	}
	else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
			!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
	{
		objKey = "FileExt_htm html shtml shtm";
		strcpy(szKeyNo1Fname,"keyword_no1_htm html shtml shtm.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_htm html shtml shtm.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_htm html shtml shtm.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_htm html shtml shtm.txt");
	}
	else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
	{
		objKey = "FileExt_c cpp";
		strcpy(szKeyNo1Fname,"keyword_no1_c cpp.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_c cpp.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_c cpp.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_c cpp.txt");
	}
	else if(!objFileExt.CompareNoCase("h"))
	{
		objKey = "FileExt_h";
		strcpy(szKeyNo1Fname,"keyword_no1_h.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_h.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_h.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_h.txt");
	}
	else if(!objFileExt.CompareNoCase("asm"))
	{
		objKey = "FileExt_asm";
		strcpy(szKeyNo1Fname,"keyword_no1_asm.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_asm.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_asm.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_asm.txt");
	}
	else if(!objFileExt.CompareNoCase("asp"))
	{
		objKey = "FileExt_asp";
		strcpy(szKeyNo1Fname,"keyword_no1_asp.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_asp.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_asp.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_asp.txt");
	}
	else if(!objFileExt.CompareNoCase("frm"))
	{
		objKey = "FileExt_frm";
		strcpy(szKeyNo1Fname,"keyword_no1_frm.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_frm.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_frm.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_frm.txt");
	}
	else if(!objFileExt.CompareNoCase("bas"))
	{
		objKey = "FileExt_bas";
		strcpy(szKeyNo1Fname,"keyword_no1_bas.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_bas.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_bas.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_bas.txt");
	}
	else if(!objFileExt.CompareNoCase("pc"))
	{
		objKey = "FileExt_pc";
		strcpy(szKeyNo1Fname,"keyword_no1_pc.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_pc.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_pc.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_pc.txt");
	}
	else if(!objFileExt.CompareNoCase("log"))
	{
		objKey = "FileExt_log";
		strcpy(szKeyNo1Fname,"keyword_no1_log.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_log.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_log.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_log.txt");
	}
	else if(!objFileExt.CompareNoCase("dat"))
	{
		objKey = "FileExt_dat";
		strcpy(szKeyNo1Fname,"keyword_no1_dat.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_dat.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_dat.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_dat.txt");
	}
	else if(!objFileExt.CompareNoCase("bat"))
	{
		objKey = "FileExt_bat";
		strcpy(szKeyNo1Fname,"keyword_no1_bat.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_bat.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_bat.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_bat.txt");
	}
	else if(!objFileExt.CompareNoCase("ini"))
	{
		objKey = "FileExt_ini";
		strcpy(szKeyNo1Fname,"keyword_no1_ini.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_ini.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_ini.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_ini.txt");
	}
	else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
	{
		objKey = "FileExt_mak mk";
		strcpy(szKeyNo1Fname,"keyword_no1_mak mk.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_mak mk.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_mak mk.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_mak mk.txt");
	}
	else if(!objFileExt.CompareNoCase("java"))
	{
		objKey = "FileExt_java";
		strcpy(szKeyNo1Fname,"keyword_no1_java.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_java.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_java.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_java.txt");
	}
	else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
	{
		objKey = "FileExt_y l";
		strcpy(szKeyNo1Fname,"keyword_no1_y l.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_y l.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_y l.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_y l.txt");
	}
	else if(!objFileExt.CompareNoCase("wrl"))
	{
		objKey = "FileExt_wrl";
		strcpy(szKeyNo1Fname,"keyword_no1_wrl.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_wrl.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_wrl.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_wrl.txt");
	}
	else
	{
		objKey = "FileExt_other";
		strcpy(szKeyNo1Fname,"keyword_no1_other.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_other.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_other.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_other.txt");
	}

	COLORREF FontColor       = m_pApp->GetProfileInt(objKey,"FontColor",RGB(255,255,255));
	COLORREF BackColor       = m_pApp->GetProfileInt(objKey,"BackColor",RGB(0,0,0));
	COLORREF InvertFontColor = m_pApp->GetProfileInt(objKey,"InvertFontColor",RGB(0,0,0));
	COLORREF InvertBackColor = m_pApp->GetProfileInt(objKey,"InvertBackColor",RGB(255,255,255));
	COLORREF KeyNo1Color     = m_pApp->GetProfileInt(objKey,"KeyNo1Color",RGB(0,255,255));
	COLORREF KeyNo2Color     = m_pApp->GetProfileInt(objKey,"KeyNo2Color",RGB(0,255,255));
	COLORREF KeyNo3Color     = m_pApp->GetProfileInt(objKey,"KeyNo3Color",RGB(0,255,255));
	COLORREF KeyNo4Color     = m_pApp->GetProfileInt(objKey,"KeyNo4Color",RGB(0,255,255));
	COLORREF MarkLineColor   = m_pApp->GetProfileInt(objKey,"MarkLineColor",RGB(128,0,128));
	BOOL     bComment1Clr    = m_pApp->GetProfileInt(objKey,"bComment1Clr",1);
	COLORREF CommentColor    = m_pApp->GetProfileInt(objKey,"CommentColor",RGB(0,255,0));
	BOOL     bComment2Clr    = m_pApp->GetProfileInt(objKey,"bComment2Clr",0);
	COLORREF CommentColor2   = m_pApp->GetProfileInt(objKey,"CommentColor2",RGB(0,255,0));
	BOOL     bComment3Clr    = m_pApp->GetProfileInt(objKey,"bComment3Clr",0);
	COLORREF CommentColor3   = m_pApp->GetProfileInt(objKey,"CommentColor3",RGB(0,255,0));
	BOOL     bComment4Clr    = m_pApp->GetProfileInt(objKey,"bComment4Clr",0);
	COLORREF CommentColor4   = m_pApp->GetProfileInt(objKey,"CommentColor4",RGB(0,255,0));
	BOOL     bDispLineNo     = m_pApp->GetProfileInt(objKey,"bDispLineNo",1);
	COLORREF LineNoColor     = m_pApp->GetProfileInt(objKey,"LineNoColor",RGB(0,255,255));
	COLORREF LineNoBackColor = m_pApp->GetProfileInt(objKey,"LineNoBackColor",RGB(0,0,0));
	BOOL     bDispRuler	     = m_pApp->GetProfileInt(objKey,"bDispRuler",1);
	COLORREF RulerColor	     = m_pApp->GetProfileInt(objKey,"RulerColor",RGB(0,0,0));
	COLORREF RulerBackColor  = m_pApp->GetProfileInt(objKey,"RulerBackColor",RGB(192,192,192));
	BOOL     bDispCRLF		 = m_pApp->GetProfileInt(objKey,"bDispCRLF",1);
	COLORREF CRLFColor		 = m_pApp->GetProfileInt(objKey,"CRLFColor",RGB(255,255,0));
	BOOL     bDispTab		 = m_pApp->GetProfileInt(objKey,"bDispTab",0);
	COLORREF TabColor		 = m_pApp->GetProfileInt(objKey,"TabColor",RGB(255,255,255));
	BOOL     bDispZenkakuSpace = m_pApp->GetProfileInt(objKey,"bDispZenkakuSpace",0);
	COLORREF ZenkakuSpaceColor = m_pApp->GetProfileInt(objKey,"ZenkakuSpaceColor",RGB(255,255,255));
	BOOL     bDispUnderLine  = m_pApp->GetProfileInt(objKey,"bDispUnderLine",1);
	COLORREF UnderLineColor  = m_pApp->GetProfileInt(objKey,"UnderLineColor",RGB(255,255,0));
	BOOL     bDispUrl		 = m_pApp->GetProfileInt(objKey,"bDispUrl",1);
	COLORREF UrlColor		 = m_pApp->GetProfileInt(objKey,"UrlColor",RGB(255,0,255));
	BOOL     bDispEMail	     = m_pApp->GetProfileInt(objKey,"bDispEMail",1);
	COLORREF EMailColor	     = m_pApp->GetProfileInt(objKey,"EMailColor",RGB(255,0,255));

	SetFontColor(FontColor);
	DeleteAllKeyWord();
	SetKeyNo1Color(KeyNo1Color,szKeyNo1Fname);
	SetKeyNo2Color(KeyNo2Color,szKeyNo2Fname);
	SetKeyNo3Color(KeyNo3Color,szKeyNo3Fname);
	SetKeyNo4Color(KeyNo4Color,szKeyNo4Fname);
	SetFlagComment1Clr(bComment1Clr);
	SetCommentColor(CommentColor);
	SetFlagComment2Clr(bComment2Clr);
	SetCommentColor2(CommentColor2);
	SetFlagComment3Clr(bComment3Clr);
	SetCommentColor3(CommentColor3);
	SetFlagComment4Clr(bComment4Clr);
	SetCommentColor4(CommentColor4);
	SetFlagUrlColor(bDispUrl);
	SetUrlColor(UrlColor);
	SetFlagEMailColor(bDispEMail);
	SetEMailColor(EMailColor);
	POSITION pos = GetFirstViewPosition();
	while(pos != 0)
	{
		CEditorView* pView = (CEditorView*)GetNextView(pos);
		if(pView!=NULL)
		{
			pView->SetFontColor(FontColor);
			pView->SetBackColor(BackColor);
			pView->SetInvertFontColor(InvertFontColor);
			pView->SetInvertBackColor(InvertBackColor);
			pView->SetMarkLineColor(MarkLineColor);
			pView->SetFlagDispLineNo(bDispLineNo);
			pView->SetLineNoColor(LineNoColor);
			pView->SetLineNoBackColor(LineNoBackColor);
			pView->SetFlagDispRuler(bDispRuler);
			pView->SetRulerColor(RulerColor);
			pView->SetRulerBackColor(RulerBackColor);
			pView->SetFlagDispCRLF(bDispCRLF);
			pView->SetCRLFColor(CRLFColor);
			pView->SetFlagDispTab(bDispTab);
			pView->SetTabColor(TabColor);
			pView->SetFlagDispZenkakuSpace(bDispZenkakuSpace);
			pView->SetZenkakuSpaceColor(ZenkakuSpaceColor);
			pView->SetFlagDispUnderLine(bDispUnderLine);
			pView->SetUnderLineColor(UnderLineColor);
		}
	}

	//�r���[�̃f�t�H���g�l��ύX
	m_pApp->WriteProfileInt("CEditorView","FontColor",FontColor);
	m_pApp->WriteProfileInt("CEditorView","BackColor",BackColor);
	m_pApp->WriteProfileInt("CEditorView","InvertFontColor",InvertFontColor);
	m_pApp->WriteProfileInt("CEditorView","InvertBackColor",InvertBackColor);
	m_pApp->WriteProfileInt("CEditorView","KeyNo1Color",KeyNo1Color);
	m_pApp->WriteProfileInt("CEditorView","KeyNo2Color",KeyNo2Color);
	m_pApp->WriteProfileInt("CEditorView","KeyNo3Color",KeyNo3Color);
	m_pApp->WriteProfileInt("CEditorView","KeyNo4Color",KeyNo4Color);
	m_pApp->WriteProfileInt("CEditorView","MarkLineColor",MarkLineColor);
	m_pApp->WriteProfileInt("CEditorView","bComment1Clr",bComment1Clr);
	m_pApp->WriteProfileInt("CEditorView","CommentColor",CommentColor);
	m_pApp->WriteProfileInt("CEditorView","bComment2Clr",bComment2Clr);
	m_pApp->WriteProfileInt("CEditorView","CommentColor2",CommentColor2);
	m_pApp->WriteProfileInt("CEditorView","bComment3Clr",bComment3Clr);
	m_pApp->WriteProfileInt("CEditorView","CommentColor3",CommentColor3);
	m_pApp->WriteProfileInt("CEditorView","bComment4Clr",bComment4Clr);
	m_pApp->WriteProfileInt("CEditorView","CommentColor4",CommentColor4);
	m_pApp->WriteProfileInt("CEditorView","bDispLineNo",bDispLineNo);
	m_pApp->WriteProfileInt("CEditorView","LineNoColor",LineNoColor);
	m_pApp->WriteProfileInt("CEditorView","LineNoBackColor",LineNoBackColor);
	m_pApp->WriteProfileInt("CEditorView","bDispRuler",bDispRuler);
	m_pApp->WriteProfileInt("CEditorView","RulerColor",RulerColor);
	m_pApp->WriteProfileInt("CEditorView","RulerBackColor",RulerBackColor);
	m_pApp->WriteProfileInt("CEditorView","bDispCRLF",bDispCRLF);
	m_pApp->WriteProfileInt("CEditorView","CRLFColor",CRLFColor);
	m_pApp->WriteProfileInt("CEditorView","bDispTab",bDispTab);
	m_pApp->WriteProfileInt("CEditorView","TabColor",TabColor);
	m_pApp->WriteProfileInt("CEditorView","bDispZenkakuSpace",bDispZenkakuSpace);
	m_pApp->WriteProfileInt("CEditorView","ZenkakuSpaceColor",ZenkakuSpaceColor);
	m_pApp->WriteProfileInt("CEditorView","bDispUnderLine",bDispUnderLine);
	m_pApp->WriteProfileInt("CEditorView","UnderLineColor",UnderLineColor);
	m_pApp->WriteProfileInt("CEditorView","bDispUrl",bDispUrl);
	m_pApp->WriteProfileInt("CEditorView","UrlColor",UrlColor);
	m_pApp->WriteProfileInt("CEditorView","bDispEMail",bDispEMail);
	m_pApp->WriteProfileInt("CEditorView","EMailColor",EMailColor);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�q�k��`�F�b�N����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsAboveURL
	(	long	lLine,
		long	lOffset
	)
{
	CLine* pobjLine = m_objLineManager.GetOneLine(lLine);
	if(pobjLine == NULL)
		return FALSE;

	return pobjLine->IsAboveURL(lOffset);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���|����������`�F�b�N����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsAboveEMail
	(	long	lLine,
		long	lOffset
	)
{
	CLine* pobjLine = m_objLineManager.GetOneLine(lLine);
	if(pobjLine == NULL)
		return FALSE;

	return pobjLine->IsAboveEMail(lOffset);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����v���r���[�t���O�擾												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagPrintPreview()
{
	return m_bIsPrintPreview;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����v���r���[�t���O�ݒ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagPrintPreview
	(	BOOL	bIsPrintPreview
	)
{
	m_bIsPrintPreview = bIsPrintPreview;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�o�C�g���擾����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::GetBytes
	(	long	lLimitLine,				//�w��s
		long*	plLimitBytes,			//�w��s�܂ł̃o�C�g���ԋp�p�o�b�t�@
		long*	plSumBytes				//���o�C�g���ԋp�p�o�b�t�@
	)
{
	m_objLineManager.GetBytes(lLimitLine,plLimitBytes,plSumBytes);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C�����e�ēǂݍ��ݕK�v���`�F�b�N����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsNeedReload()			//TRUE : �K�v����AFALSE : �K�v�Ȃ�
{
	if(m_pobjLastWrittenTime==NULL)
		return FALSE;
	if(	m_strFilePath.GetLength()==0 )
		return FALSE;

	//�t�@�C���ŏI�X�V�����擾
	CFileStatus		objFileStatus;
	CFile::GetStatus(m_strFilePath,objFileStatus);
	CTimeSpan	objSpan(0,0,0,5);
	CTimeSpan	objDiff	 = objFileStatus.m_mtime - (*m_pobjLastWrittenTime);
	CTimeSpan	objDiff2 = (*m_pobjLastWrittenTime) - objFileStatus.m_mtime;
	if(	objDiff>objSpan	|| objDiff2>objSpan )
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C�����e�ēǂݍ��ݏ���											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ReloadDocument(BOOL bIsConfirm)
{
	CString objMsg;

	//�t�@�C�����`�F�b�N
	if(	m_strFilePath.GetLength()==0 )
		return;

	//�t�@�C���I�[�v��
	CFile		objFile;
	if(	!objFile.Open( m_strFilePath, CFile::modeRead, NULL ) )
		return;

	if(	bIsConfirm )
	{
		//�ēǂݍ��݊m�F
		char	szMsg[1024];
		if(IsModified())
		{
			objMsg.LoadString(IDS_CONFIRM_REREAD1);
			sprintf(szMsg,(LPCTSTR)objMsg,m_strFilePath);
			int	nRet = m_pApp->MyMessageBox(szMsg,(MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2),0);
			if(nRet==IDNO)
			{
				objFile.Close();
				return;
			}
		}
		else
		{
			objMsg.LoadString(IDS_CONFIRM_REREAD2);
			sprintf(szMsg,(LPCTSTR)objMsg,m_strFilePath);
			int	nRet = m_pApp->MyMessageBox(szMsg,(MB_YESNO|MB_ICONQUESTION),0);
			if(nRet==IDNO)
			{
				objFile.Close();
				return;
			}
		}
	}

	//�A�[�J�C�u�ɘA��
	CArchive	ar(&objFile,CArchive::load);

	//�����o������
	m_dwFileAttributes = 0;
	m_bGrepMode	= 0;
	m_objLineManager.Initialize();
	m_objUndoManager.Initialize();

	//�t�@�C�����e�ǂݍ���
	Serialize(ar);

	//�L�����b�g�ʒu�������ʒu��
	POSITION pos = GetFirstViewPosition();
	while(pos != 0)
	{
		CEditorView* pView = (CEditorView*)GetNextView(pos);
		pView->MySetCaretPos(CPoint(pView->m_LineNoPos.x,pView->m_RulerPos.y));
		m_pApp->MessagePump();
	}

	//�N���[�Y
	ar.Close();
	objFile.Close();

	//�t�@�C���ŏI�X�V�����擾
	if(m_pobjLastWrittenTime != NULL)
		delete m_pobjLastWrittenTime;
	m_pobjLastWrittenTime = new CTime();
	CFileStatus		objFileStatus;
	CFile::GetStatus(m_strFilePath,objFileStatus);
	(*m_pobjLastWrittenTime) = objFileStatus.m_mtime;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C�����擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::MyGetFileName
	(	char*	pPtr
	)
{
	strcpy(pPtr,m_strFileName);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���p�X�擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::MyGetFilePath
	(	char*	pPtr
	)
{
	strcpy(pPtr,m_strFilePath);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���g���q�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CString CEditorDoc::MyGetFileExt()
{
	CString objStr = m_strFileExt;
	return objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ǂݍ��ݐ�p����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsReadOnly()
{
	if(	(m_dwFileAttributes	& FILE_ATTRIBUTE_READONLY) != 0 )
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ǂݍ��ݐ�p�ݒ�														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::SetReadOnly()
{
	BOOL	bRet=FALSE;
	DWORD	dwOrg =	m_dwFileAttributes;
	CString objMsg;

	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
	if(	m_strFilePath.GetLength() )
	{
		bRet = SetFileAttributes(m_strFilePath,m_dwFileAttributes);
		if(bRet	== FALSE)
		{
			objMsg.LoadString(IDS_ERRCHANGEFILEATTR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			m_dwFileAttributes = dwOrg;
			return FALSE;
		}
	}

	//Undo,Redo	�Ǘ��N���X������
	m_objUndoManager.Initialize();
	//�h�L�������g�N���[��
	SetModifiedFlag(FALSE);

	//�^�C�g���X�V
	CString	objTitle = GetTitle();
	if(	(objTitle.GetLength() >	2) &&
		(objTitle.Right(2) == "	*")
	)
	{
		objTitle = objTitle.Left(objTitle.GetLength() -	2);
		SetTitle(objTitle);
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ǂݍ��ݐ�p�ݒ�														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::SetReadOnly2()
{
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ǂݍ��ݐ�p����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::UnsetReadOnly()
{
	BOOL	bRet=FALSE;
	DWORD	dwOrg =	m_dwFileAttributes;
	CString objMsg;

	m_dwFileAttributes &= ~(FILE_ATTRIBUTE_READONLY);
	if(	m_strFilePath.GetLength() )
	{
		bRet = SetFileAttributes(m_strFilePath,m_dwFileAttributes);
		if(bRet	== FALSE)
		{
			objMsg.LoadString(IDS_ERRCHANGEFILEATTR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			m_dwFileAttributes = dwOrg;
			return FALSE;
		}
	}
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F��h�L�������g����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsEmpty()
{
	if(	GetLineCount() == 0 )
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�m�c�n�\����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsCanUndo()
{
	if(m_objUndoManager.IsCanUndo())
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�q�d�c�n�\����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsCanRedo()
{
	if(m_objUndoManager.IsCanRedo())
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s���擾����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorDoc::GetLineCount()
{
	return m_objLineManager.GetLineCount();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�s�擾����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLine* CEditorDoc::GetOneLine
	(	long	lLine			//�s�ԍ�
	)
{
	return m_objLineManager.GetOneLine(lLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�s�̒����̍ő�l���擾����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorDoc::GetMaxLineLength()
{
	return m_objLineManager.GetMaxLineLength();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w��s�̕����񒷎擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorDoc::GetLineLength(long	lLineNo)
{
	return m_objLineManager.GetLineLength(lLineNo);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�Q�o�C�g�����̑��o�C�g�`�F�b�N����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::MyIsDBCSLeadByteEx	//TRUE:�Q�o�C�g�����̑��o�C�g,FALSE:���̑�
	(	ULONG	lLineNo,	//�s�ԍ�
		ULONG	lOffset		//�s������̃I�t�Z�b�g
	)
{
	if(lOffset < 0)
		return FALSE;

	CLine*	pobjLine = m_objLineManager.GetOneLine(lLineNo);
	if(pobjLine==NULL)//�Ȃ�����
		return FALSE;
	if(lOffset >= (ULONG)pobjLine->GetLength())
		return FALSE;

	BYTE c = (BYTE)pobjLine->GetAt(lOffset);
	BOOL bRet =	IsDBCSLeadByteEx(0,c);
	return bRet;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�^�u�R�[�h�`�F�b�N����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsTabCode //TRUE:�^�u�R�[�h,FALSE:���̑�
	(	ULONG	lLineNo,	//�s�ԍ�
		ULONG	lOffset		//�s������̃I�t�Z�b�g
	)
{
	if(lOffset < 0)
		return FALSE;

	CLine*	pobjLine = m_objLineManager.GetOneLine(lLineNo);
	if(pobjLine==NULL)//�Ȃ�����
		return FALSE;
	if(lOffset >= (ULONG)pobjLine->GetLength())
		return FALSE;

	UCHAR c	= pobjLine->GetAt(lOffset);
	if(c ==	'\t')
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�s�Ɋ܂܂��^�u�̐��擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorDoc::GetTabCount
	(	long	lLineNo		//�s�ԍ�
	)
{
	CLine*	pobjLine = m_objLineManager.GetOneLine(lLineNo);
	if(pobjLine==NULL)//�Ȃ�����
		return 0;

	long lTabCount=0;
	for(long lCnt=0; lCnt<pobjLine->GetLength(); )
	{
		UCHAR c	= pobjLine->GetAt(lCnt);
		if(IsDBCSLeadByteEx(0,c))
		{
			lCnt+=2;
			continue;
		}
		if(c ==	'\t')
			lTabCount++;
		lCnt++;
	}
	return lTabCount;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����o�^����															|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AddCharacter
	(	long	lCurrentLine,			// �����o�^�Ώۂ̍s�ԍ�
		long	lCurrentOffset,			// �����o�^�Ώۂ̗�ԍ��i�޲ĒP�ʁj
		UINT	n1stChar,				// �o�^��������޲�
		UINT	n2ndChar,				// �o�^��������޲�
		BOOL	bInsertKey,				// �}���^�㏑��Ӱ���׸�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i�������́j����
	CUndoItemAddCharacter* pobjUndoItemAddCharacter	= new CUndoItemAddCharacter(this);

	//�������s
	pobjUndoItemAddCharacter->Do( lCurrentLine,
								  lCurrentOffset,
								  n1stChar,
								  n2ndChar,
								  bInsertKey,
								  objCaretPos,
								  objScrollPosLog,
								  bDispRuler,
								  bDispLineNo );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemAddCharacter);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s�R�[�h����														|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessReturnCode
	(	long	lCurrentLine,			// �����o�^�Ώۂ̍s�ԍ�
		long	lCurrentOffset,			// �����o�^�Ώۂ̗�ԍ��i�޲ĒP�ʁj
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i���s�j����
	CUndoItemReturn* pobjUndoItemReturn	= new CUndoItemReturn(this);

	//�������s
	pobjUndoItemReturn->Do(	lCurrentLine,
							lCurrentOffset,
							objCaretPos,
							objScrollPosLog,
							bDispRuler,
							bDispLineNo	);

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemReturn);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FDELETE�R�[�h����														|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessDeleteCode
	(	long	lCurrentLine,			// �����o�^�Ώۂ̍s�ԍ�
		long	lCurrentOffset,			// �����o�^�Ώۂ̗�ԍ��i�޲ĒP�ʁj
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i����DELETE�j����
	CUndoItemDelChar* pobjUndoItemDelChar =	new	CUndoItemDelChar(this);

	//�������s
	pobjUndoItemDelChar->Do( lCurrentLine,
							 lCurrentOffset,
							 objCaretPos,
							 objScrollPosLog,
							 bDispRuler,
							 bDispLineNo );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemDelChar);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FBACK SPACE�R�[�h����													|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessBackSpaceCode
	(	long	lCurrentLine,			// �����o�^�Ώۂ̍s�ԍ�
		long	lCurrentOffset,			// �����o�^�Ώۂ̗�ԍ��i�޲ĒP�ʁj
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i����BACK SPACE�j����
	CUndoItemBackSpaceChar*	pobjUndoItemBackSpaceChar =	new	CUndoItemBackSpaceChar(this);

	//�������s
	pobjUndoItemBackSpaceChar->Do( lCurrentLine,
								   lCurrentOffset,
								   objCaretPos,
								   objScrollPosLog,
								   bDispRuler,
								   bDispLineNo );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemBackSpaceChar);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I��͈̓e�L�X�g�폜����												|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::DeleteSelectedArea
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		BOOL	bToClipBoard,			// �N���b�v�{�[�h�փR�s�[���邩�H(TRUE:����AFALSE:���Ȃ��j
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i�I��͈͏����j����
	CUndoItemCutSelectArea*	pobjUndoItemCutSelectArea =	new	CUndoItemCutSelectArea(this);

	//�������s
	pobjUndoItemCutSelectArea->Do(lSelectStartLine,
								  lSelectStartOffset,
								  lSelectEndLine,
								  lSelectEndOffset,
								  bToClipBoard,
								  objCaretPos,
								  objScrollPosLog,
								  bDispRuler,
								  bDispLineNo );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemCutSelectArea);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����t������															|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessPaste
	(	long	lCurrentLine,			// �L�����b�g�ʒu�̍s�ԍ�
		long	lCurrentOffset,			// �L�����b�g�ʒu�̗�ԍ�
		long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i����t�������j����
	CUndoItemEditPaste*	pobjUndoItemEditPaste =	new	CUndoItemEditPaste(this);

	//�������s
	pobjUndoItemEditPaste->Do(	lCurrentLine,
								lCurrentOffset,
								lSelectStartLine,
								lSelectStartOffset,
								lSelectEndLine,
								lSelectEndOffset,
								objCaretPos,
								objScrollPosLog,
								bDispRuler,
								bDispLineNo	);

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemEditPaste);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�n�k�d�h���b�v����													|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessOleDrop
	(	COleDataObject*	pobjOleDataObject,		// �n�k�d�f�[�^�I�u�W�F�N�g
		long	lCurrentLine,					// �L�����b�g�ʒu�̍s�ԍ�
		long	lCurrentOffset,					// �L�����b�g�ʒu�̗�ԍ�
		CPoint&	objCaretPos,					// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,				// �X�N���[���ʒu
		BOOL	bDispRuler,						// ���[���\���t���O
		BOOL	bDispLineNo						// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i�n�k�d�h���b�v�����j����
	CUndoItemOleDrop* pobjUndoItemOleDrop =	new	CUndoItemOleDrop(this);

	//�������s
	pobjUndoItemOleDrop->Do(	pobjOleDataObject,
								lCurrentLine,
								lCurrentOffset,
								objCaretPos,
								objScrollPosLog,
								bDispRuler,
								bDispLineNo	);

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemOleDrop);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I��P��u������														|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessReplaceSelectWord
	(	CString& objReplaceStr,			// �u���㕶����
		long	lCurrentLine,			// �L�����b�g�ʒu�̍s�ԍ�
		long	lCurrentOffset,			// �L�����b�g�ʒu�̗�ԍ�
		long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i�u�������j����
	CUndoItemReplaceSelectWord*	pobjUndoItemReplaceSelectWord =	new	CUndoItemReplaceSelectWord(this);

	//�������s
	pobjUndoItemReplaceSelectWord->Do(	objReplaceStr,
										lCurrentLine,
										lCurrentOffset,
										lSelectStartLine,
										lSelectStartOffset,
										lSelectEndLine,
										lSelectEndOffset,
										objCaretPos,
										objScrollPosLog,
										bDispRuler,
										bDispLineNo	);

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemReplaceSelectWord);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�͈͑I�����Ă����Ԃł̂s�`�a�L�[���쏈��							|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessTabWhenSelected
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i�͈͑I�����̃^�u�R�[�h�����j����
	CUndoItemTabWhenSelected* pobjUndoItemTabWhenSelected =	new	CUndoItemTabWhenSelected(this);

	//�������s
	pobjUndoItemTabWhenSelected->Do( lSelectStartLine,
									 lSelectStartOffset,
									 lSelectEndLine,
									 lSelectEndOffset,
									 objCaretPos,
									 objScrollPosLog,
									 bDispRuler,
									 bDispLineNo );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemTabWhenSelected);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�͈͑I�����Ă����Ԃł̂r���������{�s�`�a�L�[���쏈��				|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessShiftTabWhenSelected
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i�͈͑I�����̃V�t�g�^�u�R�[�h�����j����
	CUndoItemShiftTabWhenSelected* pobjUndoItemShiftTabWhenSelected	= new CUndoItemShiftTabWhenSelected(this);

	//�������s
	pobjUndoItemShiftTabWhenSelected->Do( lSelectStartLine,
										 lSelectStartOffset,
										 lSelectEndLine,
										 lSelectEndOffset,
										 objCaretPos,
										 objScrollPosLog,
										 bDispRuler,
										 bDispLineNo );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemShiftTabWhenSelected);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�͈͑I�����Ă���̈�̐擪�󔒍폜����								|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessDelTopSpaces
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X����
	CUndoItemDelTopSpaces* pobjUndoItemDelTopSpaces	= new CUndoItemDelTopSpaces(this);

	//�������s
	pobjUndoItemDelTopSpaces->Do( lSelectStartLine,
								 lSelectStartOffset,
								 lSelectEndLine,
								 lSelectEndOffset,
								 objCaretPos,
								 objScrollPosLog,
								 bDispRuler,
								 bDispLineNo );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemDelTopSpaces);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�^�u�|�����p�X�y�[�X�ϊ�����											|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessTab2Space
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo,			// �s�ԍ��\���t���O
		long	lfWidth					// �t�H���g��
	)
{
	//�������\����N���X�i�^�u�|�����p�X�y�[�X�����j����
	CUndoItemTab2Space*	pobjUndoItemTab2Space =	new	CUndoItemTab2Space(this);

	//�������s
	pobjUndoItemTab2Space->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo,
							   lfWidth );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemTab2Space);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���p�X�y�[�X�|���^�u�ϊ�����											|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessSpace2Tab
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo,			// �s�ԍ��\���t���O
		long	lfWidth					// �t�H���g��
	)
{
	//�������\����N���X�i���p�X�y�[�X�|���^�u�����j����
	CUndoItemSpace2Tab*	pobjUndoItemSpace2Tab =	new	CUndoItemSpace2Tab(this);

	//�������s
	pobjUndoItemSpace2Tab->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo,
							   lfWidth );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemSpace2Tab);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�S�p�X�y�[�X�|�����p�X�y�[�X�ϊ�����									|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessSpaceZen2Han
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo,			// �s�ԍ��\���t���O
		long	lfWidth					// �t�H���g��
	)
{
	//�������\����N���X�i�S�p�X�y�[�X�|�����p�X�y�[�X�����j����
	CUndoItemSpaceZen2Han* pobjUndoItemSpaceZen2Han	= new CUndoItemSpaceZen2Han(this);

	//�������s
	pobjUndoItemSpaceZen2Han->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo,
							   lfWidth );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemSpaceZen2Han);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���p�X�y�[�X�|���S�p�X�y�[�X�ϊ�����									|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessSpaceHan2Zen
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo,			// �s�ԍ��\���t���O
		long	lfWidth					// �t�H���g��
	)
{
	//�������\����N���X�i���p�X�y�[�X�|���S�p�X�y�[�X�����j����
	CUndoItemSpaceHan2Zen* pobjUndoItemSpaceHan2Zen	= new CUndoItemSpaceHan2Zen(this);

	//�������s
	pobjUndoItemSpaceHan2Zen->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo,
							   lfWidth );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemSpaceHan2Zen);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���p���Ł|���S�p�J�^�J�i�ϊ�����										|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessKatakanaHan2Zen
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo,			// �s�ԍ��\���t���O
		long	lfWidth					// �t�H���g��
	)
{
	//�������\����N���X�i���p���Ł|���S�p�J�^�J�i�����j����
	CUndoItemKatakanaHan2Zen* pobjUndoItemKatakanaHan2Zen =	new	CUndoItemKatakanaHan2Zen(this);

	//�������s
	pobjUndoItemKatakanaHan2Zen->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo,
							   lfWidth );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemKatakanaHan2Zen);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�S�p�J�^�J�i�|�����p���ŕϊ�����										|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessKatakanaZen2Han
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo,			// �s�ԍ��\���t���O
		long	lfWidth					// �t�H���g��
	)
{
	//�������\����N���X�i�S�p�J�^�J�i�|�����p���ŏ����j����
	CUndoItemKatakanaZen2Han* pobjUndoItemKatakanaZen2Han =	new	CUndoItemKatakanaZen2Han(this);

	//�������s
	pobjUndoItemKatakanaZen2Han->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo,
							   lfWidth );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemKatakanaZen2Han);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���p�p�����|���S�p�p�����ϊ�����										|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessAlnumHan2Zen
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo,			// �s�ԍ��\���t���O
		long	lfWidth					// �t�H���g��
	)
{
	//�������\����N���X�i���p�p�����|���S�p�p���������j����
	CUndoItemAlnumHan2Zen* pobjUndoItemAlnumHan2Zen	= new CUndoItemAlnumHan2Zen(this);

	//�������s
	pobjUndoItemAlnumHan2Zen->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo,
							   lfWidth );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemAlnumHan2Zen);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�S�p�p�����|�����p�p�����ϊ�����										|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessAlnumZen2Han
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo,			// �s�ԍ��\���t���O
		long	lfWidth					// �t�H���g��
	)
{
	//�������\����N���X�i�S�p�p�����|�����p�p���������j����
	CUndoItemAlnumZen2Han* pobjUndoItemAlnumZen2Han	= new CUndoItemAlnumZen2Han(this);

	//�������s
	pobjUndoItemAlnumZen2Han->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo,
							   lfWidth );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemAlnumZen2Han);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�͈͑I��̈�ւ̂b�^�b�{�{�R�����g�}������							|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessInsCppComment
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i�͈͑I��̈�ւ̂b�^�b�{�{�R�����g�}�������j����
	CUndoItemInsCppComment*	pobjUndoItemInsCppComment =	new	CUndoItemInsCppComment(this);

	//�������s
	pobjUndoItemInsCppComment->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemInsCppComment);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�͈͑I��̈�̂b�^�b�{�{�R�����g�폜����								|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessDelCppComment
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i�͈͑I��̈�̂b�^�b�{�{�R�����g�폜�����j����
	CUndoItemDelCppComment*	pobjUndoItemDelCppComment =	new	CUndoItemDelCppComment(this);

	//�������s
	pobjUndoItemDelCppComment->Do( lSelectStartLine,
								   lSelectStartOffset,
								   lSelectEndLine,
								   lSelectEndOffset,
								   objCaretPos,
								   objScrollPosLog,
								   bDispRuler,
								   bDispLineNo );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemDelCppComment);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�͈͑I��̈�ւ̂u�a�R�����g�}������									|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessInsVbComment
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i�͈͑I��̈�ւ̂u�a�R�����g�}�������j����
	CUndoItemInsVbComment* pobjUndoItemInsVbComment	= new CUndoItemInsVbComment(this);

	//�������s
	pobjUndoItemInsVbComment->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemInsVbComment);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�͈͑I��̈�̂u�a�R�����g�폜����									|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessDelVbComment
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i�͈͑I��̈�̂u�a�R�����g�폜�����j����
	CUndoItemDelVbComment* pobjUndoItemDelVbComment	= new CUndoItemDelVbComment(this);

	//�������s
	pobjUndoItemDelVbComment->Do( lSelectStartLine,
								  lSelectStartOffset,
								  lSelectEndLine,
								  lSelectEndOffset,
								  objCaretPos,
								  objScrollPosLog,
								  bDispRuler,
								  bDispLineNo );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemDelVbComment);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�͈͑I��̈�ւ̃A�Z���u���R�����g�}������							|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessInsAsmComment
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i�͈͑I��̈�ւ̃A�Z���u���R�����g�}�������j����
	CUndoItemInsAsmComment*	pobjUndoItemInsAsmComment =	new	CUndoItemInsAsmComment(this);

	//�������s
	pobjUndoItemInsAsmComment->Do( lSelectStartLine,
								   lSelectStartOffset,
								   lSelectEndLine,
								   lSelectEndOffset,
								   objCaretPos,
								   objScrollPosLog,
								   bDispRuler,
								   bDispLineNo );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemInsAsmComment);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�͈͑I��̈�̃A�Z���u���R�����g�폜����								|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessDelAsmComment
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i�͈͑I��̈�̃A�Z���u���R�����g�폜�����j����
	CUndoItemDelAsmComment*	pobjUndoItemDelAsmComment =	new	CUndoItemDelAsmComment(this);

	//�������s
	pobjUndoItemDelAsmComment->Do( lSelectStartLine,
								   lSelectStartOffset,
								   lSelectEndLine,
								   lSelectEndOffset,
								   objCaretPos,
								   objScrollPosLog,
								   bDispRuler,
								   bDispLineNo );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemDelAsmComment);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�͈͑I��̈�ւ̈��p�����}������										|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessInsQuote
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i�͈͑I��̈�ւ̃��[�����p�����}�������j����
	CUndoItemInsQuote*	pobjUndoItemInsQuote = new CUndoItemInsQuote(this);

	//�������s
	pobjUndoItemInsQuote->Do( lSelectStartLine,
							  lSelectStartOffset,
							  lSelectEndLine,
							  lSelectEndOffset,
							  objCaretPos,
							  objScrollPosLog,
							  bDispRuler,
							  bDispLineNo );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemInsQuote);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�͈͑I��̈�̈��p�����폜����										|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessDelQuote
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i�͈͑I��̈�̃��[�����p�����폜�����j����
	CUndoItemDelQuote*	pobjUndoItemDelQuote = new CUndoItemDelQuote(this);

	//�������s
	pobjUndoItemDelQuote->Do( lSelectStartLine,
							  lSelectStartOffset,
							  lSelectEndLine,
							  lSelectEndOffset,
							  objCaretPos,
							  objScrollPosLog,
							  bDispRuler,
							  bDispLineNo );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemDelQuote);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���t������}������													|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessInsDay
	(	long		lCurrentLine,			// �L�����b�g�ʒu�s
		long		lCurrentOffset,			// �L�����b�g�ʒu�I�t�Z�b�g
		CString&	objDayStr,				// ���t������
		CPoint&		objCaretPos,			// �L�����b�g�ʒu
		CPoint&		objScrollPosLog,		// �X�N���[���ʒu
		BOOL		bDispRuler,				// ���[���\���t���O
		BOOL		bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i���t������}�������j����
	CUndoItemInsDay* pobjUndoItemInsDay	= new CUndoItemInsDay(this);

	//�������s
	pobjUndoItemInsDay->Do(	lCurrentLine,
							lCurrentOffset,
							objDayStr,
							objCaretPos,
							objScrollPosLog,
							bDispRuler,
							bDispLineNo	);

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemInsDay);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����������}������													|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessInsTime
	(	long		lCurrentLine,			// �L�����b�g�ʒu�s
		long		lCurrentOffset,			// �L�����b�g�ʒu�I�t�Z�b�g
		CString&	objTimeStr,				// ����������
		CPoint&		objCaretPos,			// �L�����b�g�ʒu
		CPoint&		objScrollPosLog,		// �X�N���[���ʒu
		BOOL		bDispRuler,				// ���[���\���t���O
		BOOL		bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i����������}�������j����
	CUndoItemInsTime* pobjUndoItemInsTime =	new	CUndoItemInsTime(this);

	//�������s
	pobjUndoItemInsTime->Do( lCurrentLine,
							lCurrentOffset,
							objTimeStr,
							objCaretPos,
							objScrollPosLog,
							bDispRuler,
							bDispLineNo	);

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemInsTime);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�p���|���啶���ϊ�����												|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessAl2Upper
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i�p���|���啶���ϊ������j����
	CUndoItemAl2Upper* pobjUndoItemAl2Upper	= new CUndoItemAl2Upper(this);

	//�������s
	pobjUndoItemAl2Upper->Do( lSelectStartLine,
							  lSelectStartOffset,
							  lSelectEndLine,
							  lSelectEndOffset,
							  objCaretPos,
							  objScrollPosLog,
							  bDispRuler,
							  bDispLineNo );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemAl2Upper);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�p���|���������ϊ�����												|*/
/*|	���	�F																		|*/
/*|	���l	�F�f�U�C���p�^�[���ł̂b�������������p�^�[�����g�p						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessAl2Lower
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint&	objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�������\����N���X�i�p���|���������ϊ������j����
	CUndoItemAl2Lower* pobjUndoItemAl2Lower	= new CUndoItemAl2Lower(this);

	//�������s
	pobjUndoItemAl2Lower->Do( lSelectStartLine,
							  lSelectStartOffset,
							  lSelectEndLine,
							  lSelectEndOffset,
							  objCaretPos,
							  objScrollPosLog,
							  bDispRuler,
							  bDispLineNo );

	//�������\����Ǘ��N���X�֓o�^
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemAl2Lower);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�^�u�T�C�Y�擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorDoc::GetTabSize()
{
	return m_objLineManager.GetTabSize();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�^�u�T�C�Y�ݒ菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetTabSize(long lTabSize)
{
	m_objLineManager.SetTabSize(lTabSize);
	//�r���[�֍X�V�ʒm
	UpdateAllViews(NULL,NULL,NULL);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s�R�[�h�敪�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorDoc::GetKaigyoKind()
{
	return m_objLineManager.GetKaigyoKind();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s�R�[�h�敪�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetKaigyoKind
	(	long	lKaigyoKind
	)
{
	if(lKaigyoKind == m_objLineManager.GetKaigyoKind())	//�ύX�Ȃ�
		return;

	m_objLineManager.SetKaigyoKind(lKaigyoKind);
	if(	!IsGrep()   && !IsDump()    && !IsConv()	 && 
		!IsWCount() && !IsFSearch() && !IsReadOnly() && 
		!IsBinary() && !IsEncript() && !IsDecript()  &&
		!IsEncode() && !IsCompare() && !IsDirCmp()
	)
	{
		//�h�L�������g�_�[�e�B
		SetModifiedFlag(TRUE);

		//�^�C�g���X�V
		CString	objTitle = GetTitle();
		if(	(objTitle.GetLength() <	2) ||
			(objTitle.Right(2) != "	*")
		)
		{
			objTitle +=	" *";
			SetTitle(objTitle);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I��͈̓e�L�X�g�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLine* CEditorDoc::MyGetSelectedText
	(	long	lSelectStartLine,		//�I���J�n�s�ԍ�
		long	lSelectStartOffset,		//�I���J�n��ԍ�
		long	lSelectEndLine,			//�I���I���s�ԍ�
		long	lSelectEndOffset		//�I���I����ԍ�
	)
{
	return m_objLineManager.MyGetSelectedText(lSelectStartLine,
											  lSelectStartOffset,
											  lSelectEndLine,
											  lSelectEndOffset);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�s�ݒ菈��															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetOneLine
	(	long		lLineNo,		//�s�ԍ�
		CLine*		pobjLine		//�s
	)
{
	m_objLineManager.SetOneLine(lLineNo,pobjLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w��s�폜����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::DeleteOneLine
	(	long	lLineNo			//�폜�Ώۍs
	)
{
	m_objLineManager.DeleteOneLine(lLineNo);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s�}������															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::InsertOneLine
	(	long	lLineNo,
		CLine*	pobjLine
	)
{
	m_objLineManager.InsertOneLine(lLineNo,pobjLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�[�N�t���O�ݒ菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagMark
	(	long	lLine,
		BOOL	bMark
	)
{
	m_objLineManager.SetFlagMark(lLine,bMark);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�[�N�t���O�擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagMark
	(	long	lLine
	)
{
	return m_objLineManager.GetFlagMark(lLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�[�N�s���擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorDoc::GetMarkLineCount()
{
	return m_objLineManager.GetMarkLineCount();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ۑ����s���󔒍폜�t���O�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagDelEndSpacesOnSave()
{
	return m_bDelEndSpacesOnSave;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ۑ����s���󔒍폜�t���O�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagDelEndSpacesOnSave
	(	BOOL	bDelEndSpacesOnSave
	)
{
	m_bDelEndSpacesOnSave =	bDelEndSpacesOnSave;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ۑ����󔒁|���^�u�ϊ��t���O�擾����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagSpace2TabOnSave()
{
	return m_bSpace2TabOnSave;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ۑ����󔒁|���^�u�ϊ��t���O�ݒ菈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagSpace2TabOnSave
	(	BOOL	bSpace2TabOnSave
	)
{
	m_bSpace2TabOnSave = bSpace2TabOnSave;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I��̈�̂t�t�c��������������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::UuDecode
	(	long	lSelectStartLine,
		long	lSelectStartOffset,
		long	lSelectEndLine,
		long	lSelectEndOffset
	)
{
	CUudecoder	objUudecoder;
	CString objBuf;
	BYTE	byBuf[128];
	LONG	lLen=0,lLineNo=0;
	CString objMsg;

	/* �w�b�_�s���� */
	CLine* pobjLine	= (CLine*)GetOneLine(lSelectStartLine);
	if(pobjLine == NULL)
	{
		objMsg.LoadString(IDS_NO_UUDECODEHEADER);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if( pobjLine->Compare(lSelectStartOffset,(const unsigned char*)"begin",5) &&
		pobjLine->Compare(lSelectStartOffset,(const unsigned char*)"BEGIN",5) )
	{
		objMsg.LoadString(IDS_NO_UUDECODEHEADER);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	/* �o�̓t�@�C�����擾 */
	char	szFileName[_MAX_FNAME];
	CString objStrFName=_T("");
	CFileDialog	objFileDialog(FALSE);
	objMsg.LoadString(IDS_UUDECODE_OUTFILE_TITLE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if( objFileDialog.DoModal() != IDOK )
		return;

	//�t�@�C�����擾
	objStrFName = objFileDialog.GetPathName();
	strcpy(szFileName,(LPCTSTR)objStrFName);

	/* �o�̓t�@�C���I�[�v�� */
	CFile	objFile;
	if( objFile.Open( szFileName, (CFile::modeCreate|CFile::modeWrite)) == NULL )
	{
		/* �t�@�C���I�[�v���G���[ */
		objMsg.LoadString(IDS_FOPEN_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	CArchive	ar(&objFile,CArchive::store);

	/* �t�t������������ */
	for(lLineNo=lSelectStartLine+1; lLineNo<lSelectEndLine; lLineNo++)
	{
		pobjLine = (CLine*)GetOneLine(lLineNo);
		if(pobjLine == NULL)
		{
			objMsg.LoadString(IDS_NO_UUDECODEFOOTER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}

		objBuf.Empty();
		memset(byBuf,0,sizeof(byBuf));
		lLen=0;
		objBuf = pobjLine->String();
		if(!objBuf.IsEmpty())
		{
			if(!objUudecoder.StringDecode(&objBuf,byBuf,&lLen))
				break;
			ar.Write(byBuf,lLen);
		}
	}

	/* �N���[�Y */
	ar.Close();
	objFile.Close();

	objMsg.LoadString(IDS_END_UUDECODE);
	m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I��̈�̂a�������U�S�f�R�[�h����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::Base64Decode
	(	long	lSelectStartLine,
		long	lSelectStartOffset,
		long	lSelectEndLine,
		long	lSelectEndOffset
	)
{
	CBase64		objBase64;
	LONG		lLine=0,lLen=0;
	CString		objBuf;
	BYTE		byBuf[128];
	CString		objMsg;

	//�o�̓t�@�C�����擾
	char	szFileName[_MAX_FNAME];
	CString objStrFName=_T("");
	CFileDialog	objFileDialog(FALSE);
	objMsg.LoadString(IDS_BASE64_OUTFILE_TITLE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if( objFileDialog.DoModal() != IDOK )
		return;

	//�t�@�C�����擾
	objStrFName = objFileDialog.GetPathName();
	strcpy(szFileName,(LPCTSTR)objStrFName);

	//�o�̓t�@�C���I�[�v��
	CFile	objFile;
	if( objFile.Open( szFileName, (CFile::modeCreate|CFile::modeWrite)) == 0 )
	{
		//�t�@�C���I�[�v���G���[
		objMsg.LoadString(IDS_FOPEN_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	CArchive	ar(&objFile,CArchive::store);

	//�a�`�r�d�U�S�f�R�[�h
	for(lLine=lSelectStartLine; lLine<=lSelectEndLine; lLine++)
	{
		CLine* pobjLine = (CLine*)GetOneLine(lLine);
		if(pobjLine == NULL)
		{
			objMsg.LoadString(IDS_ILLEGAL_LINE_BASE64DECODE);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}

		objBuf.Empty();
		memset(byBuf,0,sizeof(byBuf));
		lLen=0;
		if(lLine==lSelectStartLine)
			objBuf = pobjLine->Data(lSelectStartOffset);
		else
			objBuf = pobjLine->String();

		if(!objBuf.IsEmpty())
		{
			if(!objBase64.StringDecode(&objBuf,byBuf,&lLen))
				break;
			ar.Write(byBuf,lLen);
		}
	}

	//�N���[�Y
	ar.Close();
	objFile.Close();

	objMsg.LoadString(IDS_END_BASE64DECODE);
	m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I��̈�̂p�����������|�o�����������������f�R�[�h����				|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::QuotedPrintableDecode
	(	long	lSelectStartLine,
		long	lSelectStartOffset,
		long	lSelectEndLine,
		long	lSelectEndOffset
	)
{
	CQuotedPrintable	objQuotedPrintable;
	LONG	lLine=0,lLen=0;
	CString objBuf;
	BYTE		byBuf[128];
	CString objMsg;

	//�o�̓t�@�C�����擾
	char	szFileName[_MAX_FNAME];
	CString objStrFName=_T("");
	CFileDialog	objFileDialog(FALSE);
	objMsg.LoadString(IDS_QUOTE_OUTFILE_TITLE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if( objFileDialog.DoModal() != IDOK )
		return;

	//�t�@�C�����擾
	objStrFName = objFileDialog.GetPathName();
	strcpy(szFileName,(LPCTSTR)objStrFName);

	//�o�̓t�@�C���I�[�v��
	CFile	objFile;
	if( objFile.Open( szFileName, (CFile::modeCreate|CFile::modeWrite)) == NULL )
	{
		//�t�@�C���I�[�v���G���[
		objMsg.LoadString(IDS_FOPEN_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	CArchive	ar(&objFile,CArchive::store);

	//�p�����������o�����������������f�R�[�h
	for(lLine=lSelectStartLine; lLine<=lSelectEndLine; lLine++)
	{
		CLine* pobjLine = (CLine*)GetOneLine(lLine);
		if(pobjLine == NULL)
		{
			objMsg.LoadString(IDS_ILLEGAL_LINE_QUOTEDDECODE);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}

		objBuf.Empty();
		memset(byBuf,0,sizeof(byBuf));
		lLen=0;
		if(lLine==lSelectStartLine)
			objBuf = pobjLine->Data(lSelectStartOffset);
		else
			objBuf = pobjLine->String();

		if(!objBuf.IsEmpty())
		{
			if(!objQuotedPrintable.StringDecode(&objBuf,byBuf,&lLen))
				break;
			ar.Write(byBuf,lLen);
			if(lLine!=lSelectEndLine)
			{
				//�s����"="�łȂ��ꍇ�ɂ́A���������f�[�^�̉��s�Ƃ��ĉ��߂���
				if(objBuf.GetAt(objBuf.GetLength()-1)!='=')
				{
					ar << (BYTE)'\r';
					ar << (BYTE)'\n';
				}
			}
		}
	}

	//�N���[�Y
	ar.Close();
	objFile.Close();

	objMsg.LoadString(IDS_END_QUOTEDDECODE);
	m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc �p�u���b�N�֐��i�J���[�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h�J���[�����O����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::KeyWordColoring
	(	CLine*	pobjLine
	)
{
	m_objColorManager.KeyWordColoring(pobjLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ʏ�̕����F�ݒ菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFontColor
	(	COLORREF	color
	)
{
	m_objColorManager.SetFontColor(color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�b�E�b�{�{�R�����g�L���t���O�擾����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagComment1Clr()
{
	return m_objColorManager.GetFlagComment1Clr();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�b�E�b�{�{�R�����g�L���t���O�ݒ菈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagComment1Clr
	(	BOOL	bComment1Clr
	)
{
	m_objColorManager.SetFlagComment1Clr(bComment1Clr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�F�擾�����i�b�^�b�{�{�p�j									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetCommentColor()
{
	return m_objColorManager.GetCommentColor();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�F�ݒ菈���i�b�^�b�{�{�p�j									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetCommentColor
	(	COLORREF	color
	)
{
	m_objColorManager.SetCommentColor(color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�a�R�����g�L���t���O�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagComment2Clr()
{
	return m_objColorManager.GetFlagComment2Clr();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�a�R�����g�L���t���O�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagComment2Clr
	(	BOOL	bComment2Clr
	)
{
	m_objColorManager.SetFlagComment2Clr(bComment2Clr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�F�擾�����i�u�a�p�j											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetCommentColor2()
{
	return m_objColorManager.GetCommentColor2();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�F�ݒ菈���i�u�a�p�j											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetCommentColor2
	(	COLORREF	color
	)
{
	m_objColorManager.SetCommentColor2(color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A�Z���u���R�����g�L���t���O�擾����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagComment3Clr()
{
	return m_objColorManager.GetFlagComment3Clr();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A�Z���u���R�����g�L���t���O�ݒ菈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagComment3Clr
	(	BOOL	bComment3Clr
	)
{
	m_objColorManager.SetFlagComment3Clr(bComment3Clr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�F�擾�����i�A�Z���u���p�j									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetCommentColor3()
{
	return m_objColorManager.GetCommentColor3();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�F�ݒ菈���i�A�Z���u���p�j									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetCommentColor3
	(	COLORREF	color
	)
{
	m_objColorManager.SetCommentColor3(color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���p�����R�����g�L���t���O�擾����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagComment4Clr()
{
	return m_objColorManager.GetFlagComment4Clr();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���p�����R�����g�L���t���O�ݒ菈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagComment4Clr
	(	BOOL	bComment4Clr
	)
{
	m_objColorManager.SetFlagComment4Clr(bComment4Clr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�F�擾�����i���p�����p�j										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetCommentColor4()
{
	return m_objColorManager.GetCommentColor4();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�F�ݒ菈���i���p�����p�j										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetCommentColor4
	(	COLORREF	color
	)
{
	m_objColorManager.SetCommentColor4(color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�q�k�L���t���O�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagUrlColor()
{
	return m_objColorManager.GetFlagUrlColor();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�q�k�L���t���O�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagUrlColor
	(	BOOL	bUrl
	)
{
	m_objColorManager.SetFlagUrlColor(bUrl);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�q�k�F�擾����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetUrlColor()
{
	return m_objColorManager.GetUrlColor();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�q�k�F�ݒ菈��														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetUrlColor
	(	COLORREF	color
	)
{
	m_objColorManager.SetUrlColor(color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���|���������L���t���O�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagEMailColor()
{
	return m_objColorManager.GetFlagEMailColor();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���|���������L���t���O�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagEMailColor
	(	BOOL	bEMail
	)
{
	m_objColorManager.SetFlagEMailColor(bEMail);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���|���������F�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetEMailColor()
{
	return m_objColorManager.GetEMailColor();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���|���������F�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetEMailColor
	(	COLORREF	color
	)
{
	m_objColorManager.SetEMailColor(color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�S�L�[���[�h�̍폜����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::DeleteAllKeyWord()
{
	m_objColorManager.DeleteAllKeyWord();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h�m���P�̕����F�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetKeyNo1Color()
{
	return m_objColorManager.GetKeyNo1Color();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h��`�t�@�C���i�m�n�P�j�ǂݍ��݁��F�ݒ菈��					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetKeyNo1Color
	(	COLORREF	color,
		char*		szKeyNo1Fname
	)
{
	m_objColorManager.SetKeyNo1Color(color,szKeyNo1Fname);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h�m���Q�̕����F�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetKeyNo2Color()
{
	return m_objColorManager.GetKeyNo2Color();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h��`�t�@�C���i�m�n�Q�j�ǂݍ��݁��F�ݒ菈��					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetKeyNo2Color
	(	COLORREF	color,
		char*		szKeyNo2Fname
	)
{
	m_objColorManager.SetKeyNo2Color(color,szKeyNo2Fname);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h�m���R�̕����F�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetKeyNo3Color()
{
	return m_objColorManager.GetKeyNo3Color();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h��`�t�@�C���i�m�n�R�j�ǂݍ��݁��F�ݒ菈��					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetKeyNo3Color
	(	COLORREF	color,
		char*		szKeyNo3Fname
	)
{
	m_objColorManager.SetKeyNo3Color(color,szKeyNo3Fname);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h�m���S�̕����F�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetKeyNo4Color()
{
	return m_objColorManager.GetKeyNo4Color();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h��`�t�@�C���i�m�n�S�j�ǂݍ��݁��F�ݒ菈��					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetKeyNo4Color
	(	COLORREF	color,
		char*		szKeyNo4Fname
	)
{
	m_objColorManager.SetKeyNo4Color(color,szKeyNo4Fname);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc �p�u���b�N�֐��i�f�q�d�o�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�q�d�o���[�h�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetGrepMode
	(	BOOL	bGrepMode			//TRUE : �f�q�d�o�p�AFALSE : �f�q�d�o�ł͂Ȃ�
	)
{
	m_bGrepMode	= bGrepMode;

	//�ǂݍ��ݐ�p�Ƃ���
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�q�d�o����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsGrep()			//TRUE : �f�q�d�o�p�AFALSE : �f�q�d�o�ł͂Ȃ�
{
	return m_bGrepMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�q�d�o���ʂP�s�ǉ�													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendGrepLine
	(	CLine&	objGrepLine
	)
{
	m_objLineManager.AppendGrepLine(objGrepLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�g�s�s�o�_�E�����[�h���ʂP�s�ǉ�										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendHttpLine
	(	CLine&	objHttpLine
	)
{
	m_objLineManager.AppendHttpLine(objHttpLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w��s�W�����v���M													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SendJumpMsg
	(	long	lDestLine,				// �W�����v��s�ԍ��i�P�I���W���j
		BOOL	bIsInvalidate			// �ĕ`�悷�邩�H
	)
{
	POSITION pos = GetFirstViewPosition();
	while(pos != 0)
	{
		CEditorView* pView = (CEditorView*)GetNextView(pos);
		//�w��s�ւ̃W�����v�w��
		pView->Jump2LineOffset((lDestLine-1),0);
		if(bIsInvalidate)
			pView->Invalidate();

		m_pApp->MessagePump();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc �p�u���b�N�֐��i�����R�[�h�ϊ��֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����R�[�h�ϊ����ʕ\�����[�h�ݒ菈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetConvMode
	(	BOOL	bConvMode			//TRUE : �����R�[�h�ϊ����ʕ\���p�AFALSE : �ł͂Ȃ�
	)
{
	m_bConvMode	= bConvMode;

	//�ǂݍ��ݐ�p�Ƃ���
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����R�[�h�ϊ����ʕ\�����[�h��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsConv()
{
	return m_bConvMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����R�[�h�ϊ����ʂP�s�ǉ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendConvLine
	(	CLine&	objConvLine
	)
{
	m_objLineManager.AppendConvLine(objConvLine);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc �p�u���b�N�֐��i�_���v�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_���v���ʕ\�����[�h�ݒ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetDumpMode
	(	BOOL	bDumpMode			//TRUE : �_���v�p�AFALSE : �_���v�ł͂Ȃ�
	)
{
	m_bDumpMode	= bDumpMode;

	//�ǂݍ��ݐ�p�Ƃ���
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_���v���ʕ\�����[�h����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsDump()
{
	return m_bDumpMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_���v���ʂP�s�ǉ�													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendDumpLine
	(	CLine&	objDumpLine
	)
{
	m_objLineManager.AppendDumpLine(objDumpLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�b�q�A�k�e�̐��擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::GetCRLFCount
	(	long*	plCRCount,		//CR�̐�(out)
		long*	plLFCount		//LF�̐�(out)
	)
{
	m_objLineManager.GetCRLFCount(plCRCount,plLFCount);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc �p�u���b�N�֐��i���[�h�J�E���g�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[�h�J�E���g���ʕ\�����[�h�ݒ�										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetWCountMode
	(	BOOL	bWCountMode			//TRUE : ���[�h�J�E���g�p�AFALSE : ���[�h�J�E���g�ł͂Ȃ�
	)
{
	m_bWCountMode =	bWCountMode;

	//�ǂݍ��ݐ�p�Ƃ���
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[�h�J�E���g���ʕ\�����[�h����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsWCount()
{
	return m_bWCountMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[�h�J�E���g���ʂP�s�ǉ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendWCountLine
	(	CLine&	objWCountLine
	)
{
	m_objLineManager.AppendWCountLine(objWCountLine);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc �p�u���b�N�֐��i�t�@�C�������֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���������[�h�ݒ菈��											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFSearchMode
	(	BOOL	bFSearchMode			//TRUE : �t�@�C�������p�AFALSE : �t�@�C�������ł͂Ȃ�
	)
{
	m_bFSearchMode = bFSearchMode;

	//�ǂݍ��ݐ�p�Ƃ���
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����������														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsFSearch()			//TRUE : �t�@�C�������p�AFALSE : �t�@�C�������ł͂Ȃ�
{
	return m_bFSearchMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���������ʂP�s�ǉ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendFSearchLine
	(	CLine&	objFSearchLine
	)
{
	m_objLineManager.AppendFSearchLine(objFSearchLine);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc �p�u���b�N�֐��i�t�@�C���Í����֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���Í������ʕ\�����[�h�ݒ菈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetEncriptMode
	(	BOOL	bEncriptMode
	)
{
	m_bEncriptMode = bEncriptMode;

	//�ǂݍ��ݐ�p�Ƃ���
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���Í������ʕ\�����[�h����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsEncript()
{
	return m_bEncriptMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���Í������ʂP�s�ǉ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendEncriptLine
	(	CLine&	objEncriptLine
	)
{
	m_objLineManager.AppendEncriptLine(objEncriptLine);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc �p�u���b�N�֐��i�t�@�C���������֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C�����������ʕ\�����[�h�ݒ菈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetDecriptMode
	(	BOOL	bDecriptMode
	)
{
	m_bDecriptMode = bDecriptMode;

	//�ǂݍ��ݐ�p�Ƃ���
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C�����������ʕ\�����[�h����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsDecript()
{
	return m_bDecriptMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C�����������ʂP�s�ǉ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendDecriptLine
	(	CLine&	objDecriptLine
	)
{
	m_objLineManager.AppendDecriptLine(objDecriptLine);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc �p�u���b�N�֐��i�G���R�[�h�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�G���R�[�h���ʕ\�����[�h�ݒ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetEncodeMode
	(	BOOL	bEncodeMode
	)
{
	m_bEncodeMode = bEncodeMode;

	//�ǂݍ��ݐ�p�Ƃ���
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�G���R�[�h���ʕ\�����[�h����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsEncode()
{
	return m_bEncodeMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�G���R�[�h���ʂP�s�ǉ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendEncodeLine
	(	CLine&	objEncodeLine
	)
{
	m_objLineManager.AppendEncodeLine(objEncodeLine);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc �p�u���b�N�֐��i�t�@�C����r�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r���ʕ\�����[�h�ݒ�										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetCompareMode
	(	BOOL		bCompareMode,
		char*		szFileCmpFile1,
		char*		szFileCmpFile2
	)
{
	m_bCompareMode = bCompareMode;
	m_objFileCmpFile1 = szFileCmpFile1;
	m_objFileCmpFile2 = szFileCmpFile2;

	//�ǂݍ��ݐ�p�Ƃ���
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r���ʕ\�����[�h����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsCompare()
{
	return m_bCompareMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r�@��r�t�@�C���擾										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::GetCompareFiles
	(	CString&	objFileCmpFile1,	// �t�@�C����r�p���t�@�C���p�X
		CString&	objFileCmpFile2		// �t�@�C����r�p���t�@�C���p�X
	)
{
	objFileCmpFile1	= m_objFileCmpFile1;
	objFileCmpFile2	= m_objFileCmpFile2;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r���ʂP�s�ǉ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendCompareLine
	(	CLine*	pobjCompareLine
	)
{
	m_objLineManager.AppendCompareLine(pobjCompareLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r���ʂP�s�}��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::InsertCompareLine
	(	LONG	lLine,
		CLine*	pobjCompareLine
	)
{
	m_objLineManager.InsertCompareLine(lLine,pobjCompareLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w��s�̃t�@�C����r�s��ʎ擾										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorDoc::GetDiffLineKind
	(	long	lLine
	)
{
	return m_objLineManager.GetDiffLineKind(lLine);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc �p�u���b�N�֐��i�f�B���N�g����r�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r���ʕ\�����[�h�ݒ�									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetDirCmpMode
	(	BOOL	bDirCmpMode
	)
{
	m_bDirCmpMode = bDirCmpMode;

	//�ǂݍ��ݐ�p�Ƃ���
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r���ʕ\�����[�h����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsDirCmp()
{
	return m_bDirCmpMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r���ʂP�s�}��											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::InsertDirCmpLine
	(	LONG	lLine,
		CLine*	pobjDirCmpLine
	)
{
	m_objLineManager.InsertDirCmpLine(lLine,pobjDirCmpLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r�p�A�f�B���N�g�����ݒ�								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetDirCmpFolder
	(	CString&	objDirCmpFolder1,
		CString&	objDirCmpFolder2
	)
{
	m_objDirCmpFolder1 = objDirCmpFolder1;
	m_objDirCmpFolder2 = objDirCmpFolder2;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r�p�A�f�B���N�g�����擾								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::GetDirCmpFolder
	(	CString&	objDirCmpFolder1,
		CString&	objDirCmpFolder2
	)
{
	objDirCmpFolder1 = m_objDirCmpFolder1;
	objDirCmpFolder2 = m_objDirCmpFolder2;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r�p�A�����ʎ擾										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorDoc::GetDirCmpLineKind
	(	long	lLineNo
	)
{
	return m_objLineManager.GetDirCmpLineKind(lLineNo);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc �p�u���b�N�֐��i�o�C�i���\���֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�o�C�i���\�����[�h�ݒ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetBinaryMode
	(	BOOL	bBinaryMode
	)
{
	m_bBinaryMode =	bBinaryMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�o�C�i���\�����[�h����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsBinary()
{
	return m_bBinaryMode;
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc �v���e�N�g�֐��i���[�����M�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[�����M�󋵃_�C�A���O�\���҂�										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::WaitForMailSendDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjMailSendStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjMailSendStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjMailSendStatusDlg->m_hWnd);
			if(bRet!=0)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

