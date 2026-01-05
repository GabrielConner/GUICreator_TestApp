#ifndef PPM_INCLUDE_PPACK_TEXT_RENDERING_H
#define PPM_INCLUDE_PPACK_TEXT_RENDERING_H

#include "pPack/vector.h"
#include "glad/glad.h"
#include <map>

namespace pPack {
namespace text_rendering {


struct CharacterMetrics {
  vector::IVector2 pixelBottomLeft;
  vector::IVector2 pixelTopRight;

  vector::Vector2 uvBottomLeft;
  vector::Vector2 uvTopRight;

  vector::IVector2 size;
  vector::IVector2 bearing;
  int advance;


  CharacterMetrics();
  CharacterMetrics(vector::IVector2 Size, vector::IVector2 Bearing, float Advance);
};



struct RenderTextInfo {
  float fontScale;
  float lineSize;
  float textWidth;

  bool centerX;
  bool centerY;

  bool startTop;

  vector::Vector2 startPos;
  vector::Vector4 color;

  vector::IVector2 frameSize;

  RenderTextInfo(vector::IVector2 FrameSize);
};




class Bitmap {
  unsigned int texture;
  vector::IVector2 textureSize;
  int fontSize;

  unsigned char charLow;
  unsigned char charHigh;

  friend Bitmap GenerateBitmap(const char* fontFile, int size, unsigned char low, unsigned char high, int filtering);
  friend void Render(Bitmap& bitmap, std::string text, RenderTextInfo info);

public:
  std::map<unsigned char, CharacterMetrics> characterMapping;

  void LoadFromFile(const char* file, int filtering);
  void SaveToFile(const char* file, const char* imageLocation);


  bool Valid();


  void Destroy();


  int GetFontSize();
  unsigned char GetCharLow();
  unsigned char GetCharHigh();



  CharacterMetrics operator[](unsigned char c);



  Bitmap();
};



void Start();
void End();


Bitmap GenerateBitmap(const char* fontFile, int size, unsigned char low, unsigned char high, int filtering);
void Render(Bitmap& bitmap, std::string text, RenderTextInfo info);


} // namespace text_rendering
} // namespace pPack

#endif