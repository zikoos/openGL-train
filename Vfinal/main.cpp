//============================================================================
// Name        : Geom2.cpp
// Author      : Rhezali,AIT OMAR, Niemien
// Version     : finale
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//////////////////////////include des Glut \\\\\\\\\\\\\\\\\\\\\\\\\\\\\

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

//////////////////////////include des librairies \\\\\\\\\\\\\\\\\\\\\\\\\\\\\

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <vector>


#include <cstring>
#include <cstdlib>


//////////////////////////include des .h \\\\\\\\\\\\\\\\\\\\\\\\\\\\\

#include "CDonneesGraphe.h"
#include "CGraphe.h"
#include "CSommet.h"
#include "CPoint3f.h"
#include "CArc.h"
#include "CData2i.h"
#include "CPointAnnexe.h"
#include "CSegment.h"
#include "train.h"
#include "wagon.h"


using namespace std;

//////Déclaration des fonctions\\\\\\

void affichage();
void Idle();
train init_train(int rnd);
wagon init_wagon(train);
void visible(int);
void Onresize(int,int);
GLvoid Initialise(char*);
GLvoid DefineWindow(char*);
void KeyListen(unsigned char, int, int);
void Spe_Keyboard(int, int, int);

//////Déclaration des fonctions pour le décor\\\\\\

static void Arbre( GLUquadricObj * quad, float, float, float);
static void Arbre2(GLUquadricObj * quad,float , float , float );
static void immeuble(GLUquadricObj * quad,float , float , float );
static void Tunnel( GLUquadricObj * quad,float, float, float, float, float, float);
static void maison(GLfloat X, GLfloat Y, GLfloat Z, float, float, float);
static void carosse(GLfloat X, GLfloat Y, GLfloat Z, float, float, float);
static void quais(GLfloat X, GLfloat Y, GLfloat Z, float Rotx, float Roty, float Rotz);
static void quain(GLfloat X, GLfloat Y, GLfloat Z, float Rotx, float Roty, float Rotz);
void soleil(float X, float Y, float Z);



//////Constantes\\\\\\

static int slices = 16;
static int stacks = 16;

float angleBleu=0;
float angleVert=0;
float angleRouge=0;
GLuint herbe;

//////Déclaration des trains\\\\\\

train T1;
train T2;
train T3;

//////Déclaration des wagons\\\\\\

wagon W11;
wagon W12;
wagon W21;
wagon W22;
wagon W31;
wagon W32;


//////paramètres d'ombrage et d'éclairage\\\\\\

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };
const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

//////Paramètres de la caméra\\\\\\
    //  Oeil de la caméra   \\

    static int xcam = -10;
    static int ycam = -10;
    double distcam = 10;


    // Point visé par la caméra

    static double xcentre=14;
    static double ycentre=8;
    static double zcentre=0;

////// construction des tables du graphe \\\\\\

CDonneesGraphe gdata("SXYZ.TXT", "SIF.TXT", "PAXYZ.TXT", "AXYZ.TXT");

////// Constrcuction de la base de données orientée objet du graphe \\\\\\

CGraphe graphe(gdata);
int main(int argc,char* argv[]) {
    srand((unsigned)time(NULL));


    //Tirages aléatoires des sommets de départ des 3 trains: ils ne partent jamais du même sommet
    int rnd=rand()%gdata.NS;
    int rnd2=rnd;
    int rnd3=rnd;
    while(rnd2==rnd){rnd2=rand()%gdata.NS;}
    while(rnd3==rnd|rnd3==rnd2){rnd3=rand()%gdata.NS;}

    //Initialisation train vert
    T1 = init_train(rnd);
    //Initisation des wagons verts
        W11 = init_wagon(T1);
        W12 = init_wagon(T1);
    //Initialisation train bleu
    T2 = init_train(rnd2);
    //Initisation des wagons bleus
        W21 = init_wagon(T2);
        W22 = init_wagon(T2);
    //Initialisation train rouge
    T3 = init_train(rnd3);
        //Initisation des wagons rouge
        W31 = init_wagon(T3);
        W32 = init_wagon(T3);
        glutInit(&argc,argv);
        Initialise(argv[0]);
        glutReshapeFunc(Onresize);//gestion de la taille de la fenêtre
        glutDisplayFunc(affichage); // affichage
        glutIdleFunc(Idle);//déplacement des mobiles
        glutKeyboardFunc(KeyListen);//interaction avec le clavier
        glutSpecialFunc(Spe_Keyboard);
    glutMainLoop();

    return EXIT_SUCCESS;
}


