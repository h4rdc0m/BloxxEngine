/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V - All rights reserved
 */

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;

/**
 * This function runs once at startup.
 * @param appstate The application state
 * @param argc The number of arguments on the command line
 * @param argv The values of the arguments on the command line
 * @return App result of the initialization
 */
SDL_AppResult SDL_AppInit(void** appState, int argc, char** argv)
{
    SDL_SetAppMetadata("BloxxEngine Sandbox", "0.0.1", "nl.combat.bloxxengine-sandbox");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("BloxxEngine Sandbox", 800, 600, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_Log("SDL_CreateWindowAndRenderer failed: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/**
 * This function runs when a SDL event occurs.
 * @param appState The application state
 * @param event The SDL event being handled
 * @return App result
 */
SDL_AppResult SDL_AppEvent(void* appState, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

/**
 * This function runs once per frame, and is the heart of the program.
 * @param appState The application state
 * @return The app result
 */
SDL_AppResult SDL_AppIterate(void* appState)
{
    const double now = static_cast<double>(SDL_GetTicks()) / 1000.0;
    /* Choose the color for the frame we will draw. The sine wave trick makes it fade between colors smoothly. */
    const auto red = static_cast<float>(0.5 + 0.5 * SDL_sin(now));
    const auto green = static_cast<float>(0.5 + 0.5 * SDL_cos(now + SDL_PI_D * 2 / 3));
    const auto blue = static_cast<float>(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));

    SDL_SetRenderDrawColorFloat(renderer, red, green, blue, 1.0f);

    /* Clear the window to the draw color */
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

/**
 * This function is called once on application exit.
 * @param appState The app state
 * @param result The app result the application exited on
 */
void SDL_AppQuit(void* appState, SDL_AppResult result)
{
    /** SDL will clean up the window and renderer for us. */
}