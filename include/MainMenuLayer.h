#include "GameLayer.h"
#include "Gui/MainMenu.h"

class MainMenuLayer : public GameLayer {
    private:
        MainMenu m_mainMenu;

    public:
        MainMenuLayer();
        ~MainMenuLayer();

        void update(int dt) override;
};
