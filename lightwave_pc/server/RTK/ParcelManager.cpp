#include "StdAfx.h"
#include "ParcelManager.h"
#include "GlobalUtil\FileUtil.h"
#include "CadManager.h"
#include "GlobalUtil\\FileUtil.h"

using namespace GlobalUtil;

namespace ProgramManager
{
	bool __LoadCif( LPCTSTR sCifPath )
	{
		CParcelManager* pManager = CParcelManager::GetInstance();
		CString sLoadFileName = sCifPath;
		pManager->LoadCif( sLoadFileName );

		return false;
	}

	CParcelManager* CParcelManager::m_pThis = NULL;

	CParcelManager* CParcelManager::GetInstance()
	{
		if( m_pThis == NULL )
		{
			m_pThis = new CParcelManager;
		}
		return m_pThis;
	}

	void CParcelManager::ReleaseInstance()
	{
		if( m_pThis != NULL )
		{
			delete m_pThis;
			m_pThis = NULL;
		}
	}



	CParcelManager::CParcelManager(void)
	{

	}


	CParcelManager::~CParcelManager(void)
	{

	}

	void CParcelManager::ClearAllData()
	{
		m_lsParcelMap.RemoveAll();
		m_lsMapBoundMap.RemoveAll();
		m_lsBasePointMap.RemoveAll();
	}

	bool CParcelManager::LoadCif( CString sCifFileName )
	{
		CString sReadPath = sCifFileName;
	
		ClearAllData();
		bool bReadResult = _ReadCifFile( sReadPath );
		if( bReadResult == false )
		{
			ClearAllData();
		}

		if( bReadResult == true )
		{
			CCadManager* pCad = CCadManager::GetInstance();
			pCad->ReCreateParcelData();
		}

		return bReadResult;
	}

	bool CParcelManager::IfEncryptionFile( CString sCifFileName )
	{
		return false;
	}


	bool CParcelManager::_ReadCifFile( CString sFileName )
	{
		// ---------------------------------------------
		// ���� �غ�
		CStdioFile FReadCif;	// �������� �ڵ�
		GlobalUtil::CFileUtil flCifFile;
		
		//.FileOpen( sFileName , "r+" ) == false )
		if( FReadCif.Open( sFileName, CFile::modeRead ) == false )
		{
			return false;
		}
		
		CString sReadBuf;
//		long nFileEndPos = flCifFile.GetEndPos();
//		flCifFile.MovePos(0);
		FReadCif.SeekToBegin();
		long nFileEndPos = FReadCif.GetLength();
		while( true )
		{
//			if( flCifFile.GetPos() >= nFileEndPos ) break;
			if( FReadCif.GetPosition() >= nFileEndPos ) break;


//			long nReadSize = flCifFile.ReadLine( sReadBuf );
//			if( nReadSize <= 0 )
//			{
//				continue;
//			}
			if( FReadCif.ReadString( sReadBuf ) == false ) break;

			sReadBuf = sReadBuf.Trim();
			if( sReadBuf == "EOF" )
			{
				// CIF��
				break;
			}
			if( sReadBuf == "<CIF����S>" )
			{
				/*
				// 
				nReadSize = flCifFile.ReadToEndString2("<CIF����E>",sReadBuf);
				if( nReadSize < 0 )
				{
					// ���� �߻�
					MessageBox(NULL,"CIF���� �ױ׽ֿ� ������ �ֽ��ϴ�.","CIF����",MB_OK );
					flCifFile.FileClose();
					return false;
				}
				continue;
				*/
				while( true )
				{
					if( FReadCif.ReadString( sReadBuf ) == false ) break;
					sReadBuf = sReadBuf.Trim();
					if( sReadBuf == "<CIF����E>" ) break;
				}
				continue;
				
			}
			if( sReadBuf == "<����S>" )
			{
				CParcel prNewParcel;
				if( prNewParcel.SettingParcelFromCifFile( FReadCif ) == true )
				{
					m_lsParcelMap.SetAt( prNewParcel.GetMapKey(), prNewParcel );
				}
				continue;
				/*
				int nNowPos = flCifFile.GetPos();
				// ���̾� 1
				nReadSize = flCifFile.ReadToEndString2("<����E>",sReadBuf);
				if( nReadSize < 0 )
				{
					// ���� �߻�
					MessageBox(NULL,"���� �ױ׽ֿ� ������ �ֽ��ϴ�","CIF����",MB_OK );
					flCifFile.FileClose();
					return false;
				}
				CParcel prNewParcel;
				int nPosAddjustment = -1;
				if( prNewParcel.SettingParcelFromCifString( sReadBuf ) == true )
				{
					m_lsParcelMap.SetAt( prNewParcel.GetUniqID(), prNewParcel );
				}
				continue;
				*/
			}
			if( sReadBuf == "<������S>" )
			{
				// ���̾� 71
				
				CBasePoint bptNewPoint;
				if( bptNewPoint.SettingBasePointFromCifFile( FReadCif ) == true )
				{
					m_lsBasePointMap.SetAt( bptNewPoint.GetPointName(), bptNewPoint );
				}				
				continue;
				/*
				
				nReadSize = flCifFile.ReadToEndString2( "<������E>" , sReadBuf );
				if( nReadSize < 0 )
				{
					// ���� �߻�
					MessageBox(NULL,"������ �ױ׽ֿ� ������ �ֽ��ϴ�","CIF����",MB_OK );
					flCifFile.FileClose();
					return false;
				}
				CBasePoint bptNewPoint;
				if( bptNewPoint.SettingBastPointFromCifString( sReadBuf ) == true )
				{
					m_lsBasePointMap.SetAt( bptNewPoint.GetPointName() , bptNewPoint );
				}
				continue;
				*/
			}
			if( sReadBuf == "<����S>" )
			{
				// ���̾� 60
				CMapBound mbNewMapBound;
				if( mbNewMapBound.SettingMapBoundFromCifFile( FReadCif ) == true )
				{
					m_lsMapBoundMap.SetAt( mbNewMapBound.GetUniqKey() , mbNewMapBound );
				}
				continue;
				/*
				nReadSize = flCifFile.ReadToEndString2( "<����E>" , sReadBuf );
				if( nReadSize < 0 )
				{
					// ���� �߻� 
					MessageBox( NULL, "���� �ױ׽ֿ� ������ �ֽ��ϴ�.", "CIF����",MB_OK );
					flCifFile.FileClose();
					return false;
				}
				CMapBound mbNewMapBound;
				if( mbNewMapBound.SettingMapBoundFromCifString( sReadBuf ) == true )
				{
					m_lsMapBoundMap.SetAt( mbNewMapBound.GetUniqKey() , mbNewMapBound );
				}
				continue;
				*/
			}
		}
		flCifFile.FileClose();
		return true;
	}

