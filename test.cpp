#include "cluirlib.cpp"
#include <iostream>
#include <string>


int main ()
{


   auto drawer = cluir::NewRenderEngine();
   auto SCR = cluir::NewScreen();


   auto blk2 = cluir::NewBlock(cluir::Block::BlockT::Void);
   auto blk = cluir::NewBlock(cluir::Block::BlockT::Void);
  
   blk.UseFancyBorder()
      ->Add_Title(" Neat ")
      ->CreateList({
            " * Now i can do vodoo magic",
            " * Render text to a screen in list manner ",
            " TODO: literally anything else"," [?] cool ?", 
            " [!] yes."}
            ,2);

   SCR.add_blocks({ blk, blk2, blk2 })->block_alignment(cluir::BlockAlignment::Horizontal_Tiled);
   SCR.flush(); 



   auto frame = drawer.BuildFrame(SCR);
   drawer.render(frame);
   drawer.hold_frame(2).finish();
}
