#include <gb/gb.h>
#include "Rouse_16x16_GBTD.c"
#include "newCharacter.c"
#include "bg_Tile_Forest.c"
#include "map_forest.c"
#include "PY_Splashscreen_data.c"
#include "PY_Splashscreen_map.c"
#include <gb/font.h>
#include <stdio.h>
#include "windowmap.c"

#define mapSizeX 64
struct myNewCharacter rouse;
struct myNewCharacter monster_dog;
struct myNewCharacter rousew;

UBYTE scrollX = 0;
BYTE tileCounter = 0;
UBYTE temporala = 0, temporalb = 0;
UWORD counter;
//UINT8 screnSizex = 360;
UINT8 spriteSize = 8;
UINT8 gravity = -2;
UINT8 currentSpeedY;
UINT8 floorPosition=120;
UINT8 playerPosition;
BYTE jumping;
UINT8 i;
UINT8 HP = 100;
font_t mini_font; //variable for the chars/font

UBYTE checkCollision(struct myNewCharacter* one,struct myNewCharacter* two) //Revisar la logica de esta linea
{
    return (one->x >= two->x && one->x <= two->x + two->width) && (one->y >= two->y && one->y <= two->y + two->height) || (two->x >= one->x && two->x <= one->x + one->width) && (two->y >= one->y && two->y <= one->y + one->height);
}

UBYTE isDeath(UINT8 stateHP)
{
    if(stateHP == 0)
    {
        return 0;
    }
    else{
        return 1;
    }
}

void moveCharacter(struct myNewCharacter* myCharacter, UINT8 positionX, UINT8 positionY)
{
    move_sprite(myCharacter->spriteID[0], positionX, positionY);
    move_sprite(myCharacter->spriteID[1], positionX + spriteSize, positionY);
    move_sprite(myCharacter->spriteID[2], positionX, positionY + spriteSize);
    move_sprite(myCharacter->spriteID[3], positionX + spriteSize, positionY + spriteSize);
}

INT8 hitSurface(UINT8 projectedYposition)
{
    if(projectedYposition >= floorPosition)
    {
        return floorPosition;
    }
    return -1;
}

void jump()
{
    INT8 possibleFloor;

    if(jumping==0)
    {
        jumping = 1;
        currentSpeedY = 10;
    }
    currentSpeedY = currentSpeedY + gravity;
    rouse.y= rouse.y - currentSpeedY;

    possibleFloor = hitSurface(rouse.y);

    if(possibleFloor > -1)
    {
        jumping = 0;
        moveCharacter(&rouse,rouse.x, possibleFloor);
    }
    else
    {
        moveCharacter(&rouse,rouse.x,rouse.y);
    }
}

void performtdelay(UINT8 numloops)
{
    UINT8 ii;
    for(ii=0; ii<numloops; ii++)
    {
        wait_vbl_done();
    } 
}

void setUpMainCharacter()
{
    rouse.x = 14;
    rouse.y = floorPosition;
    rouse.width = 9;
    rouse.height = 16;

    set_sprite_tile(0,0);
    rouse.spriteID[0] = 0;
    set_sprite_tile(1,1);
    rouse.spriteID[1] = 1;
    set_sprite_tile(2,2);
    rouse.spriteID[2] = 2;
    set_sprite_tile(3,3);
    rouse.spriteID[3] = 3;

    moveCharacter(&rouse, rouse.x, rouse.y);
    
}

void setUpwalk()
{
    rousew.x = 14;
    rousew.y = floorPosition;
    rousew.width = 9;
    rousew.height = 16;

    set_sprite_tile(8,8);
    rouse.spriteID[0] = 8;
    set_sprite_tile(9,9);
    rouse.spriteID[1] = 9;
    set_sprite_tile(10,10);
    rouse.spriteID[2] = 10;
    set_sprite_tile(11,11);
    rouse.spriteID[3] = 11;

    moveCharacter(&rousew, rousew.x, rousew.y);

}

