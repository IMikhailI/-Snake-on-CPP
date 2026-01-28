#include <cstring>
#include <iostream>

#include "gui/cli/snake_cli.h"
#include "gui/cli/tetris_cli.h"

#ifdef ENABLE_GUI
#include <QApplication>

#include "gui/desktop/main_menu_window.h"
#endif

int runInteractive() {
  while (true) {
    std::cout << "\n=== Выберите игру ===\n"
              << " 1) Tetris\n"
              << " 2) Snake\n"
              << " 3) Выход\n"
              << "> ";
    int choice = 0;
    if (!(std::cin >> choice)) return 0;
    switch (choice) {
      case 1:
        run_tetris_cli();
        break;
      case 2:
        run_snake_cli();
        break;
      case 3:
        return 0;
      default:
        std::cout << "Неверный выбор, попробуйте снова.\n";
        continue;
    }
  }
}

int main(int argc, char** argv) {
#ifdef ENABLE_GUI
  if (argc > 1 && std::strcmp(argv[1], "--gui") == 0) {
    QApplication app(argc, argv);
    MainMenuWindow menu;
    menu.show();
    return app.exec();
  }
#endif
  return runInteractive();
}
