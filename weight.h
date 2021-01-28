//Определение класса weight

#ifndef WEIGHT_H
#define WEIGHT_H

#include <graphics.h>

class weight //Класс соответствующий одному грузу
{           
  public:
  float X0,Y0; //Координаты положения равновесия                 
  float X1,X2,X3,Y1,Y2,Y3; //Отклонения по X и Y в три различных момента времени
  int t; //время затухания колебаний
  
  int dev(); //проверка на наличие отклонения(deviate)
  void draw(int ph,float cX,float cY); //рисование груза, в зависимости от фазы цикла и коээфициентов сжатия картинки       
};


int weight::dev()
{
    if ((X1 == X2)&&(X2 == X3)&&(Y1 == Y2)&&(Y2 == Y3)&&(X1==0)&&(Y1==0))
    {
      return 0;     
    }
    else
    {
      return 1;  
    }  
};

void weight::draw(int ph,float cX,float cY)
{
    if (ph==1)
    {
      fillellipse((X0+X1)*cX,(Y1+Y0)*cY,3,3);     
    }
    else if (ph==2)
    {
      fillellipse((X0+X3)*cX,(Y0+Y3)*cY,3,3);  
    }
    else
    {
      fillellipse((X0+X2)*cX,(Y0+Y2)*cY,3,3);   
    }  
}; 
 
#endif /* WEIGHT_H */