//Le paramètre d'entrée rnd donne l'indice du sommet de départ et garantit que les 3 trains ne démarrent pas du même sommet
train init_train(int rnd){

    //initialisation des trains
        train TGV;

        //Déclaration de sa vitesse initiale
        TGV.Vitesse = 0.01;

        //position initiale du train (aléatoire sur un sommet)
        CSommet Sommet_depart=graphe.list_sommet.at(rnd);

        // tirage au hasard de l'arc à suivre
        int idArcrnd=rand()%graphe.getListArcSortantDuSommet(Sommet_depart).size();
        TGV.ArcSuivi = graphe.getListArcSortantDuSommet(Sommet_depart).at(idArcrnd);
        int Id_somme_depart = TGV.ArcSuivi.id_sommet_ini;
        //Initialisation de ses coordonnées comme les coordonnées du sommet de départ
        TGV.Coord=(CPoint3f)graphe.list_sommet.at(Id_somme_depart);
        //Initialisation de l'indice du point annexe suivant
        TGV.IndPASuiv=0;


    return TGV;
}
//Le paramètre d'entrée "train" permet de choisir à quel train attacher le wagon
wagon init_wagon(train TGV){

    wagon WGE;

    //Calcul du prochain point annexe que va suivre le train
    CPoint3f prochPA=graphe.getListPointAnnexeArc(TGV.ArcSuivi).at(TGV.IndPASuiv);

    double norme=sqrt(pow(prochPA.X-T2.Coord.X,2)+pow(prochPA.Y-T2.Coord.Y,2)+pow(prochPA.Z-T2.Coord.Z,2));

        //calcul de la distance séparant le train de son wagon
            double dX=-(prochPA.X-TGV.Coord.X)/norme;
            double dY=-(prochPA.Y-TGV.Coord.Y)/norme;
            double dZ=-(prochPA.Z-TGV.Coord.Z)/norme;

    //Initialisation des coordonnées du wagon comme celles du train auxquelles on ajoute la distance fixée
    WGE.Coord.X=TGV.Coord.X+dX;
    WGE.Coord.Y=TGV.Coord.Y+dY;
    WGE.Coord.Z=TGV.Coord.Z+dZ;

    return WGE;

}

void affichage(void){
     // herbe
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, herbe);
    glBegin(GL_QUADS);
    glTexCoord2d(-10,-10);  glVertex3d(-10,-10,0); glColor3ub(255,255,255);
    glTexCoord2d(40,-10);  glVertex3d(40,-10,0); glColor3ub(255,255,255);
    glTexCoord2d(40,40);  glVertex3d(40,40,0); glColor3ub(255,255,255);
    glTexCoord2d(-10,40);  glVertex3d(-10,40,0); glColor3ub(255,255,255);
    glEnd();
    glDisable(GL_TEXTURE_2D);
////***************

