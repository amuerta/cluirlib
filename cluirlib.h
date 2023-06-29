//
// 
// Command Line User Interface Rendering Library - CLUIR - LIB
//
//

#include <iostream>
#include <cmath>
#include <string>
#include <vector>


//
// Declaration for types and functions widly used in cluir
//
//
template<typename T>
struct 
Dynamic2DArray { 
  std::vector<std::vector<T>> array;
  T at(uint x , uint y);
  uint size_x();
  uint size_y();
  void clear();
};

typedef uint pixel;
typedef uint color;
typedef std::string image_piece;
template<typename T> struct vec2 { T x; T y; };



//
// Cluir structure initialization
//

namespace cluir {
  
  struct Rules {
    struct pixel_type {
      enum type {
        Regular,
        HorizontalBorder,
        VerticalBorder,
        TopLeftCornerBorder,
        TopRightCornerBorder,
        BottomLeftCornerBorder,
        BottomRightCornerBorder,
      } type;
      char32_t pixel;
    };
    std::vector<pixel_type> global_pixel_types;
    
  };

  struct Screen {
    
    struct Block 
    { 
      struct Object {};
      enum BlockT { Text, Canvas, Picture , Grid , Graph , Panel , Menu } BlockType;
      vec2<uint> position;
      vec2<uint> size;
    };

    vec2<uint> screen_size;
    
    std::vector<Block> Blocks;
    Dynamic2DArray<pixel> ValuesMap;
    Dynamic2DArray<color> ColorMap;
    
    void get_screen_size();
  };

  struct Renderer {
    enum UpdateType { OnDemand, Dynamic, Static } UpdateType;
    Dynamic2DArray<image_piece> Frame;
    Dynamic2DArray<image_piece> BuildFrame(Screen scr);
    void render(Dynamic2DArray<image_piece> frame);
    void flush();
    void quit();
  };

  struct InputManager {};
  struct DataManager {};


  Screen NewScreen();
  Screen::Block NewBlock(Screen::Block::BlockT type);
  Screen::Block FillBlock(std::vector<Screen::Block::Object> elements);
 

  // end of namespace 
}


