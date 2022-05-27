#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <memory>

using namespace std::literals;

struct Point
{
    int x, y;

    Point(int x = 0, int y = 0) : x{x}, y{y}
    {}
};

std::ostream& operator<<(std::ostream& out, const Point& pt)
{
    out << "(" << pt.x << ", " << pt.y << ")";
    return out;
}

class IShape
{
public:
    virtual ~IShape() = default;
    virtual void move(int dx, int dy) = 0;
    virtual void draw() const = 0;
};

class Shape : public IShape
{
    Point coord_;
public:
    Shape(const Point& coord) : coord_{coord}
    {}

    Shape(int x = 0, int y = 0) : coord_{x, y}
    {}

    ~Shape() noexcept override
    {
        std::cout << "~Shape(" << coord_ << ")\n";
    }

    void move(int dx, int dy) override
    {
        coord_.x += dx;
        coord_.y += dy;
    }

    const Point& coord() const
    {
        return coord_;
    }
};


class Circle : public Shape
{
    int radius_;
public:
    Circle(int x = 0, int y = 0, int r = 0)
        : Shape{x, y}, radius_{r}
    {}

    Circle(const Point& pt, int r) : Shape{pt}, radius_{r}
    {}

    ~Circle() noexcept override
    {
        std::cout << "~Circle(" << coord() << ")\n";
    }

    int radius() const
    {
        return radius_;
    }

    void draw() const override
    {
        std::cout << "Circle at " << coord() << " with r: " << radius_ << "\n";
    }
};


class Rectangle : public Shape
{
    uint32_t width_;
    uint32_t height_;
public:
    Rectangle(int x, int y, uint32_t w, uint32_t h)
        : Shape{x, y}, width_{w}, height_{h}
    {}

    Rectangle(const Point& pt, uint32_t w, uint32_t h)
        : Shape{pt}, width_{w}, height_{h}
    {}

    ~Rectangle() noexcept override
    {
        std::cout << "~Rectangle(" << coord()<< ")\n";
    }

    uint32_t width() const
    {
        return width_;
    }

    uint32_t height() const
    {
        return height_;
    }

    void draw() const override
    {
        std::cout << "Rectangle at " << coord()
                  << " w: " << width_<< ", h: " << height_ << "\n";
    }
};

class Line : public Shape
{
    Point end_;
public:
    Line(int x_start = 0, int y_start = 0, int x_end = 0, int y_end = 0)
        : Shape{x_start, y_start}, end_{x_end, y_end}
    {}

    Line(const Point& start, const Point& end) : Shape{start}, end_{end}
    {}

    ~Line() override
    {
        std::cout << "~Line(" << coord() << ")\n";
    }

    void move(int dx, int dy) override
    {
        Shape::move(dx, dy); // call of method from base class

        end_.x += dx;
        end_.y += dy;
    }

    void draw() const override
    {
        std::cout << "Line from " << coord() << " to " << end_ << "\n";
    }
};

class Polygon : public IShape
{
    std::vector<Point> points_;
public:
    Polygon(std::initializer_list<Point> pts)
    {
        for(const auto& pt : pts)
        {
            points_.push_back(pt);
        }
    }

    void move(int dx, int dy) override
    {
        for(auto& pt : points_)
        {
            pt.x += dx;
            pt.y += dy;
        }
    }

    void draw() const override
    {
        std::cout << "Polygon: ";
        for(const auto& pt : points_)
            std::cout << pt << " ";
        std::cout << "\n";
    }
};

void draw_all(const std::vector<IShape*> shape_ptrs)
{
    for(auto shp_ptr : shape_ptrs)
    {
        shp_ptr->draw();
    }
}

void move_all(const std::vector<IShape*> shape_ptrs, int dx, int dy)
{
    for(IShape* shp_ptr : shape_ptrs)
    {
        shp_ptr->move(dx, dy);
    }
}

void kill_em_all(std::vector<IShape*> shape_ptrs)
{
    for(IShape* ptr_shp : shape_ptrs)
        delete ptr_shp;
}


namespace WithRawPointers
{
    class Drawing
    {
        std::vector<IShape*> shapes_;
    public:
        Drawing() = default;
        Drawing(const Drawing&) = delete;
        Drawing& operator=(const Drawing&) = delete;
        Drawing(Drawing&&) = default;
        Drawing& operator=(Drawing&&) = default;

        ~Drawing() noexcept
        {
            kill_em_all(shapes_);
        }