///////*********************
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
///////////////////////////////////Gestion de la caméra en mode normal\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\_
    gluLookAt(xcam,ycam,distcam ,xcentre,ycentre,zcentre,2,1,2);

        //affichage des sommets
        for (int i=0;i<graphe.list_sommet.size();i++)
        {
                float Xtemp_som=graphe.list_sommet.at(i).X;
                float Ytemp_som=graphe.list_sommet.at(i).Y;
                float Ztemp_som=graphe.list_sommet.at(i).Z;
                glPushMatrix();
                glColor3d(0,0,255);
                    glTranslated(Xtemp_som,Ytemp_som,Ztemp_som);
                    glutWireSphere(0.4,slices,stacks);
        glPopMatrix();
        }

        //affichage des points annexes
   for (int i=0;i<graphe.list_point_annexe.size();i++){
                float Xtemp_pa=graphe.list_point_annexe.at(i).X;
                float Ytemp_pa=graphe.list_point_annexe.at(i).Y;
                float Ztemp_pa=graphe.list_point_annexe.at(i).Z;
                glPushMatrix();
                glColor3d(150,150,190);
                        glTranslated(Xtemp_pa,Ytemp_pa,Ztemp_pa);
                        glutWireSphere(0.2,slices,stacks);
                glPopMatrix();
        }

        //affichage des arcs
        CArc Arctemp;
        glLineWidth(3);
        for (int i=0;i<graphe.list_arc.size();i++)
        {
            glColor3d(3,3,2);
                Arctemp = graphe.list_arc.at(i);
                int u=Arctemp.list_point_annexe.size();
                float Xtemp_arc[u];
                float Ytemp_arc[u];
                float Ztemp_arc[u];
                float Xini=graphe.list_sommet.at(Arctemp.id_sommet_ini).X;
                float Yini=graphe.list_sommet.at(Arctemp.id_sommet_ini).Y;
                float Zini=graphe.list_sommet.at(Arctemp.id_sommet_ini).Z;
                float Xfin=graphe.list_sommet.at(Arctemp.id_sommet_fin).X;
                float Yfin=graphe.list_sommet.at(Arctemp.id_sommet_fin).Y;
                float Zfin=graphe.list_sommet.at(Arctemp.id_sommet_fin).Z;
                for (int r=0;r<u;r++)
                {
                        Xtemp_arc[r]=graphe.list_point_annexe.at(Arctemp.list_point_annexe.at(r)).X;
                        Ytemp_arc[r]=graphe.list_point_annexe.at(Arctemp.list_point_annexe.at(r)).Y;
                        Ztemp_arc[r]=graphe.list_point_annexe.at(Arctemp.list_point_annexe.at(r)).Z;
                }
                glBegin(GL_LINE_STRIP);
                glVertex3f(Xini,Yini,Zini);
                for (int r=0;r<u;r++)
                {
                    glVertex3f(Xtemp_arc[r],Ytemp_arc[r],Ztemp_arc[r]);
                }
                glVertex3f(Xfin,Yfin,Zfin);
                glEnd();
        }

        // affichage du train bleu à la position courrante
        glPushMatrix();
        glTranslated(T2.Coord.X,T2.Coord.Y,T2.Coord.Z);
        glRotatef(angleBleu,0,0,1); //rotation pour suivre le graphe dans le plan
        glColor3d(0,0,255);
        glutSolidSphere(0.5,slices,stacks);
        glPopMatrix();

                //affichage  1er wagon bleu
                glPushMatrix();
                glTranslated(W21.Coord.X,W21.Coord.Y,W21.Coord.Z);
                glColor3d(0,0,255);
                glutSolidCube(0.8);
                glPopMatrix();

                //affichage 2eme wagon bleu
                glPushMatrix();
                glTranslated(W22.Coord.X,W22.Coord.Y,W22.Coord.Z);
                glColor3d(0,0,255);
                glutSolidCube(0.8);
                glPopMatrix();

        //affichage du train vert
        glPushMatrix();
        glTranslated(T1.Coord.X,T1.Coord.Y,T1.Coord.Z);
        glRotatef(angleVert,0,0,1); //rotation pour suivre le graphe dans le plan
        glColor3d(0,255,0);
        glutSolidSphere(0.5,slices,stacks);
        glPopMatrix();

                //affichage  1er wagon vert
                glPushMatrix();
                glTranslated(W11.Coord.X,W11.Coord.Y,W11.Coord.Z);
                glColor3d(0,255,0);
                glutSolidCube(0.8);
                glPopMatrix();

                //affichage 2eme wagon vert
                glPushMatrix();
                glTranslated(W12.Coord.X,W12.Coord.Y,W12.Coord.Z);
                glColor3d(0,255,0);
                glutSolidCube(0.8);
                glPopMatrix();

        //affichage du train rouge
        glPushMatrix();
        glTranslated(T3.Coord.X,T3.Coord.Y,T3.Coord.Z);
        glRotatef(angleRouge,0,0,1); //rotation pour suivre le graphe dans le plan
        glColor3d(255,0,0);
        //glutSolidCube(1);
        glutSolidSphere(0.5,slices,stacks);
        glPopMatrix();

                //affichage  1er wagon rose
                glPushMatrix();
                glTranslated(W31.Coord.X,W31.Coord.Y,W31.Coord.Z);
                glColor3d(255,0,0);
                //glutSolidSphere(0.5,slices,stacks);
                glutSolidCube(0.8);
                glPopMatrix();

                //affichage 2eme wagon rose
                glPushMatrix();
                glTranslated(W32.Coord.X,W32.Coord.Y,W32.Coord.Z);
                glColor3d(255,0,0);
                //glutSolidSphere(0.5,slices,stacks);
                glutSolidCube(0.8);
                glPopMatrix();

///////////////////Dessin du décor\\\\\\\\\\\\\\\\\\\\\\\\\\\

    static GLUquadricObj * quad;
    quad = gluNewQuadric();

    // positions pour placer objets
    CPoint3f Pos1 = gdata.AXYZ.at(gdata.PAXYZ.at(4).colonne_1-1);
    CPoint3f Pos2 = gdata.AXYZ.at(gdata.PAXYZ.at(2).colonne_1-3);
    CPoint3f Pos3 = gdata.AXYZ.at(gdata.PAXYZ.at(6).colonne_1-1);
    CPoint3f Pos4 = gdata.AXYZ.at(gdata.PAXYZ.at(7).colonne_1-2);
    CPoint3f Pos5 = gdata.AXYZ.at(gdata.PAXYZ.at(4).colonne_1-5);

    glColor3d(1.,1,1);


