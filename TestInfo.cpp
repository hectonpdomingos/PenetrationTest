// TestInfo.cpp : implementation file
//
#include "stdafx.h"
#include <vector>
#include "PentestTool.h"
#include "TestInfo.h"
#include "Report.h"
#include "Update.h"
#include "DataConnection.h"
//#include <fstream.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//using namespace std;
/////////////////////////////////////////////////////////////////////////////
// TestInfo dialog
TestInfo* TestInfo::s_pTestInfo = NULL;
int TestInfo::i = 1;
bool TestInfo::mFinishStatus = false;
extern Scheme* scheme;
typedef std::vector<std::string> strPath;
extern std::vector<strPath> str2DArrAttackStrings;
extern std::vector<pathStatus> str2DArrHostStatus;
std::vector<std::string> toolParameter;
extern int schemeBeginTime;
extern int schemeEndTime;
//Client *client = new Client("192.168.1.246", 6666);
//std::ofstream oout;
TestInfo::TestInfo(CWnd* pParent /*=NULL*/)
	: CDialog(TestInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(TestInfo)
	//}}AFX_DATA_INIT
	m_strBeginInfo = _T("Begin perform penetration tesing.....");
	m_strAllInfo = m_strBeginInfo;
	mFlag = true; //û��
	mStatus = true; //����·����־λ����Line 159
	mThreadStatus = false; //����������־λ
	mSearchBase = false; //û��
	mTimeWarining = true; //�Ƿ����ʱ�����ѣ������������ȫ���ã���Line 113
	mReport = new Report();
	i = 0;
	//GetDlgItem(IDC_BUTTON_REPORT)->EnableWindow(FALSE);
	s_pTestInfo = this;
}

TestInfo::~TestInfo()
{
	delete mReport;
}

BOOL TestInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_EDIT_PENTESTINFO)->SetWindowTextA(m_strBeginInfo); //���ñ༭��IDC_EDIT_PENTESTINFO������Ϊm_strBeginInfo
	GetDlgItem(IDC_BUTTON_REPORT)->EnableWindow(TRUE); //�ð�ťIDC_BUTTON_REPORT��Ϊ���ã���ɫ��ʾ�����ã�
	SetTimer(1, 3000, NULL); //����1�ż�ʱ����ÿ3�봥��һ��
	return true;
}

void TestInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TestInfo)
	DDX_Control(pDX, IDC_EDIT_PENTESTINFO, m_testInfo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(TestInfo, CDialog)
	//{{AFX_MSG_MAP(TestInfo)
		ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_REPORT, OnButtonShowReport)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, OnButtonProcess)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TestInfo message handlers

//1�Ŷ�ʱ������Ӧ����������nIdEvent���Ƕ�ʱ�����1����Ϊֻ��һ����ʱ��������û��switch case�жϱ��
void TestInfo::OnTimer(UINT nIdEvent)
{
	//if (mFlag == true)
	//{
	//	DisplayInfo();
	//}
	//WriteInfo();
	WriteInfo(); //��log.txt���ȡ�ı����ڡ�����̨�༭������ʾ
	if (mFinishStatus == false)
	{
		GetDlgItem(IDC_BUTTON_REPORT)->EnableWindow(TRUE);
	}
	GetLocalTime(&mCurrentTime); //��ȡ��ǰʱ��
	int hour = static_cast<int> (mCurrentTime.wHour);
	int minute = static_cast<int> (mCurrentTime.wMinute);
	int sum = hour * 60 + minute; //����ɷ���
	if ( (sum < schemeBeginTime) || (sum > schemeEndTime))
	{
		//�����˱�ʾ��������͸����ʱ�䣬������ܼ��ߵĺܣ�����ȥ��
		DWORD dwCode;
		if (mThreadStatus) //������������ģ���ʹmTimeWariningΪfalse������Ҳ����ֹ��͸
		{
			::GetExitCodeThread(mThread, &dwCode);
			::TerminateThread(mThread, dwCode);
			::CloseHandle(mThread->m_hThread);
		}
		if (mTimeWarining) //����������ǵ���������ѣ�û�����κ���ֹ��͸�Ĵ���
		{
			mTimeWarining = false;
			MessageBox("��͸����ʱ�䲻���Ϸ���Ҫ�����ʱ��������", "ʱ������", MB_ICONERROR);
			GetDlgItem(IDC_BUTTON_REPORT)->EnableWindow(TRUE);
		}
		
	}
}

