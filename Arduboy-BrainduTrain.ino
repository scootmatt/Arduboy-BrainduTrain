#include <Arduboy.h>
#include "braindu_bitmaps.h"

#define _TIMELIFE 86
#define _FRAMERATE 8
#define _COUNTDOWN 3

byte pad,pad1,pad2,pad3;     //Button press buffer used to stop pause repeating
byte firepad,firepad1;
byte oldpad,oldpad1,oldpad2,oldpad3;
byte oldfirepad,oldfirepad1;
byte select=0;            //used for selection on menu screen
byte timelife=_TIMELIFE;      //counts down life with time
byte currentScore = 0;
byte countdown = (_COUNTDOWN * _FRAMERATE)-1;   //frames used for countdown
boolean start=false;    //If in title or not
boolean menuselect=false;   //If menu selction made or not
boolean gameselect=false;   //if game selected or not

Arduboy arduboy;

//Used to delay images while reading button input
boolean pollFireButton(int n)
{
  byte pass = 0;
  for(int i = 0; i < n; i++)
  {
    delay(15);
    firepad = arduboy.pressed(A_BUTTON);
    firepad1 = arduboy.pressed(B_BUTTON);
    if(firepad == 1 && oldfirepad == 0)
    {
      pass = 16;
    }
    oldfirepad = firepad;
    if(firepad1 == 1 && oldfirepad1 == 0)
    {
      pass = 32;
    }
    oldfirepad1 = firepad1;
    if(pass != 0) { return true; }
  }
  return false;
}

byte getDirectionBut(int n)
{
  byte pass = 0;
  for(int i = 0; i < n; i++)
  {
    delay(15);
    pad = arduboy.pressed(UP_BUTTON);
    pad1 = arduboy.pressed(DOWN_BUTTON);
    pad2 = arduboy.pressed(LEFT_BUTTON);
    pad3 = arduboy.pressed(RIGHT_BUTTON);
    firepad = arduboy.pressed(A_BUTTON);
    firepad1 = arduboy.pressed(B_BUTTON);
    if(pad == 1 && oldpad == 0) { pass = 1; }
    oldpad = pad;
    if(pad1 == 1 && oldpad1 == 0) { pass = 2; }
    oldpad1 = pad1;
    if(pad2 == 1 && oldpad2 == 0) { pass = 4; }
    oldpad2 = pad2;
    if(pad3 == 1 && oldpad3 == 0) { pass = 8; }
    oldpad3 = pad3;
    if(firepad == 1 && oldfirepad == 0) { pass = 16; }
    oldfirepad = firepad;
    if(firepad1 == 1 && oldfirepad1 == 0) { pass = 32; }
    oldfirepad1 = firepad1;
    if(pass != 0) { return pass; }
  }
  return pass;
}

boolean titlescreen()
{
  //Clears the screen
  arduboy.clear();
  arduboy.setCursor(5,2);
  arduboy.setTextSize(2);
  arduboy.print("Braindu-");
  arduboy.setCursor(60,25);
  arduboy.print("Train");
  arduboy.setTextSize(1);
  arduboy.drawBitmap(5, 25, title_brain, 32, 24, WHITE);
  arduboy.display();
  if (pollFireButton(25))
  {
    return true;
  }

  //Flash "Press FIRE" 5 times
  for(byte i = 0; i < 5; i++)
  {
    //Draws "PRESS A or B to BEGIN"
    arduboy.setCursor(2, 55);
    arduboy.print("PRESS A or B to BEGIN");
    arduboy.display();

    if (pollFireButton(40))
    {
      return true;
    }
    //Removes "PRESS A or B to BEGIN"
    arduboy.clear();
    arduboy.setCursor(5,2);
    arduboy.setTextSize(2);
    arduboy.print("Braindu-");
    arduboy.setCursor(60,25);
    arduboy.print("Train");
    arduboy.setTextSize(1);
    arduboy.drawBitmap(5, 25, title_brain, 32, 24, WHITE);
    arduboy.display();

    arduboy.display();
    if (pollFireButton(25))
    {
      return true;
    }
  }

  return false;
}

