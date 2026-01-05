//#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
//#define STBI_FAILURE_USERMSG


#include "pPack/textRendering.h"
#include "pPack/windowManager.h"
#include "pPack/shaderHandling.h"

#include <ft2build.h>
#include "freetype/freetype.h"
#include "stb_image.h"
#include "stb_image_write.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include <fstream>
#include <vector>

#include "stdlib.h"


using namespace ::pPack;
using namespace ::pPack::text_rendering;
using namespace ::pPack::vector;
using namespace ::pPack::shader_handling;
using namespace ::pPack::window_manager;




namespace {





struct BitmapFileInfo {
  unsigned char charLow;
  unsigned char charHigh;
  int fontSize;
  vector::IVector2 size;


  BitmapFileInfo();
};


struct CharacterVertex {
  IVector2 position;
  Vector2 uv;

  CharacterVertex();
  CharacterVertex(IVector2 Position, Vector2 UV);
};


CharacterVertex* AddCharactersIntoBuffer(CharacterVertex* vertices, Bitmap& bitmap, IVector2 startAt, std::string chars);
bool FileExists(char const* file);
bool IsWhiteSpace(unsigned char c);




const char signature[8] = {'B', 'I', 'T', 'M', 'A', 'P', 'T', 'R'};


const char* vertLocs[] = {"./shaders/textRendering/textRendering.vert"};
const char* fragLocs[] = {"./shaders/textRendering/textRendering.frag"};


FT_Library library;
FT_Error err;

ShaderHandler shader = ShaderHandler();


unsigned int VAO, VBO;


CharacterVertex* vertices = nullptr;


// Allows for about 1.33... million characters in one batch
// Probably enough
constexpr size_t _VERTEX_BUFFER_SIZE = 128000000;


} // namespace









namespace pPack {
namespace text_rendering {





// CharacterMetrics
// --------------------------------------------------

// Constructor
CharacterMetrics::CharacterMetrics() : pixelBottomLeft(0), pixelTopRight(0), uvBottomLeft(0), uvTopRight(0), size(0), bearing(0), advance(0) {}
CharacterMetrics::CharacterMetrics(IVector2 Size, IVector2 Bearing, float Advance)
: pixelBottomLeft(0), pixelTopRight(0), uvBottomLeft(0), uvTopRight(0), size(Size), bearing(Bearing), advance(Advance) {}

// --------------------------------------------------
// CharacterMetrics





// RenderTextInfo
// --------------------------------------------------

// Constructor
RenderTextInfo::RenderTextInfo(IVector2 FrameSize) : fontScale(1.0), lineSize(1.0), textWidth(2.0), centerX(false), centerY(false), startTop(false), startPos(-1.f, 1.f), color(0.f, 0.f, 0.f, 1.0f), frameSize(FrameSize) {}

// --------------------------------------------------





// Bitmap
// --------------------------------------------------

void Bitmap::LoadFromFile(const char* file, int filtering) {
  if (file == nullptr) {
    std::cerr << "Null string\n";
    return;
  }

  if (!FileExists(file)) {
    return;
  }

  std::ifstream stream(file, std::ios::binary | std::ios::ate);

  size_t fileLength = stream.tellg();

  uint16_t imageFileLength = 0;
  char* bitmapImageFile = nullptr;
  BitmapFileInfo fileInfo;

  char* buffer = nullptr;
  CharacterMetrics* metricsBuffer = nullptr;
  void* imageBuffer = nullptr;

  int charCount = 0;



  // Get file textureSize
  stream.seekg(0, std::ios::beg);

  // Needs 8 bytes for signature
  if (fileLength < 8) {
    stream.close();
    std::cerr << "Invalid file\n";
    return;
  }

  // Buffers
  buffer = (char*)calloc(fileLength, sizeof(char));

  // Make intellisense happy
  if (buffer == 0) {
    throw std::bad_alloc();
  }

  metricsBuffer = (CharacterMetrics*)buffer;


  // Get signature
  stream.read(buffer, 8);

  // Check signature
  if (memcmp(signature, buffer, 8) != 0) {
    std::cerr << "Invalid bitmap file\n";
    goto returnJump;
  }


  // string length
  stream.read((char*)&imageFileLength, 2);

  // Get string
  bitmapImageFile = (char*)malloc(imageFileLength + 1);

  // Make intellisense happy
  if (bitmapImageFile == 0) {
    throw std::bad_alloc();
  }

  bitmapImageFile[imageFileLength] = '\0';
  stream.read(bitmapImageFile, imageFileLength);

  if (!FileExists(bitmapImageFile)) {
    std::cerr << "Bitmap image DNE\n";
    goto returnJump;
  }


  // Get basic information
  stream.read((char*)&fileInfo, sizeof(BitmapFileInfo));
  if (fileInfo.charLow > fileInfo.charHigh || fileInfo.size == 0 || fileInfo.fontSize == 0) {
    std::cerr << "Invalid bitmap character range or size\n";
    goto returnJump;
  }


  charCount = (int)fileInfo.charHigh - fileInfo.charLow + 1;


  // Check if file has enough room for stated character range
  if (charCount * sizeof(CharacterMetrics) > fileLength - stream.tellg()) {
    std::cerr << "Invalid bitmap file\n";
    goto returnJump;
  }


  // Read image file
  imageBuffer = stbi_load(bitmapImageFile, &textureSize.x, &textureSize.y, nullptr, 1);
  if (imageBuffer == nullptr) {
    std::cerr << stbi_failure_reason() << '\n';
    goto returnJump;
  }


  // Destory any existing data
  Destroy();


  // Read all metrics
  stream.read(buffer, charCount * sizeof(CharacterMetrics));


  // Loop and set metrics in mapping
  for (unsigned char c = fileInfo.charLow; ; c++, metricsBuffer++) {
    characterMapping[c] = *metricsBuffer;


    if (c == fileInfo.charHigh) {
      break;
    }
  }


  int packAlignment, currentTexture;
  glGetIntegerv(GL_UNPACK_ALIGNMENT, &packAlignment);
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);