        void add(IShape* shp)
        {
            shapes_.push_back(shp);
        }

        void render() const
        {
            draw_all(shapes_);
        }
    };
}

class Drawing
{
    std::vector<std::unique_ptr<IShape>> shapes_;
public:
    void add(std::unique_ptr<IShape> shp)
    {
        shapes_.push_back(std::move(shp));
    }

    void render() const
    {
        for(auto& shp : shapes_)
            shp->draw();
    }
};

void memory_leak_demo();

void raw_pointer_memory_leak();

void smart_pointer_memory_ok();

void memory_leak_demo()
{
    std::vector<IShape*> album_cover = {
        new Circle(100, 200, 90),
        new Rectangle(200, 400, 100, 200),
        new Line{100, 100, 500, 600},
        new Polygon{{100, 200}, {200, 400}, {300, 400}}
    };

    draw_all(album_cover);

    album_cover.at(100); //xxxxxx

    kill_em_all(album_cover);
}

void raw_pointer_memory_leak()
{
    IShape* ptr_shp = new Circle(100, 200, 300);
    ptr_shp->draw();

    std::vector<int> vec(1'000'000'000'000);

    delete ptr_shp;
}


void smart_pointer_memory_ok()
{
    std::unique_ptr<IShape> ptr_shp = std::make_unique<Circle>(100, 200, 300);
    ptr_shp->draw();

    std::shared_ptr<IShape> shared_shp = std::make_shared<Circle>(100, 200, 400);
    std::shared_ptr<IShape> other_shared_ptr = shared_shp;

    std::unique_ptr<IShape> other_ptr = std::move(ptr_shp);
    other_ptr->draw();
}

void casting_in_inheritance()
{
    Rectangle rect{200, 300, 100, 500};
    Polygon pgn{{100, 200}, {200, 300}, {100, 100}};

    IShape* ptr_shape = &rect;
    ptr_shape->draw();

    Rectangle* ptr_rect = static_cast<Rectangle*>(ptr_shape);
    std::cout << "Width: " << ptr_rect->width() << "\n";

    ptr_shape = &pgn;
    ptr_shape->draw();

    ptr_rect = dynamic_cast<Rectangle*>(ptr_shape);
    if (ptr_rect)
        std::cout << "Width: " << ptr_rect->width() << "\n";
    else
        std::cout << "dynamic_cast returned nullptr == different type of shape" << std::endl;

    try
    {
        Rectangle& ref_rect = dynamic_cast<Rectangle&>(*ptr_shape);
    }
    catch(const std::bad_cast& e)
    {
        std::cout << e.what() << "\n";
    }
}

int main()
{
    {
        //        Shape shp(100, 200);
        //        shp.draw();
        //        shp.move(20, 30);
        //        shp.draw();

        Circle c(400, 500, 50);
        c.draw();
        c.move(20, 30);
        c.draw();
        assert(c.radius() == 50);

        Point pt{50, 90};
        Rectangle r(pt, 200, 50);
        r.draw();
        r.move(-10, 50);
        r.draw();

        Line l{400, 100, 440, 665};

        std::cout << "\n---------------\n";

        std::vector<IShape*> shape_ptrs = {&c, &r, &l};
        draw_all(shape_ptrs);

        std::cout << "\n---------------\n";
        move_all(shape_ptrs, 500, 400);
        draw_all(shape_ptrs);
    }

    std::cout << "\n---------------\n";

    std::vector<IShape*> album_cover = {
        new Circle(100, 200, 90),
        new Rectangle(200, 400, 100, 200),
        new Line{100, 100, 500, 600},
        new Polygon{{100, 200}, {200, 400}, {300, 400}}
    };

    draw_all(album_cover);

    kill_em_all(album_cover);

    std::cout << "\n---------------\n";

    {
        Drawing drw;

        drw.add(std::make_unique<Circle>(100, 200, 90));
        drw.add(std::make_unique<Rectangle>(200, 400, 100, 200));
        drw.add(std::make_unique<Line>(100, 100, 500, 600));
        auto points = {Point{100, 200}, Point{200, 400}, Point{300, 400}};
        drw.add(std::make_unique<Polygon>(points));

        drw.render();

        //    Drawing backup = drw;
        //    backup.render();
    }

    std::cout << "\n---------------\n";

    try
    {
        //memory_leak_demo();
        //raw_pointer_memory_leak();
    }
    catch (...)
    {
    }

    casting_in_inheritance();

    std::cout << "\n---------------\n";
}