// coordonnées associées aux différentes positions
    float X1 = Pos1.X;
    float Y1 = Pos1.Y;
    float Z1 = Pos1.Z;

    float X2 = Pos2.X;
    float Y2 = Pos2.Y;
    float Z2 = Pos2.Z;

    float X3 = Pos3.X;
    float Y3 = Pos3.Y;
    float Z3 = Pos3.Z;

    float X4 = Pos4.X;
    float Y4 = Pos4.Y;
    float Z4 = Pos4.Z;

    float X5 = Pos5.X;
    float Y5 = Pos5.Y;
    float Z5 = Pos5.Z;

    //tunnel
    Tunnel(quad, X3, Y3, Z3, 90, -45, -400);
    Tunnel(quad, X2+0.3, Y2+0.2, Z2, 90, -55, -15);
    //gare
    quain(X3-8, Y3+1, Z1, 90, 1, 0.5 );
    quais(X3-8, Y3-1, Z1, 90, 1, 0.5 );
    //arbres
    Arbre(quad, X2+1,Y2+11,Z2);
    Arbre(quad, X3-9,Y3-4,Z3);
    Arbre(quad, X3-4,Y3-5,Z3);
    Arbre(quad, X4+6,Y4+8,Z4);
    Arbre(quad, X4-2,Y4-1,Z4);
    Arbre2(quad, X5+3,Y5+1,Z5);
    Arbre2(quad, X1+5,Y1+4,Z1);
    Arbre2(quad, X1-2,Y1-1,Z1);
    Arbre2(quad, X1-1,Y2+2,Z1);
    //sol
    float S1 [] = {-3,-3,Z1};
    float S2 [] = {28,-3,Z1};
    float S3 [] = {28,28,Z1};
    float S4 [] = {-3,28,Z1};
    glPushMatrix();
        glColor3d(0.6,1,0.);
        glBegin(GL_QUADS);
            glVertex3fv(S1);
            glVertex3fv(S2);
            glVertex3fv(S3);
            glVertex3fv(S4);
        glEnd();
    glPopMatrix();
    //soleil
    soleil(0, 20, 8);
    //maison
    maison(X1+7, Y1+8, Z1, 90, 1, 0.5 );

    // carosse
    carosse(X1-1, Y1+7, Z1, 90, 1, 0.5 );
    carosse(X5+4, Y5-3, Z5, 90, 1, 0.5 );

    glFlush();//envoyer a l'écran
}



//La taille de la fenêtre 3D est en entrée de la fonction
void Onresize(int width, int height){

    const float ar = (float) width / (float) height;

    //définir la vue 3D
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

GLvoid Initialise(char * Titre){

    GLvoid DefineWindow( char * ) ;
    glutInitDisplayMode(GLUT_RGB);
    DefineWindow(Titre);
    //definir espace visible
    glPointSize(10);
    glLineWidth(2.5);
    glOrtho(0.,200.,0.,200.,0.,1.);//xmin xmax ymin ymax zmin zmax
}

GLvoid DefineWindow( char * Titre){
        int width, height;
        width = glutGet(GLUT_SCREEN_WIDTH);
        height = glutGet(GLUT_SCREEN_HEIGHT);
        //Création de la fenetre
        glutInitWindowPosition(width/10,height/10);
        glutInitWindowSize(width*0.75,height*0.75);

        glutCreateWindow("Train2014");

        glClearColor(0.9,0.7,0.4,0.6);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        //Fonction d'éclairage et luminosité
        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHTING);
        glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
        glutSwapBuffers();
}

