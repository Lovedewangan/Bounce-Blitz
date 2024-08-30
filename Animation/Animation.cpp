#include <freeglut.h>
#include <iostream>
#include<math.h>
#include<random>
using namespace std;

random_device rd;
mt19937 gen(rd());

uniform_real_distribution<> colorDis(0.0, 1.0);

float red = 1.0f;
float green = 1.0f;
float blue = 1.0f;

float M_PI = 3.1416;
float factor = 0.0f;

int score = 0;

float rock_x_position = 0.5f + 0.38f;
float rock_y_position = 0.0f;
float rock_size = 0.09f;

float amplitude = 0.03f;   
float frequency = 4.0f;    


float circle_x_position = -0.6f;
float circle_y_position = 0.0f;
float original_circle_y_position = 0.0f;

bool isFalling = false;
bool isElevating = false;
bool isCollisionDetected = false;

float wave_offset = 0.0f;  
float wave_speed = 0.005f; 

float rock_offset = 0.0f;
float rock_speed = 0.01;



float obstacleX_Position = 0.5f + rock_offset + 0.38f;

void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}



void drawRock(float x_pos, float y_pos) {
    glColor3f(0.24f, 0.13f, 0.07f); 
    glBegin(GL_POLYGON);

    glVertex2f(x_pos + rock_offset + 0.4f, y_pos - 0.15f);  // Bottom left
    glVertex2f(x_pos + rock_offset + 0.38f, y_pos - 0.12f); // Mid left
    glVertex2f(x_pos + rock_offset + 0.37f, y_pos - 0.09f); // Upper mid left
    glVertex2f(x_pos + rock_offset + 0.36f, y_pos - 0.05f); // Upper left
    glVertex2f(x_pos + rock_offset + 0.4f, y_pos);          // Top left
    glVertex2f(x_pos + rock_offset + 0.45f, y_pos + 0.02f); // Top mid left
    glVertex2f(x_pos + rock_offset + 0.50f, y_pos + 0.03f); // Top center
    glVertex2f(x_pos + rock_offset + 0.55f, y_pos + 0.02f); // Top mid right
    glVertex2f(x_pos + rock_offset + 0.60f, y_pos);         // Top right
    glVertex2f(x_pos + rock_offset + 0.63f, y_pos - 0.04f); // Upper right
    glVertex2f(x_pos + rock_offset + 0.65f, y_pos - 0.08f); // Mid right
    glVertex2f(x_pos + rock_offset + 0.64f, y_pos - 0.12f); // Bottom right
    glVertex2f(x_pos + rock_offset + 0.60f, y_pos - 0.15f); // Bottom mid right
    glVertex2f(x_pos + rock_offset + 0.54f, y_pos - 0.17f); // Bottom center right
    glVertex2f(x_pos + rock_offset + 0.48f, y_pos - 0.18f); // Bottom center left
    glVertex2f(x_pos + rock_offset + 0.44f, y_pos - 0.17f); // Bottom mid left

    glEnd();
}

void drawPillarRock(float x_pos, float y_pos) {
    glColor3f(0.3f, 0.3f, 0.3f); 
    glBegin(GL_POLYGON);

    glVertex2f(x_pos + rock_offset, y_pos + 1.0f);    
    glVertex2f(x_pos + rock_offset, y_pos + 0.08f);    
    glVertex2f(x_pos + rock_offset + 0.2f, y_pos + 0.08f); 
    glVertex2f(x_pos + rock_offset + 0.2f, y_pos + 1.0f); 

    glEnd();
}
bool checkCollisionPillar() {
    
    float pillarLeft = 2.3f + rock_offset;         
    float pillarRight = pillarLeft + 0.2f;        
    float pillarBottom = 0.08f;                    
                            
    float radius = 0.090f + factor;
    float ballLeft = circle_x_position - radius;     
    float ballRight = circle_x_position + radius ;    
       
    float ballTop = circle_y_position + radius;      

    

    return (ballRight >= pillarLeft && ballLeft <= pillarRight) && (ballTop >= pillarBottom);
}


