#include <SFML/Graphics.hpp>
#include <time.h>
#include "Connector.hpp"
using namespace sf;

int size = 56;
Vector2f offset(28,28);

Sprite f[32]; //figures
std::string position="";//defines the most important functions of the standard std::string class: removing, random access

int board[8][8] =  // حق المواقع حق الخلفية 
    {-1,-2,-3,-4,-5,-3,-2,-1,
	 -6,-6,-6,-6,-6,-6,-6,-6,
	  0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0,
	  6, 6, 6, 6, 6, 6, 6, 6,
	  1, 2, 3, 4, 5, 3, 2, 1};

std::string toChessNote(Vector2f p) // يخزن الموقع القديم في الشاشة السوداء
{
  std::string s = "";
  s += char(p.x/size+97);
  s += char(7-p.y/size+49);
  return s;
}

Vector2f toCoord(char a,char b) // حق لما يكلة
{
   int x = int(a) - 97;
   int y = 7-int(b)+49;
   return Vector2f(x*size,y*size);
}

void move(std::string str)
{
    Vector2f oldPos = toCoord(str[0],str[1]);
    Vector2f newPos = toCoord(str[2],str[3]);
                 // حق الاكل
	for(int i=0;i<32;i++)
     if (f[i].getPosition()==newPos) f[i].setPosition(-100,-100);
		
    for(int i=0;i<32;i++)
     if (f[i].getPosition()==oldPos) f[i].setPosition(newPos); 

	
}

void loadPosition()    //  توزيع اللعابين *************
{
	int k=0;
  	for(int i=0;i<8;i++)
 	 for(int j=0;j<8;j++)
     {
       int n = board[i][j];
	   if (!n) continue; // شرط
	   int x = abs(n)-1;
	   int y = n>0?1:0;
	   f[k].setTextureRect( IntRect(size*x,size*y,size,size) );//figures
	   f[k].setPosition(size*j,size*i);
	   k++;
     }                                   //**************

	 for(int i=0;i<position.length();i+=5) // حق التراجع 
	   move(position.substr(i,4));
}


int main()
{
    RenderWindow window(VideoMode(504, 504), "The Chess! (press SPACE)");
          //حجم الشاشة
		

	Texture t1,t2;   // اضافة الصور
    t1.loadFromFile("images/figures.png"); 
	t2.loadFromFile("images/board.png");

	for(int i=0;i<32;i++) f[i].setTexture(t1);// حق اظهار اللعابين
	Sprite sBoard(t2); 

	loadPosition();  // التوزيع

    bool isMove=false;
	float dx=0, dy=0;
	Vector2f oldPos,newPos;
	std::string str;
	int n=0; 

    while (window.isOpen())
    {
   	    Vector2i pos = Mouse::getPosition(window) - Vector2i(offset);

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

		
			/////drag and drop///////*******************************
			if (e.type == Event::MouseButtonPressed)
				if (e.key.code == Mouse::Left)
				  for(int i=0;i<32;i++)
				  if (f[i].getGlobalBounds().contains(pos.x,pos.y))
					  {
					   isMove=true; n=i;// مصفوفةعلشان الشكل
					   dx=pos.x - f[i].getPosition().x;
					   dy=pos.y - f[i].getPosition().y;
					   oldPos  =  f[i].getPosition(); //الموقع القديم
                      }

             if (e.type == Event::MouseButtonReleased)
				if (e.key.code == Mouse::Left)
				 {
				  isMove=false;
			      Vector2f p = f[n].getPosition() + Vector2f(size/2,size/2);// كل واحد لا يدخل في موقع الثاني
                  newPos = Vector2f( size*int(p.x/size), size*int(p.y/size) );//  كل واحد لا يدخل في موقع الثاني 
				  str = toChessNote(oldPos)+toChessNote(newPos);
				  move(str); // استدعاء دالة الاكل 
				  if (oldPos!=newPos) position+=str+" ";
			      f[n].setPosition(newPos);                   
			     }                       
		                   //***********************************

       //comp move
	   if (Keyboard::isKeyPressed(Keyboard::Space))
       {
		 str =  getNextMove(position);
 		 		 
         oldPos = toCoord(str[0],str[1]);
         newPos = toCoord(str[2],str[3]);
		 
         for(int i=0;i<32;i++) if (f[i].getPosition()==oldPos) n=i;
         
		 

		move(str);  position+=str+" ";
		f[n].setPosition(newPos); 
        }

		if (isMove) f[n].setPosition(pos.x-dx,pos.y-dy);

    ////// draw  ///////
	window.clear();  // ضروري قبل الرسم
    window.draw(sBoard);
	for(int i=0;i<32;i++) f[i].move(offset);
    for(int i=0;i<32;i++) window.draw(f[i]); window.draw(f[n]);// اللعابين
	for(int i=0;i<32;i++) f[i].move(-offset);
 	window.display();
	}

	//CloseConnection();

    return 0;
}
