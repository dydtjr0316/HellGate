//#include "stdafx.h"
//#include "CClient.h"
//#include "CDataBase.h"
//
//// 클라이언트 작업 후 추가 및 변경
//
//
//
//
//void CDataBase::HandleDiagnosticRecord(SQLSMALLINT hType, RETCODE RetCode)
//{
//	SQLSMALLINT iRec = 0;
//	SQLINTEGER  iError;
//	WCHAR       wszMessage[1000];
//	WCHAR       wszState[SQL_SQLSTATE_SIZE + 1];
//
//	if (RetCode == SQL_INVALID_HANDLE) {
//		fwprintf(stderr, L"Invalid handle!\n");
//		return;
//	}
//
//	while (SQLGetDiagRec(hType, hstmt, ++iRec, wszState, &iError, wszMessage,
//		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)), (SQLSMALLINT*)NULL) == SQL_SUCCESS) {
//		// Hide data truncated.. 
//		if (wcsncmp(wszState, L"01004", 5)) {
//			fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
//		}
//	}
//	//while (true);
//}
//mutex l;
//
//void CDataBase::UpSert_DB(CClient& c)
//{
//	l.lock();
//	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
//
//	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
//
//		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
//
//			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
//
//				retcode = SQLConnect(hdbc, (SQLWCHAR*)L"2020_fall", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);
//
//				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
//
//					SQLWCHAR query[1024];
//
//					int n = atoi(c.m_name);
//
//					wsprintf(query, L"EXEC store_data %d, %d, %d, %d, %d, %d, %d",
//						c.m_id, n, (int)c.m_ilevel, (int)c.x, (int)c.y, (int)c.m_iHP, c.m_iexp);
//					retcode = SQLExecDirect(hstmt, (SQLWCHAR*)query, SQL_NTS);
//					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//						cout << "DB 저장 성공, ID : " << c.m_id << endl;
//						cout << "NAME : " << n << endl << endl;
//					}
//					else {
//						HandleDiagnosticRecord(SQL_HANDLE_STMT, retcode);
//					}
//					{
//						//retcode = SQLExecDirect(hstmt, (SQLWCHAR*)L"EXEC select_highlevel 50", SQL_NTS);
//						//if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//
//						//	// Bind columns 1, 2, and 3  
//						//	retcode = SQLBindCol(hstmt, 1, SQL_C_LONG, &ID, 100, &cbID);
//						//	retcode = SQLBindCol(hstmt, 2, SQL_C_WCHAR, szName, NAME_LEN, &cbName);
//						//	retcode = SQLBindCol(hstmt, 3, SQL_C_LONG, &LEVEL, 100, &cbLEVEL);
//
//						//	// Fetch and print each row of data. On an error, display a message and exit.  
//						//	/*for (int i = 0; ; i++) {
//						//		retcode = SQLFetch(hstmt);
//						//		if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
//						//			show_error();
//						//		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
//						//		{
//						//			w
//  << i + 1 << " : " << ID << ", " << szName << ", " << LEVEL << endl;
//						//		}
//						//		else
//						//			break;
//						//	}*/
//						//}
//					}
//					// Process data  
//					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//						SQLCancel(hstmt);
//						SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
//					}
//
//					SQLDisconnect(hdbc);
//				}
//
//				SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
//			}
//		}
//		SQLFreeHandle(SQL_HANDLE_ENV, henv);
//	}
//	l.unlock();
//
//}
//
//void CDataBase::Update_DB(CClient& c)
//{
//	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
//
//	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
//
//		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
//
//			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
//
//				retcode = SQLConnect(hdbc, (SQLWCHAR*)L"2020_fall", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);
//
//				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
//
//					SQLWCHAR query[1024];
//					char name[30];
//
//					// 여기
//					string temp = c.m_name;
//					const char* ttemp = temp.c_str();
//					strcpy_s(name, c.m_name);
//					int intname = atoi(c.m_name);
//
//
//
//					cout << name << endl;
//
//					wsprintf(query, L"EXEC UPDATE_DB %d, %d, %d, %d, %d, %d, %d",
//						c.m_id, intname, (int)c.m_ilevel, (int)c.x, (int)c.y, (int)c.m_iHP, c.m_iexp);
//					retcode = SQLExecDirect(hstmt, (SQLWCHAR*)query, SQL_NTS);
//					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//						printf("DB 저장 성공, ID : %d NAME : %d\n", c.m_id, intname);
//						cout << "DB 저장 성공, ID : " << c.m_id << endl;
//						cout << "NAME : " << intname << endl << endl;
//					}
//					else {
//						HandleDiagnosticRecord(SQL_HANDLE_STMT, retcode);
//					}
//					// Process data  
//					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//						SQLCancel(hstmt);
//						SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
//					}
//					SQLDisconnect(hdbc);
//				}
//				SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
//			}
//		}
//		SQLFreeHandle(SQL_HANDLE_ENV, henv);
//	}
//}
//
//void CDataBase::Import_DB(CClient& c)
//{
//	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
//
//	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
//
//		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
//
//			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
//
//				retcode = SQLConnect(hdbc, (SQLWCHAR*)L"2020_fall", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);
//
//				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
//					cout << c.m_id << endl;
//
//					SQLWCHAR query[1024];
//					wsprintf(query, L"EXEC select_highlevel %d", c.m_id);
//
//					retcode = SQLExecDirect(hstmt, (SQLWCHAR*)query, SQL_NTS);
//					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//
//						// Bind columns 1, 2, and 3  
//						retcode = SQLBindCol(hstmt, 1, SQL_C_LONG, &ID, 100, &cbID);
//						retcode = SQLBindCol(hstmt, 2, SQL_C_LONG, &NAME, 100, &cbName);
//						retcode = SQLBindCol(hstmt, 3, SQL_C_LONG, &USER_LEVEL, 100, &cbLEVEL);
//						retcode = SQLBindCol(hstmt, 4, SQL_C_LONG, &X, 100, &x);
//						retcode = SQLBindCol(hstmt, 5, SQL_C_LONG, &Y, 100, &y);
//						retcode = SQLBindCol(hstmt, 6, SQL_C_LONG, &HP, 100, &hp);
//						retcode = SQLBindCol(hstmt, 7, SQL_C_LONG, &EXP, 100, &exp);
//
//						c.m_id = ID;
//						_itoa_s(NAME, c.m_name, 10);
//						c.m_ilevel = ID;
//						c.x = X;
//						c.y = Y;
//						c.m_iHP = HP;
//						c.m_iexp = EXP;
//
//						printf("DB 저장 성공");
//					}
//
//					else {
//						HandleDiagnosticRecord(SQL_HANDLE_STMT, retcode);
//					}
//
//					// Process data  
//					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
//						SQLCancel(hstmt);
//						SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
//					}
//
//					SQLDisconnect(hdbc);
//				}
//
//				SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
//			}
//		}
//		SQLFreeHandle(SQL_HANDLE_ENV, henv);
//	}
//}
//
//void CDataBase::Export_DB()
//{
//
//}
	