	// ###########################################
	// 

	///////////////////////////////////////////////////////
	// sParcelKey�� �ش��ϴ� ������ ������ �����ش�.
	bool CParcelManager::ShowParcelInfoTodlg( CString sParcelKey )
	{
		CParcel * pParcel = &(m_lsParcelMap[sParcelKey]);
		if( pParcel != NULL )
		{
			if( dlgParcelInfo.GetSafeHwnd() == NULL )
			{
				dlgParcelInfo.Create( CParcelInfoDlg::IDD );
			}
			if( dlgParcelInfo.GetSafeHwnd() == NULL )
			{
				MessageBox( NULL, "��������â�� �����Ҽ� �����ϴ�.","ERROR",MB_OK);
				return false;
			}
			dlgParcelInfo.SetViewParcel( pParcel );
			dlgParcelInfo.ShowWindow( SW_SHOW );
			return true;
		}
		return false;
	}

	// 
	// ###########################################


	bool CParcelManager::SetNotAllocationNode( CBasePoint ptPoint )
	{
		m_lsNotAllocationNode.Add( ptPoint );
		CCadManager *pCad = CCadManager::GetInstance();
		pCad->MakeBasePoint( ptPoint , "211" );

		return true;
	}

	bool CParcelManager::SetMakeLineOfTwoAllocationPointConnect()
	{
		if( m_lsNotAllocationNode.GetCount() < 2 ) return false;
		int nLastIndex = m_lsNotAllocationNode.GetCount() - 1;
		CDS_Line lnTemp;
		lnTemp.SetLine( m_lsNotAllocationNode[nLastIndex-1].GetPosition() , m_lsNotAllocationNode[nLastIndex].GetPosition() );
		CCadManager *pCad = CCadManager::GetInstance();
		pCad->MakeLine( lnTemp , "211" );
	}

	int CParcelManager::GetCountOfNotAllocationNode()
	{
		return m_lsNotAllocationNode.GetCount();
	}

	CBasePoint& CParcelManager::GetNotAllocationPointOfIndex( int nIndex )
	{
		return m_lsNotAllocationNode.GetAt(nIndex);
	}

};