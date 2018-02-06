#pragma once

#include "cObject.h"

class cPyramid;

class cGrid : public cObject
{
private:
	vector<ST_PC_VERTEX>	m_vecPCVertex;
	vector<cPyramid*>		m_vecPyramid;

public:
	cGrid();
	~cGrid();

	void Setup();
	void Render();
};

