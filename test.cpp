#include "cluirlib.cpp"
#include <iostream>

int main ()
{


  
   auto SCR = cluir::NewScreen();
   SCR.draw_rect_percents({25,25}, {50,50}); 
   auto drawer = cluir::NewRenderEngine();
   auto frame = drawer.BuildFrame(SCR);
   
   
   drawer.render(frame);
   drawer.hold_frame(2).flush();
 }
