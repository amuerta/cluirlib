#include "cluirlib.h"
#include <string>
#include <type_traits>
#include <vector>





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


void debug(std::string msg) {std::cout << "\033[0;32m" << msg << "\033[0m\n";}
bool expression_with_error(std::string message) {std::cout<<"[!]>>"<<message<<"\n"; return false;}
void gotoxy(int x, int y) {printf("\033[%d;%dH", y, x);}
//printf("%c[%d;%df",0x1B,y,x)^^^^^^^^^^^^^^^^^; 
void cursorOnLineUp() { printf("\033[1A"); }
void hideCursor()  {std::cout << "\e[?25l";}
double parse_percents(percent number) { return (double)number / 100; }
uint RGB(size_t red, size_t green, size_t blue) 
{
  return red*1000*1000 + 1000*green + blue;
}

vec2<uint> get_real_from_percents(vec2<uint> number, vec2<percent> percents) 
{
  vec2<uint> real = { 
    (uint)(number.x * parse_percents(percents.x)), 
    (uint)(number.y * parse_percents(percents.y)) 
  };
  return real; 
}

//COLOR_RENDER
//echo -e "\e[48;2;125;132;243m AAAAAAa"

namespace cluir
{

//
//
// TOOL FUNCTIONS
//
//

  bool Screen::integrety_check(point cordinate) {
    return (cordinate.x < screen_size.x and cordinate.y < screen_size.y) ? true : expression_with_error("Out of bounds (x;y) -> " + std::to_string(cordinate.x) + ":" + std::to_string(cordinate.y)); 
  }
   

