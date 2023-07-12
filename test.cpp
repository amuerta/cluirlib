#include "cluirlib.cpp"

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
   

   
    std::string text =  "   About CLUIRLIB";
    std::string text2 = "Cluir is abarviation for Command Line User Interface Rendering LIBrary, this is my take on learning how to build more complex things like tui libraries and stuff made using them";
    std::string text3 = "This thing will be able to everything that you would expect from tui library (Menus, Grid, On Canvas Drawing, Text, and many more), for now its under development, just like my soft brain. ";
    std::string text4 = "If you want to start using it, you can clone the repo and just copy cluirlib.h and cluirlib.cpp files to your project, include only .cpp file.";
   

    blk
       .UseFancyBorder()
       ->setPadding({2,2})
       //->Add_FilledRect({4,4},{10,3})
       ->Add_Title(" CLUIR TEXT-ONLY DEMO ")
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
   //    // ->CreateLinkedList({
   //    //       &text,
   //    //       &text,
   //    //       &text,
   //    //       }, 
   //    //       1)
   //    //->Add_Text(0, 0, &text2);
       ;
   //    
   SCR.add_blocks({ blk, blk2, blk2, })->block_alignment(cluir::BlockAlignment::Horizontal_Tiled);
   // 
   SCR.set_drawing_pixel(FILLED_PIXEL);
   //SCR.draw_line({0,15}, {5, 5}); 
   SCR.draw_circle({150,25}, 8, 2, false);
   SCR.draw_circle({110,30}, 8, 2, false);
   SCR.draw_line({138,25}, {122,27});
   SCR.draw_line({164,25}, {140,5});
   SCR.draw_line({110-16,30}, {70,10});

   SCR.flush(); 
   // SCR.flush_colors();   
   
   auto keymanager = cluir::NewInputManager();
   //keymanager.SetKeys({});

   auto frame = drawer.BuildFrame(SCR);
   drawer.render(frame);
   keymanager.WaitForAnyEvent();
   std::cout << cluir::colorize(color) + "aaaaaaaaaaaa";



    
      // std::cout << "term";
    drawer.finish();

}
