#include "LODmath.h"
using namespace LODmath;
CBit::CBit() 
{
	m_pBits = NULL;
	m_Row = m_Col = 0; 
}
CBit::~CBit()
{
	if (m_pBits)
	{
		delete [] m_pBits;
		m_pBits = NULL;
	}
}
bool CBit::Create(UINT R, UINT C)
{
	m_Row = R, m_Col = C;

	if (m_pBits) delete [] m_pBits;

	UINT size = R * C/32;

	if (R*C%32) size+=1;

	m_pBits = new UINT[size];

	memset(m_pBits, 0, size*4); 

	return m_pBits != NULL;
}

void CBit::Reset()
{
	UINT size = m_Row * m_Col/32;

	if (m_Row*m_Col%32) size+=1;

	memset(m_pBits, 0, size*4); 
}