  Block NewBlock(Block::BlockT type) 
  {
    Block blk;
    blk.border_widht = 0;
    blk.last_object_position = {0,0};
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
    system("stty raw -echo"); // echo removes echoing of user input
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
  
  // color 

  Screen *Screen::SetColorBind(pixel type, color color_id) 
  {
    color_rules.push_back({
        type,
        color_id,
    });
    return this;
  }

  color Screen::get_color_from_bind(pixel type) 
  {
    for(uint i = 0; i < color_rules.size(); i++) {
      if(color_rules.at(i).type == type) return color_rules.at(i).c;
    }
    return COLORS::WHITE;
  }


  // alignment

  void Screen::horizontal_tiled_align() {
    for (uint block_counter = 0; block_counter < BlockList.size(); block_counter++) 
    {
      uint size_x = ((screen_size.x - screen_padding.x*2) / BlockList.size()) - 1;
      uint size_y = screen_size.y - (screen_padding.x*2);
      
      uint root_y = 0 + screen_padding.x;
      uint root_x = screen_padding.x + size_x * block_counter;

      BlockList.at(block_counter).size = {
        .x = size_x,
        .y = size_y
      };
      
      BlockList.at(block_counter).position = {
        .x = root_x,
        .y = root_y
      };
    } 
  }

  void Screen::tiled_align() {
    uint root_y=0;
    uint root_x=0;
    uint size_x=0;
    uint size_y=0;

    //if (BlockList.size() % 2) {
      
      for (uint block = 0; block < BlockList.size(); block++) 
      {
        size_x = screen_size.x / (BlockList.size() - 1) - 1;

        if (block % 2) size_y = screen_size.y / 2 - 1;
        else size_y = screen_size.y / 2 - 1;

        if (block % 2) root_y = size_y  - 1;
        else root_y = 0;

        if (block % 2) root_x = size_x * block;
        else root_x = root_x;



      BlockList.at(block).size = {
        .x = size_x,
        .y = size_y
      };
      
      BlockList.at(block).position = {
        .x = root_x,
        .y = root_y
      };

      }
    //}
  }

  
  // drawing 
 
  Screen *Screen::set_drawing_pixel(pixel type) 
  {
    pixel_to_draw = type;
    return  this;
  }


  Screen *Screen::draw_point(point p, pixel pix) 
  {
    ValuesMap.at(p.x, p.y) = pix;
    return this;
  }



  Screen *Screen::write_text(uint max_line_lenth,point origin,std::string text) 
  {

    if (max_line_lenth < text.size()) {
      uint line_counter = 0;
      uint char_counter = 0;
      for(uint lenth=0; lenth < text.size(); lenth++) { 
        if(char_counter > max_line_lenth) {
          ValuesMap.at(origin.x+char_counter, origin.y + line_counter) = text.at(lenth);
          line_counter++;
          char_counter=0;
        }
        else 
        {
          ValuesMap.at(origin.x+char_counter, origin.y + line_counter) = text.at(lenth);
          char_counter++;
        }
      }
    }
    else {

      for(uint lenth=0; lenth < text.size(); lenth++) 
      {
        ValuesMap.at(origin.x+lenth, origin.y) = text.at(lenth);
      }
    }
    return this;
  }





  Screen *Screen::draw_line(point begin, point end ) {

    int x = begin.x;
    int y = begin.y;
    int x2 = end.x;
    int y2 = end.y;

    int w = x2 - x ;
    int h = y2 - y ;
    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0 ;
   
    if (w<0) dx1 = -1 ; else if (w>0) dx1 = 1 ;
    if (h<0) dy1 = -1 ; else if (h>0) dy1 = 1 ;
    if (w<0) dx2 = -1 ; else if (w>0) dx2 = 1 ;
    
    int longest = abs(w) ;
    int shortest = abs(h) ;
    
    if (!(longest>shortest)) {
        longest = abs(h) ;
        shortest = abs(w) ;
        if (h<0) dy2 = -1 ; else if (h>0) dy2 = 1 ;
        dx2 = 0 ;            
    }
    
    int numerator = longest >> 1 ;
    
    for (int i=0;i<=longest;i++) {
      ValuesMap.at(x,y) = FILLED_PIXEL;  
        numerator += shortest ;
        if (!(numerator<longest)) {
            numerator -= longest ;
            x += dx1 ;
            y += dy1 ;
        } else {
            x += dx2 ;
            y += dy2 ;
        }
    }

    return this;
  }


  Screen *Screen::draw_circle(point center, uint radius, float x_multiplyer, bool filled) {

    static const double PI = 3.1415926535;
    double i, angle, x1, y1;

    if(!filled) {
      for(i = 0; i < 360; i += 1)
      {
        angle = i;
        x1 = radius * cos(angle * PI / 180) * x_multiplyer;
        y1 = radius * sin(angle * PI / 180);
        ValuesMap.at(center.x + (uint)x1, center.y + (uint)y1) = FILLED_PIXEL;
      }
    } else {
      for(uint u = 0; u<2; u++ ) {
        for(i = 0; i < 360; i += 0.1)
        {
          angle = i;
          x1 = (radius-u) * cos(angle * PI / 180) * x_multiplyer;
          y1 = (radius-u) * sin(angle * PI / 180);
          //ValuesMap.at(center.x + (uint)x1, center.y + (uint)y1) = FILLED_PIXEL;
          draw_line(center, 
              {
              center.x + (uint)x1,
              center.y + (uint)y1
              });
        }
      }
    }
    return  this;
  }

  

  Screen *Screen::draw_rect(point top_left_position, vec2<int> size, pixel line_types[6], bool filled) {
    const pixel horizontal_line = line_types[0];
    const pixel vertical_line = line_types[1];
    const pixel top_left_corner = line_types[2];
    const pixel top_right_corner = line_types[3];
    const pixel bottom_right_corner = line_types[4];
    const pixel bottom_left_corner = line_types[5];
    
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
    
    if (!filled) {
      for(uint horizontal=begin_x; horizontal < end_x; horizontal++) 
      {
        ValuesMap.at(horizontal, begin_y) = horizontal_line;
        ValuesMap.at(horizontal, end_y-1) = horizontal_line;
      }
      for(uint vertical=begin_y; vertical < end_y; vertical++) 
      {
        ValuesMap.at(begin_x, vertical) = vertical_line;
        ValuesMap.at(end_x-1, vertical) = vertical_line;
      }
    }
    else {
      for(uint vertical=begin_y; vertical < end_y; vertical++) 
      {
        for(uint horizontal=begin_x; horizontal < end_x; horizontal++) 
        {
          ValuesMap.at(horizontal, vertical) = FILLED_PIXEL;
        }
      }
    }

    ValuesMap.at(top_left_position.x, top_left_position.y) = top_left_corner;
    ValuesMap.at(top_left_position.x+size.x-1, top_left_position.y) = top_right_corner;
    ValuesMap.at(top_left_position.x+size.x-1, top_left_position.y+size.y-1) = bottom_right_corner;
    ValuesMap.at(top_left_position.x, top_left_position.y+size.y-1) = bottom_left_corner;

    return this;
  }

 

  Screen *Screen::draw_rect_percents(vec2<percent> top_left_position, vec2<percent> size) {
    
    vec2<uint> real_top_left_position = get_real_from_percents(screen_size, top_left_position);
    vec2<uint> real_size = get_real_from_percents(screen_size, size);
    
    for(uint horizontal=real_top_left_position.x; horizontal < real_top_left_position.x+real_size.x; horizontal++) 
    {
      ValuesMap.at(horizontal, real_top_left_position.y) = 2;
      ValuesMap.at(horizontal, real_top_left_position.y+real_size.y-1) = 2;
    }
    for(uint vertical=real_top_left_position.y; vertical < real_top_left_position.y+real_size.y; vertical++) 
    {
      ValuesMap.at(real_top_left_position.x, vertical) = 2;
      ValuesMap.at(real_top_left_position.x+real_size.x-1, vertical) = 2;
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

      case BlockAlignment::Tiled:
      {
        tiled_align();
      } break;
    }

    return this;
  }



  image_piece Renderer::convert_raw_color(color value) 
  {
    color rgb[3];
    rgb[0] = value / (1000*1000);
    rgb[1] = (value / 1000) % 1000;
    rgb[2] = value - value / 1000*1000; 
    return  "\e[38;2;" + std::to_string(rgb[0]) + ";" + std::to_string(rgb[1]) + ";" + std::to_string(rgb[2]) + "m";
  }

  std::string colorize(color value) 
  {
    Renderer dummy;
    return dummy.convert_raw_color(value);
  }
  
  image_piece Renderer::convert_raw_pixel(uint pixel) {
    switch (pixel) {
      case EMPTY_PIXEL:
        return " ";
        break;
      case SEMI_FILLED_PIXEL: 
        return "🮐";
      case FILLED_PIXEL:
        return "█";
      case HORIZ_BORDER_PIXEL:
        return "═";
      case VERT_BORDER_PIXEL:
        return "║";
      case TOPLEFT_BORDER_PIXEL:
        return "╔";
      case TOPRIGHT_BORDER_PIXEL:
        return "╗";
      case BOTRIGHT_BORDER_PIXEL:
        return "╝";
      case BOTLEFT_BORDER_PIXEL:
        return "╚";
      case 0 ... 255:  // ASCII 
        {
          std::string s(1,char(pixel));
          return s;
        }

      default: 
        return "E";
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
    ColorMap.reserve(output_size.x, output_size.y);

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

   Screen Screen::fill_empty() {
    for (int h = 0; h < screen_size.y; h++) {
      for (int w = 0; w < screen_size.x; w++) {
        ValuesMap.at(w, h) = EMPTY_PIXEL;
      }
    }
    return *this;
  }
   

   Screen *Screen::set_padding(uint pixels) 
   {
     screen_padding.x = pixels;
     screen_padding.y = pixels;
     return this;
   }
 
   Screen *Screen::set_margin(uint pixels) 
   {
     screen_margin.x = pixels;
     screen_margin.y = pixels;
     return this;
   }

   Screen *Screen::set_padding(vec2<uint>pixels)
   {

     screen_padding.x = pixels.x;
     screen_padding.y = pixels.y;
     return this;
   }


   Screen *Screen::set_margin(vec2<uint> pixels) 
   {
     screen_margin.x = pixels.x;
     screen_margin.y = pixels.y;
     return this;
   }


  Screen NewScreen() {
    Screen scr;
    scr.scale(100,100);
    scr.screen_margin = {0,0};
    scr.screen_padding = {0,0};
    scr.fill_empty();
    std::cout << "\nscreen created\n";
    std::cout << "scr.size.x = " << scr.screen_size.x << " | scr.size.y = " << scr.screen_size.y << " \n"; 
    return scr;
  }

//
//
// PROP METHODS
//
//

  Block *Block::setPadding(vec2<uint> pixels) 
  {
    block_padding.x = pixels.x;
    block_padding.y = pixels.y;
    return this;
  }
  

  Block *Block::setMargin(vec2<uint> pixels) 
  {
    block_margin.x = pixels.x;
    block_margin.y = pixels.y;
    return this;
  }
  

  Block *Block::Add_Text(uint paragraph_padding, uint text_padding,  std::string *text) 
  {
    Object o;
    o.type = o.Text;
    for(uint i=0; i<paragraph_padding; i++) {text->append(" ");}
    o.PtrData.at(0) = text;
    o.ObjectData.at(0).point_norm = { .x = position.x + 2 + text_padding, .y = position.y + 2 };
    MapBlock({o});
    return this;
  }

  

  Block *Block::Add_FilledRect(vec2<uint> root_pos, vec2<int> size) 
  {
    Object o;
    o.type = o.FilledRect;
    o.ObjectData.at(0).point_norm = root_pos;
    o.ObjectData.at(1).size_int = size;
    MapBlock({o});
    return this;
  }



  Block *Block::UseFancyBorder() {
    Object border;
    border_widht = 1;
    border.type = border.FancyBorder;
    MapBlock({border});
    return this;
  }

  Block *Block::UseSolidBorder() {
    Object border;
    border_widht = 1;
    border.type = border.Border;
    MapBlock({border});
    return this;
  }


  Block *Block::Add_Title(std::string label) {
    Object title;
    title.type = title.Title;
    title.ExceptionalObjectData.at(0) =  label ;
    for(size_t i = 0; i < ObjectList.size(); i++ ) 
    {
      if (ObjectList.at(i).type == Object::Type::FancyBorder) {
        title.ExceptionalObjectData.at(0) = " " + label + " ";
        title.ObjectData.at(1).checker = true;
        break; 
      }
    }
    std::cout << "title obj = " << title.ExceptionalObjectData.at(0) << "\n";
    MapBlock({title});
    return this;
  }


  Block *Block::CreateLinkedList(std::vector<std::string*> ptr_list, size_t spacing) {
   Object l;
    l.type = l.LinkedList;
    l.ObjectData.at(1).size_single = spacing;
    l.PtrData.resize(ptr_list.size());
    for (size_t lines = 0; lines < ptr_list.size(); lines++) 
    {
      l.PtrData.at(lines) = ptr_list.at(lines);
    }
    MapBlock({l});
    return this;
  }
 
  Block *Block::CreateList(std::vector<std::string> list, size_t spacing_multiplier) {
    Object l;
    l.type = l.List;
    l.ObjectData.at(1).size_single = spacing_multiplier;
    l.ExceptionalObjectData.resize(list.size()+1);
    for (size_t lines = 0; lines < list.size(); lines++) 
    {
      l.ExceptionalObjectData.at(lines) = list.at(lines);
    }

    MapBlock({l});
    return this;
  }

//
//
// BLOCKS 
//
//



  void Block::MapBlock(std::vector<Object> elements)
  {
    for (uint i = 0; i < elements.size(); i++) 
    {
      ObjectList.push_back(elements.at(i)); 
    }
  }

// EXAMPLE IF I EVER WANT TO GO BACK FOR THE OLD DESIGN
// I DONT.
  // void Block::border_solid(Object *target) 
  // {
  //   auto& point1 = target->ObjectData.at(0).point_norm;
  //   auto& size1 = target->ObjectData.at(1).size_int;
  //   point1.x = position.x+DEAFULT_SCREEN_PADDING;
  //   point1.y = position.y+DEAFULT_SCREEN_PADDING;
  //   size1.x = size.x;
  //   size1.y = size.y-1;
  // }



  void Screen::handler(Object obj, Block blok) {

    auto& chk = obj.ObjectData.at(1);
    auto& data = obj.ObjectData;
    auto& value1 = obj.ObjectData.at(0);
    auto& value2 = obj.ObjectData.at(1);
    auto& except_value = obj.ExceptionalObjectData;
    auto& except_value1 = obj.ExceptionalObjectData.at(0);
    auto& ptr_value = obj.PtrData;
    pixel FILLER[6] = {FILLED_PIXEL,FILLED_PIXEL,FILLED_PIXEL,FILLED_PIXEL,FILLED_PIXEL,FILLED_PIXEL};
    pixel FANCYBORDER[6] = {HORIZ_BORDER_PIXEL,VERT_BORDER_PIXEL,TOPLEFT_BORDER_PIXEL,TOPRIGHT_BORDER_PIXEL,BOTRIGHT_BORDER_PIXEL, BOTLEFT_BORDER_PIXEL} ;
  
    switch (obj.type) {
      
      case Object::Type::Point:
        ValuesMap.at(value1.point_norm.x, value1.point_norm.y);
        break;
      
      case Object::Type::Line:
        draw_line(value1.point_norm, value2.point_norm );
        break;
      
      case Object::Type::Rect:
        draw_rect(value1.point_norm,value2.size_int, FILLER, false);
        break;
      
      case Object::Type::FilledRect:
        draw_rect(value1.point_norm,value2.size_int, FILLER, true);
      
      case Object::Type::Circle:
        draw_circle(value1.point_norm, value2.single_int, 2 , false);
        break;
      
      case Object::Type::Text:
        write_text(blok.size.x - blok.block_padding.x,value1.point_norm, *ptr_value.at(0));
        break;
      
      case Object::Type::Border:
        {
          //blok.border_solid(&obj);
          //  its much more simpler to write a huge tower of swicth->case
          //  rather than do dozsens of functions that overcomplicate a 
          //  regular state machine like this one.
          auto& point1 = obj.ObjectData.at(0).point_norm;
          auto& size1 = obj.ObjectData.at(1).size_int;
          point1.x = blok.position.x+DEAFULT_SCREEN_PADDING;
          point1.y = blok.position.y+DEAFULT_SCREEN_PADDING;
          size1.x = blok.size.x;
          size1.y = blok.size.y-1;
          draw_rect(value1.point_norm,value2.size_int,FILLER,false);
          break;
        }

      case Object::Type::Title:
        {
          auto& point1 = obj.ObjectData.at(0).point_norm;
          auto& chk = obj.ObjectData.at(1).checker;
          auto& str1 = obj.ExceptionalObjectData.at(0);

          point1.x = blok.position.x + (blok.size.x - str1.size())/2;
          point1.y = blok.position.y+DEAFULT_SCREEN_PADDING;

          std::cout << "* title pos (" << point1.x << ":" << point1.y << ")\n";
          write_text(blok.size.x-1,value1.point_norm, except_value1);
          if (obj.ObjectData.at(1).checker == true) {
            draw_point(value1.point_norm, FILLED_PIXEL);
            draw_point({value1.point_norm.x+(uint)except_value1.size()-1, value1.point_norm.y}, FILLED_PIXEL);
          }
          break;
        }
      
      case Object::Type::List:
        {
          auto& point1 = obj.ObjectData.at(0).point_norm;
          auto& spacing = obj.ObjectData.at(1).size_single;
          point1.x = blok.position.x + blok.border_widht + DEAFULT_SCREEN_PADDING;
          point1.y = blok.position.y + blok.border_widht + DEAFULT_SCREEN_PADDING;
          uint cordinate = point1.y + blok.last_object_position.y;
          uint padding = 0;
          std::cout << "( point ) : " << value1.point_norm.x << " : " << value1.point_norm.y << "\n"; 
          for(uint l=0; l < except_value.size(); l++) {
            padding = ((float)except_value.at(l).size() / (blok.size.x - DEAFULT_SCREEN_PADDING*4));
            std::cout << "(padding) : " << padding << "\n"; 
            {
              point constructed_point = {
                point1.x + blok.block_padding.x, 
                //cordinate = cordinate + round((float)l/2) + padding
                // scizophrenia be like
                cordinate + blok.block_padding.y + l 
              };
              write_text(blok.size.x-blok.block_padding.x*2,constructed_point, except_value.at(l));
              cordinate = cordinate + padding+ (uint)spacing; 
              blok.last_object_position.y = cordinate; 
            }
          }

          break;
        }


      case Object::Type::LinkedList:
        {
          auto& point1 = obj.ObjectData.at(0).point_norm;
          auto& spacing = obj.ObjectData.at(1).size_single;
          point1.x = blok.position.x + blok.border_widht + DEAFULT_SCREEN_PADDING;//blok.block_padding.x;
          point1.y = blok.position.y + blok.border_widht + DEAFULT_SCREEN_PADDING; // blok.block_padding.y;
          uint cordinate = point1.y + blok.last_object_position.y;
          uint padding = 0;
          std::cout << "( point ) : " << value1.point_norm.x << " : " << value1.point_norm.y << "\n"; 
          for(uint l=0; l < ptr_value.size(); l++) {
            std::string& siz = *ptr_value.at(l);
            std::cout << "size * ->" << siz.size() << "\n";
            padding = ((float) siz.size() / (blok.size.x - DEAFULT_SCREEN_PADDING*4));
            std::cout << "(padding) : " << padding << "\n"; 
            {
              point constructed_point = {
                point1.x + blok.block_padding.x, 
                cordinate + blok.block_padding.y + l 
              };
              write_text(blok.size.x-blok.block_padding.x*4,constructed_point, siz );
              cordinate = cordinate + padding+ (uint)spacing; 
              blok.last_object_position.y = cordinate;
            }
          }

          break;
        }


      case Object::Type::FancyBorder:
        { 
          auto& point1 = obj.ObjectData.at(0).point_norm;
          auto& size1 = obj.ObjectData.at(1).size_int;
          point1.x = blok.position.x+DEAFULT_SCREEN_PADDING;
          point1.y = blok.position.y+DEAFULT_SCREEN_PADDING;
          size1.x = blok.size.x;
          size1.y = blok.size.y-1;

          draw_rect(value1.point_norm,value2.size_int, FANCYBORDER, false);
          break;
        }
      case Object::Type::Nothing:
        break;
    }
  }

  void Screen::flush_colors() 
  {
    std::cout << "flush_color called\n";
    for(uint h = 0 ; h < screen_size.y; h++) {
      for(uint w = 0; w < screen_size.x; w++) {
        ColorMap.at(w, h) = get_color_from_bind(ValuesMap.at(w, h));
      }
    }
  }

  void Screen::flush()
  {
    std::cout << "\n flush called \n";
    auto target = ValuesMap;
   
    for (uint blocks = 0; blocks < BlockList.size(); blocks++) {
      auto List = BlockList.at(blocks).ObjectList;
      auto& blok = BlockList.at(blocks); 
      
      std::cout << "handler called\n";
      for (uint i = 0; i < List.size(); i++) 
      {
        auto obj = List.at(i);
        handler(obj, blok);
      }
    }

    flush_colors();
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
        if(x_pos == scr.screen_size.x-DEAFULT_SCREEN_PADDING) 
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
          image_piece str_color = convert_raw_color(scr.ColorMap.at(x_pos, y_pos));
          Image.at(x_pos, y_pos) = str_color + str_pixel;
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


  InputManager *InputManager::SetKeys(key_binds k) {
    Keys = k;
    return this;
  }

  event InputManager::generate_event_from_key(char key) 
  {
    // bypassing switch const limitation

    if(key == Keys.Back) return event::Back;
    else if(key == Keys.Close) return event::Close;
    else if(key == Keys.Press) return event::Press;
    else if(key == Keys.ScrollDown) return event::ScrollDown;
    else if(key == Keys.ScrollUp) return event::ScrollUp;
    else if(key == Keys.ScrollLeft) return event::ScrollLeft;
    else if(key == Keys.ScrollRight) return event::ScrollRight;
    else if(key == Keys.Terminate) return event::Terminate;
    else if(key == Keys.Nothing) return event::Nothing;
    else return event::Nothing;
  }

  event InputManager::readUserInput ()
  {
    return generate_event_from_key(readkey());
  }

  void InputManager::WaitForAnyEvent() {
    while (true) {
      char input = readkey();
      if (input) {
        auto dummy = NewRenderEngine();
        dummy.finish();
        break;
      } 
    }
  }


  InputManager NewInputManager() {
    InputManager im = {};
    im.SetKeys({
        .Back = KEY_ESCAPE,
        .Close = 'q',
        .Press = '\n',
        .ScrollUp = 'w',
        .ScrollDown = 's',
        .ScrollRight = 'd',
        .ScrollLeft = 'a',
        .Terminate = 'z',
        .Nothing = (char)0
        });
    return  im;
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
