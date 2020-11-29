#pragma once
#include <fstream>
#include <vector>
using namespace std;

class dataGrid
{
    public:
        dataGrid();
        friend fstream& operator>>(fstream& os, dataGrid& gridIn);
        double getGridSize();
        double * getSquareSize();
        double * getScalingVec();
        int getRowLen();
        int getColLen();
        int getLeftOffset();
        int getRightOffset();
        int getBottomOffset();
        int getTopOffset();
        double * dataToRGB(double data);
        double * getColorAt(int row, int col);
        const char* getTimeAt(int col);

    private:
        double gridSize;
        int dataRowLen;
        int dataColLen;
        int leftOffset;
        int rightOffset;
        int bottomOffset;
        int topOffset;
        double squareSize[2];
        double scalingVec[2];

        vector<vector<double>> grid;
        vector<string> times;
};

