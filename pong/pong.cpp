// pong.cpp
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <cmath>

// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Paddle constants
const int PADDLE_WIDTH = 20;
const int PADDLE_HEIGHT = 100;
const int PADDLE_SPEED = 5;

// Ball constants
const int BALL_RADIUS = 10;
const int BALL_SPEED = 5;

// Particle constants
struct Particle
{
    float x, y;
    float velX, velY;
    float alpha;    // Transparency
    float lifetime; // Remaining lifetime in seconds
    SDL_Color color;
};

// Structure to represent a Paddle
struct Paddle
{
    float x, y; // Top-left corner
    int width, height;
    int speed;
};

// Structure to represent the Ball
struct Ball
{
    float x, y; // Center position
    float velX, velY;
    int radius;
};

// Function prototypes
bool init(SDL_Window **window, SDL_Renderer **renderer);
void closeSDL(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font);
void drawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius, SDL_Color color);
void drawFilledCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius, SDL_Color color);
void renderText(SDL_Renderer *renderer, TTF_Font *font, const std::string &message, SDL_Color color, int x, int y);
std::string getStreakMessage(int streakCount);

// Particle system
class ParticleSystem
{
public:
    ParticleSystem(int maxParticles)
        : maxParticles(maxParticles)
    {
        particles.reserve(maxParticles);
    }

    void emit(float x, float y, SDL_Color color)
    {
        if (particles.size() >= maxParticles)
            return;

        Particle p;
        p.x = x;
        p.y = y;
        // Random velocity
        float angle = (rand() % 360) * (M_PI / 180.0f);
        float speed = static_cast<float>((rand() % 50) / 10.0 + 50.0); // 5 to 10 units/sec
        p.velX = cos(angle) * speed;
        p.velY = sin(angle) * speed;
        p.alpha = 255.0f;
        p.lifetime = 1.0f; // 1 second
        p.color = color;

        particles.push_back(p);
    }

    void update(float deltaTime)
    {
        for (auto it = particles.begin(); it != particles.end();)
        {
            it->x += it->velX * deltaTime;
            it->y += it->velY * deltaTime;
            it->lifetime -= deltaTime;
            if (it->lifetime <= 0.0f)
            {
                it = particles.erase(it);
            }
            else
            {
                // Fade out
                it->alpha = (it->lifetime / 1.0f) * 255.0f;
                ++it;
            }
        }
    }

    void render(SDL_Renderer *renderer)
    {
        for (const auto &p : particles)
        {
            SDL_Color color = p.color;
            color.a = static_cast<Uint8>(p.alpha);
            drawFilledCircle(renderer, static_cast<int>(p.x), static_cast<int>(p.y), 2, color);
        }
    }

private:
    std::vector<Particle> particles;
    int maxParticles;
};

// Function to initialize SDL, SDL_ttf
bool init(SDL_Window **window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: "
                  << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        std::cout << "SDL_ttf could not initialize! TTF_Error: "
                  << TTF_GetError() << std::endl;
        return false;
    }

    *window = SDL_CreateWindow("Vector Pong with Fireball and Fancy Paddles",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH,
                               SCREEN_HEIGHT,
                               SDL_WINDOW_SHOWN);
    if (*window == nullptr)
    {
        std::cout << "Window could not be created! SDL_Error: "
                  << SDL_GetError() << std::endl;
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == nullptr)
    {
        std::cout << "Renderer could not be created! SDL_Error: "
                  << SDL_GetError() << std::endl;
        return false;
    }

    // Set renderer draw color to black
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);

    return true;
}

// Function to close SDL and TTF
void closeSDL(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font)
{
    if (font != nullptr)
    {
        TTF_CloseFont(font);
    }

    if (renderer != nullptr)
    {
        SDL_DestroyRenderer(renderer);
    }

    if (window != nullptr)
    {
        SDL_DestroyWindow(window);
    }

    TTF_Quit();
    SDL_Quit();
}

// Function to draw an outline circle
void drawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

// Function to draw a filled circle
void drawFilledCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, centreX + dx, centreY + dy);
            }
        }
    }
}

// Function to render text
void renderText(SDL_Renderer *renderer, TTF_Font *font, const std::string &message, SDL_Color color, int x, int y)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, message.c_str(), color);
    if (surface == nullptr)
    {
        std::cout << "Unable to render text surface! TTF_Error: "
                  << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr)
    {
        std::cout << "Unable to create texture from rendered text! SDL_Error: "
                  << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
}

// Function to get streak message based on streak count
std::string getStreakMessage(int streakCount)
{
    if (streakCount == 2)
    {
        return "Double Kill!";
    }
    else if (streakCount == 3)
    {
        return "Triple Kill!";
    }
    else if (streakCount == 4)
    {
        return "Ultra Kill!";
    }
    else if (streakCount >= 5)
    {
        return "Holy Shit!";
    }
    return "";
}

