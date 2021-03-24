#include "Escultor.h"
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#define RAIO 40 // Constante de referencia
#define PI 3.141592653589793238463

void drawTemple(int r, Sculptor &e, float t, float w){
	//Seta a cor para vermelho/laranja para desenhar o templo a partir da base
	e.setColor8(115,12,12,0.8);
	//desenha o templo a partir da base
	e.putCylinder(0,0,3*w+2,2*r-2*w,r-w-2);
	e.cutBox(0,2*r-2*w,-2*w,2*w,3*w+2,r-w-3);
	e.cutBox(0,2*r-2*w,-6*w,-10*w,3*w+2,r-w-3);
	e.cutBox(0,2*r-2*w,6*w,10*w,3*w+2,r-w-3);
	e.putCylinder(0,0,r+2*w+1,2*r+2*w,1);
	e.putCylinder(0,0,r+8*w+1,r+6*w,1);
	e.putCylinder(0,0,2*r+1+5*w,r-2*w,1);
	e.putCylinder(0,0,2*r+5*w+1,r,1);
	//Seta a cor para preto para o interior do templo
	e.setColor8(0,0,0,0.8);
	//desenha o interior do templo
	e.putCylinder(0,0,3*w+2,(2*r-2*w)-2,r-w-2);
	//Seta a cor para azul ceu para as paredes do templo
	e.setColor8(58,65,175,0.8);
	//desenha a parte azul do templo
	e.putCylinder(0,0,r-w,(2*r-2*w)+2,w);
	e.putCylinder(0,0,r+w/2,(2*r-2*w)+2,3*w/2);
	e.putCylinder(0,0,r+5*w,r+4*w,3*w);
	e.putCylinder(0,0,2*r+w,r-2*w,4*w);
	e.putBox(r-4,r-1,-4,4,2*r+w+1,2*r+5*w-1);
	//Seta a cor para cinza escuro para telhado do templo
	e.setColor8(35,35,65,0.8);
	//desenha o telhado do templo
	for(unsigned l=0;l<3*w+1;l++){
        e.putCylinder(0,0,r+2*w+(l*w/4),2*r+2*w-(l*w/2),w/4);}
   for(unsigned m=0;m<3*w+1;m++){
        e.putCylinder(0,0,r+8*w+(m*w/4),r+6*w-(m*w/2),w/4);}
    for(unsigned n=0;n<3*w+3;n++){
        e.putCylinder(0,0,2*r+5*w+(n*1),40-(n*2.5),1);}
    e.putCylinder(0,0,2*r+8*w,w,2*w+3);
    //Seta a cor para dourado para o topo do templo
	e.setColor8(205,145,0,0.8);
	//desenha o topo do templo e a placa
    e.putCylinder(0,0,3*r,w-2,3*w+3);
    e.putSphere(0,0,3*r+2*w+3,w);
    e.putBox(r-w,r-2,-5,5,2*r+w,2*r+5*w);
    for(unsigned o=1;o<=3;o++){
        e.putSphere(r,0,2*r+w+1+(4*o),w/2);}
}