UINT TestInfo::Penetrate(LPVOID lpParam)
{

	int i =0;
	Sleep(10); //�˽��̹���10ms����֪����
	//s_pTestInfoʵ���Ͼ������TestInfo��ʵ������Ϊ���߳���Ĺ������̺߳���Penetrate������ȫ�ֺ���������ֻ��ͨ��ȫ�ֻ�ָ̬�����������еĶ�����
	s_pTestInfo->mFlag = false; //mFlag��ֵ�����û�ù�������Ҳ��
	s_pTestInfo->fout.open("log.txt", ios::app);
	if (!s_pTestInfo->fout.is_open())
	{
		//MessageBox("Open file error", "ERROR", MB_ICONERROR);
	}
	//Client����һ����ͨ��socket��װ��
	Client *client = new Client("192.168.6.2", 6666); //������͸���Է�����������Ӧ�øĳ�192.168.1.177��
	vector<strPath>::iterator iterStrArrAttackStringsForOnePath = str2DArrAttackStrings.begin();
	vector<pathStatus>::iterator iterArrHostStatusForOnePath = str2DArrHostStatus.begin();
	char buffer[10000];
	string allParameter = ""; //û��
	
	int iPathIndex = 0;
	int iCaseIndex = 0;
	strPath strArrAttackStringsForOnePath;
	for (; iterStrArrAttackStringsForOnePath != str2DArrAttackStrings.end(); ++iterStrArrAttackStringsForOnePath, ++iterArrHostStatusForOnePath)
	{
		iCaseIndex = 0;
		pathStatus arrHostStatusForOnePath = *iterArrHostStatusForOnePath;
		strArrAttackStringsForOnePath = *iterStrArrAttackStringsForOnePath;
		string strAttackString; //�����ַ���
		string successStr = ""; //û�ã����涼ע�͵���
		string failStr = ""; //û��
	
		strPath::iterator iterStrAttackString = strArrAttackStringsForOnePath.begin();
		s_pTestInfo->mStatus = true;
		for (; iterStrAttackString != strArrAttackStringsForOnePath.end(); iterStrAttackString++)
		{
			if (s_pTestInfo->mStatus == false) //���socket����fail����mStatus��false����������·������͸
			{
				break;
			}
			strAttackString = *iterStrAttackString; //�����ַ���
			/*if (allParameter.find(parameter) != std::string::npos)
			{
				schemeStatus[pathIndex][caseIndex].status = true;
				successStr += "Case to penetrate";
				successStr += schemeStatus[pathIndex][caseIndex].hotsName;
				successStr += " for ";
				successStr += schemeStatus[pathIndex][caseIndex].cveNumber;
				successStr += "have executed.";
				//successStr += "\r\n";
				instance->fout << successStr.c_str() << endl;
				//WriteInfo(successStr.c_str());
				caseIndex++;
				continue;
			}
			else 
			{
				allParameter += parameter;
			}*/
			allParameter += strAttackString; //û��
			Sleep(50);
			client->Send(strAttackString); //����͸���Է��������͹����ַ��������ǹ�������
			int begin = 0;
			int end = 0;
			begin = strAttackString.find("RHOST"); //Ŀ�����ҡ�RHOST=192.168.1.1 ���������ַ���
			end = strAttackString.find(" ", begin);
			string hostName = strAttackString.substr(begin + 6, end); //begin + 6��ʾ������RHOST=�������hostNameΪ192.168.1.1
			string strTemp = "Penetrate host ";
			strTemp += hostName;
			strTemp += "......Please wait!\r\n";
			//mAllInfo += temp;
			//�ѿ���̨����д���ļ��Ȼ���ʱ����Ӧ�����ٶ�ȡ�ļ�����ʾ�ڿ���̨�༭���DZYд�ĺñ�Ť��
			s_pTestInfo->fout << strTemp.c_str(); //��׼��C++�ļ���д��ʽ��DZY˼·ͦ�ҵģ��ļ���дһ����C++��fstream��һ����MFC��CFile
			s_pTestInfo->fout.close();
			Sleep(6); //���sleep�ù���
			while (true) //ͬ������socket����ֵ
			{
				int len = client->Recive(buffer);
				buffer[len] = '\0';
				if ((len > 0) && (strcmp(buffer, "success") == 0))
				{
					str2DArrHostStatus[iPathIndex][iCaseIndex].status = true; //��ʾhost��͸�ɹ�
					s_pTestInfo->fout.open("log.txt", ios::app);
					s_pTestInfo->fout << "penetrate "<< hostName.c_str() << "success" << endl;
					s_pTestInfo->fout.close();
					iCaseIndex++;
					break;
				}
				else if ((len > 0) && (strcmp(buffer, "fail") == 0))
				{
					//when a case is fail, then will ignore the path's other cases
					str2DArrHostStatus[iPathIndex][iCaseIndex].status = false; //��ʾhost��͸ʧ��
					s_pTestInfo->fout.open("log.txt", ios::app);
					s_pTestInfo->fout << "penetrate "<< hostName.c_str() << "fail" << endl;
					s_pTestInfo->fout.close();
					s_pTestInfo->mStatus = false; //�������forѭ������͸��һ��·��
					//instance->fout << buffer << endl;
					iCaseIndex++;
					break;
				}
				else
				{
					//socket���յ��Ĳ���successҲ����fail��˵����һЩִ��״̬��Ϣ��ֱ��д���ļ���
					s_pTestInfo->fout.open("log.txt", ios::app);
					s_pTestInfo->fout << buffer;
					s_pTestInfo->fout.close();
					continue;
				}
			}
		}
		iPathIndex++;
	}
	mFinishStatus = true; //��ʾ��͸��ɣ���ʱ����Ӧ�����Ͳ��ٸ�����
	//GetDlgItem(IDC_BUTTON_REPORT)->EnableWindow(TRUE);
	return 0;
}