  // Generate bitmap texture
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, fileInfo.size.x, fileInfo.size.y, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, imageBuffer);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);


  // Restore
  glPixelStorei(GL_UNPACK_ALIGNMENT, packAlignment);
  glBindTexture(GL_TEXTURE_2D, currentTexture);


  charLow = fileInfo.charLow;
  charHigh = fileInfo.charHigh;
  textureSize = fileInfo.size;


  returnJump:

  // Free resources

  stbi_image_free(imageBuffer);
  free(buffer);
  free(bitmapImageFile);
  stream.close();
}





void Bitmap::SaveToFile(const char* file, const char* imageLocation) {
  if (file == nullptr || imageLocation == nullptr) {
    std::cerr << "Null strings\n";
    return;
  }

  // Check if valid
  if (!Valid()) {
    std::cerr << "Invalid bitmap\n";
    return;
  }

  // Write signature and information
  std::ofstream stream(file, std::ios::binary | std::ios::trunc);
  stream.write(signature, 8);

  uint16_t stringLen = (uint16_t)strlen(imageLocation);
  stream.write((char*)&stringLen, 2);
  stream.write(imageLocation, stringLen);
  stream.write((char*)&charLow, 1);
  stream.write((char*)&charHigh, 1);

  // padding
  stream.seekp(2, std::ios::cur);

  stream.write((char*)&fontSize, sizeof(fontSize));
  stream.write((char*)&textureSize, sizeof(IVector2));


  // Write metrics
  for (unsigned char c = charLow; ; c++) {
    CharacterMetrics& m = characterMapping[c];
    stream.write((char*)&m, sizeof(CharacterMetrics));


    if (c == charHigh)
      break;
  }


  // Close stream
  stream.flush();
  stream.close();



  // Allocate texture data
  void* textureData = malloc(textureSize.x * textureSize.y);


  int packAlignment;
  glGetIntegerv(GL_UNPACK_ALIGNMENT, &packAlignment);


  // Get texture data
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glGetTextureImage(texture, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, textureSize.x * textureSize.y, textureData);


  // Restore
  glPixelStorei(GL_PACK_ALIGNMENT, packAlignment);



  // Write texture
  stbi_write_png(imageLocation, textureSize.x, textureSize.y, 1, textureData, textureSize.x);

  // Free texture data
  free(textureData);
}





bool Bitmap::Valid() { 
  return texture != 0 && textureSize != 0 && charLow <= charHigh && !characterMapping.empty();
}





void Bitmap::Destroy() {
  // Delete and reset everything
  glDeleteTextures(1, &texture);
  characterMapping.clear();
  textureSize = 0;
  charLow = 0;
  charHigh = 0;
  texture = 0;
}





int Bitmap::GetFontSize() { return fontSize; }
unsigned char Bitmap::GetCharLow() { return charLow; }
unsigned char Bitmap::GetCharHigh() { return charHigh; }





CharacterMetrics Bitmap::operator[](unsigned char c) {
  if (!characterMapping.contains(c)) {
    return CharacterMetrics();
  }

  return characterMapping[c];
}