void drawBase(int r, Sculptor &e){
	float t = 5*r;//tamanho do quadrado onde as coisas do cenário estarão contidas
	float w = r/10;//referencia menor
	//Seta a cor para bege para desenhar a chão
	e.setColor8(175,162,159,0.8);
	//desenha o altar do templo
	e.putCylinder(0,0,0,t,w+10);
	e.cutCylinder(0,0,w,t-5,w+10);
	e.putCylinder(0,0,0,t-25,2*w+10);
	e.cutCylinder(0,0,2*w,t-30,2*w+10);
	e.putCylinder(0,0,0,t-50,3*w+10);
	e.cutCylinder(0,0,3*w,t-55,3*w+10);
	//desenha os detalhes dos apoios do altar
	for(unsigned j=1;j<3*w;j++){
        for(unsigned k=0;k<=2*w+3;k++){
            if(k==0){
                float Xs = (t-52.5)*cos(j*6);
                float Ys = pow(((t-52.5)*(t-52.5)-(Xs*Xs)),0.5);
                e.cutCylinder(Xs,Ys,3*w+2.5,w,w);
                e.cutCylinder(-Xs,Ys,3*w+2.5,w,w);
                e.cutCylinder(Xs,-Ys,3*w+2.5,w,w);
                e.cutCylinder(-Xs,-Ys,3*w+2.5,w,w);
                if(j<=5){
                    float Xz = (t-52.5)*cos(0.1013+(j-1)*(PI/10));
                    float Yz = pow(((t-52.5)*(t-52.5)-(Xz*Xz)),0.5);
                    e.putCylinder(Xs,Ys,3*w,w-2,3*w);
                    e.putCylinder(-Xs,Ys,3*w,w-2,3*w);
                    e.putCylinder(Xs,-Ys,3*w,w-2,3*w);
                    e.putCylinder(-Xs,-Ys,3*w,w-2,3*w);}}
            if(k==1){
                float Xs = (t-27.5)*cos(j*6);
                float Ys = pow(((t-27.5)*(t-27.5)-(Xs*Xs)),0.5);
                e.cutCylinder(Xs,Ys,2*w+2.5,w,w);
                e.cutCylinder(-Xs,Ys,2*w+2.5,w,w);
                e.cutCylinder(Xs,-Ys,2*w+2.5,w,w);
                e.cutCylinder(-Xs,-Ys,2*w+2.5,w,w);
                if(j<=5){
                    float Xz = (t-27.5)*cos(0.0899+(j-1)*(PI/10));
                    float Yz = pow(((t-27.5)*(t-27.5)-(Xz*Xz)),0.5);
                    e.putCylinder(Xs,Ys,2*w,w-2,3*w);
                    e.putCylinder(-Xs,Ys,2*w,w-2,3*w);
                    e.putCylinder(Xs,-Ys,2*w,w-2,3*w);
                    e.putCylinder(-Xs,-Ys,2*w,w-2,3*w);}}
            if(k==2){
                float Xs = (t-2.5)*cos(j*6);
                float Ys = pow(((t-2.5)*(t-2.5)-(Xs*Xs)),0.5);
                e.cutCylinder(Xs,Ys,w+2.5,w,w);
                e.cutCylinder(-Xs,Ys,w+2.5,w,w);
                e.cutCylinder(Xs,-Ys,w+2.5,w,w);
                e.cutCylinder(-Xs,-Ys,w+2.5,w,w);
                if(j<=5){
                    float Xz = (t-2.5)*cos(0.0808+(j-1)*(PI/10));
                    float Yz = pow(((t-2.5)*(t-2.5)-(Xz*Xz)),0.5);
                    e.putCylinder(Xs,Ys,w,w-2,3*w);
                    e.putCylinder(-Xs,Ys,w,w-2,3*w);
                    e.putCylinder(Xs,-Ys,w,w-2,3*w);
                    e.putCylinder(-Xs,-Ys,w,w-2,3*w);}}}}
	//abre espaço para as escadarias
	e.cutBox(t-58,t,-4*w,4*w,0,3*w+10);
	e.cutBox(58-t,-t,-4*w,4*w,0,3*w+10);
	e.cutBox(-4*w,4*w,t-58,t,0,3*w+10);
	e.cutBox(-4*w,4*w,58-t,-t,0,3*w+10);
	//Seta a cor para um bege/cinza mais escuro
    e.setColor8(202,204,195,0.8);
	//desenha as escadarias
	for(unsigned i=1;i<=3*w;i++){
        e.putBox(t+2-4*(i-1),t-2-4*(i-1),-4*w,4*w,0,i);
        e.putBox(-t-2+4*(i-1),-t+2+4*(i-1),-4*w,4*w,0,i);
        e.putBox(-4*w,4*w,t+2-4*(i-1),t-2-4*(i-1),0,i);
        e.putBox(-4*w,4*w,-t-2+4*(i-1),-t+2+4*(i-1),0,i);}
    //desenha o caminho no altar até o templo
    e.putCylinder(0,0,3*w,2*r,2);
    e.putBox(-4*w,4*w,0,t-46,2*w,3*w+2);
    e.putBox(-4*w,4*w,0,46-t,2*w,3*w+2);
    e.putBox(0,t-46,-4*w,4*w,2*w,3*w+2);
    e.putBox(0,46-t,-4*w,4*w,2*w,3*w+2);
    //Seta a cor para bege para desenhar a piso
	e.setColor8(175,162,159,0.8);
	//desenha o chão
	e.putCylinder(0,0,-2,t,2);
    //desenha os demais componentes do cenário (estrutura do templo)
    drawTemple(r,e,t,w);
}

int main(){
	Sculptor s;//Cria um sculptor para fazer os desenhos
	drawBase(RAIO,s);//Desenha o universo
	s.write("Templo_do_ceu.off");//Escreve no arquivo
}
