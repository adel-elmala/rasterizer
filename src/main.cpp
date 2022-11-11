#include "rasterizer.h"

bool window_should_close = false;
int nPixelsx = 640;
int nPixelsy = 480;
const double PI = 3.14159265359;

void rasterSquare(SDL_Surface *screen);
void processEvents();

int main(int argc, char *argv[])
{

    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Rasterizer",            // window title
        SDL_WINDOWPOS_UNDEFINED, // initial x position
        SDL_WINDOWPOS_UNDEFINED, // initial y position
        nPixelsx,                // width, in pixels
        nPixelsy,                // height, in pixels
        SDL_WINDOW_OPENGL        // flags - see below
    );

    if (window == NULL)
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // // but instead of creating a renderer, we can draw directly to the screen
    SDL_Surface *screen = SDL_GetWindowSurface(window);

    // objParser parser("./objFiles/teapot.obj");
    objParser parser("./objFiles/cow.obj");
    fprintf(stdout, "OBJ PARSER: done\n");

    char winTitle[512];
    winTitle[511] = '\0';
    init_Model_to_screen_mat();

    initZBuffer(nPixelsx, nPixelsy);
    while (!window_should_close)
    {
        // clear screen
        SDL_FillRect(screen, NULL, 0x000000);
        uint32_t sTime = SDL_GetTicks();
        // clear z-buffer
        clearZBuffer(nPixelsx, nPixelsy);
        // update state, draw the current frame
        processEvents();

        for (const auto t : parser.triangles)
        {
            // rasterTriangle(screen,t);
            // rasterWireFrameTriangle(screen, t);
            rasterTriangle(screen, t);
        }
        // rasterSquare(screen);

        SDL_UpdateWindowSurface(window);

        // update window title to current FPS
        uint32_t frameTime = SDL_GetTicks() - sTime;
        float fps = frameTime > 0 ? (1000.0f / frameTime) : 0;
        snprintf(winTitle, 512, "FPS: %.1f .", fps);
        SDL_SetWindowTitle(window, winTitle);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void rasterSquare(SDL_Surface *screen)
{
    Vector3 tp1 = Vector3(1.0, 1.0, 1.0);
    Vector3 tp2 = Vector3(1.0, -1.0, 1.0);
    Vector3 tp3 = Vector3(-1.0, -1.0, 1.0);
    triangle t;
    t.v1.pos = tp1;
    t.v2.pos = tp2;
    t.v3.pos = tp3;
    t.v1.col = RGBColor(1.0, 0.0, 0.0);
    t.v2.col = RGBColor(1.0, 0.0, 0.0);
    t.v3.col = RGBColor(1.0, 0.0, 0.0);


    Vector3 t2p1 = Vector3(1.0, 2.0, 0.0);
    Vector3 t2p2 = Vector3(1.0, 0.0, 0.0);
    Vector3 t2p3 = Vector3(-1.0, 0.0, 0.0);
    
    // Vector3 t2p1 = Vector3(1.0, 1.0, 1.0);
    // Vector3 t2p2 = Vector3(-1.0, -1.0, 1.0);
    // Vector3 t2p3 = Vector3(-1.0, 1.0, 1.0);
    triangle t2;
    t2.v1.pos = t2p1;
    t2.v2.pos = t2p2;
    t2.v3.pos = t2p3;
    t2.v1.col = RGBColor(0.0, 1.0, 0.0);
    t2.v2.col = RGBColor(0.0, 1.0, 0.0);
    t2.v3.col = RGBColor(0.0, 1.0, 0.0);
    // rasterWireFrameTriangle(screen, t);
    // rasterWireFrameTriangle(screen, t2);
    rasterTriangle(screen, t2);
    rasterTriangle(screen, t);
}

void processEvents()
{
    static double rotate_deg = 0.0;
    static double uniScale = 1.0;
    static Vector3 eye(0.0);
    static double gazeDirTheta = PI / 2.0;
    static Vector3 gazeDir(cos(gazeDirTheta), 0, -sin(gazeDirTheta));
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
        {
            window_should_close = true;
            break;
        }
        case SDL_KEYDOWN:
        {
            // fprintf(stdout, "keyboard\n");
            /* Check the SDLKey values and move change the coords */
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                window_should_close = true;
                break;

            case SDLK_RIGHT:
            {
                // fprintf(stdout, "pressed\n");
                rotate_deg += PI / 25;
                constructWorldMat(Mw, Vector3(uniScale), Vector3(0.0, 0.0, -120), rotate_deg);
                update_Model_to_screen_mat();
                break;
            }
            case SDLK_LEFT:
            {
                // fprintf(stdout, "pressed\n");
                rotate_deg -= PI / 25;
                constructWorldMat(Mw, Vector3(uniScale), Vector3(0.0, 0.0, -120), rotate_deg);
                update_Model_to_screen_mat();
                break;
            }
            case SDLK_UP:
            {
                uniScale += 0.2;
                constructWorldMat(Mw, Vector3(uniScale), Vector3(0.0, 0.0, -120), rotate_deg);
                update_Model_to_screen_mat();
                break;
            }
            case SDLK_DOWN:
            {
                uniScale -= 0.2;
                constructWorldMat(Mw, Vector3(uniScale), Vector3(0.0, 0.0, -120), rotate_deg);
                update_Model_to_screen_mat();
                break;
            }
            // case SDLK_w:
            // {
            //     eye.m_z += 0.25;
            //     constructCamMat(Mc, eye, gazeDir, Vector3(0.0, 1.0, 0.0));
            //     update_Model_to_screen_mat();
            //     break;
            // }
            // case SDLK_s:
            // {
            //     eye.m_z -= 0.25;
            //     constructCamMat(Mc, eye, gazeDir, Vector3(0.0, 1.0, 0.0));
            //     update_Model_to_screen_mat();
            //     break;
            // }
            // case SDLK_a:
            // {
            //     gazeDirTheta -= PI / 100;
            //     gazeDir = Vector3(cos(gazeDirTheta), 0, -sin(gazeDirTheta));
            //     constructCamMat(Mc, eye, gazeDir, Vector3(0.0, 1.0, 0.0));
            //     update_Model_to_screen_mat();
            //     break;
            // }
            // case SDLK_d:
            // {
            //     gazeDirTheta += PI / 100;
            //     gazeDir = Vector3(cos(gazeDirTheta), 0, -sin(gazeDirTheta));
            //     constructCamMat(Mc, eye, gazeDir, Vector3(0.0, 1.0, 0.0));
            //     update_Model_to_screen_mat();
            //     break;
            // }
            default:
                break;
            }
        }
        }
    }
}
