#include "cluirlib.cpp"
#include <string>

int main ()
{

   auto drawer = cluir::NewRenderEngine();
   auto SCR = cluir::NewScreen();
   
   SCR.set_padding(0);
  
   auto blk2 = cluir::NewBlock(cluir::Block::BlockT::Void);
   auto blk = cluir::NewBlock(cluir::Block::BlockT::Void);

    uint color = 218'247'166;
   
    SCR.SetColorBind(HORIZ_BORDER_PIXEL,    color )
       ->SetColorBind(VERT_BORDER_PIXEL,    color )
       ->SetColorBind(TOPRIGHT_BORDER_PIXEL,color )
       ->SetColorBind(TOPLEFT_BORDER_PIXEL, color )
       ->SetColorBind(BOTLEFT_BORDER_PIXEL, color )
       ->SetColorBind(BOTRIGHT_BORDER_PIXEL,color )
       ->SetColorBind(FILLED_PIXEL,color )
    ;
   
   //blk2.UseFancyBorder();
   


   std::string text =  "   About Tea";
   std::string text2 = "Tea is an aromatic beverage prepared by pouring hot or boiling water over cured or fresh leaves of Camellia sinensis, an evergreen shrub native to East Asia which probably originated in the borderlands of southwestern China and northern Myanmar.";
   std::string text3 = "Tea is also made, but rarely, from the leaves of Camellia taliensis. After plain water, tea is the most widely consumed drink in the world.";
   std::string text4 = "There are many different types of tea; some have a cooling, slightly bitter, and astringent flavour, while others have profiles that include sweet, nutty, floral, or grassy notes. Tea has a stimulating effect in humans primarily due to its caffeine content.";

   blk
      .UseFancyBorder()
      ->setPadding({2,2})
      ->Add_Title(" CLUIR TEXT DEMO ")
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
            &text3,
            &text4
            }, 2)
      // ->CreateLinkedList({
      //       &text,
      //       &text,
      //       &text,
      //       }, 
      //       1)
      //->Add_Text(0, 0, &text2);
      ;
      
   SCR.add_blocks({ blk, blk2, blk2, })->block_alignment(cluir::BlockAlignment::Horizontal_Tiled);
   

   
   SCR.flush(); 
   // SCR.flush_colors();   
   
   auto keymanager = cluir::NewInputManager();
   //keymanager.SetKeys({});

   auto frame = drawer.BuildFrame(SCR);
   drawer.render(frame);
   keymanager.WaitForAnyEvent();
   std::cout << cluir::colorize(color) + "aaaaaaaaaaaa";
   // if(keymanager.readUserInput()==cluir::event::Terminate)
      // std::cout << "term";
   // drawer.finish();

}