void TestInfo::WriteInfo()
{
	
	mFile.Open("log.txt", CFile::modeRead | CFile::shareDenyNone);
	int nLen = mFile.GetLength();
	char buffer[100000];
	buffer[nLen+1] =0;
	mFile.Read(buffer, nLen);
	mFile.Close();
	GetDlgItem(IDC_EDIT_PENTESTINFO)->SetWindowText(buffer);
	UpdateData(FALSE); //�������ݲ���������GetDlgItem�൱�ڰѿؼ�ֱ�Ӱ�CEdit��������ʱ�ǲ���ҪUpdateData�ģ�DZY���һ����
	m_testInfo.SetScrollPos(SB_VERT,m_testInfo.GetLineCount(),TRUE);
	m_testInfo.SetSel(-1); //�����б�ʾ������������������һ�����
}

void TestInfo::OnButtonShowReport() 
{
	// TODO: Add your control notification handler code here
	mReport->DoModal(); //��ʾ���洰��
}

void TestInfo::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnCancel(); //Ĭ�ϵ��˳�����
}

//���鿴���̡���ť����Ӧ�������������͸�Ŀ�ʼ
void TestInfo::OnButtonProcess()
{
	// TODO: Add your control notification handler code here
	int i = 0;
	if (mSearchBase) //mSearchBase���������ʼ��Ϊfalse�󣬾ʹ�û����1��������SearchBase��Զ�����ܵ���
	{
		SearchBase();
	}
	mThread = AfxBeginThread(Penetrate, NULL, 0, 0, 0, NULL); //MFC��׼�Ķ��̷߳�ʽ֮һ  �������̷߳�ʽ
	mThreadStatus = true; //��ʾ�Ѿ���ʼ��͸
}
void TestInfo::SearchBase()
{
	//��û�е��ù���һ�����ͺ��������ÿ��ˣ�����
	vecPath path = scheme->GetAllPath();
	vecPath::iterator pathIt = path.begin();
	PentestCase pCase;
	PentestPath pPath;
	std::vector<PentestCase> pathCases;
	std::string cveNumber;
	for (; pathIt != path.end(); ++pathIt)
	{	
		pPath = *pathIt;
		pathCases = pPath.pathCase;

		for (std::vector<PentestCase>::iterator caseIt = pathCases.begin(); caseIt != pathCases.end(); ++ caseIt)
		{
			pCase = *caseIt;

			cveNumber = pCase.cvenumber;
			CString strSql;
			strSql.Format("select ToolDescription.toolName, \
							ToolDescription.toolPath, Command.* from \
							Command, ToolDescription where toolCveNumber='%s'", cveNumber.c_str());

			DataConnection dataConnection;
			_RecordsetPtr pRecordset;
			dataConnection.OnInitADOConn();
	
			if(!dataConnection.GetRecordSet(pRecordset , (_bstr_t)strSql))
			{
				return;
			}
			_variant_t  parameter;
			CString str;
			parameter = pRecordset->GetCollect("toolName");
			str=parameter.vt==VT_NULL?"":(char*)(_bstr_t)parameter;;
			toolParameter.push_back(str.GetBuffer(str.GetLength()));
		}
	}
}

