#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <bits/stdc++.h>

std::unordered_map<int, std::string> colorMap;

void changeSquareColor(SDL_Renderer* renderer, std::vector<SDL_Rect>& squares, int squareIndex, const std::string& colorString) {
    if (squareIndex >= 0 && squareIndex < squares.size()) {
        colorMap[squareIndex] = colorString;
        int r, g, b, a;
        sscanf(colorString.c_str(), "rgba(%d,%d,%d,%d)", &r, &g, &b, &a);

        SDL_Rect& square = squares[squareIndex];
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderFillRect(renderer, &square);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &square);
    }
}

int main(int argc, char* argv[]) {
    //-------------------------------------------------------------------Initialization-----------------------------------------------------------------
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    // Initialize SDL_ttf
    TTF_Init();

    // Load a font
    TTF_Font* font = TTF_OpenFont("/home/tinluyen/box_color/font/OpenSans-Light.ttf", 14);
    if (!font) {
        std::cout << "Failed to load font" << std::endl;
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 300, 600, SDL_WINDOW_SHOWN);

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // Set the background color of the window
    SDL_SetRenderDrawColor(renderer, 207, 235, 246, 255);
    SDL_RenderClear(renderer);

//--------------------------------------------------------------------------Draw first rectangle-------------------------------------------------
    SDL_Rect rect;
    rect.x = (300 - 280) / 2;  // Center the rectangle horizontally
    rect.y = (600 - 180) / 15;  // Center the rectangle vertically
    rect.w = 280;
    rect.h = 180;

    int cornerRadius = 10;  // Adjust the corner radius as desired

    SDL_SetRenderDrawColor(renderer, 238, 238, 238, 255);
    roundedBoxRGBA(renderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, cornerRadius, 238, 238, 238, 255);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_SetRenderDrawColor(renderer, 238, 238, 238, 255);
    roundedRectangleRGBA(renderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, cornerRadius, 0, 0, 0, 255);

    //-------------------------------------------------------------------------Draw squares in first rectangle-------------------------------------------------
    // Read the color data from the text file
    std::ifstream file("/home/tinluyen/box_color/database_color/colors4rectangle1.txt");
    if (!file) {
        std::cout << "Failed to open colors.txt" << std::endl;
        return 1;
    }

    std::vector<std::string> colorStrings;
    std::string colorString;
    while (std::getline(file, colorString)) {
        colorStrings.push_back(colorString);
    }
    file.close();

    if (colorStrings.size() != 15) {
        std::cout << "Invalid number of colors in colors.txt" << std::endl;
        return 1;
    }

    // Draw the squares
    SDL_Rect square;
    square.w = 43;
    square.h = 43;
    square.y = rect.y + 13;   // Adjust the y-coordinate for the squares

    int colorIndex = 0;

    // Draw 3 rows of 5 squares
    for (int row = 0; row < 3; row++) {
        square.y = rect.y + 13 + (row * 53);  // Adjust the y-coordinate for each row
        for (int col = 0; col < 5; col++) {
            square.x = rect.x + 13 + (col * 53);  // Adjust the x-coordinate for each square

            // Parse the color string from the file
            std::string colorString = colorStrings[colorIndex];
            int r, g, b, a;
            sscanf(colorString.c_str(), "rgba(%d,%d,%d,%d)", &r, &g, &b, &a);

            // Set the color of the square
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            SDL_RenderFillRect(renderer, &square);

            // Render the number in the middle of the square
            SDL_Color textColor = { 255, 255, 255 };
            std::string textToRender = "0";
            if (r == 253 && g == 136 && b == 81 && a == 255) {
                textToRender = "1";
            } else if (r == 153 && g == 153 && b == 153 && a == 255) {
                textToRender = "2";
            } else if (r == 255 && g == 191 && b == 40 && a == 255) {
                textToRender = "3";
            } else if (r == 255 && g == 191 && b == 41 && a == 255) {
                textToRender = "4";
            } else if (r == 76 && g == 133 && b == 217 && a == 255) {
                textToRender = "5";
            } else if (r == 66 && g == 193 && b == 129 && a == 255) {
                textToRender = "6";
            } else if (r == 138 && g == 207 && b == 87 && a == 255) {
                textToRender = "7";
            } else if (r == 0 && g == 174 && b == 238 && a == 255) {
                textToRender = "8";
            } else if (r == 180 && g == 180 && b == 180 && a == 255) {
                textToRender = "9";
            }
            SDL_Surface* surface = TTF_RenderText_Solid(font, textToRender.c_str(), textColor);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

            int textWidth = surface->w;
            int textHeight = surface->h;

            SDL_Rect textRect;
            textRect.x = square.x + (square.w - textWidth) / 2;
            textRect.y = square.y + (square.h - textHeight) / 2;
            textRect.w = textWidth;
            textRect.h = textHeight;

            SDL_RenderCopy(renderer, texture, nullptr, &textRect);

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);

            colorIndex++;
        }
    }