//Fonction de déplacement des mobiles
void Idle(){
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    ///////////////////////Déplacement train bleu\\\\\\\\\\\\\\\\\\\\\\\\\\\

    double dX;
    double dY;
    double dZ;

    double dXW;
    double dYW;
    double dZW;

    //Test pour vérifier que le point suivant est un point annexe
    if (T2.IndPASuiv<graphe.getListPointAnnexeArc(T2.ArcSuivi).size()){
        // position a atteindre
        CPoint3f prochPA=graphe.getListPointAnnexeArc(T2.ArcSuivi).at(T2.IndPASuiv);
        //Déplacement
 double norm=sqrt(pow(prochPA.X-T2.Coord.X,2)+pow(prochPA.Y-T2.Coord.Y,2)+pow(prochPA.Z-T2.Coord.Z,2));
        dX=(prochPA.X-T2.Coord.X)/norm*T2.Vitesse;
        dY=(prochPA.Y-T2.Coord.Y)/norm*T2.Vitesse;
        dZ=(prochPA.Z-T2.Coord.Z)/norm*T2.Vitesse;
        angleBleu = atan(dY/dX)*360/2/3.14;

        //deplacement wagon
        dXW=(prochPA.X-T2.Coord.X)/norm;
        dYW=(prochPA.Y-T2.Coord.Y)/norm;
        dZW=(prochPA.Z-T2.Coord.Z)/norm;

        W21.Coord.X=T2.Coord.X-dXW;
        W21.Coord.Y=T2.Coord.Y-dYW;
        W21.Coord.Z=T2.Coord.Z-dZW;

        W22.Coord.X=T2.Coord.X-2*dXW;
        W22.Coord.Y=T2.Coord.Y-2*dYW;
        W22.Coord.Z=T2.Coord.Z-2*dZW;



        //déplacement train
        T2.Coord.X=T2.Coord.X+dX;
        T2.Coord.Y=T2.Coord.Y+dY;
        T2.Coord.Z=T2.Coord.Z+dZ;

        if (fabs(T2.Coord.X-prochPA.X)<0.1){
            T2.IndPASuiv+=1;
        }

    } else {    // On arrive à un sommet
    // position a atteindre
    CSommet prochSommet=graphe.list_sommet.at(T2.ArcSuivi.id_sommet_fin);

    if(pow(pow(prochSommet.X-T2.Coord.X,2)+pow(prochSommet.Y-T2.Coord.Y,2),0.5)>0.1){
        //Si le train n'est pas au sommet
        //Déplacement
 double norm=sqrt(pow(prochSommet.X-T2.Coord.X,2)+pow(prochSommet.Y-T2.Coord.Y,2)+pow(prochSommet.Z-T2.Coord.Z,2));
        dX=(prochSommet.X-T2.Coord.X)/norm*T2.Vitesse;
        dY=(prochSommet.Y-T2.Coord.Y)/norm*T2.Vitesse;
        dZ=(prochSommet.Z-T2.Coord.Z)/norm*T2.Vitesse;
        angleBleu = atan(dY/dX)*360/2/3.14;

        //deplacement wagon
        dXW=(prochSommet.X-T2.Coord.X)/norm;
        dYW=(prochSommet.Y-T2.Coord.Y)/norm;
        dZW=(prochSommet.Z-T2.Coord.Z)/norm;

        W21.Coord.X=T2.Coord.X-dXW;
        W21.Coord.Y=T2.Coord.Y-dYW;
        W21.Coord.Z=T2.Coord.Z-dZW;

        W22.Coord.X=T2.Coord.X-2*dXW;
        W22.Coord.Y=T2.Coord.Y-2*dYW;
        W22.Coord.Z=T2.Coord.Z-2*dZW;

        //déplacement train
        T2.Coord.X=T2.Coord.X+dX;
        T2.Coord.Y=T2.Coord.Y+dY;
        T2.Coord.Z=T2.Coord.Z+dZ;

    // Mise a jour des indicateurs du train quand celui ci est sur le sommet
    }else{
        // tirage au hasard de l'arc à suivre
        int idArcrnd = rand()%graphe.getListArcSortantDuSommet(prochSommet).size();
        CArc prochainArc = graphe.getListArcSortantDuSommet(prochSommet).at(idArcrnd);
        // Test s'il n'y a personne sur l'arc
        if(prochainArc.id_arc!=T1.ArcSuivi.id_arc&prochainArc.id_arc!=T3.ArcSuivi.id_arc){
            T2.ArcSuivi = prochainArc;
            int Id_somme_depart = T2.ArcSuivi.id_sommet_ini;
            T2.Coord = (CPoint3f)graphe.list_sommet.at(Id_somme_depart);
            T2.IndPASuiv = 0;
        }
    }

    }

//------------------------Déplacement train vert------------------------------//

    //test pour savoir si le point suivant est bien un point annexe
    if (T1.IndPASuiv<graphe.getListPointAnnexeArc(T1.ArcSuivi).size()){
        // position a atteindre
        CPoint3f prochPA_V=graphe.getListPointAnnexeArc(T1.ArcSuivi).at(T1.IndPASuiv);
        //Déplacement
 double norm=sqrt(pow(prochPA_V.X-T1.Coord.X,2)+pow(prochPA_V.Y-T1.Coord.Y,2)+pow(prochPA_V.Z-T1.Coord.Z,2));
        dX=(prochPA_V.X-T1.Coord.X)/norm*T1.Vitesse;
        dY=(prochPA_V.Y-T1.Coord.Y)/norm*T1.Vitesse;
        dZ=(prochPA_V.Z-T1.Coord.Z)/norm*T1.Vitesse;
        angleVert = atan(dY/dX)*360/2/3.14;

        //deplacement wagon
        dXW=(prochPA_V.X-T1.Coord.X)/norm;
        dYW=(prochPA_V.Y-T1.Coord.Y)/norm;
        dZW=(prochPA_V.Z-T1.Coord.Z)/norm;

        W11.Coord.X=T1.Coord.X-dXW;
        W11.Coord.Y=T1.Coord.Y-dYW;
        W11.Coord.Z=T1.Coord.Z-dZW;

        W12.Coord.X=T1.Coord.X-2*dXW;
        W12.Coord.Y=T1.Coord.Y-2*dYW;
        W12.Coord.Z=T1.Coord.Z-2*dZW;


        //déplacement train
        T1.Coord.X=T1.Coord.X+dX;
        T1.Coord.Y=T1.Coord.Y+dY;
        T1.Coord.Z=T1.Coord.Z+dZ;

        if (fabs(T1.Coord.X-prochPA_V.X)<0.1){
            T1.IndPASuiv+=1;
        }
    } else {    // On arrive à un sommet
        // position a atteindre
        CSommet prochSommet_V=graphe.list_sommet.at(T1.ArcSuivi.id_sommet_fin);

        if(pow(pow(prochSommet_V.X-T1.Coord.X,2)+pow(prochSommet_V.Y-T1.Coord.Y,2),0.5)>0.1){
            //Déplacement
     double norm=sqrt(pow(prochSommet_V.X-T1.Coord.X,2)+pow(prochSommet_V.Y-T1.Coord.Y,2)+pow(prochSommet_V.Z-T1.Coord.Z,2));
            dX=(prochSommet_V.X-T1.Coord.X)/norm*T1.Vitesse;
            dY=(prochSommet_V.Y-T1.Coord.Y)/norm*T1.Vitesse;
            dZ=(prochSommet_V.Z-T1.Coord.Z)/norm*T1.Vitesse;
            angleVert = atan(dY/dX)*360/2/3.14;

            //déplacement wagons
            dXW=(prochSommet_V.X-T1.Coord.X)/norm;
            dYW=(prochSommet_V.Y-T1.Coord.Y)/norm;
            dZW=(prochSommet_V.Z-T1.Coord.Z)/norm;

            W11.Coord.X=T1.Coord.X-dXW;
            W11.Coord.Y=T1.Coord.Y-dYW;
            W11.Coord.Z=T1.Coord.Z-dZW;

            W12.Coord.X=T1.Coord.X-2*dXW;
            W12.Coord.Y=T1.Coord.Y-2*dYW;
            W12.Coord.Z=T1.Coord.Z-2*dZW;

            //déplacement train
            T1.Coord.X=T1.Coord.X+dX;
            T1.Coord.Y=T1.Coord.Y+dY;
            T1.Coord.Z=T1.Coord.Z+dZ;

        // Mise a jour des indicateurs du train quand celui ci est sur le sommet
        }else{

            // tirage au hasard de l'arc à suivre
            int idArcrnd_V = rand()%graphe.getListArcSortantDuSommet(prochSommet_V).size();
            CArc prochainArc_V = graphe.getListArcSortantDuSommet(prochSommet_V).at(idArcrnd_V);

             // Test s'il n'y a personne sur l'arc
            if(prochainArc_V.id_arc!=T2.ArcSuivi.id_arc&prochainArc_V.id_arc!=T3.ArcSuivi.id_arc){
            T1.ArcSuivi = prochainArc_V;
            int Id_somme_depart = T1.ArcSuivi.id_sommet_ini;
            T1.Coord = (CPoint3f)graphe.list_sommet.at(Id_somme_depart);
            T1.IndPASuiv = 0;
            }
        }
    }

    ///////////////////////////////Déplacement train rouge\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

    //Test pour savoir si le point suivant est bien un point annexe
    if (T3.IndPASuiv<graphe.getListPointAnnexeArc(T3.ArcSuivi).size()){
        // position a atteindre
        CPoint3f prochPA_R=graphe.getListPointAnnexeArc(T3.ArcSuivi).at(T3.IndPASuiv);
        //Déplacement
 double norm=sqrt(pow(prochPA_R.X-T3.Coord.X,2)+pow(prochPA_R.Y-T3.Coord.Y,2)+pow(prochPA_R.Z-T3.Coord.Z,2));
        dX=(prochPA_R.X-T3.Coord.X)/norm*T3.Vitesse;
        dY=(prochPA_R.Y-T3.Coord.Y)/norm*T3.Vitesse;
        dZ=(prochPA_R.Z-T3.Coord.Z)/norm*T3.Vitesse;
        angleRouge = atan(dY/dX)*360/2/3.14;

        //deplacement wagon
        dXW=(prochPA_R.X-T3.Coord.X)/norm;
        dYW=(prochPA_R.Y-T3.Coord.Y)/norm;
        dZW=(prochPA_R.Z-T3.Coord.Z)/norm;

        W31.Coord.X=T3.Coord.X-dXW;
        W31.Coord.Y=T3.Coord.Y-dYW;
        W31.Coord.Z=T3.Coord.Z-dZW;

        W32.Coord.X=T3.Coord.X-2*dXW;
        W32.Coord.Y=T3.Coord.Y-2*dYW;
        W32.Coord.Z=T3.Coord.Z-2*dZW;



        //déplacement train
        T3.Coord.X=T3.Coord.X+dX;
        T3.Coord.Y=T3.Coord.Y+dY;
        T3.Coord.Z=T3.Coord.Z+dZ;

        if (fabs(T3.Coord.X-prochPA_R.X)<0.1){
            T3.IndPASuiv+=1;
        }
    } else {    // On arrive à un sommet
        // position a atteindre
        CSommet prochSommet_R=graphe.list_sommet.at(T3.ArcSuivi.id_sommet_fin);

        if(pow(pow(prochSommet_R.X-T3.Coord.X,2)+pow(prochSommet_R.Y-T3.Coord.Y,2),0.5)>0.1){
            //Déplacement
     double norm=sqrt(pow(prochSommet_R.X-T3.Coord.X,2)+pow(prochSommet_R.Y-T3.Coord.Y,2)+pow(prochSommet_R.Z-T3.Coord.Z,2));
            dX=(prochSommet_R.X-T3.Coord.X)/norm*T3.Vitesse;
            dY=(prochSommet_R.Y-T3.Coord.Y)/norm*T3.Vitesse;
            dZ=(prochSommet_R.Z-T3.Coord.Z)/norm*T3.Vitesse;
            angleRouge = atan(dY/dX)*360/2/3.14;

            //deplacement wagon
            dXW=(prochSommet_R.X-T3.Coord.X)/norm;
            dYW=(prochSommet_R.Y-T3.Coord.Y)/norm;
            dZW=(prochSommet_R.Z-T3.Coord.Z)/norm;

            W31.Coord.X=T3.Coord.X-dXW;
            W31.Coord.Y=T3.Coord.Y-dYW;
            W31.Coord.Z=T3.Coord.Z-dZW;

            W32.Coord.X=T3.Coord.X-2*dXW;
            W32.Coord.Y=T3.Coord.Y-2*dYW;
            W32.Coord.Z=T3.Coord.Z-2*dZW;

            //déplacement train

            T3.Coord.X=T3.Coord.X+dX;
            T3.Coord.Y=T3.Coord.Y+dY;
            T3.Coord.Z=T3.Coord.Z+dZ;

        // Mise a jour des indicateurs du train quand celui ci est sur le sommet
        }else{

            // tirage au hasard de l'arc à suivre
            int idArcrnd_R =rand()%graphe.getListArcSortantDuSommet(prochSommet_R).size();
            CArc prochainArc_R =graphe.getListArcSortantDuSommet(prochSommet_R).at(idArcrnd_R);

            //Test s'il n'y a personne sur l'arc
            if(prochainArc_R.id_arc!=T2.ArcSuivi.id_arc&prochainArc_R.id_arc!=T1.ArcSuivi.id_arc){
                T3.ArcSuivi = prochainArc_R;
                int Id_somme_depart = T3.ArcSuivi.id_sommet_ini;
                T3.Coord = (CPoint3f)graphe.list_sommet.at(Id_somme_depart);
                T3.IndPASuiv = 0;
            }
        }
    }

    glutPostRedisplay();
}

