// EditorView.h : CEditorView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(EDITORVIEW_H)		//�Q�d�C���N���[�h�h�~
#define EDITORVIEW_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//class CEditorApp:
class CEditorDoc;
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
class COperation;
class CMainFrame;
class BCMenu;

class CEditorView : public CView
{
	friend class CEditorDoc;
	friend class CUndoItemAddCharacter;					// �t�����h�N���X�錾
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
	friend class CUndoItemDelQuote;
	friend class CUndoItemInsDay;
	friend class CUndoItemInsTime;
	friend class CUndoItemAl2Upper;
	friend class CUndoItemAl2Lower;
	friend class COperation;
	friend class CMainFrame;

protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CEditorView();										// �R���X�g���N�^
	DECLARE_DYNCREATE(CEditorView)

// �����o�֐�
public:
	CEditorDoc* GetDocument();
	BOOL		IsSelected();							// �͈͑I�𒆃`�F�b�N
	long		GetSelectLineCount();					// �I���s���擾
	void		ClearSelectArea();						// �I��͈͉���
	BOOL		DoFind(CString&,BOOL,BOOL,BOOL,BOOL);	// �������s
	BOOL		DoIncrimentalSearch(long,long,CString&,BOOL);// �C���N�������^���������s
	void		DoReplace(CString&);					// �u�����s
	BOOL		GetLogfontOfDisplay(LOGFONT*);			// �\���t�H���g�̂k�n�f�e�n�m�s�\���̎擾
	BOOL		GetLogfontOfPrint(LOGFONT*);			// �󎚃t�H���g�̂k�n�f�e�n�m�s�\���̎擾
	long		GetDispFontPoint();						// �\���t�H���g�̃|�C���g�擾
	CString		GetDispFontName();						// �\���t�H���g���擾
	void		SetDispFontPoint(long);					// �\���t�H���g�̃|�C���g�ݒ�
	void 		SetDispFontName(CString&);				// �\���t�H���g���ݒ�
	long		GetPrintFontPoint();					// �󎚃t�H���g�̃|�C���g�擾
	CString		GetPrintFontName();						// �󎚃t�H���g���擾
	void		SetPrintFontPoint(long);				// �󎚃t�H���g�̃|�C���g�ݒ�
	void 		SetPrintFontName(CString&);				// �󎚃t�H���g���ݒ�
	COLORREF	GetFontColor();							// �ʏ�̕����F�擾
	void		SetFontColor(COLORREF);					// �ʏ�̕����F�ݒ�
	COLORREF	GetBackColor();							// �ʏ�̔w�i�F�擾
	void		SetBackColor(COLORREF);					// �ʏ�̔w�i�F�ݒ�
	COLORREF	GetInvertFontColor();					// ���]���̕����F�擾
	void		SetInvertFontColor(COLORREF);			// ���]���̕����F�ݒ�
	COLORREF	GetInvertBackColor();					// ���]���̔w�i�F�擾
	void		SetInvertBackColor(COLORREF);			// ���]���̔w�i�F�ݒ�
	BOOL		GetFlagDispLineNo();					// �s�ԍ��\���t���O�擾
	void		SetFlagDispLineNo(BOOL);				// �s�ԍ��\���t���O�ݒ�
	COLORREF	GetLineNoColor();						// �s�ԍ��F�擾
	void		SetLineNoColor(COLORREF);				// �s�ԍ��F�ݒ�
	COLORREF	GetLineNoBackColor();					// �s�ԍ��̔w�i�̐F�擾
	void		SetLineNoBackColor(COLORREF);			// �s�ԍ��̔w�i�̐F�ݒ�
	BOOL		GetFlagDispRuler();						// ���[���\���t���O�擾
	void		SetFlagDispRuler(BOOL);					// ���[���\���t���O�ݒ�
	COLORREF	GetRulerColor();						// ���[���F�擾
	void		SetRulerColor(COLORREF);				// ���[���F�ݒ�
	COLORREF	GetRulerBackColor();					// ���[���̔w�i�̐F�擾
	void		SetRulerBackColor(COLORREF);			// ���[���̔w�i�̐F�ݒ�
	BOOL		GetFlagDispCRLF();						// ���s�}�[�N�\���t���O�擾
	void		SetFlagDispCRLF(BOOL);					// ���s�}�[�N�\���t���O�ݒ�
	COLORREF	GetCRLFColor();							// ���s�}�[�N�F�擾
	void		SetCRLFColor(COLORREF);					// ���s�}�[�N�F�ݒ�
	BOOL		GetFlagDispTab();						// �^�u�R�[�h�\���t���O�擾
	void		SetFlagDispTab(BOOL);					// �^�u�R�[�h�\���t���O�ݒ�
	COLORREF	GetTabColor();							// �^�u�R�[�h�F�擾
	void		SetTabColor(COLORREF);					// �^�u�R�[�h�F�ݒ�
	BOOL		GetFlagDispZenkakuSpace();				// �S�p�X�y�[�X�\���t���O�擾
	void		SetFlagDispZenkakuSpace(BOOL);			// �S�p�X�y�[�X�\���t���O�ݒ�
	COLORREF	GetZenkakuSpaceColor();					// �S�p�X�y�[�X�F�擾
	void		SetZenkakuSpaceColor(COLORREF);			// �S�p�X�y�[�X�F�ݒ�
	BOOL		GetFlagDispUnderLine();					// �A���_�[���C���\���t���O�擾
	void		SetFlagDispUnderLine(BOOL);				// �A���_�[���C���\���t���O�ݒ�
	COLORREF	GetUnderLineColor();					// �A���_�[���C���F�擾
	void		SetUnderLineColor(COLORREF);			// �A���_�[���C���F�ݒ�
	BOOL		GetFlagPrintLineNo();					// �s�ԍ�����t���O�擾
	void		SetFlagPrintLineNo(BOOL);				// �s�ԍ�����t���O�ݒ�
	BOOL		GetFlagPrint2Dangumi();					// �Q�i�g�݈���t���O�擾
	void		SetFlagPrint2Dangumi(BOOL);				// �Q�i�g�݈���t���O�ݒ�
	int			GetPrintHeaderLayoutPos();				// ����w�b�_���C�A�E�g�ʒu�擾
	void		SetPrintHeaderLayoutPos(int);			// ����w�b�_���C�A�E�g�ʒu�ݒ�
	int			GetPrintFooterLayoutPos();				// ����t�b�^���C�A�E�g�ʒu�擾
	void		SetPrintFooterLayoutPos(int);			// ����t�b�^���C�A�E�g�ʒu�ݒ�
	void		GetStrHeaderFormat(CString&);			// ����w�b�_�t�H�[�}�b�g�擾
	void		SetStrHeaderFormat(CString&);			// ����w�b�_�t�H�[�}�b�g�ݒ�
	void		GetStrFooterFormat(CString&);			// ����t�b�^�t�H�[�}�b�g�擾
	void		SetStrFooterFormat(CString&);			// ����t�b�^�t�H�[�}�b�g�ݒ�
	long		GetLineInterval();						// �s�Ԏ擾
	void		SetLineInterval(long);					// �s�Ԑݒ�
	int			GetCaretType();							// �L�����b�g�`��擾
	void		SetCaretType(int);						// �L�����b�g�`��ݒ�
	void		HoldCaretBlinkTime(UINT);				// �L�����b�g�_�ŊԊu�ێ�
	int			GetCaretBehave();						// �L�����b�g�̐U�镑���擾
	void		SetCaretBehave(int);					// �L�����b�g�̐U�镑���ݒ�
	int			GetCaretMoveType();						// �L�����b�g�ړ��^�C�v�擾
	void		SetCaretMoveType(int);					// �L�����b�g�ړ��^�C�v�ݒ�
	void		SetFlagDispEOFMark(BOOL);				// �d�n�e�}�[�N�\���t���O�ݒ�
	CLine* 		MyGetSelectedText(LONG*);				// �I��͈̓e�L�X�g�擾
	int			GetScrollPosLogX();						// �_���X�N���[���ʒu�w�擾
	int			GetScrollPosLogY();						// �_���X�N���[���ʒu�x�擾
	void		AdjustVScrollPos(SCROLLINFO*);			// �����X�N���[���ʒu����
	void		AdjustHScrollPos(SCROLLINFO*);			// �����X�N���[���ʒu����
	COLORREF	GetMarkLineColor();						// �}�[�N�s�F�擾
	void		SetMarkLineColor(COLORREF);				// �}�[�N�s�F�ݒ�
	void		ClearIncrimentalSearchLineOffset();		// �C���N�������^���T�[�`�J�n�s�A�I�t�Z�b�g�N���A
	void		GetIncrimentalSearchString(CString&);	// �C���N�������^���T�[�`������擾
	COLORREF	GetDiffDelLineColor();					// �t�@�C����r�p�A�폜�s�F�擾
	void		SetDiffDelLineColor(COLORREF);			// �t�@�C����r�p�A�폜�s�F�ݒ�
	COLORREF	GetDiffAddLineColor();					// �t�@�C����r�p�A�ǉ��s�F�擾
	void		SetDiffAddLineColor(COLORREF);			// �t�@�C����r�p�A�ǉ��s�F�ݒ�
	COLORREF	GetDirDiffLineColor();					// �f�B���N�g����r�p�A����t�@�C�����F�擾
	void		SetDirDiffLineColor(COLORREF);			// �f�B���N�g����r�p�A����t�@�C�����F�ݒ�

// �����o�ϐ�
private:
	CEditorApp*	m_pApp;									// �A�v���P�[�V�����N���X�ւ̃|�C���^