void setUpFirstEnemy()
{
    monster_dog.x = 100;
    monster_dog.y = floorPosition;
    monster_dog.width = 13;
    monster_dog.height = 9;

    set_sprite_tile(4,4);
    monster_dog.spriteID[0] = 4;
    set_sprite_tile(5,5);
    monster_dog.spriteID[1] = 5;
    set_sprite_tile(6,6);
    monster_dog.spriteID[2] = 6;
    set_sprite_tile(7,7);
    monster_dog.spriteID[3] = 7;

     moveCharacter(&monster_dog, monster_dog.x, monster_dog.y);
}

UINT8 healthState()
{
    if(checkCollision(&rouse, &monster_dog))
    {
      HP -= 10;
    }
    return HP;
}

void fadeOut(){
    
    for(i=0; i<4;i++) {
        switch (i)
        {
        case 0:
            BGP_REG = 0xE4;
            break;
        case 1:
            BGP_REG = 0xF9;
            break;
        case 2:
            BGP_REG = 0xFE;
            break;
        case 3:
            BGP_REG = 0xFF;
            break;
        default:
            break;
        }
        performtdelay(5);
    }
}

void fadeIn(){
    
    for(i=0; i<3;i++) {
        switch (i)
        {
        case 0:
            BGP_REG = 0xFE;
            break;
        case 1:
            BGP_REG = 0xF9;
            break;
        case 2:
            BGP_REG = 0xE4;
            break;
        }
        performtdelay(5);
    }
}

void gameOverScreen(){
    HIDE_SPRITES;
    fadeOut();
    fadeIn();
    printf("Game over");
}


void main(){
    
    set_bkg_data(0,256,PY_Splashscreen_data);
    set_bkg_tiles(0,0,20,18,PY_Splashscreen_map);
    SHOW_BKG;
    DISPLAY_ON;
    waitpad(J_START);
    fadeOut();

    
    font_init(); // initialitate the font library
    mini_font = font_load(font_min); //load the type of font
    font_set(mini_font);
    
    jumping = 0;
    counter = 0;
    set_sprite_data(0,20,Rouse_character);
    setUpMainCharacter();
    setUpFirstEnemy();
    set_bkg_data(37,56,&tile_Forest);
    set_bkg_tiles(0,0,20,18,map_Forest);
    /*for(temporala = 0; temporala != 18; temporala++){

    set_bkg_tiles(0,temporala,20,18,(map_Forest+counter));
    counter= counter + mapSizeX;
    }*/
    set_win_tiles(0,0,7,0,window_map);
    move_win(8,136);
    SHOW_SPRITES;
    SHOW_WIN;
    fadeIn();

    while(isDeath(HP))
    {
        wait_vbl_done;
        if((joypad() & J_A) || jumping == 1)
        {
            jump();
        }
         if((joypad() & J_LEFT) && (scrollX != 0)){
             scroll_bkg(-1,0);
             tileCounter--;
           rouse.x -= 2;
           moveCharacter(&rouse, rouse.x, rouse.y);
           //moveCharacter(&rousew,rouse.x,rouse.y);
       }
       if((joypad() & J_RIGHT) && (scrollX < mapSizeX - 20)){
           scroll_bkg(1,0);
           tileCounter++;
           rouse.x += 2;
           moveCharacter(&rouse, rouse.x, rouse.y);
           //moveCharacter(&rousew,rouse.x,rouse.y);
       }

      if(tileCounter == 8){
           scrollX++;
           tileCounter = 0;

           counter = scrollX + 21;
           temporalb = counter % 32;

           for(temporala = 0; temporala != 18; temporala++){
               set_bkg_tiles(temporalb, temporala,6,1, *(map_Forest+counter));
               counter = counter + mapSizeX;
           }
       }
       /*if(rouse.x >= mapSizeX){
           rouse.x = 14;
           scroll_bkg(144,0);
       }*/ //Requiere de otra condicion para evitar reiniciar pos del persoanje.

       monster_dog.x -= 5;
    

       if(monster_dog.x <= 0){
           monster_dog.x = 144;
       }
       
    
       moveCharacter(&monster_dog,monster_dog.x,monster_dog.y);
       healthState();

    performtdelay(5);
    }
    gameOverScreen();
}