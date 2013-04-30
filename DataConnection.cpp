#include "DataConnection.h"

//���ݿ������װ�࣬����ADO����ʵ��


//Constructor
DataConnection::DataConnection()
{
	//do nothing
}

//Destructor
DataConnection::~DataConnection()
{
	//do nothing
}

//Initialize connectiong to database
void  DataConnection::OnInitADOConn()
{
	
	::CoInitialize(NULL);
	HRESULT hr;
	
	try
	{	
		hr = m_pConnection.CreateInstance("ADODB.Connection");
		
		if(m_pConnection->State) m_pConnection->Close(); 

		if( SUCCEEDED(hr) )
		{
			m_pConnection->ConnectionTimeout = 5;
			_bstr_t strConnect = "Provider=SQLOLEDB;Server=127.0.0.1;Database=tool;uid=sa;pwd="; //���ӱ���SQL Server���ݿ�����е�tool���ݿ�
			m_pConnection->Open(strConnect,"","",adModeUnknown);
		}
	}
	
	catch(_com_error e) //����ʧ�ܣ�����SQL Server���ݿ�ûװ
	{
		MessageBox(NULL,e.Description(),"Information",MB_ICONINFORMATION | MB_OK);
	}
}

//Get record of database
bool DataConnection::GetRecordSet(_RecordsetPtr &m_pRecordset , _bstr_t bstrSQL)
//��һ���������������ͣ�Ҳ���Ǵ����������ɿ��������ķ���ֵ���ڶ���������SQL��䣬�����롣
{
	try
	{	
		if(m_pConnection==NULL) //��ʼ������
			OnInitADOConn();
		
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pConnection->CursorLocation = adUseClient;
		
		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		return true;
	}
	
	catch(_com_error e) //ʧ��
	{		
		MessageBox(NULL,e.Description(),"��ʾ",MB_ICONINFORMATION | MB_OK);
		return false;
	}
}


BOOL DataConnection::ExecuteSQL(_bstr_t bstrSQL)
{	
	try
	{	
		if(m_pConnection == NULL) //��ʼ������
		{
			OnInitADOConn();
		}
		
		m_pConnection->Execute(bstrSQL,NULL,adCmdText);
		return true;
	}
	catch(_com_error e) //ʧ��
	{
		MessageBox(NULL,e.Description(),"Information",MB_ICONINFORMATION | MB_OK);
		return false;
	}
}

//Close the connection
//�رռ�¼��������
void DataConnection::ExitConnect()
{

	if (m_pRecordset != NULL)
	{
		m_pRecordset->Close();
	}
	m_pConnection->Close();
	::CoUninitialize();
}