// Constructor
Bitmap::Bitmap() : texture(0), textureSize(0), fontSize(0), charLow(0), charHigh(0), characterMapping() {}

// --------------------------------------------------
// Bitmap





void Start() {

  // Start freetype
  err = FT_Init_FreeType(&library);
  if (err) {
    std::cerr << "Failed to initialize freetype\n";
    return;
  }


  // Create shader
  ShaderCreateInfo infos[] = {ShaderCreateInfo(vertLocs, 1, GL_VERTEX_SHADER), ShaderCreateInfo(fragLocs, 1, GL_FRAGMENT_SHADER)};
  shader = ShaderHandler::CreateShader("textRendering__SHADER", infos, 2);


  // Create buffers
  glCreateVertexArrays(1, &VAO);
  glCreateBuffers(1, &VBO);


  vertices = (CharacterVertex*)malloc(_VERTEX_BUFFER_SIZE);
}





void End() {
  FT_Done_FreeType(library);

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  free(vertices);
}





Bitmap GenerateBitmap(const char* fontFile, int size, unsigned char low, unsigned char high, int filtering) {
  Bitmap bitmap = Bitmap();

  // Check parameters
  if (!FileExists(fontFile) || size <= 0 || low > high || fontFile == nullptr) {
    std::cerr << "Invalid file or parameters\n";
    return bitmap;
  }

  // Load font
  FT_Face fontFace;
  err = FT_New_Face(library, fontFile, 0, &fontFace);
  if (err) {
    std::cerr << "Invalid font file\n";
    return bitmap;
  }
  FT_Set_Pixel_Sizes(fontFace, 0, size);



  // Load characters
  for (unsigned char c = low; ; c++) {
    // Load without creating buffer
    FT_Load_Char(fontFace, c, FT_LOAD_BITMAP_METRICS_ONLY);

    bitmap.characterMapping[c] = CharacterMetrics(
      IVector2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows),
      IVector2(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top),
      fontFace->glyph->advance.x >> 6
    );


    if (c == high)
      break;
  }


  // Get character metrics and texture size
  int maxWidth = size * ceil(sqrt(high - low + 1));
  IVector2 textureSize = {maxWidth,0};
  int largestOnRow = 0;
  int penX = 0;

  for (unsigned char c = low; ; c++) {
    CharacterMetrics& metrics = bitmap.characterMapping[c];

    // Add 2 pixels to prevent edges from touching
    if (penX + metrics.size.x + 2 > maxWidth) {
      penX = 0;
      textureSize.y += largestOnRow;
      largestOnRow = 0;
    }

    metrics.pixelBottomLeft = IVector2(penX, textureSize.y + metrics.size.y);
    metrics.pixelTopRight = IVector2(penX + metrics.size.x, textureSize.y);
    penX += metrics.size.x + 2;

    if (metrics.size.y > largestOnRow) {
      largestOnRow = metrics.size.y;
    }


    if (c == high)
      break;
  }
  textureSize.y += largestOnRow;


  // Set character uv coords
  for (unsigned char c = low; ; c++) {
    CharacterMetrics& metrics = bitmap.characterMapping[c];

    metrics.pixelBottomLeft.y = textureSize.y - metrics.pixelBottomLeft.y;
    metrics.pixelTopRight.y = textureSize.y - metrics.pixelTopRight.y;

    metrics.uvBottomLeft = {(float)metrics.pixelBottomLeft.x / textureSize.x, (float)metrics.pixelTopRight.y / textureSize.y};
    metrics.uvTopRight = {(float)metrics.pixelTopRight.x / textureSize.x, (float)metrics.pixelBottomLeft.y / textureSize.y};


    if (c == high)
      break;
  }




  // Set unpack alignment since characters probably won't be aligned to any boundry
  int packAlignment, currentTexture;
  glGetIntegerv(GL_UNPACK_ALIGNMENT, &packAlignment); 
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);



  // Set some data
  bitmap.textureSize = textureSize;
  bitmap.charLow = low;
  bitmap.charHigh = high;
  bitmap.fontSize = size;


  // Create texutre
  glGenTextures(1, &bitmap.texture);
  glBindTexture(GL_TEXTURE_2D, bitmap.texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, textureSize.x, textureSize.y, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

  // Put each character into texture based on previous generated coords
  for (unsigned char c = low; ; c++) {
    CharacterMetrics& metrics = bitmap.characterMapping[c];

    // Load second time with buffer
    FT_Load_Char(fontFace, c, FT_LOAD_RENDER);

    glTexSubImage2D(GL_TEXTURE_2D, 0, metrics.pixelBottomLeft.x, metrics.pixelBottomLeft.y,
                    metrics.size.x, metrics.size.y, GL_RED_INTEGER, GL_UNSIGNED_BYTE, fontFace->glyph->bitmap.buffer);


    if (c == high)
      break;
  }



  // Restore
  glPixelStorei(GL_UNPACK_ALIGNMENT, packAlignment);
  glBindTexture(GL_TEXTURE_2D, currentTexture);

  // Free resources
  FT_Done_Face(fontFace);

  return bitmap;
}