//------------------------------------------------------------Button apply-----------------------------------------------------------------------------
    // Define the dimensions and position of the button
    int buttonWidth = 250;
    int buttonHeight = 40;
    int buttonX = (300 - buttonWidth) / 2;  // Center the button horizontally
    int buttonY = (600 - buttonHeight) / 2.3;  // Center the button vertically

    // Create a rectangle that represents the button
    SDL_Rect buttonRect;
    buttonRect.x = buttonX;
    buttonRect.y = buttonY;
    buttonRect.w = buttonWidth;
    buttonRect.h = buttonHeight;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    roundedBoxRGBA(renderer, buttonRect.x, buttonRect.y, buttonRect.x + buttonRect.w, buttonRect.y + buttonRect.h, cornerRadius, 255, 255, 255, 255);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    roundedRectangleRGBA(renderer, buttonRect.x, buttonRect.y, buttonRect.x + buttonRect.w, buttonRect.y + buttonRect.h, cornerRadius, 0, 0, 0, 255);

    // Set the text color
    SDL_Color textColor2 = { 0, 0, 0 };  // Black color

    // Render the text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Apply", textColor2);
    if (!textSurface) {
        std::cout << "Failed to render text surface" << std::endl;
        return 1;
    }

    // Create a texture from the text surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // Get the dimensions of the text surface
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;

    // Calculate the position to center the text on the button
    int textX = buttonX + (buttonWidth - textWidth) / 2;
    int textY = buttonY + (buttonHeight - textHeight) / 2;

    // Set the destination rectangle for the text
    SDL_Rect textRect;
    textRect.x = textX;
    textRect.y = textY;
    textRect.w = textWidth;
    textRect.h = textHeight;

    // Render the text texture onto the button
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

//-----------------------------------------------------------------Button Ok-------------------------------------------------------------------------------
    // Define the dimensions and position of the button
    int buttonWidth2 = 70;
    int buttonHeight2 = 40;
    int buttonX2 = (300 - buttonWidth2) / 2 - 90;  // Center the button horizontally
    int buttonY2 = (600 - buttonHeight2) / 1.9;  // Center the button vertically

    // Create a rectangle that represents the button
    SDL_Rect buttonRect2;
    buttonRect2.x = buttonX2;
    buttonRect2.y = buttonY2;
    buttonRect2.w = buttonWidth2;
    buttonRect2.h = buttonHeight2;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    roundedBoxRGBA(renderer, buttonRect2.x, buttonRect2.y, buttonRect2.x + buttonRect2.w, buttonRect2.y + buttonRect2.h, cornerRadius, 255, 255, 255, 255);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    roundedRectangleRGBA(renderer, buttonRect2.x, buttonRect2.y, buttonRect2.x + buttonRect2.w, buttonRect2.y + buttonRect2.h, cornerRadius, 0, 0, 0, 255);

    // Render the text surface
    SDL_Surface* textSurface2 = TTF_RenderText_Solid(font, "OK", textColor2);
    if (!textSurface2) {
        std::cout << "Failed to render text surface" << std::endl;
        return 1;
    }

    // Create a texture from the text surface
    SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);

    // Get the dimensions of the text surface
    int textWidth2 = textSurface2->w;
    int textHeight2 = textSurface2->h;

    // Calculate the position to center the text on the button
    int textX2 = buttonX2 + (buttonWidth2 - textWidth2) / 2;
    int textY2 = buttonY2 + (buttonHeight2 - textHeight2) / 2;

    // Set the destination rectangle for the text
    SDL_Rect textRect2;
    textRect2.x = textX2;
    textRect2.y = textY2;
    textRect2.w = textWidth2;
    textRect2.h = textHeight2;

    // Render the text texture onto the button
    SDL_RenderCopy(renderer, textTexture2, nullptr, &textRect2);