//Interaction clavier pour modifier la vitesse des trains (la touche enfoncée est en paramètre d'entrée)
void KeyListen(unsigned char touche, int x, int y){
    switch (touche)
    {
        case 'v' :
                if(T1.Vitesse < 0.2){
                    T1.Vitesse += 0.01;//augmenter la vitesse du tran vert
                    break;
                }

        case 'd' :
                if(T1.Vitesse > 0.2){
                    T1.Vitesse -= 0.01;//augmenter la vitesse du tran vert
                    break;
                }

        case 'b' :
                if(T2.Vitesse < 0.2){
                    T2.Vitesse += 0.01;//augmenter la vitesse du tran bleu
                    break;
                    } else {
                    T1.Vitesse -= T1.Vitesse/2;//diminuer la vitesse du train vert
                    break;
                }

        case 'y' :
                if(T2.Vitesse > 0.01){
                    T2.Vitesse -= 0.01;//diminuer la vitesse du train bleu
                    break;
                }else {
                    T2.Vitesse -= T2.Vitesse/2;
                    break;

                }

        case 'r' :
                if(T3.Vitesse < 0.2){
                    T3.Vitesse += 0.01;//augmenter la vitesse du tran rouge
                    break;
                }

        case 'z' :
                if(T3.Vitesse > 0.01){
                    T3.Vitesse -= 0.01;//diminuer la vitesse du train rouge
                    break;
                }


        case 'q':
            exit(0);//fermer la fenêtre
            break;

        case '-':
            distcam = distcam+1;//zoom sur la scène
            break;
        case '+':
            distcam = distcam-1;//dé-zoom sur la scène
            break;
    }
    glutPostRedisplay();
}

