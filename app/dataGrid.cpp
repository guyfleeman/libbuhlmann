#include <math.h>	
#include <stdio.h>
#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include "dataGrid.h"

using namespace std;

dataGrid::dataGrid()
{
	gridSize = 20.0;
	dataRowLen = 0;
	dataColLen = 17;
	leftOffset = 7;
	rightOffset = 2;
	bottomOffset = 2;
	topOffset = 1;
}

double * dataGrid::dataToRGB(double data)
{
	static double color[3];
	if (data <= 1)
	{
		color[0] = 0;
		color[1] = data;
		color[2] = 1.0 - data;
	}
	else 
	{
		color[0] = data - 1;
		color[1] = 1.6 - data;
		color[2] = 0;
	}
	return color;
}

double * dataGrid::getColorAt(int row, int col)
{
	double data = grid.at(row).at(col);
	return dataToRGB(data);
}

const char* dataGrid::getTimeAt(int col)
{
	return times.at(col).c_str();
}

double dataGrid::getGridSize()
{
	return gridSize;
}

double * dataGrid::getSquareSize()
{
	return squareSize;
}

double * dataGrid::getScalingVec()
{
	return scalingVec;
}


int dataGrid::getRowLen()
{
	return dataRowLen;
}

int dataGrid::getColLen()
{
	return dataColLen;
}

int dataGrid::getLeftOffset()
{
	return leftOffset;
}

int dataGrid::getRightOffset()
{
	return rightOffset;
}

int dataGrid::getBottomOffset()
{
	return bottomOffset;
}

int dataGrid::getTopOffset()
{
	return topOffset;
}

fstream& operator>>(fstream& os, dataGrid & gridIn)
{
	string num;
    //keep storing values from the text file so long as data exists:
	int cur_col = 0;
	int cur_row = 0;
	vector<double> row_vec;
    while (os >> num)
	{
		if (cur_col == 0)
		{
			gridIn.times.push_back(num);
		}
		else
		{
			row_vec.push_back(stod(num));
		}
		
        cur_col++;
		if (cur_col == gridIn.dataColLen + 1)
		{
			cur_col = 0;
			cur_row ++;
			gridIn.grid.push_back(row_vec);
			row_vec.clear();
		}
    }
	gridIn.dataRowLen = cur_row;

	gridIn.squareSize[0] = gridIn.gridSize / (gridIn.dataRowLen + gridIn.leftOffset * 4.5 + gridIn.rightOffset + 1);
 	gridIn.squareSize[1] = gridIn.gridSize / (gridIn.dataColLen + gridIn.bottomOffset + gridIn.topOffset + 1);

	gridIn.scalingVec[0] = gridIn.gridSize / (gridIn.dataRowLen + gridIn.leftOffset * 4.5 + gridIn.rightOffset);
 	gridIn.scalingVec[1] = gridIn.gridSize / (gridIn.dataColLen + gridIn.bottomOffset + gridIn.topOffset);

    return os;
}