int main(int argc, char *args[])
{
    srand(static_cast<unsigned int>(time(0)));

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (!init(&window, &renderer))
    {
        std::cout << "Failed to initialize!\n";
        return -1;
    }

    // Load native macOS Arial font
    const char *fontPath = "/Library/Fonts/Arial Unicode.ttf"; // Ensure this path is correct
    TTF_Font *font = TTF_OpenFont(fontPath, 24);               // Font size 24
    if (font == nullptr)
    {
        std::cout << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        closeSDL(window, renderer, font);
        return -1;
    }

    // Initialize paddles
    Paddle player;
    player.width = PADDLE_WIDTH;
    player.height = PADDLE_HEIGHT;
    player.x = 50;
    player.y = (SCREEN_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f);
    player.speed = PADDLE_SPEED;

    Paddle ai;
    ai.width = PADDLE_WIDTH;
    ai.height = PADDLE_HEIGHT;
    ai.x = SCREEN_WIDTH - 50 - PADDLE_WIDTH;
    ai.y = (SCREEN_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f);
    ai.speed = PADDLE_SPEED - 2; // AI is slightly slower for difficulty

    // Initialize ball
    Ball ball;
    ball.radius = BALL_RADIUS;
    ball.x = SCREEN_WIDTH / 2.0f;
    ball.y = SCREEN_HEIGHT / 2.0f;
    // Randomize initial direction
    ball.velX = (rand() % 2 == 0) ? BALL_SPEED : -BALL_SPEED;
    ball.velY = (rand() % 2 == 0) ? BALL_SPEED : -BALL_SPEED;

    // Initialize scores
    int playerScore = 0;
    int aiScore = 0;

    // Streak variables
    int playerStreak = 0;
    int aiStreak = 0;

    // Current streak message
    std::string currentStreakMessage = "";
    Uint32 streakMessageStartTime = 0; // Time when the message was triggered

    // Streak message duration in milliseconds
    const Uint32 STREAK_MESSAGE_DURATION = 2000; // 2 seconds

    // Initialize Particle System for fireball
    ParticleSystem fireballParticles(500); // Max 500 particles

    bool quit = false;
    SDL_Event e;

    // Text color: white
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Color streakColor = {255, 0, 0, 255}; // Red for streak messages

    // For deltaTime calculation
    Uint32 lastTick = SDL_GetTicks();

    // Game loop
    while (!quit)
    {
        // Calculate deltaTime
        Uint32 currentTick = SDL_GetTicks();
        float deltaTime = (currentTick - lastTick) / 1000.0f; // seconds
        lastTick = currentTick;

        // Handle events
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
                quit = true;
        }

        // Handle player input
        const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_W] && player.y > 0)
        {
            player.y -= player.speed;
            if (player.y < 0)
                player.y = 0;
        }
        if (currentKeyStates[SDL_SCANCODE_S] && player.y < SCREEN_HEIGHT - player.height)
        {
            player.y += player.speed;
            if (player.y > SCREEN_HEIGHT - player.height)
                player.y = SCREEN_HEIGHT - player.height;
        }

        // Simple AI: move towards the ball's y position
        if (ai.y + ai.height / 2.0f < ball.y && ai.y < SCREEN_HEIGHT - ai.height)
        {
            ai.y += ai.speed;
        }
        else if (ai.y + ai.height / 2.0f > ball.y && ai.y > 0)
        {
            ai.y -= ai.speed;
        }

        // Move the ball
        ball.x += ball.velX;
        ball.y += ball.velY;

        // Emit particles at the ball's position
        SDL_Color fireColor = {255, 165, 0, 255}; // Orange
        fireballParticles.emit(ball.x, ball.y, fireColor);

        // Update particles
        fireballParticles.update(deltaTime);

        // Collision with top and bottom
        if (ball.y - ball.radius <= 0 || ball.y + ball.radius >= SCREEN_HEIGHT)
        {
            ball.velY = -ball.velY;
        }

        // Collision with player paddle
        // Check if the ball is within the paddle's x range
        if (ball.x - ball.radius <= player.x + player.width &&
            ball.x + ball.radius >= player.x &&
            ball.y + ball.radius >= player.y &&
            ball.y - ball.radius <= player.y + player.height)
        {
            ball.velX = fabs(ball.velX); // Ensure ball moves to the right
        }

        // Collision with AI paddle
        if (ball.x + ball.radius >= ai.x &&
            ball.x - ball.radius <= ai.x + ai.width &&
            ball.y + ball.radius >= ai.y &&
            ball.y - ball.radius <= ai.y + ai.height)
        {
            ball.velX = -fabs(ball.velX); // Ensure ball moves to the left
        }

        // Check for scoring
        bool scored = false;
        if (ball.x - ball.radius < 0)
        {
            aiScore++;
            aiStreak++;
            playerStreak = 0; // Reset player's streak

            // Determine if AI has a streak
            std::string streakMsg = getStreakMessage(aiStreak);
            if (!streakMsg.empty())
            {
                currentStreakMessage = streakMsg;
                streakMessageStartTime = SDL_GetTicks(); // Record the time
            }

            // Reset the ball to center with random direction
            ball.x = SCREEN_WIDTH / 2.0f;
            ball.y = SCREEN_HEIGHT / 2.0f;
            ball.velX = (rand() % 2 == 0) ? BALL_SPEED : -BALL_SPEED;
            ball.velY = (rand() % 2 == 0) ? BALL_SPEED : -BALL_SPEED;
            scored = true;
        }
        else if (ball.x + ball.radius > SCREEN_WIDTH)
        {
            playerScore++;
            playerStreak++;
            aiStreak = 0; // Reset AI's streak

            // Determine if Player has a streak
            std::string streakMsg = getStreakMessage(playerStreak);
            if (!streakMsg.empty())
            {
                currentStreakMessage = streakMsg;
                streakMessageStartTime = SDL_GetTicks(); // Record the time
            }

            // Reset the ball to center with random direction
            ball.x = SCREEN_WIDTH / 2.0f;
            ball.y = SCREEN_HEIGHT / 2.0f;
            ball.velX = (rand() % 2 == 0) ? BALL_SPEED : -BALL_SPEED;
            ball.velY = (rand() % 2 == 0) ? BALL_SPEED : -BALL_SPEED;
            scored = true;
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
        SDL_RenderClear(renderer);

        // Draw paddles (fancy paddles with multiple rectangles for visual flair)
        SDL_Color paddleColor = {0, 255, 0, 255};            // Green
        SDL_Color paddleOutlineColor = {255, 255, 255, 255}; // White

        // Player Paddle
        SDL_Rect playerRect = {static_cast<int>(player.x), static_cast<int>(player.y), player.width, player.height};
        // Draw paddle body
        SDL_SetRenderDrawColor(renderer, paddleColor.r, paddleColor.g, paddleColor.b, paddleColor.a);
        SDL_RenderFillRect(renderer, &playerRect);
        // Draw paddle outline
        SDL_SetRenderDrawColor(renderer, paddleOutlineColor.r, paddleOutlineColor.g, paddleOutlineColor.b, paddleOutlineColor.a);
        SDL_RenderDrawRect(renderer, &playerRect);

        // AI Paddle
        SDL_Rect aiRect = {static_cast<int>(ai.x), static_cast<int>(ai.y), ai.width, ai.height};
        // Draw paddle body
        SDL_SetRenderDrawColor(renderer, paddleColor.r, paddleColor.g, paddleColor.b, paddleColor.a);
        SDL_RenderFillRect(renderer, &aiRect);
        // Draw paddle outline
        SDL_SetRenderDrawColor(renderer, paddleOutlineColor.r, paddleOutlineColor.g, paddleOutlineColor.b, paddleOutlineColor.a);
        SDL_RenderDrawRect(renderer, &aiRect);

        // Draw ball as a fireball (filled circle with particle effects)
        SDL_Color ballColor = {255, 69, 0, 255}; // OrangeRed
        drawFilledCircle(renderer, static_cast<int>(ball.x), static_cast<int>(ball.y), ball.radius, ballColor);

        // Render particles
        fireballParticles.render(renderer);

        // Render score
        std::string scoreText = std::to_string(playerScore) + " : " + std::to_string(aiScore);
        int scoreTextWidth, scoreTextHeight;
        // To center the score, we'll first render it to a surface to get its size
        SDL_Surface *scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
        if (scoreSurface != nullptr)
        {
            SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
            if (scoreTexture != nullptr)
            {
                scoreTextWidth = scoreSurface->w;
                scoreTextHeight = scoreSurface->h;
                SDL_Rect scoreRect = {(SCREEN_WIDTH / 2) - (scoreTextWidth / 2), 20, scoreTextWidth, scoreTextHeight};
                SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
                SDL_DestroyTexture(scoreTexture);
            }
            SDL_FreeSurface(scoreSurface);
        }

        // Render streak message if applicable
        Uint32 currentTime = SDL_GetTicks();
        if (!currentStreakMessage.empty())
        {
            if (currentTime - streakMessageStartTime <= STREAK_MESSAGE_DURATION)
            {
                // Render streak message
                SDL_Color streakColor = {255, 0, 0, 255}; // Red
                int streakTextWidth, streakTextHeight;
                SDL_Surface *streakSurface = TTF_RenderText_Solid(font, currentStreakMessage.c_str(), streakColor);
                if (streakSurface != nullptr)
                {
                    SDL_Texture *streakTexture = SDL_CreateTextureFromSurface(renderer, streakSurface);
                    if (streakTexture != nullptr)
                    {
                        streakTextWidth = streakSurface->w;
                        streakTextHeight = streakSurface->h;
                        SDL_Rect streakRect = {(SCREEN_WIDTH / 2) - (streakTextWidth / 2), 60, streakTextWidth, streakTextHeight};
                        SDL_RenderCopy(renderer, streakTexture, NULL, &streakRect);
                        SDL_DestroyTexture(streakTexture);
                    }
                    SDL_FreeSurface(streakSurface);
                }
            }
            else
            {
                // Streak message duration has elapsed
                currentStreakMessage = "";
            }
        }

        // Update screen
        SDL_RenderPresent(renderer);

        // Delay to control frame rate (approx. 60 FPS)
        SDL_Delay(16);
    }

    // Clean up and quit
    closeSDL(window, renderer, font);

    return 0;
}
