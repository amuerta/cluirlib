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
#include <termios.h>
#include <variant>

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
bool expression_with_error(std::string message);
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


#define DEAFULT_OBJECT_BUFFER_SIZE 10
#define DEAFULT_SCREEN_PADDING 1

#define EMPTY_PIXEL 100'0 
#define SEMI_FILLED_PIXEL 100'1
#define FILLED_PIXEL 100'2

#define JUNCTION_BORDER 100'20
#define HORIZ_BORDER_PIXEL 100'21
#define VERT_BORDER_PIXEL 100'22 
#define TOPLEFT_BORDER_PIXEL 100'23
#define TOPRIGHT_BORDER_PIXEL 100'24
#define BOTRIGHT_BORDER_PIXEL 100'25
#define BOTLEFT_BORDER_PIXEL 100'26

//
// Cluir structure initialization
//



namespace cluir {
 
  typedef vec2<uint> point;
  typedef vec2<uint> line;
  typedef vec2<uint> rectangle;
  typedef Dynamic2DArray<pixel> canvas; 
 
  

  enum event {
    Close,
    Terminate,
    Nothing
  };

  struct action {
    char key;
    event job;
  };

  struct Object {
    enum Type {
      Point,
      Line,
      Rect,
      Circle,
      Text,
      Border,
      FancyBorder,
      Title,
      Nothing,
    } type;
   
   union variying_data {
      vec2<int> size_int;
      vec2<uint> size_uint;
      point point_norm;
      int single_int;
      char single_char;
   };
   std::vector<variying_data> ObjectData;
   std::vector<std::string> ExceptionalObjectData;
   // i really dont like std::variant<T> 
   Object() {
     ObjectData.resize(DEAFULT_OBJECT_BUFFER_SIZE);
     ExceptionalObjectData.resize(DEAFULT_OBJECT_BUFFER_SIZE);
   }
  };
 
  struct Property {
    enum Type {
      borders,
    } type;
  };

  enum BlockAlignment {
    Horizontal_Tiled,
  };

  struct Block 
  { 
    enum BlockT { Text, Canvas, Picture , Grid , Graph , Panel , Menu , Void} BlockType;
    
    point position;
    vec2<uint> size;
    vec2<percent> position_percents;
    vec2<percent> size_percents;
   
    std::vector<Object> ObjectList;

    private:void MapBlock(std::vector<Object> elements);
    public:void border_solid(Object *target);
    public:void title(std::string label, Object *target);
    public:Block *UseFancyBorder();
    public:Block *UseSolidBorder();
    public:Block *Add_Title(std::string label);

  };


  namespace Rules {
    struct pixelType {
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
    enum updateType {
      onInput,
      updatesPS,
      onEvent
    };
  };

  struct Screen {
    
    pixel pixel_to_draw;

    vec2<uint> screen_size;
    std::vector<Block> BlockList;
    Dynamic2DArray<pixel> ValuesMap;
    Dynamic2DArray<color> ColorMap;

    private:bool integrety_check(point cordinate);
    private:vec2<uint> get_screen_size();
    private:void horizontal_tiled_align();
    
    public:Screen scale(uint scaling_factor_x, uint scaling_factor_y);
    public:Screen fill_solid();
    public:Screen fill_empty();
    public:Screen *set_drawing_pixel(pixel type);
    public:
     Screen *write_text(point origin,std::string text);
     Screen *draw_rect(vec2<uint> top_left_position, vec2<int> size, pixel line_types[6]);
     Screen *draw_rect_percents(vec2<percent> top_left_position, vec2<percent> size);
     Screen *draw_line(point begin, point end);
     Screen *draw_line_percents(vec2<percent> begin, vec2<percent> end);
     Screen *draw_circle(point center, uint radius);
    public:Screen *add_blocks(std::vector<Block> blk);
    public:Screen *block_alignment(BlockAlignment Type);
    public:Screen *remove_block_byId(uint BlockId);
    public:void flush();
  };

  struct Renderer {
    enum UpdateType { OnDemand, Dynamic, Static } UpdateType;
    Dynamic2DArray<image_piece> Frame;
    Dynamic2DArray<image> BuildFrame(Screen scr);
    void render(Dynamic2DArray<image_piece> frame);
    Renderer finish();
    Renderer hold_frame(uint seconds);
    image_piece convert_raw_pixel(uint pixel);
  };

  struct InputManager {
    char readkey();
    std::vector<event> ActionList;
    event readUserInput();
    void StartInputServer();
  };
  struct DataManager {};


  Screen NewScreen();
  Renderer NewRenderEngine();
  Block NewBlock(Block::BlockT type);


  // end of namespace 
}