//Interaction clavier pour naviguer dans la scène (la touche enfoncée est en paramètre d'entrée)
void Spe_Keyboard(int key, int x, int y){

    //mouvement de la caméra avec les flèches directionnelles
    switch(key){

    case GLUT_KEY_UP:
            xcam = xcam+1;
            ycam = ycam+1;
            glutPostRedisplay();
            break;

    case GLUT_KEY_RIGHT:
            xcam = xcam+1;
            ycam = ycam-1;
            glutPostRedisplay();
            break;

    case GLUT_KEY_DOWN:
            xcam = xcam-1;
            ycam = ycam-1;
            glutPostRedisplay();
            break;

     case GLUT_KEY_LEFT:
            xcam = xcam-1;
            ycam = ycam+1;
            glutPostRedisplay();
            break;

            glutPostRedisplay();

    }
}





//Dessin en quadrique et avec des transformations d'un arbre (les coordonnées de l'arbre sont en paramètre d'entrée)
static void Arbre( GLUquadricObj * quad,float X, float Y, float Z){
 glPushMatrix();
        glColor3d(0.3,0.5,0.3);
        glTranslated(X, Y, Z);
        glRotatef(20 ,-0.8, 1, 0);
        gluCylinder(quad, 0.1, 0.1, 1.4, 105, 1);
        glColor3d(0.7,0.8,0.9);
        glTranslated(0, 0, 1.8);
        glRotatef(180 ,0, 1, 0);
        gluCylinder(quad, 0, 0.8, 0.8, 55, 1);
        gluCylinder(quad, 0, 0.4, 0.8, 45, 1);
        gluCylinder(quad, 0, 0.1, 0.8, 35, 1);
    glPopMatrix();
}

