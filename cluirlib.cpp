#include "cluirlib.h"
#include <vector>

//
//
// TODO:
//    * make screen edges more pleasant and give user a choice 
//    how to handle them
//      - C.M. Current solution is about creating a border out 
//      of 1 char to each side of terminal window
//
//



template<typename T>
uint Dynamic2DArray<T>::size_x() {
  return size.x; 
}
template<typename T>
uint Dynamic2DArray<T>::size_y() {
  return size.y;
}

template<typename  T>
T& Dynamic2DArray<T>::at(uint x, uint y) 
{
  return array.at(y).at(x);
}

template<typename  T>
void Dynamic2DArray<T>::reserve(uint size_x , uint size_y) {
  size.x = size_x;
  size.y = size_y;
  array.resize(size_y);
  for (uint rows = 0; rows < size_y; rows++) 
  {
    array.at(rows).resize(size_x);
  }
} 


bool expression_with_error(std::string message) {std::cout<<"[!]>>"<<message<<"\n"; return false;}
void gotoxy(int x, int y) {printf("\033[%d;%dH", y, x);}
//printf("%c[%d;%df",0x1B,y,x)^^^^^^^^^^^^^^^^^; 
void cursorOnLineUp() { printf("\033[1A"); }
void hideCursor()  {std::cout << "\e[?25l";}
double parse_percents(percent number) { return (double)number / 100; }

vec2<uint> get_real_from_percents(vec2<uint> number, vec2<percent> percents) 
{
  vec2<uint> real = { 
    (uint)(number.x * parse_percents(percents.x)), 
    (uint)(number.y * parse_percents(percents.y)) 
  };
  return real; 
}


namespace cluir
{

//
//
// TOOL FUNCTIONS
//
//

  bool Screen::integrety_check(point cordinate) {
    return (cordinate.x < screen_size.x and cordinate.y < screen_size.y) ? true : expression_with_error("Out of bounds"); 
  }
   

  Block NewBlock(Block::BlockT type) 
  {
    Block blk;
    blk.BlockType = type;
    return blk;
  }
  
  vec2<uint> Screen::get_screen_size() 
  {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    uint y = w.ws_row;
    uint x = w.ws_col;
    vec2<uint> SIZE = { x , y };
    return SIZE;
  }


  char InputManager::readkey() 
  {
    system("stty raw");
    char ch = getchar(); 
    // terminate when "." is pressed
    system("stty cooked");
    return ch;
  }


//
//
// SCREEN FUNCTIONS
//
//

  void Screen::horizontal_tiled_align() {
    for (uint block_counter = 0; block_counter < BlockList.size(); block_counter++) 
    {
      uint size_x = screen_size.x / BlockList.size() - 1;  
      uint size_y = screen_size.y;
      uint root_x = size_x * block_counter;

      BlockList.at(block_counter).size = {
        .x = size_x,
        .y = size_y
      };
      
      BlockList.at(block_counter).position = {
        .x = root_x,
        .y = 0
      };
    } 
  }

  Screen *Screen::set_drawing_pixel(pixel type) 
  {
    pixel_to_draw = type;
    std::cout << pixel_to_draw << " <- pixel \n";
    return  this;
  }

  Screen *Screen::draw_line(point begin, point end, canvas *target ) {
    std::cout << "draw call  : draw_pixel -> " << pixel_to_draw << "\n";
    int difference_x = (end.x - begin.x);
    int difference_y = (end.y - begin.y);
    int sign_x = (begin.x < end.x) ? 1 : -1;
    int sign_y = (begin.y < end.y) ? 1 : -1;
    int change_bias = difference_x - difference_y;

    if(integrety_check({end.x,end.y})) 
    {
      target->at(end.x, end.y) = pixel_to_draw;
    }
    while(begin.x != end.x || begin.y != end.y) 
    {
      if(integrety_check({begin.x, begin.y})) 
      {
        target->at(begin.x, begin.y) = pixel_to_draw;
      }
      int bias_after = change_bias * 2;
      if (bias_after > -difference_y) 
      {
        change_bias -= difference_y;
        begin.x += sign_x;
      }
      if (bias_after < difference_x) 
      {
        change_bias += difference_x;
        begin.y += sign_y;
      }
    } 
    return this;
  }

  
  Screen *Screen::draw_line_percents(vec2<percent> begin, vec2<percent> end, canvas *target ) {
    std::cout << "draw call  : draw_pixel -> " << pixel_to_draw << "\n";
    point real_begin = get_real_from_percents( get_screen_size() ,  begin);
    point real_end = get_real_from_percents( get_screen_size() ,  end);

    int difference_x = (real_end.x - real_begin.x);
    int difference_y = (real_end.y - real_begin.y);
    int sign_x = (real_begin.x < real_end.x) ? 1 : -1;
    int sign_y = (real_begin.y < real_end.y) ? 1 : -1;
    int change_bias = difference_x - difference_y;

    if(integrety_check({real_end.x,real_end.y})) {
      target->at(real_end.x, real_end.y) = pixel_to_draw;
    }
    while(real_begin.x != real_end.x || real_begin.y != real_end.y) 
    {
      if(integrety_check({real_end.x,real_end.y})) {
        target->at(real_begin.x, real_begin.y) = pixel_to_draw;
      }
      int bias_after = change_bias * 2;
      if (bias_after > -difference_y) 
      {
        change_bias -= difference_y;
        real_begin.x += sign_x;
      }
      if (bias_after < difference_x) 
      {
        change_bias += difference_x;
        real_begin.y += sign_y;
      }
    } 
    return this;
  }