//----------------------------------------------------------------------Button Undo---------------------------------------------------------------------------

    int buttonWidth3 = 70;
    int buttonHeight3 = 40;
    int buttonX3 = (300 - buttonWidth3) / 2;  // Center the button horizontally
    int buttonY3 = (600 - buttonHeight3) / 1.9;  // Center the button vertically

    // Create a rectangle that represents the button
    SDL_Rect buttonRect3;
    buttonRect3.x = buttonX3;
    buttonRect3.y = buttonY3;
    buttonRect3.w = buttonWidth3;
    buttonRect3.h = buttonHeight3;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    roundedBoxRGBA(renderer, buttonRect3.x, buttonRect3.y, buttonRect3.x + buttonRect3.w, buttonRect3.y + buttonRect3.h, cornerRadius, 255, 255, 255, 255);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    roundedRectangleRGBA(renderer, buttonRect3.x, buttonRect3.y, buttonRect3.x + buttonRect3.w, buttonRect3.y + buttonRect3.h, cornerRadius, 0, 0, 0, 255);

    // Render the text surface
    SDL_Surface* textSurface3 = TTF_RenderText_Solid(font, "Undo", textColor2);
    if (!textSurface3) {
        std::cout << "Failed to render text surface" << std::endl;
        return 1;
    }

    // Create a texture from the text surface
    SDL_Texture* textTexture3 = SDL_CreateTextureFromSurface(renderer, textSurface3);

    // Get the dimensions of the text surface
    int textWidth3 = textSurface3->w;
    int textHeight3 = textSurface3->h;

    // Calculate the position to center the text on the button
    int textX3 = buttonX3 + (buttonWidth3 - textWidth3) / 2;
    int textY3 = buttonY3 + (buttonHeight3 - textHeight3) / 2;

    // Set the destination rectangle for the text
    SDL_Rect textRect3;
    textRect3.x = textX3;
    textRect3.y = textY3;
    textRect3.w = textWidth3;
    textRect3.h = textHeight3;

    // Render the text texture onto the button
    SDL_RenderCopy(renderer, textTexture3, nullptr, &textRect3);