	// �h���b�O���h���b�v�֘A
	COleDropTarget	m_objOleDropTarget;					// �n�k�d�h���b�v�^�[�Q�b�g

	// �`��֘A
	CRgn*		m_pobjInvalidRegion;					// ������ذ�ޮ�
	CRect*		m_pobjInvalidRect;						// �����ȋ�`
	BOOL		m_bUseClipRgn;							// �د��ذ�ޮݕ`���׸�
	BOOL		m_bNoUseOffBuffer;						// �̽�ذ��ޯ̧�s�g�p�׸�

	// �S�p�X�y�[�X�֘A
	BOOL		m_bDispZenkakuSpace;					// �S�p�X�y�[�X�`��t���O
	COLORREF	m_ZenkakuSpaceColor;					// �S�p�X�y�[�X�̐F

	// �^�u�֘A
	BOOL		m_bDispTab;								// �^�u�R�[�h�`��t���O
	COLORREF	m_TabColor;								// �^�u�R�[�h�̐F

	// ���[���֘A
	BOOL		m_bDispRuler;							// ���[���\���t���O
	COLORREF	m_RulerColor;							// ���[���̐F
	COLORREF	m_RulerBackColor;						// ���[���̔w�i�̐F
	char		m_szRuler[32];							// ���[��
	CPoint		m_RulerPos;								// ���[���\�����̃��[�������ʒu

