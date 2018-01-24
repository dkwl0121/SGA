#pragma once

class cPyramid;

class cGrid
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

