#include <QApplication>

#include "ui/board.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Board board;
    board.setWindowTitle("chess-bot");
    board.resize(BOARD_SIZE, BOARD_SIZE);
    board.setFixedSize(board.size());
    board.show();

    return app.exec();
}
