#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>

struct Map {
  Map(int w, int h) : width(w), height(h) {}
  int width, height;

  rapidjson::Document encode() {
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);

    writer.StartObject();
    writer.Key("width");
    writer.Int(width);
    writer.Key("height");
    writer.Int(height);
    writer.EndObject();

    rapidjson::Document d;
    d.Parse(s.GetString());

    return d;
  }
};
