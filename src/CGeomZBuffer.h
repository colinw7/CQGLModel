#ifndef CGEOM_ZBUFFER_H
#define CGEOM_ZBUFFER_H

#include <CBresenham.h>
#include <CImage.h>
#include <CFont.h>
#include <CILineDash.h>
#include <CMathGeom2D.h>
#include <CGeom3DRenderer.h>

class CGeomFace3D;

class CGeom3DBresenham : public CBresenham {
 public:
  CGeom3DBresenham(CGeom3DRenderer *renderer) :
   renderer_(renderer) {
  }

  void drawPoint(int x, int y) {
    renderer_->drawPoint(CIPoint2D(x, y));
  }

  virtual const CILineDash &getLineDash() const {
    static CILineDash dash;

    return dash;
  }

 private:
  CGeom3DRenderer *renderer_;
};

class CGeomZBuffer {
 private:
  class Renderer : public CGeom3DRenderer {
   private:
    CGeomZBuffer      *buffer_;
    double             z_, dz_;
    CGeom3DBresenham   bresenham_;
    bool               alpha_;

   public:
    Renderer(CGeomZBuffer *buffer) :
     buffer_(buffer), z_(0), dz_(0), bresenham_(this), alpha_(false) {
    }

    Renderer *dup() const {
      return new Renderer(buffer_);
    }

    uint getWidth () const { return buffer_->getWidth (); }
    uint getHeight() const { return buffer_->getHeight(); }

    void setForeground(const CRGBA &rgba) {
      buffer_->setForeground(rgba);
    }

    void drawZLine(int x1, int y1, double z1, int x2, int y2, double z2, bool alpha=false) {
      bool alpha1 = setZAlpha(alpha);

      int dx = abs(x2 - x1);
      int dy = abs(y2 - y1);

      if (dx > dy) {
        if (x2 > x1) {
          z_  = z1;
          dz_ = (z2 - z1)/(x2 - x1);
        }
        else {
          z_  = z2;
          dz_ = (z1 - z2)/(x1 - x2);
        }
      }
      else {
        if (y2 > y1) {
          z_  = z1;
          dz_ = (z2 - z1)/(y2 - y1);
        }
        else {
          z_  = z2;
          dz_ = (z1 - z2)/(y1 - y2);
        }
      }

      int width  = getWidth ();
      int height = getHeight();

      if (CMathGeom2D::clipLine(0, 0, width - 1, height - 1, &x1, &y1, &x2, &y2))
        bresenham_.drawLine(x1, y1, x2, y2);

      setZAlpha(alpha1);
    }

    bool getZAlpha() const { return alpha_; }

    bool setZAlpha(bool alpha) {
      std::swap(alpha_, alpha);

      return alpha;
    }

    void drawAlphaZLine(int x1, int y1, int x2, int y2) {
      drawZLine(x1, y1, 0, x2, y2, 1, true);
    }

    void drawZLine(int x1, int y1, int x2, int y2) {
      drawZLine(x1, y1, 0, x2, y2, 1, false);
    }

    void drawPoint(const CIPoint2D &point) {
      if (! getZAlpha()) {
        buffer_->drawFacePoint(NULL, point.x, point.y, z_);

        z_ += dz_;
      }
      else
        buffer_->drawOverlayPoint(point.x, point.y);
    }

    void drawLine(const CIPoint2D &, const CIPoint2D &) {
      //buffer_->drawOverlayZLine(point1, point2);
    }

    void setFont(CFontPtr font) {
      buffer_->setFont(font);
    }

    void drawString(const CIPoint2D &point, const std::string &str) {
      buffer_->drawOverlayImageString(point.x, point.y, str);
    }

    void fillCircle(const CIPoint2D &, double) {
      //buffer_->fillCircle(c, r);
    }

#if 0
    void drawClippedChar(const CIPoint2D &point, char c) {
      buffer_->drawOverlayImageChar(point.x, point.y, c);
    }

    void drawClippedRectangle(const CIBBox2D &bbox) {
      buffer_->drawRectangle(bbox);
    }

    void fillClippedRectangle(const CIBBox2D &bbox) {
      buffer_->fillRectangle(bbox);
    }

    void fillClippedTriangle(const CIPoint2D &point1, const CIPoint2D &point2,
                             const CIPoint2D &point3) {
      buffer_->fillTriangle(point1, point2, point3);
    }

    void drawClippedPolygon(const std::vector<CIPoint2D> &points) {
      buffer_->drawPolygon(points);
    }

