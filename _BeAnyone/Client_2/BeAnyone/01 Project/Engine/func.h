#pragma once

void TestInit();

void TestUpdate();

void TestRender();

void TestRelease();

int GetSizeofFormat(DXGI_FORMAT _eFormat);

template<class T, int iSize>
void Safe_Delete_arr( T( &arr )[iSize] ) 
{
	for (int i = 0; i < iSize; ++i)
	{
		SAFE_DELETE( arr[i] );
	}
}

template<typename T>
void Safe_Delete_Vector( vector<T*> _vec ) {
	for (UINT i = 0; i < _vec.size(); ++i)
	{
		SAFE_DELETE( _vec[i] );
	}
}

template<typename T1, typename T2>
void Safe_Delete_Map( map<T1, T2>& _map ) {
	for (auto& pair : _map)
	{
		SAFE_DELETE( pair.second );
	}

	_map.clear();
}
void TestRelease();

void SaveWString( FILE* _pFile, const wstring& _str );

wchar_t* LoadWString( FILE* _pFile );

Vec3 DecomposeRotMat( const Matrix& _matRot );