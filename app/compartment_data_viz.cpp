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

    glBegin(GL_POLYGON);
    glVertex2f(xOffset, yOffset);
    glVertex2f(xOffset + width, yOffset);
    glVertex2f(xOffset + width, yOffset + height);
    glVertex2f(xOffset, yOffset + height);
    glEnd();
}

void renderBitmapString(float x, float y, void *font,const char *string)
{
    const char *c;
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
} 

void drawRotatedLabel(char*s, int x,int y,int z)
{
	glPushMatrix();
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    
    glScalef(.003, .0047, .003);
    glRotatef(90, 0, 0, 1);
    glTranslatef(1250, -125, 0);
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
    glClear( GL_COLOR_BUFFER_BIT);
 
	glColor3f(1,1,1);
    glLoadIdentity();                           // start with identity matrix
    glOrtho(0.0, 20, 0.0, 20, -1.0, 1.0);   // setup a viewing grid
    glMatrixMode(GL_MODELVIEW);   
    char text[] = "Compartment Number";
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	drawRotatedLabel(text,1,1,0);
    

    glMatrixMode(GL_PROJECTION);              // setup viewing projection
    glLoadIdentity();                           // start with identity matrix
    glOrtho(0.0, gridPtr -> getGridSize(), 0.0, gridPtr -> getGridSize(), -1.0, 1.0);   // setup a viewing grid

    double * scalingVec = gridPtr -> getScalingVec();
    double rowScaling = scalingVec[0];
    double colScaling = scalingVec[1];
    double* color;
    double* squareSize = gridPtr -> getSquareSize();
    double width = squareSize[0];
    double height = squareSize[1];

    //Create data squares
    for (int j = 0; j < gridPtr -> getRowLen(); j++)
    {
        for (int i = 0; i < gridPtr -> getColLen(); i++)
        {   //Set compartment values
            if (j == 0)
            {
                char buffer[20];
                sprintf (buffer, "%d", i);
                glColor3f(1.0, 1.0, 1.0);
                renderBitmapString(width / 2, double(i + gridPtr -> getBottomOffset()) * colScaling, font, buffer);
            }
            color = gridPtr -> getColorAt(j, i);
            glColor3f(color[0], color[1], color[2]);
            createSquare(double(j + gridPtr -> getLeftOffset()) * rowScaling, double(i + gridPtr -> getBottomOffset()) * colScaling, width, height);
        }
        //Set time values
        glColor3f(1.0, 1.0, 1.0);
        renderBitmapString(double(j + gridPtr -> getLeftOffset()) * rowScaling, 1, font, gridPtr -> getTimeAt(j));
    }
    glColor3f(1.0, 1.0, 1.0);
    renderBitmapString(7.7, 0.1, font, "Time (s)");
    renderBitmapString(5.3, 0.5 + colScaling * (gridPtr -> getGridSize() - gridPtr -> getTopOffset()), font, "Compartment Loading Values");

    //Create colormap; 0.1 from 0 to 2
    double i_max = 2.0;
    double i_step = 0.1;
    double cmap_width = width / 2;
    double cmap_height = gridPtr -> getColLen() / (i_max / i_step);
    for (double i = 0; i < i_max + i_step; i += i_step)
    {
        color = gridPtr -> dataToRGB(i);
        glColor3f(color[0], color[1], color[2]);
        createSquare(gridPtr -> getGridSize() - width, gridPtr -> getBottomOffset() + i / i_step * cmap_height,  cmap_width, cmap_height);
        if (((int) round(i / i_step)) % 5 == 0 )
        {
            char buffer[20];
            sprintf (buffer, "%0.1f", i);
            renderBitmapString(gridPtr -> getGridSize() - 2 * width + 0.5, gridPtr -> getBottomOffset() + i / i_step * cmap_height, font, buffer);
        }
    }
    glFlush();
}

int main(int argc, char **argv)
{

    dataGrid grid;
    gridPtr = &grid;

    fstream inFile;
    inFile.open("../data/fake_tc1.txt", fstream::in);
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