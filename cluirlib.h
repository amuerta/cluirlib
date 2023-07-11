//
// 
// Command Line User Interface Rendering Library - CLUIR - LIB
//
//

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <cstdio>
#include <ios>
#include <termios.h>

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
color RGB(size_t red, size_t green, size_t blue);

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


#define KEY_ESCAPE 27
#define KEY_SPACE 32



#define DEAFULT_OBJECT_BUFFER_SIZE 32
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


enum COLORS {
  WHITE = 255'255'255,
};

//
// Cluir structure initialization
//



namespace cluir {
 
  typedef vec2<uint> size;
  typedef vec2<uint> point;
  typedef vec2<uint> line;
  typedef vec2<uint> rectangle;
  typedef Dynamic2DArray<pixel> canvas; 

  typedef char key;
  

  enum event {
    Back,
    Close,
    Press,
    ScrollUp,
    ScrollDown,
    ScrollRight,
    ScrollLeft,
    Terminate,
    Nothing
  };

  struct action {
    char key;
    event evt;
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
      List,
      LinkedList,
      Nothing,
    } type;
   
   union variying_data {
      vec2<int> size_int;
      vec2<uint> size_uint;
      point point_norm;
      int single_int;
      uint single_uint;
      size_t size_single;
      char single_char;
      bool checker;
   };

   std::vector<variying_data> ObjectData;
   std::vector<std::string> ExceptionalObjectData;
   std::vector<std::string*> PtrData;
   // i really dont like std::variant<T> 
   Object() {
     ObjectData.resize(DEAFULT_OBJECT_BUFFER_SIZE);
     ExceptionalObjectData.resize(DEAFULT_OBJECT_BUFFER_SIZE*4);
     PtrData.resize(DEAFULT_OBJECT_BUFFER_SIZE*4);
   }
  };
 
  // struct Property {
  //   enum Type {
  //     borders,
  //   } type;
  // };

  enum BlockAlignment {
    Horizontal_Tiled,
    Tiled
  };

  struct Block 
  { 
    enum BlockT { Text, Canvas, Picture , Grid , Graph , Panel , Menu , Void} BlockType;
    
    point last_object_position;
    point position;
    vec2<percent> position_percents;
    
    vec2<uint> size;
    vec2<percent> size_percents;
 
    vec2<uint> block_padding;
    vec2<uint> block_margin;

    uint border_widht;

    std::vector<Object> ObjectList;

    private:void MapBlock(std::vector<Object> elements);
    
    public:void border_solid(Object *target);
    public:void title(std::string label, Object *target);
    public:void list(std::vector<std::string> elements, Object *target);
    
    // Scaling functions 
    public:Block *setPadding(vec2<uint> pixels);
    public:Block *setMargin(vec2<uint> pixels);

    public:Block *Add_Text(uint paragraph_padding, uint text_padding,  std::string *text );
    public:Block *CreateList(std::vector<std::string> list, size_t spacing);
    public:Block *CreateLinkedList(std::vector<std::string*> ptr_list, size_t spacing);
    public:Block *UseFancyBorder();
    public:Block *UseSolidBorder();
    public:Block *Add_Title(std::string label);

  };



  struct Screen {
    
    pixel pixel_to_draw;

    vec2<uint> screen_size;
    vec2<uint> screen_padding;
    vec2<uint> screen_margin;


    std::vector<Block> BlockList;
    Dynamic2DArray<pixel> ValuesMap;
    Dynamic2DArray<color> ColorMap;

    struct color_bind { pixel type; color c; };
    std::vector<color_bind> color_rules;
    Screen *SetColorBind(pixel type, color color_id);
    color get_color_from_bind(pixel type);
    
    private:bool integrety_check(point cordinate);
    private:vec2<uint> get_screen_size();
    private:void horizontal_tiled_align();
    private:void tiled_align();

    public:Screen scale(uint scaling_factor_x, uint scaling_factor_y);
    public:Screen *set_padding(vec2<uint> pixels);
    public:Screen *set_margin(vec2<uint> pixels);
    public:Screen fill_solid();
    public:Screen fill_empty();
    public:Screen *set_drawing_pixel(pixel type);
    public:
     //Useless: Screen *write_linkedtext(uint max_line_lenth, point origin,std::string *text);
     Screen *write_text(uint max_line_lenth,point origin,std::string text);
     Screen *draw_point(point p, pixel pix);
     Screen *draw_rect(vec2<uint> top_left_position, vec2<int> size, pixel line_types[6]);
     Screen *draw_rect_percents(vec2<percent> top_left_position, vec2<percent> size);
     Screen *draw_line(point begin, point end);
     Screen *draw_line_percents(vec2<percent> begin, vec2<percent> end);
     Screen *draw_circle(point center, uint radius);
    public:Screen *add_blocks(std::vector<Block> blk);
    public:Screen *block_alignment(BlockAlignment Type);
    // TODO: public:Screen *block_alignment(std::vector<std::vector<uint>> proportions);
    public:Screen *remove_block_byId(uint BlockId);
    public:void handler(Object obj, Block blk);
    public:void flush();
    public:void flush_colors();
  
    // OVERLOADS
    public:Screen *set_padding(uint pixels);
    public:Screen *set_margin(uint pixels);
  };

  struct Renderer {
    enum UpdateType { OnDemand, Dynamic, Static } UpdateType;
    Dynamic2DArray<image_piece> Frame;
    Dynamic2DArray<image> BuildFrame(Screen scr);
    void render(Dynamic2DArray<image_piece> frame);
    Renderer finish();
    Renderer hold_frame(uint seconds);
    image_piece convert_raw_color(color value);
    image_piece convert_raw_pixel(uint pixel);
  };

  struct InputManager {
    
    struct key_binds {
      char Back;
      char Close;
      char Press;
      char ScrollUp;
      char ScrollDown;
      char ScrollRight;
      char ScrollLeft;
      char Terminate;
      char Nothing;
      
    } Keys;

    InputManager *SetKeys(key_binds k);
    
    private:event generate_event_from_key(key k);
    private:char readkey();
    public:std::vector<event> EventJournal;
    event readUserInput();
    void StartInputServer();
    void WaitForAnyEvent();
  };

  struct DataManager {

  };
  
  InputManager NewInputManager();
  Screen NewScreen();
  Renderer NewRenderEngine();
  Block NewBlock(Block::BlockT type);


  // end of namespace 
}

