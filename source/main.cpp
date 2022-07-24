#include "../include/RSPL.hpp"

RSGL::window win("Lonic Jump",{500,500,500,700},{255,255,255});
bool running = true;

enum ptypes{NORMAL,PLANK,MOVINGN,MOVINGP,NONE}; 

std::string lonic="res/images/lonic.png";
RSGL::Button restart({win.r.width/3+100,win.r.length/3+40,40,40},{0});

int main(){
    srand(time(NULL));
    RSGL::entity player((RSGL::rect){250,151,80,80},RSGL::object);

	/*initialize platforms */ 
	std::vector<RSGL::entity> platforms; std::vector<ptypes> platformTypes;
	for (platforms; platforms.size() < 12; platforms.insert(platforms.end(),{(RSGL::rect){rand() % 500-68,rand() % 700 + 100,68,14},RSGL::platform})){}
    for (platformTypes; platformTypes.size() < platforms.size(); platformTypes.insert(platformTypes.end(),NONE)){}

	float dy = 0; // player's down velocity
    int up=0; int vs[10]={1,1,1,1,1,1,1,1,1,1};

	while (running){
        win.checkEvents(); dy += 0.2;
	    if (win.event.type == RSGL::quit) running=false;

		if (win.isPressed("A") || win.isPressed("Left")) player.r.x -= 4;
		else if (win.isPressed("D") || win.isPressed("Right")) player.r.x += 4;

		if (player.r.x > 500) player.r.x = 0;
		if (player.r.x < -45) player.r.x = win.r.width - player.r.width;

		// player's jump mechanism
        if (player.fall(platforms)) player.r.y+=5;
	    else if (player.jump(platforms)){ lonic="res/images/lonic.png"; player.r.length=80; player.r.y -= player.ifCollide({},{0,-8},8);}
        else{ 
            bool jump=true; 
            for (int i = 0; i < platforms.size(); i++) if (RSGL::RectCollideRect(player.r,platforms[i].r) && !(platformTypes[i] == NORMAL || platformTypes[i] == MOVINGN)){ jump=false; platforms[i].r.x=-200; break; }
            if (jump){ player.jump(50,1); lonic="res/images/lonicLow.png"; player.r.length=70;}
        }
        for (int i = 0; i < platforms.size(); i++){
            if (player.r.y < 150 && i != up){
                if (i==9) player.r.y = 400;  platforms[i].r.y += dy; up=i;
                if (platforms[i].r.y > 700){ platforms[i].r={rand() % 500-68,0,68,14}; platformTypes[i]=NONE; up=-1; }
            }
            if (platformTypes[i]==MOVINGN || platformTypes[i]==MOVINGP){
                if (RSGL::RectCollideRect(platforms[i].r,{-10,0,1,win.r.length}) || RSGL::RectCollideRect(platforms[i].r,{win.r.width-2,0,11,win.r.length}) ) vs[i] = vs[i]*-1;
                platforms[i].r.x+=5*vs[i];
            }   
            if (platformTypes[i]==NONE){ 
                int rn = rand() % 100; RSGL::point ranges[4]={{0,50},{51,75},{76,90},{91,100}};
                ptypes ts[4] = {NORMAL,PLANK,MOVINGN,MOVINGP};
                for (int j=0; j < 4; j++) if(rn >= ranges[j].x && rn <= ranges[j].y){ platformTypes[i]=ts[j]; break; }
            }
            if (platformTypes[i] == NORMAL || platformTypes[i] == MOVINGN){
                RSGL::drawRoundRect(platforms[i].r,{0,0,0});
                RSGL::drawRoundRect({platforms[i].r.x+1,platforms[i].r.y+1,platforms[i].r.width-2,platforms[i].r.length-2},{0,255,0});
            } else RSGL::drawImage("res/images/broken.png",platforms[i].r);
        }
        RSGL::drawImage(lonic,player.r);
        if (player.r.y > win.r.length+player.r.length){
            RSGL::drawText("Lonic fell.",{win.r.width/3,win.r.length/3,25},"res/fonts/PublicPixel.ttf",{255,0,0});
            restart.checkEvents(); RSGL::drawImage("res/images/restart.png",restart.rect);
            if (restart.event == 1){
                platforms={}; platformTypes={};
                for (platforms; platforms.size() < 12; platforms.insert(platforms.end(),{(RSGL::rect){rand() % 500-68,rand() % 700 + 100,68,14},RSGL::platform})){}
                for (platformTypes; platformTypes.size() < platforms.size(); platformTypes.insert(platformTypes.end(),NONE)){}
                dy = 0;  up=0; 
                RSGL::entity newplayer((RSGL::rect){250,151,80,80},RSGL::object); player=newplayer;
            }
        }
		win.clear();
	}
}