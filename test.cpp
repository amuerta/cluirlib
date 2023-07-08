#include "cluirlib.cpp"
#include <iostream>
#include <string>

int main ()
{

   auto drawer = cluir::NewRenderEngine();
   auto SCR = cluir::NewScreen();


   auto blk2 = cluir::NewBlock(cluir::Block::BlockT::Void);
   auto blk = cluir::NewBlock(cluir::Block::BlockT::Void);
 
   blk2.UseFancyBorder();

   blk.UseFancyBorder()
      ->Add_Title(" Neat ")
      ->CreateList({
            "* I am a magician of text ui heheh",
            "* Render text to a screen in list manner",
            "TODO: literally anything else",
            "[?] cool?", 
            "[!] yes."}
            ,0);

   SCR.add_blocks({ blk, blk2, blk, })->block_alignment(cluir::BlockAlignment::Tiled);
   SCR.flush(); 
   
   auto keymanager = cluir::NewInputManager();
   //keymanager.SetKeys({});

   auto frame = drawer.BuildFrame(SCR);
   drawer.render(frame);
   keymanager.WaitForEvent();
}
