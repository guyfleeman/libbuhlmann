/*
 * Author: Liz Prucka, Yvonne Yeh 
 * Class: ECE 6122
 * Last Date Modified: 12/01/2020
 *
 * Description:
 * Visualize the compartment loading values across the entire time span with
 * openGL. 
 */

#include <stdio.h>
#include <GL/glut.h>
#include <iostream> 
#include <string.h>
#include <cmath>
#include "dataGrid.h"

using namespace std;

dataGrid *gridPtr;

void *font = GLUT_BITMAP_9_BY_15;

void createSquare(double xOffset, double yOffset, double width, double height)
{
    /*
     * draw a square, used for the majority of graph drawing values w/ diff
     * colors
     * param[in]    (xOffset, yOffset)      lower left corner of the square 
     * param[in]    width                   width of square
     * param[in]    height                  height of square
     */
    
    glBegin(GL_POLYGON);
    glVertex2f(xOffset, yOffset);
    glVertex2f(xOffset + width, yOffset);
    glVertex2f(xOffset + width, yOffset + height);
    glVertex2f(xOffset, yOffset + height);
    glEnd();
}

void renderBitmapString(float x, float y, void *font,const char *string)
{
    /* 
     * draw a string. Useful for x & y axis labels.
     * param[in]    (x, y)      position (left) of the text
     * param[in]    font        font type
     * param[in]    string      text to be displayed     
     */
    const char *c;
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
} 

void drawRotatedLabel(char*s, double x, double y)
{
    /*
     * draw a text sideways for y axis label. 
     * param[in]    s       text to draw
     * param[in]    (x,y)   position (bottom) of the text
     */
    
	glPushMatrix();
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    
    glScalef(.003, .0047, .003);
    glRotatef(90, 0, 0, 1);
    glTranslatef(y * 250, -x * 250, 0);
    glLineWidth(0.2);

    for (int i = 0; i < strlen(s); i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN , s[i]);
        glTranslatef(20, 0, 0);
    }
    glPopMatrix();
}

void display(void)
{
    /*
     * draw all elements in the window
     */

    glClear( GL_COLOR_BUFFER_BIT);
 
	glColor3f(1,1,1);
    glLoadIdentity();                           // start with identity matrix
    glOrtho(0.0, 20, 0.0, 20, -1.0, 1.0);       // setup a viewing grid
    glMatrixMode(GL_MODELVIEW);   
    
    // draw y-axis label
    char text[] = "Compartment Number";
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//drawRotatedLabel(text,1,1,0);
	drawRotatedLabel(text, 1.5, 5.5);
    
    // set up viewing grid
    glMatrixMode(GL_PROJECTION);                // setup viewing projection
    glLoadIdentity();                           // start with identity matrix
    glOrtho(0.0, gridPtr -> getGridSize(), 0.0, gridPtr -> getGridSize(), -1.0, 1.0);   // setup a viewing grid

    double * scalingVec = gridPtr -> getScalingVec();
    double rowScaling = scalingVec[0];
    double colScaling = scalingVec[1];
    double* color;
    double* squareSize = gridPtr -> getSquareSize();
    double width = squareSize[0];
    double height = squareSize[1];

    //Create main data squares, starting by column
    int numRows = gridPtr -> getRowLen();
    int xLabelDist = (numRows - 1) / 8;
    for (int j = 0; j < numRows; j++)
    {
        for (int i = 0; i < gridPtr -> getColLen(); i++)
        {  
            // y axis values 
            if (j == 0)
            {
                char buffer[20];
                sprintf (buffer, "%d", i);
                glColor3f(1.0, 1.0, 1.0);
                renderBitmapString(double(gridPtr->getLeftOffset())/2 + 0.5, double(i + gridPtr -> getBottomOffset() + 0.25) * colScaling, font, buffer);
            }
            
            // add all squares 
            color = gridPtr -> getColorAt(j, i);
            glColor3f(color[0], color[1], color[2]);
            createSquare(double(j * rowScaling + gridPtr -> getLeftOffset() + 0.2), double(i * colScaling + gridPtr -> getBottomOffset()), width, height);
        }
        
        // x-axis values
        if (j % xLabelDist == 0) 
        {
            if (j / xLabelDist != numRows / xLabelDist)
            {
                glColor3f(1.0, 1.0, 1.0);
                renderBitmapString(double(j * rowScaling + gridPtr -> getLeftOffset()), 1, font, gridPtr -> getTimeAt(j));
            }
        }
    }

    // draw the last x-axis value
    glColor3f(1.0, 1.0, 1.0);
    renderBitmapString(gridPtr -> getGridSize() - gridPtr -> getRightOffset() - 0.25, 1, font, gridPtr -> getTimeAt(numRows-1));

    // draw x-axis label and chart Title
    glColor3f(1.0, 1.0, 1.0);
    renderBitmapString(8.5, 0.1, font, "Time (s)");
    renderBitmapString(6, gridPtr -> getGridSize() - double(gridPtr -> getTopOffset())/3*2, font, "Compartment Loading Values");

    // Create colormap legend; 0.1 from 0 to 1.6 
    double i_max = 1.6;
    double i_step = 0.1;
    double cmap_width = double(gridPtr -> getRightOffset())/3; // was width / 2;
    double cmap_height = gridPtr -> getColLen() / (i_max / i_step);
    for (double i = 0; i < i_max + i_step; i += i_step)
    {
        color = gridPtr -> dataToRGB(i);
        glColor3f(color[0], color[1], color[2]);
        createSquare(gridPtr -> getGridSize() - cmap_width, gridPtr -> getBottomOffset() + i / i_step * cmap_height,  cmap_width, cmap_height);
        
        // colored numbers
        if (((int) round(i / i_step)) % 5 == 0 )
        {
            // display legend numbers (0, 0.5, 1, 1.5)
            char buffer[20];
            sprintf (buffer, "%0.1f", i);
            renderBitmapString(gridPtr -> getGridSize() - 1.7,  gridPtr -> getBottomOffset() + i / i_step * cmap_height + 0.25, font, buffer);
        }
    }
    glFlush();
}

int main(int argc, char **argv)
{
    /*
     * set up window, load the data to be displayed
     */

    dataGrid grid;
    gridPtr = &grid;

    fstream inFile;
    inFile.open("../data/tc_rel.txt", fstream::in);
    inFile >> *gridPtr;
    inFile.close();
    glutInit(&argc, argv);
    glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(100,100);
    glutInitWindowSize(600,400);
    glutCreateWindow ("Compartment Loading Values");

    glClearColor(0.0, 0.0, 0.0, 0.0);         // black background
    glMatrixMode(GL_PROJECTION);              // setup viewing projection
    glLoadIdentity();                           // start with identity matrix
    glOrtho(0.0, gridPtr -> getGridSize(), 0.0, gridPtr -> getGridSize(), -1.0, 1.0);   // setup a viewing grid

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}