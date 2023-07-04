#include "cluirlib.cpp"
#include <iostream>


int main ()
{


   auto drawer = cluir::NewRenderEngine();
   auto SCR = cluir::NewScreen();

   
   auto blk2 = cluir::NewBlock(cluir::Block::BlockT::Void);
   auto blk = cluir::NewBlock(cluir::Block::BlockT::Void);
   blk.Use_Border();
   SCR.add_blocks({ blk, blk2, blk })->block_alignment(cluir::BlockAlignment::Horizontal_Tiled);
   SCR.flush(); 
   //SCR.draw_rect({15,15}, {8,5}); 
   //SCR.set_drawing_pixel(FILLED_PIXEL)
   //   ->draw_line_percents({10,5}, {10,85},&SCR.ValuesMap);
  
      

   auto frame = drawer.BuildFrame(SCR);
   drawer.render(frame);
   drawer.hold_frame(2).finish();
}
