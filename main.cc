#include "quez/game_engine.h"
#include "quez/custom_game_state.h"

int main() {
    GameEngine engine;
    engine.init("Quez", 520, 620, false, WindowFlag::WINDOWED, 0, 1);
    engine.setDebugFPS(false);

    engine.changeState(CustomGameState::instance());

    while (engine.isRunning()) {
        engine.updateFPS();
        engine.handleEvent();
        engine.update();
        engine.draw();
        engine.lateFPS();
    }

    engine.cleanup();

    return 0;
}