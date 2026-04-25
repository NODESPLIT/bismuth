#include "../../src/crystal/body.hpp"
#include <simple2d.h>

class Window {
  public:
    Reference update;
    Reference render;

    S2D_Window *window;

    Window(Table config);
    ~Window();
};

Window* Current = nullptr;

void Update() { Instance->call(Current->update, false); }
void Render() { Instance->call(Current->render, false); }

Window::Window(Table config) {
  string title = config["title"] ? config["title"]->as<String>() : "";
  int width = config["width"] ? config["width"]->as<Number>() : 640;
  int height = config["height"] ? config["height"]->as<Number>() : 480;

  if (config["update"]) update = config["update"];
  if (config["render"]) render = config["render"];

  window = S2D_CreateWindow(
    title.c_str(),
    width, height,
    config["update"] ? &Update : NULL,
    config["render"] ? &Render : NULL,
    0
  );

  Current = this;
  S2D_Show(window);
}

Window::~Window() {
  S2D_FreeWindow(window);
}

vector<float> Components(int color) {
  float red = ((color & 0x00FF0000) >> 16) / 255;
  float green = ((color & 0x0000FF00) >> 8) / 255;
  float blue = (color & 0x000000FF) / 255;
  return vector<float>{ red, green, blue };
}

class Draw final: public Bismuth::API {
  public:
    Reference construct(Runtime* instance) override {
      Instance = instance;
      Reference exports = Value::Empty(Type::Table);

      exports->SET("Window", Block::Bound([](Array arguments) {
        Window* window = new Window(arguments[0]->as<Table>());
        return Value::Empty();
      }));

      Reference draw = Value::Empty(Type::Table);
      draw->SET("Line", Block::Bound([](Array arguments) {
        Array from = arguments[0]->as<Array>();
        Array to = arguments[1]->as<Array>();

        Table config = arguments.size() >= 3 ? arguments[2]->as<Table>() : Table{};
        int thickness = config["thickness"] ? config["thickness"]->as<Number>() : 1;
        
        int color = config["color"] ? config["color"]->as<Number>() : 0xFFFFFF;
        vector<float> components = Components(color);

        S2D_DrawLine(
          from[0]->as<Number>(),
          from[1]->as<Number>(),
          to[0]->as<Number>(),
          to[1]->as<Number>(),
          thickness,
          components[0], components[1], components[2], 1.0,
          components[0], components[1], components[2], 1.0,
          components[0], components[1], components[2], 1.0,
          components[0], components[1], components[2], 1.0
        );

        return Value::Empty();
      }));

      draw->SET("Text", Block::Bound([](Array arguments) {
        Array position = arguments[0]->as<Array>();
        String font = arguments[1]->as<String>();
        String string = arguments[2]->as<String>();

        Table config = arguments.size() >= 4 ? arguments[3]->as<Table>() : Table{};
        int size = config["size"] ? config["size"]->as<Number>() : 20;
        int color = config["color"] ? config["color"]->as<Number>() : 0xFFFFFF;

        S2D_Text *text = S2D_CreateText(font.c_str(), string.c_str(), size);

        vector<float> components = Components(color);
        text->color.r = components[0];
        text->color.g = components[1];
        text->color.b = components[2];
        text->color.a = 1.0;

        int x = position[0] ? position[0]->as<Number>() : 0;
        int y = position[1] ? position[1]->as<Number>() : 0;

        text->x = x;
        text->y = y;

        S2D_DrawText(text);
        return Value::Empty();
      }));

      exports->SET("Draw", draw);
      return exports;
    }
};

extern "C" BOOST_SYMBOL_EXPORT Draw crystal;
Draw crystal;