	// �s�ԍ��֘A
	BOOL		m_bDispLineNo;							// �s�ԍ��\���t���O
	COLORREF	m_LineNoColor;							// �s�ԍ��̐F
	COLORREF	m_LineNoBackColor;						// �s�ԍ��̔w�i�̐F
	char		m_szLineNoFormat[32];					// �s�ԍ��\���t�H�[�}�b�g
	long		m_lLineNoWidth;							// �s�ԍ��\����
	CPoint		m_LineNoPos;							// �s�ԍ��\�����̍s���ʒu

	// ���s�}�[�N�֘A
	BOOL		m_bDispCRLF;							// ���s�}�[�N�\���t���O
	COLORREF	m_CRLFColor;							// ���s�}�[�N�̐F

	// �I�t�X�N���[���o�b�t�@�֘A
	int			m_nOffBufferWidth;						// �̽�ذ��ޯ̧�̕�
    CDC			m_objOffBufferDC;						// �̽�ذ��ޯ̧��DC
    CBitmap		m_objOffBufferBmp;						// �̽�ذ��ޯ̧

	// �L�����b�g�֘A
	int			m_nCaretType;							// �L�����b�g�`��
	int			m_nCaretBehave;							// ��ʃX�N���[�����̃L�����b�g�̐U�镑��
	CPoint		m_caretPos;								// �L�����b�g�\���ʒu
	UINT		m_unCaretBlinkTime;						// �L�����b�g�_�ŊԊu
	int			m_nCaretMoveType;						// �L�����b�g�ړ��^�C�v
	int			m_nLogPreCaretX;						// ���O�̃L�����b�g�_���w�ʒu

	// IME�֘A
	HIMC m_hImc;
	BOOL m_bIsImmOpen;									// �h�l�l�ϊ����t���O