//------------------------------------------------------------------Button Cancel--------------------------------------------------------------------

    // Define the dimensions and position of the button
    int buttonWidth4 = 70;
    int buttonHeight4 = 40;
    int buttonX4 = (300 - buttonWidth4) / 2 + 90;  // Center the button horizontally
    int buttonY4 = (600 - buttonHeight4) / 1.9;  // Center the button vertically

    // Create a rectangle that represents the button
    SDL_Rect buttonRect4;
    buttonRect4.x = buttonX4;
    buttonRect4.y = buttonY4;
    buttonRect4.w = buttonWidth4;
    buttonRect4.h = buttonHeight4;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    roundedBoxRGBA(renderer, buttonRect4.x, buttonRect4.y, buttonRect4.x + buttonRect4.w, buttonRect4.y + buttonRect4.h, cornerRadius, 255, 255, 255, 255);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    roundedRectangleRGBA(renderer, buttonRect4.x, buttonRect4.y, buttonRect4.x + buttonRect4.w, buttonRect4.y + buttonRect4.h, cornerRadius, 0, 0, 0, 255);


    // Render the text surface
    SDL_Surface* textSurface4 = TTF_RenderText_Solid(font, "Cancel", textColor2);
    if (!textSurface4) {
        std::cout << "Failed to render text surface" << std::endl;
        return 1;
    }

    // Create a texture from the text surface
    SDL_Texture* textTexture4 = SDL_CreateTextureFromSurface(renderer, textSurface4);

    // Get the dimensions of the text surface
    int textWidth4 = textSurface4->w;
    int textHeight4 = textSurface4->h;

    // Calculate the position to center the text on the button
    int textX4 = buttonX4 + (buttonWidth4 - textWidth4) / 2;
    int textY4 = buttonY4 + (buttonHeight4 - textHeight4) / 2;

    // Set the destination rectangle for the text
    SDL_Rect textRect4;
    textRect4.x = textX4;
    textRect4.y = textY4;
    textRect4.w = textWidth4;
    textRect4.h = textHeight4;

    // Render the text texture onto the button
    SDL_RenderCopy(renderer, textTexture4, nullptr, &textRect4);

 //--------------------------------------------------------------------------draw rectangle 2-----------------------------------------------------------------------------
    SDL_Rect rect3;
    rect3.x = (300 - 280) / 2;  // Center the rectangle horizontally
    rect3.y = (600 - 250);  // Center the rectangle vertically
    rect3.w = 280;
    rect3.h = 100;
    SDL_SetRenderDrawColor(renderer, 238, 238, 238, 255);
    SDL_RenderFillRect(renderer, &rect3);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect3);


//-----------------------------------------------------------------draw double small rectangle 1-------------------------------------------------------------------
    // Draw the smaller rectangle inside the larger rectangle
    SDL_Rect smallRect;
    smallRect.x = rect3.x + (rect3.w - 95);  // Center the smaller rectangle horizontally
    smallRect.y = rect3.y + (rect3.h - 90);   // Center the smaller rectangle vertically
    smallRect.w = 80;
    smallRect.h = 20;

    // Set the fill color to a different color (e.g., blue)
    SDL_SetRenderDrawColor(renderer, 238, 238, 238, 255);
    SDL_RenderFillRect(renderer, &smallRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &smallRect);

    SDL_Rect smallRect2;
    smallRect2.x = rect3.x + (rect3.w - 265);  // Center the smaller rectangle horizontally
    smallRect2.y = rect3.y + (rect3.h - 90);   // Center the smaller rectangle vertically
    smallRect2.w = 150;
    smallRect2.h = 20;

    // Set the fill color to a different color (e.g., blue)
    SDL_SetRenderDrawColor(renderer, 238, 238, 238, 255);
    SDL_RenderFillRect(renderer, &smallRect2);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &smallRect2);

    // Load the image surface from a file
    SDL_Surface* imageSurface = IMG_Load("/home/tinluyen/box_color/images/triangle.png");
    if (imageSurface == nullptr) {
        // Handle error if the image fails to load
        printf("Unable to load image. SDL_image Error: %s\n", IMG_GetError());
    }

    // Create a texture from the image surface
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if (imageTexture == nullptr) {
        // Handle error if the texture fails to create
        printf("Unable to create texture from surface. SDL Error: %s\n", SDL_GetError());
    }

    // Set the destination rectangle for the image
    SDL_Rect destinationRect;
    destinationRect.x = smallRect.x;
    destinationRect.y = smallRect.y;
    destinationRect.w = smallRect.w/5;
    destinationRect.h = smallRect.h/2;

    // Copy the texture onto the renderer at the destination rectangle
    SDL_RenderCopy(renderer, imageTexture, nullptr, &destinationRect);

    // Load the image surface from a file
    SDL_Surface* imageSurface2 = IMG_Load("/home/tinluyen/box_color/images/triangle2.png");
    if (imageSurface2 == nullptr) {
        // Handle error if the image fails to load
        printf("Unable to load image. SDL_image Error: %s\n", IMG_GetError());
    }

    // Create a texture from the image surface
    SDL_Texture* imageTexture2 = SDL_CreateTextureFromSurface(renderer, imageSurface2);
    if (imageTexture2 == nullptr) {
        // Handle error if the texture fails to create
        printf("Unable to create texture from surface. SDL Error: %s\n", SDL_GetError());
    }

    // Set the destination rectangle for the image
    SDL_Rect destinationRect2;
    destinationRect2.x = smallRect.x;
    destinationRect2.y = smallRect.y + 10;
    destinationRect2.w = smallRect.w/5;
    destinationRect2.h = smallRect.h/2;

    // Copy the texture onto the renderer at the destination rectangle
    SDL_RenderCopy(renderer, imageTexture2, nullptr, &destinationRect2);
