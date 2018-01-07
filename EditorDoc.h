// EditorDoc.h : CEditorDoc �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(EDITORDOC_H)		//�Q�d�C���N���[�h�h�~
#define EDITORDOC_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CColorManager;
class CLineManager;
class CUndoManager;
class CUndoItemAddCharacter;
class CUndoItemDelChar;
class CUndoItemReturn;
class CUndoItemBackSpaceChar;
class CUndoItemCutSelectArea;
class CUndoItemEditPaste;
class CUndoItemReplaceSelectWord;
class CUndoItemTabWhenSelected;
class CUndoItemShiftTabWhenSelected;
class CUndoItemDelTopSpaces;
class CUndoItemTab2Space;
class CUndoItemSpace2Tab;
class CUndoItemSpaceZen2Han;
class CUndoItemSpaceHan2Zen;
class CUndoItemKatakanaHan2Zen;
class CUndoItemKatakanaZen2Han;
class CUndoItemAlnumHan2Zen;
class CUndoItemAlnumZen2Han;
class CUndoItemOleDrop;
class CUndoItemInsCppComment;
class CUndoItemDelCppComment;
class CUndoItemInsVbComment;
class CUndoItemDelVbComment;
class CUndoItemInsAsmComment;
class CUndoItemDelAsmComment;
class CUndoItemInsQuote;
class CUndoItemDelQuote;
class CUndoItemInsDay;
class CUndoItemInsTime;
class CUndoItemAl2Upper;
class CUndoItemAl2Lower;
class CMailSendStatusDlg;