void Render(Bitmap& bitmap, std::string text, RenderTextInfo info) {
  if (text.size() == 0) {
    return;
  }


  if (!bitmap.Valid()) {
    std::cerr << "Invalid bitmap\n";
    return;
  }
  
  // Gets current window scale, in display settings
  float xscale, yscale;
  glfwGetWindowContentScale(Window::GetCurrentContext()->windowRef, &xscale, &yscale);


  // scaling
  info.frameSize.x /= xscale * info.fontScale;
  info.frameSize.y /= yscale * info.fontScale;


  // Move startpos to [0,0] - [1,1] range
  info.startPos += 1;
  info.startPos /= 2;

  info.textWidth /= 2;

  // Get positioning info in pixels
  int startXPixels = info.startPos.x * info.frameSize.x;
  int startYPixels = info.startPos.y * info.frameSize.y;
  int widthPixels = info.textWidth * info.frameSize.x;
  int maxXPixels = startXPixels + widthPixels;
  int jumpPixels = info.lineSize * bitmap.fontSize;

  if (!info.startTop) {
    startYPixels -= jumpPixels;
  }


  std::string line = std::string();
  std::string word = std::string();
  bool oneWord = true;

  
  // Keeps track of where to add next chunk of characters, and character count
  CharacterVertex* addSpot = vertices;

  IVector2 linePos = IVector2(startXPixels, startYPixels);
  IVector2 pen = IVector2(startXPixels, startYPixels);
  int wordLength = 0;

  bool nl = false;
  bool ws = false;
  bool cr = false;

  for (char c : text) {
    nl = false;

    CharacterMetrics& metrics = bitmap.characterMapping[c];

    if (IsWhiteSpace(c)) {

      // Skips first white space after a fresh new line
      if (nl) {
        continue;
      }

      if (word.size() != 0) {
        line += word;
        word.clear();
        wordLength = 0;
      }

      ws = true;
      oneWord = false;
    } else {
      ws = false;
    }


    // If new line or needs new line
    if (pen.x + metrics.bearing.x + metrics.size.x >= maxXPixels || c == '\r' || c == '\n') {

      if (c == '\n' && cr == true) {
        cr = false;
        continue;
      }

      // Stops \n if \r happend first
      if (c == '\r') {
        cr = true;
      } else {
        cr = false;
      }

      // Split single word up to multiple lines
      if (oneWord) {
        line += word;
        word.clear();
        wordLength = 0;
        nl = true;
      }

      // Centers on the line
      if (info.centerX) {
        linePos.x = startXPixels + (maxXPixels - pen.x + wordLength) / 2;
      }


      addSpot = AddCharactersIntoBuffer(addSpot, bitmap, linePos, line);
      line.clear();

      // Move down
      pen.x = startXPixels + wordLength;
      pen.y -= jumpPixels;
      linePos = IVector2(startXPixels, linePos.y - jumpPixels);
      oneWord = true;

      if (ws) {
        continue;
      }
    }


    word += c;
    pen.x += metrics.advance;
    wordLength += metrics.advance;
  }

  // Get whatever is left over, out
  line += word;
  if (info.centerX) {
    linePos.x = startXPixels + (maxXPixels - pen.x) / 2;
  }
  addSpot = AddCharactersIntoBuffer(addSpot, bitmap, linePos, line);



  size_t vertexCount = size_t(addSpot - vertices);


  int savedProgram, currentTexture, activeTexture, currentVAO, currentVBO;
  glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTexture);
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
  glGetIntegerv(GL_BUFFER_BINDING, &currentVBO);
  glGetIntegerv(GL_CURRENT_PROGRAM, &savedProgram);


  shader.Active();

  // Transform from pixel space to view space
  glm::mat4 transform = glm::mat4(1);
  transform = glm::translate(transform, glm::vec3(-1.0f, -1.0f, 0.0f));

  transform = glm::scale(transform, glm::vec3(2.0f / info.frameSize.x, 2.0f / info.frameSize.y, 1.0f));

  if (info.centerY) {
    transform = glm::translate(transform, glm::vec3(0, (startYPixels - pen.y + jumpPixels) / 2.f, 0));
  }


  ShaderHandler::SetMat4("transform", glm::value_ptr(transform));
  ShaderHandler::SetVector4("textColor", info.color);
  ShaderHandler::SetInt("bitmapTexture", 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, bitmap.texture);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(CharacterVertex), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribIPointer(0, 2, GL_INT, sizeof(CharacterVertex), 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CharacterVertex), (void*)(sizeof(IVector2)));

  glDrawArrays(GL_TRIANGLES, 0, vertexCount);


  // Restore
  ShaderHandler::SetCurrentShader(savedProgram);
  glActiveTexture(activeTexture);
  glBindTexture(GL_TEXTURE_2D, currentTexture);
  glBindBuffer(GL_ARRAY_BUFFER, currentVBO);
  glBindVertexArray(currentVAO);
}





} // namespace text_rendering
} // namespace pPack