//-----------------------------------------------------------------draw double small rectangle 2-------------------------------------------------------------------
    SDL_Rect smallRect3;
    smallRect3.x = rect3.x + (rect3.w - 95);  // Center the smaller rectangle horizontally
    smallRect3.y = rect3.y + (rect3.h - 63);   // Center the smaller rectangle vertically
    smallRect3.w = 80;
    smallRect3.h = 20;

    // Set the fill color to a different color (e.g., blue)
    SDL_SetRenderDrawColor(renderer, 238, 238, 238, 255);
    SDL_RenderFillRect(renderer, &smallRect3);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &smallRect3);

    SDL_Rect smallRect4;
    smallRect4.x = rect3.x + (rect3.w - 265);  // Center the smaller rectangle horizontally
    smallRect4.y = rect3.y + (rect3.h - 63);   // Center the smaller rectangle vertically
    smallRect4.w = 150;
    smallRect4.h = 20;

    // Set the fill color to a different color (e.g., blue)
    SDL_SetRenderDrawColor(renderer, 238, 238, 238, 255);
    SDL_RenderFillRect(renderer, &smallRect4);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &smallRect4);

    // // Set the destination rectangle for the image
    SDL_Rect destinationRect3;
    destinationRect3.x = smallRect3.x;
    destinationRect3.y = smallRect3.y;
    destinationRect3.w = smallRect3.w/5;
    destinationRect3.h = smallRect3.h/2;

    // // Copy the texture onto the renderer at the destination rectangle
    SDL_RenderCopy(renderer, imageTexture, nullptr, &destinationRect3);


    // Set the destination rectangle for the image
    SDL_Rect destinationRect4;
    destinationRect4.x = smallRect3.x;
    destinationRect4.y = smallRect3.y + 10;
    destinationRect4.w = smallRect3.w/5;
    destinationRect4.h = smallRect3.h/2;

    // // Copy the texture onto the renderer at the destination rectangle
    SDL_RenderCopy(renderer, imageTexture2, nullptr, &destinationRect4);