  /*
  Screen Screen::draw_line(point begin, point end) {

    point line_begining = (begin.x < end.x) ? begin : end;
    point line_ending = (begin.x < end.x) ? end : begin;
    uint lenth = end.x - begin.x;
    uint height = end.y - begin.y;
    double slope = (double)height/(double)lenth;
    std::cout << "slope ==> "<< slope << "\n";
    for(uint x = line_begining.x; x < line_ending.x; x++) 
    { 
      if(slope > 1 ) {
        for (uint y = round(slope*x); y < round(slope*(x+1)); y++) {
          ValuesMap
            .at( x , y ) = FILLED_PIXEL;
        }
      }
      else {
        ValuesMap
          .at( x , round((slope)*x) ) = FILLED_PIXEL;
      }
      std::cout << "x : " << x << "  y : " << round((slope * x)) << "\n";
    }
    return *this;
  }
*/

  Screen *Screen::draw_circle(point center, uint radius, canvas *target ) {
    rectangle render_area = {
      .x = radius * 2,
      .y = radius * 2
    };

    point area_root = {
      .x = center.x - radius,
      .y = center.y - radius
    };

    for (uint ypos = area_root.y; ypos < render_area.y + area_root.y; ypos++) 
    { for (uint xpos = area_root.x; xpos < render_area.x + area_root.x; xpos++) 
      { if (std::pow(radius, 2) < ( std::pow(xpos,2) + std::pow(ypos,2)) ) {
           target->at(xpos,ypos) = FILLED_PIXEL;
         }
      }
    }
    return  this;
  }

  Screen *Screen::draw_rect(point top_left_position, vec2<int> size, canvas *target ) {
    uint
      end_x, begin_x,
      end_y, begin_y
    ;
    //
    // FOR X DIMENSION
    //
    if (top_left_position.x + size.x >= top_left_position.x) 
    {
      begin_x = top_left_position.x;
      end_x = top_left_position.x + size.x;
    }
    else {
      begin_x = top_left_position.x + size.x;
      end_x = top_left_position.x;
    }
    //
    // FOR Y DIMENSION
    //
    if (top_left_position.y + size.y >= top_left_position.y) 
    {
      begin_y = top_left_position.y;
      end_y = top_left_position.y + size.y;
    }
    else {
      begin_y = top_left_position.y + size.y;
      end_y = top_left_position.y;
    }
    for(uint horizontal=begin_x; horizontal < end_x; horizontal++) 
    {
      target->at(horizontal, begin_y) = 2;
      target->at(horizontal, end_y-1) = 2;
    }
    for(uint vertical=begin_y; vertical < end_y; vertical++) 
    {
      target->at(begin_x, vertical) = 2;
      target->at(end_x-1, vertical) = 2;
    }
    return this;
  }



  Screen *Screen::draw_rect_percents(vec2<percent> top_left_position, vec2<percent> size, canvas *target) {
    
    vec2<uint> real_top_left_position = get_real_from_percents(screen_size, top_left_position);
    vec2<uint> real_size = get_real_from_percents(screen_size, size);
    
    for(uint horizontal=real_top_left_position.x; horizontal < real_top_left_position.x+real_size.x; horizontal++) 
    {
      target->at(horizontal, real_top_left_position.y) = 2;
      target->at(horizontal, real_top_left_position.y+real_size.y-1) = 2;
    }
    for(uint vertical=real_top_left_position.y; vertical < real_top_left_position.y+real_size.y; vertical++) 
    {
      target->at(real_top_left_position.x, vertical) = 2;
      target->at(real_top_left_position.x+real_size.x-1, vertical) = 2;
    }
    return this;
  }

  Screen *Screen::add_blocks(std::vector<Block> blk) {
    for(uint i = 0; i < blk.size(); i++) 
    {
      BlockList.push_back(blk.at(i));
    }
    return this;
  }

  Screen *Screen::block_alignment(BlockAlignment Type) {
    switch (Type) {
     
      case BlockAlignment::Horizontal_Tiled: 
      {
        horizontal_tiled_align();
      } break;
    }

    return this;
  }

 

  image_piece Renderer::convert_raw_pixel(uint pixel) {
    switch (pixel) {
      case 0:
        return " ";
        break;
      case 1: 
        return "🮐";
      case 2:
        return "█";
      default: 
        return "!";
        break;
    }
  }

  Screen Screen::scale(percent scaling_factor_x, percent scaling_factor_y) {
    vec2<uint> actual_size = get_screen_size();
    vec2<uint> output_size;
    
    double double_scaling_factor_x = ((double)scaling_factor_x / 100);
    double double_scaling_factor_y = ((double)scaling_factor_y / 100);

    output_size.x = actual_size.x * double_scaling_factor_x;
    output_size.y = actual_size.y * double_scaling_factor_y;
    
    ValuesMap.reserve(output_size.x, output_size.y);

    screen_size.x = output_size.x;
    screen_size.y = output_size.y-1;
    
    return *this;
  }