namespace {





// BitmapFileInfo
// --------------------------------------------------

// Constructor
BitmapFileInfo::BitmapFileInfo() : charLow(0), charHigh(0), fontSize(0), size(0) {}

// --------------------------------------------------
// BitmapFileInfo





// CharacterVertex
// --------------------------------------------------

CharacterVertex::CharacterVertex() : position(0), uv(0) {}
CharacterVertex::CharacterVertex(IVector2 Position, Vector2 UV) : position(Position), uv(UV) {}

// --------------------------------------------------
// CharacterVertex





CharacterVertex* AddCharactersIntoBuffer(CharacterVertex* vertices, Bitmap& bitmap, IVector2 startAt, std::string chars) {
  if (chars.size() == 0) {
    return vertices;
  }

  IVector2 pen = startAt;

  CharacterVertex verts[6];

  // First character shouldn't move forward by it's bearing
  if (!IsWhiteSpace(chars[0])) {
    CharacterMetrics& metrics = bitmap.characterMapping[chars[0]];

    // Only difference here
    IVector2 pos = {pen.x, pen.y - (metrics.size.y - metrics.bearing.y)};

    verts[0] = CharacterVertex(pos, metrics.uvBottomLeft);
    verts[1] = CharacterVertex({pos.x + metrics.size.x, pos.y}, {metrics.uvTopRight.x, metrics.uvBottomLeft.y});
    verts[2] = CharacterVertex({pos.x, pos.y + metrics.size.y}, {metrics.uvBottomLeft.x, metrics.uvTopRight.y});

    verts[3] = CharacterVertex({pos.x + metrics.size.x, pos.y}, {metrics.uvTopRight.x, metrics.uvBottomLeft.y});
    verts[4] = CharacterVertex({pos.x, pos.y + metrics.size.y}, {metrics.uvBottomLeft.x, metrics.uvTopRight.y});
    verts[5] = CharacterVertex(pos + metrics.size, metrics.uvTopRight);

    memcpy(vertices, verts, 6 * sizeof(CharacterVertex));

    pen.x += metrics.advance;

    vertices += 6;
  }



  for (size_t i = 1; i < chars.size(); i++) {
    char c = chars[i];
    CharacterMetrics& metrics = bitmap.characterMapping[c];

    if (IsWhiteSpace(c)) {
      pen.x += metrics.advance;
      continue;
    }


    IVector2 pos = {pen.x + metrics.bearing.x, pen.y - (metrics.size.y - metrics.bearing.y)};

    verts[0] = CharacterVertex(pos, metrics.uvBottomLeft);
    verts[1] = CharacterVertex({pos.x + metrics.size.x, pos.y}, {metrics.uvTopRight.x, metrics.uvBottomLeft.y});
    verts[2] = CharacterVertex({pos.x, pos.y + metrics.size.y}, {metrics.uvBottomLeft.x, metrics.uvTopRight.y});

    verts[3] = CharacterVertex({pos.x + metrics.size.x, pos.y}, {metrics.uvTopRight.x, metrics.uvBottomLeft.y});
    verts[4] = CharacterVertex({pos.x, pos.y + metrics.size.y}, {metrics.uvBottomLeft.x, metrics.uvTopRight.y});
    verts[5] = CharacterVertex(pos + metrics.size, metrics.uvTopRight);

    memcpy(vertices, verts, 6 * sizeof(CharacterVertex));

    vertices += 6;

    pen.x += metrics.advance;
  }


  return vertices;
}





bool FileExists(char const* file) {
  struct _stat buffer;
  if (_stat(file, &buffer) == 0)
    return true;
  return false;
}





bool IsWhiteSpace(unsigned char c) {
  return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}





} // namespace