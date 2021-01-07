#pragma once
class CClient;
class CDataBase
{
public:
	void UpSert_DB(CClient& c);
	void Update_DB(CClient& c);
	void Import_DB(CClient& c);
	void Export_DB();
	void HandleDiagnosticRecord(SQLSMALLINT hType, RETCODE RetCode);
private:
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt = 0;
	SQLRETURN retcode;
	SQLWCHAR szName[NAME_LEN];
	SQLINTEGER ID, USER_LEVEL, NAME, X, Y, HP, EXP;
	SQLLEN cbName = 0, cbID = 0, cbLEVEL = 0;
	SQLLEN x = 0, y = 0, hp = 0, exp = 0;

	SQLSMALLINT iRec = 0;
	SQLINTEGER  iError;
	WCHAR       wszMessage[1000];
	WCHAR       wszState[SQL_SQLSTATE_SIZE + 1];
};

