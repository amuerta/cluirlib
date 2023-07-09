#include "cluirlib.cpp"

int main ()
{

   auto drawer = cluir::NewRenderEngine();
   auto SCR = cluir::NewScreen();


   auto blk2 = cluir::NewBlock(cluir::Block::BlockT::Void);
   auto blk = cluir::NewBlock(cluir::Block::BlockT::Void);

   // uint color = 255'255'204;
   //
   // SCR.SetColorBind(HORIZ_BORDER_PIXEL,    color )
   //    ->SetColorBind(VERT_BORDER_PIXEL,    color )
   //    ->SetColorBind(TOPRIGHT_BORDER_PIXEL,color )
   //    ->SetColorBind(TOPLEFT_BORDER_PIXEL, color )
   //    ->SetColorBind(BOTLEFT_BORDER_PIXEL, color )
   //    ->SetColorBind(BOTRIGHT_BORDER_PIXEL,color )
   //    ->SetColorBind(FILLED_PIXEL,color )
   // ;
   //
   blk2.UseFancyBorder();


   std::string text = "AAAAAA";
   std::string text2 = "BBBBBbAAAAAA";

   blk.UseFancyBorder()
      ->Add_Title(" Neat ")
      // ->CreateList({
      //       "* I am a magician of text ui heheh",
      //       "* Render text to a screen in list manner",
      //       "TODO: literally anything else",
      //       "[?] cool?", 
      //       "[!] yes."}
      //       ,1)
      ->CreateLinkedList({
            &text,
            &text2,
            }, 1);
      ;
      
   SCR.add_blocks({ blk, blk2, blk, })->block_alignment(cluir::BlockAlignment::Horizontal_Tiled);
   text = "aa";

   SCR.flush(); 
   // SCR.flush_colors();   
   
   auto keymanager = cluir::NewInputManager();
   //keymanager.SetKeys({});

   auto frame = drawer.BuildFrame(SCR);
   drawer.render(frame);
   keymanager.WaitForAnyEvent();
 
   // if(keymanager.readUserInput()==cluir::event::Terminate)
      // std::cout << "term";
   // drawer.finish();

}