static void Arbre2(GLUquadricObj * quad,float X, float Y, float Z){
 glPushMatrix();
        glColor3d(0.3,0.5,0.3);
        glTranslated(X, Y, Z);
        glRotatef(20 ,-0.8, 1, 0);
        gluCylinder(quad, 0.1, 0.1, 1.4, 105, 1);
        glColor3d(0,255,0);
        glTranslated(0, 0, 1.8);
        glRotatef(180 ,0, 1, 0);
        glutSolidSphere(0.8, slices,stacks);
    glPopMatrix();
}
//**************************gare***********************
static void quais(GLfloat X, GLfloat Y, GLfloat Z, float Rotx, float Roty, float Rotz)
{
    glPushMatrix();
        glColor3d(0.5,0.5,0.5);
        glTranslated(X,Y,Z);
        glRotatef(90 ,90, 0, 0);
        glutSolidCube(1);
        glTranslated(1,0,0);
        glutSolidCube(1);
        glTranslated(0,1,0);
        glutSolidCube(1);
        glTranslated(-1,0,0);
        glutSolidCube(1);
        glColor3d(0.8,0.8,0.8);
        glTranslated(-0.25,-1,-0.5);
        glutSolidCube(0.5);
        glTranslated(0.5,0,0);
        glutSolidCube(0.5);
        glTranslated(0.5,0,0);
        glutSolidCube(0.5);
        glTranslated(0.5,0,0);
        glutSolidCube(0.5);
        //glPopMatrix();

    glPopMatrix();
}
// Creation de la gare quai nord
static void quain(GLfloat X, GLfloat Y, GLfloat Z, float Rotx, float Roty, float Rotz)
{
    glPushMatrix();
        glColor3d(0.5,0.5,0.5);
        glTranslated(X,Y,Z);
        glRotatef(90 ,Rotx, Roty, Rotz);
        glutSolidCube(1);
        glTranslated(1,0,0);
        glutSolidCube(1);
        glTranslated(0,1,0);
        glutSolidCube(1);
        glTranslated(-1,0,0);
        glutSolidCube(1);
        glColor3d(0.8,0.8,0.8);
        glTranslated(-0.25,-1,0.5);
        glutSolidCube(0.5);
        glTranslated(0.5,0,0);
        glutSolidCube(0.5);
        glTranslated(0.5,0,0);
        glutSolidCube(0.5);
        glTranslated(0.5,0,0);
        glutSolidCube(0.5);
        //glPopMatrix();

    glPopMatrix();
}


//Dessin en quadrique et avec des transformations d'un tunnel (les coordonnées du tunnel sont en paramètre d'entrée)
static void Tunnel( GLUquadricObj * quad,float X, float Y, float Z, float Rotx,
float Roty, float Rotz){
    glPushMatrix();
        glColor3d(1,0.5,0.5);
        glTranslated(X,Y,Z);
        glRotatef(Rotx ,Roty, Rotz, 0);
        gluQuadricDrawStyle(quad, GLU_LINE);
        gluCylinder(quad, 0.8, 0.8, 4, 35, 5);

    glPopMatrix();
}

//Dessin en quadrique etavec des transformations et des vertex d'une maison (les coordonnées de la maison sont en paramètre d'entrée)
static void maison(GLfloat X, GLfloat Y, GLfloat Z, float Rotx, float Roty, float
Rotz){
    glPushMatrix();
    glColor3d(0.1,0.5,0.5);
    glTranslated(X,Y,Z);
    glRotatef(Rotx ,2, Rotz, 0);
    glutSolidCube(2);                 // bâtiment

    glTranslatef(0,1.4,0);
    glPushMatrix();
    glColor3d(0.1,0.5,0.5);                // toit
    glRotatef(-90,2,0,0);
    glutSolidCone(2,1,16,8);
    glPopMatrix();

    glTranslatef(.75,.5,-.75);
    glPushMatrix();                   // cheminée
    glScalef(1,3,1);
    glutSolidCube(.25);
    glPopMatrix();
    glPopMatrix();

    glTranslatef(0,-.65,2);

    glPopMatrix();

}
//Dessin en quadrique etavec des transformations et des vertex d'une carosse (les coordonnées de la carosse
static void carosse(GLfloat X, GLfloat Y, GLfloat Z, float Rotx, float Roty, float
Rotz){
     // carosse
    glPushMatrix();
    glTranslated(X,Y,Z);
    glRotatef(Rotx ,Roty, Rotz, 0);
    glPushMatrix();                   // body
    glScalef(2,.5,1);
    glutSolidCube(.5);
    glPopMatrix();
    glTranslatef(0,0,.25);
    glPushMatrix();
    glTranslatef(-.4,-.2,0);
    glutSolidTorus(.05,.1,8,8);       // roue
    glTranslatef(.8,0,0);
    glutSolidTorus(.05,.1,8,8);       // roue
    glPopMatrix();
    glTranslatef(0,0,-.5);
    glPushMatrix();
    glTranslatef(-.4,-.2,0);
    glutSolidTorus(.05,.1,8,8);       // roue
    glTranslatef(.8,0,0);
    glutSolidTorus(.05,.1,8,8);       // roue
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();

}







//***********soliel***********
 void soleil(float X, float Y, float Z)
{
    glPushMatrix();
        glColor3d(255,255,0);
        glTranslated(X, Y, Z);
        glutSolidSphere(1.5,slices,stacks);
    glPopMatrix();
}