bool checkCollision() {
    float rockLeft = rock_x_position + rock_offset - rock_size  ;
    float rockRight = rock_x_position + rock_offset + rock_size ;
    float rockTop = rock_y_position + rock_size - 0.01f ;


    float radius = 0.02f + factor;
    float ballLeft = circle_x_position - radius;
    float ballRight = circle_x_position + radius;
    float ballBottom = circle_y_position - radius;

    return !(rockRight < ballLeft || rockLeft > ballRight || rockTop < ballBottom);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    const int numSegments = 100;
    float angleIncrement = 2.0f * M_PI / numSegments;

    /*BALL*/

   

    

    

    /*GROUND*/
    glColor3f(0.3f, 0.8f, 0.15f);


    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.1f);
    glVertex2f(1.0f, -0.1f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();


    /*SOIL*/
    float angleIncrementWave = 2.0f * M_PI;

    /*glColor3f(0.24f, 0.13f, 0.07f);*/
    glColor3f(0.36f, 0.25f, 0.20f);

    glBegin(GL_TRIANGLE_STRIP);
    for (float x = -1.0f; x <= 1.0f; x += 0.01f) {
        float y = amplitude * sin(angleIncrementWave * frequency * (x + wave_offset)) + -0.5f;
        glVertex2f(x, y);
        glVertex2f(x, -1.0f);
    }
    glEnd();


    /*glColor3f(0.36f, 0.25f, 0.20f);*/
    glColor3f(0.24f, 0.13f, 0.07f);

    glBegin(GL_TRIANGLE_STRIP);
    for (float x = -1.0f; x <= 1.0f; x += 0.01f) {
        float y = amplitude * sin(angleIncrementWave * frequency * (x + wave_offset)) + -0.52f;
        glVertex2f(x, y);
        glVertex2f(x, -1.0f);
    }
    glEnd();
    
    /*OBSTACLES*/
    //glColor3f(0.24f, 0.13f, 0.07f);
    //glBegin(GL_POLYGON);
    //glVertex2f(0.4f + rock_offset, -0.15f);  
    //glVertex2f(0.38f + rock_offset, -0.12f); 
    //glVertex2f(0.37f + rock_offset, -0.09f); 
    //glVertex2f(0.36f + rock_offset, -0.05f); 
    //glVertex2f(0.4f + rock_offset, 0.0f);    
    //glVertex2f(0.45f + rock_offset, 0.02f);  
    //glVertex2f(0.50f + rock_offset, 0.03f);  
    //glVertex2f(0.55f + rock_offset, 0.02f);  
    //glVertex2f(0.60f + rock_offset, 0.0f);   
    //glVertex2f(0.63f + rock_offset, -0.04f); 
    //glVertex2f(0.65f + rock_offset, -0.08f); 
    //glVertex2f(0.64f + rock_offset, -0.12f); 
    //glVertex2f(0.60f + rock_offset, -0.15f); 
    //glVertex2f(0.54f + rock_offset, -0.17f); 
    //glVertex2f(0.48f + rock_offset, -0.18f); 
    //glVertex2f(0.44f + rock_offset, -0.17f); 
    //glEnd();

    drawRock(0.5f, 0.0f);

    /*PILLAR_ROCK*/
   /* glColor3f(0.24f, 0.13f, 0.07f);
    glBegin(GL_POLYGON);

    glVertex2f(0.5f + rock_offset, 1.0f);
    glVertex2f(0.5f + rock_offset, 0.08f);
    glVertex2f(0.6f + rock_offset, 0.08f);
    glVertex2f(0.6f + rock_offset, 1.0f);

    glEnd();*/

    drawPillarRock(2.3f, 0.0f);


    char scoreText[20];
    sprintf_s(scoreText, "Score: %d", score);
    renderBitmapString(0.8f, 0.9f, GLUT_BITMAP_HELVETICA_18, scoreText);

    renderBitmapString(0.45f, -0.8f, GLUT_BITMAP_HELVETICA_18, "Love Dewangan, 500109339");
    
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(circle_x_position, circle_y_position);
    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * angleIncrement;
        float x = circle_x_position + (0.1 + factor) * cos(angle);
        float y = circle_y_position + (0.1 + factor) * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (isCollisionDetected) {
        glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
        glBegin(GL_QUADS);
        glVertex2f(1.0f, 1.0f);
        glVertex2f(-1.0f, 1.0f);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f(1.0f, -1.0f);
        glEnd();
    }

    glDisable(GL_BLEND);

    if (isCollisionDetected) {
        glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
        glBegin(GL_QUADS);
        glVertex2f(0.2f, 0.7f);
        glVertex2f(-0.2f, 0.7f);
        glVertex2f(-0.2f, 0.4f);
        glVertex2f(0.2f, 0.4f);
        glEnd();
    }

    if (isCollisionDetected) {
        renderBitmapString(-0.09f, 0.60f, GLUT_BITMAP_HELVETICA_18, "Game Over");

        sprintf_s(scoreText, "Your Score: %d", score);
        renderBitmapString(-0.125f, 0.50f, GLUT_BITMAP_HELVETICA_18, scoreText);
    }

    

    glFlush();
}