//-----------------------------------------------------------------draw double small rectangle 3-------------------------------------------------------------------

    SDL_Rect smallRect5;
    smallRect5.x = rect3.x + (rect3.w - 95);  // Center the smaller rectangle horizontally
    smallRect5.y = rect3.y + (rect3.h - 37);   // Center the smaller rectangle vertically
    smallRect5.w = 80;
    smallRect5.h = 20;

    // Set the fill color to a different color (e.g., blue)
    SDL_SetRenderDrawColor(renderer, 238, 238, 238, 255);
    SDL_RenderFillRect(renderer, &smallRect5);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &smallRect5);

    SDL_Rect smallRect6;
    smallRect6.x = rect3.x + (rect3.w - 265);  // Center the smaller rectangle horizontally
    smallRect6.y = rect3.y + (rect3.h - 37);   // Center the smaller rectangle vertically
    smallRect6.w = 150;
    smallRect6.h = 20;

    // Set the fill color to a different color (e.g., blue)
    SDL_SetRenderDrawColor(renderer, 238, 238, 238, 255);
    SDL_RenderFillRect(renderer, &smallRect6);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &smallRect6);

    SDL_Rect destinationRect5;
    destinationRect5.x = smallRect5.x;
    destinationRect5.y = smallRect5.y;
    destinationRect5.w = smallRect5.w/5;
    destinationRect5.h = smallRect5.h/2;

    // // Copy the texture onto the renderer at the destination rectangle
    SDL_RenderCopy(renderer, imageTexture, nullptr, &destinationRect5);


    // Set the destination rectangle for the image
    SDL_Rect destinationRect6;
    destinationRect6.x = smallRect5.x;
    destinationRect6.y = smallRect5.y + 10;
    destinationRect6.w = smallRect5.w/5;
    destinationRect6.h = smallRect5.h/2;

    // // Copy the texture onto the renderer at the destination rectangle
    SDL_RenderCopy(renderer, imageTexture2, nullptr, &destinationRect6);

//-------------------------------------------------------------------------draw rectangle 3--------------------------------------------------------------------------
   // Draw the rectangle
    SDL_Rect rect2;
    rect2.x = (300 - 280) / 2;  // Center the rectangle horizontally
    rect2.y = (600 - 100);      // Center the rectangle vertically
    rect2.w = 280;
    rect2.h = 90;

    SDL_SetRenderDrawColor(renderer, 238, 238, 238, 255);
    SDL_RenderFillRect(renderer, &rect2);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect2);

    // Read the color data from the text file
    std::ifstream file2("/home/tinluyen/box_color/database_color/colors4rectangle3.txt");
    if (!file2) {
        std::cout << "Failed to open colors.txt" << std::endl;
        return 1;
    }

    std::vector<std::string> colorStrings2;
    std::string colorString2;
    while (std::getline(file2, colorString2)) {
        colorStrings2.push_back(colorString2);
    }

    file2.close();

    if (colorStrings2.size() != 16) {
        std::cout << "Invalid number of colors in colors.txt" << std::endl;
        return 1;
    }

    // Draw the squares
    std::vector<SDL_Rect> squares(16);

    // Set the width and height of each square
    int squareWidth = 23;
    int squareHeight = 33;

    // Set the initial position of the first square
    int initialX = rect2.x + 15;
    int initialY = rect2.y + 8;

    // Initialize the positions of the squares
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 8; col++) {
            int index = row * 8 + col;

            squares[index].x = initialX + col * (squareWidth + 8);
            squares[index].y = initialY + row * (squareHeight + 8);
            squares[index].w = squareWidth;
            squares[index].h = squareHeight;
        }
    }

    int colorIndex2 = 0;

    // Draw the squares
    for (int i = 0; i < 16; i++) {
        // Parse the color string from the file
        std::string colorString2 = colorStrings2[colorIndex2];
        int r, g, b, a;
        sscanf(colorString2.c_str(), "rgba(%d,%d,%d,%d)", &r, &g, &b, &a);

        // Set the color of the square
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderFillRect(renderer, &squares[i]);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &squares[i]);

        colorIndex2++;
    }

//--------------------------------------------------------------------------Update window----------------------------------------------------------------------
    // Update the window
    SDL_RenderPresent(renderer);

    // Wait for the window to be closed
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        int index;
        std::string c_color;
        std::cout << "Please choose index: "; std::cin >> index;
        std::cout << "Please choose color: "; std::cin >> c_color;

        // Only update color if index valid
        if (index >= 0 && index < squares.size()) {
            changeSquareColor(renderer, squares, index, c_color); // Change color for square at index
        }

        // Re-draw square with new color 
        for (int i = 0; i < squares.size(); i++) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &squares[i]);
        }

        // Update window
        SDL_RenderPresent(renderer);
    }

    // Cleanup and quit
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}