    void fillClippedPolygon(const std::vector<CIPoint2D> &points) {
      buffer_->fillPolygon(points);
    }

    void drawClippedImage(const CIPoint2D &point, CImagePtr image) {
      buffer_->drawImage(point, image);
    }

    int getCharWidth  (char) { return 8; }
    int getCharWidth  ()     { return 8; }
    int getCharAscent ()     { return 12; }
    int getCharDescent()     { return 0; }

    int getStringWidth(const std::string &str) {
      return str.size()*getCharWidth();
    }
#endif

   private:
    Renderer(const Renderer &rhs);
    Renderer &operator=(const Renderer &rhs);
  };

  struct Pixel {
    bool         set;
    double       z;
    CRGBA        rgba;
    CGeomFace3D *face;

    Pixel() :
      set (false),
      z   (0.0),
      rgba(0, 0, 0),
      face(NULL) {
    }

   private:
    Pixel(const Pixel &rhs);
    Pixel &operator=(const Pixel &rhs);
  };

  struct Line {
    bool   set;
    uint   width;
    uint   start, end;
    Pixel *pixels;

    Line(uint width1=0) :
      set(false), width(width1), start(), end(), pixels() {
    }

    void clear() {
      set = false;

      for (uint x = start; x <= end; ++x)
        pixels[x].set = false;
    }

   private:
    Line(const Line &rhs);
    Line &operator=(const Line &rhs);
  };

  //------

  CGeom3DRenderer *renderer_;
  Line            *lines_;
  uint             width_, height_;
  CRGBA            rgba_;
  CFontPtr         font_;
  Renderer         zrenderer_;
  bool             writable_;

 public:
  CGeomZBuffer(CGeom3DRenderer *renderer) :
   renderer_(renderer), lines_(NULL), width_(0), height_(0), rgba_(0,0,0),
   font_(), zrenderer_(this), writable_(true) {
    updateSize();
  }

 ~CGeomZBuffer() {
    resize(0, 0);
  }

  CGeom3DRenderer *getRenderer() const { return renderer_; }

  void setRenderer(CGeom3DRenderer *renderer) {
    renderer_ = renderer;
  }

  uint getWidth () const { return width_ ; }
  uint getHeight() const { return height_; }

  void updateSize() {
    if (renderer_)
      resize(renderer_->getWidth (), renderer_->getHeight());
  }

  void resize(uint width, uint height) {
    if (width == width_ || height == height_)
      return;

    for (uint y = 0; y < height_; ++y)
      delete [] lines_[y].pixels;

    delete [] lines_;

    //-----

    width_  = width;
    height_ = height;

    if (height_ > 0) {
      lines_ = new Line [height_];

      for (uint y = 0; y < height_; ++y) {
        Line *line = &lines_[y];

        line->width  = width_;
        line->pixels = new Pixel [width_];
      }
    }
    else
      lines_ = NULL;
  }

  void clear() {
    for (uint y = 0; y < height_; ++y) {
      Line *line = &lines_[y];

      if (! line->set) continue;

      line->clear();
    }
  }

  void setForeground(const CRGBA &rgba) {
    rgba_ = rgba;
  }

  void setFont(CFontPtr font) {
    font_ = font;
  }

  void drawFacePoint(CGeomFace3D *face, int x, int y, double z) {
    if (z > 0) return;

    if (x < 0 || x >= int(width_ ) ||
        y < 0 || y >= int(height_))
      return;

    Line *line = &lines_[y];

    if (! lines_[y].set) {
      line->set   = true;
      line->start = x;
      line->end   = x;
    }
    else {
      line->start = std::min(line->start, uint(x));
      line->end   = std::max(line->end  , uint(x));
    }

    Pixel *pixel = &line->pixels[x];

    if      (! pixel->set) {
      if (writable_) {
        pixel->z    = z;
        pixel->rgba = rgba_;
        pixel->set  = true;

        if (face)
          pixel->face = face;
      }
      else {
        if (! rgba_.isTransparent()) {
          pixel->rgba = rgba_;
          pixel->set  = true;
        }
      }
    }
    else if (z > pixel->z) {
      if (writable_) {
        pixel->z    = z;
        pixel->rgba = rgba_;

        if (face)
          pixel->face = face;
      }
      else
        pixel->rgba = CRGBA::modeCombine(rgba_, pixel->rgba,
                                         CRGBA_COMBINE_SRC_ALPHA,
                                         CRGBA_COMBINE_ONE_MINUS_SRC_ALPHA);
    }
  }

