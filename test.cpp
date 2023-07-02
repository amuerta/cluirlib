#include "cluirlib.cpp"
#include <iostream>

int main ()
{


  
   auto SCR = cluir::NewScreen();
   //SCR.draw_rect({15,15}, {8,5}); 
   SCR.draw_line({10,5}, {10,15});
   

   auto drawer = cluir::NewRenderEngine();
   auto frame = drawer.BuildFrame(SCR);
   
   
   drawer.render(frame);
   drawer.hold_frame(2).flush();
 }
