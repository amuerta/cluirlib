//
// 
// Command Line User Interface Rendering Library - CLUIR - LIB
//
//

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <asm-generic/ioctls.h>
#include <cstdint>
#include <cstdio>
#include <ios>


//#include <conio.h>

//
// Declaration for types and functions widly used in cluir
//
//


typedef uint fps;
typedef uint percent;
typedef uint pixel;
typedef uint color;
typedef std::string image_piece;
typedef std::string image;
template<typename T> struct vec2 { T x; T y; };

void gotoxy(int x, int y);

void cursorOnLineUp();             
void hideCursor();            
double parse_percents(percent number);


template<typename T>
struct 
Dynamic2DArray {
  std::vector<std::vector<T>> array;
  vec2<uint> size;
  T& at(uint x , uint y);
  void reserve(uint size_x , uint size_y);
  uint size_x();
  uint size_y();
  void clear();
};

#define EMPTY_PIXEL 0 
#define SEMI_FILLED 1
#define FILLED_PIXEL 2

#define JUNCTION_BORDER 20
#define TOP_BORDER_PIXEL 21
#define BOTTOM_BORDER_PIXEL 22
#define LEFT_BORDER_PIXEL 23 
#define RIGHT_BORDER_PIXEL 24

//
// Cluir structure initialization
//



namespace cluir {
 
  typedef vec2<uint> point;
  typedef vec2<uint> line;
  typedef vec2<uint> rectangle;
  

  struct Object {
    enum Type {
      Nothing,
    } type;
  };
  

  struct Block 
  { 
    enum BlockT { Text, Canvas, Picture , Grid , Graph , Panel , Menu , Void} BlockType;
    vec2<uint> position;
    vec2<uint> size;
    std::vector<Object> ObjectList;

    Block add_object(Object::Type);
  };


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

    vec2<uint> screen_size;
    std::vector<Block> BlockList;
    Dynamic2DArray<pixel> ValuesMap;
    Dynamic2DArray<color> ColorMap;

    private:vec2<uint> get_screen_size();
    public:Screen scale(uint scaling_factor_x, uint scaling_factor_y);
    public:Screen fill_solid();
    public:Screen draw_rect(vec2<uint> top_left_position, vec2<int> size);
    public:Screen draw_rect_percents(vec2<percent> top_left_position, vec2<percent> size);
    public:Screen draw_line(point begin, point end);
    public:Screen draw_circle(point center, uint radius);
  };

  struct Renderer {
    enum UpdateType { OnDemand, Dynamic, Static } UpdateType;
    Dynamic2DArray<image_piece> Frame;
    Dynamic2DArray<image> BuildFrame(Screen scr);
    void render(Dynamic2DArray<image_piece> frame);
    Renderer flush();
    Renderer hold_frame(uint seconds);
    image_piece convert_raw_pixel(uint pixel);
  };

  struct InputManager {};
  struct DataManager {};


  Screen NewScreen();
  Renderer NewRenderEngine();
  Block NewBlock(Block::BlockT type);
  Block FillBlock(std::vector<Object> elements);

  // end of namespace 
}


