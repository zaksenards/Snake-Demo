#include <Raylib.h>
#include <raymath.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#define WINDOW_TITLE  "Raylib Snake demo"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

// Represents a single player sector
struct Units
{
    Rectangle rect;
    Color color;
    int scale;

    Units(Vector2 _pos, int _scale, Color _color):scale(_scale),color(_color)
    {
        rect.x = _pos.x;
        rect.y = _pos.y;
        rect.width = _scale;
        rect.height = _scale;
    }

    void setPosition(Vector2 pos)
    {
        rect.x = pos.x;
        rect.y = pos.y;
    }

    Vector2 getPosition()
    {
        Vector2 vector = {rect.x, rect.y};
        return vector;
    }

    void draw()
    {
        DrawRectangleRec(rect, color);
    }
};

struct Player
{
    Units* units[(WINDOW_WIDTH*WINDOW_HEIGHT)/20];
    Vector2 direction;
    Vector2 stepSize;
    Vector2 position;
    int nUnits = 2;

    Player(Vector2 pos):position(pos)
    {
        direction = {0,1};
        stepSize = {20,20};

        for(int i = 1; i <= nUnits; i++)
        {
            units[i-1] = new Units(pos, 20, BLUE);
        }
    }

    ~Player()
    {
        for(int i = 1; i <= nUnits; i++)
        {
            delete units[i-1];
        }
    }

    void increase()
    {
        Rectangle tailRect = units[nUnits-1]->rect;
        Vector2 tailPosition = {tailRect.x, tailRect.y};
        units[nUnits] = new Units(tailPosition, 20, BLUE);
        nUnits++;
    }

    void update()
    {
        // Update moviment direction
        
        int dirY = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
        int dirX = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);

        if(abs(dirY) && direction.y == 0){
            direction.y = dirY;
            direction.x = 0;
        }

        if(abs(dirX) && direction.x == 0){
            direction.x = dirX;
            direction.y = 0;
        }

        // Calculate the new position
        Vector2 multi = Vector2Multiply(direction,stepSize);
        position = Vector2Add(position, multi);

        // Update units position
        Vector2 newPosition = position;
        Vector2 lastPosition;
        for(int i = 1; i <= nUnits; i++)
        {
            lastPosition = units[i-1]->getPosition();
            units[i-1]->setPosition(newPosition);

            Vector2 tmp = lastPosition;
            lastPosition = newPosition;
            newPosition = tmp;
        }
    }

    void draw()
    {
        for(int i = 1; i <= nUnits; i++)
        {
            Units* unit = units[i-1];
            unit->draw();
        }
    }

    bool isDead()
    {
        Units* head = units[0];
        for(int i = 3; i <= nUnits; i++)
        {
            Units* body = units[i-1];
            if(CheckCollisionRecs(head->rect, body->rect))
                return true;
        }

        bool horizontal = position.x-stepSize.x < 0 || position.x+stepSize.x > WINDOW_WIDTH;
        bool vertical = position.y-stepSize.y < 0 || position.y+stepSize.y > WINDOW_HEIGHT;

        return horizontal || vertical;
    }
};

int randint(int max)
{
    int n = ((int)rand()*20)%max;
    return n;
}

int main(int argc, char* argv[])
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(15);

    Player player({400, 300});
    Units food = Units({randint(800), randint(600)}, 20, RED);
    srand(time(NULL));

    while(!WindowShouldClose())
    {

        if(player.isDead())
            DrawText("You Lose!", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 50, RED);
        else
            player.update();

        bool collision = CheckCollisionRecs(player.units[0]->rect, food.rect);

        BeginDrawing();
        {
            ClearBackground(BLACK);
            player.draw();
            food.draw();

            if(collision)
            {
                food = Units({randint(800), randint(600)}, 20, RED);
                player.increase();
            }
        }
        EndDrawing();
    }

    CloseWindow();
}
