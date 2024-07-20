#include "Camera.h"

Camera::Camera()
    : window(sf::VideoMode::getDesktopMode(), "The Great War", sf::Style::Fullscreen) {
    window.setFramerateLimit(0);
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorGrabbed(true);

    screenSize = window.getSize();
    offsetX = - (screenSize.x / 2.0f);
    offsetY = -(screenSize.y / 2.0f);
}

bool Camera::Update(InputState& state) {
    sf::Event event;
    while (window.pollEvent(event)) {}
    const InputState& inputState = state;

    if(inputState.isEscapePressed) return false; 

    clickPan(inputState);
    scrollPan(inputState);
    snapPan(inputState);

    return true;
}

void Camera::Zoom(sf::Event& event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        float worldXBeforeZoom, worldYBeforeZoom;
        ScreenToWorld(mousePos.x, mousePos.y, worldXBeforeZoom, worldYBeforeZoom);

        if (event.mouseWheelScroll.delta > 0) {
            scaleX *= 1.05f;
            scaleY *= 1.05f;
        }
        else if (event.mouseWheelScroll.delta < 0) {
            scaleX *= 0.95f;
            scaleY *= 0.95f;
        }

        bool scaleChanged = false;

        if (scaleX > 2.0f) scaleX = 2.0f;
        else if (scaleX < 0.5f) scaleX = 0.5f;
        else scaleChanged = true;

        if (scaleY > 2.0f) scaleY = 2.0f;
        else if (scaleY < 0.5f) scaleY = 0.5f;
        else scaleChanged = true;

        if (scaleChanged) {
            float worldXAfterZoom, worldYAfterZoom;
            ScreenToWorld(mousePos.x, mousePos.y, worldXAfterZoom, worldYAfterZoom);

            offsetX += worldXBeforeZoom - worldXAfterZoom;
            offsetY += worldYBeforeZoom - worldYAfterZoom;
        }
    }
}

void Camera::WorldToScreen(float worldX, float worldY, int& outScreenX, int& outScreenY) 
{
    outScreenX = (int)((worldX - offsetX) * scaleX);
    outScreenY = (int)((worldY - offsetY) * scaleY);
}

void Camera::ScreenToWorld(int screenX, int screenY, float& outWorldX, float& outWorldY)
{
    outWorldX = (((float)screenX / scaleX) + offsetX) + ((1920.f-screenSize.x)/2.f);
    outWorldY = (((float)screenY / scaleY) + offsetY) - ((1080.f - screenSize.y)/32.f);
}

void Camera::clickPan(const InputState& inputState) {
    sf::Vector2i mousePos = inputState.mousePosition;
    sf::Vector2u windowSize = window.getSize();

    if (inputState.isMiddleMouseButtonPressed) {
        if (!mouseButtonPanning) {
            mouseButtonPanning = true;
            startPanX = mousePos.x;
            startPanY = mousePos.y;
        }
        else {
            offsetX -= (mousePos.x - startPanX) / scaleX;
            offsetY -= (mousePos.y - startPanY) / scaleY;

            startPanX = mousePos.x;
            startPanY = mousePos.y;
        }
    }
    else {
        mouseButtonPanning = false;
    }
}

void Camera::scrollPan(const InputState& inputState) {
    const int edgeThreshold = 150;
    sf::Vector2i mousePos = inputState.mousePosition;
    sf::Vector2u windowSize = window.getSize();

    float panSpeedX = 0.0f;
    float panSpeedY = 0.0f;

    if (mousePos.x <= edgeThreshold)
        panSpeedX = (edgeThreshold - mousePos.x) / (static_cast<float>(edgeThreshold) * scaleX);
    else if (mousePos.x >= static_cast<int>(windowSize.x) - edgeThreshold)
        panSpeedX = -(mousePos.x - (static_cast<int>(windowSize.x) - edgeThreshold)) / (static_cast<float>(edgeThreshold) * scaleX);

    if (mousePos.y <= edgeThreshold)
        panSpeedY = (edgeThreshold - mousePos.y) / (static_cast<float>(edgeThreshold) * scaleY);
    else if (mousePos.y >= static_cast<int>(windowSize.y) - edgeThreshold)
        panSpeedY = -(mousePos.y - (static_cast<int>(windowSize.y) - edgeThreshold)) / (static_cast<float>(edgeThreshold) * scaleY);

    if (panSpeedX != 0.0f || panSpeedY != 0.0f) {
        offsetX -= panSpeedX * 10;
        offsetY -= panSpeedY * 10;
    }
}

void Camera::snapPan(const InputState& inputState)
{
    //Once there are scenery and units on the battlefield, snap panning should be done via hotkeys, snapping the camera to the position of a unit.
}

void Camera::Draw(std::vector<sf::Sprite>& sprites, const InputState& inputState)
{
    window.clear(sf::Color::White);
    int centerOffsetX = window.getSize().x / 2;

    for (auto& sprite : sprites)
    {
        auto position = sprite.getPosition();
        WorldToScreen(position.x + centerOffsetX, position.y, screenX, screenY);
        sprite.setPosition(static_cast<float>(screenX), static_cast<float>(screenY));
        window.draw(sprite);
    }

    ScreenToWorld(inputState.mousePosition.x - (window.getSize().x / 2) - 50, inputState.mousePosition.y - 100, worldX,
                  worldY);

    text1.setString("Mouse Grid (X: " + std::to_string(inputState.selectedCell.x) +
                    ", Y: " + std::to_string(inputState.selectedCell.y) + ")");
    window.draw(text1);

    window.display();
}