class CEditorDoc : public CDocument
{
	friend class CEditorApp;						//�t�����h�N���X�錾
	friend class CMainFrame;
	friend class CUndoItemAddCharacter;
	friend class CUndoItemDelChar;
	friend class CUndoItemReturn;
	friend class CUndoItemBackSpaceChar;
	friend class CUndoItemCutSelectArea;
	friend class CUndoItemEditPaste;
	friend class CUndoItemReplaceSelectWord;
	friend class CUndoItemTabWhenSelected;
	friend class CUndoItemShiftTabWhenSelected;
	friend class CUndoItemDelTopSpaces;
	friend class CUndoItemTab2Space;
	friend class CUndoItemSpace2Tab;
	friend class CUndoItemSpaceZen2Han;
	friend class CUndoItemSpaceHan2Zen;
	friend class CUndoItemKatakanaHan2Zen;
	friend class CUndoItemKatakanaZen2Han;
	friend class CUndoItemAlnumHan2Zen;
	friend class CUndoItemAlnumZen2Han;
	friend class CUndoItemOleDrop;
	friend class CUndoItemInsCppComment;
	friend class CUndoItemDelCppComment;
	friend class CUndoItemInsVbComment;
	friend class CUndoItemDelVbComment;
	friend class CUndoItemInsAsmComment;
	friend class CUndoItemDelAsmComment;
	friend class CUndoItemInsQuote;
	friend class CUndoitemDelQuote;
	friend class CUndoItemInsDay;
	friend class CUndoItemInsTime;
	friend class CUndoItemAl2Upper;
	friend class CUndoItemAl2Lower;

protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CEditorDoc();
	DECLARE_DYNCREATE(CEditorDoc)

//�I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual void OnCloseDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void SetFontOfCurrentFileExt(const char*);						// �t�@�C���g���q�ɑΉ�����t�H���g�ݒ�
	void SetColorOfCurrentFileExt(const char*);						// �t�@�C���g���q�ɑΉ�����F�ݒ�
	void GetBytes(long,long*,long*);								// ���o�C�g���擾
	BOOL IsNeedReload();											// �t�@�C�����e�ēǂݍ��ݕK�v���`�F�b�N
	void ReloadDocument(BOOL);										// �t�@�C�����e�ēǂݍ���
	void MyGetFileName(char*);										// �t�@�C�����擾����
	void MyGetFilePath(char*);										// �t�@�C���p�X�擾����
	CString MyGetFileExt();											// �t�@�C���g���q�擾����
	BOOL IsReadOnly();												// �ǂݍ��ݐ�p����
	BOOL SetReadOnly();												// �ǂݍ��ݐ�p�ݒ�
	BOOL SetReadOnly2();											// �ǂݍ��ݐ�p�ݒ�
	BOOL UnsetReadOnly();											// �ǂݍ��ݐ�p����
	BOOL IsEmpty();													// ��h�L�������g����
	BOOL IsCanUndo();												// �t�m�c�n�\����
	BOOL IsCanRedo();												// �q�d�c�n�\����
	long GetTabSize();												// �^�u�T�C�Y�擾
	void SetTabSize(long);											// �^�u�T�C�Y�ݒ�
	long GetKaigyoKind();											// ���s�R�[�h�敪�擾
	void SetKaigyoKind(long);										// ���s�R�[�h�敪�ݒ�
	long GetLineLength(long);										// �w��s�̕����񒷎擾����
	long GetMaxLineLength();										// �P�s�̒����̍ő�l���擾
	long GetLineCount();											// �s���擾����
	CLine* GetOneLine(long);										// �w��s�̕�����擾����
	void SetOneLine(long,CLine*);									// �w��s�̕�����ݒ菈��
	void DeleteOneLine(long);										// �w��s�폜����
	void InsertOneLine(long,CLine*);								// �s�}������
	BOOL MyIsDBCSLeadByteEx(ULONG,ULONG);							// �Q�o�C�g�����̑��o�C�g�`�F�b�N����
	long GetTabCount(long);											// �P�s�Ɋ܂܂��^�u�̐��擾����
	BOOL IsTabCode(ULONG,ULONG);									// �^�u�R�[�h�`�F�b�N����
	void AddCharacter(long,long,UINT,UINT,BOOL,CPoint&,CPoint&,BOOL,BOOL);	// �����o�^
	void ProcessReturnCode(long,long,CPoint&,CPoint&,BOOL,BOOL);	// ���s�R�[�h����
	void ProcessDeleteCode(long,long,CPoint&,CPoint&,BOOL,BOOL);	// DELETE�R�[�h����
	void ProcessBackSpaceCode(long,long,CPoint&,CPoint&,BOOL,BOOL);	// BACK SPACE�R�[�h����
	void DeleteSelectedArea(long,long,long,long,BOOL,CPoint&,CPoint&,BOOL,BOOL);// �I��͈̓e�L�X�g�폜
	CLine* MyGetSelectedText(long,long,long,long);					// �I��͈̓e�L�X�g�擾
	void ProcessPaste(long,long,long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// ����t������
	void ProcessOleDrop(COleDataObject*,long,long,CPoint&,CPoint&,BOOL,BOOL);// �n�k�d�h���b�v����
	void ProcessReplaceSelectWord(CString&,long,long,long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// �I��͈͒u������
	void ProcessTabWhenSelected(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// �͈͑I�����Ă����Ԃł̂s�`�a�L�[���쏈��
	void ProcessShiftTabWhenSelected(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// �͈͑I�����Ă����Ԃł�Shift+Tab�L�[���쏈��
	void ProcessDelTopSpaces(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// �͈͑I�����Ă���̈�̐擪�󔒍폜����
	void ProcessTab2Space(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL,long);	// �^�u�[�����p�X�y�[�X�ϊ�
	void ProcessSpace2Tab(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL,long);	// ���p�X�y�[�X�|���^�u�ϊ�
	void ProcessSpaceZen2Han(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL,long); // �S�p�X�y�[�X�|�����p�X�y�[�X�ϊ�
	void ProcessSpaceHan2Zen(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL,long); // ���p�X�y�[�X�|���S�p�X�y�[�X�ϊ�
	void ProcessKatakanaHan2Zen(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL,long); // ���p���Ł|���S�p�J�^�J�i�ϊ�
	void ProcessKatakanaZen2Han(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL,long); // �S�p�J�^�J�i�|�����p���ŕϊ�
	void ProcessAlnumHan2Zen(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL,long); // ���p�p�����|���S�p�p�����ϊ�
	void ProcessAlnumZen2Han(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL,long); // �S�p�p�����|�����p�p�����ϊ�
	void ProcessInsCppComment(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// �͈͑I���b�o�o�R�����g�}��
	void ProcessDelCppComment(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// �͈͑I���b�o�o�R�����g�폜
	void ProcessInsVbComment(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// �͈͑I���u�a�R�����g�}��
	void ProcessDelVbComment(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// �͈͑I���u�a�R�����g�폜
	void ProcessInsAsmComment(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// �͈͑I���A�Z���u���R�����g�}��
	void ProcessDelAsmComment(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// �͈͑I���A�Z���u���R�����g�폜
	void ProcessInsQuote(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// �͈͑I�����p�����}��
	void ProcessDelQuote(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// �͈͑I�����p�����폜
	void ProcessInsDay(long,long,CString&,CPoint&,CPoint&,BOOL,BOOL);// ���t������}��
	void ProcessInsTime(long,long,CString&,CPoint&,CPoint&,BOOL,BOOL);// ����������}��
	void ProcessAl2Upper(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// �p���|���啶���ϊ�
	void ProcessAl2Lower(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// �p���|���������ϊ�
	void KeyWordColoring(CLine*);									// �L�[���[�h�J���[�����O����
	void SetFontColor(COLORREF);									// �ʏ�̕����F�ݒ菈��
	BOOL GetFlagComment1Clr();										// �b�E�b�{�{�p�R�����g�L���t���O�擾
	void SetFlagComment1Clr(BOOL);									// �b�E�b�{�{�p�R�����g�L���t���O�ݒ�
	COLORREF GetCommentColor();										// �R�����g�F�擾�����i�b�^�b�{�{�p�j
	void SetCommentColor(COLORREF);									// �R�����g�F�ݒ菈���i�b�^�b�{�{�p�j
	BOOL GetFlagComment2Clr();										// �u�a�p�R�����g�L���t���O�擾
	void SetFlagComment2Clr(BOOL);									// �u�a�p�R�����g�L���t���O�ݒ�
	COLORREF GetCommentColor2();									// �R�����g�F�擾�����i�u�a�p�j
	void SetCommentColor2(COLORREF);								// �R�����g�F�ݒ菈���i�u�a�p�j
	BOOL GetFlagComment3Clr();										// �A�Z���u���p�R�����g�L���t���O�擾
	void SetFlagComment3Clr(BOOL);									// �A�Z���u���p�R�����g�L���t���O�ݒ�
	COLORREF GetCommentColor3();									// �R�����g�F�擾�����i�A�Z���u���p�j
	void SetCommentColor3(COLORREF);								// �R�����g�F�ݒ菈���i�A�Z���u���p�j
	BOOL GetFlagComment4Clr();										// ���p�����p�R�����g�L���t���O�擾
	void SetFlagComment4Clr(BOOL);									// ���p�����p�R�����g�L���t���O�ݒ�
	COLORREF GetCommentColor4();									// �R�����g�F�擾�����i���p�����p�j
	void SetCommentColor4(COLORREF);								// �R�����g�F�ݒ菈���i���p�����p�j
	BOOL GetFlagUrlColor();											// �t�q�k�L���t���O�擾
	void SetFlagUrlColor(BOOL);										// �t�q�k�L���t���O�ݒ�
	COLORREF GetUrlColor();											// �t�q�k�F�擾
	void SetUrlColor(COLORREF);										// �t�q�k�F�ݒ�
	BOOL GetFlagEMailColor();										// ���|���������L���t���O�擾
	void SetFlagEMailColor(BOOL);									// ���|���������L���t���O�ݒ�
	COLORREF GetEMailColor();										// ���|���������F�擾
	void SetEMailColor(COLORREF);									// ���|���������F�ݒ�
	void DeleteAllKeyWord();
	COLORREF GetKeyNo1Color();										// �L�[���[�h�m���P�̕����F�擾����	
	void SetKeyNo1Color(COLORREF,char*);							// �L�[���[�h��`�t�@�C���i�m�n�P�j�ǂݍ��݁��F�ݒ菈��
	COLORREF GetKeyNo2Color();										// �L�[���[�h�m���Q�̕����F�擾����	
	void SetKeyNo2Color(COLORREF,char*);							// �L�[���[�h��`�t�@�C���i�m�n�Q�j�ǂݍ��݁��F�ݒ菈��
	COLORREF GetKeyNo3Color();										// �L�[���[�h�m���R�̕����F�擾����	
	void SetKeyNo3Color(COLORREF,char*);							// �L�[���[�h��`�t�@�C���i�m�n�R�j�ǂݍ��݁��F�ݒ菈��
	COLORREF GetKeyNo4Color();										// �L�[���[�h�m���S�̕����F�擾����	
	void SetKeyNo4Color(COLORREF,char*);							// �L�[���[�h��`�t�@�C���i�m�n�S�j�ǂݍ��݁��F�ݒ菈��
	void SetConvMode(BOOL);											// �����R�[�h�ϊ����ʕ\�����[�h�ݒ菈��
	BOOL IsConv();													// �����R�[�h�ϊ����ʕ\�����[�h����
	void AppendConvLine(CLine&);									// �����R�[�h�ϊ����ʂP�s�ǉ�
	void SetEncriptMode(BOOL);										// �t�@�C���Í������ʕ\�����[�h�ݒ菈��
	BOOL IsEncript();												// �t�@�C���Í������ʕ\�����[�h����
	void AppendEncriptLine(CLine&);									// �t�@�C���Í������ʂP�s�ǉ�
	void SetDecriptMode(BOOL);										// �t�@�C�����������ʕ\�����[�h�ݒ菈��
	BOOL IsDecript();												// �t�@�C�����������ʕ\�����[�h����
	void AppendDecriptLine(CLine&);									// �t�@�C�����������ʂP�s�ǉ�
	void SetGrepMode(BOOL);											// �f�q�d�o���[�h�ݒ菈��
	BOOL IsGrep();													// �f�q�d�o����
	void AppendGrepLine(CLine&);									// �f�q�d�o���ʂP�s�ǉ�
	void AppendHttpLine(CLine&);									// �g�s�s�o�_�E�����[�h���ʂP�s�ǉ�
	void SendJumpMsg(long,BOOL);									// �w��s�W�����v���M
	void SetDumpMode(BOOL);											// �_���v���ʕ\�����[�h�ݒ�
	BOOL IsDump();													// �_���v���ʕ\�����[�h����
	void AppendDumpLine(CLine&);									// �_���v���ʂP�s�ǉ�
	void GetCRLFCount(long*,long*);									// �b�q�A�k�e�̐��擾����
	void SetWCountMode(BOOL);										// ���[�h�J�E���g���ʕ\�����[�h�ݒ�
	BOOL IsWCount();												// ���[�h�J�E���g���ʕ\�����[�h����
	void AppendWCountLine(CLine&);									// ���[�h�J�E���g���ʂP�s�ǉ�
	void SetFSearchMode(BOOL);										// �t�@�C���������ʕ\�����[�h�ݒ�
	BOOL IsFSearch();												// �t�@�C���������ʕ\�����[�h����
	void AppendFSearchLine(CLine&);									// �t�@�C���������ʂP�s�ǉ�
	void SetEncodeMode(BOOL);										// �G���R�[�h���ʕ\�����[�h�ݒ�
	BOOL IsEncode();												// �G���R�[�h���ʕ\�����[�h����
	void AppendEncodeLine(CLine&);									// �G���R�[�h���ʂP�s�ǉ�
	void SetBinaryMode(BOOL);										// �o�C�i���\�����[�h�ݒ�
	BOOL IsBinary();												// �o�C�i���\�����[�h����
	void SetFlagMark(long,BOOL);									// �}�[�N�t���O�ݒ�
	BOOL GetFlagMark(long);											// �}�[�N�t���O�擾
	long GetMarkLineCount();										// �}�[�N�s���擾
	BOOL GetFlagDelEndSpacesOnSave();								// �ۑ����s���󔒍폜�t���O�擾
	void SetFlagDelEndSpacesOnSave(BOOL);							// �ۑ����s���󔒍폜�t���O�ݒ�
	BOOL GetFlagSpace2TabOnSave();									// �ۑ����󔒁|���^�u�ϊ��t���O�擾
	void SetFlagSpace2TabOnSave(BOOL);								// �ۑ����󔒁|���^�u�ϊ��t���O�ݒ�
	void UuDecode(long,long,long,long);								// �t�t�c����������
	void Base64Decode(long,long,long,long);							// �a�������U�S�f�R�[�h
	void QuotedPrintableDecode(long,long,long,long);				// �p�����������|�o�����������������f�R�[�h
	void SetCompareMode(BOOL,char*,char*);							// �t�@�C����r���ʕ\�����[�h�ݒ�
	BOOL IsCompare();												// �t�@�C����r���ʕ\�����[�h����
	void GetCompareFiles(CString&,CString&);						// �t�@�C����r�@��r�t�@�C���擾
	void AppendCompareLine(CLine*);									// �t�@�C����r���ʂP�s�ǉ�
	void InsertCompareLine(LONG,CLine*);							// �t�@�C����r���ʂP�s�}��
	int  GetDiffLineKind(long);										// �w��s�̃t�@�C����r�p�A�s��ʎ擾
	void SetDirCmpMode(BOOL);										// �f�B���N�g����r���ʕ\�����[�h�ݒ�
	BOOL IsDirCmp();												// �f�B���N�g����r���ʕ\�����[�h����
	void InsertDirCmpLine(LONG,CLine*);								// �f�B���N�g����r���ʂP�s�}��
	void SetDirCmpFolder(CString&,CString&);						// �f�B���N�g����r�p�A�f�B���N�g�����ݒ�
	void GetDirCmpFolder(CString&,CString&);						// �f�B���N�g����r�p�A�f�B���N�g�����擾
	int  GetDirCmpLineKind(long);									// �w��s�̃f�B���N�g����r�p�A�����ʎ擾
	BOOL GetFlagPrintPreview();										// ����v���r���[�t���O�擾
	void SetFlagPrintPreview(BOOL);									// ����v���r���[�t���O�ݒ�
	BOOL IsAboveURL(long,long);										// �t�q�k��`�F�b�N����
	BOOL IsAboveEMail(long,long);									// ���|����������`�F�b�N����

	virtual ~CEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void WaitForMailSendDlgDisp();

private:
	CEditorApp*				m_pApp;							// �A�v���P�[�V�����N���X�ւ̃|�C���^
	CString					m_strFileName;					// �h�L�������g�t�@�C����
	CString					m_strFilePath;					// �h�L�������g�t�@�C���p�X
	CString					m_strFileDir;					// �h�L�������g�̂���f�B���N�g���p�X
	CString					m_strFileExt;					// �t�@�C���g���q
	DWORD					m_dwFileAttributes;				// �t�@�C������
	CTime*					m_pobjLastWrittenTime;			// �t�@�C���ŏI�X�V����
	CLineManager			m_objLineManager;				// �e�L�X�g�h�L�������g�Ǘ��N���X
	CUndoManager			m_objUndoManager;				// Undo,Redo�Ǘ��N���X
	CColorManager			m_objColorManager;				// �L�[���[�h�J���[�Ǘ��N���X
	BOOL					m_bGrepMode;					// �f�q�d�o���[�h
	BOOL					m_bConvMode;					// �����R�[�h�ϊ����ʕ\�����[�h
	BOOL					m_bDumpMode;					// �_���v���ʕ\�����[�h
	BOOL					m_bWCountMode;					// ���[�h�J�E���g���ʕ\�����[�h
	BOOL					m_bFSearchMode;					// �t�@�C���������ʕ\�����[�h
	BOOL					m_bBinaryMode;					// �o�C�i���\�����[�h
	BOOL					m_bEncriptMode;					// �t�@�C���Í������ʕ\�����[�h
	BOOL					m_bDecriptMode;					// �t�@�C�����������ʕ\�����[�h
	BOOL					m_bEncodeMode;					// �G���R�[�h���ʕ\�����[�h
	BOOL					m_bDelEndSpacesOnSave;			// �ۑ����̍s���󔒍폜�t���O
	BOOL					m_bSpace2TabOnSave;				// �ۑ����̋󔒁|���^�u�ϊ��t���O
	BOOL					m_bCompareMode;					// �t�@�C����r���ʕ\�����[�h
	CString					m_objFileCmpFile1;				// �t�@�C����r�p���t�@�C���p�X
	CString					m_objFileCmpFile2;				// �t�@�C����r�p���t�@�C���p�X
	BOOL					m_bDirCmpMode;					// �f�B���N�g����r���ʕ\�����[�h
	CString					m_objDirCmpFolder1;				// �f�B���N�g����r�p���f�B���N�g����
	CString					m_objDirCmpFolder2;				// �f�B���N�g����r�p���f�B���N�g����
	BOOL					m_bIsPrintPreview;				// ����v���r���[���t���O
	CMailSendStatusDlg*		m_pobjMailSendStatusDlg;

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CEditorDoc)
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnViewTab4();
	afx_msg void OnUpdateViewTab4(CCmdUI* pCmdUI);
	afx_msg void OnViewTab8();
	afx_msg void OnUpdateViewTab8(CCmdUI* pCmdUI);
	afx_msg void OnEditClearundobuf();
	afx_msg void OnUpdateEditClearundobuf(CCmdUI* pCmdUI);
	afx_msg void OnFileOpensamename();
	afx_msg void OnUpdateFileOpensamename(CCmdUI* pCmdUI);
	afx_msg void OnFileCopy();
	afx_msg void OnUpdateFileCopy(CCmdUI* pCmdUI);
	afx_msg void OnFileRename();
	afx_msg void OnUpdateFileRename(CCmdUI* pCmdUI);
	afx_msg void OnFileMove();
	afx_msg void OnUpdateFileMove(CCmdUI* pCmdUI);
	afx_msg void OnFileShellopen();
	afx_msg void OnUpdateFileShellopen(CCmdUI* pCmdUI);
	afx_msg void OnEditCode2euc();
	afx_msg void OnUpdateEditCode2euc(CCmdUI* pCmdUI);
	afx_msg void OnEditCode2jis();
	afx_msg void OnUpdateEditCode2jis(CCmdUI* pCmdUI);
	afx_msg void OnEditCode2sjis();
	afx_msg void OnUpdateEditCode2sjis(CCmdUI* pCmdUI);
	afx_msg void OnViewTab2();
	afx_msg void OnUpdateViewTab2(CCmdUI* pCmdUI);
	afx_msg void OnFileSendmail();
	afx_msg void OnUpdateFileSendmail(CCmdUI* pCmdUI);
	afx_msg void OnEditCrlf();
	afx_msg void OnUpdateEditCrlf(CCmdUI* pCmdUI);
	afx_msg void OnEditCr();
	afx_msg void OnUpdateEditCr(CCmdUI* pCmdUI);
	afx_msg void OnEditLf();
	afx_msg void OnUpdateEditLf(CCmdUI* pCmdUI);
	afx_msg void OnEditAllundo();
	afx_msg void OnUpdateEditAllundo(CCmdUI* pCmdUI);
	afx_msg void OnEditAllredo();
	afx_msg void OnUpdateEditAllredo(CCmdUI* pCmdUI);
	afx_msg void OnEditSourcearrange();
	afx_msg void OnUpdateEditSourcearrange(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(EDITORDOC_H)