void updateScore(int value) {
    if (!isCollisionDetected) {
        score++;
        glutTimerFunc(120, updateScore, 0);
    }
}

void updateElevate(int value) {
    if (!isCollisionDetected) {
        if (isElevating) {
            if (circle_y_position < 0.9f) {
                circle_y_position += 0.015f;
                glutPostRedisplay();
                glutTimerFunc(16, updateElevate, 0);
            }

        }
    }
    
}
void updateFall(int value) {
    float ballBottom = circle_y_position - 0.01f;
    if (!isCollisionDetected) {
        if (isFalling) {
            if (circle_y_position > original_circle_y_position) {
                circle_y_position -= 0.02f;
                if (ballBottom < original_circle_y_position) {
                    circle_y_position = original_circle_y_position;
                    isFalling = false;
                }
            }
            glutPostRedisplay();
            glutTimerFunc(16, updateFall, 0);
        }
    }

}

void updateGround(int value) {

    if (!isCollisionDetected) {
        wave_offset += wave_speed;  
        if (wave_offset < -1.0f / frequency) {  
            wave_offset += 1.0f / frequency;
        }


        rock_offset -= rock_speed;
        if (rock_offset <= -3.5f) {
            rock_offset = 0;
        }

        if (checkCollision() || checkCollisionPillar()) {
            isCollisionDetected = true;
        }
    }
    

    
        
    

    glutPostRedisplay();
    glutTimerFunc(16, updateGround, 0);
}


void mouse(int button, int state, int x, int y) {
    
    if (!isCollisionDetected) {
        if (button == 3) {
            if (factor < 0.04f) {
                factor += 0.01f;
                circle_y_position += 0.01f;

                original_circle_y_position += 0.01f;
            }

        }

        else if (button == 4) {
            if (factor > -0.04f) {
                factor -= 0.01f;
                circle_y_position -= 0.01f;

                original_circle_y_position -= 0.01f;
            }
        }
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (!isCollisionDetected) {
        if (key == 32) {
            if (!isElevating) {
                isElevating = true;
                isFalling = false;
                glutTimerFunc(16, updateElevate, 0);
            }
        }


        if (key == 'r' || key == 'R') {
            factor = 0.0f;
            circle_y_position = 0.0f;
            original_circle_y_position = 0.0f;
        }

        if (key == 'e' || key == 'E') {
            red = colorDis(gen);
            green = colorDis(gen);
            blue = colorDis(gen);
        }
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    if (!isCollisionDetected) {
        if (key == 32) {
            isElevating = false;
            isFalling = true;
            glutTimerFunc(16, updateFall, 0);
        }
    }
}


int main(int argc, char** argv) {
    
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    glutInitWindowSize(1000, 1000);
    
    glutInitWindowPosition(100, 100);
    
    glutCreateWindow("Ball Game");



    
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);

    
    glutDisplayFunc(display);

    glutMouseFunc(mouse);

    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);

    glutTimerFunc(16, updateGround, 0);
    glutTimerFunc(120, updateScore, 0);
    
    

    glutMainLoop();

    return 0;
}
