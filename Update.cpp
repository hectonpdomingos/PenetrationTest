// Update.cpp : implementation file
//
//��������û�б��ù��������ﰡ~
#include "stdafx.h"
#include "PentestTool.h"
#include "Update.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Update

IMPLEMENT_DYNCREATE(Update, CWinThread)

Update::Update()
{
}

Update::~Update()
{
}

BOOL Update::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	MessageBox(NULL, "hello", "hello", 0);
	return TRUE;
}

int Update::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(Update, CWinThread)
	//{{AFX_MSG_MAP(Update)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Update message handlers
