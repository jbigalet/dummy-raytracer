#ifndef DEF_TEXTURE
#define DEF_TEXTURE

class Texture {
  public:
    ~Texture() {};
    virtual Vector color_at(float u, float v) = 0;
};

class ConstantTexture: public Texture {
  public:
    Vector color;

    ConstantTexture(Vector color): color(color) {}
    ConstantTexture(float r, float g, float b): color(Vector(r, g, b)) {}

    Vector color_at(float u, float v){
      return color;
    }
};

class CheckerTexture: public Texture {
  public:
    Texture* t1;
    Texture* t2;

    CheckerTexture(Texture* t1, Texture* t2): t1(t1), t2(t2) {}

    Vector color_at(float u, float v){
      if(sin(300*u)*sin(300*v) > 0)
        return t1->color_at(u, v);
      else
        return t2->color_at(u, v);
    }
};

class ImageTexture: public Texture {
  public:
    int width, height;
    Vector** image;

    ImageTexture(std::string filename) {
      FILE* file = fopen(filename.c_str(), "rb");

      unsigned char info[54];
      fread(info, sizeof(unsigned char), 54, file);

      width = *(int*)&info[18];
      height = *(int*)&info[22];

      int row_size = (3*width + 3) & (~3);  // row padded to be able to read 24 bit color bmp
      unsigned char* data = new unsigned char[row_size];

      image = new Vector*[width];
      for(int i=0; i<width; i++)
        image[i] = new Vector[height];

      for(int j=0 ; j<height ; j++){
        fread(data, sizeof(unsigned char), row_size, file);
        for(int i=0 ; i<width ; i++){
          float r = int(data[3*i+2])/255.f;
          float g = int(data[3*i+1])/255.f;
          float b = int(data[3*i])/255.f;
          image[i][j] = Vector(r, g, b);
        }
      }

      fclose(file);
    }

    Vector color_at(float u, float v){
      int i = int(u*width);
      int j = int(v*height);

      if(i<0) i = 0;
      if(i>width-1) i = width-1;
      if(j<0) j = 0;
      if(j>height-1) j = height-1;

      return image[i][j];
    }
};

#endif /* DEF_TEXTURE */