	// ��۰ي֘A
	int m_nMapMode;										// ϯ��ݸ�Ӱ��
	CSize m_totalLog;									// �_���P�ʂł�İ�ٻ���
	CSize m_VPageLog;									// ������۰��ް�̘_���P�ʂł̂P�߰�ނ����轸۰ٻ���
	CSize m_VLineLog;									// ������۰��ް�̘_���P�ʂł̂P�s�����轸۰ٻ���
	CSize m_HPageLog;									// ������۰��ް�̘_���P�ʂł̂P�߰�ނ����轸۰ٻ���
	CSize m_HLineLog;									// ������۰��ް�̘_���P�ʂł̂P�s�����轸۰ٻ���

	// ÷�Ċ֘A
	BOOL		m_bInsertKey;							// Insert�L�[�����t���O(TRUE:�}���AFALSE:�㏑���j
	UINT		m_n1stChar;								// �������@����޲ĕ����̂P�޲Ė�
	UINT		m_n2ndChar;								// �������@����޲ĕ����̂Q�޲Ė�

	// �\���t�H���g�֘A
	CFont		m_objDispFont;							// �\���t�H���g
	CFont		m_objDispFontUnderLine;					// �����t���\���t�H���g
	long		m_lfHeight;								// �t�H���g�̕����̍���
	long		m_lfWidth;								// �t�H���g�̕����̕�
	long		m_lDispFontPoint;						// �\���t�H���g�̃|�C���g
	CString		m_objDispFontName;						// �\���t�H���g��

	// �J���[�֘A
	COLORREF	m_FontColor;							// �ʏ�̕����F
	COLORREF	m_BackColor;							// �ʏ�̔w�i�F
	COLORREF	m_InvertFontColor;						// ���]���̕����F
	COLORREF	m_InvertBackColor;						// ���]���̔w�i�F

	// ����֘A
	BOOL		m_bPrintLineNo;							// �s�ԍ�����t���O
	BOOL		m_bPrint2Dangumi;						// �Q�i�g�݈���t���O
	CLine		m_objPrintFoldLine;						// �܂�Ԃ�����s
	long		m_lPrintFoldLineNo;						// �܂�Ԃ�����s�ԍ�
	int			m_nHeaderLayout;						// �w�b�_���C�A�E�g�ʒu
	int			m_nFooterLayout;						// �t�b�^���C�A�E�g�ʒu
	CString		m_objStrHeaderFormat;					// �w�b�_�t�H�[�}�b�g
	CString		m_objStrFooterFormat;					// �t�b�^�t�H�[�}�b�g
	BOOL		m_bIsNowPrinting;						// ������t���O
	CFont		m_objPrintFont;							// ����p�t�H���g
	long		m_lPrintLineHeight;						// �P�s������̃f�o�C�X�P�ʂ̍���
	long		m_lPrintCharWidth;						// �P����������̃f�o�C�X�P�ʂ̕�
	long		m_lPrintFontPoint;						// �󎚃t�H���g�̃|�C���g
	CString		m_objPrintFontName;						// �󎚃t�H���g��
	long		m_lLinePerPrintPage;					// �P�y�[�W������̍s��

	// �͈͑I���֘A
	BOOL		m_bIsShiftKeyDown;						// �V�t�g�L�[�������t���O
	BOOL		m_bLButtonDown;							// �}�E�X���{�^���������t���O
	long		m_lSelectStartLine;						// �I���J�n�s�ԍ�
	long		m_lSelectStartOffset;					// �I���J�n��ԍ�
	long		m_lSelectEndLine;						// �I���I���s�ԍ�
	long		m_lSelectEndOffset;						// �I���I����ԍ�
	long		m_lPreMoveCaretLine;					// �ړ��O�̍s
	long		m_lPreMoveCaretOffset;					// �ړ��O�̗�
	long		m_lLButtonDownLine;						// �}�E�X���{�^�����������s
	long		m_lLButtonDownOffset;					// �}�E�X���{�^������������

	// �s�Ԋ֘A
	long		m_lLineInterval;						// �s�ԁi�s�N�Z���P�ʁj

	// �A���_�[���C���֘A
	BOOL		m_bDispUnderLine;						// �A���_�[���C���\���t���O
	COLORREF	m_UnderLineColor;						// �A���_�[���C���̐F

	// �}�[�N�֘A
	COLORREF	m_MarkLineColor;						// �}�[�N�s�̐F

	// �C���N�������^���T�[�`�֘A
	long		m_lIncrimentalSearchStartLine;			// �C���N�������^���T�[�`�J�n�s
	long		m_lIncrimentalSearchStartOffset;		// �C���N�������^���T�[�`�J�n�I�t�Z�b�g
	CLine		m_objIncrimentalSearchLine;				// �C���N�������^���T�[�`������

	// �t�@�C����r�ԘA
	COLORREF	m_DiffDelLineColor;						// �폜�s�̐F
	COLORREF	m_DiffAddLineColor;						// �ǉ��s�̐F

	// �f�B���N�g����r�֘A
	COLORREF	m_DirDiffLineColor;						// ����t�@�C�����̐F

	// �r�b�g�}�b�v�t���|�b�v�A�b�v���j���[
	BCMenu		m_popupmenu;

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEditorView)
	public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);						// �|�b�v�A�b�v���j���[�n���h��
	afx_msg LRESULT OnMyImeNotify(WPARAM dwCommand, LPARAM dwData);
	void CgAdjustCompositionWindow();
	void MyGetClientRect(CRect*);											// �ײ��ė̈�擾����
	void MySetScrollSizes(int,SIZE,const SIZE&,const SIZE&,const SIZE&,const SIZE&);
																			// ��۰ٻ��ސݒ菈��
	void MyScrollWindow(BOOL,int);											// ����޳��۰ُ���
	void MyScrollToPosition(CPoint&);										// ��۰وʒu�ݒ菈��
	void MyTextOut(CDC*,long,long,CLine*,long,long);						// �P�s�`�揈��
	int  MyTextOutForPrint(CDC*,long,long,CLine*,long,long,int);			// �P�s�`��i����p�j����
	void MyDrawingCRLF(CDC*,long,long,long,long,int,COLORREF,COLORREF);		// ���s�}�[�N�`�揈��
	void MyDrawingRuler(CDC*,long,long,long,COLORREF,COLORREF);				// ���[���`�揈��
	void MyDrawingZenkakuSpace(CDC*,long,long,long,long,COLORREF,COLORREF);	// �S�p�X�y�[�X�`�揈��
	void MyDrawingTab(CDC*,long,long,long,long,COLORREF,COLORREF);			// �^�u�R�[�h�`�揈��
	void MyDrawingUnderLine(CDC*);											// �A���_�[���C���`�揈��
	void MyEraseUnderLine(CDC*);											// �A���_�[���C����������
	BOOL IsWithinSelectArea(long,long,long,long,long,long);					// �I��͈͓����菈��
	void MoveCaret(UINT,int);												// �L�����b�g�ړ�����
	void MySetCaretPos(CPoint&);											// �L�����b�g�ʒu�ݒ�
	void MySetCaretToLineOffset(long,long);									// �w��s�A��ɃL�����b�g���ʒu�Â��鏈��
	void MySetCaretToMatchBrace(BOOL);										// �Ή����銇�ʂɃL�����b�g���ʒu�Â���
	void GetPosition(long,CPoint*);											// �s���̈ʒu�擾
	void GetPosition1(long,CPoint*,CPoint*);								// �L�����b�g�����ʒu�擾
	void GetPosition2(long,CPoint*,CPoint*);								// �L�����b�g�ȉ��ʒu�擾
	void GetPosition3(long,long,CPoint*);									// �w��I�t�Z�b�g�ȉ��ʒu�擾
	void GetPosition4(long,CPoint*,long);									// ������̍s���ʒu�擾
	void GetPosition5(long,CPoint*,ULONG*,CPoint*);
	long GetOffset(long,CPoint*);											// �I�t�Z�b�g�擾
	long GetPreOffset(long,long);											// ���O�I�t�Z�b�g�擾
	void AdjustSelectArea();												// �I��͈͒�������
	BOOL CaretToLineOffset(long*,long*);									// �L�����b�g�ʒu�̍s�ԍ��A��ԍ��擾
	LONG MouseToLineOffset(CPoint&,long*,long*,CPoint*);					// �}�E�X�ʒu�̍s�ԍ��A��ԍ��擾
	void CreateOffScreenBuf();												// �I�t�X�N���[���o�b�t�@�쐬
	void DestroyOffScreenBuf();												// �I�t�X�N���[���o�b�t�@�j��
	void UpdateOffBuffer();													// �I�t�X�N���[���o�b�t�@�X�V
	void Jump2LineOffset(long,long);										// �w��s�A��ւ̃W�����v
	void Jump2LineOffsetEx(long,long);										// �w��s�A��ւ̃W�����v
	void RedrawSelectArea(long,long);										// �I��͈͂̍ĕ`��
	void InvalidateCaretLine();												// �L�����b�g�s�@�P�s�ĕ`��
	void InvalidateCaretLineEx(long);										// �L�����b�g�s�@�ĕ`��
	void ParsePrintHeaderFormat(CString&,CString&,CPrintInfo*);				// ����w�b�_�t�H�[�}�b�g���
	void ParsePrintFooterFormat(CString&,CString&,CPrintInfo*);				// ����t�b�^�t�H�[�}�b�g���
	UINT CountPrintMaxPage(CPrintInfo*,long,long);							// ������y�[�W���J�E���g
	void MyPrintOut1Page(CDC*,CPrintInfo*,long,long,long);					// �t�@�C�����e�P�y�[�W���o��
	void AutoScroll();														// �I�[�g�X�N���[��
	void WordSelectByCtrlClick(CPoint&);									// CTRL+�N���b�N�ɂ�郏�[�h�Z���N�g


// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEditorView)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectall();
	afx_msg void OnUpdateEditSelectall(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnUpdateEditDel(CCmdUI* pCmdUI);
	afx_msg void OnEditDel();
	afx_msg void OnEditJump2top();
	afx_msg void OnUpdateEditJump2top(CCmdUI* pCmdUI);
	afx_msg void OnEditJump2bottom();
	afx_msg void OnUpdateEditJump2bottom(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditJump2line(CCmdUI* pCmdUI);
	afx_msg void OnEditJump2line();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnUpdateViewLineno(CCmdUI* pCmdUI);
	afx_msg void OnViewLineno();
	afx_msg void OnUpdateViewCrlf(CCmdUI* pCmdUI);
	afx_msg void OnViewCrlf();
	afx_msg void OnUpdateViewRuler(CCmdUI* pCmdUI);
	afx_msg void OnViewRuler();
	afx_msg void OnParenthesis();
	afx_msg void OnViewTab();
	afx_msg void OnUpdateViewTab(CCmdUI* pCmdUI);
	afx_msg void OnViewZenkakuspace();
	afx_msg void OnUpdateViewZenkakuspace(CCmdUI* pCmdUI);
	afx_msg void OnEditTab2space();
	afx_msg void OnUpdateEditTab2space(CCmdUI* pCmdUI);
	afx_msg void OnEditSpace2tab();
	afx_msg void OnUpdateEditSpace2tab(CCmdUI* pCmdUI);
	afx_msg void OnEditSpaceHan2zen();
	afx_msg void OnUpdateEditSpaceHan2zen(CCmdUI* pCmdUI);
	afx_msg void OnEditSpaceZen2han();
	afx_msg void OnUpdateEditSpaceZen2han(CCmdUI* pCmdUI);
	afx_msg void OnEditKatakanaHan2zen();
	afx_msg void OnUpdateEditKatakanaHan2zen(CCmdUI* pCmdUI);
	afx_msg void OnEditKatakanaZen2han();
	afx_msg void OnUpdateEditKatakanaZen2han(CCmdUI* pCmdUI);
	afx_msg void OnViewUnderline();
	afx_msg void OnUpdateViewUnderline(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewCaretIbeam(CCmdUI* pCmdUI);
	afx_msg void OnViewCaretIbeam();
	afx_msg void OnUpdateViewCaretSquare(CCmdUI* pCmdUI);
	afx_msg void OnViewCaretSquare();
	afx_msg void OnEditAlnumHan2zen();
	afx_msg void OnUpdateEditAlnumHan2zen(CCmdUI* pCmdUI);
	afx_msg void OnEditAlnumZen2han();
	afx_msg void OnUpdateEditAlnumZen2han(CCmdUI* pCmdUI);
	afx_msg void OnEditDeltopspaces();
	afx_msg void OnUpdateEditDeltopspaces(CCmdUI* pCmdUI);
	afx_msg void OnEditDeltab();
	afx_msg void OnUpdateEditDeltab(CCmdUI* pCmdUI);
	afx_msg void OnEditInstab();
	afx_msg void OnUpdateEditInstab(CCmdUI* pCmdUI);
	afx_msg void OnUpdateParenthesis(CCmdUI* pCmdUI);
	afx_msg void OnEditTagjump();
	afx_msg void OnUpdateEditTagjump(CCmdUI* pCmdUI);
	afx_msg void OnEditInscppcomment();
	afx_msg void OnUpdateEditInscppcomment(CCmdUI* pCmdUI);
	afx_msg void OnEditInsvbcomment();
	afx_msg void OnUpdateEditInsvbcomment(CCmdUI* pCmdUI);
	afx_msg void OnEditInsasmcomment();
	afx_msg void OnUpdateEditInsasmcomment(CCmdUI* pCmdUI);
	afx_msg void OnViewRedraw();
	afx_msg void OnUpdateViewRedraw(CCmdUI* pCmdUI);
	afx_msg void OnViewEof();
	afx_msg void OnUpdateViewEof(CCmdUI* pCmdUI);
	afx_msg void OnJumpSetmark();
	afx_msg void OnUpdateJumpSetmark(CCmdUI* pCmdUI);
	afx_msg void OnJumpDelmark();
	afx_msg void OnUpdateJumpDelmark(CCmdUI* pCmdUI);
	afx_msg void OnJumpNextmark();
	afx_msg void OnUpdateJumpNextmark(CCmdUI* pCmdUI);
	afx_msg void OnJumpPrevmark();
	afx_msg void OnUpdateJumpPrevmark(CCmdUI* pCmdUI);
	afx_msg void OnJumpMarklist();
	afx_msg void OnUpdateJumpMarklist(CCmdUI* pCmdUI);
	afx_msg void OnJumpAlldelmark();
	afx_msg void OnUpdateJumpAlldelmark(CCmdUI* pCmdUI);
	afx_msg void OnEditInsday();
	afx_msg void OnUpdateEditInsday(CCmdUI* pCmdUI);
	afx_msg void OnEditInstime();
	afx_msg void OnUpdateEditInstime(CCmdUI* pCmdUI);
	afx_msg void OnEditAl2upper();
	afx_msg void OnUpdateEditAl2upper(CCmdUI* pCmdUI);
	afx_msg void OnEditAl2lower();
	afx_msg void OnUpdateEditAl2lower(CCmdUI* pCmdUI);
	afx_msg void OnWordskip2right();
	afx_msg void OnWordskip2left();
	afx_msg void OnWordselect2right();
	afx_msg void OnWordselect2left();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnEditUudecode();
	afx_msg void OnUpdateEditUudecode(CCmdUI* pCmdUI);
	afx_msg void OnEditBase64decode();
	afx_msg void OnUpdateEditBase64decode(CCmdUI* pCmdUI);
	afx_msg void OnEditQuoteddecode();
	afx_msg void OnUpdateEditQuoteddecode(CCmdUI* pCmdUI);
	afx_msg void OnEditNextdiff();
	afx_msg void OnUpdateEditNextdiff(CCmdUI* pCmdUI);
	afx_msg void OnEditPrevdiff();
	afx_msg void OnUpdateEditPrevdiff(CCmdUI* pCmdUI);
	afx_msg void OnEditJump2topselect();
	afx_msg void OnEditJump2bottomselect();
	afx_msg void OnEditDircmpSyosai();
	afx_msg void OnUpdateEditDircmpSyosai(CCmdUI* pCmdUI);
	afx_msg void OnParenthesis2();
	afx_msg void OnParenthesisSelect();
	afx_msg void OnEditBrowseurl();
	afx_msg void OnUpdateEditBrowseurl(CCmdUI* pCmdUI);
	afx_msg void OnEditExecmailer();
	afx_msg void OnUpdateEditExecmailer(CCmdUI* pCmdUI);
	afx_msg void OnEditInsquote();
	afx_msg void OnUpdateEditInsquote(CCmdUI* pCmdUI);
	afx_msg void OnEditDelcppcomment();
	afx_msg void OnUpdateEditDelcppcomment(CCmdUI* pCmdUI);
	afx_msg void OnEditDelvbcomment();
	afx_msg void OnUpdateEditDelvbcomment(CCmdUI* pCmdUI);
	afx_msg void OnEditDelasmcomment();
	afx_msg void OnUpdateEditDelasmcomment(CCmdUI* pCmdUI);
	afx_msg void OnEditDelquote();
	afx_msg void OnUpdateEditDelquote(CCmdUI* pCmdUI);
	afx_msg void OnChangeInsmode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // EditorView.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline CEditorDoc* CEditorView::GetDocument()
   { return (CEditorDoc*)m_pDocument; }
#endif

#endif // !defined(EDITORVIEW_H)