  Screen Screen::fill_solid() {
    for (int h = 0; h < screen_size.y; h++) {
      for (int w = 0; w < screen_size.x; w++) {
        ValuesMap.at(w, h) = FILLED_PIXEL;
      }
    }
    return *this;
  }

  
  Screen NewScreen() {
    Screen scr;
    scr.scale(100,100); 
    std::cout << "\nscreen created\n";
    return scr;
  }

//
//
// PROP METHODS
//
//

  Block *Block::Use_Border() {
    Object border;
    border.type = border.Border;
    MapBlock({border});
    return this;
  }


//
//
// BLOCKS 
//
//



  void Block::MapBlock(std::vector<Object> elements) {
    for (uint i = 0; i < elements.size(); i++) 
    {
      ObjectList.push_back(elements.at(i)); 
    }
  }

  void Block::border(Object *target) 
  {
    auto& point1 = target->ObjectData.at(0).point_norm;
    auto& size1 = target->ObjectData.at(1).size_int;
    point1.x = position.x+1;
    point1.y = position.y+1;
    size1.x = size.x;
    size1.y = size.y-1;
  }

  void Screen::flush()
  {
    std::cout << "\n flush called \n";
    auto target = ValuesMap;

    for (uint blocks = 0; blocks < BlockList.size(); blocks++) {
      auto List = BlockList.at(blocks).ObjectList;
      auto& blok = BlockList.at(blocks); 
      for (uint i = 0; i < List.size(); i++) 
      {
        auto obj = List.at(i);
        auto& value1 = obj.ObjectData.at(0);
        auto& value2 = obj.ObjectData.at(1);
        Screen uslessScreen;
        uslessScreen.set_drawing_pixel(FILLED_PIXEL);
        

        switch (obj.type) {
          case Object::Type::Point:
            ValuesMap.at(value1.point_norm.x, value1.point_norm.y);
            break;
          case Object::Type::Line:
            uslessScreen.draw_line
              (value1.point_norm, value2.point_norm , &ValuesMap);
            break;
          case Object::Type::Rect:
            uslessScreen.draw_rect(value1.point_norm,value2.size_int, &ValuesMap);
            break;
          case Object::Type::Circle:
            uslessScreen.draw_circle(value1.point_norm, value2.single_int, &ValuesMap);
            break;
          case Object::Type::Text:
            /* to do */
            break;
          case Object::Type::Border:
            blok.border(&obj);
            uslessScreen.draw_rect(value1.point_norm,value2.size_int, &ValuesMap);
            break;
          case Object::Type::Nothing:
            break;
        }

      }
    }
  }




//
//
// OBJECT FUNCTIONS
//
//





  Dynamic2DArray<image> Renderer::BuildFrame(Screen scr) {
    Dynamic2DArray<image> Image;
    Image.reserve(scr.screen_size.x,  scr.screen_size.y);
    std::cout << "\nSCREEN SIZE : " << scr.screen_size.x << " " << scr.screen_size.y << "\n";
    for (uint y_pos=0; y_pos < scr.screen_size.y; y_pos++) {
      for (uint x_pos=0; x_pos < scr.screen_size.x; x_pos++) {
        if(x_pos == scr.screen_size.x-1) 
        {
          Image.at(x_pos, y_pos) = "\n";
        }
        else if (x_pos == 0) 
        {
          Image.at(x_pos, y_pos) = " ";
        }
        else if (y_pos == 0) 
        {
          Image.at(x_pos, y_pos) = " ";
        }
        else 
        {
          image_piece str_pixel = convert_raw_pixel(scr.ValuesMap.at(x_pos, y_pos));
          Image.at(x_pos, y_pos) = str_pixel;
        }
      }
    }
    return Image;
  }

  void Renderer::render(Dynamic2DArray<image_piece> frame) {
    std::cout << "Renderer:\n * frame size ->" << frame.size_x() << " - " << frame.size_y() << "\n";
    for(uint frame_height=0; frame_height < frame.size_y(); frame_height++) {
      for(uint frame_widht=0; frame_widht < frame.size_x(); frame_widht++) {
        //Todo:
        //  * add choice what type of border handlig user wants
        //  * make it look good
        //
        //if(frame_height == frame.size_y()-1 and frame_widht == frame.size_x()-2) //and frame.at(frame_widht, frame_height)=="\n")
        //{
        hideCursor();
        //std::cin.get();
        //} 
        //else
        //{
        std::cout <<frame.at(frame_widht, frame_height);
        //}
      }
    }
    std::cout << "\e[25m";
    
  

  }


  Renderer Renderer::finish() {
    printf("\e[?25h");
    return *this;
  }
  
  Renderer Renderer::hold_frame(uint seconds) {
    sleep(seconds);
    return  *this;
  }

  Renderer NewRenderEngine() {
    Renderer r;
    return r;
  }

}
