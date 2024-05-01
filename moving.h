#include "main.h"
enum ElementType
{
    PATH,
    WALL,
    FAKE_WALL
};
enum RobotType
{
    C = 0,
    S,
    W,
    SW
};
enum MovingObjectType
{
    SHERLOCK,
    WATSON,
    CRIMINAL,
    ROBOT
};

class MapElement
{
protected:
    ElementType type;

public:
    MapElement(ElementType in_type);
    virtual ~MapElement();
    virtual ElementType getType() const;
};

class Path : public MapElement
{
public:
    Path();
};

class Wall : public MapElement
{
public:
    Wall();
};

class FakeWall : public MapElement
{
private:
    int req_exp;

public:
    FakeWall(int in_req_exp);
    int getReqExp() const;
};
class Position
{
private:
    int r, c;

public:
    static const Position npos;
    Position(int r = 0, int c = 0);
    Position(const string &str_pos);
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const;
    bool isEqual(Position position) const;
};

class Map;

class MovingObject
{
protected:
    int index;
    Position pos;
    Map *map;
    string name;

public:
    MovingObject(int index, const Position pos, Map *map, const string &name = "");
    Position getCurrentPosition() const;
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual MovingObjectType getObjectType() const = 0;
};

class Character : public MovingObject
{

public:
    Character(int index, const Position pos, Map *map, const string &name = "");

    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual MovingObjectType getObjectType() const = 0;
};

class Sherlock : public Character
{
private:
    string moving_rule;
    int HP;
    int EXP;
    int index_moving_rule;

public:
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);

    // TODO implement các phương thức getNextPosition, move, str, getObjectType ....
    Position getNextPosition() override;
    void move();
    string str() const;

    MovingObjectType getObjectType() const;

    int getHP() const;
    int getEXP() const;
    void setHP(int hp);
    void setEXP(int exp);
};

class Watson : public Character
{
private:
    string moving_rule;
    int HP;
    int EXP;
    int index_moving_rule;

public:
    Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);

    Position getNextPosition() override;

    void move();
    string str() const override;

    MovingObjectType getObjectType() const override;

    int getHP() const;
    int getEXP() const;

    void setHP(int hp);

    void setEXP(int exp);
};

class Map
{
private:
    int num_rows, num_cols;
    MapElement ***map;

public:
    Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls);
    ~Map();

    int getNumRows() const;
    int getNumCols() const;
    ElementType getElementType(int i, int j) const;
    bool isValid(const Position pos, MovingObject *mv_obj) const;
};

class Criminal : public Character
{
private:
    Sherlock *sherlock;
    Watson *watson;
    int count;

public:
    int manhattanDistance(const Position &pos1, const Position &pos2) const;
    bool isCreatedRobotNext() const;
    Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson);
    Position getNextPosition() override;
    void move();

    string str() const;

    MovingObjectType getObjectType() const;
    int getCount() const;
};

class Robot : public MovingObject
{
protected:
    Criminal *criminal;
    // BaseItem * item;
    RobotType robot_type;

public:
    Robot(int index, const Position &pos, Map *map, RobotType robot_type, Criminal *criminal, const string &name = "");

    static Robot *create(int index, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson);
    MovingObjectType getObjectType() const;
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual RobotType getType() const = 0;
    virtual int getDistance() const = 0;
};
class RobotC : public Robot
{
private:
    Position nextPosition;

public:
    RobotC(int index, const Position &init_pos, Map *map, RobotType robot_type, Criminal *criminal);

    int getDistance(Sherlock *sherlock);

    int getDistance(Watson *watson);
    Position getNextPosition() override;
    void move();
    string str() const override;
    int getDistance() const override;
    RobotType getType() const override;
};

class RobotW : public Robot
{
private:
    Watson *watson;

public:
    RobotW(int index, const Position &init_pos, Map *map, RobotType robot_type, Criminal *criminal, Watson *watson);

    Position getNextPosition() override;

    void move() override;

    string str() const override;

    RobotType getType() const override;

    int getDistance() const override;
};

class RobotS : public Robot
{
private:
    Sherlock *sherlock;

public:
    RobotS(int index, const Position &init_pos, Map *map, RobotType robot_type, Criminal *criminal, Sherlock *sherlock);

    Position
    getNextPosition() override;
    void move() override;

    string str() const override;
    RobotType getType() const override;

    int getDistance() const override;
};

class RobotSW : public Robot
{
private:
    Sherlock *sherlock;
    Watson *watson;

public:
    RobotSW(int index, const Position &init_pos, Map *map, RobotType robot_type, Criminal *criminal, Sherlock *sherlock, Watson *watson);

    Position getNextPosition() override;
    string str() const;
    RobotType getType() const override;
    int getDistance() const override;
    void move() override;
};

class ArrayMovingObject
{
private:
    MovingObject **arr_mv_objs;
    int count;
    int capacity;

public:
    ArrayMovingObject(int capacity);
    ~ArrayMovingObject();
    bool isFull() const;
    bool add(MovingObject *mv_obj);
    MovingObject *get(int index) const;
    int size() const;
    string str() const;
};

class Configuration
{
    friend class StudyPinkProgram;

private:
    int map_num_rows;
    int map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position *arr_walls;
    int num_fake_walls;
    Position *arr_fake_walls;
    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp;
    int sherlock_init_exp;
    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp;
    int watson_init_exp;
    Position criminal_init_pos;
    int num_steps;

public:
    Configuration(const string &filepath);

    ~Configuration();
    string str() const;
};

class StudyPinkProgram
{
private:
    // Sample attributes
    Configuration *config;

    Sherlock *sherlock;
    Watson *watson;
    Criminal *criminal;

    Map *map;
    ArrayMovingObject *arr_mv_objs;

public:
    StudyPinkProgram(const string &config_file_path);
    void printMap(ofstream &OUTPUT, int roundSize) const;
    bool isStop() const;
    void printResult() const;
    void printStep(int si) const;
    void run(bool verbose);
    void run(ofstream &OUTPUT);
    ~StudyPinkProgram();
};