boolean menuscreen()
{
  while(!(gameselect)) {
    arduboy.clear();                        
    //arduboy.setCursor(0,0);                 
    //arduboy.setTextSize(1);
    //arduboy.print("*menu select screen*");
    byte sel_point_x,sel_point_y;
    switch (select) {
      case 0:
      case 1:
      case 2:
        sel_point_x = 14 + (35 * select);
        sel_point_y = 9;
        break;
      case 3:
      case 4:
      case 5:
        sel_point_x = 14 + (35 * (select - 3));
        sel_point_y = 29;
        break;
      case 6:
      case 7:
      case 8:
        sel_point_x = 14 + (35 * (select - 6));
        sel_point_y = 49;
        break;
      }
    //arduboy.drawBitmap(14, 9, LH_arrow, 4, 7, WHITE);
    //arduboy.drawBitmap(39, 9, RH_arrow, 4, 7, WHITE);
    arduboy.drawBitmap(sel_point_x,sel_point_y, LH_arrow, 4, 7, WHITE);
    arduboy.drawBitmap(sel_point_x + 25,sel_point_y, RH_arrow, 4, 7, WHITE);
    arduboy.drawBitmap(20, 5, menu_tile_0, 17, 16, WHITE);
    arduboy.drawBitmap(55, 5, menu_tile_1, 17, 16, WHITE);
    arduboy.drawBitmap(90, 5, menu_tile_2, 17, 16, WHITE);
    arduboy.drawBitmap(20, 25, menu_tile_3, 17, 16, WHITE);
    arduboy.drawBitmap(55, 25, menu_tile_4, 17, 16, WHITE);
    arduboy.drawBitmap(90, 25, menu_tile_5, 17, 16, WHITE);
    arduboy.drawBitmap(20, 45, menu_tile_6, 17, 16, WHITE);
    arduboy.drawBitmap(55, 45, menu_tile_7, 17, 16, WHITE);
    arduboy.drawBitmap(90, 45, menu_tile_8, 17, 16, WHITE);
    arduboy.display();
    //delay(1000);
    byte pres = getDirectionBut(50);
    switch (pres) {
      case 0: break;
      case 1: select += 6; arduboy.tunes.tone(5000,10); break;
      case 2: select += 3; arduboy.tunes.tone(5000,10); break;
      case 4: {
        if( (select == 0) || (select == 3) || (select == 6) ) {
          select += 2;
        }
        else { select -= 1; }
        arduboy.tunes.tone(5000,10);
        break;
      }
      case 8: {
        if( (select == 2) || (select == 5) || (select == 8) ) {
          select -= 2;
        }
        else { select++; }
        arduboy.tunes.tone(5000,10);
        break;
      }
      case 16: gameselect = true; break;
      case 32: gameselect = true; break;
      default: break;
    }
    if (select >= 9) { select -= 9; }
    
  }
  return true;
}

void drawActFrames(void) {
  arduboy.clear();
  arduboy.drawRect(0,0,105,64,WHITE);
  arduboy.drawRect(107,0,21,64,WHITE);
  switch(select) {
    case 0: arduboy.drawBitmap(109, 46, menu_tile_0, 17, 16, WHITE); break;
    case 1: arduboy.drawBitmap(109, 46, menu_tile_1, 17, 16, WHITE); break;
    case 2: arduboy.drawBitmap(109, 46, menu_tile_2, 17, 16, WHITE); break;
    case 3: arduboy.drawBitmap(109, 46, menu_tile_3, 17, 16, WHITE); break;
    case 4: arduboy.drawBitmap(109, 46, menu_tile_4, 17, 16, WHITE); break;
    case 5: arduboy.drawBitmap(109, 46, menu_tile_5, 17, 16, WHITE); break;
    case 6: arduboy.drawBitmap(109, 46, menu_tile_6, 17, 16, WHITE); break;
    case 7: arduboy.drawBitmap(109, 46, menu_tile_7, 17, 16, WHITE); break;
    case 8: arduboy.drawBitmap(109, 46, menu_tile_8, 17, 16, WHITE); break;
  }
}

void drawTimeLife(void) {
  byte tall = timelife >> 1;
  byte ystart = 2 + (43 - tall);
  arduboy.fillRect(109,ystart,8,tall,WHITE);
}

void drawScore(void) {
  byte score[3] = {0,0,0};
  if(currentScore == 100) { score[0] = 1; }
  if(currentScore < 10) {score[2] = currentScore;}
  if((currentScore > 9) && (currentScore < 100)) {
    score[1] = currentScore / 10;
    score[2] = currentScore % 10;
  }
  arduboy.setTextSize(1);   //set the text size to use
  arduboy.setCursor(120,13);
  arduboy.print(score[0]);
  arduboy.setCursor(120,21);
  arduboy.print(score[1]);
  arduboy.setCursor(120,29);
  arduboy.print(score[2]);
}

void drawCountDown(void) {
  arduboy.setCursor(45,17);
  arduboy.setTextSize(4);
  arduboy.print((countdown / _FRAMERATE)+1);
  switch(countdown) {
    case (_FRAMERATE * _COUNTDOWN)-1: delay(125);
    case (_FRAMERATE * _COUNTDOWN)- _FRAMERATE:
    case _FRAMERATE:
    arduboy.tunes.tone(5000,20);
  }
  countdown--;
  
}

boolean activity(byte select) {
  drawActFrames();
  drawTimeLife();
  drawScore();
  if(countdown == 0)
  {
    if(timelife == 0) {timelife = _TIMELIFE; return true;}
    else timelife--; return false;
  }
  else
  {
    drawCountDown();
    return false;
  }
}

void setup() {
  // put your setup code here, to run once:
  delay(500);               //delay to account for hackduboy screen delay requirement
  arduboy.begin();          //show startup message
  arduboy.setFrameRate(_FRAMERATE); //set the frame rate
  arduboy.setTextSize(1);   //set the text size to use
  arduboy.tunes.tone(987, 128);   //play bootup tune
  delay(128);
  arduboy.tunes.tone(1318, 320);
  delay(500);
}

void loop() {
  while (true)
  {
    // put your main code here, to run repeatedly:
    if (!(arduboy.nextFrame()))     //wait until the screen needs to be drawn
      return;
    while (!start)
    {
      start = titlescreen();
    }

    // Now move to the activity selection screen.
    while (!menuselect)
    {
      menuselect = menuscreen();
    }

    // Now start the selected activity.
    if(activity(select)) {
      countdown = (_COUNTDOWN * _FRAMERATE)-1;   //frames used for countdown
      start = false;
      menuselect = false;
      gameselect = false;
      select = 0;
    }
    arduboy.display();
  }
}