  void drawOverlayPoint(int x, int y) {
    if (x < 0 || x >= int(width_ ) ||
        y < 0 || y >= int(height_))
      return;

    Line *line = &lines_[y];

    if (! lines_[y].set) {
      line->set   = true;
      line->start = x;
      line->end   = x;
    }
    else {
      line->start = std::min(line->start, uint(x));
      line->end   = std::max(line->end  , uint(x));
    }

    Pixel *pixel = &line->pixels[x];

    if (pixel->set)
      pixel->rgba = CRGBA::modeCombine(rgba_, pixel->rgba,
                                       CRGBA_COMBINE_SRC_ALPHA,
                                       CRGBA_COMBINE_ONE_MINUS_SRC_ALPHA);
    else {
      if (! rgba_.isTransparent()) {
        pixel->rgba = rgba_;
        pixel->set  = true;
      }
    }
  }

  void drawZLine(int x1, int y1, double z1, int x2, int y2, double z2) {
    zrenderer_.drawZLine(x1, y1, z1, x2, y2, z2);
  }

  void drawOverlayZLine(int x1, int y1, int x2, int y2) {
    zrenderer_.drawAlphaZLine(x1, y1, x2, y2);
  }

  void drawImage(int x, int y, double z, CImagePtr image) {
    int iwidth  = image->getWidth ();
    int iheight = image->getHeight();

    int x1 = 0;
    int x2 = iwidth  - 1;
    int y1 = 0;
    int y2 = iheight - 1;

    CRGBA rgba;

    for (int yy = y1; yy <= y2; ++yy) {
      for (int xx = x1; xx <= x2; ++xx) {
        image->getRGBAPixel(xx, yy, rgba);

        setForeground(rgba);

        drawFacePoint(NULL, x + xx, y + yy, z);
      }
    }
  }

  void drawOverlayImage(int x, int y, CImagePtr image) {
    int iwidth  = image->getWidth ();
    int iheight = image->getHeight();

    int x1 = 0;
    int x2 = iwidth  - 1;
    int y1 = 0;
    int y2 = iheight - 1;

    CRGBA rgba;

    for (int yy = y1; yy <= y2; ++yy) {
      for (int xx = x1; xx <= x2; ++xx) {
        image->getRGBAPixel(xx, yy, rgba);

        setForeground(rgba);

        drawOverlayPoint(x + xx, y + yy);
      }
    }
  }

  void drawOverlayString(int x, int y, const std::string &str) {
    bool alpha = zrenderer_.setZAlpha(true);

    zrenderer_.setFont(font_);

    zrenderer_.drawString(CIPoint2D(x, y), str);

    zrenderer_.setZAlpha(alpha);
  }

  void drawOverlayImageString(int x, int y, const std::string &str) {
    CImagePtr image = font_->getStringImage(str);

    drawOverlayImage(x, y, image);
  }

  void drawOverlayImageChar(int x, int y, char c) {
    CImagePtr image = font_->getCharImage(c);

    drawOverlayImage(x, y, image);
  }

  void fillOverlayRectangle(int x, int y, int w, int h) {
    for (int y1 = 0; y1 < h; ++y1)
      drawOverlayZLine(x, y + y1, x + w - 1, y + y1);
  }

  void draw() {
    for (uint y = 0; y < height_; ++y) {
      Line *line = &lines_[y];

      if (! line->set) continue;

      for (uint x = line->start; x <= line->end; ++x) {
        Pixel *pixel = &line->pixels[x];

        if (! pixel->set) continue;

        renderer_->setForeground(pixel->rgba);

        renderer_->drawPoint(CIPoint2D(x, y));
      }
    }
  }

  CGeomFace3D *getFace(int x, int y) {
    if (x < 0 || x >= int(width_ ) ||
        y < 0 || y >= int(height_))
      return NULL;

    Line *line = &lines_[y];

    if (! line->set)
      return NULL;

    Pixel *pixel = &line->pixels[x];

    if (! pixel->set)
      return NULL;

    return pixel->face;
  }

  double getZ(int x, int y) {
    if (x < 0 || x >= int(width_ ) ||
        y < 0 || y >= int(height_))
      return 0;

    Line *line = &lines_[y];

    if (! line->set)
      return 0;

    Pixel *pixel = &line->pixels[x];

    if (! pixel->set)
      return 0;

    return pixel->z;
  }

 private:
  CGeomZBuffer(const CGeomZBuffer &rhs);
  CGeomZBuffer &operator=(const CGeomZBuffer &rhs);
};

#endif